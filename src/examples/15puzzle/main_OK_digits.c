#include "cross_lib.h"

/* 15 Puzzle - Sliding tile puzzle */

static uint8_t board[4][4];
static uint8_t empty_r;
static uint8_t empty_c;
static uint16_t move_count;

static void init_board(void)
{
    uint8_t i;
    uint8_t j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            board[i][j] = (uint8_t)(i * 4 + j + 1);
        }
    }
    board[3][3] = 0;
    empty_r = 3;
    empty_c = 3;
    move_count = 0;
}

static void do_shuffle(void)
{
    uint8_t i;
    uint8_t n;
    uint8_t nr;
    uint8_t nc;
    uint8_t dir;

    n = (uint8_t)(50 + (_XL_RAND() % 100));
    for (i = 0; i < n; i++) {
        dir = (uint8_t)(_XL_RAND() % 4);
        nr = empty_r;
        nc = empty_c;
        if (dir == 0 && nr > 0) {
            nr = nr - 1;
        } else if (dir == 1 && nr < 3) {
            nr = nr + 1;
        } else if (dir == 2 && nc > 0) {
            nc = nc - 1;
        } else if (dir == 3 && nc < 3) {
            nc = nc + 1;
        }
        if (nr != empty_r || nc != empty_c) {
            board[empty_r][empty_c] = board[nr][nc];
            board[nr][nc] = 0;
            empty_r = nr;
            empty_c = nc;
        }
    }
}

static void draw_cell(uint8_t r, uint8_t c)
{
    uint8_t val;
    uint8_t sx;
    uint8_t sy;

    val = board[r][c];
    sx = (uint8_t)(1 + c * 2);
    sy = (uint8_t)(1 + r);

    if (val == 0) {
        _XL_PRINT(sx, sy, "  ");
    } else if (val < 10) {
        _XL_PRINT(sx, sy, " ");
        _XL_CHAR((uint8_t)(sx + 1), sy, (char)('0' + val));
    } else {
        _XL_CHAR(sx, sy, '1');
        _XL_CHAR((uint8_t)(sx + 1), sy, (char)('0' + (val - 10)));
    }
}

static void draw_all(void)
{
    uint8_t i;
    uint8_t j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            draw_cell(i, j);
        }
    }
}

static void draw_moves(void)
{
    _XL_PRINT(1, 5, "MOVES:");
    _XL_PRINTD(7, 5, 3, move_count);
}

static uint8_t is_solved(void)
{
    uint8_t i;
    uint8_t j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (board[i][j] != (uint8_t)(i * 4 + j + 1)) {
                return 0;
            }
        }
    }
    return 1;
}

int main(void)
{
    uint8_t input;
    uint8_t nr;
    uint8_t nc;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        _XL_CLEAR_SCREEN();
        init_board();
        do_shuffle();

        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(1, 0, "15 PUZZLE");
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(1, 6, "ARROWS MOVE");

        draw_all();
        draw_moves();

        while (1) {
            input = _XL_INPUT();
            nr = empty_r;
            nc = empty_c;

            if (_XL_LEFT(input)) {
                if (nc < 3) {
                    nc = nc + 1;
                }
            } else if (_XL_RIGHT(input)) {
                if (nc > 0) {
                    nc = nc - 1;
                }
            } else if (_XL_UP(input)) {
                if (nr < 3) {
                    nr = nr + 1;
                }
            } else if (_XL_DOWN(input)) {
                if (nr > 0) {
                    nr = nr - 1;
                }
            }

            if (nr != empty_r || nc != empty_c) {
                board[empty_r][empty_c] = board[nr][nc];
                board[nr][nc] = 0;
                draw_cell(empty_r, empty_c);
                empty_r = nr;
                empty_c = nc;
                draw_cell(empty_r, empty_c);
                move_count = move_count + 1;
                draw_moves();
                _XL_TICK_SOUND();
            }

            if (is_solved()) {
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(1, 5, "SOLVED!    ");
                _XL_PRINT(1, 6, "PRESS ANY KEY");
                _XL_EXPLOSION_SOUND();
                _XL_WAIT_FOR_INPUT();
                break;
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }
    return 0;
}