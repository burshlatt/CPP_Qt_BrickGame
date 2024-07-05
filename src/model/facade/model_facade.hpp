#ifndef BRICKGAME_BRICK_GAME_FACADE_GAME_FACADE_HPP
#define BRICKGAME_BRICK_GAME_FACADE_GAME_FACADE_HPP

#include <memory>

#include "game.hpp"
#include "snake.hpp"
#include "tetris.hpp"

namespace s21 {
class ModelFacade {
public:
    enum class Game_t {
        kTetris,
        kSnakeWalls,
        kSnakeNoWalls
    };

public:
    ModelFacade(Game_t game) {
        SetGame(game);
    }

    ~ModelFacade() = default;

public:
    const GameInfo_t& UpdateCurrentState() const {
        return game_->GetGameInfo();
    }

    void SetGame(Game_t game) {
        if (game == Game_t::kTetris) {
            game_ = std::make_unique<Tetris>();
        } else if (game == Game_t::kSnakeWalls) {
            game_ = std::make_unique<Snake>(Snake::Mode::kWall);
        } else if (game == Game_t::kSnakeNoWalls) {
            game_ = std::make_unique<Snake>(Snake::Mode::kNoWall);
        }
    }

public:
    void UserInput(UserAction_t action) {
        switch (action) {
        case UserAction_t::kStart:
            game_->SigAct(Game::State::kStart, Game::Direction::kUp);
            break;
        case UserAction_t::kTerminate:
            game_->SigAct(Game::State::kGameOver, Game::Direction::kUp);
            break;
        case UserAction_t::kPause:
            game_->SigAct(Game::State::kPause, Game::Direction::kUp);
            break;
        case UserAction_t::kUp:
            game_->SigAct(Game::State::kShifting, Game::Direction::kUp);
            break;
        case UserAction_t::kDown:
            game_->SigAct(Game::State::kShifting, Game::Direction::kDown);
            break;
        case UserAction_t::kLeft:
            game_->SigAct(Game::State::kShifting, Game::Direction::kLeft);
            break;
        case UserAction_t::kRight:
            game_->SigAct(Game::State::kShifting, Game::Direction::kRight);
            break;
        case UserAction_t::kAction:
            game_->SigAct(Game::State::kMoving, Game::Direction::kUp);
            break;
        }
    }

private:
    std::unique_ptr<Game> game_;
};
}  // namespace s21

#endif  // BRICKGAME_BRICK_GAME_FACADE_GAME_FACADE_HPP
