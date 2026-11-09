#include "cross_lib.h"

/* Screen state tracking (indexed by real tile positions) */
static uint8_t scr_used[XSize][YSize];
static uint8_t scr_tile[XSize][YSize];
static uint8_t scr_color[XSize][YSize];

#define MAX_INV 20
#define MAX_PB 3
#define MAX_EB 3


#define _TILE_PLAYER_EVEN_L _TILE_0
#define _TILE_PLAYER_EVEN_R _TILE_1

#define _TILE_PLAYER_ODD_L  _TILE_2
#define _TILE_PLAYER_ODD_R  _TILE_3

#define _TILE_INV_EVEN_L    _TILE_4
#define _TILE_INV_EVEN_R    _TILE_5

#define _TILE_INV_ODD_L     _TILE_6
#define _TILE_INV_ODD_R     _TILE_7

#define _TILE_PLAYER_BULLET _TILE_8
#define _TILE_ENEMY_BULLET  _TILE_9


static uint8_t player_x;   /* virtual x */
static uint8_t player_y;
static uint8_t lives;
static uint16_t score;
static uint8_t game_over;
static uint8_t wave;

static uint8_t inv_x[MAX_INV];   /* virtual x */
static uint8_t inv_y[MAX_INV];
static uint8_t inv_alive[MAX_INV];
static uint8_t inv_dir;
static uint8_t inv_move_t;
static uint8_t inv_fire_t;

static uint8_t pb_x[MAX_PB];
static uint8_t pb_y[MAX_PB];
static uint8_t pb_active[MAX_PB];

static uint8_t eb_x[MAX_EB];
static uint8_t eb_y[MAX_EB];
static uint8_t eb_active[MAX_EB];

static uint16_t disp_score;
static uint8_t disp_lives;

/* ---- single-tile helpers ---- */

static void draw_cell(uint8_t x, uint8_t y, uint8_t tile, uint8_t color) {
    if (x >= (uint8_t)XSize || y >= (uint8_t)YSize) return;
    if (scr_used[x][y] && scr_tile[x][y] == tile && scr_color[x][y] == color) return;
    if (scr_used[x][y]) _XL_DELETE(x, y);
    _XL_DRAW(x, y, tile, color);
    scr_used[x][y] = 1;
    scr_tile[x][y] = tile;
    scr_color[x][y] = color;
}

static void erase_cell(uint8_t x, uint8_t y) {
    if (x >= (uint8_t)XSize || y >= (uint8_t)YSize) return;
    if (scr_used[x][y]) {
        _XL_DELETE(x, y);
        scr_used[x][y] = 0;
    }
}

static void reset_screen(void) {
    uint8_t x, y;
    for (x = 0; x < (uint8_t)XSize; x++) {
        for (y = 0; y < (uint8_t)YSize; y++) {
            scr_used[x][y] = 0;
        }
    }
}

/* ---- 2-tile entity helpers (virtual x -> two real tiles) ---- */

static void draw_player(uint8_t vx, uint8_t y) {
    uint8_t rx = (uint8_t)(vx / 2);
    if (vx % 2 == 0) {
        draw_cell(rx, y, _TILE_PLAYER_EVEN_L, _XL_GREEN);
        draw_cell((uint8_t)(rx + 1), y, _TILE_PLAYER_EVEN_R, _XL_GREEN);
    } else {
        draw_cell(rx, y, _TILE_PLAYER_ODD_L, _XL_GREEN);
        draw_cell((uint8_t)(rx + 1), y, _TILE_PLAYER_ODD_R, _XL_GREEN);
    }
}

static void erase_player(uint8_t vx, uint8_t y) {
    uint8_t rx = (uint8_t)(vx / 2);
    erase_cell(rx, y);
    erase_cell((uint8_t)(rx + 1), y);
}

static void draw_invader(uint8_t idx) {
    uint8_t vx = inv_x[idx];
    uint8_t y  = inv_y[idx];
    uint8_t rx = (uint8_t)(vx / 2);
    if (vx % 2 == 0) {
        draw_cell(rx, y, _TILE_INV_EVEN_L, _XL_CYAN);
        draw_cell((uint8_t)(rx + 1), y, _TILE_INV_EVEN_R, _XL_CYAN);
    } else {
        draw_cell(rx, y, _TILE_INV_ODD_L, _XL_CYAN);
        draw_cell((uint8_t)(rx + 1), y, _TILE_INV_ODD_R, _XL_CYAN);
    }
}

static void erase_invader(uint8_t idx) {
    uint8_t vx = inv_x[idx];
    uint8_t y  = inv_y[idx];
    uint8_t rx = (uint8_t)(vx / 2);
    erase_cell(rx, y);
    erase_cell((uint8_t)(rx + 1), y);
}

static void draw_all_invaders(void) {
    uint8_t i;
    for (i = 0; i < MAX_INV; i++) {
        if (inv_alive[i]) draw_invader(i);
    }
}

/* ---- game setup ---- */

static void init_wave(void) {
    uint8_t i, row, col;
    for (i = 0; i < MAX_INV; i++) {
        inv_alive[i] = 0;
    }
    i = 0;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 5; col++) {
            inv_x[i] = (uint8_t)(2 + col * 6);   /* virtual x; real tiles at x/2 and x/2+1 */
            inv_y[i] = (uint8_t)(2 + row * 3);
            inv_alive[i] = 1;
            i++;
        }
    }
    inv_dir = 1;
    inv_move_t = 0;
    inv_fire_t = 0;
    for (i = 0; i < MAX_PB; i++) pb_active[i] = 0;
    for (i = 0; i < MAX_EB; i++) eb_active[i] = 0;
}

static void init_game(void) {
    lives = 3;
    score = 0;
    wave = 1;
    player_x = (uint8_t)XSize;          /* virtual x; real tiles at XSize/2 and XSize/2+1 */
    player_y = (uint8_t)(YSize - 2);
    game_over = 0;
    disp_score = 0;
    disp_lives = 0;
    init_wave();
}

static uint8_t count_invaders(void) {
    uint8_t i, c;
    c = 0;
    for (i = 0; i < MAX_INV; i++) {
        if (inv_alive[i]) c++;
    }
    return c;
}

/* ---- main loop ---- */

int main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        init_game();
        _XL_CLEAR_SCREEN();
        reset_screen();

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 0, "SCORE");
        _XL_PRINTD(6, 0, 1, score);
        _XL_PRINTD(15, 0, 1, lives);
        draw_player(player_x, player_y);
        draw_all_invaders();

        /* main game loop */
        while (!game_over) {
            uint8_t inp = _XL_INPUT();
            uint8_t i, j;

            /* --- player movement --- */
            if (_XL_LEFT(inp) && player_x > 0) {
                erase_player(player_x, player_y);
                player_x--;
                draw_player(player_x, player_y);
            }
            if (_XL_RIGHT(inp)) {
                uint8_t new_rx = (uint8_t)((player_x + 1) / 2);
                if (new_rx + 1 < (uint8_t)XSize) {
                    erase_player(player_x, player_y);
                    player_x++;
                    draw_player(player_x, player_y);
                }
            }

            /* --- player fire --- */
            if (_XL_FIRE(inp)) {
                for (i = 0; i < MAX_PB; i++) {
                    if (!pb_active[i]) {
                        pb_active[i] = 1;
                        pb_x[i] = (uint8_t)(player_x / 2);
                        pb_y[i] = (uint8_t)(player_y - 1);
                        draw_cell(pb_x[i], pb_y[i], _TILE_PLAYER_BULLET, _XL_YELLOW);
                        _XL_SHOOT_SOUND();
                        break;
                    }
                }
            }

            /* --- move player bullets up --- */
            for (i = 0; i < MAX_PB; i++) {
                if (pb_active[i]) {
                    erase_cell(pb_x[i], pb_y[i]);
                    if (pb_y[i] > 0) {
                        pb_y[i]--;
                        draw_cell(pb_x[i], pb_y[i], _TILE_PLAYER_BULLET, _XL_YELLOW);
                    } else {
                        pb_active[i] = 0;
                    }
                }
            }

            /* --- invader movement timer --- */
            inv_move_t++;
            if (inv_move_t >= 15) {
                inv_move_t = 0;

                if (count_invaders() == 0) {
                    wave++;
                    for (i = 0; i < MAX_INV; i++) {
                        erase_invader(i);
                    }
                    init_wave();
                    draw_all_invaders();
                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    continue;
                }

                /* check whether any invader would go out of bounds */
                uint8_t need_down = 0;
                for (i = 0; i < MAX_INV; i++) {
                    if (inv_alive[i]) {
                        if (inv_dir) {
                            uint8_t nr = (uint8_t)((inv_x[i] + 1) / 2);
                            if (nr + 1 >= (uint8_t)XSize) need_down = 1;
                        } else {
                            if (inv_x[i] == 0) need_down = 1;
                        }
                    }
                }

                if (need_down) {
                    inv_dir = (uint8_t)(1 - inv_dir);
                    for (i = 0; i < MAX_INV; i++) {
                        if (inv_alive[i]) {
                            erase_invader(i);
                            inv_y[i]++;
                            draw_invader(i);
                        }
                    }
                } else {
                    for (i = 0; i < MAX_INV; i++) {
                        if (inv_alive[i]) {
                            erase_invader(i);
                            if (inv_dir) inv_x[i]++;
                            else         inv_x[i]--;
                            draw_invader(i);
                        }
                    }
                }
                _XL_TICK_SOUND();
            }

            /* --- invader fire: only from the lowest row --- */
            inv_fire_t++;
            if (inv_fire_t >= 40) {
                inv_fire_t = 0;

                /* find the lowest (max y) row among alive invaders */
                uint8_t max_y = 0;
                for (i = 0; i < MAX_INV; i++) {
                    if (inv_alive[i] && inv_y[i] > max_y) max_y = inv_y[i];
                }

                /* pick a random alive invader sitting on that row */
                uint8_t start = (uint8_t)(_XL_RAND() % MAX_INV);
                uint8_t idx   = 0;
                uint8_t found = 0;
                for (j = 0; j < MAX_INV && !found; j++) {
                    idx = (uint8_t)((start + j) % MAX_INV);
                    if (inv_alive[idx] && inv_y[idx] == max_y) found = 1;
                }

                if (found) {
                    for (j = 0; j < MAX_EB; j++) {
                        if (!eb_active[j]) {
                            eb_active[j] = 1;
                            eb_x[j] = (uint8_t)(inv_x[idx] / 2);
                            eb_y[j] = (uint8_t)(inv_y[idx] + 1);
                            draw_cell(eb_x[j], eb_y[j], _TILE_ENEMY_BULLET, _XL_RED);
                            break;
                        }
                    }
                }
            }

            /* --- move enemy bullets down --- */
            for (i = 0; i < MAX_EB; i++) {
                if (eb_active[i]) {
                    erase_cell(eb_x[i], eb_y[i]);
                    if (eb_y[i] < (uint8_t)(YSize - 1)) {
                        eb_y[i]++;
                        draw_cell(eb_x[i], eb_y[i], _TILE_ENEMY_BULLET, _XL_RED);
                    } else {
                        eb_active[i] = 0;
                    }

                    /* collision with player (both real tiles) */
                    uint8_t prx = (uint8_t)(player_x / 2);
                    if ((eb_x[i] == prx || eb_x[i] == (uint8_t)(prx + 1)) &&
                        eb_y[i] == player_y) {
                        eb_active[i] = 0;
                        erase_cell(eb_x[i], eb_y[i]);
                        erase_player(player_x, player_y);
                        lives--;
                        _XL_EXPLOSION_SOUND();
                        if (lives <= 0) {
                            game_over = 1;
                        } else {
                            draw_player(player_x, player_y);
                        }
                    }
                }
            }

            /* --- player bullet vs invader collision (both real tiles) --- */
            for (i = 0; i < MAX_PB; i++) {
                if (pb_active[i]) {
                    for (j = 0; j < MAX_INV; j++) {
                        if (inv_alive[j]) {
                            uint8_t irx = (uint8_t)(inv_x[j] / 2);
                            if ((pb_x[i] == irx || pb_x[i] == (uint8_t)(irx + 1)) &&
                                pb_y[i] == inv_y[j]) {
                                pb_active[i]  = 0;
                                inv_alive[j]   = 0;
                                erase_cell(pb_x[i], pb_y[i]);
                                erase_invader(j);
                                score += 10;
                                _XL_PING_SOUND();
                                break;
                            }
                        }
                    }
                }
            }

            /* --- invaders reached player row? --- */
            for (i = 0; i < MAX_INV; i++) {
                if (inv_alive[i] && inv_y[i] >= player_y) {
                    game_over = 1;
                }
            }

            /* --- update HUD --- */
            if (score != disp_score) {
                disp_score = score;
                _XL_SET_TEXT_COLOR(_XL_WHITE);
                _XL_PRINTD(6, 0, 1, score);
            }
            if (lives != disp_lives) {
                disp_lives = lives;
                _XL_SET_TEXT_COLOR(_XL_WHITE);
                _XL_PRINTD(15, 0, 1, lives);
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* game-over screen */
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(10, 10, "GAME OVER");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(10, 12, "PRESS FIRE");

        /* wait for restart */
        {
            uint8_t inp;
            while (1) {
                inp = _XL_INPUT();
                if (_XL_FIRE(inp)) break;
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
        }

        _XL_CLEAR_SCREEN();
        reset_screen();
    }

    return 0;
}


