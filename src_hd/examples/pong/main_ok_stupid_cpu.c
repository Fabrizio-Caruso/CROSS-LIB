#include "cross_lib.h"

/* Ball moves once every BALL_STEP frames at the start of a match. */
#define BALL_STEP 8
#define MIN_BALL_STEP 2
#define MAX_SPEEDUP_STEPS 12

/* Pause shown after each point, before waiting for input. */
#define POINT_PAUSE_SECONDS 1

/* Paddle size in tiles. Change this value to make paddles bigger or smaller. */
#define PADDLE_HEIGHT 5

/* CPU AI tuning. */
#define CPU_DEAD_ZONE 1       /* Rows around the target where CPU does not move. */
#define CPU_RETURN_TO_CENTER 1 /* 1 = drift back to center when ball moves away. */

/* Playfield boundaries. */
#define LEFT_BORDER_X    0
#define RIGHT_BORDER_X   (XSize - 1)
#define TOP_BORDER_Y     2
#define BOTTOM_BORDER_Y  (YSize - 1)

/* Inner playable limits, inside the border. */
#define MIN_PLAY_X       1
#define MAX_PLAY_X       (XSize - 2)
#define MIN_PLAY_Y       (TOP_BORDER_Y + 1)
#define MAX_PLAY_Y       (BOTTOM_BORDER_Y - 1)

/* Game states. */
#define STATE_PLAYING     0
#define STATE_POINT_PAUSE 4
#define STATE_GAME_OVER   3

uint8_t paddle_h; /* Effective paddle height, clamped to the available space. */

uint8_t player_y;
uint8_t computer_y;

uint8_t ball_x;
uint8_t ball_y;
uint8_t ball_dx; /* 0 = left, 1 = right */
uint8_t ball_dy; /* 0 = up,   1 = down */

/* Previous positions used to know what must be deleted. */
uint8_t prev_player_y;
uint8_t prev_computer_y;
uint8_t prev_ball_x;
uint8_t prev_ball_y;

uint8_t player_score;
uint8_t computer_score;

uint8_t last_player_score;
uint8_t last_computer_score;

uint8_t game_state;
uint8_t ball_timer;

/* Ball speed gradually increases. */
uint8_t ball_step;
uint8_t speedup_count;

void draw_paddle(uint8_t x, uint8_t y) {
    uint8_t i;

    if (x >= XSize || y >= YSize) return;

    for (i = 0; i < paddle_h; i++) {
        if (y + i < YSize) {
            _XL_DRAW(x, y + i, _TILE_1, _XL_WHITE);
        }
    }
}

/*
 * Move a vertical paddle by one tile.
 * Only the old edge tile is deleted and only the new edge tile is drawn.
 */
void move_paddle(uint8_t x, uint8_t old_y, uint8_t new_y) {
    if (old_y == new_y) return;

    /* Moving up: remove old bottom tile, add new top tile. */
    if (new_y < old_y) {
        _XL_DELETE(x, old_y + paddle_h - 1);
        _XL_DRAW(x, new_y, _TILE_1, _XL_WHITE);
    }
    /* Moving down: remove old top tile, add new bottom tile. */
    else {
        _XL_DELETE(x, old_y);
        _XL_DRAW(x, new_y + paddle_h - 1, _TILE_1, _XL_WHITE);
    }
}

/* Restore a single paddle tile if the ball was drawn over it. */
void restore_paddle_tile(uint8_t x, uint8_t y) {
    if (x == 1 && y >= player_y && y <= player_y + paddle_h - 1) {
        _XL_DRAW(x, y, _TILE_1, _XL_WHITE);
    }

    if (x == XSize - 2 && y >= computer_y && y <= computer_y + paddle_h - 1) {
        _XL_DRAW(x, y, _TILE_1, _XL_WHITE);
    }
}

void delete_ball_tile(uint8_t x, uint8_t y) {
    if (x >= XSize || y >= YSize) return;

    /*
     * If the ball is currently drawn on a paddle tile, deleting it would
     * remove part of the paddle. Redraw that single paddle tile afterwards.
     */
    _XL_DELETE(x, y);
    restore_paddle_tile(x, y);
}

/* Place and draw the ball in the center for the next rally. */
void serve_ball(void) {
    ball_x = XSize / 2;
    ball_y = (MIN_PLAY_Y + MAX_PLAY_Y) / 2;

    if (ball_x < MIN_PLAY_X) {
        ball_x = MIN_PLAY_X;
    }

    if (ball_x > MAX_PLAY_X) {
        ball_x = MAX_PLAY_X;
    }

    if (ball_y < MIN_PLAY_Y) {
        ball_y = MIN_PLAY_Y;
    }

    if (ball_y > MAX_PLAY_Y) {
        ball_y = MAX_PLAY_Y;
    }

    ball_dx = (_XL_RAND() % 2) ? 1 : 0;
    ball_dy = (_XL_RAND() % 2) ? 1 : 0;

    /* Remove the old ball position and draw the new serve. */
    delete_ball_tile(prev_ball_x, prev_ball_y);
    _XL_DRAW(ball_x, ball_y, _TILE_3, _XL_WHITE);

    prev_ball_x = ball_x;
    prev_ball_y = ball_y;

    /* Make the ball a little faster after each rally. */
    if (speedup_count < MAX_SPEEDUP_STEPS && ball_step > MIN_BALL_STEP) {
        ball_step--;
        speedup_count++;
    }
}

/*
 * Predict the row where the ball will be when it reaches the right paddle.
 * This is used by the CPU AI. Top and bottom bounces are included.
 */
uint8_t predict_right_paddle_hit_y(void) {
    short px;
    short py;
    uint8_t dy;

    /* If the ball is not moving toward the right paddle, use current row. */
    if (ball_dx != 0) {
        return ball_y;
    }

    px = ball_x;
    py = ball_y;
    dy = ball_dy;

    while (px < XSize - 2) {
        if (dy == 1) {
            py++;
        } else {
            py--;
        }

        /* Top border bounce. */
        if (py <= TOP_BORDER_Y) {
            py = MIN_PLAY_Y;
            dy = 1;
        }
        /* Bottom border bounce. */
        else if (py >= BOTTOM_BORDER_Y) {
            py = MAX_PLAY_Y;
            dy = 0;
        }

        px++;
    }

    if (py < MIN_PLAY_Y) {
        py = MIN_PLAY_Y;
    }

    if (py > MAX_PLAY_Y) {
        py = MAX_PLAY_Y;
    }

    return (uint8_t)py;
}

void draw_border(void) {
    uint8_t x;
    uint8_t y;

    /* Top border. */
    for (x = LEFT_BORDER_X; x <= RIGHT_BORDER_X; x++) {
        _XL_DRAW(x, TOP_BORDER_Y, _TILE_2, _XL_CYAN);
    }

    /* Bottom border. */
    for (x = LEFT_BORDER_X; x <= RIGHT_BORDER_X; x++) {
        _XL_DRAW(x, BOTTOM_BORDER_Y, _TILE_2, _XL_CYAN);
    }

    /* Left border. */
    for (y = TOP_BORDER_Y + 1; y < BOTTOM_BORDER_Y; y++) {
        _XL_DRAW(LEFT_BORDER_X, y, _TILE_2, _XL_CYAN);
    }

    /* Right border. */
    for (y = TOP_BORDER_Y + 1; y < BOTTOM_BORDER_Y; y++) {
        _XL_DRAW(RIGHT_BORDER_X, y, _TILE_2, _XL_CYAN);
    }
}

void update_paddle_height(void) {
    short usable;

    paddle_h = PADDLE_HEIGHT;

    /* Make sure the paddle fits inside the bordered playfield. */
    if (MAX_PLAY_Y >= MIN_PLAY_Y) {
        usable = MAX_PLAY_Y - MIN_PLAY_Y + 1;
        if ((short)paddle_h > usable && usable > 0) {
            paddle_h = (uint8_t)usable;
        }
    }

    if (paddle_h == 0) {
        paddle_h = 1;
    }
}

void init_game(void) {
    short mid_y;
    short max_paddle_y;

    update_paddle_height();

    /* Center the paddles inside the bordered playfield. */
    mid_y = (MIN_PLAY_Y + MAX_PLAY_Y) / 2;
    max_paddle_y = MAX_PLAY_Y - paddle_h + 1;

    if (mid_y < MIN_PLAY_Y) {
        mid_y = MIN_PLAY_Y;
    }

    if (max_paddle_y < MIN_PLAY_Y) {
        max_paddle_y = MIN_PLAY_Y;
    }

    player_y = (uint8_t)(mid_y - 1);
    computer_y = (uint8_t)(mid_y - 1);

    if ((short)player_y < MIN_PLAY_Y) {
        player_y = MIN_PLAY_Y;
    }

    if ((short)computer_y < MIN_PLAY_Y) {
        computer_y = MIN_PLAY_Y;
    }

    if (player_y > max_paddle_y) {
        player_y = (uint8_t)max_paddle_y;
    }

    if (computer_y > max_paddle_y) {
        computer_y = (uint8_t)max_paddle_y;
    }

    prev_player_y = player_y;
    prev_computer_y = computer_y;

    ball_x = XSize / 2;
    ball_y = YSize / 2;

    /* Keep the initial ball inside the playfield. */
    if (ball_x < MIN_PLAY_X) {
        ball_x = MIN_PLAY_X;
    }

    if (ball_x > MAX_PLAY_X) {
        ball_x = MAX_PLAY_X;
    }

    if (ball_y < MIN_PLAY_Y) {
        ball_y = MIN_PLAY_Y;
    }

    if (ball_y > MAX_PLAY_Y) {
        ball_y = MAX_PLAY_Y;
    }

    prev_ball_x = ball_x;
    prev_ball_y = ball_y;

    ball_dx = (_XL_RAND() % 2) ? 1 : 0;
    ball_dy = (_XL_RAND() % 2) ? 1 : 0;

    player_score = 0;
    computer_score = 0;

    last_player_score = 0;
    last_computer_score = 0;

    game_state = STATE_PLAYING;
    ball_timer = 0;

    /* Start at a normal speed and gradually increase during the match. */
    ball_step = BALL_STEP;
    speedup_count = 0;

    /* Draw the static border once per game. */
    draw_border();

    /* Draw initial paddles and ball only once. */
    draw_paddle(1, player_y);
    draw_paddle(XSize - 2, computer_y);

    _XL_DRAW(ball_x, ball_y, _TILE_3, _XL_WHITE);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(2, 0, 2, player_score);
    _XL_PRINTD(XSize - 4, 0, 2, computer_score);
}

void update_game(void) {
    uint8_t inp;
    uint8_t new_player_y;
    uint8_t new_computer_y;
    uint8_t nx;
    uint8_t ny;
    uint8_t hit_paddle;
    uint8_t top_hit;
    uint8_t score_changed;
    uint8_t predicted_y;
    short max_paddle_y;
    short center_top;
    short target_top;

    /* Only the playing state is updated here. */
    if (game_state != STATE_PLAYING) {
        return;
    }

    new_player_y = player_y;
    new_computer_y = computer_y;

    max_paddle_y = MAX_PLAY_Y - paddle_h + 1;
    if (max_paddle_y < MIN_PLAY_Y) {
        max_paddle_y = MIN_PLAY_Y;
    }

    inp = _XL_INPUT();

    if (_XL_UP(inp)) {
        if ((short)new_player_y > MIN_PLAY_Y) {
            new_player_y--;
        }
    }

    if (_XL_DOWN(inp)) {
        if (new_player_y < max_paddle_y) {
            new_player_y++;
        }
    }

    /* --------------------------------------------------------------- */
    /* Smarter CPU AI                                                  */
    /* --------------------------------------------------------------- */

    center_top = (MIN_PLAY_Y + MAX_PLAY_Y - paddle_h + 1) / 2;
    if (center_top < MIN_PLAY_Y) {
        center_top = MIN_PLAY_Y;
    }

    if (center_top > max_paddle_y) {
        center_top = max_paddle_y;
    }

    /*
     * If the ball is moving toward the CPU, predict where it will hit.
     * Otherwise, drift back to the center so the CPU does not overcommit.
     */
    if (ball_dx == 0) {
        predicted_y = predict_right_paddle_hit_y();
        target_top = (short)predicted_y - paddle_h / 2;

        if (target_top < MIN_PLAY_Y) {
            target_top = MIN_PLAY_Y;
        }

        if (target_top > max_paddle_y) {
            target_top = max_paddle_y;
        }
    } else {
        target_top = center_top;
    }

    /* Move only when outside the dead zone. */
    if ((short)new_computer_y < target_top - CPU_DEAD_ZONE &&
        new_computer_y > MIN_PLAY_Y) {
        new_computer_y++;
    } else if ((short)new_computer_y > target_top + CPU_DEAD_ZONE &&
               (short)new_computer_y < max_paddle_y) {
        new_computer_y--;
    }

    /* Keep the computer paddle fully inside the playfield. */
    if ((short)new_computer_y < MIN_PLAY_Y) {
        new_computer_y = MIN_PLAY_Y;
    }

    if (new_computer_y > max_paddle_y) {
        new_computer_y = (uint8_t)max_paddle_y;
    }

    /*
     * Move paddles using only single tile updates:
     * one _XL_DELETE and at most one _XL_DRAW per moving paddle.
     */
    if (new_player_y != prev_player_y) {
        move_paddle(1, prev_player_y, new_player_y);
        prev_player_y = new_player_y;
    }

    if (new_computer_y != prev_computer_y) {
        move_paddle(XSize - 2, prev_computer_y, new_computer_y);
        prev_computer_y = new_computer_y;
    }

    player_y = new_player_y;
    computer_y = new_computer_y;

    /* Ball movement. Smaller ball_step means faster ball. */
    if (ball_timer < ball_step) {
        ball_timer++;
    } else {
        ball_timer = 0;

        nx = ball_x;
        ny = ball_y;

        if (ball_dx == 1) {
            nx++;
        } else {
            nx--;
        }

        if (ball_dy == 1) {
            ny++;
        } else {
            ny--;
        }

        hit_paddle = 0;
        top_hit = 0;

        /* Left side: paddle or score. */
        if (!hit_paddle && game_state == STATE_PLAYING) {
            if (nx <= MIN_PLAY_X) {
                if (ny >= player_y && ny <= player_y + paddle_h - 1) {
                    if (ball_dx == 0) {
                        /* Draw the ball on the paddle itself. */
                        nx = 1;
                        ball_dx = 1;
                        hit_paddle = 1;
                    }
                } else if (nx == LEFT_BORDER_X || nx == MIN_PLAY_X - 1) {
                    computer_score++;
                    game_state = STATE_POINT_PAUSE;
                }
            }
        }

        /* Right side: paddle or score. */
        if (!hit_paddle && game_state == STATE_PLAYING) {
            if (nx >= MAX_PLAY_X) {
                if (ny >= computer_y && ny <= computer_y + paddle_h - 1) {
                    if (ball_dx == 1) {
                        /* Draw the ball on the paddle itself. */
                        nx = XSize - 2;
                        ball_dx = 0;
                        hit_paddle = 1;
                    }
                } else if (nx == RIGHT_BORDER_X || nx == MAX_PLAY_X + 1) {
                    player_score++;
                    game_state = STATE_POINT_PAUSE;
                }
            }
        }

        /* Top and bottom borders. */
        if (!hit_paddle && game_state == STATE_PLAYING) {
            if (ny <= TOP_BORDER_Y && ball_dy == 0) {
                ny = MIN_PLAY_Y;
                ball_dy = 1;
                top_hit = 1;
            } else if (ny >= BOTTOM_BORDER_Y && ball_dy == 1) {
                ny = MAX_PLAY_Y;
                ball_dy = 0;
            }
        }

        /*
         * Commit the new ball position.
         * delete_ball_tile() restores a paddle tile if the old ball
         * position was drawn on the paddle.
         */
        if (!hit_paddle && game_state == STATE_PLAYING) {
            if (nx != prev_ball_x || ny != prev_ball_y) {
                delete_ball_tile(prev_ball_x, prev_ball_y);
                _XL_DRAW(nx, ny, _TILE_3, _XL_WHITE);

                prev_ball_x = nx;
                prev_ball_y = ny;
            }

            ball_x = nx;
            ball_y = ny;
        } else if (hit_paddle) {
            /*
             * The ball bounced on a paddle and should be shown touching it.
             */
            delete_ball_tile(prev_ball_x, prev_ball_y);
            _XL_DRAW(nx, ny, _TILE_3, _XL_WHITE);

            prev_ball_x = nx;
            prev_ball_y = ny;
            ball_x = nx;
            ball_y = ny;
        }
    }

    /* Update score text only when it changes. */
    score_changed = 0;

    if (player_score != last_player_score) {
        _XL_PRINTD(2, 0, 2, player_score);
        last_player_score = player_score;
        score_changed = 1;
    }

    if (computer_score != last_computer_score) {
        _XL_PRINTD(XSize - 4, 0, 2, computer_score);
        last_computer_score = computer_score;
        score_changed = 1;
    }

    /* A win is also treated as a point pause first. */
    if (score_changed && (player_score >= 7 || computer_score >= 7)) {
        game_state = STATE_POINT_PAUSE;
    }
}

int main(void) {
    while (1) {
        _XL_INIT_GRAPHICS();
        _XL_INIT_INPUT();
        _XL_INIT_SOUND();

        init_game();

        while (game_state != STATE_GAME_OVER) {
            update_game();

            /* After each point: pause, then wait for input. */
            if (game_state == STATE_POINT_PAUSE) {
                _XL_SLEEP(POINT_PAUSE_SECONDS);
                _XL_WAIT_FOR_INPUT();

                if (player_score >= 7 || computer_score >= 7) {
                    game_state = STATE_GAME_OVER;
                } else {
                    serve_ball();
                    ball_timer = 0;
                    game_state = STATE_PLAYING;
                }
            }

            /* Base game loop speed. */
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* Pause before showing the winner. */
        _XL_SLEEP(1);

        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);

        if (player_score >= 7) {
            _XL_PRINT(10, YSize / 2, "PLAYER WINS");
        } else {
            _XL_PRINT(10, YSize / 2, "CPU WINS");
        }

        /* Pause after the winner text. */
        _XL_SLEEP(1);

        /* Wait for any key to restart. */
        _XL_WAIT_FOR_INPUT();
    }
}
