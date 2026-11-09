#include "cross_lib.h"

#define T_EMPTY   _TILE_0
#define T_X       _TILE_1
#define T_O       _TILE_2
#define T_BORD    _TILE_4
#define T_BIG_X   _TILE_5
#define T_BIG_O   _TILE_6
#define T_CURSOR  _TILE_7

#define FREE_SUB  0xFF

#define BOARD_W   13
#define BOARD_H   13

static uint8_t grid[9][9];
static uint8_t sub_state[3][3];

static uint8_t cur_player;
static uint8_t target_sub;
static uint8_t cur_gx;
static uint8_t cur_gy;

static uint8_t game_over;
static uint8_t winner;
static uint8_t score_x;
static uint8_t score_o;

static uint8_t bx;
static uint8_t by;
static uint8_t turn_row;
static uint8_t msg_row;

static uint8_t px(uint8_t gx)
{
    uint8_t r;

    r = bx + 1 + gx;
    if (gx >= 3) r++;
    if (gx >= 6) r++;

    return r;
}

static uint8_t py(uint8_t gy)
{
    uint8_t r;

    r = by + 1 + gy;
    if (gy >= 3) r++;
    if (gy >= 6) r++;

    return r;
}

static void draw_borders(void)
{
    uint8_t x, y;

    for (x = 0; x < BOARD_W; x++)
    {
        for (y = 0; y < BOARD_H; y++)
        {
            if (x == 0 || x == 4 || x == 8 || x == 12 ||
                y == 0 || y == 4 || y == 8 || y == 12)
            {
                _XL_DRAW(bx + x, by + y, T_BORD, _XL_WHITE);
            }
        }
    }
}

static void draw_all_empty(void)
{
    uint8_t i, j;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            _XL_DRAW(px(i), py(j), T_EMPTY, _XL_BLUE);
        }
    }
}

static void print_line(uint8_t row, char *msg)
{
    char line[17];
    uint8_t i;

    for (i = 0; i < 16; i++)
    {
        line[i] = ' ';
    }
    line[16] = 0;

    for (i = 0; i < 16; i++)
    {
        if (msg[i] == 0)
        {
            break;
        }
        line[i] = msg[i];
    }

    _XL_PRINT(0, row, line);
}

static void set_message(char *msg)
{
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    print_line(msg_row, msg);
}

static void show_scores(void)
{
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(0, 0, "X");
    _XL_PRINTD(1, 0, 3, score_x);

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(6, 0, "O");
    _XL_PRINTD(7, 0, 3, score_o);
}

static void show_turn(void)
{
    char *line_text;

    if (target_sub == FREE_SUB)
    {
        if (cur_player == 1)
        {
            line_text = "X PICKS ANY";
        }
        else
        {
            line_text = "O PICKS ANY";
        }
    }
    else
    {
        if (cur_player == 1)
        {
            line_text = "X MUST PLAY";
        }
        else
        {
            line_text = "O MUST PLAY";
        }
    }

    if (cur_player == 1)
    {
        _XL_SET_TEXT_COLOR(_XL_RED);
    }
    else
    {
        _XL_SET_TEXT_COLOR(_XL_CYAN);
    }

    print_line(turn_row, line_text);

    if (target_sub != FREE_SUB)
    {
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD(12, turn_row, 1, target_sub);
    }
}

static void draw_cell_state(uint8_t gx, uint8_t gy)
{
    uint8_t s, t;
    uint8_t tile;
    uint8_t color;

    s = gx / 3;
    t = gy / 3;

    if (sub_state[s][t] == 1)
    {
        tile = T_BIG_X;
        color = _XL_RED;
    }
    else if (sub_state[s][t] == 2)
    {
        tile = T_BIG_O;
        color = _XL_CYAN;
    }
    else if (grid[gx][gy] == 1)
    {
        tile = T_X;
        color = _XL_RED;
    }
    else if (grid[gx][gy] == 2)
    {
        tile = T_O;
        color = _XL_CYAN;
    }
    else
    {
        tile = T_EMPTY;
        color = _XL_BLUE;
    }

    _XL_DRAW(px(gx), py(gy), tile, color);
}

static void draw_cursor(uint8_t gx, uint8_t gy)
{
    _XL_DRAW(px(gx), py(gy), T_CURSOR, _XL_YELLOW);
}

static uint8_t is_cell_available(uint8_t gx, uint8_t gy)
{
    uint8_t s, t;

    s = gx / 3;
    t = gy / 3;

    if (sub_state[s][t] != 0)
    {
        return 0;
    }

    if (grid[gx][gy] != 0)
    {
        return 0;
    }

    if (target_sub != FREE_SUB)
    {
        if (s != target_sub / 3 || t != target_sub % 3)
        {
            return 0;
        }
    }

    return 1;
}

static void move_cursor(uint8_t dx, uint8_t dy)
{
    uint8_t ox, oy;
    uint8_t nx, ny;
    uint8_t s, t;
    uint8_t lx, ly;
    uint8_t step;
    uint8_t found;

    if (game_over != 0)
    {
        return;
    }

    ox = cur_gx;
    oy = cur_gy;

    if (target_sub != FREE_SUB)
    {
        s = target_sub / 3;
        t = target_sub % 3;

        if (sub_state[s][t] != 0 || ox / 3 != s || oy / 3 != t)
        {
            target_sub = FREE_SUB;
        }
    }

    found = 0;
    nx = ox;
    ny = oy;

    if (target_sub != FREE_SUB)
    {
        s = target_sub / 3;
        t = target_sub % 3;

        lx = ox - (s * 3);
        ly = oy - (t * 3);

        for (step = 1; step < 9 && found == 0; step++)
        {
            if (dx == 1)
            {
                lx = (lx + 1) % 3;
            }
            else if (dx == 2)
            {
                lx = (lx + 2) % 3;
            }

            if (dy == 1)
            {
                ly = (ly + 1) % 3;
            }
            else if (dy == 2)
            {
                ly = (ly + 2) % 3;
            }

            nx = s * 3 + lx;
            ny = t * 3 + ly;

            if (sub_state[s][t] == 0 && grid[nx][ny] == 0)
            {
                found = 1;
            }
        }

        if (found == 0)
        {
            target_sub = FREE_SUB;
        }
    }

    if (target_sub == FREE_SUB)
    {
        for (step = 1; step < 81 && found == 0; step++)
        {
            nx = (uint8_t)((ox + dx * step) % 9);
            ny = (uint8_t)((oy + dy * step) % 9);

            if (is_cell_available(nx, ny) != 0)
            {
                found = 1;
            }
        }
    }

    if (found != 0 && (nx != ox || ny != oy))
    {
        draw_cell_state(ox, oy);

        cur_gx = nx;
        cur_gy = ny;

        draw_cursor(cur_gx, cur_gy);

        _XL_TICK_SOUND();
    }
}

static uint8_t check_sub(uint8_t s, uint8_t t)
{
    uint8_t ox, oy;
    uint8_t i;
    uint8_t v;

    ox = s * 3;
    oy = t * 3;

    for (i = 0; i < 3; i++)
    {
        v = grid[ox][oy + i];

        if (v != 0 &&
            grid[ox + 1][oy + i] == v &&
            grid[ox + 2][oy + i] == v)
        {
            return v;
        }
    }

    for (i = 0; i < 3; i++)
    {
        v = grid[ox + i][oy];

        if (v != 0 &&
            grid[ox + i][oy + 1] == v &&
            grid[ox + i][oy + 2] == v)
        {
            return v;
        }
    }

    v = grid[ox][oy];

    if (v != 0 &&
        grid[ox + 1][oy + 1] == v &&
        grid[ox + 2][oy + 2] == v)
    {
        return v;
    }

    v = grid[ox + 2][oy];

    if (v != 0 &&
        grid[ox + 1][oy + 1] == v &&
        grid[ox][oy + 2] == v)
    {
        return v;
    }

    return 0;
}

static uint8_t sub_full(uint8_t s, uint8_t t)
{
    uint8_t i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (grid[s * 3 + i][t * 3 + j] == 0)
            {
                return 0;
            }
        }
    }

    return 1;
}

static uint8_t check_game(void)
{
    uint8_t i;
    uint8_t v;

    for (i = 0; i < 3; i++)
    {
        v = sub_state[i][0];

        if ((v == 1 || v == 2) &&
            sub_state[i][1] == v &&
            sub_state[i][2] == v)
        {
            return v;
        }
    }

    for (i = 0; i < 3; i++)
    {
        v = sub_state[0][i];

        if ((v == 1 || v == 2) &&
            sub_state[1][i] == v &&
            sub_state[2][i] == v)
        {
            return v;
        }
    }

    v = sub_state[0][0];

    if ((v == 1 || v == 2) &&
        sub_state[1][1] == v &&
        sub_state[2][2] == v)
    {
        return v;
    }

    v = sub_state[2][0];

    if ((v == 1 || v == 2) &&
        sub_state[1][1] == v &&
        sub_state[0][2] == v)
    {
        return v;
    }

    return 0;
}

static uint8_t any_open(void)
{
    uint8_t i, j;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (sub_state[i][j] == 0)
            {
                return 1;
            }
        }
    }

    return 0;
}

static void overlay_won(uint8_t s, uint8_t t, uint8_t p)
{
    uint8_t i, j;
    uint8_t tile;
    uint8_t color;

    if (p == 1)
    {
        tile = T_BIG_X;
        color = _XL_RED;
    }
    else
    {
        tile = T_BIG_O;
        color = _XL_CYAN;
    }

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            _XL_DRAW(px(s * 3 + i), py(t * 3 + j), tile, color);
        }
    }
}

static uint8_t set_cursor_to_open(void)
{
    uint8_t i, j;
    uint8_t s, t;
    uint8_t found;
    uint8_t gx, gy;

    found = 0;
    gx = 0;
    gy = 0;

    if (target_sub != FREE_SUB)
    {
        s = target_sub / 3;
        t = target_sub % 3;

        if (sub_state[s][t] != 0)
        {
            target_sub = FREE_SUB;
        }
    }

    if (target_sub != FREE_SUB)
    {
        s = target_sub / 3;
        t = target_sub % 3;

        for (i = 0; i < 3 && found == 0; i++)
        {
            for (j = 0; j < 3 && found == 0; j++)
            {
                gx = s * 3 + i;
                gy = t * 3 + j;

                if (grid[gx][gy] == 0)
                {
                    found = 1;
                }
            }
        }

        if (found != 0)
        {
            cur_gx = gx;
            cur_gy = gy;

            draw_cursor(cur_gx, cur_gy);

            return 1;
        }

        target_sub = FREE_SUB;
    }

    for (i = 0; i < 9 && found == 0; i++)
    {
        for (j = 0; j < 9 && found == 0; j++)
        {
            s = i / 3;
            t = j / 3;

            gx = i;
            gy = j;

            if (sub_state[s][t] == 0 && grid[i][j] == 0)
            {
                found = 1;
            }
        }
    }

    if (found != 0)
    {
        cur_gx = gx;
        cur_gy = gy;

        draw_cursor(cur_gx, cur_gy);

        return 1;
    }

    return 0;
}

static void do_move(void)
{
    uint8_t s, t;
    uint8_t w;
    uint8_t ns;
    uint8_t p;

    if (game_over != 0)
    {
        return;
    }

    if (is_cell_available(cur_gx, cur_gy) == 0)
    {
        _XL_PING_SOUND();
        return;
    }

    s = cur_gx / 3;
    t = cur_gy / 3;

    grid[cur_gx][cur_gy] = cur_player;

    if (cur_player == 1)
    {
        _XL_DRAW(px(cur_gx), py(cur_gy), T_X, _XL_RED);
    }
    else
    {
        _XL_DRAW(px(cur_gx), py(cur_gy), T_O, _XL_CYAN);
    }

    _XL_SHOOT_SOUND();

    if (sub_state[s][t] == 0)
    {
        w = check_sub(s, t);

        if (w != 0)
        {
            sub_state[s][t] = w;

            overlay_won(s, t, w);

            _XL_EXPLOSION_SOUND();
        }
        else if (sub_full(s, t))
        {
            sub_state[s][t] = 3;
        }
    }

    p = check_game();

    if (p != 0)
    {
        game_over = 1;
        winner = p;

        if (p == 1)
        {
            score_x++;
            set_message("X WINS GAME");
        }
        else
        {
            score_o++;
            set_message("O WINS GAME");
        }

        show_scores();

        return;
    }

    if (any_open() == 0)
    {
        game_over = 1;
        winner = 0;

        set_message("DRAW GAME");

        return;
    }

    ns = (cur_gx % 3) * 3 + (cur_gy % 3);

    if (sub_state[ns / 3][ns % 3] != 0)
    {
        target_sub = FREE_SUB;
    }
    else
    {
        target_sub = ns;
    }

    if (cur_player == 1)
    {
        cur_player = 2;
    }
    else
    {
        cur_player = 1;
    }

    show_turn();

    if (set_cursor_to_open() == 0)
    {
        game_over = 1;
        winner = 0;

        set_message("DRAW GAME");
    }
}

static void reset_state(void)
{
    uint8_t i, j;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            grid[i][j] = 0;
        }
    }

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            sub_state[i][j] = 0;
        }
    }

    draw_all_empty();

    target_sub = FREE_SUB;
    cur_player = 1;
    game_over = 0;
    winner = 0;

    show_scores();
    set_message("ARROWS MOVE FIRE");
    show_turn();

    if (set_cursor_to_open() == 0)
    {
        game_over = 1;
        set_message("DRAW GAME");
    }
}

int main(void)
{
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    bx = (uint8_t)((XSize - BOARD_W) / 2);
    by = (uint8_t)((YSize - BOARD_H) / 2 + 1);

    turn_row = (uint8_t)(by - 1);
    msg_row = (uint8_t)(by + BOARD_H);

    score_x = 0;
    score_o = 0;

    _XL_CLEAR_SCREEN();

    draw_borders();
    reset_state();

    for (;;)
    {
        if (game_over == 0)
        {
            input = _XL_INPUT();

            if (_XL_LEFT(input))
            {
                move_cursor(2, 0);
            }
            else if (_XL_RIGHT(input))
            {
                move_cursor(1, 0);
            }
            else if (_XL_UP(input))
            {
                move_cursor(0, 2);
            }
            else if (_XL_DOWN(input))
            {
                move_cursor(0, 1);
            }
            else if (_XL_FIRE(input))
            {
                do_move();
            }
        }
        else
        {
            input = _XL_INPUT();

            if (_XL_FIRE(input))
            {
                _XL_PING_SOUND();
                reset_state();
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}

