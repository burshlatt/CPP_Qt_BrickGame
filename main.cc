#include <QApplication>

#include "view.hpp"
#include "model_facade.hpp"
#include "controller.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    s21::ModelFacade game(s21::ModelFacade::Game_t::kSnakeWalls);
    s21::Controller controller(&game);
    s21::View window(&controller);

    window.show();

    return app.exec();
}
