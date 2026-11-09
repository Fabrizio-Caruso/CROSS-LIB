#include "cross_lib.h"

#define BOARD_TOP 1
#define FIELD_MAX 10
#define ROW_MAX 160

#define BRICK_TILE   _TILE_0
#define BORDER_TILE  _TILE_8


static uint8_t g_board[ROW_MAX][FIELD_MAX];
static uint8_t g_prev[ROW_MAX][FIELD_MAX];
static uint8_t g_cur[ROW_MAX][FIELD_MAX];

static uint8_t g_type;
static uint8_t g_rot;
static uint8_t g_x;
static uint8_t g_y;
static uint8_t g_active;
static uint8_t g_over;
static uint8_t g_over_shown;
static uint8_t g_speed;
static uint8_t g_frame;
static uint16_t g_score;
static uint8_t g_fleft;
static uint8_t g_fw;
static uint8_t g_next_type;

static short g_piece_base[7][4][2] =
{
    {{0,1},{1,1},{2,1},{3,1}},
    {{1,1},{2,1},{1,2},{2,2}},
    {{1,0},{0,1},{1,1},{2,1}},
    {{1,0},{2,0},{0,1},{1,1}},
    {{0,0},{1,0},{1,1},{2,1}},
    {{1,0},{2,0},{3,0},{1,1}},
    {{1,0},{2,0},{3,0},{3,1}}
};

static void clear_all_grids(void);
static void show_score(void);
static void draw_hud_static(void);
static void show_game_over(void);
static void setup_field(void);
static void get_cell_coord(uint8_t type, uint8_t rot, uint8_t cell, short *px, short *py);
static int collision(uint8_t type, uint8_t rot, short px, short py);
static int spawn_piece(void);
static uint8_t clear_lines(void);
static void rotate_piece(void);
static void lock_piece(void);
static void build_visual(void);
static void render_diff(void);
static void update_gravity(void);
static void process_playing(uint8_t input);
static void reset_game(void);
static void draw_borders(void);
static void draw_next_piece(void);

#if !defined(_XL_NO_COLOR)
static uint8_t color_for_value(uint8_t value)
{
    if (value == 1) { return (uint8_t)_XL_CYAN; }
    if (value == 2) { return (uint8_t)_XL_RED; }
    if (value == 3) { return (uint8_t)_XL_MAGENTA; }
    if (value == 4) { return (uint8_t)_XL_GREEN; }
    if (value == 5) { return (uint8_t)_XL_BLUE; }
    if (value == 6) { return (uint8_t)_XL_YELLOW; }
    return (uint8_t)_XL_WHITE;
}
#endif

static void clear_all_grids(void)
{
    short y;
    short x;

    for (y = 0; y < YSize - 1; y++)
    {
        for (x = 0; x < FIELD_MAX; x++)
        {
            g_board[y][x] = 0;
            g_prev[y][x] = 0;
            g_cur[y][x] = 0;
        }
    }
}

static void show_score(void)
{
    if (XSize >= 12)
    {
        _XL_PRINTD(6, 0, 5, (uint16_t)g_score);
    }
    else
    {
        _XL_PRINTD(0, 0, 5, (uint16_t)g_score);
    }
}

static void draw_hud_static(void)
{
    if (XSize >= 12)
    {
        _XL_PRINT(0, 0, "SCORE");
    }

    show_score();
}

static void show_game_over(void)
{
    if (XSize >= 9)
    {
        _XL_PRINT(0, 0, "GAME OVER");
    }
    else
    {
        _XL_PRINT(0, 0, "GAME");
        if (YSize > 1)
        {
            _XL_PRINT(0, 1, "OVER");
        }
    }
}

static void setup_field(void)
{
    uint8_t width;
    uint8_t left;

    if (XSize >= 10)
    {
        width = 10;
        left = (uint8_t)((XSize - 10) / 2);
    }
    else
    {
        width = (uint8_t)XSize;
        left = 0;
    }

    g_fw = width;
    g_fleft = left;
}

static void get_cell_coord(uint8_t type, uint8_t rot, uint8_t cell, short *px, short *py)
{
    short x;
    short y;
    short rx;
    short ry;
    short r;

    x = g_piece_base[type][cell][0];
    y = g_piece_base[type][cell][1];

    if (type != 1)
    {
        r = 0;
        while (r < rot)
        {
            rx = (short)(3 - y);
            ry = x;
            x = rx;
            y = ry;
            r++;
        }
    }

    *px = x;
    *py = y;
}

static int collision(uint8_t type, uint8_t rot, short px, short py)
{
    short c;
    short cx;
    short cy;
    short sx;
    short sy;
    short fx;
    short right;

    right = (short)g_fleft + g_fw;

    for (c = 0; c < 4; c++)
    {
        get_cell_coord(type, rot, (uint8_t)c, &cx, &cy);

        sx = px + cx;
        sy = py + cy;

        if (sx < g_fleft || sx >= right || sy < BOARD_TOP || sy >= YSize - 1)
        {
            return 1;
        }

        fx = (short)(sx - g_fleft);

        if (g_board[sy][fx] != 0)
        {
            return 1;
        }
    }

    return 0;
}

static int spawn_piece(void)
{
    short raw_x;
    short right;

    g_type = g_next_type;
    g_next_type = (uint8_t)(_XL_RAND() % 7);
    g_rot = 0;

    raw_x = (short)(g_fleft + (g_fw / 2) - 2);

    if (raw_x < g_fleft)
    {
        raw_x = (short)g_fleft;
    }

    right = (short)g_fleft + g_fw;
    if (raw_x + 3 >= right)
    {
        raw_x = (short)g_fleft;
    }

    g_x = (uint8_t)raw_x;
    g_y = (uint8_t)BOARD_TOP;

    if (collision(g_type, g_rot, raw_x, (short)BOARD_TOP))
    {
        g_active = 0;
        return 0;
    }

    g_active = 1;
    g_frame = 0;
    return 1;
}

static uint8_t clear_lines(void)
{
    short y;
    short x;
    short full;
    short shift;
    short lines;
    short fw;

    fw = (short)g_fw;
    y = BOARD_TOP;
    lines = 0;

    while (y < YSize - 1)
    {
        full = 1;

        for (x = 0; x < fw; x++)
        {
            if (g_board[y][x] == 0)
            {
                full = 0;
                break;
            }
        }

        if (full != 0)
        {
            lines++;
            shift = y;

            while (shift > BOARD_TOP)
            {
                for (x = 0; x < fw; x++)
                {
                    g_board[shift][x] = g_board[shift - 1][x];
                }
                shift--;
            }

            for (x = 0; x < fw; x++)
            {
                g_board[BOARD_TOP][x] = 0;
            }

            y = BOARD_TOP;
        }
        else
        {
            y++;
        }
    }

    return (uint8_t)lines;
}

static void lock_piece(void)
{
    short c;
    short cx;
    short cy;
    short sx;
    short sy;
    short fx;
    uint8_t lines;
    uint16_t add;

    if (g_active == 0)
    {
        return;
    }

    for (c = 0; c < 4; c++)
    {
        get_cell_coord(g_type, g_rot, (uint8_t)c, &cx, &cy);

        sx = (short)g_x + cx;
        sy = (short)g_y + cy;

        if (sx < g_fleft || sx >= (short)g_fleft + g_fw || sy < BOARD_TOP || sy >= YSize - 1)
        {
            g_active = 0;
            g_over = 1;
            return;
        }

        fx = (short)(sx - g_fleft);
        g_board[sy][fx] = (uint8_t)(g_type + 1);
    }

    g_active = 0;

    lines = clear_lines();

    if (lines != 0)
    {
        _XL_TOCK_SOUND();

        add = (uint16_t)(lines * 100);
        g_score = (uint16_t)(g_score + add);
        if (g_score < add)
        {
            g_score = 0xFFFF;
        }

        if (g_speed > 2)
        {
            g_speed--;
        }

        show_score();
    }

    g_frame = 0;

    if (!spawn_piece())
    {
        g_over = 1;
    }
}

static void rotate_piece(void)
{
    uint8_t nr;
    short bx;
    short by;
    short tx;
    short ty;

    if (g_active == 0)
    {
        return;
    }

    nr = g_rot;
    if (nr == 3)
    {
        nr = 0;
    }
    else
    {
        nr = (uint8_t)(nr + 1);
    }

    bx = (short)g_x;
    by = (short)g_y;

    tx = bx;
    ty = by;
    if (!collision(g_type, nr, tx, ty))
    {
        g_x = (uint8_t)tx;
        g_y = (uint8_t)ty;
        g_rot = nr;
        _XL_PING_SOUND();
        return;
    }

    tx = bx - 1;
    ty = by;
    if (!collision(g_type, nr, tx, ty))
    {
        g_x = (uint8_t)tx;
        g_y = (uint8_t)ty;
        g_rot = nr;
        _XL_PING_SOUND();
        return;
    }

    tx = bx + 1;
    ty = by;
    if (!collision(g_type, nr, tx, ty))
    {
        g_x = (uint8_t)tx;
        g_y = (uint8_t)ty;
        g_rot = nr;
        _XL_PING_SOUND();
        return;
    }

    tx = bx;
    ty = by + 1;
    if (!collision(g_type, nr, tx, ty))
    {
        g_x = (uint8_t)tx;
        g_y = (uint8_t)ty;
        g_rot = nr;
        _XL_PING_SOUND();
        return;
    }

    tx = bx;
    ty = by - 1;
    if (!collision(g_type, nr, tx, ty))
    {
        g_x = (uint8_t)tx;
        g_y = (uint8_t)ty;
        g_rot = nr;
        _XL_PING_SOUND();
        return;
    }

    tx = bx - 2;
    ty = by;
    if (!collision(g_type, nr, tx, ty))
    {
        g_x = (uint8_t)tx;
        g_y = (uint8_t)ty;
        g_rot = nr;
        _XL_PING_SOUND();
        return;
    }

    tx = bx + 2;
    ty = by;
    if (!collision(g_type, nr, tx, ty))
    {
        g_x = (uint8_t)tx;
        g_y = (uint8_t)ty;
        g_rot = nr;
        _XL_PING_SOUND();
        return;
    }
}

static void build_visual(void)
{
    short y;
    short x;
    short c;
    short cx;
    short cy;
    short sx;
    short sy;
    short fx;
    short fw;

    fw = (short)g_fw;

    for (y = 0; y < YSize - 1; y++)
    {
        for (x = 0; x < FIELD_MAX; x++)
        {
            g_cur[y][x] = 0;
        }
    }

    for (y = BOARD_TOP; y < YSize - 1; y++)
    {
        for (x = 0; x < fw; x++)
        {
            g_cur[y][x] = g_board[y][x];
        }
    }

    if (g_active != 0)
    {
        for (c = 0; c < 4; c++)
        {
            get_cell_coord(g_type, g_rot, (uint8_t)c, &cx, &cy);

            sx = (short)g_x + cx;
            sy = (short)g_y + cy;

            if (sx >= g_fleft && sx < g_fleft + g_fw && sy >= BOARD_TOP && sy < YSize - 1)
            {
                fx = (short)(sx - g_fleft);
                if (fx >= 0 && fx < fw)
                {
                    g_cur[sy][fx] = (uint8_t)(g_type + 1);
                }
            }
        }
    }
}

static void render_diff(void)
{
    short y;
    short x;
    short fw;
    uint8_t oldv;
    uint8_t newv;
    uint8_t sx;

    fw = (short)g_fw;

    for (y = BOARD_TOP; y < YSize - 1; y++)
    {
        for (x = 0; x < fw; x++)
        {
            sx = (uint8_t)(g_fleft + x);
            oldv = g_prev[y][x];
            newv = g_cur[y][x];

            if (oldv != newv)
            {
                if (oldv != 0)
                {
                    _XL_DELETE(sx, (uint8_t)y);
                }

                if (newv != 0)
                {
                    _XL_DRAW(sx, (uint8_t)y, BRICK_TILE, color_for_value(newv));
                }

                g_prev[y][x] = newv;
            }
        }
    }
}

static void update_gravity(void)
{
    if (g_active == 0)
    {
        return;
    }

    g_frame = (uint8_t)(g_frame + 1);

    if (g_frame >= g_speed)
    {
        g_frame = 0;

        if (collision(g_type, g_rot, (short)g_x, (short)(g_y + 1)))
        {
            lock_piece();
        }
        else
        {
            g_y = (uint8_t)(g_y + 1);
        }
    }
}

static void process_playing(uint8_t input)
{
    short cx;
    short cy;
    short ny;

    if (g_active == 0)
    {
        return;
    }

    if (_XL_LEFT(input) != 0)
    {
        cx = (short)g_x - 1;
        cy = (short)g_y;
        if (!collision(g_type, g_rot, cx, cy))
        {
            g_x = (uint8_t)cx;
            _XL_TICK_SOUND();
        }
    }

    if (_XL_RIGHT(input) != 0)
    {
        cx = (short)g_x + 1;
        cy = (short)g_y;
        if (!collision(g_type, g_rot, cx, cy))
        {
            g_x = (uint8_t)cx;
            _XL_TICK_SOUND();
        }
    }

    if (_XL_DOWN(input) != 0)
    {
        cx = (short)g_x;
        cy = (short)g_y + 1;
        if (!collision(g_type, g_rot, cx, cy))
        {
            g_y = (uint8_t)cy;
        }
    }

    if (_XL_FIRE(input) != 0 && g_active != 0)
    {
        rotate_piece();
    }

    if (_XL_UP(input) != 0 && g_active != 0)
    {
        _XL_SHOOT_SOUND();
        ny = (short)g_y;

        while (!collision(g_type, g_rot, (short)g_x, (short)(ny + 1)))
        {
            ny = (short)(ny + 1);
        }

        g_y = (uint8_t)ny;
        lock_piece();
    }
}

static void draw_borders(void)
{
    short x;
    short y;
    short right;

    right = (short)g_fleft + (short)g_fw;

    /* Top border: row BOARD_TOP - 1 */
    for (x = (short)g_fleft; x < right; x++)
    {
        _XL_DRAW((uint8_t)x, (uint8_t)(BOARD_TOP - 1), BORDER_TILE, _XL_WHITE);
    }

    /* Left border: column g_fleft - 1 */
    if (g_fleft > 0)
    {
        for (y = (short)BOARD_TOP; y < (short)YSize - 1; y++)
        {
            _XL_DRAW((uint8_t)(g_fleft - 1), (uint8_t)y, BORDER_TILE, _XL_WHITE);
        }
    }

    /* Right border: column g_fleft + g_fw */
    if (right < (short)XSize)
    {
        for (y = (short)BOARD_TOP; y < (short)YSize - 1; y++)
        {
            _XL_DRAW((uint8_t)right, (uint8_t)y, BORDER_TILE, _XL_WHITE);
        }
    }

    /* Bottom border: row YSize - 1 is last playable row;
       draw at the very bottom of the field area */
    for (x = (short)g_fleft; x < right; x++)
    {
        _XL_DRAW((uint8_t)x, (uint8_t)(YSize - 1), BORDER_TILE, _XL_WHITE);
    }
}

static void draw_next_piece(void)
{
    short c;
    short cx;
    short cy;
    short px;
    short py;
    uint8_t nx;
    uint8_t ny;
    #if !defined(_XL_NO_COLOR)
    uint8_t col;
    #endif
    if (XSize < 9)
    {
        return;
    }

    nx = (uint8_t)(XSize - 5);
    ny = (uint8_t)(BOARD_TOP + 1);

    /* Clear the 4x3 preview area first */
    for (cy = 0; cy < 3; cy++)
    {
        for (cx = 0; cx < 4; cx++)
        {
            px = (short)nx + cx;
            py = (short)ny + cy;
            if (px < (short)XSize && py < (short)YSize - 1)
            {
                _XL_DELETE((uint8_t)px, (uint8_t)py);
            }
        }
    }

    /* Draw the next piece in base orientation (rot = 0) */
    #if !defined(_XL_NO_COLOR)
    col = color_for_value((uint8_t)(g_next_type + 1));
    #endif
    for (c = 0; c < 4; c++)
    {
        get_cell_coord(g_next_type, 0, (uint8_t)c, &cx, &cy);

        px = (short)nx + cx;
        py = (short)ny + cy;

        if (px >= 0 && px < (short)XSize && py >= 0 && py < (short)YSize - 1)
        {
            _XL_DRAW((uint8_t)px, (uint8_t)py, BRICK_TILE, col);
        }
    }
}

static void reset_game(void)
{
    _XL_CLEAR_SCREEN();
    clear_all_grids();
    setup_field();

    g_score = 0;
    g_speed = 5;
    g_frame = 0;
    g_over = 0;
    g_over_shown = 0;
    g_active = 0;
    g_next_type = (uint8_t)(_XL_RAND() % 7);

    draw_hud_static();
    draw_borders();

    if (!spawn_piece())
    {
        g_over = 1;
    }

    draw_next_piece();
}

int main(void)
{
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    reset_game();

    for (;;)
    {
        if (g_over == 0)
        {
            input = _XL_INPUT();

            process_playing(input);

            if (g_over == 0)
            {
                update_gravity();
            }

            build_visual();
            render_diff();
            draw_next_piece();
        }
        else
        {
            if (g_over_shown == 0)
            {
                _XL_EXPLOSION_SOUND();
                _XL_CLEAR_SCREEN();
                clear_all_grids();
                show_game_over();
                g_over_shown = 1;
                _XL_SLEEP(1);
            }
            else
            {
                _XL_WAIT_FOR_INPUT();
                reset_game();
                continue;
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}