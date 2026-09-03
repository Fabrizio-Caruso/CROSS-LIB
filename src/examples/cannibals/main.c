#include "cross_lib.h"

uint8_t m_left;
uint8_t c_left;
uint8_t boat_left; /* 1 = left bank, 0 = right bank */
uint8_t m_boat;
uint8_t c_boat;
uint8_t game_over; /* 0 = playing, 1 = win, 2 = lose */

/* Game State */
void init_game(void)
{
    m_left = 3;
    c_left = 3;
    boat_left = 1; /* 1 = left bank, 0 = right bank */
    m_boat = 0;
    c_boat = 0;
    game_over = 0; /* 0 = playing, 1 = win, 2 = lose */
}

    
void draw_game(void) {
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 2, "MISSIONARIES AND CANNIBALS");
    _XL_PRINT(2, 4, "LEFT BANK --- BOAT --- RIGHT BANK");

    /* Draw Left Bank */
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(2, 6, "M:");
    _XL_PRINTD(4, 6, 1, m_left );
    _XL_PRINT(6, 6, " C:");
    _XL_PRINTD(8, 6, 1, c_left );

    /* Draw Boat Position */
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    if (boat_left) {
        _XL_PRINTD(16, 5, 1, m_boat);
        _XL_PRINTD(18, 5, 1, c_boat);
        _XL_PRINT( 15, 6, "[BOAT]");
    } else {
        _XL_PRINTD(26, 5, 1, m_boat);
        _XL_PRINTD(28, 5, 1, c_boat);
        _XL_PRINT( 25, 6, "[BOAT]");
    }

    /* Draw Right Bank */
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(35, 6, "M:");
    _XL_PRINTD(37, 6, 1, 3 - m_left);
    _XL_PRINT(39, 6, " C:");
    _XL_PRINTD(41, 6, 1, 3 - c_left);

    /* Controls/Status */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 10, "UP: ADD M | DOWN: ADD C | FIRE: GO");
    _XL_PRINT(2, 11, "LEFT: REM M | RIGHT: REM C");

    if (game_over == 1) {
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(15, 13, "YOU WIN!");
    } else if (game_over == 2) {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(15, 13, "GAME OVER - EATEN!");
    }
}

void check_rules(void) {
    uint8_t m_right = 3 - m_left;
    uint8_t c_right = 3 - c_left;

    /* Check if cannibals outnumber missionaries on either bank */
    if ((m_left > 0 && c_left > m_left) || (m_right > 0 && c_right > m_right)) {
        game_over = 2;
        _XL_EXPLOSION_SOUND();
    } else if (m_left == 0 && c_left == 0) {
        game_over = 1;
        _XL_PING_SOUND();
    }
}

int main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    
    for(;;)
    {
        init_game();
        while (game_over == 0) {
            draw_game();
            
            uint8_t input = _XL_INPUT();

            if (_XL_UP(input)) {
                if (m_boat < 2) m_boat++;
                _XL_TICK_SOUND();
                draw_game();
            } else if (_XL_DOWN(input)) {
                if (c_boat < 2) c_boat++;
                _XL_TICK_SOUND();
                draw_game();
            } else if (_XL_LEFT(input)) {
                if (m_boat > 0) m_boat--;
                _XL_TICK_SOUND();
                draw_game();
            } else if (_XL_RIGHT(input)) {
                if (c_boat > 0) c_boat--;
                _XL_TICK_SOUND();
                draw_game();
            } else if (_XL_FIRE(input)) {
                uint8_t total_on_boat = m_boat + c_boat;
                if (total_on_boat >= 1 && total_on_boat <= 2) {
                    if (boat_left) {
                        m_left -= m_boat;
                        c_left -= c_boat;
                        boat_left = 0;
                    } else {
                        m_left += m_boat;
                        c_left += c_boat;
                        boat_left = 1;
                    }
                    m_boat = 0;
                    c_boat = 0;
                    draw_game();    
                    _XL_SHOOT_SOUND();
                    check_rules();
                } else {
                    _XL_TOCK_SOUND(); /* Invalid boat load */
                }
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        draw_game();
        _XL_WAIT_FOR_INPUT();

    }
    return 0;
}
