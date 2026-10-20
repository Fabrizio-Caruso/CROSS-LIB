#include "cross_lib.h"

#define MAX_ENEMIES   16
#define MAX_LEVELS    8
#define LOG_COLS      16
#define LOG_ROWS      8
#define HALF_COLS     8
#define SCREEN_W      32
#define SCREEN_H      16
#define MIN_ENEMY_DIST 6

/*
 * Maze layout: 16 cols x 8 rows of logical cells, each cell = 2x2 screen tiles.
 * Left half is 8 cols x 8 rows. The full maze is built by mirroring:
 *   full row = [c0 c1 c2 c3 c4 c5 c6 c7 | c7 c6 c5 c4 c3 c2 c1 c0]
 * The mirror axis is between columns 7 and 8.
 * All open cells in each level are mutually reachable.
 */

/* ---- Maze data: left half (8 cols x 8 rows), indexed [level][col][row] ---- */
static const uint8_t maze_left[MAX_LEVELS][HALF_COLS][LOG_ROWS] = {
    /* Level 1 - Open Arena (9 enemies): fully open interior */
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
    /* Level 2 - Vertical Bars (10 enemies): isolated 1-wide pillars */
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
    /* Level 3 - Pillars (11 enemies): single-block obstacles */
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
    /* Level 4 - Cross (12 enemies): short vertical bars */
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
    /* Level 5 - Nested (13 enemies): paired pillars */
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
    /* Level 6 - Center Block (14 enemies): 2-wide center obstacle */
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
    /* Level 7 - Dense Corridors (15 enemies): alternating pillars */
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
    /* Level 8 - Labyrinth (16 enemies): scattered blocks, all passable */
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

/* Full 16x8 logical maze (mirrored) */
static uint8_t full_maze[LOG_ROWS][LOG_COLS];

/* Game state - positions in SCREEN coordinates (top-left of 2x2 entity) */
static uint8_t  level;
static uint8_t  px, py;
static uint8_t  player_dir;
static uint8_t  ex[MAX_ENEMIES];
static uint8_t  ey[MAX_ENEMIES];
static uint8_t  enemy_dir[MAX_ENEMIES];
static uint8_t  enemy_alive[MAX_ENEMIES];
static uint8_t  num_enemies;
static uint8_t  enemies_killed;
static uint8_t  bullet_active;
static uint8_t  bx, by;
static uint8_t  bullet_dir;
static uint8_t  player_dead;
static uint16_t frame_count;
static uint16_t score;

/*
 * Build full 16x8 maze by mirroring the 8-col left half.
 * Axis of symmetry is between columns 7 and 8:
 *   full_maze[r][0..7]  = left half cols 0..7
 *   full_maze[r][8..15] = left half cols 7..0  (reversed)
 */
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

/* ---- Check if a single screen tile (sx,sy) is inside a wall block ---- */
static uint8_t tile_in_wall(uint8_t sx, uint8_t sy)
{
    uint8_t cx, cy;
    if (sx >= SCREEN_W || sy >= SCREEN_H) return 1;
    cx = (uint8_t)(sx / 2);
    cy = (uint8_t)(sy / 2);
    return (full_maze[cy][cx] == 1);
}

/* ---- Check if a 2x2 entity at screen (sx,sy) overlaps any wall block ---- */
static uint8_t entity_in_wall(uint8_t sx, uint8_t sy)
{
    if (sx + 1 >= SCREEN_W || sy + 1 >= SCREEN_H) return 1;
    if (tile_in_wall(sx, sy)) return 1;
    if (tile_in_wall(sx + 1, sy)) return 1;
    if (tile_in_wall(sx, sy + 1)) return 1;
    if (tile_in_wall(sx + 1, sy + 1)) return 1;
    return 0;
}

/* ---- Check if two 2x2 entities overlap ---- */
static uint8_t entities_overlap(uint8_t ax, uint8_t ay, uint8_t bx, uint8_t by)
{
    if (ax > bx + 1) return 0;
    if (bx > ax + 1) return 0;
    if (ay > by + 1) return 0;
    if (by > ay + 1) return 0;
    return 1;
}

/* ---- Check if player can move 1 tile to screen position (sx,sy) ---- */
static uint8_t can_player_move(uint8_t sx, uint8_t sy)
{
    uint8_t i;
    if (entity_in_wall(sx, sy)) return 0;
    for (i = 0; i < num_enemies; i++) {
        if (enemy_alive[i] && entities_overlap(sx, sy, ex[i], ey[i])) return 0;
    }
    return 1;
}

/* ---- Check if enemy idx can move 1 tile to screen position (sx,sy) ---- */
static uint8_t can_enemy_move(uint8_t idx, uint8_t sx, uint8_t sy)
{
    uint8_t i;
    if (entity_in_wall(sx, sy)) return 0;
    if (entities_overlap(sx, sy, px, py)) return 0;
    for (i = 0; i < num_enemies; i++) {
        if (i != idx && enemy_alive[i] && entities_overlap(sx, sy, ex[i], ey[i])) return 0;
    }
    return 1;
}

/* ---- Draw a 2x2 wall block at logical cell (cx,cy) ---- */
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

/* ---- Draw all maze walls ---- */
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

/* ---- Draw player 2x2 at screen (px,py) ---- */
static void draw_player(void)
{
    if (player_dir == 0) {
        _XL_DRAW(px,     py,     _TILE_1, _XL_GREEN);
        _XL_DRAW(px + 1, py,     _TILE_2, _XL_GREEN);
        _XL_DRAW(px,     py + 1, _TILE_3, _XL_GREEN);
        _XL_DRAW(px + 1, py + 1, _TILE_4, _XL_GREEN);
    } else {
        _XL_DRAW(px,     py,     _TILE_5, _XL_GREEN);
        _XL_DRAW(px + 1, py,     _TILE_6, _XL_GREEN);
        _XL_DRAW(px,     py + 1, _TILE_7, _XL_GREEN);
        _XL_DRAW(px + 1, py + 1, _TILE_8, _XL_GREEN);
    }
}

/* ---- Delete player 2x2 at screen (px,py) ---- */
static void delete_player(void)
{
    _XL_DELETE(px,     py);
    _XL_DELETE(px + 1, py);
    _XL_DELETE(px,     py + 1);
    _XL_DELETE(px + 1, py + 1);
}

/* ---- Draw one enemy 2x2 at screen (ex[i],ey[i]) ---- */
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

/* ---- Delete one enemy 2x2 at screen (ex[i],ey[i]) ---- */
static void delete_enemy(uint8_t i)
{
    _XL_DELETE(ex[i],     ey[i]);
    _XL_DELETE(ex[i] + 1, ey[i]);
    _XL_DELETE(ex[i],     ey[i] + 1);
    _XL_DELETE(ex[i] + 1, ey[i] + 1);
}

/* ---- Draw all alive enemies ---- */
static void draw_all_enemies(void)
{
    uint8_t i;
    for (i = 0; i < num_enemies; i++) {
        if (enemy_alive[i]) draw_enemy(i);
    }
}

/* ---- Place player at first valid open screen position ---- */
static void place_player(void)
{
    uint8_t r, c;
    uint8_t found;
    found = 0;
    for (r = 0; r + 1 < SCREEN_H && !found; r++) {
        for (c = 0; c + 1 < SCREEN_W && !found; c++) {
            if (!entity_in_wall(c, r)) {
                px = c;
                py = r;
                found = 1;
            }
        }
    }
    player_dir = 1;
}

/* ---- Place enemies at random valid screen positions far from player ---- */
static void place_enemies(void)
{
    uint8_t i, j, k, attempts, valid, ok;
    uint16_t rx, ry;
    uint8_t dist;
    num_enemies = (uint8_t)(8 + level);
    if (num_enemies > MAX_ENEMIES) num_enemies = MAX_ENEMIES;
    enemies_killed = 0;
    for (i = 0; i < num_enemies; i++) {
        enemy_alive[i] = 1;
        enemy_dir[i] = 1;
    }
    for (i = 0; i < num_enemies; i++) {
        attempts = 0;
        do {
            rx = (uint16_t)(_XL_RAND() % (SCREEN_W - 2));
            ry = (uint16_t)(_XL_RAND() % (SCREEN_H - 2));
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
            valid = 0;
            for (j = 1; j + 1 < SCREEN_W && !valid; j++) {
                for (k = 1; k + 1 < SCREEN_H; k++) {
                    dist = (uint8_t)(((j > px) ? (uint8_t)(j - px) : (uint8_t)(px - j)) +
                                      ((k > py) ? (uint8_t)(k - py) : (uint8_t)(py - k)));
                    if (dist < MIN_ENEMY_DIST) continue;
                    if (entity_in_wall(j, k)) continue;
                    if (entities_overlap(j, k, px, py)) continue;
                    ok = 1;
                    for (k = 0; k < i; k++) {
                        if (entities_overlap(j, k, ex[k], ey[k])) {
                            ok = 0;
                            break;
                        }
                    }
                    if (ok) {
                        ex[i] = j;
                        ey[i] = k;
                        valid = 1;
                        break;
                    }
                }
            }
        }
    }
}

/* ---- Update bullet (moves horizontally, never erases wall tiles) ---- */
static void update_bullet(void)
{
    uint8_t i;

    /* Delete bullet tile only if it is NOT a wall */
    if (!tile_in_wall(bx, by)) {
        _XL_DELETE(bx, by);
    }

    /* Step horizontally */
    if (bullet_dir == 1) {
        bx++;
    } else {
        if (bx == 0) {
            bullet_active = 0;
            return;
        }
        bx--;
    }

    if (bx >= SCREEN_W || by >= SCREEN_H) {
        bullet_active = 0;
        return;
    }

    /* Hit wall: deactivate without touching wall tiles */
    if (tile_in_wall(bx, by)) {
        bullet_active = 0;
        _XL_TICK_SOUND();
        return;
    }

    /* Hit enemy */
    for (i = 0; i < num_enemies; i++) {
        if (enemy_alive[i]) {
            if (bx >= ex[i] && bx <= (uint8_t)(ex[i] + 1) &&
                by >= ey[i] && by <= (uint8_t)(ey[i] + 1)) {
                enemy_alive[i] = 0;
                enemies_killed++;
                score += 100;
                delete_enemy(i);
                _XL_EXPLOSION_SOUND();
                bullet_active = 0;
                return;
            }
        }
    }

    _XL_DRAW(bx, by, _TILE_26, _XL_YELLOW);
}

/* ---- Update all alive enemies (1-tile steps, simple AI) ---- */
static void update_enemies(void)
{
    uint8_t i;
    uint16_t rnd;
    uint8_t new_x, new_y, can, rd;
    short dx, dy;

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
                if (can) {
                    delete_enemy(i);
                    ex[i] = new_x;
                    enemy_dir[i] = 1;
                    draw_enemy(i);
                }
            } else if (dx < 0) {
                new_x = (uint8_t)(ex[i] - 1);
                new_y = ey[i];
                can = can_enemy_move(i, new_x, new_y);
                if (can) {
                    delete_enemy(i);
                    ex[i] = new_x;
                    enemy_dir[i] = 0;
                    draw_enemy(i);
                }
            } else if (dy > 0) {
                new_x = ex[i];
                new_y = (uint8_t)(ey[i] + 1);
                can = can_enemy_move(i, new_x, new_y);
                if (can) {
                    delete_enemy(i);
                    ey[i] = new_y;
                    draw_enemy(i);
                }
            } else if (dy < 0) {
                new_x = ex[i];
                new_y = (uint8_t)(ey[i] - 1);
                can = can_enemy_move(i, new_x, new_y);
                if (can) {
                    delete_enemy(i);
                    ey[i] = new_y;
                    draw_enemy(i);
                }
            }
        } else {
            rd = (uint8_t)(rnd % 4);
            if (rd == 0) {
                if (ey[i] > 0) {
                    can = can_enemy_move(i, ex[i], (uint8_t)(ey[i] - 1));
                    if (can) {
                        delete_enemy(i);
                        ey[i]--;
                        draw_enemy(i);
                    }
                }
            } else if (rd == 1) {
                can = can_enemy_move(i, ex[i], (uint8_t)(ey[i] + 1));
                if (can) {
                    delete_enemy(i);
                    ey[i]++;
                    draw_enemy(i);
                }
            } else if (rd == 2) {
                can = can_enemy_move(i, (uint8_t)(ex[i] + 1), ey[i]);
                if (can) {
                    delete_enemy(i);
                    ex[i]++;
                    enemy_dir[i] = 1;
                    draw_enemy(i);
                }
            } else {
                if (ex[i] > 0) {
                    can = can_enemy_move(i, (uint8_t)(ex[i] - 1), ey[i]);
                    if (can) {
                        delete_enemy(i);
                        ex[i]--;
                        enemy_dir[i] = 0;
                        draw_enemy(i);
                    }
                }
            }
        }
    }
}

/* ---- Check if any alive enemy overlaps the player ---- */
static uint8_t check_player_death(void)
{
    uint8_t i;
    for (i = 0; i < num_enemies; i++) {
        if (enemy_alive[i] && entities_overlap(px, py, ex[i], ey[i])) {
            return 1;
        }
    }
    return 0;
}

/* ---- Show game over, wait for FIRE ---- */
static void show_game_over(void)
{
    uint8_t input;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(12, 4, "GAME OVER");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(12, 6, "SCORE:");
    _XL_PRINTD(20, 6, 4, score);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(8, 10, "PRESS FIRE TO RESTART");
    _XL_EXPLOSION_SOUND();
    for (;;) {
        input = _XL_INPUT();
        if (_XL_FIRE(input)) break;
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

/* ---- Show level intro on clear screen, wait for FIRE ---- */
static void show_level_intro(void)
{
    uint8_t input;
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(12, 4, "LEVEL ");
    _XL_PRINTD(20, 4, 2, level);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(10, 6, "ENEMIES: ");
    _XL_PRINTD(20, 6, 2, (uint16_t)(8 + level));
    if (level > 1) {
        _XL_PRINT(10, 7, "SCORE:   ");
        _XL_PRINTD(20, 7, 4, score);
    }
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(8, 12, "PRESS FIRE TO START");
    for (;;) {
        input = _XL_INPUT();
        if (_XL_FIRE(input)) break;
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
    _XL_CLEAR_SCREEN();
}

/* ---- Setup a new level ---- */
static void setup_level(uint8_t lvl)
{
    level = lvl;
    build_maze(lvl - 1);
    place_player();
    place_enemies();
}

/* ---- Main entry point ---- */
int main(void)
{
    uint8_t input;
    uint8_t enemy_interval;
    uint8_t can;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    for (;;) {
        score = 0;

        for (level = 1; level <= MAX_LEVELS; level++) {
            show_level_intro();

            setup_level(level);
            draw_maze();
            draw_player();
            draw_all_enemies();
            bullet_active = 0;
            player_dead = 0;
            frame_count = 0;

            enemy_interval = (uint8_t)(10 - level);
            if (enemy_interval < 2) enemy_interval = 2;

            /* Level play loop */
            for (;;) {
                input = _XL_INPUT();

                /* Player movement: 1 tile per press */
                if (_XL_LEFT(input)) {
                    if (px > 0) {
                        can = can_player_move((uint8_t)(px - 1), py);
                        if (can) {
                            delete_player();
                            px--;
                            player_dir = 0;
                            draw_player();
                        }
                        _XL_PING_SOUND();
                    }
                }
                if (_XL_RIGHT(input)) {
                    if (px + 1 < SCREEN_W) {
                        can = can_player_move((uint8_t)(px + 1), py);
                        if (can) {
                            delete_player();
                            px++;
                            player_dir = 1;
                            draw_player();
                        }
                        _XL_PING_SOUND();
                    }
                }
                if (_XL_UP(input)) {
                    if (py > 0) {
                        can = can_player_move(px, (uint8_t)(py - 1));
                        if (can) {
                            delete_player();
                            py--;
                            draw_player();
                        }
                        _XL_PING_SOUND();
                    }
                }
                if (_XL_DOWN(input)) {
                    if (py + 1 < SCREEN_H) {
                        can = can_player_move(px, (uint8_t)(py + 1));
                        if (can) {
                            delete_player();
                            py++;
                            draw_player();
                        }
                        _XL_PING_SOUND();
                    }
                }

                /* Fire bullet horizontally from top of player */
                if (_XL_FIRE(input)) {
                    if (!bullet_active) {
                        if (player_dir == 1) {
                            if (px + 2 < SCREEN_W) {
                                bx = (uint8_t)(px + 2);
                                by = py;
                                if (!tile_in_wall(bx, by)) {
                                    bullet_dir = 1;
                                    bullet_active = 1;
                                    _XL_DRAW(bx, by, _TILE_26, _XL_YELLOW);
                                    _XL_SHOOT_SOUND();
                                }
                            }
                        } else {
                            if (px > 0) {
                                bx = (uint8_t)(px - 1);
                                by = py;
                                if (!tile_in_wall(bx, by)) {
                                    bullet_dir = 0;
                                    bullet_active = 1;
                                    _XL_DRAW(bx, by, _TILE_26, _XL_YELLOW);
                                    _XL_SHOOT_SOUND();
                                }
                            }
                        }
                    }
                }

                /* Update bullet */
                if (bullet_active) {
                    update_bullet();
                }

                /* Update enemies on timer */
                frame_count++;
                if (frame_count >= enemy_interval) {
                    frame_count = 0;
                    update_enemies();
                }

                /* Check player death */
                if (check_player_death()) {
                    player_dead = 1;
                    break;
                }

                /* Level complete? */
                if (enemies_killed >= num_enemies) {
                    break;
                }

                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }

            /* Player died - restart game */
            if (player_dead) {
                show_game_over();
                score = 0;
                level = 0;
                continue;
            }

            /* Level complete transition */
            if (level < MAX_LEVELS) {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(10, 5, "LEVEL ");
                _XL_PRINTD(18, 5, 2, level);
                _XL_PRINT(8, 7, "COMPLETE!");
                _XL_SET_TEXT_COLOR(_XL_WHITE);
                _XL_PRINT(10, 10, "NEXT LEVEL...");
                _XL_ZAP_SOUND();
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 60);
            }
        }

        /* Game complete */
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
    }

    return 0;
}


