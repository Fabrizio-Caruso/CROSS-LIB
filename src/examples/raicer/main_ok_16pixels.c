#include "cross_lib.h"

#define MAX_OBSTACLES 8

#define SPAWN_TIME ((YSize)/8)

#if XSize<=45
    #define ROAD_WIDTH 9
#elif XSize<=64
    #define ROAD_WIDTH 13
#else
    #define ROAD_WIDTH 15
#endif

/* Narrow road */
#define ROAD_LEFT   ((uint8_t)(XSize / 2 - (ROAD_WIDTH/2)))
#define ROAD_RIGHT  ((uint8_t)(XSize / 2 + (ROAD_WIDTH/2)))
#define ROAD_CENTER ((uint8_t)(XSize / 2))

/*
 * Player half-tile movement:
 * player_hx is a doubled coordinate.
 *   even player_hx => normal one-column car at x = player_hx/2
 *   odd  player_hx => two adjacent columns:
 *                      left  = player_hx/2
 *                      right = (player_hx+1)/2
 */

typedef struct {
    uint8_t x;
    uint8_t y;       /* top tile of the car */
    uint8_t active;
} Obstacle;

Obstacle obstacles[MAX_OBSTACLES];
uint16_t player_hx;  /* doubled horizontal position of player */
uint16_t score;
uint8_t spawn_timer;

/* ------------------------------------------------------------------ */
/*  Road helpers                                                       */
/* ------------------------------------------------------------------ */

void draw_road_tile(uint8_t x, uint8_t y)
{
    if (x == ROAD_LEFT || x == ROAD_RIGHT) {
        _XL_DRAW(x, y, _TILE_2, _XL_WHITE);   /* road edge  */
    } else if (x == ROAD_CENTER) {
        _XL_DRAW(x, y, _TILE_1, _XL_YELLOW);  /* centre line*/
    } else {
        _XL_DRAW(x, y, _TILE_0, _XL_BLUE);    /* road surface */
    }
}

void draw_road(void)
{
    uint8_t x;
    uint8_t y;

    for (x = ROAD_LEFT; x <= ROAD_RIGHT; x++) {
        for (y = 0; y < YSize; y++) {
            draw_road_tile(x, y);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Player car – two tiles tall, half-tile horizontal movement         */
/* ------------------------------------------------------------------ */

uint8_t player_left_col(void)
{
    if (player_hx & 1u) {
        return (uint8_t)(player_hx / 2);
    } else {
        return (uint8_t)(player_hx / 2);
    }
}

uint8_t player_right_col(void)
{
    /* Odd position uses the next column. Even position has only one column. */
    if (player_hx & 1u) {
        uint8_t right = (uint8_t)((player_hx + 1u) / 2);

        /* Do not go beyond previous road limit. */
        if (right > ROAD_RIGHT) {
            return ROAD_RIGHT;
        }

        return right;
    } else {
        return player_left_col();
    }
}

/* Draw one vertical car column at x: top and bottom tile. */
void draw_player_column(uint8_t x)
{
    _XL_DRAW(x, (uint8_t)(YSize - 2), _TILE_6, _XL_CYAN); /* top tile   */
    _XL_DRAW(x, (uint8_t)(YSize - 1), _TILE_5, _XL_CYAN); /* bottom tile*/
}

/* Delete one vertical car column at x and restore road underneath. */
void delete_player_column(uint8_t x)
{
    _XL_DELETE(x, (uint8_t)(YSize - 2));
    draw_road_tile(x, (uint8_t)(YSize - 2));
    _XL_DELETE(x, (uint8_t)(YSize - 1));
    draw_road_tile(x, (uint8_t)(YSize - 1));
}

/* Draw the complete current player shape. */
void draw_player(void)
{
    uint8_t left;
    uint8_t right;

    left  = player_left_col();
    right = player_right_col();

    if (left == right) {
        /* Even position: normal one-column car. */
        draw_player_column(left);
    } else {
        /* Odd half-tile position: two adjacent columns. */
        draw_player_column(left);
        draw_player_column(right);
    }
}

/* Delete the complete current player shape and restore road underneath. */
void delete_player(void)
{
    uint8_t left;
    uint8_t right;

    left  = player_left_col();
    right = player_right_col();

    if (left == right) {
        delete_player_column(left);
    } else {
        delete_player_column(left);
        delete_player_column(right);
    }
}

/*
 * Move player one half-tile to the left.
 * Only changed tiles are deleted/drawn where possible:
 *   even -> odd : keep old column, add new left column
 *   odd  -> even: remove old right column, keep common column
 */
void player_move_left(void)
{
    uint8_t old_left;
    uint8_t old_right;

    if (player_hx == ROAD_LEFT*2u) return;

    old_left  = player_left_col();
    old_right = player_right_col();

    if (player_hx & 1u) {
        /*
         * Currently odd: columns [old_left, old_right]
         * Move to even at old_left.
         * The old_left column stays unchanged; delete only the right column.
         */
        player_hx--;

        if (old_left != old_right) {
            delete_player_column(old_right);
        }
    } else {
        /*
         * Currently even: single column at old_left = old_right.
         * Move to odd between new left and old_left.
         * The old column stays unchanged; add only the new left column.
         */
        player_hx--;

        if (old_left > ROAD_LEFT) {
            draw_player_column((uint8_t)(old_left - 1));
        }
    }
}

/* Move player one half-tile to the right, clamped to road limits. */
void player_move_right(void)
{
    uint8_t old_left;
    uint8_t old_right;
    // uint16_t max_player_hx;

    /* Highest safe doubled position: odd may use left + 1 columns. */
    // if (ROAD_RIGHT >= ROAD_LEFT) {
        // max_player_hx = (uint16_t)((ROAD_RIGHT - 1u) * 2u);
    // } else {
        // max_player_hx = 0;
    // }

    if (player_hx >= (uint16_t)((ROAD_RIGHT - 1u) * 2u)) return;

    old_left  = player_left_col();
    old_right = player_right_col();

    if (!(player_hx & 1u)) {
        /*
         * Currently even: single column at old_left = old_right.
         * Move to odd between old_left and right neighbor.
         * The old column stays unchanged; add only the new right column.
         */
        player_hx++;

        if (old_right < ROAD_RIGHT) {
            draw_player_column((uint8_t)(old_right + 1));
        }
    } else {
        /*
         * Currently odd: columns [old_left, old_right].
         * Move to even at old_right.
         * The old_right column stays unchanged; delete only the left column.
         */
        player_hx++;

        if (old_left != old_right) {
            delete_player_column(old_left);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Obstacle cars – two tiles tall, move within road                  */
/* ------------------------------------------------------------------ */

uint8_t spawn_obstacle(void)
{
    uint8_t i;
    uint16_t span;
    uint16_t rx;

    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) {
            span = (uint16_t)(ROAD_RIGHT - ROAD_LEFT + 1u);
            rx   = _XL_RAND() % span;

            obstacles[i].x      = (uint8_t)(ROAD_LEFT + rx);
            obstacles[i].y      = 0;
            obstacles[i].active = 1;

            _XL_DRAW(obstacles[i].x, 0, _TILE_3, _XL_RED);   /* top    */
            _XL_DRAW(obstacles[i].x, 1, _TILE_4, _XL_RED);   /* bottom */
            return 1;
        }
    }

    return 0;
}

/* Delete both tiles of an obstacle at (x, y_top) and restore road */
void delete_obstacle_tiles(uint8_t x, uint8_t y_top)
{
    _XL_DELETE(x, y_top);
    draw_road_tile(x, y_top);

    if ((uint16_t)y_top + 1u < (uint16_t)YSize) {
        /* This placeholder matches original behavior and is immediately deleted. */
        _XL_DRAW(x, (uint8_t)(y_top + 1), _TILE_4, _XL_RED);
        _XL_DELETE(x, (uint8_t)(y_top + 1));
        draw_road_tile(x, (uint8_t)(y_top + 1));
    }
}

/* Draw both tiles of an obstacle at (x, y_top) */
void draw_obstacle_tiles(uint8_t x, uint8_t y_top)
{
    _XL_DRAW(x, y_top, _TILE_3, _XL_RED);

    if ((uint16_t)y_top + 1u < (uint16_t)YSize) {
        _XL_DRAW(x, (uint8_t)(y_top + 1), _TILE_4, _XL_RED);
    }
}

/* Lateral drift: move obstacle left or right by 1, clamped to road */
short lateral_drift(uint8_t i)
{
    uint16_t r = _XL_RAND() & 0x0Fu;   /* 0=left, 1=stay, 2=right */

    if (r == 0u && obstacles[i].x > ROAD_LEFT) {
        return -1;
    } else if (r == 2u && obstacles[i].x < ROAD_RIGHT) {
        return 1;
    }

    return 0;
}

void update_obstacles(void)
{
    uint8_t i;
    uint8_t old_x;
    uint8_t old_y;
    short dx;

    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) continue;

        old_x = obstacles[i].x;
        old_y = obstacles[i].y;

        /* Stop lateral drift near bottom, same as original. */
        dx     = (old_y >= (uint8_t)(YSize - 7u)) ? 0 : (short)lateral_drift(i);

        /* Move down */
        obstacles[i].y++;

        /* Apply lateral movement, clamped to road */
        if (dx < 0) {
            if (obstacles[i].x > ROAD_LEFT) {
                obstacles[i].x--;
            }
        } else if (dx > 0) {
            if (obstacles[i].x < ROAD_RIGHT) {
                obstacles[i].x++;
            }
        }

        /* Delete old position tiles, restore road */
        _XL_DELETE(old_x, old_y);
        draw_road_tile(old_x, old_y);

        if ((uint16_t)old_y + 1u < (uint16_t)YSize) {
            _XL_DELETE(old_x, (uint8_t)(old_y + 1));
            draw_road_tile(old_x, (uint8_t)(old_y + 1));
        }

        if (obstacles[i].y >= (uint8_t)(YSize - 1u)) {
            /* Car has left the screen */
            obstacles[i].active = 0;
        } else {
            /* Draw at new position */
            draw_obstacle_tiles(obstacles[i].x, obstacles[i].y);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Collision                                                          */
/* ------------------------------------------------------------------ */

uint8_t check_collision(void)
{
    uint8_t i;
    uint8_t left;
    uint8_t right;
    uint8_t y_limit;

    /* Player occupies rows YSize-2 and YSize-1.
     * Obstacles are considered colliding when their top tile reaches the
     * row just above/below the player area, matching original behavior. */
    y_limit = (uint8_t)(YSize - 3u);

    left  = player_left_col();
    right = player_right_col();

    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) continue;

        if (obstacles[i].y >= y_limit &&
            obstacles[i].x >= left &&
            obstacles[i].x <= right) {
            return 1;
        }
    }

    return 0;
}

/* ------------------------------------------------------------------ */
/*  Initialisation                                                     */
/* ------------------------------------------------------------------ */

void init_game(void)
{
    uint8_t i;

    /* Start in the centre as an even, normal one-column position. */
    player_hx = (uint16_t)(ROAD_CENTER * 2u);

    score       = 0;
    spawn_timer = 0;

    for (i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].active = 0;
    }
}

/* ------------------------------------------------------------------ */
/*  Main                                                               */
/* ------------------------------------------------------------------ */

int main(void)
{
    uint8_t game_over = 0;
    uint8_t input;
    uint8_t i;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    for (;;) {
        _XL_CLEAR_SCREEN();

        init_game();

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, (uint8_t)(YSize - 5u), "LEFT AND RIGHT TO STEER");
        _XL_PRINT(0, (uint8_t)(YSize - 6u), "AVOID THE RED CARS");

        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD(0, 0, 1, score);

        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();

        _XL_CLEAR_SCREEN();
        draw_road();
        draw_player();

        while (!game_over) {
            /* --- Input: half-tile movement --- */
            input = _XL_INPUT();

            if (_XL_LEFT(input)) {
                player_move_left();
                // if (player_hx > 0u) {
                    // _XL_TICK_SOUND();
                // }
            } else if (_XL_RIGHT(input)) {
                // uint16_t max_player_hx;

                // if (ROAD_RIGHT >= ROAD_LEFT) {
                    // max_player_hx = (uint16_t)((ROAD_RIGHT - 1u) * 2u);
                // } else {
                    // max_player_hx = 0;
                // }

                player_move_right();
                // if (player_hx <= (uint16_t)((ROAD_RIGHT - 1u) * 2u) &&
                    // player_hx != ((uint16_t)(ROAD_CENTER * 2u))) {
                    // /* Sound only when movement actually happened. */
                // }

                /* Simpler: sound when position changed can be checked before/after,
                   but to avoid extra state here we use a small local comparison. */
            }

            /* --- Spawn --- */
            spawn_timer++;
            if (spawn_timer >= SPAWN_TIME) {
                spawn_timer = 0;
                spawn_obstacle();
            }

            /* --- Update all obstacles (vertical + lateral within road) --- */
            update_obstacles();

            /* --- Collision --- */
            if (check_collision()) {
                game_over = 1;
                _XL_EXPLOSION_SOUND();

                delete_player();

                for (i = 0; i < MAX_OBSTACLES; i++) {
                    if (obstacles[i].active) {
                        _XL_DELETE(obstacles[i].x, obstacles[i].y);
                        draw_road_tile(obstacles[i].x, obstacles[i].y);

                        if ((uint16_t)obstacles[i].y + 1u < (uint16_t)YSize) {
                            _XL_DELETE(obstacles[i].x,
                                       (uint8_t)(obstacles[i].y + 1));
                            draw_road_tile(obstacles[i].x,
                                           (uint8_t)(obstacles[i].y + 1));
                        }

                        obstacles[i].active = 0;
                    }
                }

                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(ROAD_LEFT, (uint8_t)(YSize / 2), "GAME OVER");

                _XL_SLEEP(1);
                _XL_WAIT_FOR_INPUT();
            } else {
                score++;
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_PRINTD(0, 0, 1, score);
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        game_over = 0;
    }

    return 0;
}
