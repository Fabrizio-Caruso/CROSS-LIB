#include "cross_lib.h"

#define BOARD_TOP 1
#define FIELD_MAX 16
#define ROW_MAX 16
#define SCALE 2

/* 4 unique tiles per colour (value 1..7); index 0 unused */
static const uint8_t g_tile_map[8][4] = {
    {  0,  1,  2,  3},   /* [0] unused          */
    {  4,  5,  6,  7},   /* [1] CYAN            */
    {  8,  9, 10, 11},   /* [2] RED             */
    { 12, 13, 14, 15},   /* [3] MAGENTA         */
    { 16, 17, 18, 19},   /* [4] GREEN           */
    { 20, 21, 22, 23},   /* [5] BLUE            */
    { 24, 25, 26, 27},   /* [6] YELLOW          */
    { 28, 29, 30, 31}    /* [7] WHITE           */
};

#define BORDER_TILE _TILE_26

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
static uint8_t g_field_h;

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

static uint8_t color_for_value(uint8_t value);
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
static void draw_border(void);
static void draw_cell(short ax, short ay, uint8_t value);
static void erase_cell(short ax, short ay);

/* ─── helpers ─────────────────────────────────────────────────────────── */

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

/* Draw a 2×2 block of tiles for one logical cell.
   ax, ay are absolute cell coordinates (g_x / g_y space). */
static void draw_cell(short ax, short ay, uint8_t value)
{
    short px;
    short py;
    uint8_t col;
    uint8_t t0, t1, t2, t3;

    if (value < 1 || value > 7) { return; }

    px = (short)(ax * SCALE);
    py = (short)(ay * SCALE);

    col = color_for_value(value);
    t0 = g_tile_map[value][0];
    t1 = g_tile_map[value][1];
    t2 = g_tile_map[value][2];
    t3 = g_tile_map[value][3];

    _XL_DRAW((uint8_t)px,     (uint8_t)py,     t0, col);
    _XL_DRAW((uint8_t)(px+1), (uint8_t)py,     t1, col);
    _XL_DRAW((uint8_t)px,     (uint8_t)(py+1), t2, col);
    _XL_DRAW((uint8_t)(px+1), (uint8_t)(py+1), t3, col);
}

static void erase_cell(short ax, short ay)
{
    short px;
    short py;

    px = (short)(ax * SCALE);
    py = (short)(ay * SCALE);

    _XL_DELETE((uint8_t)px,     (uint8_t)py);
    _XL_DELETE((uint8_t)(px+1), (uint8_t)py);
    _XL_DELETE((uint8_t)px,     (uint8_t)(py+1));
    _XL_DELETE((uint8_t)(px+1), (uint8_t)(py+1));
}

/* Draw a 1-pixel-wide white border around the play field. */
static void draw_border(void)
{
    short left;
    short right;
    short top;
    short bottom;
    short i;

    left   = (short)(g_fleft * SCALE) - 1;
    right  = (short)((g_fleft + g_fw) * SCALE);
    top    = (short)(BOARD_TOP * SCALE) - 1;
    bottom = (short)((BOARD_TOP + g_field_h) * SCALE);

    /* top edge */
    if (top >= 0)
    {
        for (i = left; i <= right; i++)
        {
            if (i >= 0 && i < XSize)
            {
                _XL_DRAW((uint8_t)i, (uint8_t)top, BORDER_TILE, (uint8_t)_XL_WHITE);
            }
        }
    }

    /* bottom edge */
    if (bottom < YSize)
    {
        for (i = left; i <= right; i++)
        {
            if (i >= 0 && i < XSize)
            {
                _XL_DRAW((uint8_t)i, (uint8_t)bottom, BORDER_TILE, (uint8_t)_XL_WHITE);
            }
        }
    }

    /* left edge (skip corners already drawn) */
    if (left >= 0)
    {
        for (i = top + 1; i <= bottom - 1; i++)
        {
            if (i >= 0 && i < YSize)
            {
                _XL_DRAW((uint8_t)left, (uint8_t)i, BORDER_TILE, (uint8_t)_XL_WHITE);
            }
        }
    }

    /* right edge (skip corners) */
    if (right < XSize)
    {
        for (i = top + 1; i <= bottom - 1; i++)
        {
            if (i >= 0 && i < YSize)
            {
                _XL_DRAW((uint8_t)right, (uint8_t)i, BORDER_TILE, (uint8_t)_XL_WHITE);
            }
        }
    }
}

/* ─── core game logic (unchanged except field-height bound) ───────────── */

static void clear_all_grids(void)
{
    short y;
    short x;

    for (y = 0; y < ROW_MAX; y++)
    {
        for (x = 0; x < FIELD_MAX; x++)
        {
            g_board[y][x] = 0;
            g_prev[y][x]  = 0;
            g_cur[y][x]   = 0;
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
    uint8_t max_w;
    uint8_t max_h;

    /* field width in cells: at most 10, must fit in XSize pixels (2 per cell) */
    max_w = (uint8_t)(XSize / SCALE);
    if (max_w > FIELD_MAX) { max_w = FIELD_MAX; }
    g_fw = max_w;
    g_fleft = (uint8_t)((XSize - (uint8_t)(g_fw * SCALE)) / SCALE);

    /* field height in cells: from BOARD_TOP to (YSize-2)/2 inclusive */
    max_h = (uint8_t)((YSize - 2) / SCALE);
    if (max_h <= BOARD_TOP) { max_h = BOARD_TOP + 1; }
    g_field_h = (uint8_t)(max_h - BOARD_TOP + 1);
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
    short bottom;

    right  = (short)g_fleft + g_fw;
    bottom = (short)(BOARD_TOP + g_field_h);

    for (c = 0; c < 4; c++)
    {
        get_cell_coord(type, rot, (uint8_t)c, &cx, &cy);

        sx = px + cx;
        sy = py + cy;

        if (sx < g_fleft || sx >= right || sy < BOARD_TOP || sy >= bottom)
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

    g_type = (uint8_t)(_XL_RAND() % 7);
    g_rot  = 0;

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
    g_frame  = 0;
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
    short bottom;

    fw     = (short)g_fw;
    bottom = (short)(BOARD_TOP + g_field_h);
    y      = BOARD_TOP;
    lines  = 0;

    while (y < bottom)
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

        if (sx < g_fleft || sx >= (short)g_fleft + g_fw ||
            sy < BOARD_TOP || sy >= (short)(BOARD_TOP + g_field_h))
        {
            g_active = 0;
            g_over   = 1;
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

    tx = bx; ty = by;
    if (!collision(g_type, nr, tx, ty))
    { g_x = (uint8_t)tx; g_y = (uint8_t)ty; g_rot = nr; _XL_PING_SOUND(); return; }

    tx = bx - 1; ty = by;
    if (!collision(g_type, nr, tx, ty))
    { g_x = (uint8_t)tx; g_y = (uint8_t)ty; g_rot = nr; _XL_PING_SOUND(); return; }

    tx = bx + 1; ty = by;
    if (!collision(g_type, nr, tx, ty))
    { g_x = (uint8_t)tx; g_y = (uint8_t)ty; g_rot = nr; _XL_PING_SOUND(); return; }

    tx = bx; ty = by + 1;
    if (!collision(g_type, nr, tx, ty))
    { g_x = (uint8_t)tx; g_y = (uint8_t)ty; g_rot = nr; _XL_PING_SOUND(); return; }

    tx = bx; ty = by - 1;
    if (!collision(g_type, nr, tx, ty))
    { g_x = (uint8_t)tx; g_y = (uint8_t)ty; g_rot = nr; _XL_PING_SOUND(); return; }

    tx = bx - 2; ty = by;
    if (!collision(g_type, nr, tx, ty))
    { g_x = (uint8_t)tx; g_y = (uint8_t)ty; g_rot = nr; _XL_PING_SOUND(); return; }

    tx = bx + 2; ty = by;
    if (!collision(g_type, nr, tx, ty))
    { g_x = (uint8_t)tx; g_y = (uint8_t)ty; g_rot = nr; _XL_PING_SOUND(); return; }
}

/* ─── rendering ───────────────────────────────────────────────────────── */

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
    short bottom;

    fw     = (short)g_fw;
    bottom = (short)(BOARD_TOP + g_field_h);

    for (y = 0; y < ROW_MAX; y++)
    {
        for (x = 0; x < FIELD_MAX; x++)
        {
            g_cur[y][x] = 0;
        }
    }

    for (y = BOARD_TOP; y < bottom; y++)
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

            if (sx >= g_fleft && sx < g_fleft + fw &&
                sy >= BOARD_TOP && sy < bottom)
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
    short bottom;
    uint8_t oldv;
    uint8_t newv;

    fw     = (short)g_fw;
    bottom = (short)(BOARD_TOP + g_field_h);

    for (y = BOARD_TOP; y < bottom; y++)
    {
        for (x = 0; x < fw; x++)
        {
            oldv = g_prev[y][x];
            newv = g_cur[y][x];

            if (oldv != newv)
            {
                if (oldv != 0)
                {
                    erase_cell((short)(g_fleft + x), y);
                }

                if (newv != 0)
                {
                    draw_cell((short)(g_fleft + x), y, newv);
                }

                g_prev[y][x] = newv;
            }
        }
    }
}

/* ─── game flow ───────────────────────────────────────────────────────── */

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

static void reset_game(void)
{
    _XL_CLEAR_SCREEN();
    clear_all_grids();
    setup_field();

    g_score       = 0;
    g_speed       = 5;
    g_frame       = 0;
    g_over        = 0;
    g_over_shown  = 0;
    g_active      = 0;

    draw_border();
    draw_hud_static();

    if (!spawn_piece())
    {
        g_over = 1;
    }
}

/* ─── main ────────────────────────────────────────────────────────────── */

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