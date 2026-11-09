#include "cross_lib.h"

#define HORIZONTAL_TILE _TILE_20
#define VERTICAL_TILE   _TILE_21

static uint8_t board[4][4];
static uint8_t empty_r;
static uint8_t empty_c;
static uint16_t move_count;


static uint8_t top_tile[] = { \
_TILE_0,
_TILE_1,
_TILE_2,
_TILE_3,
_TILE_4,
_TILE_5,
_TILE_6,
_TILE_7,
_TILE_8,
_TILE_9
};

static uint8_t bottom_tile[] = { \
_TILE_10,
_TILE_11,
_TILE_12,
_TILE_13,
_TILE_14,
_TILE_15,
_TILE_16,
_TILE_17,
_TILE_18,
_TILE_19
};



/*
 * Solvability: we start from the solved state and apply only legal
 * slide moves.  The solved state is trivially solvable and the set
 * of solvable configurations is closed under legal moves, so every
 * state we reach is solvable.
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
 * Draws one 4x4 screen-tile block for game cell (r, c).
 * Layout:
 *   Row 0: HORIZONTAL_TILE x4
 *   Row 1: VERTICAL_TILE, C1, C2, VERTICAL_TILE
 *   Row 2: VERTICAL_TILE, C3, C4, VERTICAL_TILE
 *   Row 3: HORIZONTAL_TILE x4
 * where C1..C4 are the 4 distinct centre tiles for this value.
 * Value v uses tiles _TILE_(4*(v-1)+1) .. _TILE_(4*(v-1)+4).
 */
static void draw_tile(uint8_t r, uint8_t c)
{
    uint8_t val, px, py, digit0, digit1;
    val = board[r][c];
    px = (uint8_t)(c * 4);
    py = (uint8_t)(1 + r * 4);

    _XL_DRAW(px,     py,     HORIZONTAL_TILE, _XL_WHITE);
    _XL_DRAW(px + 1, py,     HORIZONTAL_TILE, _XL_WHITE);
    _XL_DRAW(px + 2, py,     HORIZONTAL_TILE, _XL_WHITE);
    _XL_DRAW(px + 3, py,     HORIZONTAL_TILE, _XL_WHITE);

    _XL_DRAW(px,     py + 1, VERTICAL_TILE,  _XL_WHITE);
    _XL_DRAW(px,     py + 2, VERTICAL_TILE,  _XL_WHITE);
    _XL_DRAW(px + 3, py + 1, VERTICAL_TILE,  _XL_WHITE);
    _XL_DRAW(px + 3, py + 2, VERTICAL_TILE,  _XL_WHITE);

    if (val == 0) {
        _XL_DELETE(px + 1, py + 1);
        _XL_DELETE(px + 2, py + 1);
        _XL_DELETE(px + 1, py + 2);
        _XL_DELETE(px + 2, py + 2);
    } else {
        // base = (uint8_t)(4 * (val - 1));
        digit1 = val % 10;
        digit0 = val / 10;
        _XL_DRAW(px + 1, py + 1, top_tile[digit0], _XL_CYAN);
        _XL_DRAW(px + 2, py + 1, top_tile[digit1], _XL_CYAN);
        _XL_DRAW(px + 1, py + 2, bottom_tile[digit0], _XL_CYAN);
        _XL_DRAW(px + 2, py + 2, bottom_tile[digit1], _XL_CYAN);
    }

    _XL_DRAW(px,     py + 3, HORIZONTAL_TILE, _XL_WHITE);
    _XL_DRAW(px + 1, py + 3, HORIZONTAL_TILE, _XL_WHITE);
    _XL_DRAW(px + 2, py + 3, HORIZONTAL_TILE, _XL_WHITE);
    _XL_DRAW(px + 3, py + 3, HORIZONTAL_TILE, _XL_WHITE);
}

/* Draw a single column (4 px tall) of the 4x4 block.
 * cx is the column offset 0..3 within the block. */
static void draw_col(uint8_t px, uint8_t py, uint8_t cx, uint8_t val)
{
    uint8_t digit0, digit1, c1, c2, c3, c4;
    // base = (uint8_t)(4 * (val - 1));
    // TODO: Optimize this by avoiding useless computation 
    digit0 = val / 10;
    digit1 = val % 10;
    c1 = top_tile[digit0];
    c2 = top_tile[digit1];
    c3 = bottom_tile[digit0];
    c4 = bottom_tile[digit1];

    _XL_DRAW(px, py,     HORIZONTAL_TILE, _XL_WHITE);
    if (cx == 0 || cx == 3) {
        _XL_DRAW(px, py + 1, VERTICAL_TILE, _XL_WHITE);
        _XL_DRAW(px, py + 2, VERTICAL_TILE, _XL_WHITE);
    } else if (cx == 1) {
        _XL_DRAW(px, py + 1, c1, _XL_CYAN);
        _XL_DRAW(px, py + 2, c3, _XL_CYAN);
    } else {
        _XL_DRAW(px, py + 1, c2, _XL_CYAN);
        _XL_DRAW(px, py + 2, c4, _XL_CYAN);
    }
    _XL_DRAW(px, py + 3, HORIZONTAL_TILE, _XL_WHITE);
}

/* Clear a single column (4 px tall). */
static void clear_col(uint8_t px, uint8_t py)
{
    _XL_DELETE(px, py);
    _XL_DELETE(px, py + 1);
    _XL_DELETE(px, py + 2);
    _XL_DELETE(px, py + 3);
}

/* Draw a single row (4 px wide) of the 4x4 block.
 * ry is the row offset 0..3 within the block. */
static void draw_row(uint8_t px, uint8_t py, uint8_t ry, uint8_t val)
{
    uint8_t digit0, digit1, c1, c2, c3, c4;
    // base = (uint8_t)(4 * (val - 1));
    // TODO: Optimize this by avoiding useless cases
    digit0 = val / 10;
    digit1 = val % 10;
    c1 = top_tile[digit0];
    c2 = top_tile[digit1];
    c3 = bottom_tile[digit0];
    c4 = bottom_tile[digit1];

    if (ry == 0 || ry == 3) {
        _XL_DRAW(px,     py, HORIZONTAL_TILE, _XL_WHITE);
        _XL_DRAW(px + 1, py, HORIZONTAL_TILE, _XL_WHITE);
        _XL_DRAW(px + 2, py, HORIZONTAL_TILE, _XL_WHITE);
        _XL_DRAW(px + 3, py, HORIZONTAL_TILE, _XL_WHITE);
    } else {
        _XL_DRAW(px,     py, VERTICAL_TILE, _XL_WHITE);
        if (ry == 1) {
            _XL_DRAW(px + 1, py, c1, _XL_CYAN);
            _XL_DRAW(px + 2, py, c2, _XL_CYAN);
        } else {
            _XL_DRAW(px + 1, py, c3, _XL_CYAN);
            _XL_DRAW(px + 2, py, c4, _XL_CYAN);
        }
        _XL_DRAW(px + 3, py, VERTICAL_TILE, _XL_WHITE);
    }
}

/* Clear a single row (4 px wide). */
static void clear_row(uint8_t px, uint8_t py)
{
    _XL_DELETE(px,     py);
    _XL_DELETE(px + 1, py);
    _XL_DELETE(px + 2, py);
    _XL_DELETE(px + 3, py);
}

/*
 * Animate the sliding of a 4x4 tile from (from_r,from_c) to (to_r,to_c).
 * The tile moves one screen-tile at a time over 4 steps.
 */
static void animate_move(uint8_t from_r, uint8_t from_c,
                         uint8_t to_r, uint8_t to_c, uint8_t val)
{
    uint8_t src_px, src_py, s;
    src_px = (uint8_t)(from_c * 4);
    src_py = (uint8_t)(1 + from_r * 4);

    if (to_r == from_r) {
        if (to_c > from_c) {
            /* moving right */
            for (s = 0; s < 4; s++) {
                clear_col(src_px + s, src_py);
                draw_col(src_px + s + 4, src_py, s, val);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
        } else {
            /* moving left */
            for (s = 0; s < 4; s++) {
                clear_col(src_px + 3 - s, src_py);
                draw_col(src_px - 1 - s, src_py, 3-s, val);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
        }
    } else {
        if (to_r > from_r) {
            /* moving down */
            for (s = 0; s < 4; s++) {
                clear_row(src_px, src_py + s);
                draw_row(src_px, src_py + s + 4, s, val);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
        } else {
            /* moving up */
            for (s = 0; s < 4; s++) {
                clear_row(src_px, src_py + 3 - s);
                draw_row(src_px, src_py - 1 - s, 3-s, val);
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
        }
    }
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
            if((i==3)&&(j==3))
            {
                return 1;
            }
            if (board[i][j] != (uint8_t)(i * 4 + j + 1)) {
                // _XL_PRINTD(0,YSize-1,2,i);
                // _XL_PRINTD(6,YSize-1,2,j);
                // _XL_WAIT_FOR_INPUT();
                return 0;
            }
        }
    }
    return 1;
}

int main(void)
{
    uint8_t input, nr, nc, moved_val;

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
                moved_val = board[nr][nc];
                board[empty_r][empty_c] = board[nr][nc];
                board[nr][nc] = 0;
                animate_move(nr, nc, empty_r, empty_c, moved_val);
                empty_r = nr;
                empty_c = nc;
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