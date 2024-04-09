#ifndef BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP
#define BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP

#include <algorithm>
#include <fstream>
#include <random>
#include <vector>

#include "game.hpp"

namespace s21 {
class Snake : public Game {
public:
    enum class Mode : bool {
        kWall,
        kNoWall
    };

public:
    Snake(Mode mode) {
        is_walls_ = mode == Mode::kWall;

        std::random_device rd;
        rand_gen_.seed(rd());

        Reset();

        GetHighScore();
    }

public:
    GameInfo_t GetGameInfo() const override {
        return game_info_;
    }

public:
    void Stop() noexcept override {
        game_info_.pause = true;
    }

    void ResetState() override {
        Reset();
    }

    void SigAct(State state, Direction direct) override {
        while (true) {
            switch (state) {
            case State::kSTART:
                GetHighScore();
                game_info_.pause = false;
                state = State::kSPAWN;
                break;
            case State::kSPAWN:
                MergeField();
                state = State::kMOVING;
                break;
            case State::kMOVING:
                if (can_do_) {
                    UpdateState();
                    state = State::kREACH;
                } else {
                    can_do_ = true;
                    return;
                }
                break;
            case State::kSHIFTING:
                can_do_ = false;
                Move(direct);
                state = State::kREACH;
                break;
            case State::kREACH:
                CheckApple();
                return;
                break;
            case State::kGAMEOVER:
                game_info_.game_over = true;
                return;
                break;
            }
        }
    }

private:
    void GetHighScore() {
        std::ifstream file("../../../../datasets/score_snake.txt", std::ios::in);

        if (file.is_open())
            file >> game_info_.high_score;
    }

    void SetHighScore(int high) {
        std::ofstream file("../../../../datasets/score_snake.txt", std::ios::out);

        if (file.is_open())
            file << high;
    }

    void Move(Direction direct) {
        if (!game_info_.pause) {
            UpdateDirection(direct);
            ClearField();
            MoveTail(dots_.begin() + 1);
            MoveHead();

            if (CheckCollide()) {
                game_info_.game_over = true;
                return;
            }

            MergeField();
        }
    }

    void UpdateState() {
        if (!game_info_.pause) {
            ClearField();
            MoveTail(dots_.begin() + 1);
            MoveHead();

            if (CheckCollide()) {
                game_info_.game_over = true;
                return;
            }

            MergeField();
        }
    }

    void Reset() {
        dots_.clear();
        dots_ = {
            Coords(5, 10), Coords(5, 11),
            Coords(5, 12), Coords(5, 13)
        };

        direction_ = Direction::kUp;
        game_info_.Reset();

        GenerateApple();
    }

    void UpdateDirection(Direction direct) noexcept {
        switch (direct) {
        case Direction::kUp:
            if (direction_ != Direction::kDown)
                direction_ = direct;
            break;
        case Direction::kDown:
            if (direction_ != Direction::kUp)
                direction_ = direct;
            break;
        case Direction::kLeft:
            if (direction_ != Direction::kRight)
                direction_ = direct;
            break;
        case Direction::kRight:
            if (direction_ != Direction::kLeft)
                direction_ = direct;
            break;
        }
    }

    void MoveTail(std::vector<Coords>::iterator it) noexcept {
        if (it != dots_.end()) {
            const auto& [x_prev, y_prev]{*(it - 1)};

            MoveTail(it + 1);

            auto& [x, y]{*it};

            x = x_prev;
            y = y_prev;
        }
    }

    void MoveHead() {
        auto head{dots_.begin()};
        auto& [x, y]{*head};

        switch (direction_) {
        case Direction::kUp:
            --y;
            break;
        case Direction::kDown:
            ++y;
            break;
        case Direction::kLeft:
            --x;
            break;
        case Direction::kRight:
            ++x;
            break;
        }

        if (!is_walls_)
            СorrectMovement(head);
    }

    bool CheckCollide() {
        auto head{dots_.begin()};
        bool is_collide{false};

        if (is_walls_) {
            is_collide = CheckCollisionWithWalls(head);

            if (is_collide)
                return is_collide;
        }

        is_collide = CheckCollision();

        return is_collide;
    }

    bool CheckCollision() {
        auto count{std::count(dots_.begin(), dots_.end(), dots_.front())};

        if (count > 1)
            return true;

        return false;
    }

    bool CheckCollisionWithWalls(std::vector<Coords>::iterator it) noexcept {
        auto& [x, y]{*it};

        if (x < 0 || y < 0) {
            return true;
        } else if (x > GameInfo_t::field_cols - 1 ||
                   y > GameInfo_t::field_rows - 1) {
            return true;
        }

        return false;
    }

    void CheckApple() {
        if (dots_.front() == apple_) {
            AddDot();
            GenerateApple();

            ++game_info_.score;

            if (game_info_.score > game_info_.high_score) {
                SetHighScore(game_info_.score);
                GetHighScore();
            }

            LevelUp();

            MergeField();
        }
    }

    void AddDot() {
        auto [x, y]{dots_.back()};
        auto [x_prev, y_prev]{*(dots_.end() - 2)};

        if (x > x_prev) {
            ++x;
        } else if (x < x_prev) {
            --x;
        } else if (y > y_prev) {
            ++y;
        } else if (y < y_prev) {
            --y;
        }

        dots_.emplace_back(x, y);
        СorrectMovement(dots_.end() - 1);
    }

    void LevelUp() {
        game_info_.level = game_info_.score / 5 + 1;
        game_info_.speed = 300 - (game_info_.level * 10);
    }

    void СorrectMovement(std::vector<Coords>::iterator it) noexcept {
        auto& [x, y]{*it};

        if (x < 0) {
            x = GameInfo_t::field_cols - 1;
        } else if (y < 0) {
            y = GameInfo_t::field_rows - 1;
        } else if (x > GameInfo_t::field_cols - 1) {
            x = 0;
        } else if (y > GameInfo_t::field_rows - 1) {
            y = 0;
        }
    }

    void GenerateApple() {
        std::uniform_int_distribution<> dist_x(0, GameInfo_t::field_cols - 1);
        std::uniform_int_distribution<> dist_y(0, GameInfo_t::field_rows - 1);

        auto& [x, y]{apple_};

        x = dist_x(rand_gen_);
        y = dist_y(rand_gen_);

        auto it{std::find(dots_.begin(), dots_.end(), apple_)};

        while (it != dots_.end()) {
            x = dist_x(rand_gen_);
            y = dist_y(rand_gen_);

            it = std::find(dots_.begin(), dots_.end(), apple_);
        }
    }

    void ClearField() noexcept {
        for (int i{}; i < GameInfo_t::field_rows; ++i)
            for (int j{}; j < GameInfo_t::field_cols; ++j)
                game_info_.field[i][j] = 0;
    }

    void MergeField() noexcept {
        for (const auto& [x, y] : dots_)
            game_info_.field[y][x] = 1;

        const auto& [x, y]{apple_};
        game_info_.field[y][x] = 2;
    }

private:
    Coords apple_;
    std::vector<Coords> dots_;

    bool is_walls_;
    bool can_do_;

    std::mt19937 rand_gen_;

    Direction direction_;
    GameInfo_t game_info_;
};
}  // namespace s21

#endif  // BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP
