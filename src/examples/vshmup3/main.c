#include "cross_lib.h"

#define MAX_ENEMIES       5
#define MAX_BULLETS      10
#define PLAYER_Y         (YSize - 2)
#define ENEMY_SPEED      3
#define SHOOT_INTERVAL   8
#define BULLET_SPEED     2
#define ENEMY_DROP_EVERY 60

void draw_player(uint8_t vx, uint8_t py);
void erase_player_col(uint8_t col, uint8_t py);
void draw_enemy(uint8_t vx, uint8_t ey);
void erase_enemy(uint8_t vx, uint8_t ey);
void draw_bullet(uint8_t bx, uint8_t by);
void erase_bullet(uint8_t bx, uint8_t by);


/* ---------- helpers ---------- */

void draw_player(uint8_t vx, uint8_t py) {
    if (vx & 1) {
        _XL_DRAW(vx / 2,     py,     _TILE_3, _XL_CYAN);
        _XL_DRAW(vx / 2,     py + 1, _TILE_4, _XL_CYAN);
        _XL_DRAW(vx / 2 + 1, py,     _TILE_5, _XL_CYAN);
        _XL_DRAW(vx / 2 + 1, py + 1, _TILE_6, _XL_CYAN);
    } else {
        _XL_DRAW(vx / 2,     py,     _TILE_3, _XL_CYAN);
        _XL_DRAW(vx / 2,     py + 1, _TILE_4, _XL_CYAN);
    }
}

void erase_player_col(uint8_t col, uint8_t py) {
    _XL_DELETE(col, py);
    _XL_DELETE(col, py + 1);
}

void draw_enemy(uint8_t vx, uint8_t ey) {
    if (vx & 1) {
        _XL_DRAW(vx / 2,     ey, _TILE_7, _XL_RED);
        _XL_DRAW(vx / 2 + 1, ey, _TILE_8, _XL_RED);
        _XL_DRAW(vx / 2 + 2, ey, _TILE_9, _XL_RED);
    } else {
        _XL_DRAW(vx / 2,     ey, _TILE_7, _XL_RED);
        _XL_DRAW(vx / 2 + 1, ey, _TILE_8, _XL_RED);
    }
}

void erase_enemy(uint8_t vx, uint8_t ey) {
    if (vx & 1) {
        _XL_DELETE(vx / 2,     ey);
        _XL_DELETE(vx / 2 + 1, ey);
        _XL_DELETE(vx / 2 + 2, ey);
    } else {
        _XL_DELETE(vx / 2,     ey);
        _XL_DELETE(vx / 2 + 1, ey);
    }
}

void draw_bullet(uint8_t bx, uint8_t by) {
    if (bx < XSize && by < YSize) {
        _XL_DRAW(bx, by, _TILE_10, _XL_YELLOW);
    }
}

void erase_bullet(uint8_t bx, uint8_t by) {
    if (bx < XSize && by < YSize) {
        _XL_DELETE(bx, by);
    }
}


int main(void) {
    uint8_t running = 1;

    while (running) {
        /* ---- state ---- */
        uint8_t  p_vx       = XSize;
        uint8_t  prev_p_vx  = XSize;
        uint8_t  score      = 0;
        uint8_t  lives      = 3;

        uint8_t  e_active[MAX_ENEMIES];
        uint8_t  e_vx[MAX_ENEMIES];
        uint8_t  e_dir[MAX_ENEMIES];
        uint8_t  e_y[MAX_ENEMIES];
        uint16_t e_timer[MAX_ENEMIES];
        uint16_t e_drop_counter[MAX_ENEMIES];

        /* b_dir: 0 = enemy bullet (down), 1 = player bullet (up) */
        uint8_t  b_active[MAX_BULLETS];
        uint8_t  b_x[MAX_BULLETS];
        uint8_t  b_y[MAX_BULLETS];
        uint8_t  b_dir[MAX_BULLETS];

        uint8_t  i;
        uint8_t  input;
        uint8_t  hit;
        uint16_t t;

        /* ---- init ---- */
        _XL_INIT_GRAPHICS();
        _XL_INIT_INPUT();
        _XL_INIT_SOUND();
        _XL_CLEAR_SCREEN();

        for (i = 0; i < MAX_ENEMIES; i++) {
            e_active[i]       = 0;
            e_drop_counter[i] = 0;
        }
        for (i = 0; i < MAX_BULLETS; i++) {
            b_active[i] = 0;
        }

        /* ---- main loop ---- */
        while (lives > 0) {

            /* ===== input / player movement ===== */
            prev_p_vx = p_vx;
            input = _XL_INPUT();
            if (_XL_LEFT(input)) {
                if (p_vx > 0) p_vx--;
            } else if (_XL_RIGHT(input)) {
                if (p_vx < 2 * XSize - 2) p_vx++;
            }

            /* erase player tiles no longer covered */
            if (prev_p_vx != p_vx) {
                if ((prev_p_vx & 1) && !(p_vx & 1)) {
                    if (p_vx > prev_p_vx) {
                        erase_player_col(prev_p_vx / 2, PLAYER_Y);
                    } else {
                        erase_player_col(prev_p_vx / 2 + 1, PLAYER_Y);
                    }
                }
            }

            /* ===== player shooting ===== */
            if (_XL_FIRE(input)) {
                for (i = 0; i < MAX_BULLETS; i++) {
                    if (!b_active[i]) {
                        b_active[i] = 1;
                        b_x[i]     = p_vx / 2;
                        b_y[i]     = PLAYER_Y - 1;
                        b_dir[i]   = 1;
                        _XL_SHOOT_SOUND();
                        break;
                    }
                }
            }

            /* ===== spawn enemies ===== */
            for (i = 0; i < MAX_ENEMIES; i++) {
                if (!e_active[i]) {
                    t = _XL_RAND();
                    if ((t % 100) < 2) {
                        e_active[i]       = 1;
                        e_vx[i]           = (uint8_t)(_XL_RAND() % (2 * XSize - 3));
                        e_dir[i]          = (uint8_t)(_XL_RAND() & 1);
                        e_y[i]            = 0;
                        e_timer[i]        = SHOOT_INTERVAL + (uint16_t)(_XL_RAND() % SHOOT_INTERVAL);
                        e_drop_counter[i] = 0;
                    }
                }
            }

            /* ===== update enemies ===== */
            for (i = 0; i < MAX_ENEMIES; i++) {
                if (!e_active[i]) continue;

                e_drop_counter[i]++;

                if (e_drop_counter[i] >= ENEMY_DROP_EVERY) {
                    /* --- vertical step: erase old, increment y --- */
                    e_drop_counter[i] = 0;
                    erase_enemy(e_vx[i], e_y[i]);
                    e_y[i]++;

                    if (e_y[i] >= YSize - 2) {
                        e_active[i] = 0;
                        continue;
                    }
                } else {
                    /* --- horizontal move: erase old, change vx --- */
                    erase_enemy(e_vx[i], e_y[i]);

                    if (e_dir[i]) {
                        e_vx[i] += ENEMY_SPEED;
                        if (e_vx[i] >= 2 * XSize - 3) {
                            e_vx[i] = 2 * XSize - 3;
                            e_dir[i] = 0;
                        }
                    } else {
                        if (e_vx[i] < ENEMY_SPEED) {
                            e_vx[i] = 0;
                            e_dir[i] = 1;
                        } else {
                            e_vx[i] -= ENEMY_SPEED;
                        }
                    }
                }

                /* enemy shooting */
                e_timer[i]--;
                if (e_timer[i] == 0 && e_y[i] < PLAYER_Y - 2) {
                    for (t = 0; t < MAX_BULLETS; t++) {
                        if (!b_active[t]) {
                            b_active[t] = 1;
                            b_x[t]     = (uint8_t)(e_vx[i] / 2);
                            b_y[t]     = e_y[i] + 1;
                            b_dir[t]   = 0;
                            break;
                        }
                    }
                    e_timer[i] = SHOOT_INTERVAL + (uint16_t)(_XL_RAND() % SHOOT_INTERVAL);
                }
            }

            /* ===== update bullets ===== */
            for (i = 0; i < MAX_BULLETS; i++) {
                if (!b_active[i]) continue;
                erase_bullet(b_x[i], b_y[i]);

                if (b_dir[i] == 1) {
                    if (b_y[i] <= BULLET_SPEED) {
                        b_active[i] = 0;
                    } else {
                        b_y[i] -= BULLET_SPEED;
                    }
                } else {
                    b_y[i] += BULLET_SPEED;
                    if (b_y[i] >= YSize) {
                        b_active[i] = 0;
                    }
                }
            }

            /* ===== collision: player bullets vs enemies ===== */
            for (i = 0; i < MAX_BULLETS; i++) {
                if (!b_active[i] || b_dir[i] != 1) continue;
                for (t = 0; t < MAX_ENEMIES; t++) {
                    if (!e_active[t]) continue;

                    uint8_t e_left  = e_vx[t] / 2;
                    uint8_t e_right = (e_vx[t] & 1) ? (e_vx[t] / 2 + 2) : (e_vx[t] / 2 + 1);

                    if (b_x[i] >= e_left && b_x[i] <= e_right &&
                        b_y[i] == e_y[t]) {
                        erase_bullet(b_x[i], b_y[i]);
                        b_active[i] = 0;
                        erase_enemy(e_vx[t], e_y[t]);
                        e_active[t] = 0;
                        score++;
                        _XL_PING_SOUND();
                        break;
                    }
                }
            }

            /* ===== collision: enemy bullets vs player ===== */
            for (i = 0; i < MAX_BULLETS; i++) {
                if (!b_active[i] || b_dir[i] != 0) continue;
                hit = 0;
                if (p_vx & 1) {
                    if ((b_x[i] == p_vx / 2 || b_x[i] == p_vx / 2 + 1) &&
                        (b_y[i] >= PLAYER_Y && b_y[i] <= PLAYER_Y + 1)) {
                        hit = 1;
                    }
                } else {
                    if (b_x[i] == p_vx / 2 &&
                        (b_y[i] >= PLAYER_Y && b_y[i] <= PLAYER_Y + 1)) {
                        hit = 1;
                    }
                }
                if (hit) {
                    erase_bullet(b_x[i], b_y[i]);
                    b_active[i] = 0;
                    lives--;
                    _XL_EXPLOSION_SOUND();
                }
            }

            /* ===== collision: enemies vs player (contact) ===== */
            for (i = 0; i < MAX_ENEMIES; i++) {
                if (!e_active[i]) continue;
                hit = 0;
                uint8_t e_left  = e_vx[i] / 2;
                uint8_t e_right = (e_vx[i] & 1) ? (e_vx[i] / 2 + 2) : (e_vx[i] / 2 + 1);

                if (p_vx & 1) {
                    uint8_t p_left  = p_vx / 2;
                    uint8_t p_right = p_vx / 2 + 1;
                    if ((e_left <= p_right && e_right >= p_left) &&
                        (e_y[i] >= PLAYER_Y && e_y[i] <= PLAYER_Y + 1)) {
                        hit = 1;
                    }
                } else {
                    uint8_t p_col = p_vx / 2;
                    if ((e_left <= p_col && e_right >= p_col) &&
                        (e_y[i] >= PLAYER_Y && e_y[i] <= PLAYER_Y + 1)) {
                        hit = 1;
                    }
                }

                if (hit) {
                    erase_enemy(e_vx[i], e_y[i]);
                    e_active[i] = 0;
                    lives--;
                    _XL_EXPLOSION_SOUND();
                }
            }

            /* ===== render: player ===== */
            draw_player(p_vx, PLAYER_Y);

            /* ===== render: enemies ===== */
            for (i = 0; i < MAX_ENEMIES; i++) {
                if (!e_active[i]) continue;
                draw_enemy(e_vx[i], e_y[i]);
            }

            /* ===== render: bullets ===== */
            for (i = 0; i < MAX_BULLETS; i++) {
                if (!b_active[i]) continue;
                draw_bullet(b_x[i], b_y[i]);
            }

            /* ===== HUD ===== */
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINTD(0, 0, 1, score);
            _XL_PRINTD(XSize - 3, 0, 1, lives);

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* ---- game over screen ---- */
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(5, YSize / 2 - 2, "GAME OVER");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0, YSize / 2 + 1, 1, score);
        _XL_WAIT_FOR_INPUT();

        /* loop restarts */
    }

    return 0;
}
