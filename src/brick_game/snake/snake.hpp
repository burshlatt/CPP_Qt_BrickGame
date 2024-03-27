#ifndef BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP
#define BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP

#include <vector>
#include <random>
#include <algorithm>

#include "game_info.hpp"

namespace s21 {
class Snake {
public:
    enum class Direction {
        kUp,
        kDown,
        kLeft,
        kRight
    };

private:
    using Coords = std::pair<int, int>;

public:
    Snake() {
        ResetState();
    }

    ~Snake() = default;

public:
    GameInfo_t GetGameInfo() const noexcept {
        return game_info_;
    }

public:
    void Start() {
        game_info_.pause = false;
        UpdateState(direction_);
    }

    void Stop() noexcept {
        game_info_.pause = true;
    }

    void ResetState() {
        dots_.clear();
        dots_ = {
            Coords(5, 10), Coords(5, 11),
            Coords(5, 12), Coords(5, 13)
        };

        direction_ = Direction::kUp;
        game_info_.Reset();

        GenerateApple();
    }

    void UpdateState(Direction direct) {
        if (!game_info_.pause) {
            UpdateDirection(direct);
            ClearField();
            MoveTail(dots_.begin() + 1);
            MoveHead();
            CheckApple();
            MergeField();
        }
    }

private:
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

        СorrectMovement(head);
        CheckCollision();
    }

    void CheckCollision() {
        auto count{std::count(dots_.begin(), dots_.end(), dots_.front())};

        if (count > 1)
            game_info_.game_over = true;
    }

    void CheckApple() {
        if (dots_.front() == apple_) {
            AddDot();
            GenerateApple();
            ++game_info_.score;
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
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist_x(0, GameInfo_t::field_cols - 1);
        std::uniform_int_distribution<> dist_y(0, GameInfo_t::field_rows - 1);

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

    void MergeField() noexcept {
        for (const auto& [x, y] : dots_)
            game_info_.field[y][x] = 1;

        const auto& [x, y]{apple_};
        game_info_.field[y][x] = 2;
    }

private:
    Coords apple_;
    std::vector<Coords> dots_;

    Direction direction_;
    GameInfo_t game_info_;
};
} // namespace s21

#endif // BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP
