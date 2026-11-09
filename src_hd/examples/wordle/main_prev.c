#include "cross_lib.h"
#include "wordle_dict.h"

/* wordle_dict.h is expected to provide:
   const char *wordle_get_word(uint16_t index);
   uint16_t wordle_word_count(void);
   uint8_t  wordle_is_valid(const char *word);
*/

#define GRID_COLS   5
#define GRID_ROWS   6
#define GRID_X      2
#define GRID_Y      2
#define TILE_SP     2
#define TITLE_Y     0
#define MSG_Y       15
#define TRIES_X     14
#define TRIES_Y     2

#define STATE_PLAY  0
#define STATE_WON   1
#define STATE_LOST  2

#define T_GREEN     0
#define T_YELLOW    1
#define T_GRAY      2

// static const char *const wordle_words[] =
// {
    // "WORDLE",
    // "APPLE",
    // "BRAVE",
    // "CRAFT",
    // "DREAM",
    // /* ... your existing words ... */
// };

static const uint16_t wordle_words_count =
    (uint16_t)(sizeof(wordle_words) / sizeof(wordle_words[0]));

extern const char *const wordle_words[];
// extern const uint16_t wordle_words_count;


static uint8_t wordle_word_eq(const char *a, const char *b)
{
    uint8_t i;
    for (i = 0; i < 5; i++)
    {
        if (a[i] != b[i])
            return 0;
    }
    return 1;//(a[5] == '\0' && b[5] == '\0');
}

const char *wordle_get_word(uint16_t index)
{
    if (index >= wordle_words_count)
        return 0;
    return wordle_words[index];
}

uint16_t wordle_word_count(void)
{
    return wordle_words_count;
}

uint8_t wordle_is_valid(const char *word)
{
    uint16_t i;
    if (word == 0)
        return 0;
    // _XL_PRINT(0,14,word);
    
    for (i = 0; i < wordle_words_count; i++)
    {
        // _XL_PRINT(0,15,wordle_words[i]);
        // _XL_WAIT_FOR_INPUT();
        if (wordle_word_eq(word, wordle_words[i]))
            return 1;
    }
    return 0;
}


static uint8_t c2t(uint8_t ch)
{
    return (uint8_t)(ch - 'A' + 1);
}

static uint8_t t2c(uint8_t t)
{
    return (uint8_t)(t - 1 + 'A');
}

static void draw_cell(uint8_t row, uint8_t col, uint8_t tile_id, uint8_t color)
{
    uint8_t x = (uint8_t)(GRID_X + col * TILE_SP);
    uint8_t y = (uint8_t)(GRID_Y + row * TILE_SP);
    _XL_DRAW(x, y, tile_id, color);
}

static void draw_empty_cell(uint8_t row, uint8_t col)
{
    draw_cell(row, col, _TILE_0, _XL_BLUE);
}

static void draw_letter_cell(uint8_t row, uint8_t col, uint8_t letter, uint8_t color)
{
    // draw_cell(row, col, c2t(letter), color);
    uint8_t x = (uint8_t)(GRID_X + col * TILE_SP);
    uint8_t y = (uint8_t)(GRID_Y + row * TILE_SP);
    _XL_SET_TEXT_COLOR(color);
    // _XL_CHAR(x,y,c2t(letter));
    // _XL_DRAW(x, y, c2t(letter), color);
    _XL_CHAR(x, y, 'A'+letter-1);

}

static void draw_all_empty(void)
{
    uint8_t r, c;
    for (r = 0; r < GRID_ROWS; r++)
    {
        for (c = 0; c < GRID_COLS; c++)
        {
            draw_empty_cell(r, c);
        }
    }
}

static uint8_t evaluate(const uint8_t *guess, const uint8_t *target, uint8_t *res)
{
    uint8_t i;
    uint8_t cnt[26];
    uint8_t is_match[5];
    uint8_t all_green;

    for (i = 0; i < 26; i++)
    {
        cnt[i] = 0;
    }
    for (i = 0; i < 5; i++)
    {
        cnt[target[i] - 'A']++;
    }

    for (i = 0; i < 5; i++)
    {
        is_match[i] = 0;
    }

    for (i = 0; i < 5; i++)
    {
        if (guess[i] == target[i])
        {
            res[i] = T_GREEN;
            is_match[i] = 1;
            cnt[target[i] - 'A']--;
        }
    }

    for (i = 0; i < 5; i++)
    {
        if (!is_match[i])
        {
            if (cnt[guess[i] - 'A'] > 0)
            {
                res[i] = T_YELLOW;
                cnt[guess[i] - 'A']--;
            }
            else
            {
                res[i] = T_GRAY;
            }
        }
    }

    all_green = 1;
    for (i = 0; i < 5; i++)
    {
        if (res[i] != T_GREEN)
        {
            all_green = 0;
            break;
        }
    }
    return all_green;
}

static void draw_result_row(uint8_t row, const uint8_t *guess, const uint8_t *res)
{
    uint8_t c;
    for (c = 0; c < GRID_COLS; c++)
    {
        if (res[c] == T_GREEN)
        {
            draw_letter_cell(row, c, guess[c], _XL_GREEN);
        }
        else if (res[c] == T_YELLOW)
        {
            draw_letter_cell(row, c, guess[c], _XL_YELLOW);
        }
        else
        {
            draw_letter_cell(row, c, guess[c], _XL_CYAN);
        }
    }
}

static void draw_current_row(uint8_t row, const uint8_t *guess, uint8_t cursor)
{
    uint8_t c;
    for (c = 0; c < GRID_COLS; c++)
    {
        if (guess[c] != 0)
        {
            if (c == cursor)
            {
                draw_letter_cell(row, c, guess[c], _XL_MAGENTA);
            }
            else
            {
                draw_letter_cell(row, c, guess[c], _XL_WHITE);
            }
        }
        else
        {
            if (c == cursor)
            {
                draw_cell(row, c, _TILE_0, _XL_MAGENTA);
            }
            else
            {
                draw_cell(row, c, _TILE_0, _XL_WHITE);
            }
        }
    }
}

static void draw_title(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(5, TITLE_Y, "WORDLE");
}

static void draw_tries(uint8_t n)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(TRIES_X, TRIES_Y, "TRIES");
    _XL_PRINTD(TRIES_X, (uint8_t)(TRIES_Y + 1), 1, (uint16_t)n);
}

static void show_msg(const char *msg)
{
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(2, MSG_Y, msg);
}

static void clear_msg(void)
{
    _XL_SET_TEXT_COLOR(_XL_BLUE);
    _XL_PRINT(2, MSG_Y, "                    ");
}

static void game_over_screen(uint8_t won, const uint8_t *target)
{
    uint8_t i;
    char word[6];

    for (i = 0; i < 5; i++)
    {
        word[i] = (char)target[i];
    }
    word[5] = '\0';

    if (won)
    {
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(3, MSG_Y, "GREAT! YOU WON");
    }
    else
    {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(3, MSG_Y, "LOST! WORD WAS");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(3, (uint8_t)(MSG_Y + 1), word);
    }
}

static void new_game(uint8_t *target, uint8_t *guess, uint8_t *attempts,
                      uint8_t *cursor, uint8_t *state)
{
    uint16_t idx;
    const char *w;
    uint8_t i;

    idx = (uint16_t)(_XL_RAND() % wordle_word_count());
    w = wordle_get_word(idx);
    for (i = 0; i < 5; i++)
    {
        target[i] = (uint8_t)w[i];
    }

    for (i = 0; i < 5; i++)
    {
        guess[i] = 0;
    }

    *attempts = 0;
    *cursor = 0;
    *state = STATE_PLAY;

    _XL_CLEAR_SCREEN();
    draw_title();
    draw_tries(0);
    draw_all_empty();
    draw_current_row(0, guess, 0);
    clear_msg();
}

int main(void)
{
    uint8_t target[5];
    uint8_t guess[5];
    uint8_t attempts;
    uint8_t cursor;
    uint8_t state;
    uint8_t input;
    uint8_t res[5];
    uint8_t won;
    uint8_t all_set;
    uint8_t i;
    char word[6];
    uint8_t prev_cursor;
    uint8_t prev_letter;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    new_game(target, guess, &attempts, &cursor, &state);

    for (;;)
    {
        if (state == STATE_PLAY)
        {
            input = _XL_INPUT();

            if (_XL_LEFT(input))
            {
                prev_cursor = cursor;
                if (cursor > 0)
                {
                    cursor--;
                }
                _XL_TICK_SOUND();
                draw_current_row(attempts, guess, cursor);
            }
            else if (_XL_RIGHT(input))
            {
                prev_cursor = cursor;
                if (cursor < GRID_COLS - 1)
                {
                    cursor++;
                }
                _XL_TICK_SOUND();
                draw_current_row(attempts, guess, cursor);
            }
            else if (_XL_UP(input))
            {
                prev_letter = guess[cursor];
                if (guess[cursor] == 0)
                {
                    guess[cursor] = 'A';
                }
                else if (guess[cursor] < 'Z')
                {
                    guess[cursor]++;
                }
                else
                {
                    guess[cursor] = 'A';
                }
                _XL_PING_SOUND();
                draw_current_row(attempts, guess, cursor);
            }
            else if (_XL_DOWN(input))
            {
                prev_letter = guess[cursor];
                if (guess[cursor] == 0)
                {
                    guess[cursor] = 'Z';
                }
                else if (guess[cursor] > 'A')
                {
                    guess[cursor]--;
                }
                else
                {
                    guess[cursor] = 'Z';
                }
                _XL_PING_SOUND();
                draw_current_row(attempts, guess, cursor);
            }
            else if (_XL_FIRE(input))
            {
                all_set = 1;
                for (i = 0; i < 5; i++)
                {
                    if (guess[i] == 0)
                    {
                        all_set = 0;
                        break;
                    }
                }

                if (all_set)
                {
                    for (i = 0; i < 5; i++)
                    {
                        word[i] = (char)guess[i];
                    }
                    // word[5] = '\0';

                    if (wordle_is_valid(word))
                    {
                        won = evaluate(guess, target, res);
                        draw_result_row(attempts, guess, res);
                        _XL_TOCK_SOUND();

                        if (won)
                        {
                            state = STATE_WON;
                            game_over_screen(1, target);
                            _XL_SHOOT_SOUND();
                        }
                        else if (attempts >= GRID_ROWS - 1)
                        {
                            state = STATE_LOST;
                            game_over_screen(0, target);
                            _XL_EXPLOSION_SOUND();
                        }
                        else
                        {
                            attempts++;
                            draw_tries(attempts);
                            draw_current_row(attempts, guess, 0);
                            for (i = 0; i < 5; i++)
                            {
                                guess[i] = 0;
                            }
                            cursor = 0;
                        }
                    }
                    else
                    {
                        show_msg("NOT A WORD");
                        _XL_ZAP_SOUND();
                    }
                }
                else
                {
                    show_msg("NEED 5 LETTERS");
                    _XL_ZAP_SOUND();
                }
            }
        }
        else
        {
            /* Game over - wait for any key to restart */
            input = _XL_INPUT();
            if (_XL_FIRE(input))
            {
                _XL_SLEEP(1);
                new_game(target, guess, &attempts, &cursor, &state);
            }
            else
            {
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}