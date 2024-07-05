#ifndef BRICKGAME_GUI_DESKTOP_VIEW_HPP
#define BRICKGAME_GUI_DESKTOP_VIEW_HPP

#include <memory>

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>

#include "controller.hpp"
#include "graphic_widget.hpp"

#define SNAKE 0
#define WALLS 0
#define NO_WALLS 1

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace s21 {
class View : public QMainWindow {
    Q_OBJECT

public:
    View(Controller* contr, QWidget* parent = nullptr);
    ~View();

private:
    bool focusNextPrevChild(bool) override;
    void showEvent(QShowEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void ChangeGame();
    //    void BtnSoundClicked();
    void ChangeGameMode();
    void ExecTimerAction();
    void BtnStartClicked();
    void BtnResetClicked();
    void BtnPauseClicked();
    void UpdateState(UserAction_t act);
    void SetShadowEffect(QWidget* wdg);
    void ShiftClicked(UserAction_t act);

private:
    std::unique_ptr<Ui::View> ui_;
    std::unique_ptr<QTimer> timer_;

    std::unique_ptr<GraphicField> game_field_;
    std::unique_ptr<GraphicField> figure_field_;

    GameInfo_t game_info_;
    Controller* controller_;
};
}  // namespace s21

#endif  // BRICKGAME_GUI_DESKTOP_VIEW_HPP
