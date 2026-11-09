#include "cross_lib.h"

/*
  Screen coordinates are inclusive:
    0 <= x <= XSize
    0 <= y <= YSize

  So total columns = XSize + 1
  total rows     = YSize + 1
*/
#define SCREEN_W (XSize + 1)
#define SCREEN_H (YSize + 1)

/*
  Packed per-cell state:
    high byte  = tile_id
    low byte   = color_id

  This lets us avoid calling _XL_DRAW or _XL_DELETE when nothing changed.
*/
#define EMPTY_STATE ((uint16_t)_TILE_0 << 8) | (uint16_t)_XL_WHITE

/* Direction encoding:
     V_LEFT  = -1
     V_ZERO  =  0
     V_RIGHT = +1

   For x: left/right.
   For y: V_LEFT means up, V_RIGHT means down.
*/
#define V_LEFT 0
#define V_ZERO 1
#define V_RIGHT 2

/* Game states */
#define STATE_TITLE 0
#define STATE_PLAYING 1
#define STATE_GAME_OVER 2
#define STATE_WIN 3

/* Ball movement result codes */
#define MOVE_MOVED 0
#define MOVE_BLOCK 1
#define MOVE_PADDLE 2
#define MOVE_LOST 3
#define MOVE_WIN 4

/* Ball moves every second frame to make the game a little slower. */
#define BALL_DELAY 2

#define SCORE_MAX 65535

/* Per-cell last drawn state: tile + color */
static uint16_t last_cell[SCREEN_W][SCREEN_H];

/* Block map: 1 = block present, 0 = empty */
static uint8_t block_grid[SCREEN_W][SCREEN_H];

/* Game variables */
static uint8_t state;
static uint8_t serving;

static uint8_t ball_x, ball_y, ball_vx, ball_vy;
static uint8_t paddle_x, paddle_w;

static uint16_t score;
static uint8_t lives;
static uint16_t blocks_left;

/* HUD cache so we do not reprint unchanged values */
static uint16_t last_score;
static uint8_t last_lives;

/* Ball timer for slower ball movement */
static uint8_t ball_timer;

/* Prototypes */
static void reset_screen_state(void);
static void set_cell(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id);

static uint8_t text_len(char *s);
static uint8_t center_x(uint8_t len);
static void print_center(char *s, uint8_t y);

static uint8_t top_row(void);
static uint8_t paddle_row(void);

static uint8_t block_color(uint8_t r);
static void init_blocks(void);

static uint8_t score_x(void);
static uint8_t lives_label_x(void);
static void draw_hud_full(void);
static void update_hud(void);

static uint8_t is_paddle(uint8_t x, uint8_t y);
static uint8_t is_block(uint8_t x, uint8_t y);
static void break_block(uint8_t x, uint8_t y);

static void refresh_paddle_cells(uint8_t old_x);
static void set_ball_on_paddle(void);
static void new_game(void);

static void show_title_screen(void);
static void show_end_screen(uint8_t won);

static uint8_t move_ball(void);
static void normalize_velocity(void);
static void paddle_bounce(uint8_t hit_x);

static uint8_t next_x(uint8_t x, uint8_t dir);
static uint8_t next_y(uint8_t y, uint8_t dir);
static uint8_t opp_dir(uint8_t d);
static uint8_t get_paddle_width(void);

static void move_ball_screen(uint8_t old_x, uint8_t old_y);


/* ------------------------------------------------------------------ */
/* Screen state helpers                                                */
/* ------------------------------------------------------------------ */

static void reset_screen_state(void)
{
    uint16_t x;
    uint16_t y;

    for (y = 0; y <= (uint16_t)YSize; ++y) {
        for (x = 0; x <= (uint16_t)XSize; ++x) {
            last_cell[x][y] = EMPTY_STATE;
        }
    }
}


/*
   Draw or delete only if the requested tile/color state is different
   from what we previously recorded for that cell.
*/
static void set_cell(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id)
{
    uint16_t new_state;
    uint8_t old_tile;

    if ((uint16_t)x > (uint16_t)XSize || (uint16_t)y > (uint16_t)YSize) {
        return;
    }

    new_state = ((uint16_t)tile_id << 8) | (uint16_t)color_id;

    if (last_cell[x][y] == new_state) {
        return;
    }

    old_tile = (uint8_t)(last_cell[x][y] >> 8);

    if (tile_id == _TILE_0 && old_tile != _TILE_0) {
        _XL_DELETE(x, y);
    } else if (tile_id != _TILE_0) {
        _XL_DRAW(x, y, tile_id, color_id);
    }

    last_cell[x][y] = new_state;
}


/* ------------------------------------------------------------------ */
/* Text helpers                                                       */
/* ------------------------------------------------------------------ */

static uint8_t text_len(char *s)
{
    uint8_t n = 0;

    while (s[n] != '\0') {
        ++n;
    }

    return n;
}


static uint8_t center_x(uint8_t len)
{
    uint16_t total = (uint16_t)(XSize + 1);

    if ((uint16_t)len > total) {
        return 0;
    }

    return (uint8_t)((total - (uint16_t)len) / 2);
}


/*
   Prints a centered string. If the screen is too narrow, it truncates
   the string so we do not write outside the allowed coordinate range.
*/
static void print_center(char *s, uint8_t y)
{
    char buf[32];
    uint16_t total = (uint16_t)(XSize + 1);
    uint8_t len;
    uint8_t x;
    uint8_t i;

    if ((uint16_t)y > (uint16_t)YSize) {
        y = (uint8_t)YSize;
    }

    len = text_len(s);

    if ((uint16_t)len > total) {
        for (i = 0; i < total; ++i) {
            buf[i] = s[i];
        }
        buf[total] = '\0';
        len = (uint8_t)total;
        x = 0;
    } else {
        x = center_x(len);
    }

    _XL_PRINT(x, y, buf);
}


/* ------------------------------------------------------------------ */
/* Playfield geometry                                                 */
/* ------------------------------------------------------------------ */

static uint8_t top_row(void)
{
    if ((uint16_t)YSize >= 5) {
        return 2;
    }
    if ((uint16_t)YSize >= 3) {
        return 1;
    }
    return 0;
}


static uint8_t paddle_row(void)
{
    return (uint8_t)YSize;
}


/* ------------------------------------------------------------------ */
/* Blocks                                                             */
/* ------------------------------------------------------------------ */

static uint8_t block_color(uint8_t r)
{
    if (r == 0) {
        return _XL_RED;
    }
    if (r == 1) {
        return _XL_CYAN;
    }
    if (r == 2) {
        return _XL_GREEN;
    }
    if (r == 3) {
        return _XL_MAGENTA;
    }
    if (r == 4) {
        return _XL_YELLOW;
    }

    return _XL_BLUE;
}


static void init_blocks(void)
{
    uint16_t x;
    uint16_t y;
    uint8_t r;
    uint8_t rows;
    uint16_t available;
    uint8_t margin;
    uint8_t color;

    blocks_left = 0;

    /* Clear logical block map. The screen itself was already cleared. */
    for (y = 0; y <= (uint16_t)YSize; ++y) {
        for (x = 0; x <= (uint16_t)XSize; ++x) {
            block_grid[x][y] = 0;
        }
    }

    if ((uint16_t)paddle_row() >= (uint16_t)top_row()) {
        available = (uint16_t)((uint16_t)paddle_row() - (uint16_t)top_row());
    } else {
        available = 0;
    }

    rows = 5;

    if (available < 7) {
        if (available >= 3) {
            rows = (uint8_t)(available - 2);
        } else {
            rows = 0;
        }
    }

    if (rows > 5) {
        rows = 5;
    }

    margin = ((uint16_t)XSize >= 2) ? 1 : 0;

    for (r = 0; r < rows; ++r) {
        y = (uint16_t)(top_row() + 1 + r);

        if (y >= (uint16_t)paddle_row()) {
            break;
        }

        color = block_color(r);

        for (x = margin; x <= (uint16_t)XSize - margin; ++x) {
            block_grid[x][y] = 1;
            ++blocks_left;
            set_cell((uint8_t)x, (uint8_t)y, _TILE_1, color);
        }
    }
}


/* ------------------------------------------------------------------ */
/* HUD                                                                */
/* ------------------------------------------------------------------ */

static uint8_t score_x(void)
{
    if ((uint16_t)(XSize + 1) >= 12) {
        return 7;
    }
    return 1;
}


static uint8_t lives_label_x(void)
{
    uint16_t total = (uint16_t)(XSize + 1);

    if (total < 24) {
        return 0;
    }

    /* "LIVES" is 5 chars, then one digit at lx+6 */
    return (uint8_t)(total - 7);
}


static void draw_hud_full(void)
{
    uint8_t lx;

    _XL_SET_TEXT_COLOR(_XL_WHITE);

    if ((uint16_t)(XSize + 1) >= 12) {
        _XL_PRINT(1, 0, "SCORE");
    }

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINTD(score_x(), 0, 1, score);

    if ((uint16_t)(XSize + 1) >= 24) {
        lx = lives_label_x();

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(lx, 0, "LIVES");

        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD((uint8_t)(lx + 6), 0, 1, lives);
    }
}


static void update_hud(void)
{
    uint8_t lx;

    if (score != last_score) {
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD(score_x(), 0, 1, score);
        last_score = score;
    }

    if (lives != last_lives) {
        if ((uint16_t)(XSize + 1) >= 24) {
            lx = lives_label_x();
            _XL_SET_TEXT_COLOR(_XL_YELLOW);
            _XL_PRINTD((uint8_t)(lx + 6), 0, 1, lives);
        }

        last_lives = lives;
    }
}


/* ------------------------------------------------------------------ */
/* Paddle and ball helpers                                            */
/* ------------------------------------------------------------------ */

static uint8_t is_paddle(uint8_t x, uint8_t y)
{
    if ((uint16_t)y != (uint16_t)paddle_row()) {
        return 0;
    }

    if ((uint16_t)x >= (uint16_t)paddle_x &&
        (uint16_t)x < (uint16_t)paddle_x + (uint16_t)paddle_w) {
        return 1;
    }

    return 0;
}


static uint8_t is_block(uint8_t x, uint8_t y)
{
    if ((uint16_t)x > (uint16_t)XSize || (uint16_t)y > (uint16_t)YSize) {
        return 0;
    }

    return block_grid[x][y];
}


static void break_block(uint8_t x, uint8_t y)
{
    uint16_t ns;

    if ((uint16_t)x > (uint16_t)XSize || (uint16_t)y > (uint16_t)YSize) {
        return;
    }

    if (!block_grid[x][y]) {
        return;
    }

    block_grid[x][y] = 0;

    if (blocks_left > 0) {
        --blocks_left;
    }

    if (score < SCORE_MAX) {
        ns = (uint16_t)(score + 10);
        if (ns > SCORE_MAX) {
            ns = SCORE_MAX;
        }
        score = ns;
    }

    set_cell(x, y, _TILE_0, _XL_WHITE);
}


/*
   Refresh only the paddle cells that may have changed.
   set_cell() itself suppresses unchanged draw/delete calls.
*/
static void refresh_paddle_cells(uint8_t old_x)
{
    uint16_t x;
    uint16_t min_x = (uint16_t)old_x;
    uint16_t max_x = (uint16_t)paddle_x + (uint16_t)paddle_w - 1;
    uint16_t old_end = (uint16_t)old_x + (uint16_t)paddle_w - 1;

    if (old_end > max_x) {
        max_x = old_end;
    }

    if ((uint16_t)paddle_x < min_x) {
        min_x = (uint16_t)paddle_x;
    }

    if (max_x > (uint16_t)XSize) {
        max_x = (uint16_t)XSize;
    }

    for (x = min_x; x <= max_x; ++x) {
        if (is_paddle((uint8_t)x, paddle_row())) {
            set_cell((uint8_t)x, paddle_row(), _TILE_2, _XL_CYAN);
        } else {
            set_cell((uint8_t)x, paddle_row(), _TILE_0, _XL_WHITE);
        }
    }
}


static void set_ball_on_paddle(void)
{
    ball_x = (uint8_t)(paddle_x + paddle_w / 2);

    if ((uint16_t)paddle_row() > 0) {
        ball_y = (uint8_t)((uint16_t)paddle_row() - 1);
    } else {
        ball_y = 0;
    }
}


/*
   If the ball moved, delete its old cell and draw its new cell.
   If it did not move, do nothing.
*/
static void move_ball_screen(uint8_t old_x, uint8_t old_y)
{
    if (old_x == ball_x && old_y == ball_y) {
        return;
    }

    set_cell(old_x, old_y, _TILE_0, _XL_WHITE);
    set_cell(ball_x, ball_y, _TILE_3, _XL_YELLOW);
}


/* ------------------------------------------------------------------ */
/* Game setup and screens                                             */
/* ------------------------------------------------------------------ */

static void new_game(void)
{
    _XL_CLEAR_SCREEN();
    reset_screen_state();

    serving = 1;
    score = 0;
    lives = 3;
    last_score = 0;
    last_lives = 0;
    blocks_left = 0;
    ball_timer = 0;

    paddle_w = get_paddle_width();
    paddle_x = (uint8_t)(((uint16_t)(XSize + 1) - (uint16_t)paddle_w) / 2);

    init_blocks();
    draw_hud_full();
    refresh_paddle_cells(paddle_x);

    set_ball_on_paddle();
    ball_vx = V_ZERO;
    ball_vy = V_LEFT; /* up */

    set_cell(ball_x, ball_y, _TILE_3, _XL_YELLOW);

    if (blocks_left == 0) {
        state = STATE_WIN;
    } else {
        state = STATE_PLAYING;
    }
}


static void show_title_screen(void)
{
    uint8_t input;
    uint8_t y1;
    uint8_t y2;

    state = STATE_TITLE;

    _XL_CLEAR_SCREEN();
    reset_screen_state();

    last_score = 0;
    last_lives = 0;

    y1 = (uint8_t)(YSize / 2);
    y2 = (uint8_t)((YSize / 2) + 3);

    if ((uint16_t)y2 > (uint16_t)YSize) {
        y2 = (uint8_t)YSize;
    }

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    print_center("ARKANOID", y1);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    print_center("PRESS FIRE TO START", y2);

    while (state == STATE_TITLE) {
        input = _XL_INPUT();

        if (_XL_FIRE(input)) {
            break;
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}


static void show_end_screen(uint8_t won)
{
    uint8_t input;
    uint8_t y1;
    uint8_t y2;

    if (won) {
        state = STATE_WIN;
    } else {
        state = STATE_GAME_OVER;
    }

    _XL_CLEAR_SCREEN();
    reset_screen_state();

    y1 = (uint8_t)(YSize / 2);
    y2 = (uint8_t)((YSize / 2) + 3);

    if ((uint16_t)y2 > (uint16_t)YSize) {
        y2 = (uint8_t)YSize;
    }

    if (won) {
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        print_center("YOU WIN", y1);
        _XL_EXPLOSION_SOUND();
    } else {
        _XL_SET_TEXT_COLOR(_XL_RED);
        print_center("GAME OVER", y1);
        _XL_ZAP_SOUND();
    }

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    print_center("PRESS FIRE TO RESTART", y2);

    while (state != STATE_TITLE) {
        input = _XL_INPUT();

        if (_XL_FIRE(input)) {
            break;
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}


/* ------------------------------------------------------------------ */
/* Ball movement and collision                                        */
/* ------------------------------------------------------------------ */

static uint8_t next_x(uint8_t x, uint8_t dir)
{
    if (dir == V_LEFT) {
        if ((uint16_t)x > 0) {
            return (uint8_t)((uint16_t)x - 1);
        }
        return 0;
    }

    if (dir == V_RIGHT) {
        if ((uint16_t)x < (uint16_t)XSize) {
            return (uint8_t)((uint16_t)x + 1);
        }
        return (uint8_t)XSize;
    }

    return x;
}


static uint8_t next_y(uint8_t y, uint8_t dir)
{
    if (dir == V_LEFT) { /* up */
        if ((uint16_t)y > (uint16_t)top_row()) {
            return (uint8_t)((uint16_t)y - 1);
        }
        return top_row();
    }

    if (dir == V_RIGHT) { /* down */
        if ((uint16_t)y < (uint16_t)paddle_row()) {
            return (uint8_t)((uint16_t)y + 1);
        }
        return paddle_row();
    }

    return y;
}


static uint8_t opp_dir(uint8_t d)
{
    if (d == V_LEFT) {
        return V_RIGHT;
    }

    if (d == V_RIGHT) {
        return V_LEFT;
    }

    return V_ZERO;
}


/*
   Avoid degenerate zero velocity components after bounces.
*/
static void normalize_velocity(void)
{
    uint8_t r = (uint8_t)(_XL_RAND() & 1);

    if (ball_vx == V_ZERO) {
        if ((uint16_t)ball_x == 0) {
            ball_vx = V_RIGHT;
        } else if ((uint16_t)ball_x >= (uint16_t)XSize) {
            ball_vx = V_LEFT;
        } else {
            ball_vx = r ? V_RIGHT : V_LEFT;
        }
    }

    if (ball_vy == V_ZERO) {
        if ((uint16_t)ball_y == (uint16_t)top_row()) {
            ball_vy = V_RIGHT; /* down */
        } else if ((uint16_t)ball_y >= (uint16_t)paddle_row()) {
            ball_vy = V_LEFT;  /* up */
        } else {
            ball_vy = r ? V_RIGHT : V_LEFT;
        }
    }
}


/*
   Paddle bounce: reflect vertically and choose a horizontal direction
   based on where the paddle was hit.
*/
static void paddle_bounce(uint8_t hit_x)
{
    uint8_t offset = 0;
    uint8_t third;
    uint8_t r;

    if ((uint16_t)hit_x >= (uint16_t)paddle_x &&
        (uint16_t)hit_x < (uint16_t)paddle_x + (uint16_t)paddle_w) {
        offset = (uint8_t)((uint16_t)hit_x - (uint16_t)paddle_x);
    }

    if ((uint16_t)paddle_w <= 1) {
        r = (uint8_t)(_XL_RAND() & 1);
        ball_vx = r ? V_RIGHT : V_LEFT;
    } else {
        third = (uint8_t)((uint16_t)paddle_w / 3);

        if ((uint16_t)offset < (uint16_t)third) {
            ball_vx = V_LEFT;
        } else if ((uint16_t)offset > (uint16_t)(paddle_w - 1 - third)) {
            ball_vx = V_RIGHT;
        } else {
            r = (uint8_t)(_XL_RAND() & 1);
            ball_vx = r ? V_RIGHT : V_LEFT;
        }
    }

    ball_vy = V_LEFT; /* up */
}


/*
   Move the ball one tile in its current direction.
   Handles walls, paddle, and blocks.
*/
static uint8_t move_ball(void)
{
    uint8_t nx;
    uint8_t ny;
    uint8_t hit_x_block = 0;
    uint8_t hit_y_block = 0;
    uint8_t broke = 0;

    /* If the ball is already in the paddle row, push it out or lose. */
    if ((uint16_t)ball_y == (uint16_t)paddle_row()) {
        if (is_paddle(ball_x, paddle_row())) {
            ball_y = (uint8_t)((uint16_t)paddle_row() - 1);
            ball_vy = V_LEFT; /* up */
        } else {
            return MOVE_LOST;
        }
    }

    /* Reflect top wall. */
    if (ball_vy == V_LEFT && (uint16_t)ball_y == (uint16_t)top_row()) {
        ball_vy = V_RIGHT; /* down */
    }

    /* Reflect side walls. */
    if (ball_vx == V_LEFT && (uint16_t)ball_x == 0) {
        ball_vx = V_RIGHT;
    }

    if (ball_vx == V_RIGHT && (uint16_t)ball_x >= (uint16_t)XSize) {
        ball_vx = V_LEFT;
    }

    normalize_velocity();

    nx = next_x(ball_x, ball_vx);
    ny = next_y(ball_y, ball_vy);

    /* Paddle collision from above. */
    if ((uint16_t)ny == (uint16_t)paddle_row() && ball_vy == V_RIGHT) {
        if (is_paddle(nx, paddle_row()) || is_paddle(ball_x, paddle_row())) {
            ball_y = (uint8_t)((uint16_t)paddle_row() - 1);
            paddle_bounce(nx);
            _XL_TICK_SOUND();
            return MOVE_PADDLE;
        } else {
            return MOVE_LOST;
        }
    }

    /* Block collision: check horizontal, vertical, and target cells. */
    hit_x_block = (ball_vx != V_ZERO && is_block(nx, ball_y));
    hit_y_block = (ball_vy != V_ZERO && is_block(ball_x, ny));

    if (hit_x_block || hit_y_block) {
        if (hit_x_block) {
            break_block(nx, ball_y);
            broke = 1;
            ball_vx = opp_dir(ball_vx);
        }

        if (hit_y_block) {
            break_block(ball_x, ny);
            broke = 1;
            ball_vy = opp_dir(ball_vy);
        }
    } else if (is_block(nx, ny)) {
        /* Diagonal corner collision. */
        break_block(nx, ny);
        broke = 1;
        ball_vx = opp_dir(ball_vx);
        ball_vy = opp_dir(ball_vy);
    }

    if (broke) {
        normalize_velocity();
        _XL_PING_SOUND();

        if (blocks_left == 0) {
            return MOVE_WIN;
        }

        return MOVE_BLOCK;
    }

    ball_x = nx;
    ball_y = ny;

    return MOVE_MOVED;
}


/* ------------------------------------------------------------------ */
/* Paddle width                                                       */
/* ------------------------------------------------------------------ */

static uint8_t get_paddle_width(void)
{
    uint8_t w;
    uint16_t maxw = (uint16_t)(XSize + 1);

    if ((uint16_t)XSize >= 7) {
        w = 5;
    } else if ((uint16_t)XSize >= 2) {
        w = 3;
    } else {
        w = 1;
    }

    if (w > maxw) {
        w = (uint8_t)maxw;
    }

    return w;
}


/* ------------------------------------------------------------------ */
/* Main game loop                                                     */
/* ------------------------------------------------------------------ */

uint8_t main(void)
{
    uint8_t input;
    uint8_t result;
    uint8_t old_x;
    uint8_t old_y;
    uint16_t max_paddle_x;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    show_title_screen();

    while (1) {
        new_game();

        while (state == STATE_PLAYING) {
            input = _XL_INPUT();
            result = MOVE_MOVED;

            /* Move paddle. */
            old_x = paddle_x;

            if (_XL_LEFT(input)) {
                if ((uint16_t)paddle_x > 0) {
                    --paddle_x;
                }
            }

            if (_XL_RIGHT(input)) {
                max_paddle_x = (uint16_t)(XSize + 1 - paddle_w);

                if ((uint16_t)paddle_x < max_paddle_x) {
                    ++paddle_x;
                }
            }

            refresh_paddle_cells(old_x);

            /* Remember old ball position for screen update. */
            old_x = ball_x;
            old_y = ball_y;

            if (serving) {
                set_ball_on_paddle();
                move_ball_screen(old_x, old_y);

                if (_XL_FIRE(input)) {
                    serving = 0;
                    ball_vx = (uint8_t)(_XL_RAND() & 1) ? V_RIGHT : V_LEFT;
                    ball_vy = V_LEFT; /* launch upward */
                    _XL_SHOOT_SOUND();
                } else {
                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    continue;
                }
            }

            if (ball_timer > 0) {
                --ball_timer;
            }

            if (ball_timer == 0) {
                result = move_ball();
                ball_timer = BALL_DELAY;
            }

            /* Update ball graphics only when its cell changed. */
            if (result == MOVE_MOVED || result == MOVE_PADDLE) {
                move_ball_screen(old_x, old_y);
            } else if (result == MOVE_BLOCK) {
                set_cell(ball_x, ball_y, _TILE_3, _XL_YELLOW);
            } else if (result == MOVE_LOST) {
                set_cell(old_x, old_y, _TILE_0, _XL_WHITE);

                --lives;
                update_hud();

                if (lives == 0) {
                    state = STATE_GAME_OVER;
                } else {
                    serving = 1;
                    set_ball_on_paddle();
                    ball_vx = V_ZERO;
                    ball_vy = V_LEFT; /* up */
                    move_ball_screen(old_x, old_y);
                }
            } else if (result == MOVE_WIN) {
                state = STATE_WIN;
            }

            update_hud();

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        show_end_screen(state == STATE_WIN ? 1 : 0);
    }

    return 0;
}
