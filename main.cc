#include <QApplication>

#include "view.hpp"
#include "snake.hpp"
#include "controller.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    s21::Snake model_snake;
    s21::Controller controller(&model_snake);
    s21::View window(&controller);

    window.show();

    return app.exec();
}
