#include <stdint.h>
#include "cross_lib.h"

#define GRID_W 8
#define GRID_H 18
#define BLOCK_TILE _TILE_8

/* Piece cell offsets: [type][rotation][cell] = {dx, dy} */
static const uint8_t PIECE_CELLS[7][4][4][2] = {
    // /* I piece (0) */
    // {
        // {{0,0},{1,0},{2,0},{3,0}},
        // {{1,0},{1,1},{1,2},{1,3}},
        // {{0,0},{1,0},{2,0},{3,0}},
        // {{1,0},{1,1},{1,2},{1,3}}
    // },
    /* I piece (0) */
    {
        {{0,0},{1,0},{2,0},{3,0}},   /* rotation 0: horizontal */
        {{0,0},{0,1},{0,2},{0,3}},   /* rotation 1: vertical (fixed) */
        {{0,0},{1,0},{2,0},{3,0}},   /* rotation 2 */
        {{0,0},{0,1},{0,2},{0,3}}    /* rotation 3 (fixed) */
    },
    /* O piece (1) */
    {
        {{0,0},{1,0},{0,1},{1,1}},
        {{0,0},{1,0},{0,1},{1,1}},
        {{0,0},{1,0},{0,1},{1,1}},
        {{0,0},{1,0},{0,1},{1,1}}
    },
    /* T piece (2) */
    {
        {{1,0},{0,1},{1,1},{2,1}},
        {{1,0},{1,1},{2,1},{1,2}},
        {{0,1},{1,1},{2,1},{1,2}},
        {{0,1},{1,0},{1,1},{1,2}}
    },
    /* S piece (3) */
    {
        {{1,0},{2,0},{0,1},{1,1}},
        {{1,0},{1,1},{2,1},{2,2}},
        {{1,0},{2,0},{0,1},{1,1}},
        {{1,0},{1,1},{2,1},{2,2}}
    },
    /* Z piece (4) */
    {
        {{0,0},{1,0},{1,1},{2,1}},
        {{2,0},{1,1},{2,1},{1,2}},
        {{0,0},{1,0},{1,1},{2,1}},
        {{2,0},{1,1},{2,1},{1,2}}
    },
    /* L piece (5) */
    {
        {{0,0},{0,1},{0,2},{1,2}},
        {{0,0},{1,0},{2,0},{2,1}},
        {{2,0},{2,1},{2,2},{1,2}},
        {{0,1},{0,2},{1,2},{2,2}}
    },
    /* J piece (6) */
    // {
        // {{2,0},{0,1},{1,1},{2,1}},
        // {{0,0},{0,1},{0,2},{1,0}},
        // {{0,2},{1,2},{2,2},{2,1}},
        // {{1,0},{0,1},{0,2},{1,1}}
    // }
    {
        {{2,0},{0,1},{1,1},{2,1}},   /* rotation 0 */
        {{0,0},{0,1},{0,2},{1,0}},   /* rotation 1 */
        {{0,2},{1,2},{2,2},{2,1}},   /* rotation 2 */
        {{0,0},{0,1},{0,2},{1,2}}    /* rotation 3 (fixed) */
    },
};

static const uint8_t PIECE_COLORS[7] = {
    _XL_CYAN, _XL_YELLOW, _XL_MAGENTA,
    _XL_GREEN, _XL_RED, _XL_WHITE, _XL_BLUE
};

/* Game state */
static uint8_t grid[GRID_H][GRID_W];         /* 0=empty, 1..7 = piece_type+1 */
static uint8_t screen_state[GRID_H][GRID_W]; /* mirrors what is on screen     */
static uint8_t cur_piece;
static uint8_t cur_rot;
static uint8_t cur_x;
static uint8_t cur_y;
static uint16_t score;
static uint8_t game_over_flag;

/* ---------- helpers ---------- */

static int can_place(uint8_t px, uint8_t py, uint8_t ptype, uint8_t rot)
{
    uint8_t i;
    for (i = 0; i < 4; i++) {
        uint8_t cx = px + PIECE_CELLS[ptype][rot][i][0];
        uint8_t cy = py + PIECE_CELLS[ptype][rot][i][1];
        if (cx >= GRID_W || cy >= GRID_H) return 0;
        if (grid[cy][cx] != 0) return 0;
    }
    return 1;
}

static void spawn_piece(void)
{
    cur_piece = (uint8_t)(_XL_RAND() % 7);
    cur_rot   = 0;
    cur_x     = 3;
    cur_y     = 0;
    if (!can_place(cur_x, cur_y, cur_piece, cur_rot)) {
        game_over_flag = 1;
    }
}

static void lock_piece(void)
{
    uint8_t i;
    for (i = 0; i < 4; i++) {
        uint8_t cx = cur_x + PIECE_CELLS[cur_piece][cur_rot][i][0];
        uint8_t cy = cur_y + PIECE_CELLS[cur_piece][cur_rot][i][1];
        if (cx < GRID_W && cy < GRID_H) {
            grid[cy][cx] = cur_piece + 1;
        }
    }
}


/* score text (below the grid) */
static void display_score(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, GRID_H + 1, "SCORE");
    _XL_PRINTD(6, GRID_H + 1, 1, score);
}

static void clear_lines(void)
{
    uint8_t y, x, ny;
    for (y = 0; y < GRID_H;) {
        uint8_t full = 1;
        for (x = 0; x < GRID_W; x++) {
            if (grid[y][x] == 0) { full = 0; break; }
        }
        if (full) {
            /* shift rows above down */
            for (ny = y; ny > 0; ny--) {
                for (x = 0; x < GRID_W; x++) {
                    grid[ny][x] = grid[ny - 1][x];
                }
            }
            for (x = 0; x < GRID_W; x++) {
                grid[0][x] = 0;
            }
            score += 10;
            display_score();
        } else {
            y++;
        }
    }
}

/* Only issue _XL_DRAW / _XL_DELETE for cells whose state actually changed. */
static void update_display(void)
{
    uint8_t x, y, i;
    uint8_t cx;
    uint8_t cy;
    uint8_t color;
    
    for (y = 0; y < GRID_H; y++) {
        for (x = 0; x < GRID_W; x++) {
            uint8_t should_show = grid[y][x];

            /* overlay current piece */
            if (!game_over_flag) {
                for (i = 0; i < 4; i++) {
                    cx = cur_x + PIECE_CELLS[cur_piece][cur_rot][i][0];
                    cy = cur_y + PIECE_CELLS[cur_piece][cur_rot][i][1];
                    if (cx == x && cy == y) {
                        should_show = cur_piece + 1;
                        break;
                    }
                }
            }

            if (should_show != screen_state[y][x]) {
                if (should_show == 0) {
                    _XL_DELETE(x, y);
                } else {
                    color = PIECE_COLORS[should_show - 1];
                    _XL_DRAW(x, y, BLOCK_TILE, color);
                }
                screen_state[y][x] = should_show;
            }
        }
    }
}

/* ---------- main ---------- */

int main(void)
{
    uint8_t x, y;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();


    for(;;)
    {
        _XL_CLEAR_SCREEN();
            
        for (y = 0; y < GRID_H; y++)
            for (x = 0; x < GRID_W; x++) {
                grid[y][x] = 0;
                screen_state[y][x] = 0;
            }

        score = 0;
        game_over_flag = 0;
        spawn_piece();
        display_score();

        while (!game_over_flag) {
            uint8_t input = _XL_INPUT();

            /* horizontal / vertical movement */
            if (_XL_LEFT(input)) {
                if (cur_x > 0) {
                    uint8_t nx = cur_x - 1;
                    if (can_place(nx, cur_y, cur_piece, cur_rot))
                        cur_x = nx;
                }
            }
            if (_XL_RIGHT(input)) {
                uint8_t nx = cur_x + 1;
                if (nx < GRID_W && can_place(nx, cur_y, cur_piece, cur_rot))
                    cur_x = nx;
            }
            if (_XL_DOWN(input)) {
                uint8_t ny = cur_y + 1;
                if (can_place(cur_x, ny, cur_piece, cur_rot))
                    cur_y = ny;
            }

            /* rotate on FIRE */
            if (_XL_FIRE(input)) {
                uint8_t nr = (uint8_t)((cur_rot + 1) % 4);
                if (can_place(cur_x, cur_y, cur_piece, nr))
                    cur_rot = nr;
            }

            /* gravity: fall one row each tick */
            {
                uint8_t ny = cur_y + 1;
                if (!can_place(cur_x, ny, cur_piece, cur_rot)) {
                    lock_piece();
                    clear_lines();
                    spawn_piece();
                } else {
                    cur_y = ny;
                }
            }

            update_display();

            if (game_over_flag) {
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(2, GRID_H / 2, "GAME OVER");
            }

            _XL_SLOW_DOWN((uint16_t)(_XL_SLOW_DOWN_FACTOR)/8);
        }

        /* final frame already drawn; wait for a key */
        _XL_WAIT_FOR_INPUT();
        
    }
    return 0;
}
