#include "cross_lib.h"

#define AWARI_PITS 12
#define AWARI_SEEDS_PER_PIT 3
#define AWARI_WIN_STORE 12

static uint8_t g_board[AWARI_PITS];
static uint8_t g_store[2];
static uint8_t g_player;
static uint8_t g_selected_pit;
static uint8_t g_game_over;
static uint8_t g_winner;

static uint8_t g_animating;
static uint8_t g_move_player;
static uint8_t g_move_pit;
static uint8_t g_move_hand;
static uint8_t g_move_pos;

static uint8_t g_prev_board[AWARI_PITS];
static uint8_t g_prev_store[2];
static uint8_t g_prev_player;
static uint8_t g_prev_selected_pit;
static uint8_t g_prev_game_over;
static uint8_t g_prev_winner;

static uint8_t g_prev_tile_color[16];

static char msg_title[] = "AWARI";
static char msg_p1_move[] = "P1 MOVE";
static char msg_cpu_move[] = "CPU MOVE";
static char msg_p1_wins[] = "P1 WINS";
static char msg_cpu_wins[] = "CPU WINS";
static char msg_draw[] = "DRAW";
static char spaces3[] = "   ";

static void awari_reset(void);
static void awari_clear_count_field(uint8_t x, uint8_t y);
static void awari_print_int_at(uint8_t x, uint8_t y, uint16_t value);
static void awari_draw_position_tile(uint8_t pos, uint8_t color_id);
static void awari_update_board_graphics(void);
static void awari_print_score_line(uint8_t y, char *label, uint16_t value);
static void awari_print_status(char *text);
static void awari_wait_for_restart(void);
static uint8_t awari_next_position(uint8_t pos);
static void awari_start_move(uint8_t player, uint8_t pit);
static void awari_step_animation(void);
static void awari_finish_move(void);
static void awari_check_game_over(void);
static void awari_begin_turn(void);
static uint8_t awari_player_has_seeds(uint8_t player);
static short awari_ai_score(uint8_t player, uint8_t pit);
static uint8_t awari_ai_choose_pit(uint8_t player);

int main(void)
{
    uint8_t input;
    uint8_t pit;
    uint8_t col;
    uint8_t ai_delay;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    awari_reset();
    _XL_CLEAR_SCREEN();
    awari_update_board_graphics();

    for (;;)
    {
        if (g_animating)
        {
            awari_step_animation();
        }
        else if (!g_game_over)
        {
            pit = 255;

            if (g_player == 0)
            {
                input = _XL_INPUT();

                col = g_selected_pit;
                if (col >= AWARI_PITS / 2)
                    col = (uint8_t)(13 - col);

                if (_XL_LEFT(input))
                {
                    if (col == 0)
                        col = 6;
                    else
                        col--;
                }

                if (_XL_RIGHT(input))
                {
                    if (col == 6)
                        col = 0;
                    else
                        col++;
                }

                if (_XL_UP(input) || _XL_DOWN(input))
                {
                    if (g_selected_pit < AWARI_PITS / 2)
                        g_selected_pit = (uint8_t)(13 - g_selected_pit);
                    else
                        g_selected_pit = col;
                }
                else
                {
                    if (g_selected_pit < AWARI_PITS / 2)
                        g_selected_pit = col;
                    else
                        g_selected_pit = (uint8_t)(13 - col);
                }

                if (_XL_FIRE(input))
                {
                    if ((g_player == 0 && g_selected_pit < AWARI_PITS / 2) ||
                        (g_player == 1 && g_selected_pit >= AWARI_PITS / 2))
                    {
                        if (g_board[g_selected_pit] > 0)
                        {
                            awari_start_move(g_player, g_selected_pit);
                            _XL_PING_SOUND();
                        }
                    }
                }
            }
            else
            {
                pit = awari_ai_choose_pit(1);

                if (pit != 255)
                {
                    g_selected_pit = pit;
                    awari_start_move(1, pit);
                    _XL_TOCK_SOUND();
                }
            }
        }
        else
        {
            awari_wait_for_restart();
        }

        awari_update_board_graphics();

        ai_delay = (uint8_t)(_XL_SLOW_DOWN_FACTOR / 4);
        if (ai_delay == 0)
            ai_delay = 1;
        _XL_SLOW_DOWN(ai_delay);
    }

    return 0;
}

static void awari_reset(void)
{
    short i;

    for (i = 0; i < AWARI_PITS; i++)
    {
        g_board[i] = AWARI_SEEDS_PER_PIT;
    }

    g_store[0] = 0;
    g_store[1] = 0;
    g_player = 0;
    g_selected_pit = 3;
    g_game_over = 0;
    g_winner = 255;

    g_animating = 0;
    g_move_player = 0;
    g_move_pit = 0;
    g_move_hand = 0;
    g_move_pos = 0;

    for (i = 0; i < AWARI_PITS; i++)
    {
        g_prev_board[i] = 255;
    }

    g_prev_store[0] = 255;
    g_prev_store[1] = 255;
    g_prev_player = 255;
    g_prev_selected_pit = 255;
    g_prev_game_over = 255;
    g_prev_winner = 255;

    for (i = 0; i < 16; i++)
    {
        g_prev_tile_color[i] = 255;
    }

    awari_begin_turn();
}

static void awari_clear_count_field(uint8_t x, uint8_t y)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(x, y, spaces3);
}

static void awari_print_int_at(uint8_t x, uint8_t y, uint16_t value)
{
    if (value < 10)
    {
        _XL_PRINTD((uint8_t)(x + 1), y, 1, value);
    }
    else
    {
        _XL_PRINTD(x, y, 1, value);
    }
}

static void awari_draw_position_tile(uint8_t pos, uint8_t color_id)
{
    uint8_t x;
    uint8_t y;

    if (pos < AWARI_PITS / 2)
    {
        /* Player 1 pits: bottom row, left to right. */
        x = (uint8_t)(4 + pos * 3);
        y = 10;
    }
    else if (pos < AWARI_PITS)
    {
        /* CPU pits: top row, right to left. */
        x = (uint8_t)(22 - ((short)pos - (AWARI_PITS / 2)) * 3);
        y = 5;
    }
    else if (pos == AWARI_PITS)
    {
        /* Player 1 store: right side. */
        x = 26;
        y = 7;
    }
    else
    {
        /* CPU store: left side. */
        x = 1;
        y = 7;
    }

    _XL_DRAW(x, y, _TILE_1, color_id);
}

static void awari_update_board_graphics(void)
{
    short i;
    uint8_t pos;
    uint8_t color_id;
    uint8_t count_x;
    uint8_t count_y;
    char *status_text;

    _XL_SET_TEXT_COLOR(_XL_WHITE);

    for (i = 0; i < AWARI_PITS; i++)
    {
        pos = (uint8_t)i;

        color_id = _XL_CYAN;

        if (!g_game_over)
        {
            if ((g_player == 0 && pos < AWARI_PITS / 2) ||
                (g_player == 1 && pos >= AWARI_PITS / 2))
            {
                color_id = _XL_GREEN;
            }

            if (pos == g_selected_pit)
            {
                color_id = _XL_YELLOW;
            }
        }

        if (g_prev_tile_color[pos] != color_id)
        {
            awari_draw_position_tile(pos, color_id);
            g_prev_tile_color[pos] = color_id;
        }

        if (g_prev_board[pos] != g_board[pos])
        {
            if (pos < AWARI_PITS / 2)
            {
                count_x = (uint8_t)(4 + pos * 3);
                count_y = 11;
            }
            else
            {
                count_x = (uint8_t)(22 - ((short)pos - (AWARI_PITS / 2)) * 3);
                count_y = 4;
            }

            awari_clear_count_field(count_x, count_y);
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            awari_print_int_at(count_x, count_y, g_board[pos]);
        }

        g_prev_board[pos] = g_board[pos];
    }

    color_id = _XL_MAGENTA;
    if (!g_game_over && g_player == 0)
        color_id = _XL_GREEN;

    if (g_prev_tile_color[AWARI_PITS] != color_id)
    {
        awari_draw_position_tile(AWARI_PITS, color_id);
        g_prev_tile_color[AWARI_PITS] = color_id;
    }

    if (g_prev_store[0] != g_store[0])
    {
        awari_print_score_line(14, "P1", g_store[0]);

        awari_clear_count_field(26, 8);
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        awari_print_int_at(26, 8, g_store[0]);
    }

    color_id = _XL_MAGENTA;
    if (!g_game_over && g_player == 1)
        color_id = _XL_GREEN;

    if (g_prev_tile_color[AWARI_PITS + 1] != color_id)
    {
        awari_draw_position_tile(AWARI_PITS + 1, color_id);
        g_prev_tile_color[AWARI_PITS + 1] = color_id;
    }

    if (g_prev_store[1] != g_store[1])
    {
        awari_print_score_line(3, "CPU", g_store[1]);

        awari_clear_count_field(1, 8);
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        awari_print_int_at(1, 8, g_store[1]);
    }

    g_prev_store[0] = g_store[0];
    g_prev_store[1] = g_store[1];

    if (g_prev_player != g_player ||
        g_prev_game_over != g_game_over ||
        g_prev_winner != g_winner)
    {
        status_text = msg_title;

        if (g_game_over)
        {
            if (g_winner == 0)
                status_text = msg_p1_wins;
            else if (g_winner == 1)
                status_text = msg_cpu_wins;
            else
                status_text = msg_draw;
        }
        else if (g_player == 0)
        {
            status_text = msg_p1_move;
        }
        else
        {
            status_text = msg_cpu_move;
        }

        awari_print_status(status_text);
    }

    g_prev_player = g_player;
    g_prev_game_over = g_game_over;
    g_prev_selected_pit = g_selected_pit;
    g_prev_winner = g_winner;
}

static void awari_print_score_line(uint8_t y, char *label, uint16_t value)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(1, y, label);

    awari_clear_count_field(6, y);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    awari_print_int_at(6, y, value);
}

static void awari_print_status(char *text)
{
    uint8_t x;
    short sx;

    _XL_SET_TEXT_COLOR(_XL_YELLOW);

    sx = (short)(XSize - 10) / 2;
    if (sx < 0)
        sx = 0;

    x = (uint8_t)sx;
    _XL_PRINT(x, 1, text);
}

static void awari_wait_for_restart(void)
{
    _XL_WAIT_FOR_INPUT();
    _XL_CLEAR_SCREEN();
    awari_reset();
}

static uint8_t awari_next_position(uint8_t pos)
{
    /*
       Counter-clockwise order on screen:

       Player 1 bottom pits left-to-right:   0 -> 1 -> ... -> 6
       Player 1 store right side:           14
       CPU top pits right-to-left:          7 -> 8 -> ... -> 13
       CPU store left side:                 15
       Back to Player 1 bottom pit 0.
    */

    // if (pos < AWARI_PITS / 2)
    // {
        // if (pos == AWARI_PITS / 2 - 1)
            // return AWARI_PITS;
        // else
            // return (uint8_t)(pos + 1);
    // }

    // if (pos == AWARI_PITS)
        // return AWARI_PITS / 2;

    // if (pos < AWARI_PITS)
    // {
        // if (pos == AWARI_PITS - 1)
            // return AWARI_PITS + 1;
        // else
            // return (uint8_t)(pos + 1);
    // }
    return (pos+1)%AWARI_PITS;
    // return 0;
}

static void awari_start_move(uint8_t player, uint8_t pit)
{
    if (g_board[pit] == 0)
        return;

    g_animating = 1;
    g_move_player = player;
    g_move_pit = pit;
    g_move_hand = g_board[pit];
    g_move_pos = pit;
}

static void awari_step_animation(void)
{
    uint8_t dest;

    if (g_move_hand == 0)
    {
        awari_finish_move();
        return;
    }

    if (g_board[g_move_pit] > 0)
        g_board[g_move_pit]--;

    dest = awari_next_position(g_move_pos);

    if (dest < AWARI_PITS)
    {
        g_board[dest]++;
    }
    // else if (dest == AWARI_PITS + g_move_player)
    // {
        // g_store[g_move_player]++;
    // }
    // else
    // {
        // opp = (uint8_t)(1 - g_move_player);
        // g_store[opp]++;
    // }

    g_move_pos = dest;
    g_move_hand--;

    awari_update_board_graphics();
    _XL_SLEEP(1);
}

static void awari_finish_move(void)
{
    uint8_t opp;

    g_animating = 0;
    g_player = (uint8_t)(1 - g_move_player);
    awari_check_game_over();
    awari_begin_turn();

    opp = (uint8_t)(1 - g_player);
    (void)opp;
}


static void awari_check_game_over(void)
{
    if (g_store[0] >= AWARI_WIN_STORE || g_store[1] >= AWARI_WIN_STORE)
    {
        g_game_over = 1;
        if (g_store[0] > g_store[1])
            g_winner = 0;
        else if (g_store[1] > g_store[0])
            g_winner = 1;
        else
            g_winner = 255;
    }

    if (!awari_player_has_seeds(0) && !awari_player_has_seeds(1))
    {
        g_game_over = 1;
        if (g_store[0] > g_store[1])
            g_winner = 0;
        else if (g_store[1] > g_store[0])
            g_winner = 1;
        else
            g_winner = 255;
    }
}

static void awari_begin_turn(void)
{
    uint8_t passes;

    passes = 0;

    while (!g_game_over && !awari_player_has_seeds(g_player))
    {
        g_player = (uint8_t)(1 - g_player);
        passes++;

        if (passes >= 2)
        {
            g_game_over = 1;
            if (g_store[0] > g_store[1])
                g_winner = 0;
            else if (g_store[1] > g_store[0])
                g_winner = 1;
            else
                g_winner = 255;
        }
    }
}

static uint8_t awari_player_has_seeds(uint8_t player)
{
    short i;
    short start;
    short end;

    if (player == 0)
    {
        start = 0;
        end = AWARI_PITS / 2 - 1;
    }
    else
    {
        start = AWARI_PITS / 2;
        end = AWARI_PITS - 1;
    }

    for (i = start; i <= end; i++)
    {
        if (g_board[i] > 0)
            return 1;
    }

    return 0;
}

static short awari_ai_score(uint8_t player, uint8_t pit)
{
    static uint8_t sim_board[AWARI_PITS];
    static uint8_t sim_store[2];
    short i;
    short diff;
    uint8_t hand;
    uint8_t pos;
    uint8_t opp;

    for (i = 0; i < AWARI_PITS; i++)
        sim_board[i] = g_board[i];

    sim_store[0] = g_store[0];
    sim_store[1] = g_store[1];

    hand = sim_board[pit];
    if (hand == 0)
        return -100;

    sim_board[pit] = 0;
    pos = pit;
    opp = (uint8_t)(1 - player);

    while (hand > 0)
    {
        pos = awari_next_position(pos);

        if (pos < AWARI_PITS)
        {
            sim_board[pos]++;
            hand--;
        }
        else if (pos == AWARI_PITS + player)
        {
            sim_store[player]++;
            hand--;
        }
        else
        {
            sim_store[opp]++;
            hand--;
        }
    }

    diff = (short)sim_store[player] - (short)sim_store[opp];

    if (sim_store[player] >= AWARI_WIN_STORE)
        diff += 100;
    else if (sim_store[opp] >= AWARI_WIN_STORE)
        diff -= 100;

    return diff;
}

static uint8_t awari_ai_choose_pit(uint8_t player)
{
    short i;
    short start;
    short end;
    short best_score;
    short score;
    uint8_t best_pit;
    uint16_t rand_value;

    if (!awari_player_has_seeds(player))
        return 255;

    if (player == 0)
    {
        start = 0;
        end = AWARI_PITS / 2 - 1;
    }
    else
    {
        start = AWARI_PITS / 2;
        end = AWARI_PITS - 1;
    }

    best_pit = 255;
    best_score = -1000;

    for (i = start; i <= end; i++)
    {
        if (g_board[i] > 0)
        {
            score = awari_ai_score(player, (uint8_t)i);

            if (best_pit == 255 || score >= best_score)
            {
                best_pit = (uint8_t)i;
                best_score = score;
            }
        }
    }

    if (best_pit != 255)
    {
        rand_value = _XL_RAND();

        if ((rand_value % 4) == 0 && best_score > -1000)
        {
            for (i = start; i <= end; i++)
            {
                if (g_board[i] > 0)
                {
                    score = awari_ai_score(player, (uint8_t)i);

                    if (score == best_score)
                        return (uint8_t)i;
                }
            }
        }

        return best_pit;
    }

    return 255;
}
