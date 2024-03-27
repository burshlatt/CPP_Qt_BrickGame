#include "view.hpp"
#include "./ui_view.h"

namespace s21 {
View::View(Controller* contr, QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::View),
    timer_(new QTimer),
    controller_(contr)
{
    ui_->setupUi(this);
    setFixedSize(350, 630);
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

    field_ = std::make_unique<GraphicWidget>(ui_->gvField);

    connect(timer_, &QTimer::timeout, this, &View::ExecTimerAction);
    connect(ui_->btnStart, &QPushButton::clicked, this, &View::BtnStartClicked);
    connect(ui_->btnPause, &QPushButton::clicked, this, &View::BtnPauseClicked);
    connect(ui_->cbGame, &QComboBox::currentIndexChanged, this, &View::ChangeGame);

    connect(ui_->btnUp, &QPushButton::clicked, this, [this]() {
        action_ = UserAction_t::kUp;
    });

    connect(ui_->btnDown, &QPushButton::clicked, this, [this]() {
        action_ = UserAction_t::kDown;
    });

    connect(ui_->btnLeft, &QPushButton::clicked, this, [this]() {
        action_ = UserAction_t::kLeft;
    });

    connect(ui_->btnRight, &QPushButton::clicked, this, [this]() {
        action_ = UserAction_t::kRight;
    });
}

View::~View() {
    delete timer_;
    delete ui_;
}

void View::SetShadowEffect(QWidget* wdg) {
    QGraphicsDropShadowEffect* shadow{new QGraphicsDropShadowEffect()};
    shadow->setBlurRadius(10);
    shadow->setXOffset(0);
    shadow->setYOffset(0);
    shadow->setColor(Qt::black);

    wdg->setGraphicsEffect(shadow);
}

void View::BtnStartClicked() {
    if (!timer_->isActive())
        timer_->start(250);
}

void View::BtnPauseClicked() {
    if (timer_->isActive())
        timer_->stop();
}

void View::BtnSoundClicked() {

}

void View::ExecTimerAction() {
    controller_->UserInput(action_);

    GameInfo_t info{controller_->UpdateCurrentState()};

    if (info.game_over) {
        timer_->stop();
        controller_->UserInput(UserAction_t::kTerminate);
    } else {
        field_->Draw(info);
        ui_->lbScore->setText(QString::number(info.score));
        ui_->lbHighScore->setText(QString::number(info.high_score));
    }
}

void View::ChangeGame() {
    if (ui_->cbGame->currentIndex() == 1) {
        ui_->cbWalls->setVisible(false);
    } else {
        ui_->cbWalls->setVisible(true);
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
    if (event->key() == Qt::Key_Up) {
        action_ = UserAction_t::kUp;
    } else if (event->key() == Qt::Key_Down) {
        action_ = UserAction_t::kDown;
    } else if (event->key() == Qt::Key_Left) {
        action_ = UserAction_t::kLeft;
    } else if (event->key() == Qt::Key_Right) {
        action_ = UserAction_t::kRight;
    } else {
        QWidget::keyPressEvent(event);
    }
}
} // namespace s21
