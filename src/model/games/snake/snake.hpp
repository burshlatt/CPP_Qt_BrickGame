#ifndef BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP
#define BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP

#include <algorithm>
#include <random>
#include <vector>

#include "game.hpp"

namespace s21 {
class Snake final : public Game {
public:
    enum class Mode : bool {
        kWall,
        kNoWall
    };

public:
    Snake(Mode mode) {
        is_walls_ = mode == Mode::kWall;

        ResetState();
    }

public:
    const GameInfo_t& GetGameInfo() const final {
        return game_info_;
    }

public:
    void SigAct(State state, Direction direct) final {
        while (true) {
            switch (state) {
            case State::kStart:
                game_info_.pause = false;
                state = State::kSpawn;
                break;
            case State::kSpawn:
                MergeFields();
                state = State::kMoving;
                break;
            case State::kMoving:
                Move();
                state = State::kReach;
                break;
            case State::kShifting:
                Shift(direct);
                state = State::kReach;
                break;
            case State::kReach:
                HandleAppleCollision();
                return;
                break;
            case State::kPause:
                game_info_.pause = true;
                return;
                break;
            case State::kGameOver:
                ResetState();
                return;
                break;
            }
        }
    }

private:
    void ResetState() {
        dots_ = {
            Coords(5, 10), Coords(5, 11),
            Coords(5, 12), Coords(5, 13)
        };

        direction_ = Direction::kUp;
        game_info_.Reset();

        GenerateApple();
    }

    void Move() {
        if (!game_info_.pause) {
            ClearField();
            MoveTail(dots_.begin() + 1);
            MoveHead();

            if (IsCollision()) {
                ResetState();
                return;
            }

            MergeFields();
        }
    }

    void Shift(Direction direct) {
        if (!game_info_.pause) {
            UpdateDirection(direct);
            Move();
        }
    }

    void UpdateDirection(Direction direct) noexcept {
        switch (direct) {
        case Direction::kUp:
            direction_ = direction_ != Direction::kDown ? direct : direction_;
            break;
        case Direction::kDown:
            direction_ = direction_ != Direction::kUp ? direct : direction_;
            break;
        case Direction::kLeft:
            direction_ = direction_ != Direction::kRight ? direct : direction_;
            break;
        case Direction::kRight:
            direction_ = direction_ != Direction::kLeft ? direct : direction_;
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

    bool IsCollision() {
        auto head{dots_.begin()};
        bool is_collision{false};

        if (is_walls_) {
            is_collision = IsWallCollision(head);

            if (is_collision)
                return is_collision;
        }

        is_collision = IsTailCollision();

        return is_collision;
    }

    bool IsTailCollision() {
        auto count{std::count(dots_.begin(), dots_.end(), dots_.front())};

        if (count > 1)
            return true;

        return false;
    }

    bool IsWallCollision(std::vector<Coords>::iterator it) noexcept {
        auto& [x, y]{*it};

        if (x < 0 || y < 0) {
            return true;
        } else if (x > GameInfo_t::field_cols - 1 ||
                   y > GameInfo_t::field_rows - 1) {
            return true;
        }

        return false;
    }

    void HandleAppleCollision() {
        if (dots_.front() == apple_) {
            AddDot();
            GenerateApple();

            ++game_info_.score;

            LevelUp();
            MergeFields();
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
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist_x(0, GameInfo_t::field_cols - 1);
        static std::uniform_int_distribution<> dist_y(0, GameInfo_t::field_rows - 1);

        auto& [x, y]{apple_};

        x = dist_x(gen);
        y = dist_y(gen);

        auto it{std::find(dots_.begin(), dots_.end(), apple_)};

        while (it != dots_.end()) {
            x = dist_x(gen);
            y = dist_y(gen);

            it = std::find(dots_.begin(), dots_.end(), apple_);
        }
    }

    void ClearField() noexcept {
        for (int i{}; i < GameInfo_t::field_rows; ++i)
            for (int j{}; j < GameInfo_t::field_cols; ++j)
                game_info_.field[i][j] = 0;
    }

    void MergeFields() noexcept {
        for (const auto& [x, y] : dots_)
            game_info_.field[y][x] = 1;

        const auto& [x, y]{apple_};
        game_info_.field[y][x] = 2;
    }

private:
    Coords apple_;
    std::vector<Coords> dots_;

    bool is_walls_;

    Direction direction_;
    GameInfo_t game_info_;
};
}  // namespace s21

#endif  // BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP
