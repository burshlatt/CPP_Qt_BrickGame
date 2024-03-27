#ifndef BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP
#define BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP

#include <vector>
#include <memory>
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
    struct Coords {
        int x;
        int y;

        Coords(int x_c=0, int y_c=0) :
            x(x_c),
            y(y_c)
        {}

        bool operator==(const Coords& o) const noexcept {
            return x == o.x && y == o.y;
        }
    };

public:
    Snake() {
        ResetState();
        MergeField();
    }

    ~Snake() = default;

public:
    GameInfo_t GetGameInfo() const {
        return game_info_;
    }

public:
    void UpdateState(Direction direct) {
        UpdateDirection(direct);
        ClearField();
        MoveTail(++dots_.begin());
        MoveHead();
        CheckApple();
        MergeField();
    }

    void ResetState() {
        dots_.clear();

        dots_.emplace_back(5, 10);
        dots_.emplace_back(5, 11);
        dots_.emplace_back(5, 12);
        dots_.emplace_back(5, 13);

        direction_ = Direction::kUp;
        game_info_.Reset();

        GenerateApple();
    }

private:
    void UpdateDirection(Direction direct) {
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

    void CheckCollision() {
        auto head{dots_.begin()};
        auto count{std::count(dots_.begin(), dots_.end(), *head)};

        if (count > 1)
                game_info_.game_over = true;
    }

    void MoveTail(std::vector<Coords>::iterator it) {
        if (it != dots_.end()) {
            auto prev_dot{it};
            --prev_dot;

            auto next_dot{it};
            MoveTail(++next_dot);

            it->x = prev_dot->x;
            it->y = prev_dot->y;
        }
    }

    void MoveHead() {
        auto head{dots_.begin()};

        switch (direction_) {
            case Direction::kUp:
                --head->y;
                break;
            case Direction::kDown:
                ++head->y;
                break;
            case Direction::kLeft:
                --head->x;
                break;
            case Direction::kRight:
                ++head->x;
                break;
        }

        СorrectMovement(head);
        CheckCollision();
    }

    void СorrectMovement(std::vector<Coords>::iterator it) {
        if (it->x < 0) {
            it->x = game_info_.field_cols - 1;
        } else if (it->y < 0) {
            it->y = game_info_.field_rows - 1;
        } else if (it->x > game_info_.field_cols - 1) {
            it->x = 0;
        } else if (it->y > game_info_.field_rows - 1) {
            it->y = 0;
        }
    }

    void AddDot() {
        int x{dots_.back().x};
        int y{dots_.back().y};

        auto prev{std::prev(dots_.end(), 2)};

        if (x > prev->x) {
            ++x;
        } else if (x < prev->x) {
            --x;
        } else if (y > prev->y) {
            ++y;
        } else if (y < prev->y) {
            --y;
        }

        dots_.emplace_back(x, y);
        СorrectMovement(dots_.end() - 1);
    }

    void CheckApple() {
        auto head{dots_.begin()};

        if (head->x == apple_.x && head->y == apple_.y) {
            AddDot();
            GenerateApple();
            ++game_info_.score;
        }
    }

    void GenerateApple() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist_x(0, GameInfo_t::field_cols - 1);
        std::uniform_int_distribution<> dist_y(0, GameInfo_t::field_rows - 1);

        apple_.x = dist_x(gen);
        apple_.y = dist_y(gen);

        auto exists{std::find(dots_.begin(), dots_.end(), apple_)};

        while (exists != dots_.end()) {
            apple_.x = dist_x(gen);
            apple_.y = dist_y(gen);

            exists = std::find(dots_.begin(), dots_.end(), apple_);
        }
    }

    void ClearField() {
        for (int i{}; i < GameInfo_t::field_rows; ++i)
            for (int j{}; j < GameInfo_t::field_cols; ++j)
                game_info_.field[i][j] = 0;
    }

    void MergeField() {
        for (const auto& coords : dots_)
            game_info_.field[coords.y][coords.x] = 1;

        game_info_.field[apple_.y][apple_.x] = 2;
    }

private:
    Coords apple_;
    std::vector<Coords> dots_;

    Direction direction_;
    GameInfo_t game_info_;
};
} // namespace s21

#endif // BRICKGAME_BRICK_GAME_SNAKE_SNAKE_HPP
