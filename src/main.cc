#include <QApplication>

#include "view.hpp"
#include "model_facade.hpp"
#include "controller.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/images/brick_game_icon.png"));

    s21::ModelFacade model(s21::ModelFacade::Game_t::kSnakeWalls);
    s21::Controller controller(&model);
    s21::View view(&controller);

    view.show();

    return app.exec();
}
