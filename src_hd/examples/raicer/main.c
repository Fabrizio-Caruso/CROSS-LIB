#include "cross_lib.h"

#define MAX_OBSTACLES 8

#define SPAWN_TIME ((YSize)/8)

#if XSize<=23
    #define ROAD_WIDTH 11
#elif XSize<=32
    #define ROAD_WIDTH 15
#elif XSize<=40
    #define ROAD_WIDTH 25
#else
    #define ROAD_WIDTH 37
#endif

/* Narrow road */
#define ROAD_LEFT   ((uint8_t)(XSize / 2 - (ROAD_WIDTH/2)))
#define ROAD_RIGHT  ((uint8_t)(XSize / 2 + (ROAD_WIDTH/2)))
#define ROAD_CENTER ((uint8_t)(XSize / 2))

#define BIG_ENEMY_TL _TILE_5
#define BIG_ENEMY_TR _TILE_6
#define BIG_ENEMY_BL _TILE_7
#define BIG_ENEMY_BR _TILE_8

/*
 * Player car – two tiles tall, half-tile horizontal movement.
 *   Even player_hx = 2n  =>  2-wide car: columns [n, n+1]
 *   Odd  player_hx = 2n+1 => 3-wide car: columns [n, n+1, n+2]
 *
 * Each half-tile step changes exactly one column at an edge.
 */

typedef struct {
    uint8_t x;       /* top-left tile of the enemy car */
    uint8_t y;       /* top tile of the enemy car      */
    uint8_t active;
} Obstacle;

Obstacle obstacles[MAX_OBSTACLES];
uint16_t player_hx;  /* doubled horizontal position (left edge reference) */
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
/*  Player car – 2 or 3 columns wide, two tiles tall                   */
/* ------------------------------------------------------------------ */

uint8_t player_left_col(void)
{
    return (uint8_t)(player_hx / 2u);
}

uint8_t player_right_col(void)
{
    if (player_hx & 1u) {
        /* Odd: 3-wide, right = left + 2 */
        uint8_t right = (uint8_t)((player_hx / 2u) + 2u);

        if (right > ROAD_RIGHT) return ROAD_RIGHT;
        return right;
    } else {
        /* Even: 2-wide, right = left + 1 */
        uint8_t right = (uint8_t)((player_hx / 2u) + 1u);

        if (right > ROAD_RIGHT) return ROAD_RIGHT;
        return right;
    }
}

/*
 * Determine the specific tile ID for a player cell based on:
 * 1. Parity (Even = 2-wide, Odd = 3-wide)
 * 2. Position within that shape (Left/Middle/Right column, Top/Bottom row)
 */
uint8_t get_player_tile_id(uint8_t x, uint8_t y)
{
    uint8_t left_col;
    uint8_t col_offset;
    uint16_t top_y;
    uint16_t bottom_y;

    left_col = player_left_col();

    /* Calculate relative column index (0 for leftmost, 1 for next, etc.) */
    if (x < left_col) return _TILE_5; /* Fallback, shouldn't happen */
    col_offset = (uint8_t)(x - left_col);

    top_y = (uint16_t)(YSize - 2u);
    bottom_y = (uint16_t)(YSize - 1u);

    if (!(player_hx & 1u)) {
        /* --- EVEN POSITION: 2 Columns Wide --- */
        /* Uses _TILE_5, _TILE_6, _TILE_7, _TILE_8 */

        if (y == top_y) {
            /* Top Row */
            if (col_offset == 0u) return _TILE_5; /* Top-Left */
            else                  return _TILE_6; /* Top-Right */
        }
        else if (y == bottom_y) {
            /* Bottom Row */
            if (col_offset == 0u) return _TILE_7; /* Bottom-Left */
            else                  return _TILE_8; /* Bottom-Right */
        }
    }
    else {
        /* --- ODD POSITION: 3 Columns Wide --- */
        /* Uses _TILE_9, _TILE_10, _TILE_11, _TILE_12, _TILE_13, _TILE_14 */

        if (y == top_y) {
            /* Top Row */
            if (col_offset == 0u)      return _TILE_9;  /* Top-Left */
            else if (col_offset == 1u) return _TILE_10; /* Top-Middle */
            else                       return _TILE_11; /* Top-Right */
        }
        else if (y == bottom_y) {
            /* Bottom Row */
            if (col_offset == 0u)      return _TILE_12; /* Bottom-Left */
            else if (col_offset == 1u) return _TILE_13; /* Bottom-Middle */
            else                       return _TILE_14; /* Bottom-Right */
        }
    }

    return _TILE_5; /* Fallback */
}

/* Draw one vertical car column at x. */
void draw_player_column(uint8_t x)
{
    uint8_t y_top;
    uint8_t y_bottom;
    uint8_t tile_id_top;
    uint8_t tile_id_bottom;

    y_top = (uint8_t)(YSize - 2);
    y_bottom = (uint8_t)(YSize - 1);

    /* Get the specific unique tiles for this exact cell */
    tile_id_top = get_player_tile_id(x, y_top);
    tile_id_bottom = get_player_tile_id(x, y_bottom);

    _XL_DRAW(x, y_top, tile_id_top, _XL_CYAN);     /* top tile   */
    _XL_DRAW(x, y_bottom, tile_id_bottom, _XL_CYAN);/* bottom tile*/
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
    uint8_t x;

    left  = player_left_col();
    right = player_right_col();

    for (x = left; x <= right; x++) {
        draw_player_column(x);
    }
}

/* Delete the complete current player shape and restore road underneath. */
void delete_player(void)
{
    uint8_t left;
    uint8_t right;
    uint8_t x;

    left  = player_left_col();
    right = player_right_col();

    for (x = left; x <= right; x++) {
        delete_player_column(x);
    }
}

/*
 * Move player one half-tile to the left.
 */
void player_move_left(void)
{
    if (player_hx == ROAD_LEFT * 2u) return;

    delete_player();
    player_hx--;
    draw_player();
}

/*
 * Move player one half-tile to the right.
 */
void player_move_right(void)
{
    if (player_hx >= (uint16_t)((ROAD_RIGHT - 1u) * 2u)) return;

    delete_player();
    player_hx++;
    draw_player();
}

/* ------------------------------------------------------------------ */
/*  Obstacle cars – 2 tiles wide, 2 tiles tall                         */
/* ------------------------------------------------------------------ */

uint8_t spawn_obstacle(void)
{
    uint8_t i;
    uint16_t span;
    uint16_t rx;

    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) {
            /*
             * Enemy car is 2 tiles wide.
             * x stores the top-left column, so valid range is:
             * ROAD_LEFT ... ROAD_RIGHT - 1
             */
            span = (uint16_t)(ROAD_RIGHT - ROAD_LEFT);
            rx   = _XL_RAND() % span;

            obstacles[i].x      = (uint8_t)(ROAD_LEFT + rx);
            obstacles[i].y      = 0;
            obstacles[i].active = 1;



            /* Top row of the 2x2 enemy car */
            _XL_DRAW(obstacles[i].x,        0, BIG_ENEMY_TL,  _XL_RED); /* top-left    */
            _XL_DRAW((uint8_t)(obstacles[i].x + 1u),
                     0, BIG_ENEMY_TR,  _XL_RED); /* top-right   */

            /* Bottom row of the 2x2 enemy car */
            if (1u < (uint8_t)YSize) {
                _XL_DRAW(obstacles[i].x,        1, BIG_ENEMY_BL, _XL_RED); /* bottom-left  */
                _XL_DRAW((uint8_t)(obstacles[i].x + 1u),
                         1, BIG_ENEMY_BR, _XL_RED); /* bottom-right */
            }

            return 1;
        }
    }

    return 0;
}

/* Delete a full 2x2 enemy car whose top-left corner is (x, y_top). */
void delete_obstacle_tiles(uint8_t x, uint8_t y_top)
{
    uint8_t right_x = (uint8_t)(x + 1u);
    uint8_t bottom_y;

    /* Top row */
    _XL_DELETE(x, y_top);
    draw_road_tile(x, y_top);

    if (right_x <= ROAD_RIGHT) {
        _XL_DELETE(right_x, y_top);
        draw_road_tile(right_x, y_top);
    }

    /* Bottom row */
    bottom_y = (uint8_t)(y_top + 1u);

    if (bottom_y < (uint8_t)YSize) {
        _XL_DELETE(x, bottom_y);
        draw_road_tile(x, bottom_y);

        if (right_x <= ROAD_RIGHT) {
            _XL_DELETE(right_x, bottom_y);
            draw_road_tile(right_x, bottom_y);
        }
    }
}

/* Draw a full 2x2 enemy car whose top-left corner is (x, y_top). */
void draw_obstacle_tiles(uint8_t x, uint8_t y_top)
{
    uint8_t right_x = (uint8_t)(x + 1u);
    uint8_t bottom_y;

    /* Top row */
    _XL_DRAW(x, y_top, BIG_ENEMY_TL, _XL_RED);       /* top-left   */

    if (right_x <= ROAD_RIGHT) {
        _XL_DRAW(right_x, y_top, BIG_ENEMY_TR, _XL_RED); /* top-right  */
    }

    /* Bottom row */
    bottom_y = (uint8_t)(y_top + 1u);

    if (bottom_y < (uint8_t)YSize) {
        _XL_DRAW(x, bottom_y, BIG_ENEMY_BL, _XL_RED);   /* bottom-left */

        if (right_x <= ROAD_RIGHT) {
            _XL_DRAW(right_x, bottom_y, BIG_ENEMY_BR, _XL_RED); /* bottom-right */
        }
    }
}

/* Lateral drift: move obstacle left or right by 1, clamped to road. */
short lateral_drift(uint8_t i)
{
    uint16_t r = _XL_RAND() & 0x0Fu;   /* 0=left, 1=stay, 2=right */

    if (r == 0u && obstacles[i].x > ROAD_LEFT) {
        return -1;
    } else if (r == 2u && (uint8_t)(obstacles[i].x + 1u) < ROAD_RIGHT) {
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

        /* Stop lateral drift near bottom */
        dx = (old_y >= (uint8_t)(YSize - 7u)) ? 0 : (short)lateral_drift(i);

        /* Move down */
        obstacles[i].y++;

        /* Apply lateral movement, clamped so the full 2-wide car stays on road */
        if (dx < 0) {
            if (obstacles[i].x > ROAD_LEFT) {
                obstacles[i].x--;
            }
        } else if (dx > 0) {
            if ((uint8_t)(obstacles[i].x + 1u) < ROAD_RIGHT) {
                obstacles[i].x++;
            }
        }

        /* Delete old 2x2 position and restore road underneath */
        delete_obstacle_tiles(old_x, old_y);

        if (obstacles[i].y >= (uint8_t)(YSize - 1u)) {
            obstacles[i].active = 0;
        } else {
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
    uint16_t obstacle_right_x;

    /*
     * Player occupies rows: YSize - 2 and YSize - 1.
     * Enemy car occupies rows: y and y + 1.
     * Vertical overlap begins when enemy top row is at or below YSize - 3.
     */
    y_limit = (uint8_t)(YSize - 3u);

    left  = player_left_col();
    right = player_right_col();

    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) continue;

        /* Enemy car occupies x and x + 1 */
        obstacle_right_x = (uint16_t)obstacles[i].x + 1u;

        if (obstacles[i].y >= y_limit &&
            obstacle_right_x >= left &&
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

    /* Start centred: left column at ROAD_CENTER - 1 (even, 2-wide). */
    if (ROAD_LEFT < ROAD_CENTER) {
        player_hx = (uint16_t)((ROAD_CENTER - 1u) * 2u);
    } else {
        player_hx = ROAD_LEFT * 2u;
    }

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
            input = _XL_INPUT();

            if (_XL_LEFT(input)) {
                player_move_left();
            } else if (_XL_RIGHT(input)) {
                player_move_right();
            }

            /* --- Spawn --- */
            spawn_timer++;
            if (spawn_timer >= SPAWN_TIME) {
                spawn_timer = 0;
                spawn_obstacle();
            }

            /* --- Update obstacles --- */
            update_obstacles();

            /* --- Collision --- */
            if (check_collision()) {
                game_over = 1;
                _XL_EXPLOSION_SOUND();

                delete_player();

                for (i = 0; i < MAX_OBSTACLES; i++) {
                    if (obstacles[i].active) {
                        delete_obstacle_tiles(obstacles[i].x, obstacles[i].y);
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
