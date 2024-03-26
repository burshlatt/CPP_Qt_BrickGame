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
    kAction
} UserAction_t;

struct GameInfo_t {
public:
    GameInfo_t() :
        field(field_rows, std::vector<int>(field_cols)),
        next(figure_rows, std::vector<int>(figure_cols))
    {
        Reset();
    }

    ~GameInfo_t() = default;

public:
    void Reset() {
        for(auto& row : field)
            std::fill(row.begin(), row.end(), 0);

        for(auto& row : next)
            std::fill(row.begin(), row.end(), 0);

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
    std::vector<std::vector<int>> field;
    std::vector<std::vector<int>> next;

    static constexpr int field_rows{20};
    static constexpr int field_cols{10};
    static constexpr int figure_rows{4};
    static constexpr int figure_cols{10};
};
} // namespace s21

#endif // BRICKGAME_BRICK_GAME_GAME_INFO_HPP
