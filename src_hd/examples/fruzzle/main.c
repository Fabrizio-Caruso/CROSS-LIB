#include "cross_lib.h"

#define FROG_COUNT 3
#define BOARD_SIZE (2 * FROG_COUNT + 1)

/* Left-facing frog: 4 tiles */
#define LF_T0 _TILE_0
#define LF_T1 _TILE_1
#define LF_T2 _TILE_2
#define LF_T3 _TILE_3

/* Right-facing frog: 4 other tiles */
#define RF_T0 _TILE_12
#define RF_T1 _TILE_13
#define RF_T2 _TILE_14
#define RF_T3 _TILE_15

/* State values */
#define ST_EMPTY     0
#define ST_LEFT_FROG   1
#define ST_RIGHT_FROG  2

/* Layout */
#define SPACING 7
#define BASE_X  4
#define BASE_Y  9

uint8_t board[BOARD_SIZE];
uint8_t prev_board[BOARD_SIZE];
uint8_t cursor;
uint8_t prev_cursor;

void init_game(void) {
    uint8_t i;
    for (i = 0; i < FROG_COUNT; i++) {
        board[i] = ST_RIGHT_FROG;
        board[BOARD_SIZE - 1 - i] = ST_LEFT_FROG;
    }
    board[FROG_COUNT] = ST_EMPTY;
    cursor = FROG_COUNT;
}

void draw_frog_at(uint8_t pos, uint8_t type) {
    uint8_t xb = BASE_X + pos * SPACING;
    uint8_t yb = BASE_Y;
    if (type == ST_LEFT_FROG) {
        _XL_DRAW(xb,     yb - 1, LF_T0, _XL_GREEN);
        _XL_DRAW(xb + 1, yb,     LF_T1, _XL_GREEN);
        _XL_DRAW(xb + 2, yb - 1, LF_T2, _XL_GREEN);
        _XL_DRAW(xb + 1, yb + 1, LF_T3, _XL_GREEN);
    } else {
        _XL_DRAW(xb,     yb - 1, RF_T0, _XL_RED);
        _XL_DRAW(xb + 1, yb,     RF_T1, _XL_RED);
        _XL_DRAW(xb + 2, yb - 1, RF_T2, _XL_RED);
        _XL_DRAW(xb + 1, yb + 1, RF_T3, _XL_RED);
    }
}

void clear_frog_at(uint8_t pos) {
    uint8_t xb = BASE_X + pos * SPACING;
    uint8_t yb = BASE_Y;
    _XL_DELETE(xb,     yb - 1);
    _XL_DELETE(xb + 1, yb);
    _XL_DELETE(xb + 2, yb - 1);
    _XL_DELETE(xb + 1, yb + 1);
}

void draw_cursor(uint8_t pos) {
    uint8_t xb = BASE_X + pos * SPACING;
    _XL_DRAW(xb + 1, BASE_Y - 3, _TILE_20, _XL_YELLOW);
}

void clear_cursor(uint8_t pos) {
    uint8_t xb = BASE_X + pos * SPACING;
    _XL_DELETE(xb + 1, BASE_Y - 3);
}

uint8_t check_win(void) {
    uint8_t i;
    for (i = 0; i < FROG_COUNT; i++) {
        if (board[i] != ST_LEFT_FROG) return 0;
        if (board[BOARD_SIZE - 1 - i] != ST_RIGHT_FROG) return 0;
    }
    return 1;
}

uint8_t try_move(uint8_t pos) {
    uint8_t target;

    if (board[pos] == ST_EMPTY) return 0;

    if (board[pos] == ST_RIGHT_FROG) {
        /* Step right */
        target = pos + 1;
        if (target >= BOARD_SIZE) return 0;
        if (board[target] == ST_EMPTY) {
            board[target] = ST_RIGHT_FROG;
            board[pos] = ST_EMPTY;
            _XL_PING_SOUND();
            return 1;
        }
        /* Jump right over one frog */
        target = pos + 2;
        if (target >= BOARD_SIZE) return 0;
        if (board[pos + 1] != ST_EMPTY && board[target] == ST_EMPTY) {
            board[target] = ST_RIGHT_FROG;
            board[pos] = ST_EMPTY;
            _XL_PING_SOUND();
            return 1;
        }
    } else { /* ST_LEFT_FROG */
        /* Step left */
        if (pos == 0) return 0;
        target = pos - 1;
        if (board[target] == ST_EMPTY) {
            board[target] = ST_LEFT_FROG;
            board[pos] = ST_EMPTY;
            _XL_PING_SOUND();
            return 1;
        }
        /* Jump left over one frog */
        if (pos < 2) return 0;
        target = pos - 2;
        if (board[pos - 1] != ST_EMPTY && board[target] == ST_EMPTY) {
            board[target] = ST_LEFT_FROG;
            board[pos] = ST_EMPTY;
            _XL_PING_SOUND();
            return 1;
        }
    }

    return 0;
}

void update_display(void) {
    uint8_t i;

    /* Update cursor only if it moved */
    if (cursor != prev_cursor) {
        clear_cursor(prev_cursor);
        draw_cursor(cursor);
    }

    /* Update board positions that changed */
    for (i = 0; i < BOARD_SIZE; i++) {
        if (prev_board[i] != board[i]) {
            clear_frog_at(i);
            if (board[i] != ST_EMPTY) {
                draw_frog_at(i, board[i]);
            }
        }
    }

    /* Save current state for next comparison */
    for (i = 0; i < BOARD_SIZE; i++) {
        prev_board[i] = board[i];
    }
    prev_cursor = cursor;
}

int main(void) {
    uint8_t input;
    uint8_t won;
    uint8_t i;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    /* Title and instructions */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(10, 2, "FROG JUMP PUZZLE");
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(4, 5, "L/R: SELECT  FIRE: MOVE  UP: RESTART");

    do {
        /* Initialize game state */
        init_game();

        /* Set prev to empty so first frame draws everything */
        for (i = 0; i < BOARD_SIZE; i++) {
            prev_board[i] = ST_EMPTY;
        }
        prev_cursor = cursor;

        update_display();
        _XL_SLEEP(1);

        /* Play until win or restart */
        do {
            input = _XL_INPUT();

            if (_XL_UP(input)) {
                /* Restart mid-game: reset board, keep prev for diff */
                init_game();
                _XL_TOCK_SOUND();
                update_display();
                continue;
            }

            if (_XL_LEFT(input)) {
                if (cursor > 0) cursor--;
            }
            if (_XL_RIGHT(input)) {
                if (cursor < BOARD_SIZE - 1) cursor++;
            }
            if (_XL_FIRE(input)) {
                try_move(cursor);
            }

            update_display();
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

            won = check_win();
        } while (!won);

        /* Win screen */
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(12, 8, "YOU WIN!");
        _XL_PING_SOUND();
        _XL_SLEEP(3);

    } while (1);

    return 0;
}
