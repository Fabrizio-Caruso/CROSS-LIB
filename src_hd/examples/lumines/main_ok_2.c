#include "cross_lib.h"

#define GRID_W 16
#define GRID_H 10
#define GY 2
#define EMPTY 0
#define CYAN 1
#define YELLOW 2
#define LINE_MARK 3
#define FALL_EVERY 8
#define LINE_EVERY 4

static uint8_t grid[GRID_H][GRID_W];
static uint8_t disp[GRID_H][GRID_W];
static uint8_t piece_x, piece_y;
static uint8_t piece_tiles[4];
static uint8_t piece_active;
static uint8_t line_x;
static uint16_t score;
static uint8_t game_over;
static uint8_t frame_count;

static uint8_t can_place(uint8_t px, uint8_t py)
{
    uint8_t i, j;
    if (px + 1 >= GRID_W) return 0;
    if (py + 1 >= GRID_H) return 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (grid[py + i][px + j] != EMPTY)
                return 0;
        }
    }
    return 1;
}

static void init_game(void)
{
    uint8_t i, j;
    for (i = 0; i < GRID_H; i++) {
        for (j = 0; j < GRID_W; j++) {
            grid[i][j] = EMPTY;
            disp[i][j] = EMPTY;
        }
    }
    line_x = 0;
    score = 0;
    game_over = 0;
    frame_count = 0;
    piece_active = 0;
}

static void spawn_piece(void)
{
    uint16_t r;
    uint8_t i;
    r = _XL_RAND();
    piece_x = (uint8_t)(r % (GRID_W - 1));
    piece_y = 0;
    for (i = 0; i < 4; i++) {
        piece_tiles[i] = (uint8_t)((_XL_RAND() & 1) + 1);
    }
    if (piece_tiles[0] == piece_tiles[1] &&
        piece_tiles[1] == piece_tiles[2] &&
        piece_tiles[2] == piece_tiles[3]) {
        if (piece_tiles[0] == CYAN)
            piece_tiles[3] = YELLOW;
        else
            piece_tiles[3] = CYAN;
    }
    piece_active = 1;
    if (!can_place(piece_x, piece_y)) {
        game_over = 1;
        piece_active = 0;
    }
}

static void rotate_piece(void)
{
    uint8_t t;
    if (!piece_active) return;
    t = piece_tiles[0];
    piece_tiles[0] = piece_tiles[2];
    piece_tiles[2] = piece_tiles[3];
    piece_tiles[3] = piece_tiles[1];
    piece_tiles[1] = t;
    _XL_PING_SOUND();
}

static void lock_piece(void)
{
    uint8_t i, j;
    uint8_t idx;
    idx = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            grid[piece_y + i][piece_x + j] = piece_tiles[idx];
            idx++;
        }
    }
    piece_active = 0;
}

static void apply_gravity(void)
{
    uint8_t x, y;
    uint8_t changed;
    do {
        changed = 0;
        for (x = 0; x < GRID_W; x++) {
            for (y = GRID_H - 2; y < GRID_H; y--) {
                if (grid[y + 1][x] == EMPTY && grid[y][x] != EMPTY) {
                    grid[y + 1][x] = grid[y][x];
                    grid[y][x] = EMPTY;
                    changed = 1;
                }
            }
        }
    } while (changed);
}

static void check_and_clear(void)
{
    uint8_t y, cx;
    for (y = 0; y < GRID_H - 1; y++) {
        if (line_x > 0) {
            cx = line_x - 1;
            if (grid[y][cx] != EMPTY &&
                grid[y][cx] == grid[y][cx + 1] &&
                grid[y][cx] == grid[y + 1][cx] &&
                grid[y][cx] == grid[y + 1][cx + 1]) {
                grid[y][cx] = EMPTY;
                grid[y][cx + 1] = EMPTY;
                grid[y + 1][cx] = EMPTY;
                grid[y + 1][cx + 1] = EMPTY;
                score += 100;
                _XL_TOCK_SOUND();
            }
        }
        if (line_x < GRID_W - 1) {
            cx = line_x;
            if (grid[y][cx] != EMPTY &&
                grid[y][cx] == grid[y][cx + 1] &&
                grid[y][cx] == grid[y + 1][cx] &&
                grid[y][cx] == grid[y + 1][cx + 1]) {
                grid[y][cx] = EMPTY;
                grid[y][cx + 1] = EMPTY;
                grid[y + 1][cx] = EMPTY;
                grid[y + 1][cx + 1] = EMPTY;
                score += 100;
                _XL_TOCK_SOUND();
            }
        }
    }
}

static void move_piece_down(void)
{
    if (!piece_active) return;
    if (piece_y + 2 < GRID_H) {
        if (grid[piece_y + 2][piece_x] == EMPTY &&
            grid[piece_y + 2][piece_x + 1] == EMPTY) {
            piece_y++;
        } else {
            lock_piece();
            apply_gravity();
            spawn_piece();
        }
    } else {
        lock_piece();
        apply_gravity();
        spawn_piece();
    }
}

static void move_time_line(void)
{
    if (line_x < GRID_W - 1) {
        line_x++;
        check_and_clear();
        apply_gravity();
    } else {
        line_x = 0;
    }
}

static void render(void)
{
    uint8_t x, y;
    uint8_t val;

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "SCORE");
    _XL_PRINTD(6, 0, 6, score);
    _XL_PRINT(0, 1, "FIRE ROTATE");

    for (y = 0; y < GRID_H; y++) {
        for (x = 0; x < GRID_W; x++) {
            val = EMPTY;
            if (piece_active &&
                x >= piece_x && x < piece_x + 2 &&
                y >= piece_y && y < piece_y + 2) {
                uint8_t li;
                uint8_t lj;
                uint8_t idx;
                li = x - piece_x;
                lj = y - piece_y;
                idx = lj * 2 + li;
                val = piece_tiles[idx];
            } else if (grid[y][x] != EMPTY) {
                val = grid[y][x];
            } else if (x == line_x) {
                val = LINE_MARK;
            }
            if (val != disp[y][x]) {
                if (val == EMPTY) {
                    _XL_DELETE(x, y + GY);
                } else if (val == CYAN) {
                    _XL_DRAW(x, y + GY, _TILE_0, _XL_CYAN);
                } else if (val == YELLOW) {
                    _XL_DRAW(x, y + GY, _TILE_0, _XL_YELLOW);
                } else {
                    _XL_DRAW(x, y + GY, _TILE_1, _XL_WHITE);
                }
                disp[y][x] = val;
            }
        }
    }

    if (game_over) {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(2, 5, "GAME OVER");
        _XL_PRINT(2, 7, "PRESS FIRE");
    }
}

int main(void)
{
    uint8_t input;
    uint8_t restart;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    restart = 1;

    while (1) {
        if (restart) {
            uint8_t i, j;
            init_game();
            _XL_CLEAR_SCREEN();
            for (i = 0; i < GRID_H; i++) {
                for (j = 0; j < GRID_W; j++) {
                    disp[i][j] = EMPTY;
                }
            }
            spawn_piece();
            restart = 0;
        }

        input = _XL_INPUT();
        if (_XL_FIRE(input)) {
            if (game_over) {
                _XL_SLEEP(1);
                restart = 1;
                continue;
            }
            rotate_piece();
        }

        if (!game_over) {
            frame_count++;
            if (frame_count % FALL_EVERY == 0) {
                move_piece_down();
            }
            if (frame_count % LINE_EVERY == 0) {
                move_time_line();
            }
        }

        render();

        if (game_over) {
            _XL_EXPLOSION_SOUND();
            _XL_WAIT_FOR_INPUT();
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}