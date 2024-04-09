#include <QComboBox>
#include <QGraphicsDropShadowEffect>

#include "view.hpp"
#include "ui_view.h"

namespace s21 {
View::View(Controller* contr, QWidget* parent) :
    QMainWindow(parent),
    ui_(std::make_unique<Ui::View>()),
    timer_(std::make_unique<QTimer>()),
    controller_(contr)
{
    ui_->setupUi(this);
    setFixedSize(350, 620);
    setFocusPolicy(Qt::StrongFocus);

    SetShadowEffect(ui_->frPanel);
    SetShadowEffect(ui_->btnStart);
    SetShadowEffect(ui_->btnSound);
    SetShadowEffect(ui_->btnPause);
    SetShadowEffect(ui_->btnUp);
    SetShadowEffect(ui_->btnDown);
    SetShadowEffect(ui_->btnLeft);
    SetShadowEffect(ui_->btnRight);
    SetShadowEffect(ui_->btnReset);

    ui_->label_12->setVisible(false);
    ui_->gvNextFigure->setVisible(false);

    game_field_ = std::make_unique<GraphicField>(FieldType::kGameField, ui_->gvField);
    figure_field_ = std::make_unique<GraphicField>(FieldType::kFigureField, ui_->gvNextFigure);

    timer_->start(game_info_.speed);

    connect(timer_.get(), &QTimer::timeout, this, &View::ExecTimerAction);
    connect(ui_->cbGame, &QComboBox::currentIndexChanged, this, &View::ChangeGame);
    connect(ui_->cbWalls, &QComboBox::currentIndexChanged, this, &View::ChangeGameMode);

    connect(ui_->btnStart, &QPushButton::clicked, this, [this]() {
        UpdateState(UserAction_t::kStart);
    });

    connect(ui_->btnPause, &QPushButton::clicked, this, [this]() {
        controller_->UserInput(UserAction_t::kPause);
    });

    connect(ui_->btnReset, &QPushButton::clicked, this, [this]() {
        UpdateState(UserAction_t::kTerminate);
    });

    connect(ui_->btnUp, &QPushButton::clicked, this, [this]() {
        UpdateState(UserAction_t::kUp);
    });

    connect(ui_->btnDown, &QPushButton::clicked, this, [this]() {
        UpdateState(UserAction_t::kDown);
    });

    connect(ui_->btnLeft, &QPushButton::clicked, this, [this]() {
        UpdateState(UserAction_t::kLeft);
    });

    connect(ui_->btnRight, &QPushButton::clicked, this, [this]() {
        UpdateState(UserAction_t::kRight);
    });
}

View::~View() {}

void View::SetShadowEffect(QWidget* wdg) {
    QGraphicsDropShadowEffect* shadow{new QGraphicsDropShadowEffect()};

    shadow->setBlurRadius(10);
    shadow->setXOffset(0);
    shadow->setYOffset(0);
    shadow->setColor(Qt::black);

    wdg->setGraphicsEffect(shadow);
}

// void View::BtnSoundClicked() {

//}

void View::UpdateState(UserAction_t act) {
    controller_->UserInput(act);
    game_info_ = controller_->UpdateCurrentState();
    game_field_->Draw(game_info_.field);
    figure_field_->Draw(game_info_.next.figure);
}

void View::ExecTimerAction() {
    if (game_info_.game_over) {
        UpdateState(UserAction_t::kTerminate);
    } else {
        UpdateState(UserAction_t::kAction);

        ui_->lbLevel->setText(QString::number(game_info_.level));
        ui_->lbSpeed->setText(QString::number(game_info_.speed));
        ui_->lbScore->setText(QString::number(game_info_.score));
        ui_->lbHighScore->setText(QString::number(game_info_.high_score));

        timer_->setInterval(game_info_.speed);
    }
}

void View::ChangeGame() {
    UpdateState(UserAction_t::kTerminate);

    if (ui_->cbGame->currentIndex() == 0) {
        ui_->cbWalls->setVisible(true);
        ui_->gvNextFigure->setVisible(false);
        ui_->label_12->setVisible(false);
        ui_->cbWalls->setCurrentIndex(0);
        controller_->SetGame(ModelFacade::Game_t::kSnakeWalls);
    } else if (ui_->cbGame->currentIndex() == 1) {
        ui_->cbWalls->setVisible(false);
        ui_->gvNextFigure->setVisible(true);
        ui_->label_12->setVisible(true);
        controller_->SetGame(ModelFacade::Game_t::kTetris);
    }
}

void View::ChangeGameMode() {
    if (ui_->cbWalls->currentIndex() == 0) {
        controller_->SetGame(ModelFacade::Game_t::kSnakeWalls);
    } else if (ui_->cbWalls->currentIndex() == 1) {
        controller_->SetGame(ModelFacade::Game_t::kSnakeNoWalls);
    }
}

bool View::focusNextPrevChild(bool) {
    return false;
}

void View::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    this->setFocus();
}

void View::focusInEvent(QFocusEvent* event) {
    QMainWindow::focusInEvent(event);
}

void View::focusOutEvent(QFocusEvent* event) {
    QMainWindow::focusOutEvent(event);
    setFocus();
}

void View::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_R:
        UpdateState(UserAction_t::kStart);
        break;
    case Qt::Key_Space:
        UpdateState(UserAction_t::kTerminate);
        break;
    case Qt::Key_P:
        controller_->UserInput(UserAction_t::kPause);
        break;
    case Qt::Key_Up:
        UpdateState(UserAction_t::kUp);
        break;
    case Qt::Key_Down:
        UpdateState(UserAction_t::kDown);
        break;
    case Qt::Key_Left:
        UpdateState(UserAction_t::kLeft);
        break;
    case Qt::Key_Right:
        UpdateState(UserAction_t::kRight);
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}
}  // namespace s21
