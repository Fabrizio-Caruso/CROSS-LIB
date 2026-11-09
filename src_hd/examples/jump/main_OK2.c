#include "cross_lib.h"

#define GROUND_Y (YSize - 1)
#define CAR_X (XSize / 4)
#define JUMP_VEL 4
#define BONUS_HEIGHT 10
#define HOVER_FRAMES 6

#define BONUS_POINTS 100

int main(void) {
    uint8_t input;
    uint8_t jump_height;
    short jump_vel;
    uint8_t jump_state;
    uint8_t hover_frames;
    uint8_t frame_count;
    uint8_t wheel_frame;
    uint8_t speed_frames;
    uint8_t move_timer;
    uint16_t score;
    uint16_t distance;
    uint8_t obstacle_x;
    uint8_t obstacle_active;
    uint8_t obstacle_double;
    uint8_t bonus_x;
    uint8_t bonus_active;
    uint8_t bonus_y;
    uint8_t prev_jump_height;
    uint8_t obs_spawn_cd;
    uint8_t bonus_spawn_cd;
    uint16_t rand_val;
    uint8_t car_bottom_y;
    uint8_t car_mid_y;
    uint8_t car_top_y;
    uint8_t car_init;
    uint8_t fall;
    uint8_t wheel_tile;
    uint8_t game_over;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        jump_height = 0;
        jump_vel = 0;
        jump_state = 0;
        hover_frames = 0;
        frame_count = 0;
        speed_frames = 4;
        move_timer = 0;
        score = 0;
        distance = 0;
        obstacle_active = 0;
        obstacle_x = 0;
        obstacle_double = 0;
        bonus_active = 0;
        bonus_x = 0;
        bonus_y = 0;
        prev_jump_height = 0;
        obs_spawn_cd = 30;
        bonus_spawn_cd = 60;
        car_init = 0;
        car_bottom_y = 0;
        car_mid_y = 0;
        car_top_y = 0;
        fall = 0;
        wheel_tile = 0;
        game_over = 0;

        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 4, "CAR RUNNER");
        _XL_PRINT(2, 6, "PRESS FIRE TO JUMP");
        _XL_WAIT_FOR_INPUT();
        _XL_CLEAR_SCREEN();

        while (!game_over) {
            frame_count++;
            wheel_frame = frame_count % 4;

            switch (wheel_frame) {
                case 0: wheel_tile = _TILE_11; break;
                case 1: wheel_tile = _TILE_12; break;
                case 2: wheel_tile = _TILE_13; break;
                default: wheel_tile = _TILE_14; break;
            }

            input = _XL_INPUT();
            if (_XL_FIRE(input) && jump_state == 0) {
                jump_state = 1;
                jump_vel = JUMP_VEL;
                _XL_PING_SOUND();
            }

            if(!(frame_count&1))
            {
                /* --- Jump physics with hover at peak --- */
                if (jump_state == 1) {
                    /* Rising: velocity decreases each frame */
                    jump_height += (uint8_t)jump_vel;
                    jump_vel--;
                    if (jump_vel == 0) {
                        jump_state = 2;
                        hover_frames = HOVER_FRAMES;
                    }
                } else if (jump_state == 2) {
                    /* Hovering at peak: stay for HOVER_FRAMES ticks */
                    hover_frames--;
                    if (hover_frames == 0) {
                        jump_state = 3;
                        jump_vel = -1;
                    }
                } else if (jump_state == 3) {
                    /* Falling: speed increases each frame */
                    fall = (uint8_t)(-jump_vel);
                    if (fall >= jump_height) {
                        jump_height = 0;
                        jump_state = 0;
                        _XL_TOCK_SOUND();
                    } else {
                        jump_height -= fall;
                    }
                    jump_vel--;
                }
            }
            /* Ramp up speed every 100 points */
            if (distance > 0 && (distance % 100) == 0) {
                if (speed_frames > 2) speed_frames--;
            }

            /* Scroll the world left */
            move_timer++;
            if (move_timer >= speed_frames) {
                move_timer = 0;
                distance++;

                if (obstacle_active) {
                    _XL_DELETE(obstacle_x, GROUND_Y);
                    if (obstacle_double) {
                        _XL_DELETE(obstacle_x, GROUND_Y - 1);
                    }
                    obstacle_x--;
                    if (obstacle_x == 0) {
                        obstacle_active = 0;
                        rand_val = _XL_RAND();
                        obs_spawn_cd = (uint8_t)(rand_val % 25) + 15;
                    } else {
                        _XL_DRAW(obstacle_x, GROUND_Y, _TILE_0, _XL_RED);
                        if (obstacle_double) {
                            _XL_DRAW(obstacle_x, GROUND_Y - 1, _TILE_0, _XL_RED);
                        }
                    }
                } else {
                    if (obs_spawn_cd > 0) {
                        obs_spawn_cd--;
                        if (obs_spawn_cd == 0) {
                            obstacle_active = 1;
                            obstacle_x = XSize - 1;
                            rand_val = _XL_RAND();
                            obstacle_double = (uint8_t)(rand_val & 1);
                            _XL_DRAW(obstacle_x, GROUND_Y, _TILE_0, _XL_RED);
                            if (obstacle_double) {
                                _XL_DRAW(obstacle_x, GROUND_Y - 1, _TILE_0, _XL_RED);
                            }
                        }
                    }
                }

                if (bonus_active) {
                    _XL_DELETE(bonus_x, bonus_y);
                    bonus_x--;
                    if (bonus_x == 0) {
                        bonus_active = 0;
                        bonus_spawn_cd = 50;
                    } else {
                        _XL_DRAW(bonus_x, bonus_y, _TILE_20, _XL_YELLOW);
                    }
                } else {
                    if (bonus_spawn_cd > 0) {
                        bonus_spawn_cd--;
                        if (bonus_spawn_cd == 0) {
                            bonus_active = 1;
                            bonus_x = XSize - 1;
                            bonus_y = GROUND_Y - BONUS_HEIGHT;
                            _XL_DRAW(bonus_x, bonus_y, _TILE_20, _XL_YELLOW);
                        }
                    }
                }
            }

            /* HUD */
            score = distance;
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINTD(0, 0, 4, score);

            /* Draw car (only redraw when height changes, else just spin wheels) */
            car_bottom_y = GROUND_Y - jump_height;
            car_mid_y = GROUND_Y - jump_height - 1;
            car_top_y = GROUND_Y - jump_height - 2;

            if (jump_height != prev_jump_height || !car_init) {
                if (car_init) {
                    _XL_DELETE(CAR_X, GROUND_Y - prev_jump_height);
                    _XL_DELETE(CAR_X + 3, GROUND_Y - prev_jump_height);
                    _XL_DELETE(CAR_X, GROUND_Y - prev_jump_height - 1);
                    _XL_DELETE(CAR_X + 1, GROUND_Y - prev_jump_height - 1);
                    _XL_DELETE(CAR_X + 2, GROUND_Y - prev_jump_height - 1);
                    _XL_DELETE(CAR_X + 3, GROUND_Y - prev_jump_height - 1);
                    _XL_DELETE(CAR_X + 1, GROUND_Y - prev_jump_height - 2);
                    _XL_DELETE(CAR_X + 2, GROUND_Y - prev_jump_height - 2);
                }
                /* Wheels (animated) */
                _XL_DRAW(CAR_X, car_bottom_y, wheel_tile, _XL_CYAN);
                _XL_DRAW(CAR_X + 3, car_bottom_y, wheel_tile, _XL_CYAN);
                /* Bottom body: 4 distinct tiles */
                _XL_DRAW(CAR_X, car_mid_y, _TILE_5, _XL_CYAN);
                _XL_DRAW(CAR_X + 1, car_mid_y, _TILE_6, _XL_CYAN);
                _XL_DRAW(CAR_X + 2, car_mid_y, _TILE_7, _XL_CYAN);
                _XL_DRAW(CAR_X + 3, car_mid_y, _TILE_8, _XL_CYAN);
                /* Top: 2 central tiles */
                _XL_DRAW(CAR_X + 1, car_top_y, _TILE_9, _XL_CYAN);
                _XL_DRAW(CAR_X + 2, car_top_y, _TILE_10, _XL_CYAN);

                prev_jump_height = jump_height;
                car_init = 1;
            } else {
                /* Height unchanged (grounded or hovering): only spin wheels */
                _XL_DELETE(CAR_X, car_bottom_y);
                _XL_DELETE(CAR_X + 3, car_bottom_y);
                _XL_DRAW(CAR_X, car_bottom_y, wheel_tile, _XL_CYAN);
                _XL_DRAW(CAR_X + 3, car_bottom_y, wheel_tile, _XL_CYAN);
            }

            /* Collision with obstacle */
            if (obstacle_active) {
                if (obstacle_x >= CAR_X && obstacle_x <= CAR_X + 3) {
                    if (obstacle_double) {
                        if (jump_height <= 1) {
                            game_over = 1;
                            _XL_EXPLOSION_SOUND();
                        }
                    } else {
                        if (jump_height == 0) {
                            game_over = 1;
                            _XL_EXPLOSION_SOUND();
                        }
                    }
                }
            }

            /* Collect bonus (must be mid-jump to reach it) */
            if (bonus_active && !game_over) {
                if (bonus_x >= CAR_X && bonus_x <= CAR_X + 3) {
                    if (bonus_y == car_bottom_y || bonus_y == car_mid_y || bonus_y == car_top_y) {
                        bonus_active = 0;
                        _XL_DELETE(bonus_x, bonus_y);
                        score += BONUS_POINTS;
                        _XL_SHOOT_SOUND();
                    }
                }
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* Game over screen */
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(2, 4, "GAME OVER");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 6, "SCORE");
        _XL_PRINTD(8, 6, 4, score);
        _XL_PRINT(2, 8, "PRESS ANY KEY TO RESTART");
        _XL_WAIT_FOR_INPUT();
    }
}

