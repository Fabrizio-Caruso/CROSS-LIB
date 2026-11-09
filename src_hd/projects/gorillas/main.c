#include "cross_lib.h"

/* ------------------------------------------------------------ */
/*  Helpers                                                      */
/* ------------------------------------------------------------ */

static void draw_gorilla(uint8_t x, uint8_t y, uint8_t color_id)
{
    /* Small two-pixel gorilla marker.
       Only draws if inside the screen. */
    if (x < XSize && y < YSize) {
        _XL_DRAW(x, y, _TILE_1, color_id);

        if ((y + 1) < YSize) {
            _XL_DRAW(x, (uint8_t)(y + 1), _TILE_1, color_id);
        }
    }
}

static void erase_gorilla(uint8_t x, uint8_t y)
{
    if (x < XSize && y < YSize) {
        _XL_DELETE(x, y);
    }

    if ((y + 1) < YSize) {
        _XL_DELETE(x, (uint8_t)(y + 1));
    }
}

static void draw_banana(uint8_t x, uint8_t y)
{
    if (x < XSize && y < YSize) {
        _XL_DRAW(x, y, _TILE_2, _XL_YELLOW);
    }
}

static void erase_banana(uint8_t x, uint8_t y)
{
    if (x < XSize && y < YSize) {
        _XL_DELETE(x, y);
    }
}

/* ------------------------------------------------------------ */
/*  Building generation and background drawing                   */
/* ------------------------------------------------------------ */

static void init_world(uint8_t heights[])
{
    uint8_t x;

    /* Create simple building tops.
       Keep some playable gaps near left/right edges. */
    for (x = 0; x < XSize; x++) {
        if ((x % 7) == 3) {
            /* valley / gap */
            heights[x] = (uint8_t)(YSize - 1);
        } else if ((x % 5) == 0) {
            /* tall building */
            heights[x] = (uint8_t)((YSize * 2) / 5);
        } else if ((x % 3) == 0) {
            /* medium building */
            heights[x] = (uint8_t)(YSize - 1 - YSize / 4);
        } else {
            /* low building or ground level */
            heights[x] = (uint8_t)(YSize - 1 - (YSize / 6));
        }

        if (heights[x] == 0) {
            heights[x] = 1;
        }
    }

    /* Force player start columns to be safe. */
    heights[1] = (uint8_t)(YSize - 2);
    heights[XSize - 2] = (uint8_t)(YSize - 2);
}

static void draw_background(uint8_t heights[])
{
    uint8_t x, y;

    _XL_CLEAR_SCREEN();

    /* Draw building bodies from the top height down to bottom. */
    for (x = 0; x < XSize; x++) {
        for (y = heights[x]; y < YSize; y++) {
            if ((y - heights[x]) == 0) {
                _XL_DRAW(x, y, _TILE_3, _XL_CYAN);   /* roof */
            } else {
                _XL_DRAW(x, y, _TILE_4, _XL_BLUE);    /* wall */
            }
        }
    }

    /* Draw a dark bottom line if there is room. */
    for (x = 0; x < XSize; x++) {
        if ((YSize - 1) >= heights[x]) {
            _XL_DRAW(x, (uint8_t)(YSize - 1), _TILE_5, _XL_MAGENTA);
        }
    }
}

/* ------------------------------------------------------------ */
/*  HUD                                                          */
/* ------------------------------------------------------------ */

static void draw_hud(uint16_t score, uint8_t lives)
{
    /* Score left, lives right. */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(1, 0, 3, score);

    if (XSize >= 8) {
        _XL_PRINTD((uint8_t)(XSize - 4), 0, 2, lives);
    } else {
        /* Very narrow fallback. */
        _XL_CHAR((uint8_t)(XSize - 1), 0, (char)('1' + (lives > 9 ? 9 : lives)));
    }
}

static void draw_message(const char *msg)
{
    uint8_t x;
    uint8_t len = 0;
    const char *p;

    for (p = msg; *p != '\0'; p++) {
        len++;
    }

    if ((uint16_t)len > XSize) {
        len = XSize;
    }

    x = (uint8_t)((XSize - len) / 2);

    if (x + len > XSize) {
        x = 0;
    }

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(x, (uint8_t)(YSize / 3), msg);
}

/* ------------------------------------------------------------ */
/*  Main                                                         */
/* ------------------------------------------------------------ */

int main(void)
{
    uint8_t heights[160];

    /* World / entity state. */
    uint8_t px;
    uint8_t py;
    uint8_t cx;
    uint8_t cy;
    uint8_t bx;
    uint8_t by;
    uint8_t ban_active;
    uint8_t aim_up;
    uint8_t power;
    uint8_t fire_prev;
    uint8_t player_turn;
    uint8_t turn_timer;

    /* Game state. */
    uint16_t score;
    uint8_t lives;
    uint8_t state;

    /* Input and math helpers. */
    uint8_t input;
    short bvx;
    short bvy;
    short ax;
    short ay;
    short min_y;
    short max_y;

    for (;;) {
        /* ------------------------- */
        /*  New game                 */
        /* ------------------------- */
        score = 0;
        lives = 3;

        init_world(heights);
        draw_background(heights);
        draw_hud(score, lives);

        px = 1;
        py = (uint8_t)(heights[px] - 2);
        if (py > YSize - 3) {
            py = YSize - 3;
        }

        cx = XSize - 2;
        cy = (uint8_t)(heights[cx] - 2);
        if (cy > YSize - 3) {
            cy = YSize - 3;
        }

        draw_gorilla(px, py, _XL_GREEN);
        draw_gorilla(cx, cy, _XL_RED);

        state = 1; /* start screen */

        while (state == 1) {
            draw_message("GORILLAS");
            draw_message("HOLD FIRE TO AIM");

            input = _XL_INPUT();

            if (_XL_FIRE(input)) {
                state = 2; /* play */
            } else if (_XL_LEFT(input) || _XL_RIGHT(input) ||
                       _XL_UP(input) || _XL_DOWN(input)) {
                state = 2;
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* ------------------------- */
        /*  Play loop                */
        /* ------------------------- */
        ban_active = 0;
        fire_prev = 0;
        player_turn = 1;

        while (state == 2) {
            input = _XL_INPUT();

            /* -------------------- */
            /*  Player movement     */
            /* -------------------- */
            if (!ban_active && player_turn) {
                if (_XL_LEFT(input)) {
                    if (px > 0) {
                        px--;
                        erase_gorilla((uint8_t)(px + 1), py);
                        draw_gorilla(px, py, _XL_GREEN);
                    }
                }

                if (_XL_RIGHT(input)) {
                    /* Do not walk into right half / CPU side too much. */
                    max_y = (short)XSize;
                    min_y = ((short)max_y) >> 1;

                    if ((short)px < min_y - 2) {
                        px++;
                        erase_gorilla((uint8_t)(px - 1), py);
                        draw_gorilla(px, py, _XL_GREEN);
                    }
                }
            }

            /* -------------------- */
            /*  Player aiming/throw  */
            /* -------------------- */
            if (!ban_active && player_turn) {
                power = ((power + 1) * 7) % 24;
                aim_up = (uint8_t)(power / 3);

                fire_prev = _XL_FIRE(input) ? 1 : 0;

                if (fire_prev == 0 && player_turn) {
                    /* Release: launch banana. */
                    ban_active = 1;
                    bx = px;
                    by = py;
                    bvx = (short)(aim_up * aim_up + power) / 6;
                    if (bvx < 2) bvx = 2;
                    if (bvx > XSize - 1) bvx = XSize - 1;

                    bvy = -(short)(aim_up + 3);

                    draw_banana(bx, by);
                }
            }

            /* -------------------- */
            /*  CPU turn             */
            /* -------------------- */
            if (!ban_active && !player_turn) {
                turn_timer++;

                if (turn_timer >= 40) {
                    ban_active = 1;
                    bx = cx;
                    by = cy;

                    /* Simple AI: throw hard enough to reach player side. */
                    ax = (short)cx - (short)px;
                    bvx = (short)(ax / 2 + 3);
                    if (bvx > 16) bvx = 16;
                    if (bvx < 4) bvx = 4;

                    ay = py - cy;
                    bvy = (ay / 3) - 8;
                    if (bvy > -2) bvy = -2;

                    draw_banana(bx, by);
                }
            }

            /* -------------------- */
            /*  Banana physics       */
            /* -------------------- */
            if (ban_active) {
                erase_banana(bx, by);

                bvy += 1; /* gravity */

                bx = (uint8_t)((short)bx + bvx);
                by = (uint8_t)((short)by + bvy);

                /* Side walls bounce. */
                if (bx == 0 && bvx < 0) {
                    bvx = -bvx;
                }
                if ((XSize > 0) && bx >= XSize - 1) {
                    if (bvx > 0) {
                        bvx = -bvx;
                    }
                    bx = XSize - 1;
                }

                /* Ceiling bounce. */
                if (by == 0 && bvy < 0) {
                    bvy = -bvy;
                }

                /* Building / floor collision in this column. */
                min_y = heights[bx];

                if ((short)by >= min_y) {
                    by = (uint8_t)min_y;
                    if (bvy > 0) {
                        bvy = -(bvy / 2);
                    }
                    _XL_PING_SOUND();

                    /* If banana comes to rest, remove it after a moment. */
                    if ((short)bvy >= -1 && (short)bvy <= 1) {
                        turn_timer++;
                        if (turn_timer > 60) {
                            erase_banana(bx, by);
                            ban_active = 0;
                            turn_timer = 0;

                            player_turn = !player_turn;
                            _XL_TICK_SOUND();
                        }
                    }
                }

                /* Side building collision: if column height changes a lot. */
                /* Keep it simple: bounce when entering a much taller column. */
                max_y = YSize;
                (void)max_y;

                draw_banana(bx, by);

                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            } else {
                /* Small pause between turns. */
                turn_timer++;
                if (turn_timer > 10) {
                    turn_timer = 0;
                }
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR / 2 < 1 ? 1 : (uint16_t)(_XL_SLOW_DOWN_FACTOR / 2));
            }

            /* -------------------- */
            /*  Hit detection        */
            /* -------------------- */
            if (ban_active) {
                /* Player hit. */
                if ((bx == px || bx == (px > 0 ? px - 1 : 0)) &&
                    (by == py || by == (py + 1))) {
                    erase_banana(bx, by);
                    ban_active = 0;

                    lives--;
                    draw_hud(score, lives);
                    _XL_EXPLOSION_SOUND();

                    if (lives == 0) {
                        state = 3; /* game over */
                    } else {
                        turn_timer = 0;
                        player_turn = !player_turn;
                    }
                }

                /* CPU hit. */
                if ((bx == cx || bx == (cx > 0 ? cx - 1 : 0)) &&
                    (by == cy || by == (cy + 1))) {
                    erase_banana(bx, by);
                    ban_active = 0;

                    score += 10;
                    draw_hud(score, lives);
                    _XL_EXPLOSION_SOUND();

                    turn_timer = 0;
                    player_turn = !player_turn;
                }
            }

            /* -------------------- */
            /*  Game over screen     */
            /* -------------------- */
            if (state == 3) {
                draw_message("GAME OVER");
                _XL_EXPLOSION_SOUND();

                state = 4;
            }
        }

        /* ------------------------- */
        /*  Restart                  */
        /* ------------------------- */
        while (state == 4) {
            input = _XL_INPUT();
            if (_XL_FIRE(input)) {
                state = 0;
            }
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }

    return 0;
}
