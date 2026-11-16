#include "cross_lib.h"

int main(void)
{
    uint8_t cards[16];
    uint8_t card_state[16];
    uint8_t cursor_col;
    uint8_t cursor_row;
    uint8_t score;
    uint8_t matched_count;
    uint8_t first_card;
    uint8_t first_valid;
    uint8_t i;
    uint8_t cc, cr;
    uint8_t x, y;
    uint8_t input;
    uint8_t temp;
    uint8_t tmp;
    uint8_t card_idx;
    uint8_t s;
    uint8_t shape_tile[8];

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    shape_tile[0] = (uint8_t)_TILE_0;
    shape_tile[1] = (uint8_t)_TILE_1;
    shape_tile[2] = (uint8_t)_TILE_2;
    shape_tile[3] = (uint8_t)_TILE_3;
    shape_tile[4] = (uint8_t)_TILE_4;
    shape_tile[5] = (uint8_t)_TILE_5;
    shape_tile[6] = (uint8_t)_TILE_6;
    shape_tile[7] = (uint8_t)_TILE_7;

    while(1)
    {
        _XL_CLEAR_SCREEN();

        /* Build 8 pairs: 0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7 */
        for(i = 0; i < 16; i++)
        {
            cards[i] = (uint8_t)(i / 2);
        }

        /* Shuffle */
        for(i = 0; i < 16; i++)
        {
            tmp = (uint8_t)(_XL_RAND() % 16);
            temp = cards[i];
            cards[i] = cards[tmp];
            cards[tmp] = temp;
        }

        /* All cards face-down */
        for(i = 0; i < 16; i++)
        {
            card_state[i] = 0;
        }

        cursor_col = 0;
        cursor_row = 0;
        score = 0;
        matched_count = 0;
        first_card = 0;
        first_valid = 0;

        /* Header */
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 0, "SCORE");
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD(7, 0, 1, (uint16_t)score);

        /* Draw all 16 cards face-down: grid 4x4, card=2x3, gap=1 */
        for(cr = 0; cr < 4; cr++)
        {
            for(cc = 0; cc < 4; cc++)
            {
                x = (uint8_t)(cc * 3);
                y = (uint8_t)(2 + cr * 4);
                _XL_DRAW(x, y, _TILE_0, _XL_BLUE);
                _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_BLUE);
                _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_BLUE);
                _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_BLUE);
            }
        }

        /* Initial cursor highlight at (0,0) */
        x = 0;
        y = 2;
        _XL_DRAW(x, y, _TILE_0, _XL_YELLOW);
        _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_YELLOW);
        _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
        _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);
        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);

        /* Main game loop */
        while(matched_count < 8)
        {
            input = _XL_INPUT();

            /* --- Move cursor LEFT --- */
            if(_XL_LEFT(input))
            {
                if(cursor_col > 0)
                {
                    card_idx = (uint8_t)(cursor_row * 4 + cursor_col);
                    if(card_state[card_idx] == 0)
                    {
                        x = (uint8_t)(cursor_col * 3);
                        y = (uint8_t)(2 + cursor_row * 4);
                        _XL_DRAW(x, y, _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_BLUE);
                        _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                        _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_BLUE);
                    }
                    cursor_col--;
                    card_idx = (uint8_t)(cursor_row * 4 + cursor_col);
                    if(card_state[card_idx] == 0)
                    {
                        x = (uint8_t)(cursor_col * 3);
                        y = (uint8_t)(2 + cursor_row * 4);
                        _XL_DRAW(x, y, _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_YELLOW);
                        _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
                        _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);
                    }
                }
            }

            /* --- Move cursor RIGHT --- */
            if(_XL_RIGHT(input))
            {
                if(cursor_col < 3)
                {
                    card_idx = (uint8_t)(cursor_row * 4 + cursor_col);
                    if(card_state[card_idx] == 0)
                    {
                        x = (uint8_t)(cursor_col * 3);
                        y = (uint8_t)(2 + cursor_row * 4);
                        _XL_DRAW(x, y, _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_BLUE);
                        _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                        _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_BLUE);
                    }
                    cursor_col++;
                    card_idx = (uint8_t)(cursor_row * 4 + cursor_col);
                    if(card_state[card_idx] == 0)
                    {
                        x = (uint8_t)(cursor_col * 3);
                        y = (uint8_t)(2 + cursor_row * 4);
                        _XL_DRAW(x, y, _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_YELLOW);
                        _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
                        _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);
                    }
                }
            }

            /* --- Move cursor UP --- */
            if(_XL_UP(input))
            {
                if(cursor_row > 0)
                {
                    card_idx = (uint8_t)(cursor_row * 4 + cursor_col);
                    if(card_state[card_idx] == 0)
                    {
                        x = (uint8_t)(cursor_col * 3);
                        y = (uint8_t)(2 + cursor_row * 4);
                        _XL_DRAW(x, y, _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_BLUE);
                        _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                        _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_BLUE);
                    }
                    cursor_row--;
                    card_idx = (uint8_t)(cursor_row * 4 + cursor_col);
                    if(card_state[card_idx] == 0)
                    {
                        x = (uint8_t)(cursor_col * 3);
                        y = (uint8_t)(2 + cursor_row * 4);
                        _XL_DRAW(x, y, _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_YELLOW);
                        _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
                        _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);
                    }
                }
            }

            /* --- Move cursor DOWN --- */
            if(_XL_DOWN(input))
            {
                if(cursor_row < 3)
                {
                    card_idx = (uint8_t)(cursor_row * 4 + cursor_col);
                    if(card_state[card_idx] == 0)
                    {
                        x = (uint8_t)(cursor_col * 3);
                        y = (uint8_t)(2 + cursor_row * 4);
                        _XL_DRAW(x, y, _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_BLUE);
                        _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                        _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_BLUE);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_BLUE);
                    }
                    cursor_row++;
                    card_idx = (uint8_t)(cursor_row * 4 + cursor_col);
                    if(card_state[card_idx] == 0)
                    {
                        x = (uint8_t)(cursor_col * 3);
                        y = (uint8_t)(2 + cursor_row * 4);
                        _XL_DRAW(x, y, _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_YELLOW);
                        _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
                        _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);
                        _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);
                    }
                }
            }

            /* --- FIRE: select a card --- */
            if(_XL_FIRE(input))
            {
                card_idx = (uint8_t)(cursor_row * 4 + cursor_col);
                if(card_state[card_idx] == 0)
                {
                    /* Reveal card: draw shape in white */
                    card_state[card_idx] = 1;
                    s = cards[card_idx];
                    x = (uint8_t)(cursor_col * 3);
                    y = (uint8_t)(2 + cursor_row * 4);
                    _XL_DRAW(x, y, shape_tile[s], _XL_WHITE);
                    _XL_DRAW((uint8_t)(x + 1), y, shape_tile[s], _XL_WHITE);
                    _XL_DRAW(x, (uint8_t)(y + 1), shape_tile[s], _XL_WHITE);
                    _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), shape_tile[s], _XL_WHITE);
                    _XL_DRAW(x, (uint8_t)(y + 2), shape_tile[s], _XL_WHITE);
                    _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), shape_tile[s], _XL_WHITE);
                    _XL_PING_SOUND();

                    if(!first_valid)
                    {
                        first_card = card_idx;
                        first_valid = 1;
                    }
                    else
                    {
                        /* Second card: compare with first */
                        if(cards[first_card] == cards[card_idx])
                        {
                            /* MATCH: turn both green */
                            _XL_SHOOT_SOUND();
                            card_state[first_card] = 2;
                            card_state[card_idx] = 2;
                            matched_count++;
                            score++;

                            /* First card to green */
                            cc = (uint8_t)(first_card % 4);
                            cr = (uint8_t)(first_card / 4);
                            x = (uint8_t)(cc * 3);
                            y = (uint8_t)(2 + cr * 4);
                            s = cards[first_card];
                            _XL_DRAW(x, y, shape_tile[s], _XL_GREEN);
                            _XL_DRAW((uint8_t)(x + 1), y, shape_tile[s], _XL_GREEN);
                            _XL_DRAW(x, (uint8_t)(y + 1), shape_tile[s], _XL_GREEN);
                            _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), shape_tile[s], _XL_GREEN);
                            _XL_DRAW(x, (uint8_t)(y + 2), shape_tile[s], _XL_GREEN);
                            _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), shape_tile[s], _XL_GREEN);

                            /* Second card to green */
                            cc = (uint8_t)(card_idx % 4);
                            cr = (uint8_t)(card_idx / 4);
                            x = (uint8_t)(cc * 3);
                            y = (uint8_t)(2 + cr * 4);
                            s = cards[card_idx];
                            _XL_DRAW(x, y, shape_tile[s], _XL_GREEN);
                            _XL_DRAW((uint8_t)(x + 1), y, shape_tile[s], _XL_GREEN);
                            _XL_DRAW(x, (uint8_t)(y + 1), shape_tile[s], _XL_GREEN);
                            _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), shape_tile[s], _XL_GREEN);
                            _XL_DRAW(x, (uint8_t)(y + 2), shape_tile[s], _XL_GREEN);
                            _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), shape_tile[s], _XL_GREEN);

                            /* Update score display */
                            _XL_SET_TEXT_COLOR(_XL_YELLOW);
                            _XL_PRINTD(7, 0, 1, (uint16_t)score);
                        }
                        else
                        {
                            /* NO MATCH: wait, then flip both back */
                            _XL_TOCK_SOUND();
                            _XL_SLOW_DOWN((uint16_t)(_XL_SLOW_DOWN_FACTOR * 3));
                            _XL_SLOW_DOWN((uint16_t)(_XL_SLOW_DOWN_FACTOR * 3));
                            _XL_SLOW_DOWN((uint16_t)(_XL_SLOW_DOWN_FACTOR * 3));
                            _XL_SLOW_DOWN((uint16_t)(_XL_SLOW_DOWN_FACTOR * 3));

                            /* Flip first card back (cursor is on second card) */
                            card_state[first_card] = 0;
                            cc = (uint8_t)(first_card % 4);
                            cr = (uint8_t)(first_card / 4);
                            x = (uint8_t)(cc * 3);
                            y = (uint8_t)(2 + cr * 4);
                            _XL_DRAW(x, y, _TILE_0, _XL_BLUE);
                            _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_BLUE);
                            _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                            _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_BLUE);
                            _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_BLUE);
                            _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_BLUE);

                            /* Flip second card back (cursor is here) */
                            card_state[card_idx] = 0;
                            cc = (uint8_t)(card_idx % 4);
                            cr = (uint8_t)(card_idx / 4);
                            x = (uint8_t)(cc * 3);
                            y = (uint8_t)(2 + cr * 4);
                            _XL_DRAW(x, y, _TILE_0, _XL_YELLOW);
                            _XL_DRAW((uint8_t)(x + 1), y, _TILE_0, _XL_YELLOW);
                            _XL_DRAW(x, (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
                            _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 1), _TILE_0, _XL_YELLOW);
                            _XL_DRAW(x, (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);
                            _XL_DRAW((uint8_t)(x + 1), (uint8_t)(y + 2), _TILE_0, _XL_YELLOW);
                        }
                        first_valid = 0;
                    }
                }
            }
        }

        /* Game over: show win message, wait, then restart */
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(0, 0, "YOU WIN");
        _XL_WAIT_FOR_INPUT();
    }
}