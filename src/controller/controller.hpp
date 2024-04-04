#ifndef BRICKGAME_CONTROLLER_CONTROLLER_HPP
#define BRICKGAME_CONTROLLER_CONTROLLER_HPP

#include "model_facade.hpp"

namespace s21 {
class Controller {
 public:
  Controller(ModelFacade* game) : game_(game) {}

  ~Controller() = default;

 public:
  void SetGame(ModelFacade::Game_t game) { game_->SetGame(game); }

  void UserInput(UserAction_t action) { game_->UserInput(action); }

  GameInfo_t UpdateCurrentState() const { return game_->UpdateCurrentState(); }

 private:
  ModelFacade* game_;
};
}  // namespace s21

#endif  // BRICKGAME_CONTROLLER_CONTROLLER_HPP
