#include "cross_lib.h"

static uint8_t board[4][4];
static uint8_t empty_r;
static uint8_t empty_c;
static uint16_t move_count;

/*
 * Solvability guarantee:
 * We start from the solved state and perform only legal slide moves.
 * Each slide is a transposition (swap) of the blank with an adjacent tile.
 * The set of solvable configurations is closed under legal moves, so
 * every state reachable from the solved state is solvable.
 */

static void init_board(void)
{
    uint8_t i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            board[i][j] = (uint8_t)(i * 4 + j + 1);
        }
    }
    board[3][3] = 0;
    empty_r = 3;
    empty_c = 3;
    move_count = 0;
}

static void do_shuffle(void)
{
    uint8_t i, n, dir, nr, nc;
    n = (uint8_t)(50 + (_XL_RAND() % 100));
    for (i = 0; i < n; i++) {
        dir = (uint8_t)(_XL_RAND() % 4);
        nr = empty_r;
        nc = empty_c;
        if (dir == 0 && nr > 0) {
            nr = nr - 1;
        } else if (dir == 1 && nr < 3) {
            nr = nr + 1;
        } else if (dir == 2 && nc > 0) {
            nc = nc - 1;
        } else if (dir == 3 && nc < 3) {
            nc = nc + 1;
        }
        if (nr != empty_r || nc != empty_c) {
            board[empty_r][empty_c] = board[nr][nc];
            board[nr][nc] = 0;
            empty_r = nr;
            empty_c = nc;
        }
    }
}

/*
 * Draws a single 4x4 screen-tile block for game cell (r,c).
 * Layout within the 4x4 block:
 *   Row 0: _TILE_16 x4  (horizontal border)
 *   Row 1: _TILE_0 | tile | tile | _TILE_0  (vertical borders + center top)
 *   Row 2: _TILE_0 | tile | tile | _TILE_0  (vertical borders + center bottom)
 *   Row 3: _TILE_16 x4  (horizontal border)
 * The 2x2 center uses _TILE_1 .. _TILE_15 for values 1..15.
 * The blank (value 0) has its 2x2 center deleted.
 */
static void draw_tile(uint8_t r, uint8_t c)
{
    uint8_t val, px, py, tid;
    val = board[r][c];
    px = (uint8_t)(c * 4);
    py = (uint8_t)(1 + r * 4);

    /* Upper border: 4 horizontal segments */
    _XL_DRAW(px, py, _TILE_16, _XL_WHITE);
    _XL_DRAW(px + 1, py, _TILE_16, _XL_WHITE);
    _XL_DRAW(px + 2, py, _TILE_16, _XL_WHITE);
    _XL_DRAW(px + 3, py, _TILE_16, _XL_WHITE);

    /* Middle rows: left vertical border */
    _XL_DRAW(px, py + 1, _TILE_0, _XL_WHITE);
    _XL_DRAW(px, py + 2, _TILE_0, _XL_WHITE);

    /* Middle rows: right vertical border */
    _XL_DRAW(px + 3, py + 1, _TILE_0, _XL_WHITE);
    _XL_DRAW(px + 3, py + 2, _TILE_0, _XL_WHITE);

    /* Center 2x2 */
    if (val == 0) {
        _XL_DELETE(px + 1, py + 1);
        _XL_DELETE(px + 2, py + 1);
        _XL_DELETE(px + 1, py + 2);
        _XL_DELETE(px + 2, py + 2);
    } else {
        tid = val; /* _TILE_1 through _TILE_15 */
        _XL_DRAW(px + 1, py + 1, tid, _XL_CYAN);
        _XL_DRAW(px + 2, py + 1, tid, _XL_CYAN);
        _XL_DRAW(px + 1, py + 2, tid, _XL_CYAN);
        _XL_DRAW(px + 2, py + 2, tid, _XL_CYAN);
    }

    /* Lower border: 4 horizontal segments */
    _XL_DRAW(px, py + 3, _TILE_16, _XL_WHITE);
    _XL_DRAW(px + 1, py + 3, _TILE_16, _XL_WHITE);
    _XL_DRAW(px + 2, py + 3, _TILE_16, _XL_WHITE);
    _XL_DRAW(px + 3, py + 3, _TILE_16, _XL_WHITE);
}

static void draw_all(void)
{
    uint8_t i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            draw_tile(i, j);
        }
    }
}

static void draw_moves(void)
{
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(1, 17, "MOVES ");
    _XL_PRINTD(7, 17, 3, move_count);
}

static uint8_t is_solved(void)
{
    uint8_t i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (board[i][j] != (uint8_t)(i * 4 + j + 1)) {
                return 0;
            }
        }
    }
    return 1;
}

int main(void)
{
    uint8_t input, nr, nc;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        _XL_CLEAR_SCREEN();
        init_board();
        do_shuffle();

        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(1, 0, "15 PUZZLE");

        draw_all();
        draw_moves();

        while (1) {
            input = _XL_INPUT();
            nr = empty_r;
            nc = empty_c;

            if (_XL_LEFT(input)) {
                if (nc < 3) nc = nc + 1;
            } else if (_XL_RIGHT(input)) {
                if (nc > 0) nc = nc - 1;
            } else if (_XL_UP(input)) {
                if (nr < 3) nr = nr + 1;
            } else if (_XL_DOWN(input)) {
                if (nr > 0) nr = nr - 1;
            }

            if (nr != empty_r || nc != empty_c) {
                board[empty_r][empty_c] = board[nr][nc];
                board[nr][nc] = 0;
                draw_tile(empty_r, empty_c);
                empty_r = nr;
                empty_c = nc;
                draw_tile(empty_r, empty_c);
                move_count = move_count + 1;
                draw_moves();
                _XL_TICK_SOUND();
            }

            if (is_solved()) {
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(1, 0, "SOLVED!     ");
                _XL_EXPLOSION_SOUND();
                _XL_WAIT_FOR_INPUT();
                break;
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }
    return 0;
}