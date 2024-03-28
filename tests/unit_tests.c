#include <check.h>
#include <stdlib.h>

#include "../src/brick_game/tetris/tetris.h"

START_TEST(test_start) {
    GameInfo_t info = InitializeInfo();

    UserInput(kStart);

    info = UpdateCurrentState();

    ck_assert_int_eq(info.pause, false);
    ck_assert_int_eq(info.level, 1);
    ck_assert_int_eq(info.speed, 1200);
    ck_assert_int_eq(info.score, 0);
    ck_assert_ptr_nonnull(info.field);

    UserInput(kTerminate);
}
END_TEST

START_TEST(test_terminate) {
    GameInfo_t info = InitializeInfo();

    UserInput(kTerminate);

    info = UpdateCurrentState();

    ck_assert_int_eq(info.pause, true);
    ck_assert_int_eq(info.level, 1);
    ck_assert_int_eq(info.speed, 1200);
    ck_assert_int_eq(info.score, 0);
    ck_assert_ptr_nonnull(info.field);
}
END_TEST

START_TEST(test_pause) {
    GameInfo_t info = InitializeInfo();

    UserInput(kPause);
    info = UpdateCurrentState();

    ck_assert_int_eq(info.pause, false);

    UserInput(kPause);
    info = UpdateCurrentState();

    ck_assert_int_eq(info.pause, true);
    ck_assert_int_eq(info.level, 1);
    ck_assert_int_eq(info.speed, 1200);
    ck_assert_int_eq(info.score, 0);
    ck_assert_ptr_nonnull(info.field);
}
END_TEST

START_TEST(test_left) {
    InitializeInfo();
    UserInput(kStart);
    GameInfo_t info_1 = UpdateCurrentState();
    UserInput(kLeft);
    GameInfo_t info_2 = UpdateCurrentState();

    for (int i = 0; i < NUMBER_OF_INDEXES; ++i)
        ck_assert_int_eq(info_1.current_figure.indices[i][1] - 1, info_2.current_figure.indices[i][1]);

    UserInput(kTerminate);
}
END_TEST

START_TEST(test_right) {
    InitializeInfo();
    UserInput(kStart);
    GameInfo_t info_1 = UpdateCurrentState();
    UserInput(kRight);
    GameInfo_t info_2 = UpdateCurrentState();

    for (int i = 0; i < NUMBER_OF_INDEXES; ++i)
        ck_assert_int_eq(info_1.current_figure.indices[i][1] + 1, info_2.current_figure.indices[i][1]);

    UserInput(kTerminate);
}
END_TEST

START_TEST(test_action) {
    InitializeInfo();
    UserInput(kStart);
    UpdateCurrentState();
    UpdateCurrentState();

    GameInfo_t info_1 = UpdateCurrentState();

    UserInput(kAction);

    GameInfo_t info_2 = UpdateCurrentState();

    ck_assert_int_eq(info_1.current_figure.type, info_2.current_figure.type);

    int count = 0;

    for (int i = 0; i < NUMBER_OF_INDEXES; ++i)
        if (info_1.current_figure.indices[i][1] == info_2.current_figure.indices[i][1])
            ++count;

    ck_assert_int_ne(count, 4);

    UserInput(kTerminate);
}
END_TEST

START_TEST(test_copy_figure) {
    InitializeInfo();
    UserInput(kStart);

    GameInfo_t info_1 = UpdateCurrentState();

    for (int i = 0; i < 20; ++i)
        UpdateCurrentState();

    GameInfo_t info_2 = UpdateCurrentState();

    ck_assert_int_eq(info_2.current_figure.type, info_1.next_figure.type);

    UserInput(kTerminate);
}
END_TEST

int main() {
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *runner = srunner_create(s1);

    int failed = 0;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_start);
    tcase_add_test(tc1_1, test_terminate);
    tcase_add_test(tc1_1, test_pause);
    tcase_add_test(tc1_1, test_left);
    tcase_add_test(tc1_1, test_right);
    tcase_add_test(tc1_1, test_action);
    tcase_add_test(tc1_1, test_copy_figure);

    srunner_run_all(runner, CK_ENV);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return failed == 0 ? 0 : 1;
}
