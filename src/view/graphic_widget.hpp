#ifndef BRICKGAME_GUI_DESKTOP_GPRAPHIC_WIDGET_HPP
#define BRICKGAME_GUI_DESKTOP_GPRAPHIC_WIDGET_HPP

#include <memory>
#include <vector>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

#include "game.hpp"

namespace s21 {
enum class FieldType : bool {
    kGameField,
    kFigureField
};

class GraphicField : public QGraphicsView {
public:
    GraphicField(FieldType ft, QGraphicsView* wdg = nullptr) : QGraphicsView(wdg) {
        scene_ = std::make_unique<QGraphicsScene>(this);
        setScene(scene_.get());
        scene_->addRect(0, 0, wdg->width(), wdg->height());

        if (ft == FieldType::kGameField) {
            rows_ = 20;
            cols_ = 10;
        } else if (ft == FieldType::kFigureField) {
            rows_ = 2;
            cols_ = 4;
        }

        cell_w_ = static_cast<qreal>(wdg->width() / cols_);
        cell_h_ = static_cast<qreal>(wdg->height() / rows_);
    }

    ~GraphicField() {
        ClearScene();
    }

public:
    void Draw(const GameInfo_t& info) {
        ClearScene();
        setScene(scene_.get());

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

        dots_.push_back(std::make_unique<QGraphicsRectItem>(x, y, cell_w_, cell_h_));

        dots_.back()->setBrush(brush);
        scene_->addItem(dots_.back().get());
    }

    void ClearScene() {
        dots_.clear();
        scene_->clear();
    }

private:
    std::unique_ptr<QGraphicsScene> scene_;
    std::vector<std::unique_ptr<QGraphicsRectItem>> dots_;

    int rows_;
    int cols_;
    qreal cell_w_;
    qreal cell_h_;
};
}  // namespace s21

#endif  // BRICKGAME_GUI_DESKTOP_GPRAPHIC_WIDGET_HPP
