#include "cross_lib.h"

#if XSize >= 38
    #define ROAD_W 15
#elif XSize >= 32
    #define ROAD_W 13
#else
    #define ROAD_W 11
#endif

#if XSize <= 24
    #define ROAD_L (((XSize - ROAD_W) / 2) + 2)
    #define ROAD_R ((ROAD_L + ROAD_W - 1) + 2)
#else
    #define ROAD_L ((XSize - ROAD_W) / 2)
    #define ROAD_R (ROAD_L + ROAD_W - 1)
#endif

#define CENTER_X (ROAD_L + ROAD_W / 2)
#define PLAYER_Y (YSize - 4)

/*
 * The player now moves in half-tile units.
 *
 * player_x = 2k      -> normal tile position k
 * player_x = 2k + 1  -> half tile between k and k+1, drawn as two columns:
 *                         column k and column k+1
 */

/* Maximum even logical x is ROAD_R*2. */
#define PLAYER_MIN_X ((uint16_t)ROAD_L * 2)

/*
 * If the player is at an odd half-tile position 2k+1, it occupies columns
 * k and k+1. Therefore the highest legal odd position is limited so that
 * k+1 does not exceed ROAD_R.
 *
 * Highest legal logical x:
 *   even: ROAD_R * 2
 *   odd : ROAD_R * 2 - 1
 */
#define PLAYER_MAX_X ((uint16_t)ROAD_R * 2)

/* Highest position where an odd half-tile draw is still safe. */
#define PLAYER_MAX_ODD_X ((uint16_t)ROAD_R * 2 - 1)

#define MAX_ENEMIES 8
#define MAX_BLOCKS 16

#define OBSTACLE_TILE _TILE_6
#define BORDER_TILE _TILE_5

#define PLAYER_TOP _TILE_0
#define PLAYER_BOT _TILE_1

#define ENEMY_TOP _TILE_2
#define ENEMY_BOT _TILE_3

#define BIG_ENEMY_TOP _TILE_4
#define BIG_ENEMY_BOT _TILE_7

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
} Car;

typedef struct {
    uint8_t x;
    uint8_t height;
    uint8_t entered;
    uint8_t top;
} Block;

static void draw_car(Car *c, uint8_t top_tile, uint8_t bot_tile)
{
    uint8_t i, j;

    for (i = 0; i < c->h; i++) {
        if ((uint16_t)c->y + i >= YSize) break;
        for (j = 0; j < c->w; j++) {
            if (i == 0) {
                _XL_DRAW(c->x + j, c->y, top_tile, _XL_RED);
            } else {
                _XL_DRAW(c->x + j, c->y + i, bot_tile, _XL_CYAN);
            }
        }
    }
}

static void erase_car(Car *c)
{
    uint8_t i, j;

    for (i = 0; i < c->h; i++) {
        if ((uint16_t)c->y + i >= YSize) break;
        for (j = 0; j < c->w; j++) {
            _XL_DELETE(c->x + j, c->y + i);
        }
    }
}

static void draw_borders(void)
{
    uint8_t y;

    for (y = 0; y < YSize; y++) {
        _XL_DRAW((uint8_t)(ROAD_L - 1), y, BORDER_TILE, _XL_WHITE);
        _XL_DRAW((uint8_t)(ROAD_R + 1), y, BORDER_TILE, _XL_WHITE);
    }
}

static void erase_borders(void)
{
    uint8_t y;

    for (y = 0; y < YSize; y++) {
        _XL_DELETE((uint8_t)(ROAD_L - 1), y);
        _XL_DELETE((uint8_t)(ROAD_R + 1), y);
    }
}

/*
 * Returns the leftmost column occupied by the player.
 */
static uint8_t get_left_col(uint16_t px)
{
    return (uint8_t)(px / 2);
}

/*
 * Returns the rightmost column occupied by the player.
 *
 * Even px: one tile only, left == right.
 * Odd px : two tiles, left = px/2, right = left + 1.
 */
static uint8_t get_right_col(uint16_t px)
{
    if (px & 1u) {
        return (uint8_t)(px / 2 + 1);
    }

    return (uint8_t)(px / 2);
}

/*
 * Draws the player according to its half-tile position.
 */

/* Final clean player drawing/erasing helpers. */
// static void draw_player(uint16_t px)
// {
    // uint8_t left;
    // uint8_t right;
    // uint8_t y;

    // left = get_left_col(px);
    // right = get_right_col(px);

    // for (y = PLAYER_Y; y <= (uint8_t)(PLAYER_Y + 1); y++) {
        // if (left == right) {
            // _XL_DRAW(left, y, PLAYER_TOP, _XL_GREEN);
        // } else {
            // _XL_DRAW(left, y, PLAYER_TOP, _XL_GREEN);
            // _XL_DRAW(right, y, PLAYER_TOP, _XL_GREEN);
        // }
    // }

    // /* Bottom tile */
    // for (y = (uint8_t)(PLAYER_Y + 1); y <= (uint8_t)(PLAYER_Y + 1); y++) {
        // if (left == right) {
            // _XL_DRAW(left, y, PLAYER_BOT, _XL_GREEN);
        // } else {
            // _XL_DRAW(left, y, PLAYER_BOT, _XL_GREEN);
            // _XL_DRAW(right, y, PLAYER_BOT, _XL_GREEN);
        // }
    // }
// }
static void draw_player(uint16_t px)
{
    uint8_t left;
    uint8_t right;
    uint8_t y;

    left = get_left_col(px);
    right = get_right_col(px);

    /* Top row */
    for (y = PLAYER_Y; y <= PLAYER_Y; y++) {
        if (left == right) {
            _XL_DRAW(left, y, PLAYER_TOP, _XL_GREEN);
        } else {
            _XL_DRAW(left, y, PLAYER_TOP, _XL_GREEN);
            _XL_DRAW(right, y, PLAYER_TOP, _XL_GREEN);
        }
    }

    /* Bottom row */
    for (y = (uint8_t)(PLAYER_Y + 1); y <= (uint8_t)(PLAYER_Y + 1); y++) {
        if (left == right) {
            _XL_DRAW(left, y, PLAYER_BOT, _XL_GREEN);
        } else {
            _XL_DRAW(left, y, PLAYER_BOT, _XL_GREEN);
            _XL_DRAW(right, y, PLAYER_BOT, _XL_GREEN);
        }
    }
}


// static void erase_player(uint16_t px)
// {
    // uint8_t left;
    // uint8_t right;
    // uint8_t y;

    // left = get_left_col(px);
    // right = get_right_col(px);

    // for (y = PLAYER_Y; y <= (uint8_t)(PLAYER_Y + 1); y++) {
        // if (left == right) {
            // _XL_DELETE(left, y);
        // } else {
            // _XL_DELETE(left, y);
            // _XL_DELETE(right, y);
        // }
    // }
// }
static void erase_player(uint16_t px)
{
    uint8_t left;
    uint8_t right;
    uint8_t y;

    left = get_left_col(px);
    right = get_right_col(px);

    for (y = PLAYER_Y; y <= (uint8_t)(PLAYER_Y + 1); y++) {
        if (left == right) {
            _XL_DELETE(left, y);
        } else {
            _XL_DELETE(left, y);
            _XL_DELETE(right, y);
        }
    }
}



static uint8_t is_blocked(uint8_t x, uint8_t y, Block *blocks, uint8_t block_count)
{
    uint8_t i;

    if (x < ROAD_L || x > ROAD_R) return 1;

    for (i = 0; i < block_count; i++) {
        if (blocks[i].x != x) continue;

        if (blocks[i].entered < blocks[i].height) {
            if (y < blocks[i].entered) return 1;
        } else {
            if (y >= blocks[i].top && y < (uint8_t)(blocks[i].top + blocks[i].height)) {
                return 1;
            }
        }
    }

    return 0;
}

static uint8_t has_active_at(Block *blocks, uint8_t block_count, uint8_t x)
{
    uint8_t i;

    for (i = 0; i < block_count; i++) {
        if (blocks[i].x == x) return 1;
    }

    return 0;
}

static void spawn_lateral_pair(Block *blocks, uint8_t *block_count)
{
    uint8_t h;

    if (*block_count + 2 > MAX_BLOCKS) return;
    if (has_active_at(blocks, *block_count, ROAD_L)) return;

    h = (uint8_t)(10 + _XL_RAND() % 16);

    blocks[*block_count].x = ROAD_L;
    blocks[*block_count].height = h;
    blocks[*block_count].entered = 0;
    blocks[*block_count].top = 0;
    (*block_count)++;

    blocks[*block_count].x = ROAD_R;
    blocks[*block_count].height = h;
    blocks[*block_count].entered = 0;
    blocks[*block_count].top = 0;
    (*block_count)++;
}

static void spawn_central(Block *blocks, uint8_t *block_count)
{
    uint8_t h;

    if (*block_count + 1 > MAX_BLOCKS) return;
    if (has_active_at(blocks, *block_count, CENTER_X)) return;

    h = (uint8_t)(10 + _XL_RAND() % 16);

    blocks[*block_count].x = CENTER_X;
    blocks[*block_count].height = h;
    blocks[*block_count].entered = 0;
    blocks[*block_count].top = 0;
    (*block_count)++;
}

static uint8_t cars_overlap(uint8_t ax, uint8_t ay, uint8_t aw, uint8_t ah,
                            uint8_t bx, uint8_t by, uint8_t bw, uint8_t bh)
{
    if (ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by) {
        return 1;
    }

    return 0;
}

#if XSize < 31
    #define SCORE_X ROAD_L - 6
#else
    #define SCORE_X ROAD_L - 7
#endif

static void game_loop(void)
{
    Block blocks[MAX_BLOCKS];
    Car enemies[MAX_ENEMIES];
    uint8_t block_count;
    uint8_t enemy_count;
    uint16_t player_x; /* Now in half-tile units. */
    uint8_t i, k;
    uint16_t frame;
    uint16_t score;
    uint8_t game_over;
    uint8_t input;
    uint16_t slowdown;

    _XL_CLEAR_SCREEN();
    draw_borders();

    block_count = 0;
    enemy_count = 0;

    /* Start centered on a normal tile. */
    player_x = (uint16_t)CENTER_X * 2;

    frame = 0;
    score = 0;
    game_over = 0;

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(ROAD_L, YSize / 2, "RACING");

    draw_player(player_x);

    _XL_SLEEP(1);

    for (i = 0; i < ROAD_W; i++) {
        _XL_DELETE((uint8_t)(ROAD_L + i), YSize / 2);
    }

    slowdown = _XL_SLOW_DOWN_FACTOR;

    while (!game_over) {
        uint16_t new_player_x;
        uint8_t player_left_col;
        uint8_t player_right_col;
        uint8_t py;
        uint8_t ok;

        frame++;

        if (!(frame & 7)) {
            score++;
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINTD(SCORE_X, YSize / 2, 4, score);

            if (slowdown > _XL_SLOW_DOWN_FACTOR / 4) {
                slowdown -= _XL_SLOW_DOWN_FACTOR / 100;
            }
#if !defined(_XL_SLOW_DOWN_FACTOR)
            else if (slowdown > _XL_SLOW_DOWN / 8) {
                --slowdown;
            }
#endif
        }

        /* Move obstacle blocks. */
        if (frame % 4 == 0) {
            for (i = block_count; i > 0; i--) {
                uint8_t idx;
                idx = i - 1;

                if (blocks[idx].entered < blocks[idx].height) {
                    _XL_DRAW(blocks[idx].x, blocks[idx].entered, OBSTACLE_TILE, _XL_YELLOW);
                    blocks[idx].entered++;
                } else {
                    uint8_t old_top;
                    uint8_t new_bottom;

                    old_top = blocks[idx].top;
                    new_bottom = (uint8_t)(old_top + blocks[idx].height);

                    _XL_DELETE(blocks[idx].x, old_top);

                    if (new_bottom < YSize) {
                        _XL_DRAW(blocks[idx].x, new_bottom, OBSTACLE_TILE, _XL_YELLOW);
                    }

                    blocks[idx].top++;
                }

                if (blocks[idx].entered >= blocks[idx].height && blocks[idx].top >= YSize) {
                    blocks[idx] = blocks[block_count - 1];
                    block_count--;
                }
            }
        }

        /* Spawn obstacle blocks. */
        if (frame % 60 == 0) {
            uint16_t r;
            r = _XL_RAND();

            if (r % 2 == 0) {
                spawn_lateral_pair(blocks, &block_count);
            } else {
                spawn_central(blocks, &block_count);
            }
        }

        /* Spawn enemies. */
        if (frame % 8 == 0 && enemy_count < MAX_ENEMIES) {
            uint16_t r;
            uint8_t w, h, x, max_x, ok_spawn;

            r = _XL_RAND();

            if (r % 5 == 0) {
                w = 2;
                h = 3;
            } else {
                w = 1;
                h = 2;
            }

            max_x = (uint8_t)(ROAD_R - w + 1);
            x = (uint8_t)(ROAD_L + _XL_RAND() % (max_x - ROAD_L + 1));

            ok_spawn = 1;
            {
                uint8_t m, n;
                for (n = 0; n < h && ok_spawn; n++) {
                    for (m = 0; m < w && ok_spawn; m++) {
                        if (is_blocked((uint8_t)(x + m), n, blocks, block_count)) {
                            ok_spawn = 0;
                        }
                    }
                }
            }

            if (ok_spawn) {
                enemies[enemy_count].x = x;
                enemies[enemy_count].y = 0;
                enemies[enemy_count].w = w;
                enemies[enemy_count].h = h;
                enemy_count++;
            }
        }

        /* Move enemies. */
        for (i = enemy_count; i > 0; i--) {
            uint8_t idx;
            uint8_t old_x, old_y;
            uint8_t new_x;
            uint8_t can_move_down;
            uint8_t can_move_diag;
            uint8_t do_diag;
            uint8_t dir;
            uint8_t m2, k2;

            idx = i - 1;

            if ((uint16_t)enemies[idx].y + enemies[idx].h >= YSize) {
                erase_car(&enemies[idx]);
                enemies[idx] = enemies[enemy_count - 1];
                enemy_count--;
                continue;
            }

            old_x = enemies[idx].x;
            old_y = enemies[idx].y;

            if (frame % 12 == 0 && _XL_RAND() % 3 == 0 && old_y < YSize - 6) {
                do_diag = 1;
                dir = (uint8_t)(_XL_RAND() % 2);
            } else {
                do_diag = 0;
                dir = 0;
            }

            can_move_down = 1;
            for (k2 = 0; k2 < enemies[idx].h && can_move_down; k2++) {
                uint8_t ny;
                ny = (uint8_t)(old_y + 1 + k2);

                if (ny >= YSize) {
                    can_move_down = 0;
                    break;
                }

                for (m2 = 0; m2 < enemies[idx].w && can_move_down; m2++) {
                    if (is_blocked((uint8_t)(old_x + m2), ny, blocks, block_count)) {
                        can_move_down = 0;
                    }
                }
            }

            new_x = old_x;
            if (do_diag) {
                if (dir == 0 && old_x > ROAD_L) {
                    new_x = (uint8_t)(old_x - 1);
                } else if (dir == 1 && (uint16_t)old_x + enemies[idx].w <= ROAD_R) {
                    new_x = (uint8_t)(old_x + 1);
                }
            }

            can_move_diag = 0;
            if (do_diag && new_x != old_x) {
                can_move_diag = 1;

                for (k2 = 0; k2 < enemies[idx].h && can_move_diag; k2++) {
                    uint8_t ny2;
                    ny2 = (uint8_t)(old_y + 1 + k2);

                    if (ny2 >= YSize) {
                        can_move_diag = 0;
                        break;
                    }

                    for (m2 = 0; m2 < enemies[idx].w && can_move_diag; m2++) {
                        if (is_blocked((uint8_t)(new_x + m2), ny2, blocks, block_count)) {
                            can_move_diag = 0;
                        }
                    }
                }
            }

            erase_car(&enemies[idx]);

            if (can_move_down && do_diag && can_move_diag) {
                enemies[idx].x = new_x;
                enemies[idx].y = (uint8_t)(old_y + 1);
            } else if (can_move_down) {
                enemies[idx].x = old_x;
                enemies[idx].y = (uint8_t)(old_y + 1);
            } else {
                uint8_t escaped;
                uint8_t esc_dir;
                uint8_t esc_x;
                uint8_t m3, k3, e2;

                escaped = 0;
                esc_dir = (uint8_t)(_XL_RAND() % 2);

                if (esc_dir == 0 && old_x > ROAD_L) {
                    esc_x = (uint8_t)(old_x - 1);
                    can_move_diag = 1;

                    for (k3 = 0; k3 < enemies[idx].h && can_move_diag; k3++) {
                        uint8_t ny3;
                        ny3 = (uint8_t)(old_y + k3);

                        if (ny3 >= YSize) {
                            can_move_diag = 0;
                            break;
                        }

                        for (m3 = 0; m3 < enemies[idx].w && can_move_diag; m3++) {
                            if (is_blocked((uint8_t)(esc_x + m3), ny3, blocks, block_count)) {
                                can_move_diag = 0;
                            }
                        }
                    }

                    if (can_move_diag) {
                        for (e2 = 0; e2 < enemy_count && can_move_diag; e2++) {
                            if (e2 == idx) continue;

                            if (cars_overlap(esc_x, old_y, enemies[idx].w, enemies[idx].h,
                                             enemies[e2].x, enemies[e2].y,
                                             enemies[e2].w, enemies[e2].h)) {
                                can_move_diag = 0;
                            }
                        }
                    }

                    if (can_move_diag) escaped = 1;
                }

                if (!escaped && (uint16_t)old_x + enemies[idx].w <= ROAD_R) {
                    esc_x = (uint8_t)(old_x + 1);
                    can_move_diag = 1;

                    for (k3 = 0; k3 < enemies[idx].h && can_move_diag; k3++) {
                        uint8_t ny3;
                        ny3 = (uint8_t)(old_y + k3);

                        if (ny3 >= YSize) {
                            can_move_diag = 0;
                            break;
                        }

                        for (m3 = 0; m3 < enemies[idx].w && can_move_diag; m3++) {
                            if (is_blocked((uint8_t)(esc_x + m3), ny3, blocks, block_count)) {
                                can_move_diag = 0;
                            }
                        }
                    }

                    if (can_move_diag) {
                        for (e2 = 0; e2 < enemy_count && can_move_diag; e2++) {
                            if (e2 == idx) continue;

                            if (cars_overlap(esc_x, old_y, enemies[idx].w, enemies[idx].h,
                                             enemies[e2].x, enemies[e2].y,
                                             enemies[e2].w, enemies[e2].h)) {
                                can_move_diag = 0;
                            }
                        }
                    }

                    if (can_move_diag) escaped = 1;
                }

                if (!escaped && esc_dir == 1 && old_x > ROAD_L) {
                    esc_x = (uint8_t)(old_x - 1);
                    can_move_diag = 1;

                    for (k3 = 0; k3 < enemies[idx].h && can_move_diag; k3++) {
                        uint8_t ny3;
                        ny3 = (uint8_t)(old_y + k3);

                        if (ny3 >= YSize) {
                            can_move_diag = 0;
                            break;
                        }

                        for (m3 = 0; m3 < enemies[idx].w && can_move_diag; m3++) {
                            if (is_blocked((uint8_t)(esc_x + m3), ny3, blocks, block_count)) {
                                can_move_diag = 0;
                            }
                        }
                    }

                    if (can_move_diag) {
                        for (e2 = 0; e2 < enemy_count && can_move_diag; e2++) {
                            if (e2 == idx) continue;

                            if (cars_overlap(esc_x, old_y, enemies[idx].w, enemies[idx].h,
                                             enemies[e2].x, enemies[e2].y,
                                             enemies[e2].w, enemies[e2].h)) {
                                can_move_diag = 0;
                            }
                        }
                    }

                    if (can_move_diag) escaped = 1;
                }

                if (escaped) {
                    enemies[idx].x = esc_x;
                    enemies[idx].y = old_y;
                } else {
                    enemies[idx].x = old_x;
                    enemies[idx].y = old_y;
                }
            }

            if (enemies[idx].w == 2) {
                draw_car(&enemies[idx], BIG_ENEMY_TOP, BIG_ENEMY_BOT);
            } else {
                draw_car(&enemies[idx], ENEMY_TOP, ENEMY_BOT);
            }
        }

        /* Player input: move by one half-tile. */
        input = _XL_INPUT();

        if (_XL_LEFT(input) && player_x > PLAYER_MIN_X) {
            new_player_x = player_x - 1;
            ok = 1;

            player_left_col = get_left_col(new_player_x);
            player_right_col = get_right_col(new_player_x);

            /* Check enemy collision for all columns the player would occupy. */
            for (k = 0; k < enemy_count && ok; k++) {
                if (player_left_col == player_right_col) {
                    if (cars_overlap(player_left_col, PLAYER_Y, 1, 2,
                                     enemies[k].x, enemies[k].y,
                                     enemies[k].w, enemies[k].h)) {
                        ok = 0;
                    }
                } else {
                    if (cars_overlap(player_left_col, PLAYER_Y, 1, 2,
                                     enemies[k].x, enemies[k].y,
                                     enemies[k].w, enemies[k].h) ||
                        cars_overlap(player_right_col, PLAYER_Y, 1, 2,
                                     enemies[k].x, enemies[k].y,
                                     enemies[k].w, enemies[k].h)) {
                        ok = 0;
                    }
                }
            }

            /* Check block collision for all columns the player would occupy. */
            if (ok) {
                py = PLAYER_Y;
                if (player_left_col == player_right_col) {
                    if (is_blocked(player_left_col, py, blocks, block_count)) ok = 0;
                    else if (is_blocked(player_left_col, (uint8_t)(py + 1), blocks, block_count)) ok = 0;
                } else {
                    if (is_blocked(player_left_col, py, blocks, block_count) ||
                        is_blocked(player_right_col, py, blocks, block_count)) {
                        ok = 0;
                    } else if (is_blocked(player_left_col, (uint8_t)(py + 1), blocks, block_count) ||
                               is_blocked(player_right_col, (uint8_t)(py + 1), blocks, block_count)) {
                        ok = 0;
                    }
                }
            }

            if (ok) {
                erase_player(player_x);
                player_x = new_player_x;
                draw_player(player_x);
            }
        } else if (_XL_RIGHT(input) && player_x < PLAYER_MAX_X) {
            /*
             * Prevent an odd half-tile from drawing past the old right limit.
             */
            if (player_x + 1 <= PLAYER_MAX_ODD_X || ((player_x + 1) & 1u) == 0u) {
                new_player_x = player_x + 1;
                ok = 1;

                player_left_col = get_left_col(new_player_x);
                player_right_col = get_right_col(new_player_x);

                /* Check enemy collision for all columns the player would occupy. */
                for (k = 0; k < enemy_count && ok; k++) {
                    if (player_left_col == player_right_col) {
                        if (cars_overlap(player_left_col, PLAYER_Y, 1, 2,
                                         enemies[k].x, enemies[k].y,
                                         enemies[k].w, enemies[k].h)) {
                            ok = 0;
                        }
                    } else {
                        if (cars_overlap(player_left_col, PLAYER_Y, 1, 2,
                                         enemies[k].x, enemies[k].y,
                                         enemies[k].w, enemies[k].h) ||
                            cars_overlap(player_right_col, PLAYER_Y, 1, 2,
                                         enemies[k].x, enemies[k].y,
                                         enemies[k].w, enemies[k].h)) {
                            ok = 0;
                        }
                    }
                }

                /* Check block collision for all columns the player would occupy. */
                if (ok) {
                    py = PLAYER_Y;
                    if (player_left_col == player_right_col) {
                        if (is_blocked(player_left_col, py, blocks, block_count)) ok = 0;
                        else if (is_blocked(player_left_col, (uint8_t)(py + 1), blocks, block_count)) ok = 0;
                    } else {
                        if (is_blocked(player_left_col, py, blocks, block_count) ||
                            is_blocked(player_right_col, py, blocks, block_count)) {
                            ok = 0;
                        } else if (is_blocked(player_left_col, (uint8_t)(py + 1), blocks, block_count) ||
                                   is_blocked(player_right_col, (uint8_t)(py + 1), blocks, block_count)) {
                            ok = 0;
                        }
                    }
                }

                if (ok) {
                    erase_player(player_x);
                    player_x = new_player_x;
                    draw_player(player_x);
                }
            }
        }

        /* Player collision with enemies. */
        for (i = 0; i < enemy_count && !game_over; i++) {
            player_left_col = get_left_col(player_x);
            player_right_col = get_right_col(player_x);

            if (player_left_col == player_right_col) {
                if (cars_overlap(player_left_col, PLAYER_Y, 1, 2,
                                 enemies[i].x, enemies[i].y,
                                 enemies[i].w, enemies[i].h)) {
                    game_over = 1;
                }
            } else {
                if (cars_overlap(player_left_col, PLAYER_Y, 1, 2,
                                 enemies[i].x, enemies[i].y,
                                 enemies[i].w, enemies[i].h) ||
                    cars_overlap(player_right_col, PLAYER_Y, 1, 2,
                                 enemies[i].x, enemies[i].y,
                                 enemies[i].w, enemies[i].h)) {
                    game_over = 1;
                }
            }
        }

        /* Player collision with blocks. */
        for (k = 0; k < 2 && !game_over; k++) {
            player_left_col = get_left_col(player_x);
            player_right_col = get_right_col(player_x);

            py = (uint8_t)(PLAYER_Y + k);

            if (player_left_col == player_right_col) {
                if (is_blocked(player_left_col, py, blocks, block_count)) {
                    game_over = 1;
                }
            } else {
                if (is_blocked(player_left_col, py, blocks, block_count) ||
                    is_blocked(player_right_col, py, blocks, block_count)) {
                    game_over = 1;
                }
            }
        }

        if (game_over) {
            uint8_t e;

            for (e = 0; e < enemy_count; e++) {
                erase_car(&enemies[e]);
            }

            erase_player(player_x);

            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(ROAD_L, YSize / 2, "GAME OVER");
            _XL_SLEEP(2);

            for (i = 0; i < ROAD_W; i++) {
                _XL_DELETE((uint8_t)(ROAD_L + i), YSize / 2);
            }
        }

        _XL_SLOW_DOWN(slowdown);
    }

    erase_borders();
}

int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        game_loop();
    }

    return 0;
}
