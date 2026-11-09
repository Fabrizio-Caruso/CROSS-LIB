#include "cross_lib.h"

/* --- Level size macros --- */
#define EASY_SIZE     9
#define MEDIUM_SIZE   13
#define HARD_SIZE     17
#define MAX_SIZE      HARD_SIZE

#define NUM_MINES_EASY    10
#define NUM_MINES_MEDIUM  20
#define NUM_MINES_HARD    35

/* --- Screen layout offsets --- */
#define GRID_X  2
#define GRID_Y  3

/* --- Tile identifiers --- */
#define TILE_HIDDEN   _TILE_0
#define TILE_REVEAL   _TILE_1
#define TILE_MINE     _TILE_2

/* --- Cell states --- */
#define CELL_HIDDEN   0
#define CELL_REVEALED 1
#define CELL_MINED    2

/* --- Game states --- */
#define STATE_SELECT  0
#define STATE_PLAYING 1
#define STATE_WON     2
#define STATE_LOST    3

/* --- Global data --- */
static uint8_t mine_grid[MAX_SIZE][MAX_SIZE];
static uint8_t cell_state[MAX_SIZE][MAX_SIZE];
static uint8_t adj_count[MAX_SIZE][MAX_SIZE];
static uint16_t stack_x[MAX_SIZE * MAX_SIZE];
static uint16_t stack_y[MAX_SIZE * MAX_SIZE];
static uint8_t num_colors[9];

static uint8_t  grid_size;
static uint8_t  num_mines;
static uint8_t  cur_x;
static uint8_t  cur_y;
static uint8_t  game_state;
static uint8_t  current_level;
static uint16_t revealed_count;
static uint16_t total_safe;

/* --- Helpers --- */

static void init_colors(void)
{
    num_colors[0] = _XL_CYAN;
    num_colors[1] = _XL_GREEN;
    num_colors[2] = _XL_YELLOW;
    num_colors[3] = _XL_MAGENTA;
    num_colors[4] = _XL_RED;
    num_colors[5] = _XL_BLUE;
    num_colors[6] = _XL_MAGENTA;
    num_colors[7] = _XL_YELLOW;
    num_colors[8] = _XL_WHITE;
}

static void init_grid(void)
{
    uint8_t i, j;
    for (i = 0; i < MAX_SIZE; i++) {
        for (j = 0; j < MAX_SIZE; j++) {
            mine_grid[i][j] = 0;
            cell_state[i][j] = CELL_HIDDEN;
            adj_count[i][j] = 0;
        }
    }
}

static void place_mines(void)
{
    uint16_t rx, ry;
    uint8_t placed;
    uint8_t i, j, di, dj, ni, nj;

    placed = 0;
    while (placed < num_mines) {
        rx = (uint16_t)(_XL_RAND() % grid_size);
        ry = (uint16_t)(_XL_RAND() % grid_size);
        if (!mine_grid[ry][rx]) {
            mine_grid[ry][rx] = 1;
            placed++;
        }
    }

    /* Compute adjacent-mine counts */
    for (i = 0; i < grid_size; i++) {
        for (j = 0; j < grid_size; j++) {
            adj_count[i][j] = 0;
            for (di = 0; di < 3; di++) {
                for (dj = 0; dj < 3; dj++) {
                    ni = (uint8_t)(i + di - 1);
                    nj = (uint8_t)(j + dj - 1);
                    if (ni < grid_size && nj < grid_size) {
                        if (mine_grid[ni][nj]) {
                            adj_count[i][j]++;
                        }
                    }
                }
            }
        }
    }
}

/* Iterative flood-fill reveal using an explicit stack */
static void reveal_cell(uint8_t cx, uint8_t cy)
{
    uint16_t sp;
    uint8_t x, y;
    uint8_t i, j, ni, nj;

    sp = 0;
    stack_x[sp] = cx;
    stack_y[sp] = cy;
    sp++;

    while (sp > 0) {
        sp--;
        x = (uint8_t)stack_x[sp];
        y = (uint8_t)stack_y[sp];

        if (cell_state[y][x] != CELL_HIDDEN) {
            continue;
        }

        if (mine_grid[y][x]) {
            cell_state[y][x] = CELL_MINED;
            game_state = STATE_LOST;
            return;
        }

        cell_state[y][x] = CELL_REVEALED;
        revealed_count++;

        /* Flood-fill: if zero adjacent mines, reveal neighbours */
        if (adj_count[y][x] == 0) {
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    if (i == 1 && j == 1) {
                        continue;
                    }
                    ni = (uint8_t)(y + i - 1);
                    nj = (uint8_t)(x + j - 1);
                    if (ni < grid_size && nj < grid_size) {
                        if (cell_state[ni][nj] == CELL_HIDDEN) {
                            stack_x[sp] = nj;
                            stack_y[sp] = ni;
                            sp++;
                        }
                    }
                }
            }
        }
    }
}

static void reveal_all_mines(void)
{
    uint8_t i, j;
    for (i = 0; i < grid_size; i++) {
        for (j = 0; j < grid_size; j++) {
            if (mine_grid[i][j] && cell_state[i][j] == CELL_HIDDEN) {
                cell_state[i][j] = CELL_MINED;
            }
        }
    }
}

/* --- Drawing --- */

static void draw_game(void)
{
    uint8_t x, y;
    uint8_t sx, sy;
    uint8_t msg_y;

    _XL_CLEAR_SCREEN();

    /* Title */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 1, "MINESWEEPER");

    /* Info bar */
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(2, 2, "LV");
    _XL_PRINTD(4, 2, 1, (uint16_t)(current_level + 1));
    _XL_PRINT(6, 2, "MINES");
    _XL_PRINTD(11, 2, 2, (uint16_t)num_mines);

    /* Grid cells */
    for (y = 0; y < grid_size; y++) {
        for (x = 0; x < grid_size; x++) {
            sx = (uint8_t)(GRID_X + x);
            sy = (uint8_t)(GRID_Y + y);

            if (cell_state[y][x] == CELL_HIDDEN) {
                if (x == cur_x && y == cur_y && game_state == STATE_PLAYING) {
                    _XL_DRAW(sx, sy, TILE_HIDDEN, _XL_WHITE);
                } else {
                    _XL_DRAW(sx, sy, TILE_HIDDEN, _XL_BLUE);
                }
            } else if (cell_state[y][x] == CELL_REVEALED) {
                _XL_DRAW(sx, sy, TILE_REVEAL, num_colors[adj_count[y][x]]);
                if (adj_count[y][x] > 0) {
                    _XL_SET_TEXT_COLOR(_XL_WHITE);
                    _XL_CHAR(sx, sy, (char)('0' + adj_count[y][x]));
                }
            } else if (cell_state[y][x] == CELL_MINED) {
                _XL_DRAW(sx, sy, TILE_MINE, _XL_RED);
            }
        }
    }

    /* Status / hint line below the grid */
    msg_y = (uint8_t)(GRID_Y + grid_size + 1);
    if (game_state == STATE_WON) {
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(GRID_X, msg_y, "YOU WIN PRESS FIRE TO CONTINUE");
    } else if (game_state == STATE_LOST) {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(GRID_X, msg_y, "GAME OVER PRESS FIRE TO CONTINUE");
    } else {
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(GRID_X, msg_y, "ARROWS MOVE FIRE REVEAL");
    }
}

/* --- Game flow --- */

static void start_game(uint8_t level)
{
    current_level = level;
    if (level == 0) {
        grid_size = EASY_SIZE;
        num_mines = NUM_MINES_EASY;
    } else if (level == 1) {
        grid_size = MEDIUM_SIZE;
        num_mines = NUM_MINES_MEDIUM;
    } else {
        grid_size = HARD_SIZE;
        num_mines = NUM_MINES_HARD;
    }

    init_grid();
    place_mines();
    cur_x = (uint8_t)(grid_size / 2);
    cur_y = (uint8_t)(grid_size / 2);
    game_state = STATE_PLAYING;
    revealed_count = 0;
    total_safe = (uint16_t)(grid_size * grid_size) - (uint16_t)num_mines;
}

static void select_level(void)
{
    uint8_t input;
    uint8_t sel;

    sel = 0;
    game_state = STATE_SELECT;

    while (game_state == STATE_SELECT) {
        _XL_CLEAR_SCREEN();

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(4, 2, "SELECT LEVEL");

        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(6, 4, "1 EASY 9X9 10 MINES");
        _XL_PRINT(6, 5, "2 MEDIUM 13X13 20 MINES");
        _XL_PRINT(6, 6, "3 HARD 17X17 35 MINES");

        _XL_SET_TEXT_COLOR(_XL_GREEN);
        if (sel == 0) {
            _XL_CHAR(4, 4, '>');
        } else if (sel == 1) {
            _XL_CHAR(4, 5, '>');
        } else {
            _XL_CHAR(4, 6, '>');
        }

        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(6, 8, "UP DOWN SELECT FIRE START");

        input = _XL_INPUT();
        if (_XL_UP(input)) {
            if (sel > 0) sel--;
        }
        if (_XL_DOWN(input)) {
            if (sel < 2) sel++;
        }
        if (_XL_FIRE(input)) {
            start_game(sel);
            return;
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

/* --- Main --- */

int main(void)
{
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();
    init_colors();

    /* Infinite outer loop so the game can be replayed */
    while (1) {

        select_level();

        /* Active gameplay loop */
        while (game_state == STATE_PLAYING) {
            draw_game();

            input = _XL_INPUT();

            if (_XL_LEFT(input)) {
                if (cur_x > 0) cur_x--;
            }
            if (_XL_RIGHT(input)) {
                if (cur_x < grid_size - 1) cur_x++;
            }
            if (_XL_UP(input)) {
                if (cur_y > 0) cur_y--;
            }
            if (_XL_DOWN(input)) {
                if (cur_y < grid_size - 1) cur_y++;
            }
            if (_XL_FIRE(input)) {
                if (cell_state[cur_y][cur_x] == CELL_HIDDEN) {
                    _XL_TICK_SOUND();
                    reveal_cell(cur_x, cur_y);

                    if (game_state == STATE_LOST) {
                        reveal_all_mines();
                        _XL_EXPLOSION_SOUND();
                    } else if (revealed_count >= total_safe) {
                        game_state = STATE_WON;
                        _XL_PING_SOUND();
                    }
                }
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* Show final board and wait for the player */
        draw_game();
        _XL_WAIT_FOR_INPUT();
        _XL_SLEEP(1);
    }

    return 0;
}

