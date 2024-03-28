#ifndef BRICKGAME_CONTROLLER_CONTROLLER_HPP
#define BRICKGAME_CONTROLLER_CONTROLLER_HPP

#include "game.hpp"

namespace s21 {
class Controller {
public:
    Controller(Game* model) : model_(model) {}
    ~Controller() = default;

public:
    GameInfo_t UpdateCurrentState() const noexcept {
        return model_->GetGameInfo();
    }

    void SetModel(Game* model) noexcept { model_ = model; }

public:
    void UserInput(UserAction_t action) {
        switch (action) {
            case UserAction_t::kStart:
                model_->Start();
                break;
            case UserAction_t::kTerminate:
                model_->ResetState();
                break;
            case UserAction_t::kPause:
                model_->Stop();
                break;
            case UserAction_t::kUp:
                model_->Move(Game::Direction::kUp);
                break;
            case UserAction_t::kDown:
                model_->Move(Game::Direction::kDown);
                break;
            case UserAction_t::kLeft:
                model_->Move(Game::Direction::kLeft);
                break;
            case UserAction_t::kRight:
                model_->Move(Game::Direction::kRight);
                break;
            case UserAction_t::kAction:
                model_->UpdateState();
                break;
            case UserAction_t::kWalls:
                model_->SetWalls(true);
                break;
            case UserAction_t::kNoWalls:
                model_->SetWalls(false);
                break;
        }
    }

private:
    Game* model_;
};
} // namespace s21

#endif // BRICKGAME_CONTROLLER_CONTROLLER_HPP
