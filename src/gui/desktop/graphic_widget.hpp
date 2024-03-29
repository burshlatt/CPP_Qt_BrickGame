#ifndef BRICKGAME_GUI_DESKTOP_GPRAPHIC_WIDGET_HPP
#define BRICKGAME_GUI_DESKTOP_GPRAPHIC_WIDGET_HPP

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

#include "game_info.hpp"

namespace s21 {
class GameField : public QGraphicsView {
public:
    GameField(QGraphicsView *wdg = nullptr) : QGraphicsView(wdg) {
        scene_ = new QGraphicsScene(this);
        setScene(scene_);
        scene_->addRect(0, 0, 200, 400);
    }

    GameField(const GameField& wdg) = delete;
    GameField(GameField&& wdg) noexcept = delete;

    ~GameField() {
        ClearScene();
        delete scene_;
    }

public:
    void Draw(const GameInfo_t& info) {
        ClearScene();
        setScene(scene_);

        for (int row = 0; row < rows_; ++row) {
            for (int col = 0; col < cols_; ++col) {
                if (info.field[row][col] == 1) {
                    DrawDot(row, col, QBrush(Qt::green));
                } else if (info.field[row][col] == 2) {
                    DrawDot(row, col, QBrush(Qt::red));
                }
            }
        }
    }

private:
    void DrawDot(int row, int col, QBrush brush) {
        qreal x{static_cast<qreal>(col * cell_w_)};
        qreal y{static_cast<qreal>(row * cell_h_)};

        dots_.push_back(new QGraphicsRectItem(x, y, cell_w_, cell_h_));

        dots_.back()->setBrush(brush);
        scene_->addItem(dots_.back());
    }

    void ClearScene() {
        for (qsizetype i{}; i < dots_.size(); ++i) {
            if (dots_[i]) {
                delete dots_[i];
                dots_[i] = nullptr;
            }
        }

        scene_->clear();
    }

private:
    QGraphicsScene* scene_;
    QVector<QGraphicsRectItem*> dots_;

    static constexpr int rows_{20};
    static constexpr int cols_{10};
    static constexpr qreal cell_w_{static_cast<qreal>(200 / cols_)};
    static constexpr qreal cell_h_{static_cast<qreal>(400 / rows_)};
};

class FigureField : public QGraphicsView {
public:
    FigureField(QGraphicsView *wdg = nullptr) : QGraphicsView(wdg) {
        scene_ = new QGraphicsScene(this);
        setScene(scene_);
        scene_->addRect(0, 0, 60, 30);
    }

    FigureField(const GameField& wdg) = delete;
    FigureField(GameField&& wdg) noexcept = delete;

    ~FigureField() {
        ClearScene();
        delete scene_;
    }

public:
    void Draw(const GameInfo_t& info) {
        ClearScene();
        setScene(scene_);

        for (int row = 0; row < rows_; ++row)
            for (int col = 0; col < cols_; ++col)
                if (info.next.figure[row][col] == 1)
                    DrawDot(row, col, QBrush(Qt::green));
    }

private:
    void DrawDot(int row, int col, QBrush brush) {
        qreal x{static_cast<qreal>(col * cell_w_)};
        qreal y{static_cast<qreal>(row * cell_h_)};

        dots_.push_back(new QGraphicsRectItem(x, y, cell_w_, cell_h_));

        dots_.back()->setBrush(brush);
        scene_->addItem(dots_.back());
    }

    void ClearScene() {
        for (qsizetype i{}; i < dots_.size(); ++i) {
            if (dots_[i]) {
                delete dots_[i];
                dots_[i] = nullptr;
            }
        }

        scene_->clear();
    }

private:
    QGraphicsScene* scene_;
    QVector<QGraphicsRectItem*> dots_;

    static constexpr int rows_{2};
    static constexpr int cols_{4};
    static constexpr qreal cell_w_{static_cast<qreal>(60 / cols_)};
    static constexpr qreal cell_h_{static_cast<qreal>(30 / rows_)};
};
} // namespace s21

#endif // BRICKGAME_GUI_DESKTOP_GPRAPHIC_WIDGET_HPP
