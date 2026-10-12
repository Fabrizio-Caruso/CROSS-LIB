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
static uint8_t  s_reload;

/* Level grid */
static uint8_t  grid[XSize][YSize];

/* Enemies */
static uint8_t  ex[MAX_ENEMIES], ey[MAX_ENEMIES];
static uint8_t  enemy_count;

/* Bullets */
static uint8_t  bx[MAX_BULLETS], by[MAX_BULLETS];
static uint8_t  bdir[MAX_BULLETS];
static uint8_t  bactive[MAX_BULLETS];

/* ===== Drawing helpers ===== */

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
    if (x >= XSize || y >= YSize) {
        return;
    }

    if (sbuf_tile[x][y] != tile || sbuf_color[x][y] != col) {
        _XL_DRAW(x, y, tile, col);
        sbuf_tile[x][y]  = tile;
        sbuf_color[x][y] = col;
    }
}

static void draw_static_cell(uint8_t x, uint8_t y)
{
    uint8_t cell;

    if (x >= XSize || y >= YSize) {
        return;
    }

    cell = grid[x][y];

    switch (cell) {
        case CELL_WALL:
            buf_draw(x, y, T_WALL, _XL_BLUE);
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
        default:
            buf_draw(x, y, T_FLOOR, _XL_WHITE);
            break;
    }
}

static void draw_grid(void)
{
    uint8_t x, y;

    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            draw_static_cell(x, y);
        }
    }
}

static void draw_enemy(uint8_t i)
{
    if (i >= enemy_count) {
        return;
    }

    buf_draw(ex[i], ey[i], T_ENEMY, _XL_RED);
}

static void draw_bullet(uint8_t i)
{
    if (i >= MAX_BULLETS || !bactive[i]) {
        return;
    }

    buf_draw(bx[i], by[i], T_BULLET, _XL_YELLOW);
}

static void draw_player(void)
{
    uint8_t col;

    if (shield_timer > 0) {
        col = _XL_YELLOW;
    } else {
        col = _XL_CYAN;
    }

    buf_draw(px, py, T_PLAYER, col);
}

static void draw_hud(void)
{
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(0, 0, "LV");
    _XL_PRINTD(3, 0, 2, cur_level);

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    if (XSize >= 7) {
        _XL_PRINT(0, YSize - 1, "S");
        _XL_PRINTD(2, YSize - 1, 5, score);
    } else {
        _XL_PRINTD(1, YSize - 1, 5, score);
    }

    if (has_gun) {
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(XSize - 1, 0, "G");
    } else {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(XSize - 1, 0, " ");
    }
}

/* ===== Entity helpers ===== */

static uint8_t find_enemy(uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < enemy_count; i++) {
        if (ex[i] == x && ey[i] == y) {
            return i;
        }
    }

    return 255;
}

static void remove_enemy(uint8_t idx)
{
    uint8_t j;

    if (idx >= enemy_count) {
        return;
    }

    for (j = idx; j < enemy_count - 1; j++) {
        ex[j] = ex[j + 1];
        ey[j] = ey[j + 1];
    }

    enemy_count--;
}

static uint8_t has_bullet_at(uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (bactive[i] && bx[i] == x && by[i] == y) {
            return 1;
        }
    }

    return 0;
}

static uint8_t has_enemy_at(uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < enemy_count; i++) {
        if (ex[i] == x && ey[i] == y) {
            return 1;
        }
    }

    return 0;
}

/* ===== Level helpers ===== */

static uint8_t place_item(uint8_t type)
{
    uint16_t i;
    uint8_t x, y;

    for (i = 0; i < 200; i++) {
        x = (uint8_t)(1 + _XL_RAND() % (XSize - 2));
        y = (uint8_t)(1 + _XL_RAND() % (YSize - 2));

        if (grid[x][y] == CELL_EMPTY && !(x == px && y == py)) {
            grid[x][y] = type;
            return 1;
        }
    }

    for (y = 1; y < YSize - 1; y++) {
        for (x = 1; x < XSize - 1; x++) {
            if (grid[x][y] == CELL_EMPTY && !(x == px && y == py)) {
                grid[x][y] = type;
                return 1;
            }
        }
    }

    return 0;
}

static void place_enemies(uint8_t lvl)
{
    uint16_t i;
    uint8_t x, y;
    uint16_t dx, dy;
    uint16_t min_dist;
    uint8_t num_e;

    enemy_count = 0;

    if (XSize < 3 || YSize < 3) {
        return;
    }

    num_e = (uint8_t)(2 + (lvl % 4));
    if (num_e > MAX_ENEMIES) {
        num_e = MAX_ENEMIES;
    }

    min_dist = (uint16_t)((XSize + YSize) / 3);

    /* First pass: try to keep enemies away from the player. */
    for (i = 0; i < 300 && enemy_count < num_e; i++) {
        x = (uint8_t)(1 + _XL_RAND() % (XSize - 2));
        y = (uint8_t)(1 + _XL_RAND() % (YSize - 2));

        if (grid[x][y] == CELL_EMPTY &&
            !has_enemy_at(x, y) &&
            !(x == px && y == py)) {

            dx = (uint16_t)((x > px) ? (x - px) : (px - x));
            dy = (uint16_t)((y > py) ? (y - py) : (py - y));

            if (dx + dy >= min_dist) {
                ex[enemy_count] = x;
                ey[enemy_count] = y;
                enemy_count++;
            }
        }
    }

    /* Second pass: if not enough room for the distance requirement,
       place in any free reachable-looking cell. */
    for (i = 0; i < 300 && enemy_count < num_e; i++) {
        x = (uint8_t)(1 + _XL_RAND() % (XSize - 2));
        y = (uint8_t)(1 + _XL_RAND() % (YSize - 2));

        if (grid[x][y] == CELL_EMPTY &&
            !has_enemy_at(x, y) &&
            !(x == px && y == py)) {

            ex[enemy_count] = x;
            ey[enemy_count] = y;
            enemy_count++;
        }
    }

    /* Final deterministic pass. */
    for (y = 1; y < YSize - 1; y++) {
        for (x = 1; x < XSize - 1; x++) {
            if (enemy_count >= num_e) {
                break;
            }

            if (grid[x][y] == CELL_EMPTY &&
                !has_enemy_at(x, y) &&
                !(x == px && y == py)) {

                ex[enemy_count] = x;
                ey[enemy_count] = y;
                enemy_count++;
            }
        }

        if (enemy_count >= num_e) {
            break;
        }
    }
}

static void carve_3x3(uint8_t x, uint8_t y)
{
    uint8_t dx, dy;
    uint8_t cx, cy;

    for (dy = 0; dy < 3; dy++) {
        for (dx = 0; dx < 3; dx++) {
            cx = (uint8_t)(x + dx);
            cy = (uint8_t)(y + dy);

            if (cx >= 1 && cx < XSize - 1 && cy >= 1 && cy < YSize - 1) {
                if (grid[cx][cy] == CELL_WALL) {
                    grid[cx][cy] = CELL_EMPTY;
                }
            }
        }
    }
}

/* ===== Level type: maze-like room with guaranteed path ===== */
static uint8_t maze_reach[XSize][YSize];
static uint8_t maze_visited[XSize][YSize];
static uint8_t maze_stack_x[XSize * YSize];
static uint8_t maze_stack_y[XSize * YSize];

static void maze_flood_fill(uint8_t sx, uint8_t sy)
{
    static uint8_t seen[XSize][YSize];
    uint8_t x, y, nx, ny;
    uint16_t top;

    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            seen[x][y] = 0;
            maze_reach[x][y] = 0;
        }
    }

    if (sx >= XSize || sy >= YSize) {
        return;
    }

    if (grid[sx][sy] != CELL_EMPTY) {
        return;
    }

    top = 0;

    seen[sx][sy] = 1;
    maze_reach[sx][sy] = 1;

    maze_stack_x[top] = sx;
    maze_stack_y[top] = sy;
    top++;

    while (top > 0) {
        top--;

        x = maze_stack_x[top];
        y = maze_stack_y[top];

        if (y > 0) {
            nx = x;
            ny = y - 1;

            if (!seen[nx][ny] && grid[nx][ny] == CELL_EMPTY) {
                seen[nx][ny] = 1;
                maze_reach[nx][ny] = 1;

                maze_stack_x[top] = nx;
                maze_stack_y[top] = ny;
                top++;
            }
        }

        if (x < XSize - 1) {
            nx = x + 1;
            ny = y;

            if (!seen[nx][ny] && grid[nx][ny] == CELL_EMPTY) {
                seen[nx][ny] = 1;
                maze_reach[nx][ny] = 1;

                maze_stack_x[top] = nx;
                maze_stack_y[top] = ny;
                top++;
            }
        }

        if (y < YSize - 1) {
            nx = x;
            ny = y + 1;

            if (!seen[nx][ny] && grid[nx][ny] == CELL_EMPTY) {
                seen[nx][ny] = 1;
                maze_reach[nx][ny] = 1;

                maze_stack_x[top] = nx;
                maze_stack_y[top] = ny;
                top++;
            }
        }

        if (x > 0) {
            nx = x - 1;
            ny = y;

            if (!seen[nx][ny] && grid[nx][ny] == CELL_EMPTY) {
                seen[nx][ny] = 1;
                maze_reach[nx][ny] = 1;

                maze_stack_x[top] = nx;
                maze_stack_y[top] = ny;
                top++;
            }
        }
    }
}


/* ===== Level type: large open room with optional wall blocks ===== */

static void gen_open_room(uint8_t lvl)
{
    uint8_t x, y, i;
    uint8_t num_blocks;
    uint8_t sz, bx, by;
    uint8_t dx, dy;
    uint8_t ok;

    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            if (x == 0 || y == 0 || x == XSize - 1 || y == YSize - 1) {
                grid[x][y] = CELL_WALL;
            } else {
                grid[x][y] = CELL_EMPTY;
            }
        }
    }

    num_blocks = 0;
    if (XSize > 10 && YSize > 10) {
        num_blocks = (uint8_t)(1 + _XL_RAND() % 2);
    }

    for (i = 0; i < num_blocks; i++) {
        sz = (uint8_t)(3 + _XL_RAND() % 3);
        ok = 1;

        if (sz >= (uint8_t)(XSize - 2) || sz >= (uint8_t)(YSize - 2)) {
            ok = 0;
        }

        if (ok) {
            bx = (uint8_t)(1 + _XL_RAND() % (XSize - sz - 1));
            by = (uint8_t)(1 + _XL_RAND() % (YSize - sz - 1));

            for (y = by; y < by + sz; y++) {
                for (x = bx; x < bx + sz; x++) {
                    grid[x][y] = CELL_WALL;
                }
            }
        }
    }

    dx = (uint8_t)(XSize - 3);
    if (dx < 1) {
        dx = 1;
    }
    if (dx > (uint8_t)(XSize - 2)) {
        dx = (uint8_t)(XSize - 2);
    }

    dy = (uint8_t)(YSize - 3);
    if (dy < 1) {
        dy = 1;
    }
    if (dy > (uint8_t)(YSize - 2)) {
        dy = (uint8_t)(YSize - 2);
    }

    px = 1;
    py = 1;

    grid[1][1] = CELL_EMPTY;
    grid[dx][dy] = CELL_DOOR_CLOSED;

    carve_3x3(1, 1);
    carve_3x3(dx, dy);

    place_item(CELL_KEY);
    place_item(CELL_GUN_ITEM);
    place_item(CELL_SHIELD);
    place_enemies(lvl);

    for (i = 0; i < MAX_BULLETS; i++) {
        bactive[i] = 0;
    }

    gun_cd = (uint16_t)(GUN_COOLDOWN_BASE + (uint8_t)(lvl * 2));
}


static void gen_maze_level(uint8_t lvl)
{
    uint8_t x, y, i, j;
    uint8_t sx, sy, nx, ny;
    uint8_t wx, wy;
    uint8_t tx, ty;
    uint8_t r, k, dir, found;
    uint8_t lmx, lmy, maxlx, maxly;
    uint8_t best_x, best_y;
    uint8_t placed;
    uint8_t cx, cy;
    uint16_t top;
    uint16_t best_dist, cur_dist;

    if (XSize < 5 || YSize < 5) {
        gen_open_room(lvl);
        return;
    }

    /*
        Logical maze size.
        Each logical cell becomes a 3x3 tile block.
    */
    lmx = (uint8_t)((XSize - 2) / 3);
    lmy = (uint8_t)((YSize - 2) / 3);

    if (lmx < 1 || lmy < 1) {
        gen_open_room(lvl);
        return;
    }

    /*
        Largest odd logical coordinate.
        Maze rooms are placed on odd logical coordinates,
        walls between rooms are on even logical coordinates.
    */
    maxlx = (uint8_t)(lmx - 1);
    if (maxlx > 0 && (maxlx & 1) == 0) {
        maxlx--;
    }

    maxly = (uint8_t)(lmy - 1);
    if (maxly > 0 && (maxly & 1) == 0) {
        maxly--;
    }

    /*
        Need at least two logical rooms somewhere.
        If you want a strict 2D maze only, use:
            if (maxlx < 3 || maxly < 3)
    */
    if (maxlx < 1 || maxly < 1 || (maxlx == 1 && maxly == 1)) {
        gen_open_room(lvl);
        return;
    }

    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            grid[x][y] = CELL_WALL;
        }
    }

    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            maze_visited[x][y] = 0;
        }
    }

    /*
        Start in the center of the first 3x3 room.
        Logical room (1,1) maps to top-left grid cell (4,4).
        Its center is (5,5).
    */
    px = 5;
    py = 5;

    maze_visited[1][1] = 1;
    carve_3x3(4, 4);

    top = 0;
    maze_stack_x[top] = 1;
    maze_stack_y[top] = 1;
    top++;

    /*
        Generate a normal 1-wide maze on the logical grid,
        but carve each logical open cell as a 3x3 block.
    */
    while (top > 0) {
        sx = maze_stack_x[top - 1];
        sy = maze_stack_y[top - 1];

        r = (uint8_t)(_XL_RAND() % 4);
        found = 0;

        for (k = 0; k < 4 && !found; k++) {
            dir = (uint8_t)((r + k) % 4);

            nx = sx;
            ny = sy;

            if (dir == DIR_UP) {
                if (sy > 1) {
                    ny = (uint8_t)(sy - 2);
                }
            } else if (dir == DIR_RIGHT) {
                if (sx + 2 <= maxlx) {
                    nx = (uint8_t)(sx + 2);
                }
            } else if (dir == DIR_DOWN) {
                if (sy + 2 <= maxly) {
                    ny = (uint8_t)(sy + 2);
                }
            } else {
                if (sx > 1) {
                    nx = (uint8_t)(sx - 2);
                }
            }

            if (nx <= maxlx && ny <= maxly && !maze_visited[nx][ny]) {
                wx = (uint8_t)((sx + nx) / 2);
                wy = (uint8_t)((sy + ny) / 2);

                /* Carve the 3x3 wall block between the two rooms. */
                tx = (uint8_t)(1 + 3 * wx);
                ty = (uint8_t)(1 + 3 * wy);
                carve_3x3(tx, ty);

                /* Carve the new 3x3 room. */
                tx = (uint8_t)(1 + 3 * nx);
                ty = (uint8_t)(1 + 3 * ny);
                carve_3x3(tx, ty);

                maze_visited[nx][ny] = 1;

                maze_stack_x[top] = nx;
                maze_stack_y[top] = ny;
                top++;

                found = 1;
            }
        }

        if (!found) {
            top--;
        }
    }

    /*
        Remove some 3x3 wall blocks to create loops.
        This makes corridors larger / easier.
    */
    for (j = 1; j <= maxly; j = (uint8_t)(j + 2)) {
        for (i = 1; i <= maxlx; i = (uint8_t)(i + 2)) {
            if (i + 2 <= maxlx) {
                wx = (uint8_t)(i + 1);
                wy = j;

                tx = (uint8_t)(1 + 3 * wx);
                ty = (uint8_t)(1 + 3 * wy);

                if (maze_visited[i][j] &&
                    maze_visited[i + 2][j] &&
                    tx + 1 < XSize - 1 &&
                    ty + 1 < YSize - 1 &&
                    grid[tx + 1][ty + 1] == CELL_WALL) {

                    if (_XL_RAND() % 3 == 0) {
                        carve_3x3(tx, ty);
                    }
                }
            }

            if (j + 2 <= maxly) {
                wx = i;
                wy = (uint8_t)(j + 1);

                tx = (uint8_t)(1 + 3 * wx);
                ty = (uint8_t)(1 + 3 * wy);

                if (maze_visited[i][j] &&
                    maze_visited[i][j + 2] &&
                    tx + 1 < XSize - 1 &&
                    ty + 1 < YSize - 1 &&
                    grid[tx + 1][ty + 1] == CELL_WALL) {

                    if (_XL_RAND() % 3 == 0) {
                        carve_3x3(tx, ty);
                    }
                }
            }
        }
    }

    /*
        Choose the exit from reachable 3x3 room centers.
    */
    maze_flood_fill(px, py);

    best_x = px;
    best_y = py;
    best_dist = 0;

    for (j = 1; j <= maxly; j = (uint8_t)(j + 2)) {
        for (i = 1; i <= maxlx; i = (uint8_t)(i + 2)) {
            cx = (uint8_t)(2 + 3 * i);
            cy = (uint8_t)(2 + 3 * j);

            if (cx >= XSize || cy >= YSize) {
                continue;
            }

            if (grid[cx][cy] == CELL_EMPTY && maze_reach[cx][cy]) {
                cur_dist = (uint16_t)(((cx > px) ? (cx - px) : (px - cx)) +
                                      ((cy > py) ? (cy - py) : (py - cy)));

                if (cur_dist > best_dist ||
                    (cur_dist == best_dist &&
                     ((uint16_t)cx + (uint16_t)cy) >
                     ((uint16_t)best_x + (uint16_t)best_y))) {

                    best_dist = cur_dist;
                    best_x = cx;
                    best_y = cy;
                }
            }
        }
    }

    if (best_x == px && best_y == py) {
        for (y = 1; y < YSize - 1; y++) {
            for (x = 1; x < XSize - 1; x++) {
                if (grid[x][y] == CELL_EMPTY &&
                    maze_reach[x][y] &&
                    !(x == px && y == py)) {

                    best_x = x;
                    best_y = y;
                    break;
                }
            }

            if (best_x != px || best_y != py) {
                break;
            }
        }
    }

    if (best_x == px && best_y == py) {
        gen_open_room(lvl);
        return;
    }

    grid[best_x][best_y] = CELL_DOOR_CLOSED;

    /*
        Flood fill again with the door in place.
        Then remove unreachable open cells so the level is solvable.
    */
    maze_flood_fill(px, py);

    for (y = 1; y < YSize - 1; y++) {
        for (x = 1; x < XSize - 1; x++) {
            if (grid[x][y] == CELL_EMPTY && !maze_reach[x][y]) {
                grid[x][y] = CELL_WALL;
            }
        }
    }

    maze_flood_fill(px, py);

    placed = 0;
    if (place_item(CELL_KEY)) {
        placed = 1;
    }

    if (!placed) {
        for (y = 1; y < YSize - 1; y++) {
            for (x = 1; x < XSize - 1; x++) {
                if (grid[x][y] == CELL_EMPTY &&
                    maze_reach[x][y] &&
                    !(x == px && y == py)) {

                    grid[x][y] = CELL_KEY;
                    placed = 1;
                    break;
                }
            }

            if (placed) {
                break;
            }
        }
    }

    if (!placed) {
        for (y = 0; y < YSize; y++) {
            for (x = 0; x < XSize; x++) {
                if (grid[x][y] == CELL_DOOR_CLOSED) {
                    grid[x][y] = CELL_DOOR_OPEN;
                }
            }
        }
    }

    place_item(CELL_GUN_ITEM);
    place_item(CELL_SHIELD);
    place_enemies(lvl);

    for (i = 0; i < MAX_BULLETS; i++) {
        bactive[i] = 0;
    }

    gun_cd = (uint16_t)(GUN_COOLDOWN_BASE + (uint8_t)(lvl * 2));
}



/* ===== Level type: flat centered room ===== */

static void gen_flat_16x16(uint8_t lvl)
{
    uint8_t x, y, i;
    uint8_t rw, rh;
    uint8_t ox, oy;
    uint8_t dx, dy;

    rw = 16;
    rh = 16;

    if (rw > (uint8_t)(XSize - 2)) {
        rw = (uint8_t)(XSize - 2);
    }
    if (rh > (uint8_t)(YSize - 2)) {
        rh = (uint8_t)(YSize - 2);
    }

    ox = (uint8_t)((XSize - rw) / 2);
    oy = (uint8_t)((YSize - rh) / 2);

    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            grid[x][y] = CELL_WALL;
        }
    }

    for (y = oy + 1; y <= oy + rh - 1 && y < YSize - 1; y++) {
        for (x = ox + 1; x <= ox + rw - 1 && x < XSize - 1; x++) {
            grid[x][y] = CELL_EMPTY;
        }
    }

    px = (uint8_t)(ox + 1);
    py = (uint8_t)(oy + 1);

    dx = (uint8_t)(ox + rw - 1);
    dy = (uint8_t)(oy + rh - 1);

    if (dx >= XSize - 1) {
        dx = (uint8_t)(XSize - 2);
    }
    if (dy >= YSize - 1) {
        dy = (uint8_t)(YSize - 2);
    }

    grid[px][py] = CELL_EMPTY;
    grid[dx][dy] = CELL_DOOR_CLOSED;

    place_item(CELL_KEY);
    place_item(CELL_GUN_ITEM);
    place_item(CELL_SHIELD);
    place_enemies(lvl);

    for (i = 0; i < MAX_BULLETS; i++) {
        bactive[i] = 0;
    }

    gun_cd = (uint16_t)(GUN_COOLDOWN_BASE + (uint8_t)(lvl * 2));
}



/* ===== Level dispatcher ===== */

static void gen_level(uint8_t lvl)
{
    uint8_t type;

    type = (uint8_t)(lvl % 4);

    if (type == 0) {
        gen_open_room(lvl);
    } else if (type == 2) {
        gen_flat_16x16(lvl);
    } else {
        gen_maze_level(lvl);
    }
}

static void load_level(uint8_t lvl)
{
    uint8_t i;

    gen_level(lvl);

    buf_clear();
    _XL_CLEAR_SCREEN();

    draw_grid();

    for (i = 0; i < enemy_count; i++) {
        draw_enemy(i);
    }

    draw_player();
    draw_hud();
}

/* ===== Game logic ===== */

static void check_pickups(void)
{
    uint8_t cell;
    uint8_t x, y;

    cell = grid[px][py];

    if (cell == CELL_KEY) {
        for (y = 0; y < YSize; y++) {
            for (x = 0; x < XSize; x++) {
                if (grid[x][y] == CELL_DOOR_CLOSED) {
                    grid[x][y] = CELL_DOOR_OPEN;
                    draw_static_cell(x, y);
                }
            }
        }

        grid[px][py] = CELL_EMPTY;
        draw_static_cell(px, py);

        score += 10;
        _XL_TOCK_SOUND();
        draw_hud();
    } else if (cell == CELL_GUN_ITEM) {
        has_gun = 1;
        gun_cd = (uint16_t)(GUN_COOLDOWN_BASE + (uint8_t)(cur_level * 2));

        grid[px][py] = CELL_EMPTY;
        draw_static_cell(px, py);

        _XL_TICK_SOUND();
        draw_hud();
    } else if (cell == CELL_SHIELD) {
        shield_timer = SHIELD_DURATION;

        grid[px][py] = CELL_EMPTY;
        draw_static_cell(px, py);

        _XL_PING_SOUND();
    } else if (cell == CELL_DOOR_OPEN) {
        cur_level++;

        if (cur_level > MAX_LEVELS) {
            game_state = STATE_WIN;
        } else {
            load_level(cur_level);
            s_reload = 1;
        }
    }
}

static void try_move(uint8_t dir)
{
    uint8_t nx, ny;
    uint8_t ox, oy;
    uint8_t cell;
    uint8_t idx;

    facing = dir;

    ox = px;
    oy = py;
    nx = px;
    ny = py;

    switch (dir) {
        case DIR_UP:
            if (py > 0) {
                ny--;
            }
            break;
        case DIR_RIGHT:
            if (px < XSize - 1) {
                nx++;
            }
            break;
        case DIR_DOWN:
            if (py < YSize - 1) {
                ny++;
            }
            break;
        case DIR_LEFT:
            if (px > 0) {
                nx--;
            }
            break;
    }

    if (nx >= XSize || ny >= YSize) {
        return;
    }

    cell = grid[nx][ny];

    if (cell == CELL_WALL || cell == CELL_DOOR_CLOSED) {
        return;
    }

    idx = find_enemy(nx, ny);

    if (idx != 255) {
        if (shield_timer > 0) {
            px = nx;
            py = ny;

            draw_static_cell(ox, oy);

            idx = find_enemy(px, py);
            while (idx != 255) {
                score += 50;
                _XL_PING_SOUND();
                remove_enemy(idx);
                idx = find_enemy(px, py);
            }

            draw_static_cell(px, py);
            draw_player();
            draw_hud();
        } else {
            game_state = STATE_GAME_OVER;
        }

        return;
    }

    px = nx;
    py = ny;

    draw_static_cell(ox, oy);

    check_pickups();

    if (s_reload) {
        return;
    }

    if (game_state != STATE_PLAYING) {
        return;
    }

    draw_player();
}

static void move_enemies(void)
{
    uint8_t i, ox, oy, nx, ny;
    uint8_t idx;
    short dx, dy;

    i = 0;

    while (i < enemy_count) {
        ox = ex[i];
        oy = ey[i];

        dx = (short)px - (short)ex[i];
        dy = (short)py - (short)ey[i];

        nx = ex[i];
        ny = ey[i];

        if (dx > 0) {
            nx++;
        } else if (dx < 0) {
            nx--;
        } else if (dy > 0) {
            ny++;
        } else if (dy < 0) {
            ny--;
        }

        if (nx >= 1 && nx < XSize - 1 && ny >= 1 && ny < YSize - 1) {
            if (grid[nx][ny] == CELL_EMPTY &&
                !has_bullet_at(nx, ny) &&
                !has_enemy_at(nx, ny)) {
                ex[i] = nx;
                ey[i] = ny;

                draw_static_cell(ox, oy);
                draw_enemy(i);
            }
        }

        if (ex[i] == px && ey[i] == py) {
            if (shield_timer > 0) {
                idx = find_enemy(px, py);

                while (idx != 255) {
                    score += 50;
                    _XL_PING_SOUND();
                    remove_enemy(idx);
                    idx = find_enemy(px, py);
                }

                draw_static_cell(px, py);
                draw_player();
                draw_hud();
                continue;
            } else {
                game_state = STATE_GAME_OVER;
                return;
            }
        }

        i++;
    }
}

static void update_bullets(void)
{
    uint8_t i;
    uint8_t ox, oy;
    uint8_t nx, ny;
    uint8_t cell;
    uint8_t idx;
    uint8_t any;

    any = 0;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bactive[i]) {
            continue;
        }

        any = 1;

        ox = bx[i];
        oy = by[i];
        nx = ox;
        ny = oy;

        switch (bdir[i]) {
            case DIR_UP:
                if (ny > 0) {
                    ny--;
                }
                break;
            case DIR_RIGHT:
                if (nx < XSize - 1) {
                    nx++;
                }
                break;
            case DIR_DOWN:
                if (ny < YSize - 1) {
                    ny++;
                }
                break;
            case DIR_LEFT:
                if (nx > 0) {
                    nx--;
                }
                break;
        }

        if (nx >= XSize || ny >= YSize) {
            bactive[i] = 0;
            draw_static_cell(ox, oy);
            continue;
        }

        cell = grid[nx][ny];

        if (cell == CELL_WALL || cell == CELL_DOOR_CLOSED || cell == CELL_DOOR_OPEN) {
            bactive[i] = 0;
            draw_static_cell(ox, oy);
            continue;
        }

        idx = find_enemy(nx, ny);

        if (idx != 255) {
            bactive[i] = 0;

            while (idx != 255) {
                remove_enemy(idx);
                score += 20;
                _XL_PING_SOUND();
                idx = find_enemy(nx, ny);
            }

            draw_static_cell(ox, oy);
            draw_static_cell(nx, ny);
            draw_hud();
            continue;
        }

        bx[i] = nx;
        by[i] = ny;

        draw_static_cell(ox, oy);
        draw_bullet(i);
    }

    if (any) {
        draw_player();
    }
}

static void fire_bullet(void)
{
    uint8_t nx, ny;
    uint8_t cell;
    uint8_t idx;
    uint8_t i;
    uint8_t had_enemy;

    nx = px;
    ny = py;

    switch (facing) {
        case DIR_UP:
            if (ny > 0) {
                ny--;
            }
            break;
        case DIR_RIGHT:
            if (nx < XSize - 1) {
                nx++;
            }
            break;
        case DIR_DOWN:
            if (ny < YSize - 1) {
                ny++;
            }
            break;
        case DIR_LEFT:
            if (nx > 0) {
                nx--;
            }
            break;
    }

    _XL_SHOOT_SOUND();

    if (nx >= XSize || ny >= YSize) {
        return;
    }

    cell = grid[nx][ny];

    if (cell == CELL_WALL || cell == CELL_DOOR_CLOSED || cell == CELL_DOOR_OPEN) {
        return;
    }

    had_enemy = 0;
    idx = find_enemy(nx, ny);

    while (idx != 255) {
        had_enemy = 1;
        remove_enemy(idx);
        score += 20;
        _XL_PING_SOUND();
        idx = find_enemy(nx, ny);
    }

    if (had_enemy) {
        draw_static_cell(nx, ny);
        draw_hud();
        return;
    }

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bactive[i]) {
            bx[i] = nx;
            by[i] = ny;
            bdir[i] = facing;
            bactive[i] = 1;

            draw_bullet(i);
            return;
        }
    }
}

static void show_end(void)
{
    buf_clear();
    _XL_CLEAR_SCREEN();

    if (game_state == STATE_WIN) {
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(0, 1, "WIN");
    } else {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(0, 1, "OVER");
    }

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    if (XSize >= 7) {
        _XL_PRINT(0, 2, "S");
        _XL_PRINTD(2, 2, 5, score);
    } else {
        _XL_PRINTD(1, 2, 5, score);
    }

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(0, YSize - 1, "PRESS");
}

/* ===== Main ===== */

int main(void)
{
    uint8_t input;
    uint16_t frame;

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
        frame = 0;
        s_reload = 0;

        load_level(cur_level);
        s_reload = 0;

        while (game_state == STATE_PLAYING) {
            input = _XL_INPUT();
            frame++;

            if (_XL_LEFT(input)) {
                try_move(DIR_LEFT);
            } else if (_XL_RIGHT(input)) {
                try_move(DIR_RIGHT);
            } else if (_XL_UP(input)) {
                try_move(DIR_UP);
            } else if (_XL_DOWN(input)) {
                try_move(DIR_DOWN);
            }

            if (s_reload) {
                s_reload = 0;
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                continue;
            }

            if (game_state != STATE_PLAYING) {
                break;
            }

            if (has_gun && gun_cd == 0 && _XL_FIRE(input)) {
                gun_cd = (uint16_t)(GUN_COOLDOWN_BASE + (uint8_t)(cur_level * 2));
                fire_bullet();
            }

            if (gun_cd > 0) {
                gun_cd--;
            }

            if (shield_timer > 0) {
                shield_timer--;
                if (shield_timer == 0) {
                    draw_player();
                }
            }

            if ((frame % ENEMY_MOVE_INTERVAL) == 0) {
                move_enemies();
            }

            if (game_state != STATE_PLAYING) {
                break;
            }

            update_bullets();

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        show_end();
        _XL_WAIT_FOR_INPUT();
    }

    return 0;
}

