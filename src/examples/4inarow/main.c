#include "cross_lib.h"

#define GAME_COLS 7
#define ROWS 6

#define START_ROW 3

#define PLAYER_HUMAN 1
#define PLAYER_AI 2

/* 2x2 tile set for the human player (4 distinct tiles) */
#define TILE_HUMAN_TL  _TILE_0
#define TILE_HUMAN_TR  _TILE_1
#define TILE_HUMAN_BL  _TILE_2
#define TILE_HUMAN_BR  _TILE_3

/* 2x2 tile set for the AI (4 distinct tiles, different from human) */
#define TILE_AI_TL     _TILE_4
#define TILE_AI_TR     _TILE_5
#define TILE_AI_BL     _TILE_6
#define TILE_AI_BR     _TILE_7

#define TILE_ARROW     _TILE_8

#define DIFF_EASY 0
#define DIFF_MEDIUM 1
#define DIFF_HARD 2

typedef struct {
    uint8_t r[4];
    uint8_t c[4];
    uint8_t count;
} WinLine;

/* Draw a 2x2 piece at board coordinates (col, row) */
void draw_piece(uint8_t ox, uint8_t oy, uint8_t col, uint8_t row,
                uint8_t player, uint8_t color)
{
    uint8_t x, y;

    x = ox + (uint8_t)(col * 2);
    y = oy + (uint8_t)(row * 2);

    if (player == PLAYER_HUMAN) {
        _XL_DRAW(x, y, TILE_HUMAN_TL, color);
        _XL_DRAW(x + 1, y, TILE_HUMAN_TR, color);
        _XL_DRAW(x, y + 1, TILE_HUMAN_BL, color);
        _XL_DRAW(x + 1, y + 1, TILE_HUMAN_BR, color);
    } else {
        _XL_DRAW(x, y, TILE_AI_TL, color);
        _XL_DRAW(x + 1, y, TILE_AI_TR, color);
        _XL_DRAW(x, y + 1, TILE_AI_BL, color);
        _XL_DRAW(x + 1, y + 1, TILE_AI_BR, color);
    }
}

/* Erase a 2x2 piece at screen coordinates (x, y) */
void erase_piece(uint8_t x, uint8_t y)
{
    _XL_DELETE(x, y);
    _XL_DELETE(x + 1, y);
    // _XL_DELETE(x, y + 1);
    // _XL_DELETE(x + 1, y + 1);
}

/*
 * Animate the piece dropping from the top of the board down to final_row.
 * Falls by increments of 1 tile; erases the previous position at each step.
 */
void drop_animation(uint8_t ox, uint8_t oy, uint8_t col,
                   uint8_t final_row, uint8_t player, uint8_t color)
{
    uint8_t x, y, final_y;

    x = ox + (uint8_t)(col * 2);
    final_y = oy + (uint8_t)(final_row * 2);

    /* Draw at the starting position (top of board) */
    draw_piece(ox, oy, col, 0, player, color);
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

    /* Step down by 1 tile at a time */
    for (y = oy + 1; y <= final_y; y++) {
        /* Erase the 2x2 block at the previous screen row (y-1) */
        erase_piece(x, y - 1);
        /* Draw the 2x2 block at the new screen row (y) */
        if (player == PLAYER_HUMAN) {
            _XL_DRAW(x, y, TILE_HUMAN_TL, color);
            _XL_DRAW(x + 1, y, TILE_HUMAN_TR, color);
            _XL_DRAW(x, y + 1, TILE_HUMAN_BL, color);
            _XL_DRAW(x + 1, y + 1, TILE_HUMAN_BR, color);
        } else {
            _XL_DRAW(x, y, TILE_AI_TL, color);
            _XL_DRAW(x + 1, y, TILE_AI_TR, color);
            _XL_DRAW(x, y + 1, TILE_AI_BL, color);
            _XL_DRAW(x + 1, y + 1, TILE_AI_BR, color);
        }
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

uint8_t get_lowest_empty(uint8_t board[ROWS][GAME_COLS], uint8_t col)
{
    uint8_t r;
    uint8_t result;
    result = 255;
    for (r = 0; r < ROWS; r++) {
        if (board[r][col] == 0) {
            result = r;
        }
    }
    return result;
}

void check_win(uint8_t board[ROWS][GAME_COLS], uint8_t player, WinLine *wl)
{
    uint8_t r, c, count, rr, cc;
    uint8_t i;

    wl->count = 0;

    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < GAME_COLS; c++) {
            count = 0;
            rr = r;
            cc = c;
            while (cc < GAME_COLS && board[rr][cc] == player) {
                count++;
                cc++;
            }
            if (count >= 4) {
                for (i = 0; i < 4; i++) {
                    wl->r[i] = r;
                    wl->c[i] = (uint8_t)(c + i);
                }
                wl->count = 4;
                return;
            }
        }
    }

    for (c = 0; c < GAME_COLS; c++) {
        for (r = 0; r < ROWS; r++) {
            count = 0;
            rr = r;
            cc = c;
            while (rr < ROWS && board[rr][cc] == player) {
                count++;
                rr++;
            }
            if (count >= 4) {
                for (i = 0; i < 4; i++) {
                    wl->r[i] = (uint8_t)(r + i);
                    wl->c[i] = c;
                }
                wl->count = 4;
                return;
            }
        }
    }

    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < GAME_COLS; c++) {
            count = 0;
            rr = r;
            cc = c;
            while (rr < ROWS && cc < GAME_COLS && board[rr][cc] == player) {
                count++;
                rr++;
                cc++;
            }
            if (count >= 4) {
                for (i = 0; i < 4; i++) {
                    wl->r[i] = (uint8_t)(r + i);
                    wl->c[i] = (uint8_t)(c + i);
                }
                wl->count = 4;
                return;
            }
        }
    }

    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < GAME_COLS; c++) {
            count = 0;
            rr = r;
            cc = c;
            while (rr < ROWS && cc < GAME_COLS && board[rr][cc] == player) {
                count++;
                rr++;
                cc--;
            }
            if (count >= 4) {
                for (i = 0; i < 4; i++) {
                    wl->r[i] = (uint8_t)(r + i);
                    wl->c[i] = (uint8_t)(c - i);
                }
                wl->count = 4;
                return;
            }
        }
    }
}

uint8_t has_threat(uint8_t board[ROWS][GAME_COLS], uint8_t player, uint8_t *threat_col)
{
    uint8_t r, c, rr, cc;
    uint8_t count;
    uint8_t found;

    found = 0;
    *threat_col = 255;

    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < GAME_COLS; c++) {
            if (board[r][c] == player) {
                count = 0;
                rr = r;
                cc = c;
                while (cc < GAME_COLS && board[rr][cc] == player) {
                    count++;
                    cc++;
                }
                if (count == 3) {
                    if (cc < GAME_COLS && board[r][cc] == 0) {
                        *threat_col = cc;
                        found = 1;
                        return found;
                    }
                    if (c > 0 && board[r][c - 1] == 0) {
                        *threat_col = (uint8_t)(c - 1);
                        found = 1;
                        return found;
                    }
                }
            }
        }
    }

    for (c = 0; c < GAME_COLS; c++) {
        for (r = 0; r < ROWS; r++) {
            if (board[r][c] == player) {
                count = 0;
                rr = r;
                cc = c;
                while (rr < ROWS && board[rr][cc] == player) {
                    count++;
                    rr++;
                }
                if (count == 3) {
                    if (rr < ROWS && board[rr][c] == 0) {
                        *threat_col = c;
                        found = 1;
                        return found;
                    }
                    if (r > 0 && board[r - 1][c] == 0) {
                        *threat_col = c;
                        found = 1;
                        return found;
                    }
                }
            }
        }
    }

    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < GAME_COLS; c++) {
            if (board[r][c] == player) {
                count = 0;
                rr = r;
                cc = c;
                while (rr < ROWS && cc < GAME_COLS && board[rr][cc] == player) {
                    count++;
                    rr++;
                    cc++;
                }
                if (count == 3) {
                    if (rr < ROWS && cc < GAME_COLS && board[rr][cc] == 0) {
                        *threat_col = cc;
                        found = 1;
                        return found;
                    }
                    if (r > 0 && c > 0 && board[r - 1][c - 1] == 0) {
                        *threat_col = (uint8_t)(c - 1);
                        found = 1;
                        return found;
                    }
                }
            }
        }
    }

    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < GAME_COLS; c++) {
            if (board[r][c] == player) {
                count = 0;
                rr = r;
                cc = c;
                while (rr < ROWS && cc < GAME_COLS && board[rr][cc] == player) {
                    count++;
                    rr++;
                    cc--;
                }
                if (count == 3) {
                    if (rr < ROWS && cc < GAME_COLS && board[rr][cc] == 0) {
                        *threat_col = cc;
                        found = 1;
                        return found;
                    }
                    if (r > 0 && c < (uint8_t)(GAME_COLS - 1) && board[r - 1][c + 1] == 0) {
                        *threat_col = (uint8_t)(c + 1);
                        found = 1;
                        return found;
                    }
                }
            }
        }
    }

    return found;
}

uint8_t creates_threat(uint8_t board[ROWS][GAME_COLS], uint8_t col)
{
    uint8_t row;
    uint8_t tcol;

    row = get_lowest_empty(board, col);
    if (row == 255) return 0;

    board[row][col] = PLAYER_AI;
    has_threat(board, PLAYER_AI, &tcol);
    board[row][col] = 0;

    return (tcol != 255) ? 1 : 0;
}

uint8_t gives_win(uint8_t board[ROWS][GAME_COLS], uint8_t col)
{
    uint8_t row, c2;
    WinLine wl;

    row = get_lowest_empty(board, col);
    if (row == 255) return 0;

    board[row][col] = PLAYER_AI;

    for (c2 = 0; c2 < GAME_COLS; c2++) {
        if (get_lowest_empty(board, c2) != 255) {
            board[get_lowest_empty(board, c2)][c2] = PLAYER_HUMAN;
            check_win(board, PLAYER_HUMAN, &wl);
            board[get_lowest_empty(board, c2)][c2] = 0;
            if (wl.count >= 4) {
                board[row][col] = 0;
                return 1;
            }
        }
    }

    board[row][col] = 0;
    return 0;
}

uint8_t ai_easy(uint8_t board[ROWS][GAME_COLS])
{
    uint8_t col, row;
    WinLine wl;
    uint8_t rnd;

    rnd = (uint8_t)(_XL_RAND() % 100);

    if (rnd < 40) {
        for (col = 0; col < GAME_COLS; col++) {
            row = get_lowest_empty(board, col);
            if (row != 255) {
                board[row][col] = PLAYER_AI;
                check_win(board, PLAYER_AI, &wl);
                board[row][col] = 0;
                if (wl.count >= 4) return col;
            }
        }
        for (col = 0; col < GAME_COLS; col++) {
            row = get_lowest_empty(board, col);
            if (row != 255) {
                board[row][col] = PLAYER_HUMAN;
                check_win(board, PLAYER_HUMAN, &wl);
                board[row][col] = 0;
                if (wl.count >= 4) return col;
            }
        }
    }

    col = (uint8_t)(_XL_RAND() % GAME_COLS);
    while (get_lowest_empty(board, col) == 255) {
        col = (uint8_t)((col + 1) % GAME_COLS);
    }
    return col;
}

uint8_t ai_medium(uint8_t board[ROWS][GAME_COLS])
{
    uint8_t col, row;
    WinLine wl;
    uint8_t priority[GAME_COLS];
    uint8_t i;

    for (col = 0; col < GAME_COLS; col++) {
        row = get_lowest_empty(board, col);
        if (row != 255) {
            board[row][col] = PLAYER_AI;
            check_win(board, PLAYER_AI, &wl);
            board[row][col] = 0;
            if (wl.count >= 4) return col;
        }
    }

    for (col = 0; col < GAME_COLS; col++) {
        row = get_lowest_empty(board, col);
        if (row != 255) {
            board[row][col] = PLAYER_HUMAN;
            check_win(board, PLAYER_HUMAN, &wl);
            board[row][col] = 0;
            if (wl.count >= 4) return col;
        }
    }

    priority[0] = 3;
    priority[1] = 2;
    priority[2] = 4;
    priority[3] = 1;
    priority[4] = 5;
    priority[5] = 0;
    priority[6] = 6;

    for (i = 0; i < GAME_COLS; i++) {
        col = priority[i];
        if (get_lowest_empty(board, col) != 255) {
            return col;
        }
    }

    col = (uint8_t)(_XL_RAND() % GAME_COLS);
    return col;
}

uint8_t ai_hard(uint8_t board[ROWS][GAME_COLS])
{
    uint8_t col, row;
    WinLine wl;
    uint8_t threat_col;
    uint8_t priority[GAME_COLS];
    uint8_t i;

    for (col = 0; col < GAME_COLS; col++) {
        row = get_lowest_empty(board, col);
        if (row != 255) {
            board[row][col] = PLAYER_AI;
            check_win(board, PLAYER_AI, &wl);
            board[row][col] = 0;
            if (wl.count >= 4) return col;
        }
    }

    for (col = 0; col < GAME_COLS; col++) {
        row = get_lowest_empty(board, col);
        if (row != 255) {
            board[row][col] = PLAYER_HUMAN;
            check_win(board, PLAYER_HUMAN, &wl);
            board[row][col] = 0;
            if (wl.count >= 4) return col;
        }
    }

    if (has_threat(board, PLAYER_HUMAN, &threat_col)) {
        if (get_lowest_empty(board, threat_col) != 255) {
            return threat_col;
        }
    }

    for (col = 0; col < GAME_COLS; col++) {
        if (get_lowest_empty(board, col) != 255) {
            if (creates_threat(board, col)) {
                return col;
            }
        }
    }

    priority[0] = 3;
    priority[1] = 2;
    priority[2] = 4;
    priority[3] = 1;
    priority[4] = 5;
    priority[5] = 0;
    priority[6] = 6;

    for (i = 0; i < GAME_COLS; i++) {
        col = priority[i];
        if (get_lowest_empty(board, col) != 255) {
            if (!gives_win(board, col)) {
                return col;
            }
        }
    }

    for (i = 0; i < GAME_COLS; i++) {
        col = priority[i];
        if (get_lowest_empty(board, col) != 255) {
            return col;
        }
    }

    col = (uint8_t)(_XL_RAND() % GAME_COLS);
    return col;
}

uint8_t get_ai_move(uint8_t board[ROWS][GAME_COLS], uint8_t difficulty)
{
    if (difficulty == DIFF_EASY) {
        return ai_easy(board);
    } else if (difficulty == DIFF_MEDIUM) {
        return ai_medium(board);
    } else {
        return ai_hard(board);
    }
}

void reset_board(uint8_t board[ROWS][GAME_COLS])
{
    uint8_t r, c;
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < GAME_COLS; c++) {
            board[r][c] = 0;
        }
    }
}

uint8_t is_board_full(uint8_t board[ROWS][GAME_COLS])
{
    uint8_t c;
    for (c = 0; c < GAME_COLS; c++) {
        if (board[0][c] == 0) {
            return 0;
        }
    }
    return 1;
}

void highlight_win(uint8_t board[ROWS][GAME_COLS], WinLine *wl, uint8_t ox, uint8_t oy)
{
    uint8_t i;

    for (i = 0; i < wl->count; i++) {
        if (board[wl->r[i]][wl->c[i]] == PLAYER_HUMAN) {
            draw_piece(ox, oy, wl->c[i], wl->r[i], PLAYER_HUMAN, _XL_MAGENTA);
        } else {
            draw_piece(ox, oy, wl->c[i], wl->r[i], PLAYER_AI, _XL_MAGENTA);
        }
    }
}

uint8_t select_difficulty(void)
{
    uint8_t input;
    uint8_t sel;
    uint8_t ox;
    uint8_t old_sel;

    sel = DIFF_MEDIUM;
    ox = (uint8_t)((XSize - 20) / 2);

    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(ox, 2, "SELECT DIFFICULTY");
    _XL_PRINT(ox, 6, "EASY  MEDIUM  HARD");

    _XL_DRAW(ox + 8, 7, TILE_ARROW, _XL_CYAN);

    while (1) {
        input = _XL_INPUT();

        if (_XL_LEFT(input)) {
            if (sel > DIFF_EASY) {
                old_sel = sel;
                sel--;
                if (old_sel == DIFF_EASY) {
                    _XL_DELETE(ox + 1, 7);
                } else if (old_sel == DIFF_MEDIUM) {
                    _XL_DELETE(ox + 8, 7);
                } else {
                    _XL_DELETE(ox + 15, 7);
                }
                _XL_SET_TEXT_COLOR(_XL_CYAN);
                if (sel == DIFF_EASY) {
                    _XL_DRAW(ox + 1, 7, TILE_ARROW, _XL_CYAN);
                } else {
                    _XL_DRAW(ox + 8, 7, TILE_ARROW, _XL_CYAN);
                }
                _XL_TICK_SOUND();
            }
        } else if (_XL_RIGHT(input)) {
            if (sel < DIFF_HARD) {
                old_sel = sel;
                sel++;
                if (old_sel == DIFF_EASY) {
                    _XL_DELETE(ox + 1, 7);
                } else if (old_sel == DIFF_MEDIUM) {
                    _XL_DELETE(ox + 8, 7);
                } else {
                    _XL_DELETE(ox + 15, 7);
                }
                _XL_SET_TEXT_COLOR(_XL_CYAN);
                if (sel == DIFF_MEDIUM) {
                    _XL_DRAW(ox + 8, 7, TILE_ARROW, _XL_CYAN);
                } else {
                    _XL_DRAW(ox + 15, 7, TILE_ARROW, _XL_CYAN);
                }
                _XL_TICK_SOUND();
            }
        } else if (_XL_FIRE(input)) {
            _XL_PING_SOUND();
            return sel;
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

int main(void)
{
    uint8_t board[ROWS][GAME_COLS];
    uint8_t state;
    uint8_t turn;
    uint8_t input;
    uint8_t cursor;
    uint8_t col, row;
    uint8_t ox, oy;
    uint8_t difficulty;
    uint8_t cursor_row;
    WinLine wl;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    ox = (uint8_t)((XSize - (GAME_COLS * 2)) / 2);
    oy = START_ROW;
    cursor_row = (uint8_t)(START_ROW - 1);

    while (1) {
        difficulty = select_difficulty();

        reset_board(board);
        state = 0;
        turn = PLAYER_HUMAN;
        cursor = (uint8_t)(GAME_COLS / 2);

        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(ox, 0, "CONNECT FOUR");

        if (difficulty == DIFF_EASY) {
            _XL_PRINT(ox, 1, "YOU VS EASY");
        } else if (difficulty == DIFF_MEDIUM) {
            _XL_PRINT(ox, 1, "YOU VS MEDIUM");
        } else {
            _XL_PRINT(ox, 1, "YOU VS HARD");
        }

        _XL_DRAW(ox + cursor * 2, cursor_row, TILE_ARROW, _XL_CYAN);

        while (state == 0) {
            if (turn == PLAYER_HUMAN) {
                while (state == 0 && turn == PLAYER_HUMAN) {
                    input = _XL_INPUT();

                    if (_XL_LEFT(input)) {
                        if (cursor > 0) {
                            _XL_DELETE(ox + cursor * 2, cursor_row);
                            cursor--;
                            _XL_DRAW(ox + cursor * 2, cursor_row, TILE_ARROW, _XL_CYAN);
                            _XL_TICK_SOUND();
                        }
                    } else if (_XL_RIGHT(input)) {
                        if (cursor < (uint8_t)(GAME_COLS - 1)) {
                            _XL_DELETE(ox + cursor * 2, cursor_row);
                            cursor++;
                            _XL_DRAW(ox + cursor * 2, cursor_row,  TILE_ARROW, _XL_CYAN);
                            _XL_TICK_SOUND();
                        }
                    } else if (_XL_FIRE(input)) {
                        row = get_lowest_empty(board, cursor);
                        if (row != 255) {
                            board[row][cursor] = PLAYER_HUMAN;
                            drop_animation(ox, oy, cursor, row,
                                          PLAYER_HUMAN, _XL_RED);
                            _XL_PING_SOUND();

                            check_win(board, PLAYER_HUMAN, &wl);
                            if (wl.count >= 4) {
                                state = 1;
                                turn = PLAYER_HUMAN;
                                highlight_win(board, &wl, ox, oy);
                            } else if (is_board_full(board)) {
                                state = 2;
                            } else {
                                turn = PLAYER_AI;
                            }
                        } else {
                            _XL_TOCK_SOUND();
                        }
                    }

                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                }
            } else {
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 3);

                col = get_ai_move(board, difficulty);
                row = get_lowest_empty(board, col);

                if (row != 255) {
                    board[row][col] = PLAYER_AI;
                    drop_animation(ox, oy, col, row,
                                  PLAYER_AI, _XL_YELLOW);
                    _XL_SHOOT_SOUND();

                    check_win(board, PLAYER_AI, &wl);
                    if (wl.count >= 4) {
                        state = 1;
                        turn = PLAYER_AI;
                        highlight_win(board, &wl, ox, oy);
                    } else if (is_board_full(board)) {
                        state = 2;
                    } else {
                        turn = PLAYER_HUMAN;
                    }
                }

                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 2);
            }
        }

        _XL_DELETE(ox + cursor * 2, cursor_row);
        _XL_SET_TEXT_COLOR(_XL_WHITE);

        if (state == 1) {
            if (turn == PLAYER_HUMAN) {
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(ox, 1, "YOU WIN      ");
            } else {
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(ox, 1, "CPU WINS     ");
            }
            _XL_EXPLOSION_SOUND();
        } else {
            _XL_SET_TEXT_COLOR(_XL_CYAN);
            _XL_PRINT(ox, 1,     "DRAW         ");
            _XL_ZAP_SOUND();
        }

        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();
    }

    return 0;
}