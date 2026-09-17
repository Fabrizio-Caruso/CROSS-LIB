#include "cross_lib.h"

#define CODE_LEN 4
#define NUM_COLORS 6
#define MAX_ATTEMPTS 12

#define TITLE_ROW      0
#define INSTRUCT_ROW   1
#define PALETTE_ROW    2
#define ATTEMPTS_ROW   3
#define FIRST_PREV_ROW 4
#define CURR_ROW       12
#define FEEDBACK_ROW   13
#define GAMEOVER_ROW   14

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
    _XL_PRINT(0, TITLE_ROW, "MASTERMIND");
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(0, INSTRUCT_ROW, "LR=POS UD=CLR FIRE=GO");
    for(i = 0; i < NUM_COLORS; i++) {
        _XL_DRAW(i, PALETTE_ROW, _TILE_0, color_to_xl(i));
    }
}

static void draw_attempts(uint8_t n)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, ATTEMPTS_ROW, "ATTEMPTS: ");
    _XL_PRINTD(9, ATTEMPTS_ROW, 1, n);
}

static void draw_guess_tiles(uint8_t y, uint8_t *guess, uint8_t cursor, uint8_t show_cursor)
{
    uint8_t i;
    for(i = 0; i < CODE_LEN; i++) {
        _XL_DRAW(i * 2, y, _TILE_0, color_to_xl(guess[i]));
        if(show_cursor && i == cursor) {
            _XL_DRAW(i * 2 + 1, y, _TILE_1, _XL_WHITE);
        }
    }
}

static void clear_guess_tiles(uint8_t y, uint8_t cursor, uint8_t show_cursor)
{
    uint8_t i;
    for(i = 0; i < CODE_LEN; i++) {
        _XL_DELETE(i * 2, y);
        if(show_cursor && i == cursor) {
            _XL_DELETE(i * 2 + 1, y);
        }
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

static void clear_feedback(uint8_t y)
{
    uint8_t i;
    for(i = 10; i < 22; i++) {
        _XL_DELETE(i, y);
    }
}

static void clear_prev_row(uint8_t y)
{
    uint8_t i;
    for(i = 0; i < 8; i++) {
        _XL_DELETE(i, y);
    }
    for(i = 10; i < 22; i++) {
        _XL_DELETE(i, y);
    }
}

static void draw_prev_row(uint8_t y, uint8_t *g, uint8_t ok, uint8_t soft)
{
    uint8_t i;
    for(i = 0; i < CODE_LEN; i++) {
        _XL_DRAW(i * 2, y, _TILE_0, color_to_xl(g[i]));
    }
    draw_feedback(y, ok, soft);
}

static void clear_gameover_area(void)
{
    uint8_t i;
    for(i = 0; i < 20; i++) {
        _XL_DELETE(i, GAMEOVER_ROW);
        _XL_DELETE(i, GAMEOVER_ROW + 1);
        _XL_DELETE(i, GAMEOVER_ROW + 2);
    }
    for(i = 0; i < CODE_LEN; i++) {
        _XL_DELETE(6 + i * 2, GAMEOVER_ROW + 1);
    }
}

static void draw_gameover(uint8_t won, uint8_t *secret)
{
    uint8_t i;
    _XL_SET_TEXT_COLOR(won ? _XL_GREEN : _XL_RED);
    if(won) {
        _XL_PRINT(0, GAMEOVER_ROW, "YOU WIN!");
    } else {
        _XL_PRINT(0, GAMEOVER_ROW, "GAME OVER");
    }
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, GAMEOVER_ROW + 1, "CODE:");
    for(i = 0; i < CODE_LEN; i++) {
        _XL_DRAW(6 + i * 2, GAMEOVER_ROW + 1, _TILE_0, color_to_xl(secret[i]));
    }
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(0, GAMEOVER_ROW + 2, "FIRE=RESTART");
}

static void evaluate(uint8_t *secret, uint8_t *guess, uint8_t *p_ok, uint8_t *p_soft)
{
    uint8_t i;
    uint8_t ok = 0;
    uint8_t soft = 0;
    uint8_t s_count[NUM_COLORS];
    uint8_t g_count[NUM_COLORS];

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

int main(void)
{
    uint8_t secret[CODE_LEN];
    uint8_t guess[CODE_LEN];
    uint8_t cursor;
    uint8_t attempts;
    uint8_t num_prev;
    uint8_t prev_guesses[MAX_ATTEMPTS][CODE_LEN];
    uint8_t prev_ok[MAX_ATTEMPTS];
    uint8_t prev_soft[MAX_ATTEMPTS];
    uint8_t input;
    uint8_t ok;
    uint8_t soft;
    uint8_t i;
    uint8_t game_over;
    uint8_t won;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while(1) {
        generate_code(secret);
        for(i = 0; i < CODE_LEN; i++) {
            guess[i] = 0;
        }
        cursor = 0;
        attempts = MAX_ATTEMPTS;
        num_prev = 0;
        game_over = 0;
        won = 0;

        _XL_CLEAR_SCREEN();
        draw_static();
        draw_attempts(attempts);
        draw_guess_tiles(CURR_ROW, guess, cursor, 1);

        while(1) {
            input = _XL_INPUT();

            if(game_over) {
                if(_XL_FIRE(input)) {
                    break;
                }
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                continue;
            }

            if(!input) {
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                continue;
            }

            if(_XL_LEFT(input)) {
                if(cursor > 0) {
                    _XL_DELETE(cursor * 2 + 1, CURR_ROW);
                    cursor--;
                    _XL_DRAW(cursor * 2 + 1, CURR_ROW, _TILE_1, _XL_WHITE);
                    _XL_TICK_SOUND();
                }
            }

            if(_XL_RIGHT(input)) {
                if(cursor < CODE_LEN - 1) {
                    _XL_DELETE(cursor * 2 + 1, CURR_ROW);
                    cursor++;
                    _XL_DRAW(cursor * 2 + 1, CURR_ROW, _TILE_1, _XL_WHITE);
                    _XL_TICK_SOUND();
                }
            }

            if(_XL_UP(input)) {
                guess[cursor] = (guess[cursor] + 1) % NUM_COLORS;
                _XL_DELETE(cursor * 2, CURR_ROW);
                _XL_DRAW(cursor * 2, CURR_ROW, _TILE_0, color_to_xl(guess[cursor]));
                _XL_PING_SOUND();
            }

            if(_XL_DOWN(input)) {
                if(guess[cursor] == 0) {
                    guess[cursor] = NUM_COLORS - 1;
                } else {
                    guess[cursor]--;
                }
                _XL_DELETE(cursor * 2, CURR_ROW);
                _XL_DRAW(cursor * 2, CURR_ROW, _TILE_0, color_to_xl(guess[cursor]));
                _XL_TOCK_SOUND();
            }

            if(_XL_FIRE(input)) {
                evaluate(secret, guess, &ok, &soft);
                _XL_SHOOT_SOUND();

                for(i = 0; i < CODE_LEN; i++) {
                    prev_guesses[num_prev][i] = guess[i];
                }
                prev_ok[num_prev] = ok;
                prev_soft[num_prev] = soft;
                num_prev++;

                draw_prev_row(FIRST_PREV_ROW + num_prev - 1,
                              prev_guesses[num_prev - 1], ok, soft);

                if(ok == CODE_LEN) {
                    game_over = 1;
                    won = 1;
                    _XL_EXPLOSION_SOUND();
                    clear_guess_tiles(CURR_ROW, cursor, 1);
                    draw_gameover(1, secret);
                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                    continue;
                }

                attempts--;
                _XL_DELETE(9, ATTEMPTS_ROW);
                _XL_PRINTD(9, ATTEMPTS_ROW, 1, attempts);

                clear_guess_tiles(CURR_ROW, cursor, 1);
                for(i = 0; i < CODE_LEN; i++) {
                    guess[i] = 0;
                }
                cursor = 0;
                draw_guess_tiles(CURR_ROW, guess, cursor, 1);

                draw_feedback(FEEDBACK_ROW, ok, soft);

                if(attempts == 0) {
                    game_over = 1;
                    won = 0;
                    _XL_ZAP_SOUND();
                    clear_feedback(FEEDBACK_ROW);
                    draw_gameover(0, secret);
                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                }
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }
}