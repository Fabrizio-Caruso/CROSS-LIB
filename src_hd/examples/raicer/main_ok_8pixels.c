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

/* Narrow road: 5 columns centred on screen */
#define ROAD_LEFT   ((uint8_t)(XSize / 2 - (ROAD_WIDTH/2)))
#define ROAD_RIGHT  ((uint8_t)(XSize / 2 + (ROAD_WIDTH/2)))
#define ROAD_CENTER ((uint8_t)(XSize / 2))

typedef struct {
    uint8_t x;
    uint8_t y;       /* top tile of the car */
    uint8_t active;
} Obstacle;

Obstacle obstacles[MAX_OBSTACLES];
uint8_t player_x;
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
/*  Player car – two tiles tall                                        */
/* ------------------------------------------------------------------ */

void draw_player_at(uint8_t x)
{
    _XL_DRAW(x, YSize - 2, _TILE_6, _XL_CYAN);  /* top tile   */
    _XL_DRAW(x, YSize - 1, _TILE_5, _XL_CYAN);  /* bottom tile*/
}

void delete_player_at(uint8_t x)
{
    _XL_DELETE(x, YSize - 2);
    draw_road_tile(x, YSize - 2);
    _XL_DELETE(x, YSize - 1);
    draw_road_tile(x, YSize - 1);
}

/* ------------------------------------------------------------------ */
/*  Obstacle cars – two tiles tall, move within road                  */
/* ------------------------------------------------------------------ */

uint8_t spawn_obstacle(void)
{
    uint8_t i;
    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) {
            uint16_t span = (uint16_t)(ROAD_RIGHT - ROAD_LEFT + 1);
            uint16_t rx   = _XL_RAND() % span;
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
    if (y_top + 1 < YSize) {
        _XL_DRAW(x, y_top + 1, _TILE_4, _XL_RED); /* placeholder – overwritten below */
        _XL_DELETE(x, y_top + 1);
        draw_road_tile(x, y_top + 1);
    }
}

/* Draw both tiles of an obstacle at (x, y_top) */
void draw_obstacle_tiles(uint8_t x, uint8_t y_top)
{
    _XL_DRAW(x, y_top,     _TILE_3, _XL_RED);
    if (y_top + 1 < YSize) {
        _XL_DRAW(x, y_top + 1, _TILE_4, _XL_RED);
    }
}

/* Lateral drift: move obstacle left or right by 1, clamped to road */
short lateral_drift(uint8_t i)
{
    uint16_t r = _XL_RAND()&15;   /* 0=left, 1=stay, 2=right */

    if (r == 0 && obstacles[i].x > ROAD_LEFT) {
        return -1;
    } else if (r == 2 && obstacles[i].x < ROAD_RIGHT) {
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
        dx     = (old_y>=(YSize-7) ? 0 : (int8_t)lateral_drift(i));

        /* Move down */
        obstacles[i].y++;

        /* Apply lateral movement, clamped to road */
        if (dx < 0) {
            obstacles[i].x--;
        } else if (dx > 0) {
            obstacles[i].x++;
        }

        /* Delete old position tiles, restore road */
        _XL_DELETE(old_x, old_y);
        draw_road_tile(old_x, old_y);
        _XL_DELETE(old_x, old_y + 1);
        draw_road_tile(old_x, old_y + 1);

        if (obstacles[i].y >= YSize - 1) {
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
    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].active &&
            obstacles[i].x == player_x &&
            obstacles[i].y >= (uint8_t)(YSize - 3)) {
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
    player_x    = ROAD_CENTER;
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

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    for(;;)
    {
        _XL_CLEAR_SCREEN();

        init_game();


        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, YSize - 5, "LEFT AND RIGHT TO STEER");
        _XL_PRINT(0, YSize - 6, "AVOID THE RED CARS");
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD(0, 0, 1, score);

        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();
        _XL_CLEAR_SCREEN();
        draw_road();
        draw_player_at(player_x);
        while (!game_over) {
            /* --- Input --- */
            uint8_t input = _XL_INPUT();

            if (_XL_LEFT(input)) {
                if (player_x > ROAD_LEFT) {
                    delete_player_at(player_x);
                    player_x--;
                    draw_player_at(player_x);
                    _XL_TICK_SOUND();
                }
            } else if (_XL_RIGHT(input)) {
                if (player_x < ROAD_RIGHT) {
                    delete_player_at(player_x);
                    player_x++;
                    draw_player_at(player_x);
                    _XL_TICK_SOUND();
                }
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
                uint8_t i;
                game_over = 1;
                _XL_EXPLOSION_SOUND();

                delete_player_at(player_x);

                for (i = 0; i < MAX_OBSTACLES; i++) {
                    if (obstacles[i].active) {
                        _XL_DELETE(obstacles[i].x, obstacles[i].y);
                        draw_road_tile(obstacles[i].x, obstacles[i].y);
                        _XL_DELETE(obstacles[i].x, obstacles[i].y + 1);
                        draw_road_tile(obstacles[i].x, obstacles[i].y + 1);
                        obstacles[i].active = 0;
                    }
                }

                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(ROAD_LEFT, YSize / 2, "GAME OVER");
                _XL_SLEEP(1);
                _XL_WAIT_FOR_INPUT();
            } else {
                score++;
                _XL_PRINTD(0, 0, 1, score);
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
        game_over = 0;
    }
    return 0;
}
