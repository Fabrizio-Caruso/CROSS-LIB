#include "cross_lib.h"

#define N 4

/* Board stored as exponents: 0=empty, 1=>2, 2=>4, …, 11=>2048 */
uint8_t board[N][N];
uint8_t prev_board[N][N];
uint16_t score;

/* ---- layout (derived from XSize / YSize macros) ---- */
#define GAP          1
#define BOARD_TOP   3
#define CELL_W      ((XSize - (N + 1) * GAP) / N)
#define CELL_H      ((YSize - BOARD_TOP - (N + 1) * GAP) / N)

#define CX(c)       (GAP + (c) * (CELL_W + GAP))
#define CY(r)       (BOARD_TOP + GAP + (r) * (CELL_H + GAP))

/* ---- helpers ---- */

static uint8_t val_to_color(uint8_t v) {
    switch (v) {
        case 1:  return _XL_WHITE;
        case 2:  return _XL_YELLOW;
        case 3:  return _XL_RED;
        case 4:  return _XL_MAGENTA;
        case 5:  return _XL_CYAN;
        case 6:  return _XL_GREEN;
        default: return _XL_BLUE;   /* 128 and above */
    }
}

static void fill_cell(uint8_t r, uint8_t c) {
    uint8_t x0 = CX(c), y0 = CY(r);
    uint8_t dx, dy;
    for (dy = 0; dy < CELL_H; dy++)
        for (dx = 0; dx < CELL_W; dx++)
            _XL_DRAW(x0 + dx, y0 + dy, _TILE_0, val_to_color(board[r][c]));
}

static void clear_cell(uint8_t r, uint8_t c) {
    uint8_t x0 = CX(c), y0 = CY(r);
    uint8_t dx, dy;
    for (dy = 0; dy < CELL_H; dy++)
        for (dx = 0; dx < CELL_W; dx++)
            // _XL_DELETE(x0 + dx, y0 + dy);
            _XL_DRAW(x0 + dx, y0 + dy, _TILE_0, _XL_WHITE);

}

static void draw_cell(uint8_t r, uint8_t c) {
    if (board[r][c] == 0) {
        clear_cell(r, c);
    } else {
        fill_cell(r, c);
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(CX(c), CY(r), 4, (uint16_t)(1 << board[r][c]));
    }
}

static void draw_changed(void) {
    uint8_t r, c;
    for (r = 0; r < N; r++)
        for (c = 0; c < N; c++)
            if (board[r][c] != prev_board[r][c])
                draw_cell(r, c);
}

static void save_prev(void) {
    uint8_t r, c;
    for (r = 0; r < N; r++)
        for (c = 0; c < N; c++)
            prev_board[r][c] = board[r][c];
}

/* ---- game logic ---- */

static void slide_row_left(uint8_t row) {
    uint8_t w = 0, i;
    uint8_t tmp[N];
    for (i = 0; i < N; i++) tmp[i] = board[row][i];
    for (i = 0; i < N; i++) {
        if (tmp[i]) {
            if (w > 0 && tmp[w - 1] == tmp[i]) {
                tmp[w - 1]++;
                score += (uint16_t)(1 << tmp[w - 1]);
            } else {
                tmp[w++] = tmp[i];
            }
        }
    }
    for (; w < N; w++) tmp[w] = 0;
    for (i = 0; i < N; i++) board[row][i] = tmp[i];
}

static void slide_col_up(uint8_t col) {
    uint8_t w = 0, i;
    uint8_t tmp[N];
    for (i = 0; i < N; i++) tmp[i] = board[i][col];
    for (i = 0; i < N; i++) {
        if (tmp[i]) {
            if (w > 0 && tmp[w - 1] == tmp[i]) {
                tmp[w - 1]++;
                score += (uint16_t)(1 << tmp[w - 1]);
            } else {
                tmp[w++] = tmp[i];
            }
        }
    }
    for (; w < N; w++) tmp[w] = 0;
    for (i = 0; i < N; i++) board[i][col] = tmp[i];
}

static void move_left(void) {
    uint8_t r;
    for (r = 0; r < N; r++) slide_row_left(r);
}

static void move_right(void) {
    uint8_t r, c, i;
    for (r = 0; r < N; r++) {
        /* reverse row */
        for (i = 0; i < N / 2; i++) {
            uint8_t t = board[r][i];
            board[r][i] = board[r][N - 1 - i];
            board[r][N - 1 - i] = t;
        }
    }
    move_left();
    for (r = 0; r < N; r++) {
        for (i = 0; i < N / 2; i++) {
            uint8_t t = board[r][i];
            board[r][i] = board[r][N - 1 - i];
            board[r][N - 1 - i] = t;
        }
    }
}

static void move_up(void) {
    uint8_t c;
    for (c = 0; c < N; c++) slide_col_up(c);
}

static void move_down(void) {
    uint8_t r, c, i;
    /* reverse each column */
    for (c = 0; c < N; c++) {
        for (i = 0; i < N / 2; i++) {
            uint8_t t = board[i][c];
            board[i][c] = board[N - 1 - i][c];
            board[N - 1 - i][c] = t;
        }
    }
    move_up();
    /* reverse back */
    for (c = 0; c < N; c++) {
        for (i = 0; i < N / 2; i++) {
            uint8_t t = board[i][c];
            board[i][c] = board[N - 1 - i][c];
            board[N - 1 - i][c] = t;
        }
    }
}

static uint8_t can_move(void) {
    uint8_t r, c;
    for (r = 0; r < N; r++) {
        for (c = 0; c < N; c++) {
            if (board[r][c] == 0) return 1;
            if (c + 1 < N && board[r][c] == board[r][c + 1]) return 1;
            if (r + 1 < N && board[r][c] == board[r + 1][c]) return 1;
        }
    }
    return 0;
}

static void spawn_tile(void) {
    uint8_t empty[N * N];
    uint8_t count = 0, idx, r, c;
    for (r = 0; r < N; r++)
        for (c = 0; c < N; c++)
            if (board[r][c] == 0) {
                empty[count * 2]     = r;
                empty[count * 2 + 1] = c;
                count++;
            }
    if (count > 0) {
        idx = _XL_RAND() % count;
        board[empty[idx * 2]][empty[idx * 2 + 1]] = (_XL_RAND() & 1) ? 2 : 1;
    }
}

static void reset_game(void) {
    uint8_t r, c;
    for (r = 0; r < N; r++)
        for (c = 0; c < N; c++) {
            board[r][c] = 0;
            prev_board[r][c] = 0xFF;   /* force full redraw */
        }
    score = 0;
    spawn_tile();
    spawn_tile();
}

/* ---- main ---- */

int main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        reset_game();

        /* title & score header */
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 0, "2048");
        _XL_PRINTD(XSize - 6, 0, 1, score);

        draw_changed();
        save_prev();

        while (can_move()) {
            uint8_t input = _XL_INPUT();
            uint8_t moved = 0;

            if (_XL_LEFT(input))  { move_left();  moved = 1; }
            else if (_XL_RIGHT(input)) { move_right(); moved = 1; }
            else if (_XL_UP(input))    { move_up();    moved = 1; }
            else if (_XL_DOWN(input))  { move_down();  moved = 1; }

            if (moved) {
                spawn_tile();
                _XL_PING_SOUND();

                /* update score display */
                _XL_SET_TEXT_COLOR(_XL_WHITE);
                _XL_PRINTD(XSize - 6, 0, 1, score);

                draw_changed();
                save_prev();
            }
        }

        /* game over screen */
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(0, YSize / 2, "GAME OVER");
        _XL_ZAP_SOUND();
        _XL_WAIT_FOR_INPUT();
    }
}
