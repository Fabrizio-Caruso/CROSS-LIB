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

/*
 * Player car – two tiles tall, half-tile horizontal movement.
 *   Even player_hx = 2n  =>  2-wide car: columns [n, n+1]
 *   Odd  player_hx = 2n+1 => 3-wide car: columns [n, n+1, n+2]
 *
 * Each half-tile step changes exactly one column at an edge.
 */

typedef struct {
    uint8_t x;
    uint8_t y;       /* top tile of the car */
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
 *   even(2n)  -> odd(2n-1):  old [n, n+1]       new [n-1, n, n+1]   => add col n-1
 *   odd (2n+1)-> even(2n) :  old [n, n+1, n+2]  new [n, n+1]         => del col n+2
 */
void player_move_left(void)
{
    uint8_t old_right;

    if (player_hx == ROAD_LEFT * 2u) return;

    old_right = player_right_col();

    if (player_hx & 1u) {
        /* Odd -> Even: remove the rightmost column */
        player_hx--;
        delete_player_column(old_right);
    } else {
        /* Even -> Odd: add a new leftmost column */
        player_hx--;
        draw_player_column(player_left_col());
    }
}

/*
 * Move player one half-tile to the right.
 *   even(2n)  -> odd(2n+1):  old [n, n+1]       new [n, n+1, n+2]   => add col n+2
 *   odd (2n+1)-> even(2n+2): old [n, n+1, n+2]  new [n+1, n+2]      => del col n
 */
void player_move_right(void)
{
    uint8_t old_left;

    if (player_hx >= (uint16_t)((ROAD_RIGHT - 1u) * 2u)) return;

    old_left = player_left_col();

    if (!(player_hx & 1u)) {
        /* Even -> Odd: add a new rightmost column */
        player_hx++;
        draw_player_column(player_right_col());
    } else {
        /* Odd -> Even: remove the leftmost column */
        delete_player_column(old_left);
        player_hx++;
    }
}

/* ------------------------------------------------------------------ */
/*  Obstacle cars – two tiles tall, move within road                   */
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

void delete_obstacle_tiles(uint8_t x, uint8_t y_top)
{
    _XL_DELETE(x, y_top);
    draw_road_tile(x, y_top);

    if ((uint16_t)y_top + 1u < (uint16_t)YSize) {
        _XL_DELETE(x, (uint8_t)(y_top + 1));
        draw_road_tile(x, (uint8_t)(y_top + 1));
    }
}

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

        /* Stop lateral drift near bottom */
        dx = (old_y >= (uint8_t)(YSize - 7u)) ? 0 : (short)lateral_drift(i);

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
