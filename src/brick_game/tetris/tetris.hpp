#ifndef BRICKGAME_BRICK_GAME_TETRIS_TETRIS_HPP
#define BRICKGAME_BRICK_GAME_TETRIS_TETRIS_HPP

#include <random>
#include <algorithm>

#include "game.hpp"
#include "game_info.hpp"

namespace s21 {
class Tetris : public Game {
private:
    using Figure = std::vector<std::vector<int>>;

public:
    Tetris() {
        std::random_device rd;
        rand_gen_.seed(rd());

        GenerateFigure();
        CopyFigure();
        GenerateFigure();
    }

    ~Tetris() = default;

public:
    GameInfo_t GetGameInfo() const override {
        return game_info_;
    }

public:
    void Start() override {
        game_info_.pause = false;
    }

    void Stop() noexcept override {
        game_info_.pause = true;
    }

    void ResetState() override {
        game_info_.Reset();
        GenerateFigure();
        CopyFigure();
        GenerateFigure();
    }

    void Move(Direction direct) override {
        if (!game_info_.pause)
            MakeTransformations(direct);
    }

    void UpdateState() override {
        if (!game_info_.pause) {
            if (!MoveFigureDown()) {
                RemoveLine();

                if (!CopyFigure()) {
                    game_info_.game_over = true;
                } else {
                    GenerateFigure();
                }
            }
        }
    }

private:
    void MakeTransformations(Direction direct) {
        switch (direct) {
            case Direction::kLeft:
            case Direction::kRight:
                Shift(direct);
                break;
            case Direction::kUp:
                Rotate();
                break;
            case Direction::kDown:
                while (MoveFigureDown());
                break;
        }
    }

    void GenerateFigure() {
        std::uniform_int_distribution<> dist(0, figures_.size() - 1);

        game_info_.next.type = dist(rand_gen_);
        game_info_.next.figure = figures_[game_info_.next.type];

        int index{};

        for (int i{}; i < GameInfo_t::figure_rows; ++i) {
            for (int j{}; j < GameInfo_t::figure_cols; ++j) {
                if (game_info_.next.figure[i][j] == 1) {
                    auto& [row, col]{game_info_.next.indices[index]};

                    row = i;
                    col = j + 3;

                    ++index;
                }
            }
        }
    }

    bool CopyFigure() {
        for (auto [row, col] : game_info_.next.indices)
            if (game_info_.field[row][col] == 1)
                return false;

        game_info_.current.type = game_info_.next.type;
        game_info_.current.indices = game_info_.next.indices;

        for (auto [row, col] : game_info_.current.indices)
            game_info_.field[row][col] = 1;

        return true;
    }

    bool MoveFigureDown() {
        for (auto [row, col] : game_info_.current.indices) {
            bool is_correct_move{
                row == GameInfo_t::field_rows - 1 ||
                (
                    game_info_.field[row + 1][col] == 1 &&
                    !IsAvailableIndex(row + 1, col)
                )
            };

            if (is_correct_move)
                return false;
        }

        ClearPastPositions();

        for (auto& [row, col] : game_info_.current.indices)
            game_info_.field[++row][col] = 1;

        return true;
    }

    bool IsAvailableIndex(int r, int c) {
        for (auto [row, col] : game_info_.current.indices)
            if (row == r && col == c)
                return true;

        return false;
    }

    void ClearPastPositions() {
        for (auto [row, col] : game_info_.current.indices)
            game_info_.field[row][col] = 0;
    }

    void RemoveLine() {
        int count{};

        auto it{game_info_.field.end() - 1};

        do {
            if (IsLineFill(*it)) {
                std::fill(it->begin(), it->end(), 0);

                ++count;

                MoveFieldDown(it);

                ++it;
            }
            --it;
        } while (it != game_info_.field.begin());

        if (count != 0)
            ScoreUp(count);
    }

    bool IsLineFill(const std::vector<int>& line) {
        for (auto num : line)
            if (num == 0)
                return false;

        return true;
    }

    void MoveFieldDown(std::vector<std::vector<int>>::iterator current_line) {
        auto prev_line{current_line - 1};

        do {
            std::copy(prev_line->begin(), prev_line->end(), (prev_line + 1)->begin());
            --prev_line;
        } while (prev_line != game_info_.field.begin());
    }

    void ScoreUp(int count) {
        switch (count) {
            case 1:
                game_info_.score += 100;
                break;
            case 2:
                game_info_.score += 300;
                break;
            case 3:
                game_info_.score += 700;
                break;
            case 4:
                game_info_.score += 1500;
                break;
        }

        LevelUp();
    }

    void LevelUp() {
        game_info_.level = game_info_.score / 600 + 1;
        game_info_.speed = 300 - (game_info_.level * 10);
    }

    void Shift(Direction direct) {
        int direction{1};

        if (direct == Direction::kLeft)
            direction = -1;

        if (IsShiftAvailable(direct)) {
            ClearPastPositions();

            for (auto& [row, col] : game_info_.current.indices) {
                col += direction;
                game_info_.field[row][col] = 1;
            }
        }
    }

    bool IsShiftAvailable(Direction direct) {
        int last_col{};
        int direction{-1};

        if (direct == Direction::kRight) {
            last_col = GameInfo_t::field_cols - 1;
            direction = 1;
        }

        for (auto [row, col] : game_info_.current.indices) {
            bool is_correct_move{
                col == last_col ||
                (
                    game_info_.field[row][col + direction] == 1 &&
                    !IsAvailableIndex(row, col + direction)
                )
            };

            if (is_correct_move)
                return false;
        }

        return true;
    }

    void Rotate() {
        std::vector<Coords> new_indices(4, Coords(0, 0));
        auto [center_r, center_c]{game_info_.current.indices[2]};
        bool can_rotate = true;
        int index{};

        ClearPastPositions();

        for (auto [rel_row, rel_col] : game_info_.current.indices) {
            rel_row -= center_r;
            rel_col -= center_c;

            auto& [new_r, new_c]{new_indices[index]};

            new_r = center_r - rel_col;
            new_c = center_c + rel_row;

            if (new_r < 0 || new_r >= GameInfo_t::field_rows ||
                new_c < 0 || new_c >= GameInfo_t::field_cols ||
                game_info_.field[new_r][new_c] == 1) {
                can_rotate = false;
                break;
            }

            ++index;
        }

        if (can_rotate && game_info_.current.type != 3) {
            index = 0;

            for (auto& [row, col] : game_info_.current.indices) {
                auto [new_row, new_col]{new_indices[index]};
                row = new_row;
                col = new_col;
                ++index;
            }
        }

        for (auto [row, col] : game_info_.current.indices) {
            game_info_.field[row][col] = 1;
        }
    }

private:
    GameInfo_t game_info_;

    std::mt19937 rand_gen_;

    std::vector<std::vector<std::vector<int>>> figures_{
        {
            {1, 1, 1, 1},
            {0, 0, 0, 0}
        },
        {
            {1, 0, 0, 0},
            {1, 1, 1, 0}
        },
        {
            {0, 0, 1, 0},
            {1, 1, 1, 0}
        },
        {
            {1, 1, 0, 0},
            {1, 1, 0, 0}
        },
        {
            {0, 1, 1, 0},
            {1, 1, 0, 0}
        },
        {
            {1, 1, 0, 0},
            {0, 1, 1, 0}
        },
        {
            {0, 1, 0, 0},
            {1, 1, 1, 0}
        }
    };
};
} // namespace s21

#endif // BRICKGAME_BRICK_GAME_TETRIS_TETRIS_HPP
