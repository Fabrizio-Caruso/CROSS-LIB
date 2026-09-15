#include "cross_lib.h"

#define BS 6
#define NC 6
#define TW 2
#define TH 2
#define BXOFF ((XSize - BS * TW) / 2)
#define BYOFF ((YSize - BS * TH) / 2)

uint8_t g_board[BS][BS];
uint8_t g_disp_tile[BS][BS][4];
uint8_t g_disp_col[BS][BS];
uint8_t g_cx;
uint8_t g_cy;
uint8_t g_sel;
uint8_t g_sx;
uint8_t g_sy;
uint16_t g_score;
uint8_t g_over;
uint8_t g_mrk[BS][BS];

static uint8_t candy_tiles[NC][4] = {
    {_TILE_3,  _TILE_4,  _TILE_5,  _TILE_6},
    {_TILE_7,  _TILE_8,  _TILE_9,  _TILE_10},
    {_TILE_11, _TILE_12, _TILE_13, _TILE_14},
    {_TILE_15, _TILE_16, _TILE_17, _TILE_18},
    {_TILE_19, _TILE_20, _TILE_21, _TILE_22},
    {_TILE_23, _TILE_24, _TILE_25, _TILE_27}
};

static uint8_t candy_color(uint8_t c)
{
    switch (c) {
        case 0: return _XL_RED;
        case 1: return _XL_CYAN;
        case 2: return _XL_GREEN;
        case 3: return _XL_YELLOW;
        case 4: return _XL_BLUE;
        default: return _XL_MAGENTA;
    }
}

static void draw_cell(uint8_t gx, uint8_t gy)
{
    uint8_t c;
    uint8_t sx;
    uint8_t sy;
    uint8_t q;
    uint8_t changed;
    uint8_t tile;
    uint8_t col;
    uint8_t dx;
    uint8_t dy;

    c = g_board[gx][gy];
    sx = (uint8_t)(gx * TW + BXOFF);
    sy = (uint8_t)(gy * TH + BYOFF);

    changed = 0;
    for (q = 0; q < 4; q++) {
        if (gx == g_sx && gy == g_sy && g_sel) {
            tile = _TILE_2;
            col = candy_color(c);
        } else if (gx == g_cx && gy == g_cy) {
            tile = _TILE_1;
            col = candy_color(c);
        } else if (c == 255) {
            tile = _TILE_0;
            col = _XL_MAGENTA;
        } else {
            tile = candy_tiles[c][q];
            col = candy_color(c);
        }
        if (tile != g_disp_tile[gx][gy][q] || col != g_disp_col[gx][gy]) {
            changed = 1;
        }
    }

    if (changed) {
        for (q = 0; q < 4; q++) {
            if (gx == g_sx && gy == g_sy && g_sel) {
                tile = _TILE_2;
                col = candy_color(c);
            } else if (gx == g_cx && gy == g_cy) {
                tile = _TILE_1;
                col = candy_color(c);
            } else if (c == 255) {
                tile = _TILE_0;
                col = _XL_MAGENTA;
            } else {
                tile = candy_tiles[c][q];
                col = candy_color(c);
            }
            dx = (uint8_t)(sx + (q % 2));
            dy = (uint8_t)(sy + (q / 2));
            _XL_DRAW(dx, dy, tile, col);
            g_disp_tile[gx][gy][q] = tile;
            g_disp_col[gx][gy] = col;
        }
    }
}

static void draw_all(void)
{
    uint8_t x;
    uint8_t y;
    for (y = 0; y < BS; y++) {
        for (x = 0; x < BS; x++) {
            draw_cell(x, y);
        }
    }
}

static void clear_disp(void)
{
    uint8_t x;
    uint8_t y;
    uint8_t q;
    for (y = 0; y < BS; y++) {
        for (x = 0; x < BS; x++) {
            for (q = 0; q < 4; q++) {
                g_disp_tile[x][y][q] = 255;
            }
            g_disp_col[x][y] = 255;
        }
    }
}

static void show_score(void)
{
    uint8_t sx;
    uint8_t sy;
    sx = (uint8_t)((XSize - 12) / 2);
    sy = (uint8_t)(BYOFF - 2);
    if (sy > 1) {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(sx, sy, "SCORE");
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD((uint8_t)(sx + 5), sy, 1, g_score);
    } else {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 0, "SCORE");
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD(5, 0, 1, g_score);
    }
}

static uint8_t makes_match(uint8_t x, uint8_t y)
{
    uint8_t c;
    uint8_t cnt;
    uint8_t i;

    c = g_board[x][y];
    cnt = 1;
    i = x;
    while (i > 0 && g_board[i - 1][y] == c) { cnt++; i--; }
    i = x;
    while (i < BS - 1 && g_board[i + 1][y] == c) { cnt++; i++; }
    if (cnt >= 3) return 1;

    cnt = 1;
    i = y;
    while (i > 0 && g_board[x][i - 1] == c) { cnt++; i--; }
    i = y;
    while (i < BS - 1 && g_board[x][i + 1] == c) { cnt++; i++; }
    if (cnt >= 3) return 1;

    return 0;
}

static void mark_matches(void)
{
    uint8_t x;
    uint8_t y;
    uint8_t i;

    for (x = 0; x < BS; x++) {
        for (y = 0; y < BS; y++) {
            g_mrk[x][y] = 0;
        }
    }

    for (y = 0; y < BS; y++) {
        for (x = 0; x < BS; x++) {
            if (x < BS - 2 &&
                g_board[x][y] == g_board[x+1][y] &&
                g_board[x][y] == g_board[x+2][y]) {
                i = x;
                while (i < BS && g_board[i][y] == g_board[x][y]) {
                    g_mrk[i][y] = 1;
                    i++;
                }
            }
        }
    }

    for (x = 0; x < BS; x++) {
        for (y = 0; y < BS; y++) {
            if (y < BS - 2 &&
                g_board[x][y] == g_board[x][y+1] &&
                g_board[x][y] == g_board[x][y+2]) {
                i = y;
                while (i < BS && g_board[x][i] == g_board[x][y]) {
                    g_mrk[x][i] = 1;
                    i++;
                }
            }
        }
    }
}

static uint16_t clear_marked(void)
{
    uint8_t x;
    uint8_t y;
    uint16_t n;

    n = 0;
    for (y = 0; y < BS; y++) {
        for (x = 0; x < BS; x++) {
            if (g_mrk[x][y]) {
                g_board[x][y] = 255;
                n++;
            }
        }
    }
    return n;
}

static void show_explosion(void)
{
    uint8_t x;
    uint8_t y;
    uint8_t q;
    uint8_t sx;
    uint8_t sy;
    uint8_t dx;
    uint8_t dy;

    for (y = 0; y < BS; y++) {
        for (x = 0; x < BS; x++) {
            if (g_mrk[x][y]) {
                sx = (uint8_t)(x * TW + BXOFF);
                sy = (uint8_t)(y * TH + BYOFF);
                for (q = 0; q < 4; q++) {
                    dx = (uint8_t)(sx + (q % 2));
                    dy = (uint8_t)(sy + (q / 2));
                    _XL_DRAW(dx, dy, _TILE_26, _XL_WHITE);
                    g_disp_tile[x][y][q] = _TILE_26;
                    g_disp_col[x][y] = _XL_WHITE;
                }
            }
        }
    }
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 3);
}

static void apply_gravity(void)
{
    uint8_t x;
    uint8_t y;
    uint8_t moved;

    draw_all();
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 2);

    do {
        moved = 0;
        for (x = 0; x < BS; x++) {
            for (y = BS - 1; y > 0; y--) {
                if (g_board[x][y] == 255 && g_board[x][y - 1] != 255) {
                    g_board[x][y] = g_board[x][y - 1];
                    g_board[x][y - 1] = 255;
                    moved = 1;
                }
            }
        }
        if (moved) {
            draw_all();
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    } while (moved);
}

static void fill_top(void)
{
    uint8_t x;
    uint8_t y;
    uint16_t r;

    for (x = 0; x < BS; x++) {
        for (y = 0; y < BS; y++) {
            if (g_board[x][y] == 255) {
                r = _XL_RAND();
                g_board[x][y] = (uint8_t)(r % NC);
            }
        }
    }
}

static uint8_t has_valid_move(void)
{
    uint8_t x;
    uint8_t y;
    uint8_t tmp;
    uint8_t found;

    for (y = 0; y < BS; y++) {
        for (x = 0; x < BS - 1; x++) {
            tmp = g_board[x][y];
            g_board[x][y] = g_board[x+1][y];
            g_board[x+1][y] = tmp;
            found = makes_match(x, y) || makes_match(x+1, y);
            tmp = g_board[x][y];
            g_board[x][y] = g_board[x+1][y];
            g_board[x+1][y] = tmp;
            if (found) return 1;
        }
    }
    for (x = 0; x < BS; x++) {
        for (y = 0; y < BS - 1; y++) {
            tmp = g_board[x][y];
            g_board[x][y] = g_board[x][y+1];
            g_board[x][y+1] = tmp;
            found = makes_match(x, y) || makes_match(x, y+1);
            tmp = g_board[x][y];
            g_board[x][y] = g_board[x][y+1];
            g_board[x][y+1] = tmp;
            if (found) return 1;
        }
    }
    return 0;
}

static void resolve_matches(void)
{
    uint16_t n;
    uint8_t cascade;

    cascade = 1;
    while (cascade) {
        cascade = 0;
        mark_matches();
        n = clear_marked();
        if (n > 0) {
            show_explosion();
            g_score += n * 10;
            cascade = 1;
            _XL_EXPLOSION_SOUND();
            apply_gravity();
            fill_top();
        }
    }
    draw_all();
    show_score();
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 4);
}

static void swap_cells(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    uint8_t tmp;
    tmp = g_board[x1][y1];
    g_board[x1][y1] = g_board[x2][y2];
    g_board[x2][y2] = tmp;
}

static void try_swap(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    uint8_t ok;

    swap_cells(x1, y1, x2, y2);
    _XL_SHOOT_SOUND();
    ok = makes_match(x1, y1) || makes_match(x2, y2);
    if (!ok) {
        swap_cells(x1, y1, x2, y2);
        _XL_TOCK_SOUND();
        draw_cell(x1, y1);
        draw_cell(x2, y2);
    } else {
        draw_cell(x1, y1);
        draw_cell(x2, y2);
        resolve_matches();
    }
}

static void init_board(void)
{
    uint8_t x;
    uint8_t y;
    uint16_t r;
    uint8_t attempts;

    for (y = 0; y < BS; y++) {
        for (x = 0; x < BS; x++) {
            attempts = 0;
            do {
                r = _XL_RAND();
                g_board[x][y] = (uint8_t)(r % NC);
                attempts++;
            } while (attempts < 50 && makes_match(x, y));
        }
    }
    if (!has_valid_move()) {
        init_board();
    }
}

int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    uint8_t input;
    uint8_t pcx;
    uint8_t pcy;
    uint8_t psel;
    uint8_t psx;
    uint8_t psy;
    uint8_t cx;
    uint8_t cy;

    for (;;) {
        g_score = 0;
        g_over = 0;
        g_cx = 0;
        g_cy = 0;
        g_sel = 0;
        g_sx = 0;
        g_sy = 0;

        init_board();
        clear_disp();

        _XL_CLEAR_SCREEN();
        cx = (uint8_t)((XSize - 12) / 2);
        cy = (uint8_t)((YSize - 8) / 2);
        _XL_SET_TEXT_COLOR(_XL_MAGENTA);
        _XL_PRINT(cx, cy, "CANDY CRUSH");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT((uint8_t)(cx - 4), (uint8_t)(cy + 2), "ARROWS:MOVE");
        _XL_PRINT((uint8_t)(cx - 4), (uint8_t)(cy + 4), "FIRE:SELECT");
        _XL_PRINT((uint8_t)(cx - 4), (uint8_t)(cy + 6), "ARROW:SWAP");
        _XL_SLEEP(2);
        _XL_CLEAR_SCREEN();

        draw_all();
        show_score();

        while (!g_over) {
            input = _XL_INPUT();

            pcx = g_cx;
            pcy = g_cy;
            psel = g_sel;
            psx = g_sx;
            psy = g_sy;

            if (!g_sel) {
                if (_XL_LEFT(input) && g_cx > 0) {
                    g_cx--;
                    _XL_TICK_SOUND();
                } else if (_XL_RIGHT(input) && g_cx < BS - 1) {
                    g_cx++;
                    _XL_TICK_SOUND();
                } else if (_XL_UP(input) && g_cy > 0) {
                    g_cy--;
                    _XL_TICK_SOUND();
                } else if (_XL_DOWN(input) && g_cy < BS - 1) {
                    g_cy++;
                    _XL_TICK_SOUND();
                }
                if (_XL_FIRE(input)) {
                    g_sel = 1;
                    g_sx = g_cx;
                    g_sy = g_cy;
                    _XL_PING_SOUND();
                }
            } else {
                if (_XL_FIRE(input)) {
                    g_sel = 0;
                    _XL_TICK_SOUND();
                } else if (_XL_LEFT(input) && g_cx > 0) {
                    g_cx--;
                    try_swap(g_sx, g_sy, g_cx, g_cy);
                    g_sel = 0;
                    g_sx = g_cx;
                    g_sy = g_cy;
                    if (!has_valid_move()) g_over = 1;
                } else if (_XL_RIGHT(input) && g_cx < BS - 1) {
                    g_cx++;
                    try_swap(g_sx, g_sy, g_cx, g_cy);
                    g_sel = 0;
                    g_sx = g_cx;
                    g_sy = g_cy;
                    if (!has_valid_move()) g_over = 1;
                } else if (_XL_UP(input) && g_cy > 0) {
                    g_cy--;
                    try_swap(g_sx, g_sy, g_cx, g_cy);
                    g_sel = 0;
                    g_sx = g_cx;
                    g_sy = g_cy;
                    if (!has_valid_move()) g_over = 1;
                } else if (_XL_DOWN(input) && g_cy < BS - 1) {
                    g_cy++;
                    try_swap(g_sx, g_sy, g_cx, g_cy);
                    g_sel = 0;
                    g_sx = g_cx;
                    g_sy = g_cy;
                    if (!has_valid_move()) g_over = 1;
                }
            }

            if (g_cx != pcx || g_cy != pcy ||
                g_sel != psel || g_sx != psx || g_sy != psy) {
                draw_cell(pcx, pcy);
                draw_cell(g_cx, g_cy);
                draw_cell(psx, psy);
                draw_cell(g_sx, g_sy);
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        _XL_CLEAR_SCREEN();
        cx = (uint8_t)((XSize - 12) / 2);
        cy = (uint8_t)((YSize - 8) / 2);
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(cx, cy, "GAME OVER");
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT((uint8_t)(cx - 2), (uint8_t)(cy + 2), "SCORE");
        _XL_PRINTD((uint8_t)(cx + 3), (uint8_t)(cy + 2), 1, g_score);
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT((uint8_t)(cx - 6), (uint8_t)(cy + 4), "PRESS ANY KEY");
        _XL_WAIT_FOR_INPUT();
    }

    return 0;
}