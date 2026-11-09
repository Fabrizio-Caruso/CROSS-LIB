#include "cross_lib.h"

/* Screen state tracking */
static uint8_t scr_used[XSize][YSize];
static uint8_t scr_tile[XSize][YSize];
static uint8_t scr_color[XSize][YSize];

#define MAX_INV 20
#define MAX_PB 3
#define MAX_EB 3

static uint8_t player_x;
static uint8_t player_y;
static uint8_t lives;
static uint16_t score;
static uint8_t game_over;
static uint8_t wave;

static uint8_t inv_x[MAX_INV];
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

static void init_wave(void) {
    uint8_t i, row, col;
    for (i = 0; i < MAX_INV; i++) {
        inv_alive[i] = 0;
    }
    i = 0;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 5; col++) {
            inv_x[i] = (uint8_t)(4 + col * 6);
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
    player_x = (uint8_t)(XSize / 2);
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

static void draw_invader(uint8_t idx) {
    draw_cell(inv_x[idx], inv_y[idx], _TILE_2, _XL_CYAN);
}

static void draw_all_invaders(void) {
    uint8_t i;
    for (i = 0; i < MAX_INV; i++) {
        if (inv_alive[i]) draw_invader(i);
    }
}

int main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        /* Initialize game */
        init_game();
        _XL_CLEAR_SCREEN();
        reset_screen();

        /* Draw initial state */
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 0, "SCORE");
        _XL_PRINTD(6, 0, 1, score);
        _XL_PRINTD(15, 0, 1, lives);
        draw_cell(player_x, player_y, _TILE_1, _XL_GREEN);
        draw_all_invaders();

        /* Main game loop */
        while (!game_over) {
            uint8_t inp = _XL_INPUT();
            uint8_t i, j;

            /* Player movement */
            if (_XL_LEFT(inp) && player_x > 0) {
                erase_cell(player_x, player_y);
                player_x--;
                draw_cell(player_x, player_y, _TILE_1, _XL_GREEN);
            }
            if (_XL_RIGHT(inp) && player_x < (uint8_t)(XSize - 1)) {
                erase_cell(player_x, player_y);
                player_x++;
                draw_cell(player_x, player_y, _TILE_1, _XL_GREEN);
            }

            /* Player fire */
            if (_XL_FIRE(inp)) {
                for (i = 0; i < MAX_PB; i++) {
                    if (!pb_active[i]) {
                        pb_active[i] = 1;
                        pb_x[i] = player_x;
                        pb_y[i] = (uint8_t)(player_y - 1);
                        draw_cell(pb_x[i], pb_y[i], _TILE_3, _XL_YELLOW);
                        _XL_SHOOT_SOUND();
                        break;
                    }
                }
            }

            /* Move player bullets up */
            for (i = 0; i < MAX_PB; i++) {
                if (pb_active[i]) {
                    erase_cell(pb_x[i], pb_y[i]);
                    if (pb_y[i] > 0) {
                        pb_y[i]--;
                        draw_cell(pb_x[i], pb_y[i], _TILE_3, _XL_YELLOW);
                    } else {
                        pb_active[i] = 0;
                    }
                }
            }

            /* Invader movement timer */
            inv_move_t++;
            if (inv_move_t >= 15) {
                inv_move_t = 0;

                /* Check if all invaders dead */
                if (count_invaders() == 0) {
                    wave++;
                    /* Erase all invader tiles */
                    for (i = 0; i < MAX_INV; i++) {
                        erase_cell(inv_x[i], inv_y[i]);
                    }
                    init_wave();
                    draw_all_invaders();
                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    continue;
                }

                /* Check bounds */
                uint8_t need_down = 0;
                for (i = 0; i < MAX_INV; i++) {
                    if (inv_alive[i]) {
                        if (inv_dir && inv_x[i] + 1 >= (uint8_t)XSize) need_down = 1;
                        if (!inv_dir && inv_x[i] == 0) need_down = 1;
                    }
                }

                if (need_down) {
                    inv_dir = (uint8_t)(1 - inv_dir);
                    for (i = 0; i < MAX_INV; i++) {
                        if (inv_alive[i]) {
                            erase_cell(inv_x[i], inv_y[i]);
                            inv_y[i]++;
                            draw_cell(inv_x[i], inv_y[i], _TILE_2, _XL_CYAN);
                        }
                    }
                } else {
                    for (i = 0; i < MAX_INV; i++) {
                        if (inv_alive[i]) {
                            erase_cell(inv_x[i], inv_y[i]);
                            if (inv_dir) inv_x[i]++;
                            else inv_x[i]--;
                            draw_cell(inv_x[i], inv_y[i], _TILE_2, _XL_CYAN);
                        }
                    }
                }
                _XL_TICK_SOUND();
            }

            /* Invader fire timer */
            inv_fire_t++;
            if (inv_fire_t >= 40) {
                inv_fire_t = 0;
                /* Find a random alive invader */
                uint8_t idx = (uint8_t)(_XL_RAND() % MAX_INV);
                if (inv_alive[idx]) {
                    for (j = 0; j < MAX_EB; j++) {
                        if (!eb_active[j]) {
                            eb_active[j] = 1;
                            eb_x[j] = inv_x[idx];
                            eb_y[j] = (uint8_t)(inv_y[idx] + 1);
                            draw_cell(eb_x[j], eb_y[j], _TILE_4, _XL_RED);
                            break;
                        }
                    }
                }
            }

            /* Move enemy bullets down */
            for (i = 0; i < MAX_EB; i++) {
                if (eb_active[i]) {
                    erase_cell(eb_x[i], eb_y[i]);
                    if (eb_y[i] < (uint8_t)(YSize - 1)) {
                        eb_y[i]++;
                        draw_cell(eb_x[i], eb_y[i], _TILE_4, _XL_RED);
                    } else {
                        eb_active[i] = 0;
                    }
                    /* Check collision with player */
                    if (eb_x[i] == player_x && eb_y[i] == player_y) {
                        eb_active[i] = 0;
                        erase_cell(eb_x[i], eb_y[i]);
                        erase_cell(player_x, player_y);
                        lives--;
                        _XL_EXPLOSION_SOUND();
                        if (lives <= 0) {
                            game_over = 1;
                        } else {
                            draw_cell(player_x, player_y, _TILE_1, _XL_GREEN);
                        }
                    }
                }
            }

            /* Check player bullet vs invader collisions */
            for (i = 0; i < MAX_PB; i++) {
                if (pb_active[i]) {
                    for (j = 0; j < MAX_INV; j++) {
                        if (inv_alive[j] && pb_x[i] == inv_x[j] && pb_y[i] == inv_y[j]) {
                            pb_active[i] = 0;
                            inv_alive[j] = 0;
                            erase_cell(pb_x[i], pb_y[i]);
                            erase_cell(inv_x[j], inv_y[j]);
                            score += 10;
                            _XL_PING_SOUND();
                            break;
                        }
                    }
                }
            }

            /* Check if invaders reached player row */
            for (i = 0; i < MAX_INV; i++) {
                if (inv_alive[i] && inv_y[i] >= player_y) {
                    game_over = 1;
                }
            }

            /* Update score/lives display */
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

        /* Game over screen */
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(10, 10, "GAME OVER");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(10, 12, "PRESS FIRE");

        /* Wait for restart */
        {
            uint8_t inp;
            while (1) {
                inp = _XL_INPUT();
                if (_XL_FIRE(inp)) break;
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
        }

        /* Clear screen for restart */
        _XL_CLEAR_SCREEN();
        reset_screen();
    }

    return 0;
}
