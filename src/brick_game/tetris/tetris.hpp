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
    Tetris() : current_indices_(4, Coords(0, 0)) {
        std::random_device rd;
        rand_gen_.seed(rd());

        GenerateFigure();
    }

    ~Tetris() = default;

public:
    GameInfo_t GetGameInfo() const override {
        return game_info_;
    }

    void SetWalls(bool) noexcept override {}

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
    }

    void Move(Direction direct) override {
        if (!game_info_.pause)
            MakeTransformations(direct);
    }

    void UpdateState() override {
        if (!game_info_.pause) {
            if (!MoveFigureDown()) {
                if (!GenerateFigure())
                    game_info_.game_over = true;

//                RemoveLine();
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
                break;
        }
    }

    bool GenerateFigure() {
        std::uniform_int_distribution<> dist(0, figures_.size() - 1);

        int type{dist(rand_gen_)};

        game_info_.next = figures_[type];

        int index{};

        for (int i{}; i < GameInfo_t::figure_rows; ++i) {
            for (int j{}; j < GameInfo_t::figure_cols; ++j) {
                if (game_info_.next[i][j] == 1) {
                    auto& [row, col]{current_indices_[index]};

                    row = i;
                    col = j + 3;

                    if (game_info_.field[row][col] == 1)
                        return false;

                    ++index;
                }
            }
        }

        return true;
    }

    bool MoveFigureDown() {
        for (auto [row, col] : current_indices_) {
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

        for (auto& [row, col] : current_indices_)
            game_info_.field[++row][col] = 1;

        return true;
    }

    bool IsAvailableIndex(int r, int c) {
        for (auto [row, col] : current_indices_)
            if (row == r && col == c)
                return true;

        return false;
    }

    void ClearPastPositions() {
        for (auto [row, col] : current_indices_)
            game_info_.field[row][col] = 0;
    }

    void RemoveLine() {
        int count{};

        for (auto it{game_info_.field.end() - 1}; ; --it) {
            if (IsLineFill(*it)) {
                std::fill(it->begin(), it->end(), 0);

                ++count;

                MoveFieldDown(it);

                if (it == game_info_.field.begin())
                    break;
            }
        }

        if (count != 0)
            ScoreUp(count);
    }

    bool IsLineFill(const std::vector<int>& line) {
        return std::all_of(line.begin(), line.end(), [](int num) {
            return num == 1;
        });
    }


    void MoveFieldDown(std::vector<std::vector<int>>::iterator line_it) {
        for (auto it{line_it - 1}; ; --it) {
            std::copy(it->begin(), it->end(), (it + 1)->begin());

            if (it == game_info_.field.begin())
                break;
        }
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

            for (auto& [row, col] : current_indices_) {
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

        for (auto [row, col] : current_indices_) {
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
        auto [center_r, center_c]{current_indices_[1]};

        int index{};

        for (auto [rel_row, rel_col] : current_indices_) {
            rel_row -= center_r;
            rel_col -= center_c;

            auto& [new_r, new_c]{new_indices[index]};

            new_r = center_r - rel_col;
            new_c = center_c + rel_row;

            if (new_r < 0 || new_r >= GameInfo_t::field_rows ||
                new_c < 0 || new_c >= GameInfo_t::field_cols ||
                game_info_.field[new_r][new_c] == 1) {
                return;
            }

            ++index;
        }

        ClearPastPositions();

        for (std::size_t i{}; i < current_indices_.size(); ++i) {
            current_indices_[i].first = new_indices[i].first;
            current_indices_[i].second = new_indices[i].second;
        }

        for (auto [row, col] : current_indices_)
            game_info_.field[row][col] = 1;
    }

private:
    std::mt19937 rand_gen_;
    GameInfo_t game_info_;

    std::vector<Coords> current_indices_;

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
