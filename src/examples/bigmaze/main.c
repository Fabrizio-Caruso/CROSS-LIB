#include "cross_lib.h"

#define MAX_ENEMIES       16
#define MAX_LEVELS        8
#define LOG_COLS          16
#define LOG_ROWS          8
#define HALF_COLS         8
#define SCREEN_W          32
#define SCREEN_H          16
#define MIN_ENEMY_DIST    8
#define START_LIVES       3
#define MAX_P_BULLETS     4
#define MAX_E_BULLETS     10
#define INVINCIBLE_FRAMES 180
#define FREEZE_FRAMES     150

/* ---- Maze data: left half (8 cols x 8 rows), indexed [level][col][row] ---- */
static const uint8_t maze_left[MAX_LEVELS][HALF_COLS][LOG_ROWS] = {
    {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1}
    },
    {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,1,0,0,0},
        {1,0,0,0,1,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,1,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1}
    },
    {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,1,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,1,0,0,0,0,0},
        {1,1,1,1,1,1,1,1}
    },
    {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0},
        {1,0,1,0,0,0,0,0},
        {1,0,1,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,1,0,0,0,0,0},
        {1,0,1,0,0,0,0,0},
        {1,1,1,1,1,1,1,1}
    },
    {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0},
        {1,0,1,0,0,1,0,0},
        {1,0,1,0,0,1,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,1,0,0,1,0,0},
        {1,1,1,1,1,1,1,1}
    },
    {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,0,1,1,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1}
    },
    {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0},
        {1,0,1,0,0,1,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,0,1,0,0,1,0,0},
        {1,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1}
    },
    {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0},
        {1,0,1,1,0,0,0,0},
        {1,0,0,0,0,0,0,0},
        {1,1,0,0,0,0,0,0},
        {1,0,0,0,0,1,0,0},
        {1,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1}
    }
};

static uint8_t full_maze[LOG_ROWS][LOG_COLS];

/* Game state */
static uint8_t  level;
static uint8_t  px, py;
static uint8_t  player_dir;
static uint8_t  ex[MAX_ENEMIES];
static uint8_t  ey[MAX_ENEMIES];
static uint8_t  enemy_dir[MAX_ENEMIES];
static uint8_t  enemy_alive[MAX_ENEMIES];
static uint8_t  num_enemies;
static uint8_t  enemies_killed;
static uint8_t  lives;
static uint16_t frame_count;
static uint16_t score;

/* Player bullets (up to 4 simultaneous) */
static uint8_t pbx[MAX_P_BULLETS];
static uint8_t pby[MAX_P_BULLETS];
static uint8_t pbd[MAX_P_BULLETS];
static uint8_t pba[MAX_P_BULLETS];
static uint8_t max_p_bullets;

/* Enemy bullets (up to 10 simultaneous) */
static uint8_t ebx[MAX_E_BULLETS];
static uint8_t eby[MAX_E_BULLETS];
static uint8_t ebd[MAX_E_BULLETS];
static uint8_t eba[MAX_E_BULLETS];
static uint8_t max_e_bullets;
static uint8_t enemy_bullets_active;

/* Items: type 0=extra bullet, 1=invincible, 2=freeze */
static uint8_t item_x[3];
static uint8_t item_y[3];
static uint8_t item_active[3];

/* Timers */
static uint16_t invincible_timer;
static uint16_t freeze_timer;

/* ---- Build full 16x8 maze by mirroring ---- */
static void build_maze(uint8_t lvl)
{
    uint8_t r, c;
    for (r = 0; r < LOG_ROWS; r++) {
        for (c = 0; c < HALF_COLS; c++) {
            full_maze[r][c] = maze_left[lvl][r][c];
            full_maze[r][LOG_COLS - 1 - c] = maze_left[lvl][r][c];
        }
    }
}

static uint8_t tile_in_wall(uint8_t sx, uint8_t sy)
{
    uint8_t cx, cy;
    if (sx >= SCREEN_W || sy >= SCREEN_H) return 1;
    cx = (uint8_t)(sx / 2);
    cy = (uint8_t)(sy / 2);
    return (full_maze[cy][cx] == 1);
}

static uint8_t entity_in_wall(uint8_t sx, uint8_t sy)
{
    if (sx + 1 >= SCREEN_W || sy + 1 >= SCREEN_H) return 1;
    if (tile_in_wall(sx, sy)) return 1;
    if (tile_in_wall(sx + 1, sy)) return 1;
    if (tile_in_wall(sx, sy + 1)) return 1;
    if (tile_in_wall(sx + 1, sy + 1)) return 1;
    return 0;
}

static uint8_t entities_overlap(uint8_t ax, uint8_t ay, uint8_t bx, uint8_t by)
{
    if (ax > bx + 1) return 0;
    if (bx > ax + 1) return 0;
    if (ay > by + 1) return 0;
    if (by > ay + 1) return 0;
    return 1;
}

static uint8_t can_player_move(uint8_t sx, uint8_t sy)
{
    uint8_t i;
    if (entity_in_wall(sx, sy)) return 0;
    for (i = 0; i < num_enemies; i++) {
        if (enemy_alive[i] && entities_overlap(sx, sy, ex[i], ey[i])) return 0;
    }
    return 1;
}

static uint8_t can_enemy_move(uint8_t idx, uint8_t sx, uint8_t sy)
{
    uint8_t i;
    if (entity_in_wall(sx, sy)) return 0;
    for (i = 0; i < num_enemies; i++) {
        if (i != idx && enemy_alive[i] && entities_overlap(sx, sy, ex[i], ey[i])) return 0;
    }
    return 1;
}

/* ---- Drawing ---- */
static void draw_wall_block(uint8_t cx, uint8_t cy)
{
    uint8_t sx, sy;
    sx = (uint8_t)(cx * 2);
    sy = (uint8_t)(cy * 2);
    _XL_DRAW(sx,     sy,     _TILE_0, _XL_BLUE);
    _XL_DRAW(sx + 1, sy,     _TILE_0, _XL_BLUE);
    _XL_DRAW(sx,     sy + 1, _TILE_0, _XL_BLUE);
    _XL_DRAW(sx + 1, sy + 1, _TILE_0, _XL_BLUE);
}

static void draw_maze(void)
{
    uint8_t cx, cy;
    for (cy = 0; cy < LOG_ROWS; cy++) {
        for (cx = 0; cx < LOG_COLS; cx++) {
            if (full_maze[cy][cx] == 1) {
                draw_wall_block(cx, cy);
            }
        }
    }
}

static void draw_hud(void)
{
    uint8_t cnt;
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(1, 0, "SCORE:");
    _XL_PRINTD(9, 0, 4, score);
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(18, 0, "L:");
    _XL_PRINTD(21, 0, 1, level);
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(23, 0, "x");
    _XL_PRINTD(25, 0, 1, lives);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(27, 0, "B:");
    cnt = max_p_bullets;
    _XL_PRINTD(30, 0, 1, cnt);
}

static void draw_player(void)
{
    uint8_t col;
    col = (invincible_timer > 0) ? _XL_YELLOW : _XL_GREEN;
    if (player_dir == 0) {
        _XL_DRAW(px,     py,     _TILE_1, col);
        _XL_DRAW(px + 1, py,     _TILE_2, col);
        _XL_DRAW(px,     py + 1, _TILE_3, col);
        _XL_DRAW(px + 1, py + 1, _TILE_4, col);
    } else {
        _XL_DRAW(px,     py,     _TILE_5, col);
        _XL_DRAW(px + 1, py,     _TILE_6, col);
        _XL_DRAW(px,     py + 1, _TILE_7, col);
        _XL_DRAW(px + 1, py + 1, _TILE_8, col);
    }
}

static void delete_player(void)
{
    _XL_DELETE(px,     py);
    _XL_DELETE(px + 1, py);
    _XL_DELETE(px,     py + 1);
    _XL_DELETE(px + 1, py + 1);
}

static void draw_enemy(uint8_t i)
{
    if (enemy_dir[i] == 0) {
        _XL_DRAW(ex[i],     ey[i],     _TILE_9,  _XL_RED);
        _XL_DRAW(ex[i] + 1, ey[i],     _TILE_10, _XL_RED);
        _XL_DRAW(ex[i],     ey[i] + 1, _TILE_11, _XL_RED);
        _XL_DRAW(ex[i] + 1, ey[i] + 1, _TILE_12, _XL_RED);
    } else {
        _XL_DRAW(ex[i],     ey[i],     _TILE_13, _XL_RED);
        _XL_DRAW(ex[i] + 1, ey[i],     _TILE_14, _XL_RED);
        _XL_DRAW(ex[i],     ey[i] + 1, _TILE_15, _XL_RED);
        _XL_DRAW(ex[i] + 1, ey[i] + 1, _TILE_16, _XL_RED);
    }
}

static void delete_enemy(uint8_t i)
{
    _XL_DELETE(ex[i],     ey[i]);
    _XL_DELETE(ex[i] + 1, ey[i]);
    _XL_DELETE(ex[i],     ey[i] + 1);
    _XL_DELETE(ex[i] + 1, ey[i] + 1);
}

static void draw_all_enemies(void)
{
    uint8_t i;
    for (i = 0; i < num_enemies; i++) {
        if (enemy_alive[i]) draw_enemy(i);
    }
}

/* ---- Draw items (using proper tile ID constants) ---- */
static void draw_item(uint8_t idx)
{
    uint8_t tile, col;
    if (idx == 0) { tile = _TILE_17; col = _XL_YELLOW; }
    else if (idx == 1) { tile = _TILE_18; col = _XL_CYAN; }
    else { tile = _TILE_19; col = _XL_WHITE; }
    _XL_DRAW(item_x[idx], item_y[idx], tile, col);
}

static void delete_item(uint8_t idx)
{
    _XL_DELETE(item_x[idx], item_y[idx]);
}

static void draw_all_items(void)
{
    uint8_t i;
    for (i = 0; i < 3; i++) {
        if (item_active[i]) draw_item(i);
    }
}

/* Redraw items that are not currently covered by an enemy.
   Called after enemy movement so items reappear when an enemy moves away. */
static void redraw_uncovered_items(void)
{
    uint8_t i, j;
    for (i = 0; i < 3; i++) {
        if (!item_active[i]) continue;
        for (j = 0; j < num_enemies; j++) {
            if (enemy_alive[j] &&
                item_x[i] >= ex[j] && item_x[i] <= ex[j] + 1 &&
                item_y[i] >= ey[j] && item_y[i] <= ey[j] + 1) {
                break;
            }
        }
        if (j >= num_enemies) {
            draw_item(i);
        }
    }
}

/* ---- Draw player bullets ---- */
static void draw_p_bullets(void)
{
    uint8_t i;
    for (i = 0; i < max_p_bullets; i++) {
        if (pba[i]) {
            _XL_DRAW(pbx[i], pby[i], _TILE_26, _XL_YELLOW);
        }
    }
}

static void delete_p_bullet(uint8_t i)
{
    if (!tile_in_wall(pbx[i], pby[i])) {
        _XL_DELETE(pbx[i], pby[i]);
    }
}

/* ---- Draw enemy bullets ---- */
static void draw_e_bullets(void)
{
    uint8_t i;
    for (i = 0; i < max_e_bullets; i++) {
        if (eba[i]) {
            _XL_DRAW(ebx[i], eby[i], _TILE_20, _XL_CYAN);
        }
    }
}

static void delete_e_bullet(uint8_t i)
{
    if (!tile_in_wall(ebx[i], eby[i])) {
        _XL_DELETE(ebx[i], eby[i]);
    }
}

/* ---- Place player at first valid open screen position (row >= 2) ---- */
static void place_player(void)
{
    uint8_t r, c;
    uint8_t found;
    found = 0;
    for (r = 2; r + 1 < SCREEN_H && !found; r += 2) {
        for (c = 0; c + 1 < SCREEN_W && !found; c += 2) {
            if (!entity_in_wall(c, r)) {
                px = c;
                py = r;
                found = 1;
            }
        }
    }
    player_dir = 1;
}

/* ---- Place enemies at random valid positions, far from player ---- */
static void place_enemies(uint8_t count)
{
    uint8_t i, j, attempts, valid, ok;
    uint16_t rx, ry;
    uint8_t dist;
    uint8_t fx, fy;

    num_enemies = count;
    if (num_enemies > MAX_ENEMIES) num_enemies = MAX_ENEMIES;
    enemies_killed = 0;
    for (i = 0; i < num_enemies; i++) {
        enemy_alive[i] = 1;
        enemy_dir[i] = 1;
    }
    for (i = 0; i < num_enemies; i++) {
        attempts = 0;
        valid = 0;
        do {
            rx = (uint16_t)(2 + _XL_RAND() % (SCREEN_W - 4));
            ry = (uint16_t)(2 + _XL_RAND() % (SCREEN_H - 4));
            ex[i] = (uint8_t)rx;
            ey[i] = (uint8_t)ry;
            attempts++;
            valid = !entity_in_wall(ex[i], ey[i]);
            if (valid && entities_overlap(ex[i], ey[i], px, py)) valid = 0;
            if (valid) {
                dist = (uint8_t)(((ex[i] > px) ? (ex[i] - px) : (px - ex[i])) +
                                  ((ey[i] > py) ? (ey[i] - py) : (py - ey[i])));
                if (dist < MIN_ENEMY_DIST) valid = 0;
            }
            if (valid) {
                for (j = 0; j < i; j++) {
                    if (entities_overlap(ex[i], ey[i], ex[j], ey[j])) {
                        valid = 0;
                        break;
                    }
                }
            }
        } while (!valid && attempts < 800);

        if (!valid) {
            ok = 0;
            for (fx = 2; fx + 1 < SCREEN_W && !ok; fx += 2) {
                for (fy = 2; fy + 1 < SCREEN_H && !ok; fy += 2) {
                    dist = (uint8_t)(((fx > px) ? (uint8_t)(fx - px) : (uint8_t)(px - fx)) +
                                      ((fy > py) ? (uint8_t)(fy - py) : (uint8_t)(py - fy)));
                    if (dist < MIN_ENEMY_DIST) continue;
                    if (entity_in_wall(fx, fy)) continue;
                    if (entities_overlap(fx, fy, px, py)) continue;
                    for (j = 0; j < i; j++) {
                        if (entities_overlap(fx, fy, ex[j], ey[j])) break;
                    }
                    if (j >= i) {
                        ex[i] = fx;
                        ey[i] = fy;
                        ok = 1;
                    }
                }
            }
        }
    }
}

/* ---- Place items at random valid positions ---- */
static void place_items(void)
{
    uint8_t i, j, attempts, valid;
    uint16_t rx, ry;
    for (i = 0; i < 3; i++) {
        item_active[i] = 1;
        attempts = 0;
        valid = 0;
        do {
            rx = (uint16_t)(2 + _XL_RAND() % (SCREEN_W - 4));
            ry = (uint16_t)(2 + _XL_RAND() % (SCREEN_H - 4));
            item_x[i] = (uint8_t)rx;
            item_y[i] = (uint8_t)ry;
            attempts++;
            valid = !tile_in_wall(item_x[i], item_y[i]);
            if (valid && (item_x[i] >= px && item_x[i] <= px + 1 &&
                          item_y[i] >= py && item_y[i] <= py + 1)) valid = 0;
            if (valid) {
                for (j = 0; j < num_enemies; j++) {
                    if (enemy_alive[j] &&
                        item_x[i] >= ex[j] && item_x[i] <= ex[j] + 1 &&
                        item_y[i] >= ey[j] && item_y[i] <= ey[j] + 1) {
                        valid = 0;
                        break;
                    }
                }
            }
            if (valid) {
                for (j = 0; j < i; j++) {
                    if (item_active[j] &&
                        item_x[i] == item_x[j] && item_y[i] == item_y[j]) {
                        valid = 0;
                        break;
                    }
                }
            }
        } while (!valid && attempts < 200);
        if (!valid) {
            item_active[i] = 0;
        }
    }
}

/* ---- Setup a level ---- */
static void setup_level(uint8_t lvl, uint8_t enemy_count)
{
    uint8_t i;
    level = lvl;
    build_maze(lvl - 1);
    place_player();
    place_enemies(enemy_count);
    place_items();

    for (i = 0; i < MAX_P_BULLETS; i++) pba[i] = 0;
    for (i = 0; i < MAX_E_BULLETS; i++) eba[i] = 0;
    max_p_bullets = 1;
    max_e_bullets = (uint8_t)(3 + (lvl - 1));
    if (max_e_bullets > MAX_E_BULLETS) max_e_bullets = MAX_E_BULLETS;
    enemy_bullets_active = 0;

    invincible_timer = 0;
    freeze_timer = 0;
}

/* ---- Collision helpers ---- */

/* Check if any active enemy bullet overlaps ANY tile of the player (2x2 area) */
static uint8_t e_bullets_hit_player(void)
{
    uint8_t i;
    if (invincible_timer > 0) return 0;
    for (i = 0; i < max_e_bullets; i++) {
        if (eba[i]) {
            if (ebx[i] >= px && ebx[i] <= px + 1 &&
                eby[i] >= py && eby[i] <= py + 1) {
                return 1;
            }
        }
    }
    return 0;
}

/* Check if a bullet position overlaps any alive enemy (2x2 area) */
static uint8_t bullet_hits_enemy(uint8_t bx, uint8_t by)
{
    uint8_t j;
    for (j = 0; j < num_enemies; j++) {
        if (enemy_alive[j]) {
            if (bx >= ex[j] && bx <= ex[j] + 1 &&
                by >= ey[j] && by <= ey[j] + 1) {
                return 1;
            }
        }
    }
    return 0;
}

/* Check if any active player bullet overlaps any alive enemy (2x2 area) */
static uint8_t p_bullets_hit_enemies(void)
{
    uint8_t i, j;
    for (i = 0; i < max_p_bullets; i++) {
        if (!pba[i]) continue;
        for (j = 0; j < num_enemies; j++) {
            if (enemy_alive[j]) {
                if (pbx[i] >= ex[j] && pbx[i] <= ex[j] + 1 &&
                    pby[i] >= ey[j] && pby[i] <= ey[j] + 1) {
                    enemy_alive[j] = 0;
                    enemies_killed++;
                    score += 100;
                    delete_enemy(j);
                    delete_p_bullet(i);
                    pba[i] = 0;
                    draw_hud();
                    _XL_EXPLOSION_SOUND();
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* ---- Fire a player bullet (finds a free slot) ---- */
static uint8_t fire_player_bullet(void)
{
    uint8_t i, count, bx, by;
    count = 0;
    for (i = 0; i < max_p_bullets; i++) {
        if (pba[i]) count++;
    }
    if (count >= max_p_bullets) return 0;

    if (player_dir == 1) {
        bx = (uint8_t)(px + 2);
        by = py;
        if (bx >= SCREEN_W) return 0;
        if (tile_in_wall(bx, by)) return 0;
    } else {
        if (px < 2) return 0;
        bx = (uint8_t)(px - 1);
        by = py;
        if (tile_in_wall(bx, by)) return 0;
    }

    for (i = 0; i < max_p_bullets; i++) {
        if (!pba[i]) {
            pbx[i] = bx;
            pby[i] = by;
            pbd[i] = player_dir;
            pba[i] = 1;
            _XL_DRAW(bx, by, _TILE_26, _XL_YELLOW);
            _XL_SHOOT_SOUND();
            return 1;
        }
    }
    return 0;
}

/* ---- Update all player bullets (checks old AND new tile vs enemies) ---- */
static void update_p_bullets(void)
{
    uint8_t i, j;
    uint8_t old_x, old_y;

    for (i = 0; i < max_p_bullets; i++) {
        if (!pba[i]) continue;
        old_x = pbx[i];
        old_y = pby[i];
        delete_p_bullet(i);

        if (pbd[i] == 1) {
            pbx[i]++;
        } else {
            if (pbx[i] == 0) { pba[i] = 0; continue; }
            pbx[i]--;
        }

        if (pbx[i] >= SCREEN_W || pby[i] >= SCREEN_H) { pba[i] = 0; continue; }
        if (tile_in_wall(pbx[i], pby[i])) { pba[i] = 0; _XL_TICK_SOUND(); continue; }

        /* Check collision at NEW position vs all enemies */
        for (j = 0; j < num_enemies; j++) {
            if (enemy_alive[j]) {
                if (pbx[i] >= ex[j] && pbx[i] <= ex[j] + 1 &&
                    pby[i] >= ey[j] && pby[i] <= ey[j] + 1) {
                    enemy_alive[j] = 0;
                    enemies_killed++;
                    score += 100;
                    delete_enemy(j);
                    draw_hud();
                    _XL_EXPLOSION_SOUND();
                    pba[i] = 0;
                    break;
                }
            }
        }

        /* Check collision at OLD position vs all enemies */
        if (pba[i]) {
            for (j = 0; j < num_enemies; j++) {
                if (enemy_alive[j]) {
                    if (old_x >= ex[j] && old_x <= ex[j] + 1 &&
                        old_y >= ey[j] && old_y <= ey[j] + 1) {
                        enemy_alive[j] = 0;
                        enemies_killed++;
                        score += 100;
                        delete_enemy(j);
                        draw_hud();
                        _XL_EXPLOSION_SOUND();
                        pba[i] = 0;
                        break;
                    }
                }
            }
        }

        if (pba[i]) {
            _XL_DRAW(pbx[i], pby[i], _TILE_26, _XL_YELLOW);
        }
    }
}

/* ---- Fire an enemy bullet toward player ---- */
static void fire_enemy_bullet(uint8_t idx)
{
    uint8_t i, bx, by, dir;
    short dx;
    dx = (short)px - (short)ex[idx];
    if (dx >= 0) {
        dir = 1;
        bx = (uint8_t)(ex[idx] + 2);
    } else {
        dir = 0;
        bx = (uint8_t)(ex[idx] - 1);
    }
    by = (uint8_t)(ey[idx] + 1);

    if (bx >= SCREEN_W || by >= SCREEN_H) return;
    if (tile_in_wall(bx, by)) return;
    if (bullet_hits_enemy(bx, by)) return;

    for (i = 0; i < max_e_bullets; i++) {
        if (!eba[i]) {
            ebx[i] = bx;
            eby[i] = by;
            ebd[i] = dir;
            eba[i] = 1;
            enemy_bullets_active++;
            _XL_DRAW(bx, by, _TILE_20, _XL_CYAN);
            return;
        }
    }
}

/* ---- Update all enemy bullets.
       Returns 1 if the player was hit (and is NOT invincible), 0 otherwise.
       When the player IS invincible, bullets that overlap the player are
       still stopped (disappear) but do NOT set the return to 1. ---- */
static uint8_t update_e_bullets(void)
{
    uint8_t i;
    uint8_t old_x, old_y;
    uint8_t player_hit;

    player_hit = 0;

    for (i = 0; i < max_e_bullets; i++) {
        if (!eba[i]) continue;
        old_x = ebx[i];
        old_y = eby[i];
        delete_e_bullet(i);

        if (ebd[i] == 1) {
            ebx[i]++;
        } else {
            if (ebx[i] == 0) { eba[i] = 0; enemy_bullets_active--; continue; }
            ebx[i]--;
        }

        if (ebx[i] >= SCREEN_W || eby[i] >= SCREEN_H) { eba[i] = 0; enemy_bullets_active--; continue; }
        if (tile_in_wall(ebx[i], eby[i])) { eba[i] = 0; enemy_bullets_active--; _XL_TICK_SOUND(); continue; }

        /* Enemy bullet stopped by enemy at NEW position */
        if (bullet_hits_enemy(ebx[i], eby[i])) {
            eba[i] = 0;
            enemy_bullets_active--;
            _XL_TICK_SOUND();
            continue;
        }

        /* Enemy bullet stopped by enemy at OLD position */
        if (bullet_hits_enemy(old_x, old_y)) {
            eba[i] = 0;
            enemy_bullets_active--;
            _XL_TICK_SOUND();
            continue;
        }

        /* Check collision at NEW position vs player (any tile of 2x2).
           Bullet is always stopped on overlap; only kills if not invincible. */
        if (ebx[i] >= px && ebx[i] <= px + 1 &&
            eby[i] >= py && eby[i] <= py + 1) {
            eba[i] = 0;
            enemy_bullets_active--;
            if (invincible_timer == 0) {
                player_hit = 1;
            }
            continue;
        }

        /* Check collision at OLD position vs player (any tile of 2x2).
           Bullet is always stopped on overlap; only kills if not invincible. */
        if (old_x >= px && old_x <= px + 1 &&
            old_y >= py && old_y <= py + 1) {
            eba[i] = 0;
            enemy_bullets_active--;
            if (invincible_timer == 0) {
                player_hit = 1;
            }
            continue;
        }

        _XL_DRAW(ebx[i], eby[i], _TILE_20, _XL_CYAN);
    }
    return player_hit;
}

/* ---- Update enemies ---- */
static void update_enemies(void)
{
    uint8_t i;
    uint16_t rnd;
    uint8_t new_x, new_y, can, rd;
    short dx, dy;

    if (freeze_timer > 0) return;

    for (i = 0; i < num_enemies; i++) {
        if (!enemy_alive[i]) continue;

        dx = (short)px - (short)ex[i];
        dy = (short)py - (short)ey[i];
        rnd = _XL_RAND();

        if (rnd % 10 < 6) {
            if (dx > 0) {
                new_x = (uint8_t)(ex[i] + 1);
                new_y = ey[i];
                can = can_enemy_move(i, new_x, new_y);
                if (can) { delete_enemy(i); ex[i] = new_x; enemy_dir[i] = 1; draw_enemy(i); }
            } else if (dx < 0) {
                new_x = (uint8_t)(ex[i] - 1);
                new_y = ey[i];
                can = can_enemy_move(i, new_x, new_y);
                if (can) { delete_enemy(i); ex[i] = new_x; enemy_dir[i] = 0; draw_enemy(i); }
            } else if (dy > 0) {
                new_x = ex[i];
                new_y = (uint8_t)(ey[i] + 1);
                can = can_enemy_move(i, new_x, new_y);
                if (can) { delete_enemy(i); ey[i] = new_y; draw_enemy(i); }
            } else if (dy < 0) {
                new_x = ex[i];
                new_y = (uint8_t)(ey[i] - 1);
                can = can_enemy_move(i, new_x, new_y);
                if (can) { delete_enemy(i); ey[i] = new_y; draw_enemy(i); }
            }
        } else {
            rd = (uint8_t)(rnd % 4);
            if (rd == 0) {
                if (ey[i] > 0) {
                    can = can_enemy_move(i, ex[i], (uint8_t)(ey[i] - 1));
                    if (can) { delete_enemy(i); ey[i]--; draw_enemy(i); }
                }
            } else if (rd == 1) {
                can = can_enemy_move(i, ex[i], (uint8_t)(ey[i] + 1));
                if (can) { delete_enemy(i); ey[i]++; draw_enemy(i); }
            } else if (rd == 2) {
                can = can_enemy_move(i, (uint8_t)(ex[i] + 1), ey[i]);
                if (can) { delete_enemy(i); ex[i]++; enemy_dir[i] = 1; draw_enemy(i); }
            } else {
                if (ex[i] > 0) {
                    can = can_enemy_move(i, (uint8_t)(ex[i] - 1), ey[i]);
                    if (can) { delete_enemy(i); ex[i]--; enemy_dir[i] = 0; draw_enemy(i); }
                }
            }
        }
    }
}

/* ---- Check enemy body collision with player ---- */
static uint8_t check_player_death(void)
{
    uint8_t i;
    if (invincible_timer > 0) return 0;
    for (i = 0; i < num_enemies; i++) {
        if (enemy_alive[i] && entities_overlap(px, py, ex[i], ey[i])) {
            return 1;
        }
    }
    return 0;
}

/* ---- Check item pickup ---- */
static void check_items(void)
{
    uint8_t i;
    for (i = 0; i < 3; i++) {
        if (!item_active[i]) continue;
        if (item_x[i] >= px && item_x[i] <= px + 1 &&
            item_y[i] >= py && item_y[i] <= py + 1) {
            delete_item(i);
            item_active[i] = 0;
            if (i == 0) {
                if (max_p_bullets < MAX_P_BULLETS) max_p_bullets++;
                _XL_ZAP_SOUND();
            } else if (i == 1) {
                invincible_timer = INVINCIBLE_FRAMES;
                _XL_ZAP_SOUND();
            } else {
                freeze_timer = FREEZE_FRAMES;
                _XL_ZAP_SOUND();
            }
            draw_hud();
        }
    }
}

/* ---- Show title screen ---- */
static void show_title(void)
{
    uint8_t input;
    for (;;) {
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(10, 3, "CROSS");
        _XL_PRINT(10, 5, "RUNNER");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(6, 8, "KILL ENEMIES");
        _XL_PRINT(6, 9, "COLLECT ITEMS");
        _XL_PRINT(6, 10, "SURVIVE 8 LEVELS");
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(10, 13, "PRESS FIRE");
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 2);
        for (;;) {
            input = _XL_INPUT();
            if (_XL_FIRE(input)) return;
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }
}

static void show_level_intro(uint8_t enemy_count)
{
    uint8_t input;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(12, 3, "LEVEL ");
    _XL_PRINTD(20, 3, 1, level);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(8, 5, "ENEMIES: ");
    _XL_PRINTD(18, 5, 2, (uint16_t)enemy_count);
    _XL_PRINT(8, 6, "SCORE:   ");
    _XL_PRINTD(18, 6, 4, score);
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(8, 7, "LIVES:   ");
    _XL_PRINTD(18, 7, 1, lives);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(8, 12, "PRESS FIRE TO START");
    for (;;) {
        input = _XL_INPUT();
        if (_XL_FIRE(input)) break;
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
    _XL_CLEAR_SCREEN();
}

static void show_life_lost(void)
{
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(10, 5, "LIFE LOST!");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(8, 7, "LIVES LEFT: ");
    _XL_PRINTD(22, 7, 1, lives);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(8, 10, "RESTARTING...");
    _XL_TICK_SOUND();
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 60);
    _XL_CLEAR_SCREEN();
}

static void show_game_over(void)
{
    uint8_t input;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(12, 5, "GAME OVER");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(12, 7, "SCORE:");
    _XL_PRINTD(20, 7, 4, score);
    _XL_EXPLOSION_SOUND();
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 60);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(8, 11, "PRESS FIRE");
    for (;;) {
        input = _XL_INPUT();
        if (_XL_FIRE(input)) break;
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

static void show_level_complete(void)
{
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(10, 5, "LEVEL ");
    _XL_PRINTD(18, 5, 1, level);
    _XL_PRINT(8, 7, "COMPLETE!");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(10, 10, "NEXT LEVEL...");
    _XL_ZAP_SOUND();
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 60);
}

static void show_congratulations(void)
{
    uint8_t input;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(8, 3, "CONGRATULATIONS!");
    _XL_PRINT(8, 5, "ALL LEVELS");
    _XL_PRINT(8, 6, "COMPLETE!");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(10, 9, "FINAL SCORE:");
    _XL_PRINTD(21, 9, 4, score);
    _XL_ZAP_SOUND();
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 80);
    _XL_EXPLOSION_SOUND();
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 20);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(8, 13, "PRESS FIRE");
    for (;;) {
        input = _XL_INPUT();
        if (_XL_FIRE(input)) break;
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

/* ---- Main ---- */
int main(void)
{
    uint8_t input;
    uint8_t enemy_interval;
    uint8_t fire_interval;
    uint8_t can;
    uint8_t died;
    uint8_t total_enemies;
    uint8_t remaining;
    uint8_t lvl;
    uint8_t do_game_over;
    uint8_t i;
    uint8_t e_bullet_hit;
    uint16_t enemy_fire_count;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    for (;;) {
        show_title();

        score = 0;
        lives = START_LIVES;
        do_game_over = 0;

        for (lvl = 1; lvl <= MAX_LEVELS && !do_game_over; lvl++) {
            total_enemies = (uint8_t)(8 + lvl);
            remaining = total_enemies;

            for (;;) {
                show_level_intro(remaining);

                setup_level(lvl, remaining);
                draw_maze();
                draw_player();
                draw_all_enemies();
                draw_all_items();
                draw_hud();
                draw_p_bullets();
                draw_e_bullets();

                frame_count = 0;
                enemy_fire_count = 0;

                enemy_interval = (uint8_t)(10 - lvl);
                if (enemy_interval < 2) enemy_interval = 2;
                fire_interval = (uint8_t)(20 - lvl * 2);
                if (fire_interval < 5) fire_interval = 5;

                /* ===================== Level play loop ===================== */
                for (;;) {
                    input = _XL_INPUT();
                    died = 0;

                    /* Decrement timers */
                    if (invincible_timer > 0) invincible_timer--;
                    if (freeze_timer > 0) freeze_timer--;

                    /* === BEFORE PLAYER MOVES: check enemy bullets vs player === */
                    if (e_bullets_hit_player()) {
                        died = 1;
                    }

                    /* === PLAYER HORIZONTAL: move if same dir, turn if opposite === */
                    if (!died) {
                        if (_XL_LEFT(input)) {
                            if (player_dir == 0) {
                                if (px > 0) {
                                    can = can_player_move((uint8_t)(px - 1), py);
                                    if (can) {
                                        delete_player();
                                        px--;
                                        draw_player();
                                        _XL_PING_SOUND();
                                    }
                                }
                            } else {
                                player_dir = 0;
                                delete_player();
                                draw_player();
                            }
                        }
                        if (_XL_RIGHT(input)) {
                            if (player_dir == 1) {
                                if (px + 1 < SCREEN_W) {
                                    can = can_player_move((uint8_t)(px + 1), py);
                                    if (can) {
                                        delete_player();
                                        px++;
                                        draw_player();
                                        _XL_PING_SOUND();
                                    }
                                }
                            } else {
                                player_dir = 1;
                                delete_player();
                                draw_player();
                            }
                        }
                    }

                    /* === PLAYER VERTICAL: move while held === */
                    if (!died) {
                        if (_XL_UP(input)) {
                            if (py > 0) {
                                can = can_player_move(px, (uint8_t)(py - 1));
                                if (can) {
                                    delete_player();
                                    py--;
                                    draw_player();
                                    _XL_PING_SOUND();
                                }
                            }
                        }
                        if (_XL_DOWN(input)) {
                            if (py + 1 < SCREEN_H) {
                                can = can_player_move(px, (uint8_t)(py + 1));
                                if (can) {
                                    delete_player();
                                    py++;
                                    draw_player();
                                    _XL_PING_SOUND();
                                }
                            }
                        }
                    }

                    /* === AFTER PLAYER MOVES: check enemy bullets vs player === */
                    if (!died && e_bullets_hit_player()) {
                        died = 1;
                    }

                    /* === AFTER PLAYER MOVES: check enemy body vs player === */
                    if (!died && check_player_death()) {
                        died = 1;
                    }

                    /* === FIRE PLAYER BULLET === */
                    if (!died && _XL_FIRE(input)) {
                        fire_player_bullet();
                    }

                    /* === BEFORE ENEMIES MOVE: update player bullets === */
                    if (!died) {
                        update_p_bullets();
                    }

                    /* === BEFORE ENEMIES MOVE: check player bullets vs enemies === */
                    if (!died) {
                        p_bullets_hit_enemies();
                    }

                    /* === ENEMIES MOVE === */
                    if (!died) {
                        frame_count++;
                        if (frame_count >= enemy_interval) {
                            frame_count = 0;
                            update_enemies();
                            /* Redraw items that were covered by enemies which moved away */
                            redraw_uncovered_items();
                        }
                    }

                    /* === AFTER ENEMIES MOVE: check player bullets vs enemies === */
                    if (!died) {
                        p_bullets_hit_enemies();
                    }

                    /* === AFTER ENEMIES MOVE: check enemy body vs player === */
                    if (!died && check_player_death()) {
                        died = 1;
                    }

                    /* === ENEMY FIRING === */
                    if (!died) {
                        enemy_fire_count++;
                        if (enemy_fire_count >= fire_interval) {
                            enemy_fire_count = 0;
                            if (enemy_bullets_active < max_e_bullets) {
                                i = (uint8_t)(_XL_RAND() % num_enemies);
                                if (enemy_alive[i]) {
                                    fire_enemy_bullet(i);
                                }
                            }
                        }
                    }

                    /* === UPDATE ENEMY BULLETS (stopped by enemies, hits player) === */
                    if (!died) {
                        e_bullet_hit = update_e_bullets();
                        if (e_bullet_hit) {
                            died = 1;
                        }
                    }

                    /* === AFTER ENEMY BULLETS: check vs player (safety net) === */
                    if (!died && e_bullets_hit_player()) {
                        died = 1;
                    }

                    /* === CHECK ITEMS === */
                    if (!died) {
                        check_items();
                    }

                    /* Level complete? */
                    if (enemies_killed >= num_enemies) {
                        break;
                    }

                    /* Player died */
                    if (died) {
                        break;
                    }

                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                }
                /* ==================== End level play loop ==================== */

                /* Handle death */
                if (died) {
                    lives--;
                    if (lives == 0) {
                        show_game_over();
                        do_game_over = 1;
                        break;
                    } else {
                        show_life_lost();
                        remaining = (uint8_t)(total_enemies - enemies_killed);
                        if (remaining == 0) remaining = 1;
                        continue;
                    }
                }

                /* Level complete */
                break;
            }

            if (!do_game_over && lvl < MAX_LEVELS) {
                show_level_complete();
            }
        }

        if (!do_game_over) {
            show_congratulations();
        }
    }

    return 0;
}

