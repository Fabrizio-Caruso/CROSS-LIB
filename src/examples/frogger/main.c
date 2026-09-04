#include "cross_lib.h"

#define FROG_START_X  15
#define FROG_START_Y  8
#define GOAL_ROW      1
#define SAFE_ROW      8
#define NUM_OBS       12
#define POLE_COUNT    6

static uint8_t frog_x;
static uint8_t frog_y;
static uint8_t lives;
static uint16_t score;
static uint8_t game_over;
static uint8_t level_complete;

static uint8_t obs_x[NUM_OBS];
static uint8_t obs_dir[NUM_OBS];
static uint8_t obs_lane[NUM_OBS];

static uint8_t pothole_x[POLE_COUNT];
static uint8_t pothole_filled[POLE_COUNT];

static void draw_frog(void)
{
    _XL_DRAW(frog_x, frog_y, _TILE_0, _XL_GREEN);
}

static void draw_obstacles(void)
{
    uint8_t i;
    for (i = 0; i < NUM_OBS; i++) {
        if (obs_lane[i] >= 2 && obs_lane[i] <= 7) {
            if (obs_lane[i] >= 5) {
                _XL_DRAW(obs_x[i], obs_lane[i], _TILE_1, _XL_RED);
            } else {
                _XL_DRAW(obs_x[i], obs_lane[i], _TILE_2, _XL_YELLOW);
            }
        }
    }
}

static void draw_goals(void)
{
    uint8_t i;
    for (i = 0; i < POLE_COUNT; i++) {
        if (pothole_filled[i] == 0) {
            _XL_DRAW(pothole_x[i], GOAL_ROW, _TILE_3, _XL_CYAN);
        }
    }
}

static void draw_score(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "SCORE");
    _XL_PRINTD(6, 0, 1, score);
    _XL_PRINT(15, 0, "LIVES");
    _XL_PRINTD(21, 0, 1, lives);
}

static void move_obstacles(void)
{
    uint8_t i;
    for (i = 0; i < NUM_OBS; i++) {
        if (obs_dir[i] == 1) {
            obs_x[i]++;
            if (obs_x[i] >= XSize) obs_x[i] = 0;
        } else {
            if (obs_x[i] == 0) obs_x[i] = XSize - 1;
            else obs_x[i]--;
        }
    }
}

static uint8_t check_collision(void)
{
    uint8_t i;
    uint8_t hit = 0;
    for (i = 0; i < NUM_OBS; i++) {
        if (frog_y == obs_lane[i] && frog_x == obs_x[i]) {
            hit = 1;
        }
    }
    return hit;
}

static uint8_t on_pothole(uint8_t x)
{
    uint8_t i;
    uint8_t found = 0;
    for (i = 0; i < POLE_COUNT; i++) {
        if (x == pothole_x[i] && pothole_filled[i] == 0) {
            found = 1;
        }
    }
    return found;
}

static uint8_t all_potholes_filled(void)
{
    uint8_t i;
    uint8_t all = 1;
    for (i = 0; i < POLE_COUNT; i++) {
        if (pothole_filled[i] == 0) {
            all = 0;
        }
    }
    return all;
}

static void reset_frog(void)
{
    frog_x = FROG_START_X;
    frog_y = FROG_START_Y;
}

static void init_obstacles(void)
{
    obs_x[0] = 2;  obs_dir[0] = 1; obs_lane[0] = 2;
    obs_x[1] = 12; obs_dir[1] = 1; obs_lane[1] = 2;
    obs_x[2] = 22; obs_dir[2] = 1; obs_lane[2] = 2;

    obs_x[3] = 5;  obs_dir[3] = 2; obs_lane[3] = 3;
    obs_x[4] = 15; obs_dir[4] = 2; obs_lane[4] = 3;
    obs_x[5] = 25; obs_dir[5] = 2; obs_lane[5] = 3;

    obs_x[6] = 0;  obs_dir[6] = 1; obs_lane[6] = 4;
    obs_x[7] = 10; obs_dir[7] = 1; obs_lane[7] = 4;
    obs_x[8] = 20; obs_dir[8] = 1; obs_lane[8] = 4;

    obs_x[9] = 3;  obs_dir[9] = 1; obs_lane[9] = 5;
    obs_x[10] = 13; obs_dir[10] = 1; obs_lane[10] = 5;
    obs_x[11] = 23; obs_dir[11] = 1; obs_lane[11] = 5;
}

static void init_game(void)
{
    uint8_t i;
    lives = 3;
    score = 0;
    game_over = 0;
    level_complete = 0;
    reset_frog();
    init_obstacles();

    pothole_x[0] = 5;
    pothole_x[1] = 10;
    pothole_x[2] = 15;
    pothole_x[3] = 20;
    pothole_x[4] = 25;
    pothole_x[5] = 30;

    for (i = 0; i < POLE_COUNT; i++) {
        pothole_filled[i] = 0;
    }
}

void main(void)
{
    uint8_t input;
    uint8_t i;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    init_game();

    while (1) {
        if (game_over || level_complete) {
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_RED);
            if (level_complete) {
                _XL_PRINT(8, 4, "YOU WIN");
            } else {
                _XL_PRINT(10, 4, "GAME OVER");
            }
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(10, 6, "SCORE");
            _XL_PRINTD(10, 7, 1, score);
            _XL_PRINT(8, 9, "PRESS FIRE TO RESTART");

            input = _XL_INPUT();
            if (_XL_FIRE(input)) {
                init_game();
            }
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            continue;
        }

        /* --- render --- */
        _XL_CLEAR_SCREEN();
        draw_score();
        draw_goals();
        draw_obstacles();
        draw_frog();

        /* --- input --- */
        input = _XL_INPUT();
        if (_XL_LEFT(input)) {
            if (frog_x > 0) {
                frog_x--;
                _XL_PING_SOUND();
            }
        }
        if (_XL_RIGHT(input)) {
            if (frog_x < XSize - 1) {
                frog_x++;
                _XL_PING_SOUND();
            }
        }
        if (_XL_UP(input)) {
            if (frog_y > GOAL_ROW) {
                frog_y--;
                _XL_TICK_SOUND();
            }
        }
        if (_XL_DOWN(input)) {
            if (frog_y < SAFE_ROW) {
                frog_y++;
                _XL_TICK_SOUND();
            }
        }

        /* --- check goal row --- */
        if (frog_y == GOAL_ROW) {
            if (on_pothole(frog_x)) {
                /* mark this pothole as filled */
                for (i = 0; i < POLE_COUNT; i++) {
                    if (frog_x == pothole_x[i] && pothole_filled[i] == 0) {
                        pothole_filled[i] = 1;
                        break;
                    }
                }
                score = score + 100;
                _XL_ZAP_SOUND();
                reset_frog();

                /* check win condition */
                if (all_potholes_filled()) {
                    level_complete = 1;
                }
            } else {
                lives--;
                _XL_EXPLOSION_SOUND();
                if (lives == 0) {
                    game_over = 1;
                } else {
                    reset_frog();
                }
            }
        }

        /* --- move obstacles --- */
        move_obstacles();

        /* --- collision --- */
        if (!game_over && !level_complete && frog_y > GOAL_ROW && frog_y < SAFE_ROW) {
            if (check_collision()) {
                lives--;
                _XL_EXPLOSION_SOUND();
                if (lives == 0) {
                    game_over = 1;
                } else {
                    reset_frog();
                }
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}