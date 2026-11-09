#include "cross_lib.h"

#define NUM_PLATFORMS 5
#define NUM_TURTLES   4
#define NUM_LEVELS    3
#define MARIO_TILE    _TILE_1
#define TURTLE_TILE   _TILE_2
#define PLATFORM_TILE _TILE_3
#define STAR_TILE     _TILE_4
#define POINT_TILE    _TILE_5
#define KEY_TILE      _TILE_6
#define DOOR_TILE     _TILE_7
#define MARIO_COLOR       _XL_RED
#define TURTLE_COLOR      _XL_GREEN
#define PLAT_COLOR        _XL_CYAN
#define STAR_COLOR        _XL_YELLOW
#define POINT_COLOR       _XL_MAGENTA
#define KEY_COLOR         _XL_WHITE
#define DOOR_COLOR        _XL_GREEN

typedef struct {
    uint8_t x_start;
    uint8_t x_end;
    uint8_t y_row;
} Platform;

typedef struct {
    uint8_t x;
    uint8_t dir;
    uint8_t alive;
} Turtle;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t collected;
} Item;

static void set_tile(uint8_t buf[][YSize], uint8_t x, uint8_t y,
                     uint8_t tile_id, uint8_t color_id)
{
    if (buf[x][y] != tile_id) {
        _XL_DRAW(x, y, tile_id, color_id);
        buf[x][y] = tile_id;
    }
}

static void clear_tile(uint8_t buf[][YSize], uint8_t x, uint8_t y)
{
    if (buf[x][y] != 0) {
        _XL_DELETE(x, y);
        buf[x][y] = 0;
    }
}

static void clear_tile_if(uint8_t buf[][YSize], uint8_t x, uint8_t y, uint8_t tile_id)
{
    if (buf[x][y] == tile_id) {
        _XL_DELETE(x, y);
        buf[x][y] = 0;
    }
}

static void draw_platforms(uint8_t buf[][YSize], Platform *plats)
{
    uint8_t i, j;
    for (i = 0; i < NUM_PLATFORMS; i++) {
        for (j = plats[i].x_start; j <= plats[i].x_end; j++) {
            set_tile(buf, j, plats[i].y_row, PLATFORM_TILE, PLAT_COLOR);
        }
    }
}

static void clear_platforms(uint8_t buf[][YSize], Platform *plats)
{
    uint8_t i, j;
    for (i = 0; i < NUM_PLATFORMS; i++) {
        for (j = plats[i].x_start; j <= plats[i].x_end; j++) {
            clear_tile_if(buf, j, plats[i].y_row, PLATFORM_TILE);
        }
    }
}

int main(void)
{
    Platform levels[NUM_LEVELS][NUM_PLATFORMS];
    Turtle turtles[NUM_TURTLES];
    Item star_item;
    Item point_item;
    Item key_item;
    Item door_item;
    uint8_t screen_buf[XSize][YSize];
    uint8_t mario_x, mario_y;
    short vy;
    uint8_t on_ground;
    uint8_t lives, score;
    uint16_t invincible_timer;
    uint8_t input;
    uint8_t i, j;
    uint8_t old_mx, old_my;
    uint8_t level_complete;
    uint8_t cur_level;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    for (i = 0; i < XSize; i++) {
        for (j = 0; j < YSize; j++) {
            screen_buf[i][j] = 0;
        }
    }

    /* === Level 1 platforms === */
    levels[0][0].x_start = 0;
    levels[0][0].x_end   = XSize - 1;
    levels[0][0].y_row   = YSize - 1;

    levels[0][1].x_start = 2;
    levels[0][1].x_end   = (uint8_t)(XSize / 2);
    levels[0][1].y_row   = (uint8_t)((YSize - 1) * 2 / 3);

    levels[0][2].x_start = (uint8_t)((XSize + 2) / 3);
    levels[0][2].x_end   = XSize - 3;
    levels[0][2].y_row   = (uint8_t)((YSize - 1) * 4 / 6);

    levels[0][3].x_start = 3;
    levels[0][3].x_end   = (uint8_t)((XSize + 1) / 3);
    levels[0][3].y_row   = (uint8_t)(YSize / 2);

    levels[0][4].x_start = (uint8_t)(XSize * 2 / 5);
    levels[0][4].x_end   = XSize - 4;
    levels[0][4].y_row   = (uint8_t)((YSize - 1) / 3);

    /* === Level 2 platforms (different layout) === */
    levels[1][0].x_start = 0;
    levels[1][0].x_end   = XSize - 1;
    levels[1][0].y_row   = YSize - 1;

    levels[1][1].x_start = (uint8_t)(XSize / 3);
    levels[1][1].x_end   = XSize - 2;
    levels[1][1].y_row   = (uint8_t)((YSize - 1) * 5 / 6);

    levels[1][2].x_start = 1;
    levels[1][2].x_end   = (uint8_t)(XSize / 3);
    levels[1][2].y_row   = (uint8_t)((YSize - 1) * 4 / 6);

    levels[1][3].x_start = (uint8_t)(XSize / 2);
    levels[1][3].x_end   = XSize - 1;
    levels[1][3].y_row   = (uint8_t)((YSize - 1) * 3 / 6);

    levels[1][4].x_start = 2;
    levels[1][4].x_end   = (uint8_t)(XSize / 3 + 2);
    levels[1][4].y_row   = (uint8_t)((YSize - 1) / 3);

    /* === Level 3 platforms (different layout) === */
    levels[2][0].x_start = 0;
    levels[2][0].x_end   = XSize - 1;
    levels[2][0].y_row   = YSize - 1;

    levels[2][1].x_start = 1;
    levels[2][1].x_end   = (uint8_t)(XSize * 3 / 5);
    levels[2][1].y_row   = (uint8_t)((YSize - 1) * 7 / 9);

    levels[2][2].x_start = (uint8_t)(XSize * 2 / 5);
    levels[2][2].x_end   = XSize - 1;
    levels[2][2].y_row   = (uint8_t)((YSize - 1) * 6 / 9);

    levels[2][3].x_start = 0;
    levels[2][3].x_end   = (uint8_t)(XSize / 4 + 2);
    levels[2][3].y_row   = (uint8_t)((YSize - 1) * 5 / 9);

    levels[2][4].x_start = (uint8_t)(XSize * 3 / 5);
    levels[2][4].x_end   = XSize - 2;
    levels[2][4].y_row   = (uint8_t)((YSize - 1) / 3);

    lives = 3;
    score = 0;
    cur_level = 0;

    while (1) {
        Platform *plats = &levels[cur_level][0];

        /* Reset game state */
        mario_x   = 2;
        mario_y   = plats[0].y_row - 2;
        vy        = 0;
        on_ground = 1;
        invincible_timer = 0;
        level_complete = 0;

        for (i = 0; i < NUM_TURTLES; i++) {
            turtles[i].x     = plats[i + 1].x_start + 1;
            turtles[i].dir   = 1;
            turtles[i].alive = 1;
        }

        /* Place items at safe edge positions (where turtles don't patrol) */
        star_item.x         = plats[2].x_start;
        star_item.y         = plats[2].y_row - 1;
        star_item.collected = 0;

        point_item.x        = plats[3].x_end - 1;
        point_item.y        = plats[3].y_row - 1;
        point_item.collected = 0;

        key_item.x          = plats[4].x_start;
        key_item.y          = plats[4].y_row - 1;
        key_item.collected  = 0;

        door_item.x         = plats[4].x_end - 1;
        door_item.y         = plats[4].y_row - 1;
        door_item.collected = 0;

        /* Draw platforms for this level */
        draw_platforms(screen_buf, plats);

        /* Draw initial Mario */
        set_tile(screen_buf, mario_x, mario_y, MARIO_TILE, MARIO_COLOR);
        set_tile(screen_buf, mario_x, (uint8_t)(mario_y + 1), MARIO_TILE, MARIO_COLOR);

        /* Draw turtles */
        for (i = 0; i < NUM_TURTLES; i++) {
            uint8_t ty = plats[i + 1].y_row - 1;
            set_tile(screen_buf, turtles[i].x, ty, TURTLE_TILE, TURTLE_COLOR);
            set_tile(screen_buf, (uint8_t)(turtles[i].x + 1), ty, TURTLE_TILE, TURTLE_COLOR);
        }

        /* Draw items */
        if (!star_item.collected) {
            set_tile(screen_buf, star_item.x, star_item.y, STAR_TILE, STAR_COLOR);
        }
        if (!point_item.collected) {
            set_tile(screen_buf, point_item.x, point_item.y, POINT_TILE, POINT_COLOR);
        }
        if (!key_item.collected) {
            set_tile(screen_buf, key_item.x, key_item.y, KEY_TILE, KEY_COLOR);
        }

        /* Display lives and score */
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 0, "LIVES");
        _XL_PRINTD(5, 0, 1, lives);
        _XL_PRINT((uint8_t)(XSize - 6), 0, "SCORE");
        _XL_PRINTD((uint8_t)(XSize - 1), 0, 1, score);

        /* Main game loop */
        while (lives > 0 && !level_complete) {
            input = _XL_INPUT();

            /* --- Move Mario horizontally --- */
            if (_XL_LEFT(input)) {
                clear_tile_if(screen_buf, mario_x, mario_y, MARIO_TILE);
                clear_tile_if(screen_buf, mario_x, (uint8_t)(mario_y + 1), MARIO_TILE);
                if (mario_x > 0) {
                    mario_x--;
                }
                set_tile(screen_buf, mario_x, mario_y, MARIO_TILE, MARIO_COLOR);
                set_tile(screen_buf, mario_x, (uint8_t)(mario_y + 1), MARIO_TILE, MARIO_COLOR);
            } else if (_XL_RIGHT(input)) {
                clear_tile_if(screen_buf, mario_x, mario_y, MARIO_TILE);
                clear_tile_if(screen_buf, mario_x, (uint8_t)(mario_y + 1), MARIO_TILE);
                if (mario_x < XSize - 1) {
                    mario_x++;
                }
                set_tile(screen_buf, mario_x, mario_y, MARIO_TILE, MARIO_COLOR);
                set_tile(screen_buf, mario_x, (uint8_t)(mario_y + 1), MARIO_TILE, MARIO_COLOR);
            }

            /* --- Jump (UP or FIRE) --- */
            if ((_XL_UP(input) || _XL_FIRE(input)) && on_ground) {
                vy = -4;
                on_ground = 0;
                _XL_PING_SOUND();
            }

            /* --- Check if Mario is still supported by a platform below --- */
            if (on_ground) {
                uint8_t feet_row = (uint8_t)(mario_y + 1);
                uint8_t supported = 0;
                for (i = 0; i < NUM_PLATFORMS; i++) {
                    if (feet_row == plats[i].y_row - 1 &&
                        mario_x >= plats[i].x_start &&
                        mario_x <= plats[i].x_end) {
                        supported = 1;
                        break;
                    }
                }
                if (!supported) {
                    on_ground = 0;
                    vy = 0;
                }
            }

            /* --- Apply gravity and vertical movement --- */
            if (!on_ground) {
                old_mx = mario_x;
                old_my = mario_y;

                vy++;
                if (vy > 4) {
                    vy = 4;
                }
                mario_y += (uint8_t)vy;

                /* Ceiling check */
                if (mario_y < 2) {
                    mario_y = 2;
                    vy = 1;
                }

                /* Check for hitting underside of a platform while jumping up */
                if (vy < 0) {
                    uint8_t head_row = mario_y;
                    for (i = 0; i < NUM_PLATFORMS; i++) {
                        if ((short)head_row <= (short)(plats[i].y_row - 1) &&
                            old_my >= plats[i].y_row - 1 &&
                            mario_x >= plats[i].x_start &&
                            mario_x <= plats[i].x_end) {
                            mario_y = (uint8_t)(plats[i].y_row + 1);
                            vy = 1;
                            break;
                        }
                    }
                }

                /* Platform landing check (falling down) */
                if (vy > 0) {
                    for (i = 0; i < NUM_PLATFORMS; i++) {
                        uint8_t feet = (uint8_t)(mario_y + 1);
                        if ((short)feet >= (short)plats[i].y_row &&
                            mario_x >= plats[i].x_start &&
                            mario_x <= plats[i].x_end) {
                            mario_y = (uint8_t)(plats[i].y_row - 2);
                            vy = 0;
                            on_ground = 1;
                            break;
                        }
                    }
                }

                /* Fell off screen */
                if ((short)mario_y + 2 >= (short)YSize) {
                    lives--;
                    _XL_EXPLOSION_SOUND();
                    clear_tile_if(screen_buf, old_mx, old_my, MARIO_TILE);
                    clear_tile_if(screen_buf, old_mx, (uint8_t)(old_my + 1), MARIO_TILE);
                    mario_x = old_mx;
                    mario_y = old_my;
                    break;
                }

                /* Redraw Mario at new position */
                if (mario_x != old_mx || mario_y != old_my) {
                    clear_tile_if(screen_buf, old_mx, old_my, MARIO_TILE);
                    clear_tile_if(screen_buf, old_mx, (uint8_t)(old_my + 1), MARIO_TILE);
                    set_tile(screen_buf, mario_x, mario_y, MARIO_TILE, MARIO_COLOR);
                    set_tile(screen_buf, mario_x, (uint8_t)(mario_y + 1), MARIO_TILE, MARIO_COLOR);
                }
            }

            /* --- Update turtles and check collisions --- */
            for (i = 0; i < NUM_TURTLES && lives > 0; i++) {
                if (!turtles[i].alive) continue;

                uint8_t plat_i = (uint8_t)(i + 1);
                uint8_t ty     = plats[plat_i].y_row - 1;
                uint8_t old_tx = turtles[i].x;

                /* Move turtle */
                if (turtles[i].dir == 0) {
                    if (turtles[i].x > plats[plat_i].x_start + 1) {
                        turtles[i].x--;
                    } else {
                        turtles[i].dir = 1;
                    }
                } else {
                    if ((uint8_t)(turtles[i].x + 2) < plats[plat_i].x_end) {
                        turtles[i].x++;
                    } else {
                        turtles[i].dir = 0;
                    }
                }

                /* Redraw turtle if moved (only clear tiles that belong to turtle) */
                if (turtles[i].x != old_tx) {
                    clear_tile_if(screen_buf, old_tx, ty, TURTLE_TILE);
                    clear_tile_if(screen_buf, (uint8_t)(old_tx + 1), ty, TURTLE_TILE);
                    set_tile(screen_buf, turtles[i].x, ty, TURTLE_TILE, TURTLE_COLOR);
                    set_tile(screen_buf, (uint8_t)(turtles[i].x + 1), ty, TURTLE_TILE, TURTLE_COLOR);
                }

                /* Check collision with Mario */
                if (mario_x >= turtles[i].x &&
                    mario_x <= (uint8_t)(turtles[i].x + 1)) {
                    uint8_t m_top = mario_y;
                    uint8_t m_bot = (uint8_t)(mario_y + 1);
                    if ((short)m_bot >= (short)ty && (short)m_top <= (short)ty) {
                        if (!on_ground && vy > 0) {
                            turtles[i].alive = 0;
                            score += 100;
                            _XL_SHOOT_SOUND();
                            clear_tile_if(screen_buf, turtles[i].x, ty, TURTLE_TILE);
                            clear_tile_if(screen_buf, (uint8_t)(turtles[i].x + 1), ty, TURTLE_TILE);
                        } else if (invincible_timer > 0) {
                            turtles[i].alive = 0;
                            score += 100;
                            _XL_SHOOT_SOUND();
                            clear_tile_if(screen_buf, turtles[i].x, ty, TURTLE_TILE);
                            clear_tile_if(screen_buf, (uint8_t)(turtles[i].x + 1), ty, TURTLE_TILE);
                        } else {
                            lives--;
                            _XL_EXPLOSION_SOUND();
                        }
                    }
                }
            }

            /* --- Check item collection --- */
            if (!star_item.collected && mario_x == star_item.x) {
                uint8_t m_top = mario_y;
                uint8_t m_bot = (uint8_t)(mario_y + 1);
                if ((short)m_bot >= (short)star_item.y && (short)m_top <= (short)star_item.y) {
                    star_item.collected = 1;
                    invincible_timer = 120;
                    _XL_TOCK_SOUND();
                    clear_tile(screen_buf, star_item.x, star_item.y);
                }
            }

            if (!point_item.collected && mario_x == point_item.x) {
                uint8_t m_top = mario_y;
                uint8_t m_bot = (uint8_t)(mario_y + 1);
                if ((short)m_bot >= (short)point_item.y && (short)m_top <= (short)point_item.y) {
                    point_item.collected = 1;
                    score += 200;
                    _XL_TICK_SOUND();
                    clear_tile(screen_buf, point_item.x, point_item.y);
                }
            }

            if (!key_item.collected && mario_x == key_item.x) {
                uint8_t m_top = mario_y;
                uint8_t m_bot = (uint8_t)(mario_y + 1);
                if ((short)m_bot >= (short)key_item.y && (short)m_top <= (short)key_item.y) {
                    key_item.collected = 1;
                    score += 50;
                    _XL_TOCK_SOUND();
                    clear_tile(screen_buf, key_item.x, key_item.y);
                    set_tile(screen_buf, door_item.x, door_item.y, DOOR_TILE, DOOR_COLOR);
                }
            }

            if (key_item.collected && !door_item.collected && mario_x == door_item.x) {
                uint8_t m_top = mario_y;
                uint8_t m_bot = (uint8_t)(mario_y + 1);
                if ((short)m_bot >= (short)door_item.y && (short)m_top <= (short)door_item.y) {
                    door_item.collected = 1;
                    score += 500;
                    level_complete = 1;
                    _XL_ZAP_SOUND();
                    clear_tile(screen_buf, door_item.x, door_item.y);
                }
            }

            /* --- Decrement invincibility timer --- */
            if (invincible_timer > 0) {
                invincible_timer--;
            }

            /* Update score display */
            _XL_PRINTD((uint8_t)(XSize - 1), 0, 1, score);

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* Level complete or game over screen */
        if (level_complete) {
            _XL_SET_TEXT_COLOR(_XL_GREEN);
            _XL_PRINT((uint8_t)(XSize / 4), (uint8_t)(YSize / 2), "LEVEL COMPLETE");
            cur_level++;
            if (cur_level >= NUM_LEVELS) {
                cur_level = 0;
            }
        } else {
            _XL_SET_TEXT_COLOR(_XL_YELLOW);
            _XL_PRINT((uint8_t)(XSize / 4), (uint8_t)(YSize / 2), "GAME OVER");
        }
        _XL_WAIT_FOR_INPUT();

        /* Clear Mario */
        clear_tile_if(screen_buf, mario_x, mario_y, MARIO_TILE);
        clear_tile_if(screen_buf, mario_x, (uint8_t)(mario_y + 1), MARIO_TILE);

        /* Clear turtles */
        for (i = 0; i < NUM_TURTLES; i++) {
            uint8_t ty = plats[i + 1].y_row - 1;
            if (turtles[i].alive) {
                clear_tile_if(screen_buf, turtles[i].x, ty, TURTLE_TILE);
                clear_tile_if(screen_buf, (uint8_t)(turtles[i].x + 1), ty, TURTLE_TILE);
            }
        }

        /* Clear remaining items */
        if (!star_item.collected) {
            clear_tile(screen_buf, star_item.x, star_item.y);
        }
        if (!point_item.collected) {
            clear_tile(screen_buf, point_item.x, point_item.y);
        }
        if (!key_item.collected) {
            clear_tile(screen_buf, key_item.x, key_item.y);
        }
        if (key_item.collected && !door_item.collected) {
            clear_tile(screen_buf, door_item.x, door_item.y);
        }

        /* Clear old platforms before drawing new level */
        clear_platforms(screen_buf, plats);
    }

    return 0;
}
