#ifndef BRICKGAME_BRICK_GAME_GAME_HPP
#define BRICKGAME_BRICK_GAME_GAME_HPP

#include <vector>
#include <random>
#include <algorithm>

#include "game_info.hpp"

namespace s21 {
class Game {
public:
    enum class Direction {
        kUp,
        kDown,
        kLeft,
        kRight
    };

public:
    using Coords = std::pair<int, int>;

public:
    virtual GameInfo_t GetGameInfo() const = 0;

public:
    virtual void Start() = 0;
    virtual void Stop() noexcept = 0;
    virtual void ResetState() = 0;
    virtual void Move(Direction direct) = 0;
    virtual void UpdateState() = 0;
};
} // namespace s21

#endif // BRICKGAME_BRICK_GAME_GAME_HPP
