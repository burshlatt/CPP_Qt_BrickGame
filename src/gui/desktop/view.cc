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

    timer_->start(250);

    connect(timer_, &QTimer::timeout, this, &View::ExecTimerAction);
    connect(ui_->cbGame, &QComboBox::currentIndexChanged, this, &View::ChangeGame);

    connect(ui_->btnStart, &QPushButton::clicked, this, [this]() {
        action_ = UserAction_t::kStart;
    });

    connect(ui_->btnPause, &QPushButton::clicked, this, [this]() {
        action_ = UserAction_t::kPause;
    });

    connect(ui_->btnReset, &QPushButton::clicked, this, [this]() {
        action_ = UserAction_t::kTerminate;
    });

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
    timer_->stop();
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

//void View::BtnSoundClicked() {

//}

void View::ExecTimerAction() {
    controller_->UserInput(action_);

    GameInfo_t info{controller_->UpdateCurrentState()};

    if (info.game_over) {
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
    switch (event->key()) {
    case Qt::Key_R:
        action_ = UserAction_t::kStart;
        break;
    case Qt::Key_Space:
        action_ = UserAction_t::kTerminate;
        break;
    case Qt::Key_P:
        action_ = UserAction_t::kPause;
        break;
    case Qt::Key_Up:
        action_ = UserAction_t::kUp;
        break;
    case Qt::Key_Down:
        action_ = UserAction_t::kDown;
        break;
    case Qt::Key_Left:
        action_ = UserAction_t::kLeft;
        break;
    case Qt::Key_Right:
        action_ = UserAction_t::kRight;
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}
} // namespace s21
