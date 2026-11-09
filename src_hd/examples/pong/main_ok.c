#include "cross_lib.h"

/* Ball moves once every BALL_STEP frames. Smaller = faster. */
#define BALL_STEP 8

/* Paddle size in tiles. Change this value to make paddles bigger or smaller. */
#define PADDLE_HEIGHT (YSize/5)

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

uint8_t game_state; /* 0 = playing, 1/2 = pause after score, 3 = game over */
uint8_t ball_timer;

void clear_paddle(uint8_t x, uint8_t y) {
    uint8_t i;

    if (x >= XSize || y >= YSize) return;

    for (i = 0; i < paddle_h; i++) {
        if (y + i < YSize) {
            _XL_DELETE(x, y + i);
        }
    }
}

void draw_paddle(uint8_t x, uint8_t y) {
    uint8_t i;

    if (x >= XSize || y >= YSize) return;

    for (i = 0; i < paddle_h; i++) {
        if (y + i < YSize) {
            _XL_DRAW(x, y + i, _TILE_1, _XL_WHITE);
        }
    }
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

    game_state = 0;
    ball_timer = 0;

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
    short max_paddle_y;

    if (game_state != 0) {
        game_state--;

        if (game_state == 0) {
            /* Serve from the center of the bordered playfield. */
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

            /* Delete the old ball position and draw the new serve. */
            _XL_DELETE(prev_ball_x, prev_ball_y);
            _XL_DRAW(ball_x, ball_y, _TILE_3, _XL_WHITE);

            prev_ball_x = ball_x;
            prev_ball_y = ball_y;
            _XL_SLEEP(1);
            _XL_WAIT_FOR_INPUT();
        }
    } else {
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

        /* Simple computer AI. */
        if (computer_y + 1 < ball_y - 1) {
            new_computer_y++;
        } else if (computer_y + 1 > ball_y + 1) {
            new_computer_y--;
        }

        /* Keep the computer paddle fully inside the playfield. */
        if ((short)new_computer_y < MIN_PLAY_Y) {
            new_computer_y = MIN_PLAY_Y;
        }

        if (new_computer_y > max_paddle_y) {
            new_computer_y = (uint8_t)max_paddle_y;
        }

        /* Move player paddle only if it changed. */
        if (new_player_y != prev_player_y) {
            clear_paddle(1, prev_player_y);
            draw_paddle(1, new_player_y);
            prev_player_y = new_player_y;
        }

        /* Move computer paddle only if it changed. */
        if (new_computer_y != prev_computer_y) {
            clear_paddle(XSize - 2, prev_computer_y);
            draw_paddle(XSize - 2, new_computer_y);
            prev_computer_y = new_computer_y;
        }

        player_y = new_player_y;
        computer_y = new_computer_y;

        /* Ball movement. Smaller BALL_STEP means faster ball. */
        // if (ball_timer < BALL_STEP) {
            // ball_timer++;
        // } else {
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
            if (!hit_paddle && game_state == 0) {
                if (nx <= MIN_PLAY_X) {
                    if (ny >= player_y && ny <= player_y + paddle_h - 1) {
                        if (ball_dx == 0) {
                            nx = ball_x;
                            ball_dx = 1;
                            hit_paddle = 1;
                        }
                    } else if (nx == LEFT_BORDER_X || nx == MIN_PLAY_X - 1) {
                        computer_score++;
                        game_state = 1;
                    }
                }
            }

            /* Right side: paddle or score. */
            if (!hit_paddle && game_state == 0) {
                if (nx >= MAX_PLAY_X) {
                    if (ny >= computer_y && ny <= computer_y + paddle_h - 1) {
                        if (ball_dx == 1) {
                            nx = ball_x;
                            ball_dx = 0;
                            hit_paddle = 1;
                        }
                    } else if (nx == RIGHT_BORDER_X || nx == MAX_PLAY_X + 1) {
                        player_score++;
                        game_state = 2;
                    }
                }
            }

            /* Top and bottom borders. */
            if (!hit_paddle && game_state == 0) {
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
             *
             * top_hit is no longer special for drawing: even when bouncing
             * on the top border, the old tile must be deleted so that no
             * trace is left behind.
             */
            if (!hit_paddle && game_state == 0) {
                if (nx != prev_ball_x || ny != prev_ball_y) {
                    _XL_DELETE(prev_ball_x, prev_ball_y);
                    _XL_DRAW(nx, ny, _TILE_3, _XL_WHITE);

                    prev_ball_x = nx;
                    prev_ball_y = ny;
                }

                ball_x = nx;
                ball_y = ny;
            // }
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

        if ((player_score >= 7 || computer_score >= 7) && score_changed) {
            game_state = 3;
        }
    }
}

int main(void) {
    while (1) {
        _XL_INIT_GRAPHICS();
        _XL_INIT_INPUT();
        _XL_INIT_SOUND();

        init_game();

        while (game_state != 3) {
            update_game();

            /* Base game loop speed. */
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);

        if (player_score >= 7) {
            _XL_PRINT(10, YSize / 2, "PLAYER WINS");
        } else {
            _XL_PRINT(10, YSize / 2, "CPU WINS");
        }

        _XL_SLEEP(3);
    }
}
