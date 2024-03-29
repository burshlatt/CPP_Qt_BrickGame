#ifndef BRICKGAME_GUI_DESKTOP_VIEW_HPP
#define BRICKGAME_GUI_DESKTOP_VIEW_HPP

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>

#include <memory>

#include "controller.hpp"
#include "graphic_widget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

namespace s21 {
class View : public QMainWindow {
    Q_OBJECT

public:
    View(Controller* contr, QWidget *parent = nullptr);
    ~View();

protected:
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
    void UpdateState(UserAction_t act);
    void SetShadowEffect(QWidget* wdg);

private:
    Ui::View* ui_;
    QTimer* timer_;

    GameInfo_t game_info_;

    Controller* controller_;
    std::unique_ptr<GameField> game_field_;
    std::unique_ptr<FigureField> figure_field_;

    UserAction_t action_{UserAction_t::kUp};
};
} // namespace s21

#endif // BRICKGAME_GUI_DESKTOP_VIEW_HPP
