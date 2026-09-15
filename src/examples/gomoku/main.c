#include "cross_lib.h"

#define BOARD_SIZE 9
#define EMPTY 0
#define PLAYER 1
#define AI 2

uint8_t board[BOARD_SIZE][BOARD_SIZE];
uint8_t difficulty;
uint8_t current_turn;
uint8_t game_over;
uint8_t winner;
uint8_t cursor_x;
uint8_t cursor_y;
uint8_t bx;
uint8_t by;
uint8_t ai_move_x;
uint8_t ai_move_y;
uint16_t player_wins;
uint16_t ai_wins;
uint16_t draw_count;
uint8_t win_line_x[5];
uint8_t win_line_y[5];
uint8_t win_line_len;

void draw_cell(uint8_t col, uint8_t row, uint8_t tile_id, uint8_t color_id)
{
    uint8_t px, py;
    px = (uint8_t)(bx + col * 2);
    py = (uint8_t)(by + row * 2);
    _XL_DRAW(px, py, tile_id, color_id);
    _XL_DRAW(px + 1, py, tile_id, color_id);
    _XL_DRAW(px, py + 1, tile_id, color_id);
    _XL_DRAW(px + 1, py + 1, tile_id, color_id);
}

void delete_cell(uint8_t col, uint8_t row)
{
    uint8_t px, py;
    px = (uint8_t)(bx + col * 2);
    py = (uint8_t)(by + row * 2);
    _XL_DELETE(px, py);
    _XL_DELETE(px + 1, py);
    _XL_DELETE(px, py + 1);
    _XL_DELETE(px + 1, py + 1);
}

void draw_board_cell(uint8_t col, uint8_t row)
{
    if (board[row][col] == EMPTY) {
        draw_cell(col, row, _TILE_0, _XL_GREEN);
    } else if (board[row][col] == PLAYER) {
        draw_cell(col, row, _TILE_1, _XL_YELLOW);
    } else if (board[row][col] == AI) {
        draw_cell(col, row, _TILE_2, _XL_MAGENTA);
    }
}

void init_board(void)
{
    uint8_t i, j;
    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            board[i][j] = EMPTY;
}

void draw_full_board(void)
{
    uint8_t i, j;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(bx, 0, "GOMOKU");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 1, "LVL ");
    _XL_PRINTD(4, 1, 1, difficulty);
    _XL_PRINT(8, 1, " P:");
    _XL_PRINTD(11, 1, 1, player_wins);
    _XL_PRINT(16, 1, "AI:");
    _XL_PRINTD(19, 1, 1, ai_wins);
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            draw_cell(j, i, _TILE_0, _XL_GREEN);
            if (board[i][j] == PLAYER) {
                draw_cell(j, i, _TILE_1, _XL_YELLOW);
            } else if (board[i][j] == AI) {
                draw_cell(j, i, _TILE_2, _XL_MAGENTA);
            }
        }
    }
    if (board[cursor_y][cursor_x] == EMPTY) {
        draw_cell(cursor_x, cursor_y, _TILE_3, _XL_RED);
    }
}

uint8_t check_win(uint8_t x, uint8_t y, uint8_t player)
{
    short dxs[4];
    short dys[4];
    uint8_t d;
    short sx, sy;
    uint8_t count;
    short min_sx, min_sy;
    uint8_t k;

    dxs[0] = 0;  dys[0] = 1;
    dxs[1] = 1;  dys[1] = 0;
    dxs[2] = 1;  dys[2] = 1;
    dxs[3] = 1;  dys[3] = -1;

    for (d = 0; d < 4; d++) {
        sx = (short)x - dxs[d];
        sy = (short)y - dys[d];
        while (sx >= 0 && sx < BOARD_SIZE && sy >= 0 && sy < BOARD_SIZE && board[sy][sx] == player) {
            sx -= dxs[d];
            sy -= dys[d];
        }
        min_sx = sx + dxs[d];
        min_sy = sy + dys[d];
        count = 0;
        sx = min_sx;
        sy = min_sy;
        while (sx >= 0 && sx < BOARD_SIZE && sy >= 0 && sy < BOARD_SIZE && board[sy][sx] == player) {
            count++;
            sx += dxs[d];
            sy += dys[d];
        }
        if (count >= 5) {
            for (k = 0; k < 5; k++) {
                win_line_x[k] = (uint8_t)(min_sx + k * dxs[d]);
                win_line_y[k] = (uint8_t)(min_sy + k * dys[d]);
            }
            win_line_len = 5;
            return 1;
        }
    }
    return 0;
}

void show_win_line(void)
{
    uint8_t k;
    for (k = 0; k < win_line_len; k++) {
        draw_cell(win_line_x[k], win_line_y[k], _TILE_4, _XL_WHITE);
    }
}

uint16_t score_position(uint8_t x, uint8_t y, uint8_t player)
{
    short dxs[4];
    short dys[4];
    uint8_t d;
    short sx, sy;
    uint8_t count, open;
    uint16_t total;

    dxs[0] = 0;  dys[0] = 1;
    dxs[1] = 1;  dys[1] = 0;
    dxs[2] = 1;  dys[2] = 1;
    dxs[3] = 1;  dys[3] = -1;

    total = 0;
    for (d = 0; d < 4; d++) {
        count = 1;
        open = 0;
        sx = (short)x + dxs[d];
        sy = (short)y + dys[d];
        while (sx >= 0 && sx < BOARD_SIZE && sy >= 0 && sy < BOARD_SIZE && board[sy][sx] == player) {
            count++;
            sx += dxs[d];
            sy += dys[d];
        }
        if (sx >= 0 && sx < BOARD_SIZE && sy >= 0 && sy < BOARD_SIZE && board[sy][sx] == EMPTY)
            open++;
        sx = (short)x - dxs[d];
        sy = (short)y - dys[d];
        while (sx >= 0 && sx < BOARD_SIZE && sy >= 0 && sy < BOARD_SIZE && board[sy][sx] == player) {
            count++;
            sx -= dxs[d];
            sy -= dys[d];
        }
        if (sx >= 0 && sx < BOARD_SIZE && sy >= 0 && sy < BOARD_SIZE && board[sy][sx] == EMPTY)
            open++;
        if (count >= 5) {
            total += 100000;
        } else if (count == 4) {
            if (open == 2) total += 50000;
            else if (open == 1) total += 10000;
        } else if (count == 3) {
            if (open == 2) total += 5000;
            else if (open == 1) total += 1000;
        } else if (count == 2) {
            if (open == 2) total += 500;
            else if (open == 1) total += 100;
        } else {
            if (open == 2) total += 50;
            else if (open == 1) total += 10;
        }
    }
    return total;
}

uint8_t is_board_full(void)
{
    uint8_t i, j;
    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            if (board[i][j] == EMPTY) return 0;
    return 1;
}

void ai_make_move(void)
{
    uint8_t i, j;
    uint16_t best_score, score, rnd;
    uint8_t best_i, best_j;
    uint8_t has_stone;
    uint8_t found;
    uint8_t attempts;
    uint8_t close;
    uint8_t si, sj;
    short ddx, ddy;

    best_i = BOARD_SIZE / 2;
    best_j = BOARD_SIZE / 2;
    has_stone = 0;

    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            if (board[i][j] != EMPTY) has_stone = 1;

    if (!has_stone) {
        board[best_i][best_j] = AI;
        ai_move_x = best_j;
        ai_move_y = best_i;
        return;
    }

    if (difficulty == 1) {
        found = 0;
        for (attempts = 0; attempts < 300 && !found; attempts++) {
            rnd = _XL_RAND() % (BOARD_SIZE * BOARD_SIZE);
            i = (uint8_t)(rnd / BOARD_SIZE);
            j = (uint8_t)(rnd % BOARD_SIZE);
            if (board[i][j] == EMPTY) {
                close = 0;
                for (si = 0; si < BOARD_SIZE && !close; si++) {
                    for (sj = 0; sj < BOARD_SIZE && !close; sj++) {
                        if (board[si][sj] != EMPTY) {
                            ddx = (short)i - (short)si;
                            ddy = (short)j - (short)sj;
                            if (ddx < 0) ddx = -ddx;
                            if (ddy < 0) ddy = -ddy;
                            if (ddx <= 2 && ddy <= 2) close = 1;
                        }
                    }
                }
                if (close) {
                    best_i = i;
                    best_j = j;
                    found = 1;
                }
            }
        }
        if (!found) {
            for (i = 0; i < BOARD_SIZE && !found; i++)
                for (j = 0; j < BOARD_SIZE && !found; j++)
                    if (board[i][j] == EMPTY) { best_i = i; best_j = j; found = 1; }
        }
        board[best_i][best_j] = AI;
        ai_move_x = best_j;
        ai_move_y = best_i;
        return;
    }

    best_score = 0;
    found = 0;

    if (difficulty == 2) {
        for (i = 0; i < BOARD_SIZE; i++) {
            for (j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = AI;
                    if (check_win(j, i, AI)) {
                        ai_move_x = j;
                        ai_move_y = i;
                        return;
                    }
                    board[i][j] = EMPTY;
                }
            }
        }
        for (i = 0; i < BOARD_SIZE; i++) {
            for (j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER;
                    if (check_win(j, i, PLAYER)) {
                        board[i][j] = AI;
                        ai_move_x = j;
                        ai_move_y = i;
                        return;
                    }
                    board[i][j] = EMPTY;
                }
            }
        }
    }

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                if (difficulty == 2) {
                    score = score_position(j, i, AI);
                    board[i][j] = PLAYER;
                    score += score_position(j, i, PLAYER) / 2;
                    board[i][j] = EMPTY;
                    score += _XL_RAND() % 200;
                } else {
                    score = score_position(j, i, AI);
                    board[i][j] = PLAYER;
                    score += score_position(j, i, PLAYER);
                    board[i][j] = EMPTY;
                }
                if (score > best_score || !found) {
                    best_score = score;
                    best_i = i;
                    best_j = j;
                    found = 1;
                }
            }
        }
    }

    if (found) {
        board[best_i][best_j] = AI;
        ai_move_x = best_j;
        ai_move_y = best_i;
    } else {
        game_over = 1;
        winner = 0;
    }
}

void show_game_over(void)
{
    if (winner == PLAYER || winner == AI) {
        show_win_line();
    }
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(0, by + BOARD_SIZE * 2 + 2, "                    ");
    if (winner == PLAYER) {
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(0, by + BOARD_SIZE * 2 + 2, " YOU WIN!          ");
        player_wins++;
        _XL_EXPLOSION_SOUND();
    } else if (winner == AI) {
        _XL_SET_TEXT_COLOR(_XL_MAGENTA);
        _XL_PRINT(0, by + BOARD_SIZE * 2 + 2, " AI WINS!           ");
        ai_wins++;
        _XL_ZAP_SOUND();
    } else {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, by + BOARD_SIZE * 2 + 2, " DRAW!              ");
        draw_count++;
        _XL_TOCK_SOUND();
    }
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(0, by + BOARD_SIZE * 2 + 3, " PRESS FIRE TO CONTINUE");
}

int main(void)
{
    uint8_t input;
    uint8_t sel_level;
    uint8_t old_cx, old_cy;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    player_wins = 0;
    ai_wins = 0;
    draw_count = 0;

    while (1) {
        /* --- Level Select Screen --- */
        sel_level = 1;
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT((XSize - 12) / 2, 2, "GOMOKU");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT((XSize - 16) / 2, 5, "SELECT LEVEL");
        _XL_PRINT((XSize - 12) / 2, 7, "1 - EASY");
        _XL_PRINT((XSize - 12) / 2, 8, "2 - MEDIUM");
        _XL_PRINT((XSize - 12) / 2, 9, "3 - HARD");
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(0, 12, "UP/DOWN TO SELECT");
        _XL_PRINT(0, 13, "FIRE TO START");

        while (1) {
            input = _XL_INPUT();
            if (_XL_UP(input)) {
                if (sel_level > 1) sel_level--;
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_CYAN);
                _XL_PRINT((XSize - 12) / 2, 2, "GOMOKU");
                _XL_SET_TEXT_COLOR(_XL_WHITE);
                _XL_PRINT((XSize - 16) / 2, 5, "SELECT LEVEL");
                _XL_PRINT((XSize - 12) / 2, 7, "1 - EASY");
                _XL_PRINT((XSize - 12) / 2, 8, "2 - MEDIUM");
                _XL_PRINT((XSize - 12) / 2, 9, "3 - HARD");
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT((XSize - 12) / 2, 6 + sel_level, " >");
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_PRINT(0, 12, "UP/DOWN TO SELECT");
                _XL_PRINT(0, 13, "FIRE TO START");
                _XL_TICK_SOUND();
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
            if (_XL_DOWN(input)) {
                if (sel_level < 3) sel_level++;
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_CYAN);
                _XL_PRINT((XSize - 12) / 2, 2, "GOMOKU");
                _XL_SET_TEXT_COLOR(_XL_WHITE);
                _XL_PRINT((XSize - 16) / 2, 5, "SELECT LEVEL");
                _XL_PRINT((XSize - 12) / 2, 7, "1 - EASY");
                _XL_PRINT((XSize - 12) / 2, 8, "2 - MEDIUM");
                _XL_PRINT((XSize - 12) / 2, 9, "3 - HARD");
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT((XSize - 12) / 2, 6 + sel_level, " >");
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_PRINT(0, 12, "UP/DOWN TO SELECT");
                _XL_PRINT(0, 13, "FIRE TO START");
                _XL_TICK_SOUND();
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
            if (_XL_FIRE(input)) {
                difficulty = sel_level;
                break;
            }
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* --- Game Setup --- */
        bx = (uint8_t)((XSize - BOARD_SIZE * 2) / 2);
        by = 3;
        cursor_x = BOARD_SIZE / 2;
        cursor_y = BOARD_SIZE / 2;
        current_turn = PLAYER;
        game_over = 0;
        winner = 0;
        win_line_len = 0;
        init_board();
        draw_full_board();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, by + BOARD_SIZE * 2 + 2, " YOUR TURN            ");
        _XL_PING_SOUND();

        /* --- Game Loop --- */
        while (!game_over) {
            /* Player turn */
            while (current_turn == PLAYER && !game_over) {
                input = _XL_INPUT();
                if (_XL_LEFT(input)) {
                    if (cursor_x > 0) {
                        old_cx = cursor_x;
                        old_cy = cursor_y;
                        cursor_x--;
                        if (board[old_cy][old_cx] == EMPTY) {
                            delete_cell(old_cx, old_cy);
                            draw_board_cell(old_cx, old_cy);
                        }
                        if (board[cursor_y][cursor_x] == EMPTY)
                            draw_cell(cursor_x, cursor_y, _TILE_3, _XL_RED);
                        _XL_TICK_SOUND();
                        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    }
                }
                if (_XL_RIGHT(input)) {
                    if (cursor_x < BOARD_SIZE - 1) {
                        old_cx = cursor_x;
                        old_cy = cursor_y;
                        cursor_x++;
                        if (board[old_cy][old_cx] == EMPTY) {
                            delete_cell(old_cx, old_cy);
                            draw_board_cell(old_cx, old_cy);
                        }
                        if (board[cursor_y][cursor_x] == EMPTY)
                            draw_cell(cursor_x, cursor_y, _TILE_3, _XL_RED);
                        _XL_TICK_SOUND();
                        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    }
                }
                if (_XL_UP(input)) {
                    if (cursor_y > 0) {
                        old_cx = cursor_x;
                        old_cy = cursor_y;
                        cursor_y--;
                        if (board[old_cy][old_cx] == EMPTY) {
                            delete_cell(old_cx, old_cy);
                            draw_board_cell(old_cx, old_cy);
                        }
                        if (board[cursor_y][cursor_x] == EMPTY)
                            draw_cell(cursor_x, cursor_y, _TILE_3, _XL_RED);
                        _XL_TICK_SOUND();
                        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    }
                }
                if (_XL_DOWN(input)) {
                    if (cursor_y < BOARD_SIZE - 1) {
                        old_cx = cursor_x;
                        old_cy = cursor_y;
                        cursor_y++;
                        if (board[old_cy][old_cx] == EMPTY) {
                            delete_cell(old_cx, old_cy);
                            draw_board_cell(old_cx, old_cy);
                        }
                        if (board[cursor_y][cursor_x] == EMPTY)
                            draw_cell(cursor_x, cursor_y, _TILE_3, _XL_RED);
                        _XL_TICK_SOUND();
                        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    }
                }
                if (_XL_FIRE(input)) {
                    if (board[cursor_y][cursor_x] == EMPTY) {
                        delete_cell(cursor_x, cursor_y);
                        board[cursor_y][cursor_x] = PLAYER;
                        draw_cell(cursor_x, cursor_y, _TILE_1, _XL_YELLOW);
                        _XL_SHOOT_SOUND();

                        /* Check win after PLAYER move */
                        if (check_win(cursor_x, cursor_y, PLAYER)) {
                            game_over = 1;
                            winner = PLAYER;
                            break;
                        }
                        if (is_board_full()) {
                            game_over = 1;
                            winner = 0;
                            break;
                        }
                        current_turn = AI;
                        _XL_SET_TEXT_COLOR(_XL_MAGENTA);
                        _XL_PRINT(0, by + BOARD_SIZE * 2 + 2, " AI THINKING...     ");
                        break;
                    }
                }
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }

            /* AI turn */
            if (!game_over && current_turn == AI) {
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 5);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 5);
                ai_make_move();

                if (!game_over) {
                    draw_cell(ai_move_x, ai_move_y, _TILE_2, _XL_MAGENTA);
                    _XL_TOCK_SOUND();

                    /* Check win after AI move */
                    if (check_win(ai_move_x, ai_move_y, AI)) {
                        game_over = 1;
                        winner = AI;
                    } else if (is_board_full()) {
                        game_over = 1;
                        winner = 0;
                    } else {
                        current_turn = PLAYER;
                        _XL_SET_TEXT_COLOR(_XL_WHITE);
                        _XL_PRINT(0, by + BOARD_SIZE * 2 + 2, " YOUR TURN            ");
                    }
                }
            }
        }

        /* --- Game Over Screen --- */
        show_game_over();
        _XL_WAIT_FOR_INPUT();
    }

    return 0;
}