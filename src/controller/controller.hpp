#ifndef BRICKGAME_CONTROLLER_CONTROLLER_HPP
#define BRICKGAME_CONTROLLER_CONTROLLER_HPP

#include "snake.hpp"

namespace s21 {
class Controller {
public:
    Controller(Snake* model) : model_(model) {}
    ~Controller() = default;

public:
    GameInfo_t UpdateCurrentState() const {
        return model_->GetGameInfo();
    }

public:
    void UserInput(UserAction_t action) {
        switch (action) {
            case UserAction_t::kStart:
                break;
            case UserAction_t::kTerminate:
                model_->ResetState();
                break;
            case UserAction_t::kPause:
                break;
            case UserAction_t::kUp:
                model_->UpdateState(Snake::Direction::kUp);
                break;
            case UserAction_t::kDown:
                model_->UpdateState(Snake::Direction::kDown);
                break;
            case UserAction_t::kLeft:
                model_->UpdateState(Snake::Direction::kLeft);
                break;
            case UserAction_t::kRight:
                model_->UpdateState(Snake::Direction::kRight);
                break;
            case UserAction_t::kAction:
                break;
        }
    }

private:
    Snake* model_;
};
} // namespace s21

#endif // BRICKGAME_CONTROLLER_CONTROLLER_HPP
