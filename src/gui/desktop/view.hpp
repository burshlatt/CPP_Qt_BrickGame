#ifndef BRICKGAME_GUI_DESKTOP_VIEW_HPP
#define BRICKGAME_GUI_DESKTOP_VIEW_HPP

#include <QMainWindow>
#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>
#include <QComboBox>
#include <QTimer>

#include <memory>

#include "graphic_widget.hpp"
#include "controller.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

namespace s21 {
class View : public QMainWindow {
    Q_OBJECT

public:
    View(Controller* contr, QWidget *parent = nullptr);
    ~View();

private:
    void ChangeGame();
    void BtnStartClicked();
    void BtnSoundClicked();
    void BtnPauseClicked();
    void ExecTimerAction();
    void SetShadowEffect(QWidget* wdg);
    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::View* ui_;
    QTimer* timer_;
    Controller *controller_;
    std::unique_ptr<GraphicWidget> field_;

    UserAction_t direction_{UserAction_t::kUp};
};
} // namespace s21

#endif // BRICKGAME_GUI_DESKTOP_VIEW_HPP
