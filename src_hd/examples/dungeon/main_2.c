#include "cross_lib.h"

/* ===== Constants ===== */
#define MAX_LEVELS          42
#define MAX_ENEMIES         8
#define MAX_BULLETS         4
#define GUN_COOLDOWN_BASE   30
#define SHIELD_DURATION     60
#define ENEMY_MOVE_INTERVAL 8

/* Directions */
#define DIR_UP    0
#define DIR_RIGHT 1
#define DIR_DOWN  2
#define DIR_LEFT  3

/* Game states */
#define STATE_PLAYING   0
#define STATE_GAME_OVER 1
#define STATE_WIN       2

/* Grid cell values */
#define CELL_EMPTY        0
#define CELL_WALL         1
#define CELL_KEY          2
#define CELL_DOOR_CLOSED  3
#define CELL_GUN_ITEM     4
#define CELL_SHIELD       5
#define CELL_DOOR_OPEN    6

/* Tile IDs (_TILE_0 .. _TILE_26) */
#define T_FLOOR           _TILE_0
#define T_WALL            _TILE_1
#define T_PLAYER          _TILE_2
#define T_ENEMY           _TILE_3
#define T_KEY             _TILE_4
#define T_DOOR_CLOSED     _TILE_5
#define T_GUN_ITEM        _TILE_6
#define T_SHIELD_ITEM     _TILE_7
#define T_BULLET          _TILE_8
#define T_DOOR_OPEN       _TILE_9

/* ===== Screen buffer ===== */
static uint8_t sbuf_tile[XSize][YSize];
static uint8_t sbuf_color[XSize][YSize];

/* ===== Game state ===== */
static uint8_t  cur_level;
static uint8_t  px, py;
static uint8_t  facing;
static uint8_t  has_gun;
static uint16_t gun_cd;
static uint8_t  shield_timer;
static uint8_t  game_state;
static uint16_t score;

/* Level grid */
static uint8_t  grid[XSize][YSize];

/* Enemies */
static uint8_t  ex[MAX_ENEMIES], ey[MAX_ENEMIES];
static uint8_t  enemy_count;

/* Bullets */
static uint8_t  bx[MAX_BULLETS], by[MAX_BULLETS];
static uint8_t  bdir[MAX_BULLETS];
static uint8_t  bactive[MAX_BULLETS];

/* ===== Helpers ===== */

static void buf_clear(void)
{
    uint8_t x, y;
    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            sbuf_tile[x][y]  = 255;
            sbuf_color[x][y] = 255;
        }
    }
}

static void buf_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t col)
{
    if (sbuf_tile[x][y] != tile || sbuf_color[x][y] != col) {
        _XL_DRAW(x, y, tile, col);
        sbuf_tile[x][y]  = tile;
        sbuf_color[x][y] = col;
    }
}

/* Carve a 3x3 block — only converts WALL cells to EMPTY,
   so doors and items placed earlier are never overwritten. */
static void carve_3x3(uint8_t x, uint8_t y)
{
    uint8_t dx, dy;
    for (dy = 0; dy < 3; dy++) {
        for (dx = 0; dx < 3; dx++) {
            uint8_t cx = (uint8_t)(x + dx);
            uint8_t cy = (uint8_t)(y + dy);
            if (cx >= 1 && cx < XSize - 1 && cy >= 1 && cy < YSize - 1) {
                if (grid[cx][cy] == CELL_WALL) {
                    grid[cx][cy] = CELL_EMPTY;
                }
            }
        }
    }
}

static uint8_t place_item(uint8_t type)
{
    uint8_t i;
    for (i = 0; i < 200; i++) {
        uint8_t x = (uint8_t)(1 + _XL_RAND() % (XSize - 2));
        uint8_t y = (uint8_t)(1 + _XL_RAND() % (YSize - 2));
        if (grid[x][y] == CELL_EMPTY) {
            grid[x][y] = type;
            return 1;
        }
    }
    return 0;
}

static void place_enemies(uint8_t lvl)
{
    uint8_t i;
    enemy_count = 0;
    uint8_t num_e = (uint8_t)(2 + lvl % 4);
    if (num_e > MAX_ENEMIES) num_e = MAX_ENEMIES;

    uint8_t min_dist = (uint8_t)((XSize + YSize) / 3);

    for (i = 0; i < 500 && enemy_count < num_e; i++) {
        uint8_t x = (uint8_t)(1 + _XL_RAND() % (XSize - 2));
        uint8_t y = (uint8_t)(1 + _XL_RAND() % (YSize - 2));
        if (grid[x][y] == CELL_EMPTY) {
            uint8_t dx = (x > px) ? x - px : px - x;
            uint8_t dy = (y > py) ? y - py : py - y;
            if (dx + dy >= min_dist) {
                ex[enemy_count] = x;
                ey[enemy_count] = y;
                enemy_count++;
            }
        }
    }
}

/* ===== Level type: maze with 3-wide corridors ===== */

static void gen_maze_level(uint8_t lvl)
{
    uint8_t x, y, i, j;
    uint16_t r;
    uint8_t cx, cy;
    uint8_t tx = (uint8_t)(XSize - 4);
    uint8_t ty = (uint8_t)(YSize - 4);

    for (y = 0; y < YSize; y++)
        for (x = 0; x < XSize; x++)
            grid[x][y] = CELL_WALL;

    px = 1; py = 1;
    carve_3x3(px, py);

    cx = px; cy = py;
    uint8_t steps = 0;
    uint16_t max_steps = (uint16_t)(XSize * YSize);

    while ((cx != tx || cy != ty) && steps < max_steps) {
        r = _XL_RAND();
        uint8_t nx = cx, ny = cy;

        if (r % 2 == 0) {
            if (cx < tx && (r % 3 != 1))       nx = (uint8_t)(cx + 1);
            else if (cy < ty)                  ny = (uint8_t)(cy + 1);
            else if (cx > tx)                  nx = (uint8_t)(cx - 1);
            else if (cy > ty)                  ny = (uint8_t)(cy - 1);
        } else {
            uint8_t d = (uint8_t)(r % 4);
            if      (d == DIR_UP    && cy > 3)           ny = (uint8_t)(cy - 1);
            else if (d == DIR_RIGHT && cx < XSize - 4)   nx = (uint8_t)(cx + 1);
            else if (d == DIR_DOWN  && cy < YSize - 4)   ny = (uint8_t)(cy + 1);
            else if (d == DIR_LEFT  && cx > 3)           nx = (uint8_t)(cx - 1);
        }

        if (nx == cx && ny == cy) {
            if (cx < tx)      nx = (uint8_t)(cx + 1);
            else if (cy < ty) ny = (uint8_t)(cy + 1);
            else if (cx > tx) nx = (uint8_t)(cx - 1);
            else if (cy > ty) ny = (uint8_t)(cy - 1);
        }

        cx = nx; cy = ny;
        carve_3x3(cx, cy);
        steps++;
    }

    grid[cx][cy] = CELL_DOOR_CLOSED;

    for (i = 0; i < XSize / 4 + YSize / 5; i++) {
        uint8_t sx = (uint8_t)(1 + _XL_RAND() % (XSize - 4));
        uint8_t sy = (uint8_t)(1 + _XL_RAND() % (YSize - 4));
        if (grid[sx][sy] == CELL_EMPTY) {
            for (j = 0; j < 2 + (uint8_t)(_XL_RAND() % 3); j++) {
                uint8_t d = (uint8_t)(_XL_RAND() % 4);
                if      (d == DIR_UP    && sy > 3)           sy--;
                else if (d == DIR_RIGHT && sx < XSize - 4)   sx++;
                else if (d == DIR_DOWN  && sy < YSize - 4)   sy++;
                else if (d == DIR_LEFT  && sx > 3)           sx--;
                carve_3x3(sx, sy);
            }
        }
    }

    place_item(CELL_KEY);
    place_item(CELL_GUN_ITEM);
    place_item(CELL_SHIELD);
    place_enemies(lvl);

    for (i = 0; i < MAX_BULLETS; i++) bactive[i] = 0;
    gun_cd = GUN_COOLDOWN_BASE + lvl * 2;
}

/* ===== Level type: large open room with central square walls ===== */

static void gen_open_room(uint8_t lvl)
{
    uint8_t x, y, i;

    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            if (x == 0 || y == 0 || x == XSize - 1 || y == YSize - 1)
                grid[x][y] = CELL_WALL;
            else
                grid[x][y] = CELL_EMPTY;
        }
    }

    uint8_t num_blocks = (uint8_t)(1 + _XL_RAND() % 2);
    for (i = 0; i < num_blocks; i++) {
        uint8_t sz = (uint8_t)(3 + _XL_RAND() % 3);
        uint8_t bx = (uint8_t)((XSize / 4) + _XL_RAND() % (XSize / 2));
        uint8_t by = (uint8_t)((YSize / 4) + _XL_RAND() % (YSize / 2));

        if (bx + sz >= XSize - 1) bx = (uint8_t)(XSize - 2 - sz);
        if (by + sz >= YSize - 1) by = (uint8_t)(YSize - 2 - sz);

        for (y = by; y < by + sz && y < YSize - 1; y++) {
            for (x = bx; x < bx + sz && x < XSize - 1; x++) {
                grid[x][y] = CELL_WALL;
            }
        }
    }

    px = 1; py = 1;
    grid[XSize - 2][YSize - 2] = CELL_DOOR_CLOSED;

    place_item(CELL_KEY);
    place_item(CELL_GUN_ITEM);
    place_item(CELL_SHIELD);
    place_enemies(lvl);

    for (i = 0; i < MAX_BULLETS; i++) bactive[i] = 0;
    gun_cd = GUN_COOLDOWN_BASE + lvl * 2;
}

/* ===== Level type: 16x16 flat room, no internal walls ===== */

static void gen_flat_16x16(uint8_t lvl)
{
    uint8_t x, y, i;

    uint8_t rw = 16;
    uint8_t rh = 16;
    if (rw > XSize - 2) rw = (uint8_t)(XSize - 2);
    if (rh > YSize - 2) rh = (uint8_t)(YSize - 2);

    uint8_t ox = (uint8_t)((XSize - rw) / 2);
    uint8_t oy = (uint8_t)((YSize - rh) / 2);

    for (y = 0; y < YSize; y++)
        for (x = 0; x < XSize; x++)
            grid[x][y] = CELL_WALL;

    for (y = oy + 1; y <= oy + rh - 1 && y < YSize - 1; y++) {
        for (x = ox + 1; x <= ox + rw - 1 && x < XSize - 1; x++) {
            grid[x][y] = CELL_EMPTY;
        }
    }

    px = (uint8_t)(ox + 1);
    py = (uint8_t)(oy + 1);
    grid[ox + rw - 1][oy + rh - 1] = CELL_DOOR_CLOSED;

    place_item(CELL_KEY);
    place_item(CELL_GUN_ITEM);
    place_item(CELL_SHIELD);
    place_enemies(lvl);

    for (i = 0; i < MAX_BULLETS; i++) bactive[i] = 0;
    gun_cd = GUN_COOLDOWN_BASE + lvl * 2;
}

/* ===== Level dispatcher ===== */

static void gen_level(uint8_t lvl)
{
    uint8_t type = (uint8_t)(lvl % 4);

    if (type == 0) {
        gen_open_room(lvl);
    } else if (type == 2) {
        gen_flat_16x16(lvl);
    } else {
        gen_maze_level(lvl);
    }
}

/* ===== Rendering ===== */

static void render(void)
{
    uint8_t x, y, i;
    uint8_t pcol; 
    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            switch (grid[x][y]) {
                case CELL_WALL:
                    buf_draw(x, y, T_WALL, _XL_BLUE);
                    break;
                case CELL_EMPTY:
                    buf_draw(x, y, T_FLOOR, _XL_WHITE);
                    break;
                case CELL_KEY:
                    buf_draw(x, y, T_KEY, _XL_YELLOW);
                    break;
                case CELL_DOOR_CLOSED:
                    buf_draw(x, y, T_DOOR_CLOSED, _XL_MAGENTA);
                    break;
                case CELL_DOOR_OPEN:
                    buf_draw(x, y, T_DOOR_OPEN, _XL_GREEN);
                    break;
                case CELL_GUN_ITEM:
                    buf_draw(x, y, T_GUN_ITEM, _XL_CYAN);
                    break;
                case CELL_SHIELD:
                    buf_draw(x, y, T_SHIELD_ITEM, _XL_GREEN);
                    break;
            }
        }
    }

    for (i = 0; i < enemy_count; i++) {
        col = (shield_timer > 0) ? _XL_CYAN : _XL_RED;
        buf_draw(ex[i], ey[i], T_ENEMY, col);
    }

    for (i = 0; i < MAX_BULLETS; i++) {
        if (bactive[i]) {
            buf_draw(bx[i], by[i], T_BULLET, _XL_YELLOW);
        }
    }

    /* Player is always cyan */
    uint8_t pcol = (shield_timer > 0) ? _XL_YELLOW : _XL_CYAN;
    buf_draw(px, py, T_PLAYER, pcol);

    /* HUD */
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    char hud[16];
    uint8_t lv = cur_level;
    hud[0] = 'L'; hud[1] = 'V'; hud[2] = 'L'; hud[3] = ' ';
    if (lv >= 10) {
        hud[4] = (char)('0' + lv / 10);
        hud[5] = (char)('0' + lv % 10);
        hud[6] = '\0';
    } else {
        hud[4] = ' ';
        hud[5] = (char)('0' + lv);
        hud[6] = '\0';
    }
    _XL_PRINT(0, 0, hud);

    char sc[12];
    uint16_t s = score;
    sc[0] = 'S'; sc[1] = ':'; sc[2] = ' ';
    if (s >= 1000) {
        sc[3] = (char)('0' + s / 1000);
        sc[4] = (char)('0' + (s / 100) % 10);
        sc[5] = (char)('0' + (s / 10) % 10);
        sc[6] = (char)('0' + s % 10);
        sc[7] = '\0';
    } else if (s >= 100) {
        sc[3] = ' ';
        sc[4] = (char)('0' + s / 100);
        sc[5] = (char)('0' + (s / 10) % 10);
        sc[6] = (char)('0' + s % 10);
        sc[7] = '\0';
    } else if (s >= 10) {
        sc[3] = ' '; sc[4] = ' ';
        sc[5] = (char)('0' + s / 10);
        sc[6] = (char)('0' + s % 10);
        sc[7] = '\0';
    } else {
        sc[3] = ' '; sc[4] = ' '; sc[5] = ' ';
        sc[6] = (char)('0' + s);
        sc[7] = '\0';
    }
    _XL_PRINT(8, 0, sc);

    if (has_gun && gun_cd == 0) {
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(XSize - 6, 0, "GUN");
    } else if (has_gun) {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(XSize - 6, 0, "CD ");
    }

    if (shield_timer > 0) {
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(18, 0, "SHIELD");
    }
}

/* ===== Game logic ===== */

static void move_enemies(void)
{
    uint8_t i;
    for (i = 0; i < enemy_count; i++) {
        int16_t dx = (int16_t)px - (int16_t)ex[i];
        int16_t dy = (int16_t)py - (int16_t)ey[i];

        uint8_t nx = ex[i], ny = ey[i];
        if (dx > 0)      nx++;
        else if (dx < 0) nx--;
        else if (dy > 0) ny++;
        else if (dy < 0) ny--;

        if (nx >= 1 && nx < XSize - 1 && ny >= 1 && ny < YSize - 1) {
            if (grid[nx][ny] == CELL_EMPTY) {
                ex[i] = nx;
                ey[i] = ny;
            }
        }

        if (ex[i] == px && ey[i] == py) {
            if (shield_timer > 0) {
                for (; i < enemy_count; i++) {
                    ex[i] = ex[i + 1];
                    ey[i] = ey[i + 1];
                }
                enemy_count--;
                score += 50;
            } else {
                game_state = STATE_GAME_OVER;
            }
        }
    }
}

static void update_bullets(void)
{
    uint8_t i, j;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bactive[i]) continue;

        uint8_t nx = bx[i], ny = by[i];
        switch (bdir[i]) {
            case DIR_UP:    if (ny > 0) ny--; break;
            case DIR_RIGHT: if (nx < XSize - 1) nx++; break;
            case DIR_DOWN:  if (ny < YSize - 1) ny++; break;
            case DIR_LEFT:  if (nx > 0) nx--; break;
        }

        uint8_t cell = grid[nx][ny];
        if (cell == CELL_WALL || cell == CELL_DOOR_CLOSED || cell == CELL_DOOR_OPEN) {
            bactive[i] = 0;
            continue;
        }

        bx[i] = nx; by[i] = ny;

        for (j = 0; j < enemy_count; j++) {
            if (bx[i] == ex[j] && by[i] == ey[j]) {
                bactive[i] = 0;
                for (; j < enemy_count; j++) {
                    ex[j] = ex[j + 1];
                    ey[j] = ey[j + 1];
                }
                enemy_count--;
                score += 20;
                _XL_PING_SOUND();
                break;
            }
        }
    }
}

static void fire_bullet(void)
{
    uint8_t i;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bactive[i]) {
            bx[i] = px; by[i] = py;
            bdir[i] = facing;
            bactive[i] = 1;
            _XL_SHOOT_SOUND();
            return;
        }
    }
}

static void check_pickups(void)
{
    uint8_t cell = grid[px][py];

    if (cell == CELL_KEY) {
        uint8_t x, y;
        for (y = 0; y < YSize; y++) {
            for (x = 0; x < XSize; x++) {
                if (grid[x][y] == CELL_DOOR_CLOSED) {
                    grid[x][y] = CELL_DOOR_OPEN;
                }
            }
        }
        grid[px][py] = CELL_EMPTY;
        score += 10;
        _XL_TOCK_SOUND();
    } else if (cell == CELL_GUN_ITEM) {
        has_gun = 1;
        gun_cd = GUN_COOLDOWN_BASE + cur_level * 2;
        grid[px][py] = CELL_EMPTY;
        _XL_TICK_SOUND();
    } else if (cell == CELL_SHIELD) {
        shield_timer = SHIELD_DURATION;
        grid[px][py] = CELL_EMPTY;
        _XL_PING_SOUND();
    } else if (cell == CELL_DOOR_OPEN) {
        cur_level++;
        if (cur_level > MAX_LEVELS) {
            game_state = STATE_WIN;
        } else {
            gen_level(cur_level);
            buf_clear();
            _XL_CLEAR_SCREEN();
        }
    }
}

static void try_move(uint8_t dir)
{
    uint8_t nx = px, ny = py;
    facing = dir;

    switch (dir) {
        case DIR_UP:    if (py > 0) ny--; break;
        case DIR_RIGHT: if (px < XSize - 1) nx++; break;
        case DIR_DOWN:  if (py < YSize - 1) ny++; break;
        case DIR_LEFT:  if (px > 0) nx--; break;
    }

    uint8_t cell = grid[nx][ny];
    if (cell == CELL_WALL || cell == CELL_DOOR_CLOSED) return;

    px = nx; py = ny;
    check_pickups();
}

/* ===== Main ===== */

int main(void)
{
    uint8_t input;
    uint8_t frame;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        cur_level = 1;
        score = 0;
        has_gun = 0;
        shield_timer = 0;
        facing = DIR_RIGHT;
        game_state = STATE_PLAYING;

        gen_level(cur_level);
        buf_clear();
        _XL_CLEAR_SCREEN();

        while (game_state == STATE_PLAYING) {
            frame++;

            input = _XL_INPUT();

            if (_XL_LEFT(input))       try_move(DIR_LEFT);
            else if (_XL_RIGHT(input)) try_move(DIR_RIGHT);
            else if (_XL_UP(input))    try_move(DIR_UP);
            else if (_XL_DOWN(input))  try_move(DIR_DOWN);

            if (has_gun && gun_cd == 0) {
                if (_XL_FIRE(input)) {
                    fire_bullet();
                    gun_cd = GUN_COOLDOWN_BASE + cur_level * 2;
                }
            }

            if (gun_cd > 0) gun_cd--;
            if (shield_timer > 0) shield_timer--;

            if ((frame % ENEMY_MOVE_INTERVAL) == 0) {
                move_enemies();
            }

            update_bullets();

            render();

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        buf_clear();
        _XL_CLEAR_SCREEN();

        if (game_state == STATE_WIN) {
            _XL_SET_TEXT_COLOR(_XL_YELLOW);
            _XL_PRINT(4, 8, "YOU WIN!");
        } else {
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(4, 8, "GAME OVER");
        }

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        char sc[16];
        uint16_t s = score;
        sc[0] = 'S'; sc[1] = 'C'; sc[2] = 'O'; sc[3] = 'R';
        sc[4] = 'E'; sc[5] = ':'; sc[6] = ' ';
        if (s >= 1000) {
            sc[7]  = (char)('0' + s / 1000);
            sc[8]  = (char)('0' + (s / 100) % 10);
            sc[9]  = (char)('0' + (s / 10) % 10);
            sc[10] = (char)('0' + s % 10);
        } else if (s >= 100) {
            sc[7]  = ' ';
            sc[8]  = (char)('0' + s / 100);
            sc[9]  = (char)('0' + (s / 10) % 10);
            sc[10] = (char)('0' + s % 10);
        } else if (s >= 10) {
            sc[7] = ' '; sc[8] = ' ';
            sc[9]  = (char)('0' + s / 10);
            sc[10] = (char)('0' + s % 10);
        } else {
            sc[7] = ' '; sc[8] = ' '; sc[9] = ' ';
            sc[10] = (char)('0' + s);
        }
        sc[11] = '\0';
        _XL_PRINT(4, 10, sc);

        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(2, 14, "PRESS ANY KEY TO RESTART");

        _XL_WAIT_FOR_INPUT();
    }

    return 0;
}
