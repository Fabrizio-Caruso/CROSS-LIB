#include "cross_lib.h"

#define GROUND_Y (YSize - 1)
#define CAR_X (XSize / 4)
#define JUMP_VEL 6
#define BONUS_HEIGHT 10
#define HOVER_FRAMES 2
#define MAX_OBJS 5

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
    uint8_t bonus_x;
    uint8_t bonus_active;
    uint8_t bonus_y;
    uint8_t prev_jump_height;
    uint8_t bonus_spawn_cd;
    uint16_t rand_val;
    uint8_t car_bottom_y;
    uint8_t car_mid_y;
    uint8_t car_top_y;
    uint8_t car_init;
    uint8_t fall;
    uint8_t wheel_tile;
    uint8_t game_over;
    uint8_t h;
    uint8_t x;
    uint8_t i;
    uint8_t collected;
    uint8_t spawn_wait;
    uint8_t cluster_size;
    uint8_t cluster_count;
    uint8_t obs_x[MAX_OBJS];
    uint8_t obs_h[MAX_OBJS];
    uint8_t obs_active[MAX_OBJS];
    uint8_t slot;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        jump_height = 0;
        jump_vel = 0;
        jump_state = 0;
        hover_frames = 0;
        frame_count = 0;
        speed_frames = 0;
        move_timer = 0;
        score = 0;
        distance = 0;
        bonus_active = 0;
        bonus_x = 0;
        bonus_y = 0;
        prev_jump_height = 0;
        bonus_spawn_cd = 25;
        car_init = 0;
        car_bottom_y = 0;
        car_mid_y = 0;
        car_top_y = 0;
        fall = 0;
        wheel_tile = 0;
        game_over = 0;
        spawn_wait = 10;
        cluster_size = 1;
        cluster_count = 0;
        collected = 0;

        for (i = 0; i < MAX_OBJS; i++) {
            obs_active[i] = 0;
            obs_x[i] = 0;
            obs_h[i] = 0;
        }

        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, 4, "CAR RUNNER");
        _XL_PRINT(2, 6, "PRESS FIRE TO JUMP");
        _XL_WAIT_FOR_INPUT();
        _XL_CLEAR_SCREEN();

        /* Draw the road */
        for (x = 0; x < XSize; x++) {
            _XL_DRAW(x, GROUND_Y, _TILE_0, _XL_BLUE);
        }

        while (!game_over) {
            frame_count++;
            wheel_frame = frame_count % 4;

            switch (wheel_frame) {
                case 0: wheel_tile = _TILE_11; break;
                case 1: wheel_tile = _TILE_12; break;
                case 2: wheel_tile = _TILE_13; break;
                default: wheel_tile = _TILE_14; break;
            }

            /* --- Input --- */
            input = _XL_INPUT();
            if (_XL_FIRE(input) && jump_state == 0) {
                jump_state = 1;
                jump_vel = JUMP_VEL;
                _XL_PING_SOUND();
            }

            /* --- Jump physics --- */
            if (jump_state == 1) {
                jump_height += (uint8_t)jump_vel;
                jump_vel--;
                if (jump_vel == 0) {
                    jump_state = 2;
                    hover_frames = HOVER_FRAMES;
                }
            } else if (jump_state == 2) {
                hover_frames--;
                if (hover_frames == 0) {
                    jump_state = 3;
                    jump_vel = -1;
                }
            } else if (jump_state == 3) {
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

            /* Ramp up speed every 100 points */
            // if (distance > 0 && (distance % 100) == 0) {
                // if (speed_frames > 2) speed_frames--;
            // }

            /* --- Scroll world --- */
            move_timer++;
            if (move_timer >= speed_frames) {
                move_timer = 0;
                distance++;

                /* Move all active obstacles left */
                for (i = 0; i < MAX_OBJS; i++) {
                    if (!obs_active[i]) continue;

                    for (h = 0; h < obs_h[i]; h++) {
                        _XL_DELETE(obs_x[i], GROUND_Y - h);
                    }
                    _XL_DRAW(obs_x[i], GROUND_Y, _TILE_0, _XL_BLUE);

                    obs_x[i]--;
                    if (obs_x[i] == 0) {
                        obs_active[i] = 0;
                    } else {
                        for (h = 0; h < obs_h[i]; h++) {
                            _XL_DRAW(obs_x[i], GROUND_Y - h, _TILE_1, _XL_RED);
                        }
                    }
                }

                /* Spawn logic */
                if (spawn_wait > 0) {
                    spawn_wait--;
                }
                if (spawn_wait == 0) {
                    /* Find a free slot and spawn */
                    slot = 0;
                    while (slot < MAX_OBJS && obs_active[slot]) slot++;
                    if (slot < MAX_OBJS) {
                        obs_active[slot] = 1;
                        obs_x[slot] = XSize - 1;
                        rand_val = _XL_RAND();
                        obs_h[slot] = (uint8_t)(rand_val % 3) + 2;
                        for (h = 0; h < obs_h[slot]; h++) {
                            _XL_DRAW(obs_x[slot], GROUND_Y - h, _TILE_1, _XL_RED);
                        }
                    }

                    /* Advance cluster tracking */
                    cluster_count++;
                    if (cluster_count < cluster_size) {
                        /* More obstacles in this group: 2-tile gap */
                        spawn_wait = 2;
                    } else {
                        /* Group complete: wide gap, pick next group size */
                        cluster_count = 0;
                        rand_val = _XL_RAND();
                        if (rand_val < 10923) {
                            cluster_size = 1;
                        } else if (rand_val < 21846) {
                            cluster_size = 2;
                        } else {
                            cluster_size = 3;
                        }
                        spawn_wait = (uint8_t)(15 + (rand_val % 11));
                    }
                }

                /* Move bonus */
                if (bonus_active) {
                    _XL_DELETE(bonus_x, bonus_y);
                    bonus_x--;
                    if (bonus_x == 0) {
                        bonus_active = 0;
                        bonus_spawn_cd = 25;
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

            /* --- HUD --- */
            score = distance;
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINTD(0, 0, 4, score);

            /* --- Draw car --- */
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
                _XL_DRAW(CAR_X, car_bottom_y, wheel_tile, _XL_CYAN);
                _XL_DRAW(CAR_X + 3, car_bottom_y, wheel_tile, _XL_CYAN);
                _XL_DRAW(CAR_X, car_mid_y, _TILE_5, _XL_CYAN);
                _XL_DRAW(CAR_X + 1, car_mid_y, _TILE_6, _XL_CYAN);
                _XL_DRAW(CAR_X + 2, car_mid_y, _TILE_7, _XL_CYAN);
                _XL_DRAW(CAR_X + 3, car_mid_y, _TILE_8, _XL_CYAN);
                _XL_DRAW(CAR_X + 1, car_top_y, _TILE_9, _XL_CYAN);
                _XL_DRAW(CAR_X + 2, car_top_y, _TILE_10, _XL_CYAN);
                prev_jump_height = jump_height;
                car_init = 1;
            } else {
                _XL_DELETE(CAR_X, car_bottom_y);
                _XL_DELETE(CAR_X + 3, car_bottom_y);
                _XL_DRAW(CAR_X, car_bottom_y, wheel_tile, _XL_CYAN);
                _XL_DRAW(CAR_X + 3, car_bottom_y, wheel_tile, _XL_CYAN);
            }

            /* --- Obstacle collision --- */
            for (i = 0; i < MAX_OBJS; i++) {
                if (!obs_active[i]) continue;
                if (obs_x[i] >= CAR_X && obs_x[i] <= CAR_X + 3) {
                    if (jump_height < obs_h[i]) {
                        game_over = 1;
                        _XL_EXPLOSION_SOUND();
                    }
                }
            }

            /* --- Bonus collision (shape-aware, checked every frame) --- */
            collected = 0;
            if (bonus_active && !game_over) {
                /* Check if bonus tile overlaps any car tile */
                if (bonus_y == car_bottom_y) {
                    /* Bottom row: only wheels at CAR_X and CAR_X+3 */
                    if (bonus_x == CAR_X || bonus_x == CAR_X + 3) collected = 1;
                } else if (bonus_y == car_mid_y) {
                    /* Mid row: full body CAR_X through CAR_X+3 */
                    if (bonus_x >= CAR_X && bonus_x <= CAR_X + 3) collected = 1;
                } else if (bonus_y == car_top_y) {
                    /* Top row: roof at CAR_X+1 and CAR_X+2 */
                    if (bonus_x == CAR_X + 1 || bonus_x == CAR_X + 2) collected = 1;
                }
                if (collected) {
                    bonus_active = 0;
                    _XL_DELETE(bonus_x, bonus_y);
                    score += 200;
                    _XL_SHOOT_SOUND();
                }
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* --- Game over --- */
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

