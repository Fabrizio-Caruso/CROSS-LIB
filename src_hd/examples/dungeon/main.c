#include "cross_lib.h"

/* ===== Constants ===== */
#define MAX_LEVELS          42
#define MAX_ENEMIES         8
#define MAX_BULLETS         4
#define GUN_COOLDOWN_BASE   8
#define SHIELD_DURATION     60
#define ENEMY_MOVE_INTERVAL 16

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

/* 2x2 player tiles */
#define T_PLAYER_LEFT_UL   _TILE_10
#define T_PLAYER_LEFT_UR   _TILE_11
#define T_PLAYER_LEFT_LL   _TILE_12
#define T_PLAYER_LEFT_LR   _TILE_13

#define T_PLAYER_RIGHT_UL  _TILE_14
#define T_PLAYER_RIGHT_UR  _TILE_15
#define T_PLAYER_RIGHT_LL  _TILE_16
#define T_PLAYER_RIGHT_LR  _TILE_17

/* ===== Screen buffer ===== */
static uint8_t sbuf_tile[XSize][YSize];
static uint8_t sbuf_color[XSize][YSize];

/* ===== Game state ===== */
static uint8_t  cur_level;
static uint8_t  px, py; /* Top-left of 2x2 player */
static uint8_t  facing;
static uint8_t  last_h_dir;
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

/* ===== Forward declarations ===== */
static void gen_open_room(uint8_t lvl);
static void gen_flat_16x16(uint8_t lvl);
static void gen_maze_level(uint8_t lvl);

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
    uint8_t tl, tr, bl, br;
    uint8_t x2, y2;

    if (shield_timer > 0) {
        col = _XL_YELLOW;
    } else {
        col = _XL_CYAN;
    }

    if (last_h_dir == DIR_LEFT) {
        tl = T_PLAYER_LEFT_UL;
        tr = T_PLAYER_LEFT_UR;
        bl = T_PLAYER_LEFT_LL;
        br = T_PLAYER_LEFT_LR;
    } else {
        tl = T_PLAYER_RIGHT_UL;
        tr = T_PLAYER_RIGHT_UR;
        bl = T_PLAYER_RIGHT_LL;
        br = T_PLAYER_RIGHT_LR;
    }

    x2 = (uint8_t)(px + 1);
    y2 = (uint8_t)(py + 1);

    buf_draw(px, py, tl, col);
    buf_draw(x2, py, tr, col);
    buf_draw(px, y2, bl, col);
    buf_draw(x2, y2, br, col);
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

/* ===== 2x2 player helpers ===== */

static uint8_t player_contains(uint8_t x, uint8_t y)
{
    if (XSize < 2 || YSize < 2) {
        return 0;
    }

    if (px > (uint8_t)(XSize - 2) || py > (uint8_t)(YSize - 2)) {
        return 0;
    }

    if (x < px || x > (uint8_t)(px + 1)) {
        return 0;
    }

    if (y < py || y > (uint8_t)(py + 1)) {
        return 0;
    }

    return 1;
}

static uint8_t cell_walkable(uint8_t x, uint8_t y)
{
    if (x >= XSize || y >= YSize) {
        return 0;
    }

    switch (grid[x][y]) {
        case CELL_EMPTY:
        case CELL_KEY:
        case CELL_GUN_ITEM:
        case CELL_SHIELD:
        case CELL_DOOR_OPEN:
            return 1;

        default:
            return 0;
    }
}

static uint8_t player_area_walkable(uint8_t nx, uint8_t ny)
{
    uint8_t dx, dy;
    uint8_t x, y;

    if (XSize < 2 || YSize < 2) {
        return 0;
    }

    if (nx > (uint8_t)(XSize - 2) || ny > (uint8_t)(YSize - 2)) {
        return 0;
    }

    for (dy = 0; dy < 2; dy++) {
        y = (uint8_t)(ny + dy);

        for (dx = 0; dx < 2; dx++) {
            x = (uint8_t)(nx + dx);

            if (!cell_walkable(x, y)) {
                return 0;
            }
        }
    }

    return 1;
}

static uint8_t player_area_has_enemy(uint8_t nx, uint8_t ny)
{
    uint8_t dx, dy;
    uint8_t x, y;

    if (XSize < 2 || YSize < 2) {
        return 0;
    }

    if (nx > (uint8_t)(XSize - 2) || ny > (uint8_t)(YSize - 2)) {
        return 0;
    }

    for (dy = 0; dy < 2; dy++) {
        y = (uint8_t)(ny + dy);

        for (dx = 0; dx < 2; dx++) {
            x = (uint8_t)(nx + dx);

            if (has_enemy_at(x, y)) {
                return 1;
            }
        }
    }

    return 0;
}

static uint8_t find_enemy_in_player_area(void)
{
    uint8_t i;

    for (i = 0; i < enemy_count; i++) {
        if (player_contains(ex[i], ey[i])) {
            return i;
        }
    }

    return 255;
}

static void kill_enemies_in_player_area(void)
{
    uint8_t idx;

    while ((idx = find_enemy_in_player_area()) != 255) {
        remove_enemy(idx);
        score += 50;
        _XL_PING_SOUND();
    }
}

static void draw_player_area(uint8_t x, uint8_t y)
{
    uint8_t dx, dy;
    uint8_t tx, ty;

    for (dy = 0; dy < 2; dy++) {
        ty = (uint8_t)(y + dy);

        for (dx = 0; dx < 2; dx++) {
            tx = (uint8_t)(x + dx);

            if (tx < XSize && ty < YSize) {
                draw_static_cell(tx, ty);
            }
        }
    }
}

/* ===== Level helpers ===== */

static uint8_t place_item(uint8_t type)
{
    uint16_t i;
    uint8_t x, y;

    if (XSize < 3 || YSize < 3) {
        return 0;
    }

    for (i = 0; i < 200; i++) {
        x = (uint8_t)(1 + _XL_RAND() % (XSize - 2));
        y = (uint8_t)(1 + _XL_RAND() % (YSize - 2));

        if (grid[x][y] == CELL_EMPTY && !player_contains(x, y)) {
            grid[x][y] = type;
            return 1;
        }
    }

    for (y = 1; y < YSize - 1; y++) {
        for (x = 1; x < XSize - 1; x++) {
            if (grid[x][y] == CELL_EMPTY && !player_contains(x, y)) {
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
    uint8_t pcx, pcy;

    enemy_count = 0;

    if (XSize < 3 || YSize < 3) {
        return;
    }

    num_e = (uint8_t)(2 + (lvl % 4));
    if (num_e > MAX_ENEMIES) {
        num_e = MAX_ENEMIES;
    }

    min_dist = (uint16_t)((XSize + YSize) / 3);

    pcx = (uint8_t)(px + 1);
    pcy = (uint8_t)(py + 1);

    /* First pass: try to keep enemies away from the player. */
    for (i = 0; i < 300 && enemy_count < num_e; i++) {
        x = (uint8_t)(1 + _XL_RAND() % (XSize - 2));
        y = (uint8_t)(1 + _XL_RAND() % (YSize - 2));

        if (grid[x][y] == CELL_EMPTY &&
            !has_enemy_at(x, y) &&
            !player_contains(x, y)) {

            dx = (uint16_t)((x > pcx) ? (x - pcx) : (pcx - x));
            dy = (uint16_t)((y > pcy) ? (y - pcy) : (pcy - y));

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
            !player_contains(x, y)) {

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
                !player_contains(x, y)) {

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

    if (XSize < 8 || YSize < 8) {
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

    maxlx = (uint8_t)(lmx - 1);
    if (maxlx > 0 && (maxlx & 1) == 0) {
        maxlx--;
    }

    maxly = (uint8_t)(lmy - 1);
    if (maxly > 0 && (maxly & 1) == 0) {
        maxly--;
    }

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
        Start in the first 3x3 room.
        Logical room (1,1) maps to top-left grid cell (4,4).
        The 2x2 player top-left is placed at (4,4).
    */
    px = 4;
    py = 4;

    maze_visited[1][1] = 1;
    carve_3x3(4, 4);

    top = 0;
    maze_stack_x[top] = 1;
    maze_stack_y[top] = 1;
    top++;

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
                    tx + 1 < XSize &&
                    ty + 1 < YSize &&
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
                    tx + 1 < XSize &&
                    ty + 1 < YSize &&
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

            if (grid[cx][cy] == CELL_EMPTY &&
                maze_reach[cx][cy] &&
                !player_contains(cx, cy)) {

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

    if (player_contains(best_x, best_y)) {
        for (y = 1; y < YSize - 1; y++) {
            for (x = 1; x < XSize - 1; x++) {
                if (grid[x][y] == CELL_EMPTY &&
                    maze_reach[x][y] &&
                    !player_contains(x, y)) {

                    best_x = x;
                    best_y = y;
                    break;
                }
            }

            if (!player_contains(best_x, best_y)) {
                break;
            }
        }
    }

    if (player_contains(best_x, best_y)) {
        gen_open_room(lvl);
        return;
    }

    grid[best_x][best_y] = CELL_DOOR_CLOSED;

    /*
        Flood fill again with the door in place.
    */
    maze_flood_fill(px, py);

    /*
        Remove unreachable empty cells.
    */
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
                    !player_contains(x, y)) {

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

    gun_cd = (uint16_t)(GUN_COOLDOWN_BASE); // + (uint8_t)(lvl * 2));
}

/* ===== Level type: large open room with optional wall blocks ===== */

#define MIN_NUM_BLOCKS 3

static void gen_open_room(uint8_t lvl)
{
    uint8_t x, y, i;
    uint8_t num_blocks;
    uint8_t sz, bx, by;
    uint8_t dx, dy;
    uint8_t ok;
    uint8_t found;

    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            if (x == 0 || y == 0 || x == XSize - 1 || y == YSize - 1) {
                grid[x][y] = CELL_WALL;
            } else {
                grid[x][y] = CELL_EMPTY;
            }
        }
    }

    px = 1;
    py = 1;

    num_blocks = 0;
    if (XSize > 10 && YSize > 10) {
        num_blocks = (uint8_t)(MIN_NUM_BLOCKS + _XL_RAND() % 8);
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
                    if (x < XSize && y < YSize) {
                        grid[x][y] = CELL_WALL;
                    }
                }
            }
        }
    }

    carve_3x3(1, 1);

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

    found = 0;
    if (player_contains(dx, dy)) {
        for (y = 1; y < YSize - 1 && !found; y++) {
            for (x = 1; x < XSize - 1; x++) {
                if (grid[x][y] == CELL_EMPTY && !player_contains(x, y)) {
                    dx = x;
                    dy = y;
                    found = 1;
                    break;
                }
            }
        }
    }

    if (found || !player_contains(dx, dy)) {
        carve_3x3(dx, dy);
        grid[dx][dy] = CELL_DOOR_CLOSED;
    } else {
        for (y = 0; y < YSize; y++) {
            for (x = 0; x < XSize; x++) {
                if (grid[x][y] == CELL_DOOR_CLOSED) {
                    grid[x][y] = CELL_DOOR_OPEN;
                }
            }
        }
    }

    place_item(CELL_KEY);
    place_item(CELL_GUN_ITEM);
    place_item(CELL_SHIELD);
    place_enemies(lvl);

    for (i = 0; i < MAX_BULLETS; i++) {
        bactive[i] = 0;
    }

    gun_cd = (uint16_t)(GUN_COOLDOWN_BASE); // + (uint8_t)(lvl * 2));
}

/* ===== Level type: flat centered room ===== */

static void gen_flat_16x16(uint8_t lvl)
{
    uint8_t x, y, i;
    uint8_t rw, rh;
    uint8_t ox, oy;
    uint8_t dx, dy;
    uint8_t found;

    if (XSize < 6 || YSize < 6) {
        gen_open_room(lvl);
        return;
    }

    rw = 16;
    rh = 16;

    if (rw > (uint8_t)(XSize - 2)) {
        rw = (uint8_t)(XSize - 2);
    }
    if (rh > (uint8_t)(YSize - 2)) {
        rh = (uint8_t)(YSize - 2);
    }

    if (rw < 4 || rh < 4) {
        gen_open_room(lvl);
        return;
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

    found = 0;
    if (player_contains(dx, dy)) {
        for (y = oy + 1; y <= oy + rh - 1 && y < YSize - 1 && !found; y++) {
            for (x = ox + 1; x <= ox + rw - 1 && x < XSize - 1; x++) {
                if (grid[x][y] == CELL_EMPTY && !player_contains(x, y)) {
                    dx = x;
                    dy = y;
                    found = 1;
                    break;
                }
            }
        }
    }

    if (found || !player_contains(dx, dy)) {
        grid[dx][dy] = CELL_DOOR_CLOSED;
    } else {
        gen_open_room(lvl);
        return;
    }

    place_item(CELL_KEY);
    place_item(CELL_GUN_ITEM);
    place_item(CELL_SHIELD);
    place_enemies(lvl);

    for (i = 0; i < MAX_BULLETS; i++) {
        bactive[i] = 0;
    }

    gun_cd = (uint16_t)(GUN_COOLDOWN_BASE); // + (uint8_t)(lvl * 2));
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
    uint8_t dx, dy, x, y, xx, yy;

    for (dy = 0; dy < 2; dy++) {
        y = (uint8_t)(py + dy);

        for (dx = 0; dx < 2; dx++) {
            x = (uint8_t)(px + dx);

            if (x >= XSize || y >= YSize) {
                continue;
            }

            if (grid[x][y] == CELL_KEY) {
                for (yy = 0; yy < YSize; yy++) {
                    for (xx = 0; xx < XSize; xx++) {
                        if (grid[xx][yy] == CELL_DOOR_CLOSED) {
                            grid[xx][yy] = CELL_DOOR_OPEN;
                            draw_static_cell(xx, yy);
                        }
                    }
                }

                grid[x][y] = CELL_EMPTY;
                draw_static_cell(x, y);

                score += 10;
                _XL_TOCK_SOUND();
                draw_hud();
                return;
            }
        }
    }

    for (dy = 0; dy < 2; dy++) {
        y = (uint8_t)(py + dy);

        for (dx = 0; dx < 2; dx++) {
            x = (uint8_t)(px + dx);

            if (x >= XSize || y >= YSize) {
                continue;
            }

            if (grid[x][y] == CELL_GUN_ITEM) {
                has_gun = 1;
                gun_cd = (uint16_t)(GUN_COOLDOWN_BASE); // + (uint8_t)(cur_level * 2));

                grid[x][y] = CELL_EMPTY;
                draw_static_cell(x, y);

                _XL_TICK_SOUND();
                draw_hud();
                return;
            }
        }
    }

    for (dy = 0; dy < 2; dy++) {
        y = (uint8_t)(py + dy);

        for (dx = 0; dx < 2; dx++) {
            x = (uint8_t)(px + dx);

            if (x >= XSize || y >= YSize) {
                continue;
            }

            if (grid[x][y] == CELL_SHIELD) {
                shield_timer = SHIELD_DURATION;

                grid[x][y] = CELL_EMPTY;
                draw_static_cell(x, y);

                _XL_PING_SOUND();
                return;
            }
        }
    }

    for (dy = 0; dy < 2; dy++) {
        y = (uint8_t)(py + dy);

        for (dx = 0; dx < 2; dx++) {
            x = (uint8_t)(px + dx);

            if (x >= XSize || y >= YSize) {
                continue;
            }

            if (grid[x][y] == CELL_DOOR_OPEN) {
                cur_level++;

                if (cur_level > MAX_LEVELS) {
                    game_state = STATE_WIN;
                } else {
                    load_level(cur_level);
                    s_reload = 1;
                }

                return;
            }
        }
    }
}

static void try_move(uint8_t dir)
{
    uint8_t nx, ny;
    uint8_t ox, oy;

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

    if (nx == px && ny == py) {
        return;
    }

    if (!player_area_walkable(nx, ny)) {
        return;
    }

    if (player_area_has_enemy(nx, ny)) {
        if (shield_timer > 0) {
            px = nx;
            py = ny;

            if (dir == DIR_LEFT || dir == DIR_RIGHT) {
                last_h_dir = dir;
            }

            draw_player_area(ox, oy);
            kill_enemies_in_player_area();
            draw_player_area(px, py);
            draw_player();
            draw_hud();
        } else {
            game_state = STATE_GAME_OVER;
        }

        return;
    }

    px = nx;
    py = ny;

    if (dir == DIR_LEFT || dir == DIR_RIGHT) {
        last_h_dir = dir;
    }

    draw_player_area(ox, oy);

    check_pickups();

    if (s_reload) {
        return;
    }

    if (game_state != STATE_PLAYING) {
        return;
    }

    draw_player();
}


static uint8_t enemy_cell_free(uint8_t x, uint8_t y)
{
    if (x < 1 || x >= XSize - 1 || y < 1 || y >= YSize - 1) {
        return 0;
    }

    /*
        Enemies are blocked by:
        - walls
        - closed doors
        - open doors
        - key
        - gun item
        - shield item

        So only empty floor is free for enemies.
    */
    if (grid[x][y] != CELL_EMPTY) {
        return 0;
    }

    if (has_bullet_at(x, y)) {
        return 0;
    }

    if (has_enemy_at(x, y)) {
        return 0;
    }

    return 1;
}

static uint8_t enemy_step_dir(uint8_t i, uint8_t dir, uint8_t *nx, uint8_t *ny)
{
    uint8_t x = ex[i];
    uint8_t y = ey[i];

    switch (dir) {
        case DIR_UP:
            if (y == 0) {
                return 0;
            }
            *nx = x;
            *ny = (uint8_t)(y - 1);
            break;

        case DIR_RIGHT:
            if (x >= XSize - 1) {
                return 0;
            }
            *nx = (uint8_t)(x + 1);
            *ny = y;
            break;

        case DIR_DOWN:
            if (y >= YSize - 1) {
                return 0;
            }
            *nx = x;
            *ny = (uint8_t)(y + 1);
            break;

        case DIR_LEFT:
            if (x == 0) {
                return 0;
            }
            *nx = (uint8_t)(x - 1);
            *ny = y;
            break;

        default:
            return 0;
    }

    return enemy_cell_free(*nx, *ny);
}

static uint8_t enemy_random_step(uint8_t i, uint8_t *nx, uint8_t *ny)
{
    uint8_t valid[4];
    uint8_t count = 0;
    uint8_t d;
    uint8_t pick;

    for (d = 0; d < 4; d++) {
        if (enemy_step_dir(i, d, nx, ny)) {
            valid[count] = d;
            count++;
        }
    }

    if (count == 0) {
        return 0;
    }

    pick = (uint8_t)(_XL_RAND() % count);
    enemy_step_dir(i, valid[pick], nx, ny);
    return 1;
}


static void move_enemies(void)
{
    uint8_t i;
    uint8_t ox, oy;
    uint8_t nx, ny;
    uint8_t moved;
    short dx, dy;

    i = 0;

    while (i < enemy_count) {
        ox = ex[i];
        oy = ey[i];

        /*
            If the enemy is already inside the player's 2x2 area,
            resolve collision before letting it move away.
        */
        if (player_contains(ox, oy)) {
            if (shield_timer > 0) {
                kill_enemies_in_player_area();
                draw_player_area(px, py);
                draw_player();
                draw_hud();
                continue;
            } else {
                game_state = STATE_GAME_OVER;
                return;
            }
        }

        dx = (short)px - (short)ox;
        dy = (short)py - (short)oy;

        nx = ox;
        ny = oy;
        moved = 0;

        /* Preferred direction: keep current chasing behavior. */
        if (dx > 0) {
            nx = (uint8_t)(ox + 1);
        } else if (dx < 0) {
            nx = (uint8_t)(ox - 1);
        } else if (dy > 0) {
            ny = (uint8_t)(oy + 1);
        } else if (dy < 0) {
            ny = (uint8_t)(oy - 1);
        }

        if (nx != ox || ny != oy) {
            if (enemy_cell_free(nx, ny)) {
                moved = 1;
            } else {
                /*
                    Preferred cell is blocked by wall/item/door/enemy/bullet.
                    Pick one random valid direction.
                */
                if (enemy_random_step(i, &nx, &ny)) {
                    moved = 1;
                }
            }
        }

        if (moved) {
            ex[i] = nx;
            ey[i] = ny;

            draw_static_cell(ox, oy);
            draw_enemy(i);
        }

        if (player_contains(ex[i], ey[i])) {
            if (shield_timer > 0) {
                kill_enemies_in_player_area();
                draw_player_area(px, py);
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
    uint8_t dir;
    uint8_t cell;
    uint8_t idx;
    uint8_t i;
    uint8_t had_enemy;

    dir = last_h_dir;

    if (dir != DIR_LEFT && dir != DIR_RIGHT) {
        return;
    }

    /*
        Fire from the top row of the 2x2 player.
    */
    ny = py;

    if (dir == DIR_RIGHT) {
        if (px + 2 >= XSize) {
            return;
        }

        nx = (uint8_t)(px + 2);
    } else {
        if (px == 0) {
            return;
        }

        nx = (uint8_t)(px - 1);
    }

    if (nx >= XSize || ny >= YSize) {
        return;
    }

    cell = grid[nx][ny];

    if (cell == CELL_WALL || cell == CELL_DOOR_CLOSED || cell == CELL_DOOR_OPEN) {
        return;
    }

    _XL_SHOOT_SOUND();

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
            bdir[i] = dir;
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
        last_h_dir = DIR_RIGHT;
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
                gun_cd = (uint16_t)(GUN_COOLDOWN_BASE);
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