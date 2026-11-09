#include "cross_lib.h"

/* Use an odd length. 9 gives four frogs on each side plus one empty pad. */
#define PUZZLE_LENGTH 9
#define PUZZLE_HALF ((PUZZLE_LENGTH - 1) / 2)
#define FROG_TILE_VARIANTS 4

#define PAD_TILE ((uint8_t)_TILE_8)
#define PAD_COLOR ((uint8_t)_XL_GREEN)

#define RIGHT_FROG_COLOR ((uint8_t)_XL_RED)
#define LEFT_FROG_COLOR ((uint8_t)_XL_CYAN)

#define CURSOR_LEFT 0u
#define CURSOR_RIGHT 1u

static uint8_t board[PUZZLE_LENGTH];

static uint8_t screen_x[PUZZLE_LENGTH];
static uint8_t screen_y[PUZZLE_LENGTH];

static uint8_t last_drawn[PUZZLE_LENGTH];
static uint8_t last_tile[PUZZLE_LENGTH];
static uint8_t last_color[PUZZLE_LENGTH];

/* Left-facing frogs use these four tiles. */
static uint8_t left_frog_tiles[FROG_TILE_VARIANTS] = {
    (uint8_t)_TILE_0,
    (uint8_t)_TILE_1,
    (uint8_t)_TILE_2,
    (uint8_t)_TILE_3
};

/* Right-facing frogs use four other tiles. */
static uint8_t right_frog_tiles[FROG_TILE_VARIANTS] = {
    (uint8_t)_TILE_4,
    (uint8_t)_TILE_5,
    (uint8_t)_TILE_6,
    (uint8_t)_TILE_7
};

static uint8_t cursor;
static uint8_t last_cursor_marker_pos = 255u;

static uint16_t moves;
static uint8_t solved;

static uint16_t drawn_moves = 0xFFFFu;
static uint8_t drawn_status = 255u;

static uint8_t frog_is_right(uint8_t id)
{
    return (uint8_t)(id != 0u && id <= PUZZLE_HALF);
}

static uint8_t frog_variant(uint8_t id)
{
    return (uint8_t)((id - 1u) % FROG_TILE_VARIANTS);
}

static void set_screen_positions(void)
{
    uint16_t start_x = 0u;
    uint8_t i;

    if ((uint16_t)XSize >= (uint16_t)PUZZLE_LENGTH) {
        start_x = (uint16_t)(((uint16_t)XSize - (uint16_t)PUZZLE_LENGTH) / 2u);
    }

    for (i = 0u; i < PUZZLE_LENGTH; ++i) {
        screen_x[i] = (uint8_t)(start_x + i);
        screen_y[i] = (uint8_t)(YSize / 2);
    }
}

static uint8_t board_y(void)
{
    return (uint8_t)(YSize / 2);
}

static uint8_t status_y(void)
{
    if ((uint16_t)YSize >= 1u) {
        return (uint8_t)((uint16_t)YSize - 1u);
    } else {
        return 0u;
    }
}

static uint8_t moves_value_x(void)
{
    if ((uint16_t)XSize >= 4u) {
        return (uint8_t)((uint16_t)XSize - 4u);
    } else {
        return 0u;
    }
}

static uint8_t moves_label_x(void)
{
    if ((uint16_t)XSize >= 6u) {
        return (uint8_t)((uint16_t)XSize - 6u);
    } else {
        return 0u;
    }
}

static uint8_t cursor_marker_y(void)
{
    uint8_t by = board_y();
    uint8_t sy = status_y();

    if ((uint16_t)by + 1u < (uint16_t)YSize &&
        (uint16_t)by + 1u != (uint16_t)sy) {
        return (uint8_t)(by + 1u);
    }

    if (by > 0u && by != sy) {
        return (uint8_t)(by - 1u);
    }

    return by;
}

static uint8_t cell_tile(uint8_t pos)
{
    uint8_t id = board[pos];
    uint8_t variant;

    if (id == 0u) {
        return PAD_TILE;
    }

    variant = frog_variant(id);

    if (frog_is_right(id)) {
        return right_frog_tiles[variant];
    } else {
        return left_frog_tiles[variant];
    }
}

static uint8_t cell_color(uint8_t pos)
{
    uint8_t id = board[pos];

    if (id == 0u) {
        return PAD_COLOR;
    }

    if (frog_is_right(id)) {
        return RIGHT_FROG_COLOR;
    } else {
        return LEFT_FROG_COLOR;
    }
}

static void render_cell(uint8_t pos)
{
    uint8_t tile = cell_tile(pos);
    uint8_t color = cell_color(pos);

    if (last_drawn[pos] == 0u) {
        _XL_DRAW(screen_x[pos], screen_y[pos], tile, color);
    } else if (tile != last_tile[pos] || color != last_color[pos]) {
        _XL_DELETE(screen_x[pos], screen_y[pos]);
        _XL_DRAW(screen_x[pos], screen_y[pos], tile, color);
    }

    last_drawn[pos] = 1u;
    last_tile[pos] = tile;
    last_color[pos] = color;
}

static void render_all_changed(void)
{
    uint8_t i;

    for (i = 0u; i < PUZZLE_LENGTH; ++i) {
        render_cell(i);
    }
}

static void check_solved(void)
{
    uint8_t i;

    solved = 1u;

    for (i = 0u; i < PUZZLE_LENGTH; ++i) {
        if (i == PUZZLE_HALF) {
            if (board[i] != 0u) {
                solved = 0u;
            }
        } else if (i < PUZZLE_HALF) {
            uint8_t id = board[i];

            /* Left half must contain left-facing frogs. */
            if (id == 0u || frog_is_right(id) != 0u) {
                solved = 0u;
            }
        } else {
            uint8_t id = board[i];

            /* Right half must contain right-facing frogs. */
            if (id == 0u || frog_is_right(id) == 0u) {
                solved = 0u;
            }
        }
    }
}

static void update_text(void)
{
    uint8_t status;

    if (moves != drawn_moves) {
        _XL_SET_TEXT_COLOR((uint8_t)_XL_WHITE);

        if ((uint16_t)XSize >= 4u) {
            _XL_PRINT(moves_value_x(), 0u, "    " " ");
        }

        _XL_PRINTD(moves_value_x(), 0u, 1u, moves);
        drawn_moves = moves;
    }

    status = solved ? 1u : 0u;

    if (status != drawn_status) {
        uint8_t sy = status_y();

        _XL_SET_TEXT_COLOR((uint8_t)_XL_WHITE);
        _XL_PRINT(0u, sy, "    " "    " "    " "    " "    ");

        if (solved) {
            _XL_SET_TEXT_COLOR((uint8_t)_XL_YELLOW);
            _XL_PRINT(0u, sy, "SOLVED PRESS ANY KEY");
        } else {
            _XL_SET_TEXT_COLOR((uint8_t)_XL_WHITE);
            _XL_PRINT(0u, sy, "SWAP THE FROGS");
        }

        drawn_status = status;
    }
}

static void set_cursor(uint8_t pos)
{
    uint8_t my = cursor_marker_y();

    if (last_cursor_marker_pos != 255u && last_cursor_marker_pos != pos) {
        _XL_SET_TEXT_COLOR((uint8_t)_XL_WHITE);
        _XL_PRINT(screen_x[last_cursor_marker_pos], my, " ");
    }

    cursor = pos;

    _XL_SET_TEXT_COLOR((uint8_t)_XL_YELLOW);
    _XL_PRINT(screen_x[cursor], my, "C");

    last_cursor_marker_pos = cursor;
}

static void move_cursor(uint8_t delta)
{
    if (delta == CURSOR_RIGHT) {
        if (cursor < PUZZLE_LENGTH - 1u) {
            ++cursor;
        } else {
            cursor = 0u;
        }
    } else {
        if (cursor > 0u) {
            --cursor;
        } else {
            cursor = (uint8_t)(PUZZLE_LENGTH - 1u);
        }
    }

    set_cursor(cursor);
}

static uint8_t try_move(void)
{
    uint8_t source = cursor;
    uint8_t id = board[source];
    uint8_t target = 0u;
    uint8_t found = 0u;

    if (id == 0u) {
        _XL_TICK_SOUND();
        return 0u;
    }

    if (frog_is_right(id)) {
        uint8_t p1 = (uint8_t)(source + 1u);

        if (p1 < PUZZLE_LENGTH) {
            if (board[p1] == 0u) {
                target = p1;
                found = 1u;
            } else {
                uint8_t p2 = (uint8_t)(source + 2u);

                /* Jump over exactly one opposite-facing frog. */
                if (p2 < PUZZLE_LENGTH &&
                    board[p2] == 0u &&
                    frog_is_right(board[p1]) == 0u) {
                    target = p2;
                    found = 1u;
                }
            }
        }
    } else {
        if (source > 0u) {
            uint8_t p1 = (uint8_t)(source - 1u);

            if (board[p1] == 0u) {
                target = p1;
                found = 1u;
            } else if (source >= 2u) {
                uint8_t p2 = (uint8_t)(source - 2u);

                /* Jump over exactly one opposite-facing frog. */
                if (board[p2] == 0u &&
                    frog_is_right(board[p1]) != 0u) {
                    target = p2;
                    found = 1u;
                }
            }
        }
    }

    if (!found) {
        _XL_TICK_SOUND();
        return 0u;
    }

    board[target] = id;
    board[source] = 0u;

    cursor = target;
    moves++;

    _XL_PING_SOUND();

    check_solved();

    set_cursor(cursor);

    return 1u;
}

static void init_game(void)
{
    uint8_t i;

    for (i = 0u; i < PUZZLE_LENGTH; ++i) {
        if (i < PUZZLE_HALF) {
            /* Right-facing frogs start on the left side. */
            board[i] = (uint8_t)(i + 1u);
        } else if (i == PUZZLE_HALF) {
            board[i] = 0u;
        } else {
            /* Left-facing frogs start on the right side. */
            board[i] = (uint8_t)i;
        }
    }

    moves = 0u;
    solved = 0u;

    _XL_CLEAR_SCREEN();

    for (i = 0u; i < PUZZLE_LENGTH; ++i) {
        last_drawn[i] = 0u;
        last_tile[i] = 0u;
        last_color[i] = 0u;
    }

    render_all_changed();

    _XL_SET_TEXT_COLOR((uint8_t)_XL_WHITE);
    _XL_PRINT(0u, 0u, "FROG PUZZLE");

    if ((uint16_t)XSize >= 6u) {
        _XL_PRINT(moves_label_x(), 0u, "M ");
    }

    drawn_moves = 0xFFFFu;
    drawn_status = 255u;
    update_text();

    cursor = (PUZZLE_HALF > 0) ? (uint8_t)(PUZZLE_HALF - 1u) : 0u;
    last_cursor_marker_pos = 255u;
    set_cursor(cursor);
}

void main(void)
{
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    set_screen_positions();

    while (1u) {
        uint8_t restart = 0u;

        init_game();

        do {
            input = _XL_INPUT();

            if (_XL_UP(input) || _XL_DOWN(input)) {
                restart = 1u;
            } else if (_XL_LEFT(input)) {
                move_cursor(CURSOR_LEFT);
            } else if (_XL_RIGHT(input)) {
                move_cursor(CURSOR_RIGHT);
            } else if (_XL_FIRE(input)) {
                try_move();
            }

            render_all_changed();
            update_text();

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        } while (!solved && !restart);

        if (solved) {
            _XL_TOCK_SOUND();
            _XL_SLEEP(1u);
            _XL_WAIT_FOR_INPUT();
        } else {
            _XL_SLEEP(1u);
        }
    }
}
