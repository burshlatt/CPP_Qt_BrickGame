#ifndef BRICKGAME_GUI_DESKTOP_GPRAPHIC_WIDGET_HPP
#define BRICKGAME_GUI_DESKTOP_GPRAPHIC_WIDGET_HPP

#include <memory>
#include <random>
#include <vector>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

#define GAME_FIELD_ROWS 20
#define GAME_FIELD_COLS 10
#define FIGURE_FIELD_ROWS 2
#define FIGURE_FIELD_COLS 4

namespace s21 {
enum class FieldType : bool {
    kGameField,
    kFigureField
};

class GraphicField : public QGraphicsView {
public:
    using Field = std::vector<std::vector<int>>;

public:
    GraphicField(FieldType type, QGraphicsView* wdg = nullptr) :
        QGraphicsView(wdg),
        scene_(std::make_unique<QGraphicsScene>(this))
    {
        setScene(scene_.get());
        scene_->addRect(0, 0, wdg->width(), wdg->height());

        if (type == FieldType::kGameField) {
            rows_ = GAME_FIELD_ROWS;
            cols_ = GAME_FIELD_COLS;
        } else if (type == FieldType::kFigureField) {
            rows_ = FIGURE_FIELD_ROWS;
            cols_ = FIGURE_FIELD_COLS;
        }

        cell_w_ = static_cast<qreal>(wdg->width() / cols_);
        cell_h_ = static_cast<qreal>(wdg->height() / rows_);
    }

    ~GraphicField() = default;

public:
    void Draw(const Field& field) {
        dots_.clear();
        scene_->clear();
        setScene(scene_.get());

        // auto color{GetRandomColor()};

        for (int row{}; row < rows_; ++row) {
            for (int col{}; col < cols_; ++col) {
                if (field[row][col] == 1) {
                    // DrawDot(row, col, QBrush(color));
                    DrawDot(row, col, QBrush(Qt::green));
                } else if (field[row][col] == 2) {
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

    Qt::GlobalColor GetRandomColor() {
        static constexpr Qt::GlobalColor colors[5]{
            Qt::red,
            Qt::green,
            Qt::blue,
            Qt::yellow,
            Qt::white
        };

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist(0, 4);

        return colors[dist(gen)];
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
