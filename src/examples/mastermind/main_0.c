#include "cross_lib.h"

#define CODE_LEN 4
#define NUM_COLORS 6
#define MAX_ATTEMPTS 8

static uint8_t color_to_xl(uint8_t c)
{
    switch(c) {
        case 0:  return _XL_RED;
        case 1:  return _XL_CYAN;
        case 2:  return _XL_GREEN;
        case 3:  return _XL_YELLOW;
        case 4:  return _XL_BLUE;
        case 5:  return _XL_MAGENTA;
        default: return _XL_WHITE;
    }
}

static void draw_static(void)
{
    uint8_t i;
    _XL_SET_TEXT_COLOR(_XL_MAGENTA);
    _XL_PRINT(0, 0, "MASTERMIND");
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(0, 5, "LR=POS UD=CLR FIRE=GO");
    for(i = 0; i < NUM_COLORS; i++) {
        _XL_DRAW(i, 7, _TILE_0, color_to_xl(i));
    }
}

static void clear_attempts(void)
{
    uint8_t i;
    for(i = 9; i < 11; i++) {
        _XL_DELETE(i, 1);
    }
}

static void draw_attempts(uint8_t n)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 1, "ATTEMPTS: ");
    _XL_PRINTD(9, 1, 1, n);
}

static void clear_guess_row(uint8_t y, uint8_t old_cursor)
{
    uint8_t i;
    for(i = 0; i < CODE_LEN; i++) {
        _XL_DELETE(i * 2, y);
        if(i == old_cursor) {
            _XL_DELETE(i * 2 + 1, y);
        }
    }
}

static void draw_guess(uint8_t y, uint8_t *guess, uint8_t cursor, uint8_t 
show_cursor)
{
    uint8_t i;
    for(i = 0; i < CODE_LEN; i++) {
        _XL_DRAW(i * 2, y, _TILE_0, color_to_xl(guess[i]));
        if(show_cursor && i == cursor) {
            _XL_DRAW(i * 2 + 1, y, _TILE_1, _XL_WHITE);
        }
    }
}

static void clear_feedback(uint8_t y)
{
    uint8_t i;
    for(i = 10; i < 22; i++) {
        _XL_DELETE(i, y);
    }
}

static void draw_feedback(uint8_t y, uint8_t ok, uint8_t soft)
{
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(10, y, "OK:");
    _XL_PRINTD(13, y, 1, ok);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(15, y, "SOFT:");
    _XL_PRINTD(19, y, 1, soft);
}

static void clear_prev_row(uint8_t y, uint8_t has_feedback)
{
    uint8_t i;
    for(i = 0; i < CODE_LEN; i++) {
        _XL_DELETE(i * 2, y);
    }
    if(has_feedback) {
        for(i = 10; i < 22; i++) {
            _XL_DELETE(i, y);
        }
    }
}

static void draw_prev_guess(uint8_t y, uint8_t *g, uint8_t ok, uint8_t soft)
{
    uint8_t i;
    for(i = 0; i < CODE_LEN; i++) {
        _XL_DRAW(i * 2, y, _TILE_0, color_to_xl(g[i]));
    }
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(10, y, "OK:");
    _XL_PRINTD(13, y, 1, ok);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(15, y, "SOFT:");
    _XL_PRINTD(19, y, 1, soft);
}

static void clear_game_over(void)
{
    uint8_t i;
    for(i = 0; i < 20; i++) {
        _XL_DELETE(i, 1);
        _XL_DELETE(i, 2);
        _XL_DELETE(i, 3);
        _XL_DELETE(i, 4);
    }
    for(i = 0; i < CODE_LEN; i++) {
        _XL_DELETE(6 + i * 2, 3);
    }
}

static void draw_game_over(uint8_t won, uint8_t *secret)
{
    uint8_t i;
    _XL_SET_TEXT_COLOR(won ? _XL_GREEN : _XL_RED);
    if(won) {
        _XL_PRINT(0, 1, "YOU WIN!");
    } else {
        _XL_PRINT(0, 1, "GAME OVER");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 3, "CODE: ");
        for(i = 0; i < CODE_LEN; i++) {
            _XL_DRAW(6 + i * 2, 3, _TILE_0, color_to_xl(secret[i]));
        }
    }
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(0, 5, "FIRE=RESTART");
}

static void evaluate(uint8_t *secret, uint8_t *guess, uint8_t *p_ok, uint8_t *p_soft)
{
    uint8_t i;
    uint8_t ok = 0;
    uint8_t soft = 0;
    uint8_t s_count[NUM_COLORS];
    uint8_t g_count[NUM_COLORS];

    *p_ok = 0;
    *p_soft = 0;

    for(i = 0; i < NUM_COLORS; i++) {
        s_count[i] = 0;
        g_count[i] = 0;
    }

    for(i = 0; i < CODE_LEN; i++) {
        if(secret[i] == guess[i]) {
            ok++;
        } else {
            s_count[secret[i]]++;
            g_count[guess[i]]++;
        }
    }

    for(i = 0; i < NUM_COLORS; i++) {
        if(s_count[i] > g_count[i]) {
            soft += g_count[i];
        } else {
            soft += s_count[i];
        }
    }

    *p_ok = ok;
    *p_soft = soft;
}

static void generate_code(uint8_t *code)
{
    uint8_t i;
    uint16_t r;
    for(i = 0; i < CODE_LEN; i++) {
        r = _XL_RAND();
        code[i] = (uint8_t)(r % NUM_COLORS);
    }
}

static void reset_state(uint8_t *secret, uint8_t *guess, uint8_t *cursor,
                        uint8_t *attempts, uint8_t *game_over, uint8_t *won)
{
    uint8_t i;
    generate_code(secret);
    for(i = 0; i < CODE_LEN; i++) {
        guess[i] = 0;
    }
    *cursor = 0;
    *attempts = MAX_ATTEMPTS;
    *game_over = 0;
    *won = 0;
}

static void clear_all_dynamic(void)
{
    uint8_t i;
    /* Clear attempts row */
    for(i = 0; i < 11; i++) {
        _XL_DELETE(i, 1);
    }
    /* Clear previous guess rows (2 and 3) */
    for(i = 0; i < 22; i++) {
        _XL_DELETE(i, 2);
        _XL_DELETE(i, 3);
    }
    /* Clear current guess row (4) */
    for(i = 0; i < 8; i++) {
        _XL_DELETE(i, 4);
    }
    /* Clear feedback row (4, right side) */
    for(i = 10; i < 22; i++) {
        _XL_DELETE(i, 4);
    }
    /* Clear game over area */
    for(i = 0; i < 20; i++) {
        _XL_DELETE(i, 1);
        _XL_DELETE(i, 2);
        _XL_DELETE(i, 3);
    }
}

int main(void)
{
    uint8_t secret[CODE_LEN];
    uint8_t guess[CODE_LEN];
    uint8_t cursor;
    uint8_t attempts;
    uint8_t game_over;
    uint8_t won;
    uint8_t prev1[CODE_LEN];
    uint8_t prev1_ok, prev1_soft;
    uint8_t has_prev1;
    uint8_t prev2[CODE_LEN];
    uint8_t prev2_ok, prev2_soft;
    uint8_t has_prev2;
    uint8_t input;
    uint8_t ok, soft;
    uint8_t i;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while(1) {
        /* Reset for new game */
        reset_state(secret, guess, &cursor, &attempts, &game_over, &won);
        prev1_ok = 0; prev1_soft = 0; has_prev1 = 0;
        prev2_ok = 0; prev2_soft = 0; has_prev2 = 0;

        _XL_CLEAR_SCREEN();
        draw_static();
        draw_attempts(attempts);
        draw_guess(4, guess, cursor, 1);

        while(1) {
            input = _XL_INPUT();

            if(!input) {
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                continue;
            }

            if(game_over) {
                if(_XL_FIRE(input)) {
                    /* Restart: go to top of outer while */
                    break;
                }
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                continue;
            }

            if(_XL_LEFT(input)) {
                clear_guess_row(4, cursor);
                if(cursor > 0) {
                    cursor--;
                }
                draw_guess(4, guess, cursor, 1);
                _XL_TICK_SOUND();
            }

            if(_XL_RIGHT(input)) {
                clear_guess_row(4, cursor);
                if(cursor < CODE_LEN - 1) {
                    cursor++;
                }
                draw_guess(4, guess, cursor, 1);
                _XL_TICK_SOUND();
            }

            if(_XL_UP(input)) {
                clear_guess_row(4, cursor);
                guess[cursor] = (guess[cursor] + 1) % NUM_COLORS;
                draw_guess(4, guess, cursor, 1);
                _XL_PING_SOUND();
            }

            if(_XL_DOWN(input)) {
                clear_guess_row(4, cursor);
                if(guess[cursor] == 0) {
                    guess[cursor] = NUM_COLORS - 1;
                } else {
                    guess[cursor]--;
                }
                draw_guess(4, guess, cursor, 1);
                _XL_TOCK_SOUND();
            }

            if(_XL_FIRE(input)) {
                /* Confirm guess */
                evaluate(secret, guess, &ok, &soft);
                _XL_SHOOT_SOUND();

                /* Shift previous guesses */
                if(has_prev1) {
                    /* Move prev1 to prev2 */
                    for(i = 0; i < CODE_LEN; i++) {
                        prev2[i] = prev1[i];
                    }
                    prev2_ok = prev1_ok;
                    prev2_soft = prev1_soft;
                    has_prev2 = 1;
                }

                /* Store current as prev1 */
                for(i = 0; i < CODE_LEN; i++) {
                    prev1[i] = guess[i];
                }
                prev1_ok = ok;
                prev1_soft = soft;
                has_prev1 = 1;

                /* Clear old display rows */
                clear_prev_row(2, has_prev2);
                clear_prev_row(3, has_prev1);

                /* Redraw previous guess rows */
                if(has_prev2) {
                    draw_prev_guess(2, prev2, prev2_ok, prev2_soft);
                }
                if(has_prev1) {
                    draw_prev_guess(3, prev1, prev1_ok, prev1_soft);
                }

                /* Check win */
                if(ok == CODE_LEN) {
                    game_over = 1;
                    won = 1;
                    _XL_EXPLOSION_SOUND();
                    draw_game_over(1, secret);
                    /* Clear current guess cursor */
                    clear_guess_row(4, cursor);
                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    continue;
                }

                /* Decrement attempts */
                attempts--;
                clear_attempts();
                draw_attempts(attempts);

                /* Reset guess for next round */
                for(i = 0; i < CODE_LEN; i++) {
                    guess[i] = 0;
                }
                cursor = 0;

                /* Clear old guess row and draw new */
                clear_guess_row(4, cursor);
                draw_guess(4, guess, cursor, 1);

                /* Show feedback on row 4 right side */
                draw_feedback(4, ok, soft);

                /* Check lose */
                if(attempts == 0) {
                    game_over = 1;
                    won = 0;
                    _XL_ZAP_SOUND();
                    draw_game_over(0, secret);
                    clear_guess_row(4, cursor);
                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                }
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }
}