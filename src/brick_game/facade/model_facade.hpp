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
    GameInfo_t UpdateCurrentState() const {
        return game_->GetGameInfo();
    }

    void SetGame(Game_t game) {
        if (game == Game_t::kTetris) {
            game_ = std::make_unique<Tetris>();
        } else if (game == Game_t::kSnakeWalls) {
            game_ = std::make_unique<Snake>(true);
        } else if (game == Game_t::kSnakeNoWalls) {
            game_ = std::make_unique<Snake>(false);
        }
    }

public:
    void UserInput(UserAction_t action) {
        switch (action) {
        case UserAction_t::kStart:
            game_->Start();
            break;
        case UserAction_t::kTerminate:
            game_->ResetState();
            break;
        case UserAction_t::kPause:
            game_->Stop();
            break;
        case UserAction_t::kUp:
            game_->Move(Game::Direction::kUp);
            break;
        case UserAction_t::kDown:
            game_->Move(Game::Direction::kDown);
            break;
        case UserAction_t::kLeft:
            game_->Move(Game::Direction::kLeft);
            break;
        case UserAction_t::kRight:
            game_->Move(Game::Direction::kRight);
            break;
        case UserAction_t::kAction:
            game_->UpdateState();
            break;
        }
    }

private:
    std::unique_ptr<Game> game_;
};
} // namespace s21

#endif // BRICKGAME_BRICK_GAME_FACADE_GAME_FACADE_HPP
