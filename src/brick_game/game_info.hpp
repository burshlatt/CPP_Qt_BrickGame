#ifndef BRICKGAME_BRICK_GAME_GAME_INFO_HPP
#define BRICKGAME_BRICK_GAME_GAME_INFO_HPP

#include <vector>
#include <utility>

namespace s21 {
typedef enum {
    kStart,
    kPause,
    kTerminate,
    kLeft,
    kRight,
    kUp,
    kDown,
    kAction,
} UserAction_t;

struct GameInfo_t {
private:
    struct Figure_t {
        Figure_t() :
            type(0),
            figure(figure_rows, std::vector<int>(figure_cols)),
            indices(4, std::pair<int, int>(0, 0))
        {}

        int type;
        std::vector<std::vector<int>> figure;
        std::vector<std::pair<int, int>> indices;
    };

public:
    GameInfo_t() : field(field_rows, std::vector<int>(field_cols)) {
        Reset();
    }

    ~GameInfo_t() = default;

public:
    void Reset() {
        for(auto& row : field)
            std::fill(row.begin(), row.end(), 0);

        next = Figure_t();
        current = Figure_t();

        score = 0;
        high_score = 0;
        level = 1;
        speed = 300;
        pause = true;
        game_over = false;
    }

public:
    int score;
    int high_score;
    int level;
    int speed;

    bool pause;
    bool game_over;

    Figure_t next;
    Figure_t current;

    std::vector<std::vector<int>> field;

    static constexpr int field_rows{20};
    static constexpr int field_cols{10};
    static constexpr int figure_rows{2};
    static constexpr int figure_cols{4};
};
} // namespace s21

#endif // BRICKGAME_BRICK_GAME_GAME_INFO_HPP
