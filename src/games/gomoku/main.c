#include "cross_lib.h"

#if XSize>=YSize
    #if YSize>=19
        #define BOARD_SIZE ((YSize-1)/2)
        #define Y_BOARD_OFFSET 1
    #else
        #define BOARD_SIZE ((YSize)/2)
        #define Y_BOARD_OFFSET 0
    #endif
#else
    #if XSize>=19
        #define BOARD_SIZE ((XSize-1)/2)
        #define Y_BOARD_OFFSET 1
    #else
        #define BOARD_SIZE ((XSize)/2)
        #define Y_BOARD_OFFSET 0
    #endif
#endif

#define TIE    0
#define EMPTY  0
#define PLAYER 1
#define AI     2

/* Empty board cell: 4 distinct tiles */
#define T_EMPTY_1  _TILE_12
#define T_EMPTY_2  _TILE_13
#define T_EMPTY_3  _TILE_14
#define T_EMPTY_4  _TILE_15

/* Arrow / cursor indicator */
#define T_ARROW    _TILE_26

/* Player stone: 4 distinct tiles */
#define T_PLAYER_1  _TILE_0
#define T_PLAYER_2  _TILE_1
#define T_PLAYER_3  _TILE_2
#define T_PLAYER_4  _TILE_3

/* Opponent stone: 4 distinct tiles */
#define T_OPPONENT_1 _TILE_4
#define T_OPPONENT_2 _TILE_5
#define T_OPPONENT_3 _TILE_6
#define T_OPPONENT_4 _TILE_7

/* Select / cursor: 4 distinct tiles */
#define T_SELECT_1  _TILE_8
#define T_SELECT_2  _TILE_9
#define T_SELECT_3  _TILE_10
#define T_SELECT_4  _TILE_11

#define PRINT_X 8

#define BETA 1

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
uint8_t frame_parity;
uint16_t player_wins;
uint16_t ai_wins;
uint16_t draw_count;
uint8_t win_line_x[5];
uint8_t win_line_y[5];
uint8_t win_line_len;

/* --- Drawing helpers --- */

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

void draw_cell_4tiles(uint8_t col, uint8_t row, uint8_t color_id,
                      uint8_t t1, uint8_t t2, uint8_t t3, uint8_t t4)
{
    uint8_t px, py;
    px = (uint8_t)(bx + col * 2);
    py = (uint8_t)(by + row * 2);
    _XL_DRAW(px, py, t1, color_id);
    _XL_DRAW(px + 1, py, t2, color_id);
    _XL_DRAW(px, py + 1, t3, color_id);
    _XL_DRAW(px + 1, py + 1, t4, color_id);
}

void draw_cell_empty(uint8_t col, uint8_t row, uint8_t color_id)
{
    draw_cell_4tiles(col, row, color_id, T_EMPTY_1, T_EMPTY_2, T_EMPTY_3, T_EMPTY_4);
}

void draw_cell_player(uint8_t col, uint8_t row, uint8_t color_id)
{
    draw_cell_4tiles(col, row, color_id, T_PLAYER_1, T_PLAYER_2, T_PLAYER_3, T_PLAYER_4);
}

void draw_cell_opponent(uint8_t col, uint8_t row, uint8_t color_id)
{
    draw_cell_4tiles(col, row, color_id, T_OPPONENT_1, T_OPPONENT_2, T_OPPONENT_3, T_OPPONENT_4);
}

void draw_cell_select(uint8_t col, uint8_t row, uint8_t color_id)
{
    draw_cell_4tiles(col, row, color_id, T_SELECT_1, T_SELECT_2, T_SELECT_3, T_SELECT_4);
}

/* Draw a cell in its normal (unselected) state */
void draw_normal_cell(uint8_t col, uint8_t row)
{
    if (board[row][col] == EMPTY) {
        draw_cell_empty(col, row, _XL_GREEN);
    } else if (board[row][col] == PLAYER) {
        draw_cell_player(col, row, _XL_CYAN);
    } else if (board[row][col] == AI) {
        draw_cell_opponent(col, row, _XL_WHITE);
    }
}

/*
 * Draw the cursor cell with the blinking overlay:
 *   - EMPTY cell: always show select tiles
 *   - Occupied cell: select tiles on odd frames, normal tiles on even frames
 */
void draw_cursor_cell(uint8_t col, uint8_t row)
{
    if (board[row][col] == EMPTY) {
        draw_cell_select(col, row, _XL_RED);
    } else if (board[row][col] == PLAYER) {
        if (frame_parity) {
            draw_cell_select(col, row, _XL_RED);
        } else {
            draw_cell_player(col, row, _XL_CYAN);
        }
    } else if (board[row][col] == AI) {
        if (frame_parity) {
            draw_cell_select(col, row, _XL_RED);
        } else {
            draw_cell_opponent(col, row, _XL_WHITE);
        }
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
    _XL_PRINT(XSize - 4, 0, "LV");
    _XL_PRINTD(XSize - 2, 0, 1, difficulty);
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            draw_cell_empty(j, i, _XL_GREEN);
            if (board[i][j] == PLAYER) {
                draw_cell_player(j, i, _XL_CYAN);
            } else if (board[i][j] == AI) {
                draw_cell_opponent(j, i, _XL_WHITE);
            }
        }
    }
    draw_cursor_cell(cursor_x, cursor_y);
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
    uint8_t i;
    uint8_t winning_x;
    uint8_t winning_y;
    
    if(winner==AI)
    {
        winning_x = ai_move_x;
        winning_y = ai_move_y;
    }
    else
    {
        winning_x = cursor_x;
        winning_y = cursor_y;
    }
    for(i=0;i<30;++i)
    {
        for (k = 0; k < win_line_len; k++) {
            if(winning_x==win_line_x[k] && winning_y==win_line_y[k])
            {
                draw_cell_player(win_line_x[k], win_line_y[k], _XL_GREEN);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/4);
            }
            draw_cell_player(win_line_x[k], win_line_y[k], _XL_RED);
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR/10);
        }
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
            total = 65535U;
        } else if (count == 4) {
            if (open == 2) total += 50000U;
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


/* --- Helper: find a 3-in-a-row for `player` with an open end,
 *     return the best blocking cell.  Returns 1 on success. --- */
uint8_t find_block_3(uint8_t player, uint8_t *bx_out, uint8_t *by_out)
{
    uint8_t i, j, d;
    short dxs[4] = {0, 1, 1, 1};
    short dys[4] = {1, 0, 1, -1};
    short sx, sy, px, py;
    uint8_t count, open;
    uint8_t best_x, best_y;
    uint16_t best_priority;
    uint8_t found_any;

    best_x = 0;
    best_y = 0;
    best_priority = 0;
    found_any = 0;

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] != player) continue;
            for (d = 0; d < 4; d++) {
                /* Only process from the start of a contiguous run */
                px = (short)j - dxs[d];
                py = (short)i - dys[d];
                if (px >= 0 && px < BOARD_SIZE && py >= 0 && py < BOARD_SIZE &&
                    board[py][px] == player)
                    continue;

                count = 0;
                sx = (short)j;
                sy = (short)i;
                while (sx >= 0 && sx < BOARD_SIZE && sy >= 0 && sy < BOARD_SIZE &&
                       board[sy][sx] == player) {
                    count++;
                    sx += dxs[d];
                    sy += dys[d];
                }
                if (count < 3) continue;

                /* Determine open ends */
                open = 0;
                /* forward end */
                if (sx >= 0 && sx < BOARD_SIZE && sy >= 0 && sy < BOARD_SIZE &&
                    board[sy][sx] == EMPTY)
                    open++;
                /* backward end */
                px = (short)j - dxs[d];
                py = (short)i - dys[d];
                if (px >= 0 && px < BOARD_SIZE && py >= 0 && py < BOARD_SIZE &&
                    board[py][px] == EMPTY)
                    open++;

                if (open == 0) continue;

                /* Priority: open-3 (open==2) > closed-3 (open==1),
                   longer run is more urgent */
                {
                    uint16_t prio = (uint16_t)(count * 10 + open);
                    uint8_t bx, by;

                    /* Prefer the forward open end; fall back to backward */
                    if (sx >= 0 && sx < BOARD_SIZE && sy >= 0 && sy < BOARD_SIZE &&
                        board[sy][sx] == EMPTY) {
                        bx = (uint8_t)sx;
                        by = (uint8_t)sy;
                    } else {
                        bx = (uint8_t)px;
                        by = (uint8_t)py;
                    }

                    if (!found_any || prio > best_priority) {
                        best_priority = prio;
                        best_x = bx;
                        best_y = by;
                        found_any = 1;
                    }
                }
            }
        }
    }

    if (found_any) {
        *bx_out = best_x;
        *by_out = best_y;
        return 1;
    }
    return 0;
}

/* --- Helper: check whether `player` has a winning reply anywhere
 *     on the current board (used for 1-ply lookahead). --- */
uint8_t opponent_can_win_now(uint8_t player)
{
    uint8_t i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = player;
                if (check_win(j, i, player)) {
                    board[i][j] = EMPTY;
                    return 1;
                }
                board[i][j] = EMPTY;
            }
        }
    }
    return 0;
}


/*
 * AI difficulty tiers:
 *
 *   Level 1 – "Reactive"
 *     Detects immediate wins and blocks immediate losses, then plays
 *     a random cell within 2 of an existing stone.  No evaluation.
 *
 *   Level 2 – "Tactical"
 *     Detects immediate wins, blocks immediate losses, blocks any
 *     open 3-in-a-row, then evaluates cells *near existing stones*
 *     (within 2) using equal-weight threat scoring.
 *
 *   Level 3 – "Strategic"
 *     Detects immediate wins, blocks immediate losses, blocks any
 *     open 3-in-a-row, evaluates *every* empty cell with equal-weight
 *     scoring, keeps the top-3 candidates, performs a 1-ply lookahead
 *     (simulates the move and checks whether the opponent can win in
 *     reply, applying a heavy penalty), then adds a small random
 *     jitter to break ties.
 */
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
    uint8_t block_x, block_y;
    uint8_t c;
    uint8_t top_i[3], top_j[3];
    uint16_t top_score[3];
    uint8_t top_count;
    uint8_t opp_can_win;

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

    /* --- All levels: take an immediate win if available --- */
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

    /* --- All levels: block an immediate loss if needed --- */
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

    /* ================================================================
       LEVEL 1 – Reactive: random near stones, no evaluation
       ================================================================ */
    if (difficulty == 1) {
        found = 0;
        for (attempts = 0; attempts < 200 && !found; attempts++) {
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

    /* --- Level 2 & 3: block any open 3-in-a-row --- */
    if (find_block_3(PLAYER, &block_x, &block_y)) {
        if (board[block_y][block_x] == EMPTY) {
            board[block_y][block_x] = AI;
            ai_move_x = block_x;
            ai_move_y = block_y;
            return;
        }
    }

    /* ================================================================
       LEVEL 2 – Tactical: evaluate nearby cells only
       ================================================================ */
    if (difficulty == 2) {
        best_score = 0;
        found = 0;
        for (i = 0; i < BOARD_SIZE; i++) {
            for (j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    /* Only consider cells within 2 of an existing stone */
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
                    if (!close) continue;

                    score = score_position(j, i, AI);
                    board[i][j] = PLAYER;
                    score += score_position(j, i, PLAYER);
                    board[i][j] = EMPTY;

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
            winner = TIE;
        }
        return;
    }

    /* ================================================================
       LEVEL 3 – Strategic: full-board eval + 1-ply lookahead
       ================================================================ */
    top_count = 0;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                score = score_position(j, i, AI);
                board[i][j] = PLAYER;
                score += score_position(j, i, PLAYER);
                board[i][j] = EMPTY;

                /* Maintain a top-3 list */
                if (top_count < 3) {
                    top_i[top_count] = i;
                    top_j[top_count] = j;
                    top_score[top_count] = score;
                    top_count++;
                } else if (score > top_score[2]) {
                    top_i[2] = top_i[1];
                    top_j[2] = top_j[1];
                    top_score[2] = top_score[1];
                    top_i[1] = top_i[0];
                    top_j[1] = top_j[0];
                    top_score[1] = top_score[0];
                    top_i[0] = i;
                    top_j[0] = j;
                    top_score[0] = score;
                }
            }
        }
    }

    if (top_count == 0) {
        game_over = 1;
        winner = TIE;
        return;
    }

    /* 1-ply lookahead: simulate each top candidate, check if the
       opponent can win in reply.  If so, zero out the score. */
    for (c = 0; c < top_count; c++) {
        board[top_i[c]][top_j[c]] = AI;
        opp_can_win = opponent_can_win_now(PLAYER);
        board[top_i[c]][top_j[c]] = EMPTY;
        if (opp_can_win) {
            top_score[c] = 0;
        }
    }

    /* Pick the best surviving candidate with a small random jitter */
    best_score = 0;
    found = 0;
    for (c = 0; c < top_count; c++) {
        score = top_score[c] + (uint16_t)(_XL_RAND() % 100);
        if (score > best_score || !found) {
            best_score = score;
            best_i = top_i[c];
            best_j = top_j[c];
            found = 1;
        }
    }

    board[best_i][best_j] = AI;
    ai_move_x = best_j;
    ai_move_y = best_i;
}

void show_game_over(void)
{
    if (winner == PLAYER || winner == AI) {
        show_win_line();
    }
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    if (winner == PLAYER) {
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(2, YSize - 1, "YOU WIN");
        player_wins++;
        _XL_EXPLOSION_SOUND();
    } else if (winner == AI) {
        _XL_SET_TEXT_COLOR(_XL_MAGENTA);
        _XL_PRINT(2, YSize - 1, "AI WINS");
        ai_wins++;
        _XL_ZAP_SOUND();
    } else {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, YSize - 1, "DRAW");
        draw_count++;
        _XL_TOCK_SOUND();
    }
    _XL_SLEEP(1);
}


int main(void)
{
    uint8_t input;
    uint8_t sel_level;
    uint8_t i;

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
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT((XSize - 12) / 2, 2, "GOMOKU");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT((XSize - 16) / 2 + 2, 5, "SELECT LEVEL");
        _XL_PRINT((XSize - 12) / 2, 7, "1  EASY");
        _XL_PRINT((XSize - 12) / 2, 8, "2  MEDIUM");
        _XL_PRINT((XSize - 12) / 2, 9, "3  HARD");
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(XSize/2-6, 13, "FIRE TO START");
        #if defined(BETA)
            _XL_PRINT(XSize-1-4,YSize-1,"BETA");
        #endif
        by = 0;
        bx = 0;
        for(i=0;i<YSize/2-1;++i)
        {
            if(i&1)
            {
                draw_cell_player(0,i,_XL_CYAN);
                draw_cell_player(XSize/2-1  ,i,_XL_WHITE);

            }
            else
            {
                draw_cell_player(0,i,_XL_WHITE);
                draw_cell_player(XSize/2-1,i,_XL_CYAN);
            }
        }
        _XL_SLEEP(1);

        while (1) {
            input = _XL_INPUT();
            _XL_DRAW((XSize - 12) / 2 - 1, 6 + sel_level, T_ARROW, _XL_RED);
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

            if (_XL_UP(input)) {
                if (sel_level > 1) {
                    _XL_DELETE((XSize - 12) / 2 - 1, 6 + sel_level);
                    sel_level--;
                }
                _XL_TICK_SOUND();
            }
            if (_XL_DOWN(input)) {
                if (sel_level < 3) {
                    _XL_DELETE((XSize - 12) / 2 - 1, 6 + sel_level);
                    sel_level++;
                }
                _XL_TICK_SOUND();
            }
            if (_XL_FIRE(input)) {
                difficulty   = sel_level;
                _XL_DRAW((XSize - 12) / 2 - 1, 6 + sel_level, T_ARROW, _XL_YELLOW);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR*4);
                break;
            }
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* --- Game Setup --- */
        bx = (uint8_t)((XSize - BOARD_SIZE * 2) / 2);
        by = Y_BOARD_OFFSET;
        cursor_x = BOARD_SIZE / 2;
        cursor_y = BOARD_SIZE / 2;
        current_turn = PLAYER;
        game_over = 0;
        winner = TIE;
        win_line_len = 0;
        frame_parity = 0;
        init_board();
        draw_full_board();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(PRINT_X, 0, "YOUR TURN");
        _XL_PING_SOUND();

        /* --- Game Loop --- */
        while (!game_over) {
            /* Player turn */
            while (current_turn == PLAYER && !game_over) {
                input = _XL_INPUT();
                if (_XL_LEFT(input)) {
                    if (cursor_x > 0) {
                        draw_normal_cell(cursor_x, cursor_y);
                        cursor_x--;
                        _XL_TICK_SOUND();
                        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    }
                }
                if (_XL_RIGHT(input)) {
                    if (cursor_x < BOARD_SIZE - 1) {
                        draw_normal_cell(cursor_x, cursor_y);
                        cursor_x++;
                        _XL_TICK_SOUND();
                        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    }
                }
                if (_XL_UP(input)) {
                    if (cursor_y > 0) {
                        draw_normal_cell(cursor_x, cursor_y);
                        cursor_y--;
                        _XL_TICK_SOUND();
                        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    }
                }
                if (_XL_DOWN(input)) {
                    if (cursor_y < BOARD_SIZE - 1) {
                        draw_normal_cell(cursor_x, cursor_y);
                        cursor_y++;
                        _XL_TICK_SOUND();
                        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    }
                }
                if (_XL_FIRE(input)) {
                    if (board[cursor_y][cursor_x] == EMPTY) {
                        board[cursor_y][cursor_x] = PLAYER;
                        draw_cell_player(cursor_x, cursor_y, _XL_CYAN);
                        _XL_SHOOT_SOUND();

                        if (check_win(cursor_x, cursor_y, PLAYER)) {
                            game_over = 1;
                            winner = PLAYER;
                            break;
                        }
                        if (is_board_full()) {
                            game_over = 1;
                            winner = TIE;
                            break;
                        }
                        current_turn = AI;
                        _XL_SET_TEXT_COLOR(_XL_MAGENTA);
                        _XL_PRINT(PRINT_X, 0, "THINKING ");
                        break;
                    }
                }

                /* Toggle frame parity and redraw cursor for blinking */
                frame_parity ^= 1;
                draw_cursor_cell(cursor_x, cursor_y);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }

            /* AI turn */
            if (!game_over && current_turn == AI) {
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 5);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 5);
                ai_make_move();

                if (!game_over) {
                    draw_cell_opponent(ai_move_x, ai_move_y, _XL_WHITE);
                    _XL_TOCK_SOUND();

                    if (check_win(ai_move_x, ai_move_y, AI)) {
                        game_over = 1;
                        winner = AI;
                    } else if (is_board_full()) {
                        game_over = 1;
                        winner = TIE;
                    } else {
                        current_turn = PLAYER;
                        _XL_SET_TEXT_COLOR(_XL_WHITE);
                        
                        _XL_PRINT(PRINT_X, 0, "YOUR TURN");
                    }
                }
            }
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* --- Game Over Screen --- */
        show_game_over();
        _XL_WAIT_FOR_INPUT();
    }

    return 0;
}

