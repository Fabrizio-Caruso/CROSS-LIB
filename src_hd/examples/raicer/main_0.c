#include "cross_lib.h"

#define MAX_OBSTACLES 8

#define ROAD_WIDTH ((XSize)/5)

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
    for (uint8_t x = ROAD_LEFT; x <= ROAD_RIGHT; x++) {
        for (uint8_t y = 0; y < YSize; y++) {
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
    draw_road_tile(x, YSize - 2);               /* restore road */
    _XL_DELETE(x, YSize - 1);
    draw_road_tile(x, YSize - 1);
}

/* ------------------------------------------------------------------ */
/*  Obstacle cars – two tiles tall                                     */
/* ------------------------------------------------------------------ */

uint8_t spawn_obstacle(void)
{
    for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) {
            uint16_t span = (uint16_t)(ROAD_RIGHT - ROAD_LEFT + 1);
            uint16_t rx   = _XL_RAND() % span;
            obstacles[i].x      = (uint8_t)(ROAD_LEFT + rx);
            obstacles[i].y      = 0;
            obstacles[i].active = 1;

            /* Draw both tiles of the car */
            _XL_DRAW(obstacles[i].x, 0, _TILE_3, _XL_RED);   /* top    */
            _XL_DRAW(obstacles[i].x, 1, _TILE_4, _XL_RED);   /* bottom */
            return 1;
        }
    }
    return 0;
}

void update_obstacles(void)
{
    for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) continue;

        uint8_t old_y = obstacles[i].y;
        obstacles[i].y++;

        /* Delete old top tile, restore road underneath */
        _XL_DELETE(obstacles[i].x, old_y);
        draw_road_tile(obstacles[i].x, old_y);

        if (old_y + 1 >= YSize - 1) {
            /* Bottom tile was at the last row – car leaves screen */
            obstacles[i].active = 0;
            draw_road_tile(obstacles[i].x, old_y + 1);
        } else {
            /* Delete old bottom tile, restore road */
            _XL_DELETE(obstacles[i].x, old_y + 1);
            draw_road_tile(obstacles[i].x, old_y + 1);

            /* Draw new positions */
            _XL_DRAW(obstacles[i].x, obstacles[i].y,     _TILE_3, _XL_RED);
            _XL_DRAW(obstacles[i].x, obstacles[i].y + 1, _TILE_4, _XL_RED);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Collision: obstacle rows [y, y+1] overlap player rows             */
/*              YSize-2 .. YSize-1                                    */
/* ------------------------------------------------------------------ */
uint8_t check_collision(void)
{
    for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
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
    player_x    = ROAD_CENTER;
    score       = 0;
    spawn_timer = 0;

    for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
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

    /* Draw the static road once */
    draw_road();

    init_game();
    draw_player_at(player_x);

    /* HUD text (uses PRINT, not DRAW – no tile conflict) */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, YSize - 5, "LEFT AND RIGHT TO STEER");
    _XL_PRINT(0, YSize - 6, "AVOID THE RED CARS");
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINTD(0, 0, 1, score);

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

        /* --- Spawn obstacles on a timer --- */
        spawn_timer++;
        if (spawn_timer >= 2) {
            spawn_timer = 0;
            spawn_obstacle();
        }

        /* --- Move all active cars down one row --- */
        update_obstacles();

        /* --- Collision check --- */
        if (check_collision()) {
            game_over = 1;
            _XL_EXPLOSION_SOUND();

            delete_player_at(player_x);

            /* Clean up remaining obstacles, restoring road tiles */
            for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
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
        } else {
            score++;
            _XL_PRINTD(0, 0, 1, score);
        }

        /* --- Frame pacing --- */
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}
