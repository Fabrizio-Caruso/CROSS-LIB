#include "cross_lib.h"

/* ===================== Constants ===================== */
#define MAX_ENEMIES       20
#define MAX_BULLETS       40
#define TANK_HP           9
#define INITIAL_AMMO      30
#define KILL_REWARD       10
#define ITEM_REWARD       5
#define MAX_WALLS         25
#define MAX_WALL_LEN      8
#define MIN_WALL_GAP      3
#define MAX_LEVEL_ENEMIES 20
#define FIRE_COOLDOWN     5
#define ENEMY_FIRE_CD     12
#define ENEMY_MOVE_CD     8

/* Direction vectors: N, NE, E, SE, S, SW, W, NW */
static const short DX[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
static const short DY[8] = {-1, -1, 0, 1, 1,  1,  0, -1 };

/* Screen buffer state values */
#define SB_EMPTY   0
#define SB_PLAYER  1
#define SB_ENEMY   2
#define SB_WALL    3
#define SB_BULLET  4
#define SB_ITEM    5

/* ===================== Types ===================== */
typedef struct {
    uint8_t x, y;
    uint8_t dir;
    uint8_t hp;
    uint8_t alive;
} Tank;

typedef struct {
    uint8_t x, y;
    uint8_t dir;
    uint8_t alive;
    uint8_t from_player;
} Bullet;

typedef struct {
    uint8_t x, y;
    uint8_t len;
    uint8_t horiz;
} WallSeg;

/* ===================== Globals ===================== */
static Tank     g_player;
static Tank     g_enemies[MAX_ENEMIES];
static uint8_t  g_num_enemies;
static Bullet   g_bullets[MAX_BULLETS];
static uint8_t  g_player_ammo;
static uint8_t  g_fire_cd;
static WallSeg  g_walls[MAX_WALLS];
static uint8_t  g_num_walls;
static uint8_t  g_level;
static uint16_t g_score;
static uint8_t  g_item_x, g_item_y, g_item_alive;
static uint8_t  g_screen[YSize][XSize];
static uint8_t  g_game_over;
static uint8_t  g_level_done;
static uint8_t  g_frame_count;
static uint8_t  g_enemy_cd[MAX_ENEMIES];
static uint8_t  g_enemy_mcd[MAX_ENEMIES];

/* ===================== Prototypes ===================== */
static void     init_level(void);
static void     generate_walls(void);
static void     place_enemy(uint8_t idx);
static void     spawn_item(void);
static uint8_t  is_wall_tile(uint8_t x, uint8_t y);
static uint8_t  is_valid_tank_pos(uint8_t x, uint8_t y);
static uint8_t  tank_blocked(uint8_t x, uint8_t y, uint8_t skip_enemy);
static void     move_tank(Tank *t, uint8_t dir, uint8_t skip_enemy);
static void     fire_bullet(Tank *t, uint8_t from_player);
static void     reset_bullets(void);
static void     update_bullets(void);
static void     update_enemies(void);
static void     update_player(uint8_t input);
static void     update_item(void);
static void     draw_screen(void);
static void     draw_hud(void);
static void     sb_draw(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id, uint8_t sb_id);
static void     sb_erase(uint8_t x, uint8_t y);
static void     sb_erase_tank(Tank *t);
static void     sb_draw_tank(Tank *t, uint8_t tile_id, uint8_t color_id, uint8_t sb_id);
static uint8_t  cheb_dist(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
static uint8_t  wall_gap_ok(uint8_t x, uint8_t y, uint8_t len, uint8_t horiz);
static uint8_t  get_num_enemies_for_level(uint8_t lvl);
static uint8_t  count_alive_enemies(void);

/* ===================== Main ===================== */
int main(void)
{
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    g_level = 0;
    g_score = 0;

    for (;;) {
        g_level++;
        g_score = 0;
        g_player_ammo = INITIAL_AMMO;
        g_fire_cd = 0;
        g_frame_count = 0;
        g_game_over = 0;
        g_level_done = 0;

        init_level();

        while (!g_game_over && !g_level_done) {
            g_frame_count++;
            input = _XL_INPUT();

            update_player(input);
            update_enemies();
            update_bullets();
            update_item();

            if (g_fire_cd > 0) {
                g_fire_cd--;
            }

            draw_screen();
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* End-of-level / game-over screen */
        _XL_CLEAR_SCREEN();
        if (g_level_done) {
            _XL_SET_TEXT_COLOR(_XL_GREEN);
            _XL_PRINT(5, 10, "LEVEL COMPLETE");
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(5, 12, "PRESS FIRE TO CONTINUE");
            _XL_SET_TEXT_COLOR(_XL_YELLOW);
            _XL_PRINT(5, 14, "SCORE:");
            _XL_PRINTD(11, 14, 4, g_score);
        } else {
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(5, 10, "GAME OVER");
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(5, 12, "PRESS FIRE TO CONTINUE");
        }

        _XL_WAIT_FOR_INPUT();
        for (;;) {
            input = _XL_INPUT();
            if (_XL_FIRE(input)) {
                break;
            }
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }

    return 0;
}

/* ===================== Level Init ===================== */
static void init_level(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t ec;

    /* Clear screen buffer and physical screen */
    for (i = 0; i < YSize; i++) {
        for (j = 0; j < XSize; j++) {
            g_screen[i][j] = SB_EMPTY;
        }
    }
    _XL_CLEAR_SCREEN();

    /* Walls */
    g_num_walls = 0;
    generate_walls();

    /* Player */
    g_player.x = 2;
    g_player.y = (uint8_t)(YSize / 2);
    g_player.dir = 2;
    g_player.hp = TANK_HP;
    g_player.alive = 1;

    /* Enemies */
    ec = get_num_enemies_for_level(g_level);
    g_num_enemies = 0;
    for (i = 0; i < ec; i++) {
        place_enemy(i);
        g_num_enemies++;
        g_enemy_cd[i] = (uint8_t)(ENEMY_FIRE_CD + _XL_RAND() % 6);
        g_enemy_mcd[i] = (uint8_t)(_XL_RAND() % ENEMY_MOVE_CD);
    }

    reset_bullets();
    spawn_item();
}

static uint8_t get_num_enemies_for_level(uint8_t lvl)
{
    uint8_t n;
    n = (uint8_t)(lvl + 1);
    if (n > MAX_LEVEL_ENEMIES) {
        n = MAX_LEVEL_ENEMIES;
    }
    return n;
}

/* ===================== Wall Generation ===================== */
static uint8_t cheb_dist(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    short dx, dy;
    uint8_t d;
    dx = (short)x1 - (short)x2;
    if (dx < 0) { dx = (short)(-dx); }
    dy = (short)y1 - (short)y2;
    if (dy < 0) { dy = (short)(-dy); }
    if (dx > (short)dy) {
        d = (uint8_t)dx;
    } else {
        d = (uint8_t)dy;
    }
    return d;
}

static uint8_t is_wall_tile(uint8_t x, uint8_t y)
{
    uint8_t i, j;
    for (i = 0; i < g_num_walls; i++) {
        if (g_walls[i].horiz) {
            for (j = 0; j < g_walls[i].len; j++) {
                if (x == (uint8_t)(g_walls[i].x + j) && y == g_walls[i].y) {
                    return 1;
                }
            }
        } else {
            for (j = 0; j < g_walls[i].len; j++) {
                if (x == g_walls[i].x && y == (uint8_t)(g_walls[i].y + j)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

static uint8_t wall_gap_ok(uint8_t x, uint8_t y, uint8_t len, uint8_t horiz)
{
    uint8_t i, j, k;
    uint8_t cx, cy;

    for (j = 0; j < len; j++) {
        if (horiz) {
            cx = (uint8_t)(x + j);
            cy = y;
        } else {
            cx = x;
            cy = (uint8_t)(y + j);
        }

        if (cx >= XSize || cy >= (uint8_t)(YSize - 1)) {
            return 0;
        }
        if (cy < 1) {
            return 0;
        }

        for (i = 0; i < g_num_walls; i++) {
            if (g_walls[i].horiz) {
                for (k = 0; k < g_walls[i].len; k++) {
                    if (cheb_dist(cx, cy, (uint8_t)(g_walls[i].x + k), g_walls[i].y) < MIN_WALL_GAP) {
                        return 0;
                    }
                }
            } else {
                for (k = 0; k < g_walls[i].len; k++) {
                    if (cheb_dist(cx, cy, g_walls[i].x, (uint8_t)(g_walls[i].y + k)) < MIN_WALL_GAP) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

static void generate_walls(void)
{
    uint8_t i;
    uint8_t attempts;
    uint8_t num_segs;
    uint8_t wx, wy, wl, wh;

    num_segs = 5;
    if (g_level > 5) {
        num_segs = (uint8_t)(5 + g_level - 5);
    }
    if (num_segs > MAX_WALLS) {
        num_segs = MAX_WALLS;
    }

    /* Vary random sequence per level */
    for (i = 0; i < g_level; i++) {
        _XL_RAND();
    }

    g_num_walls = 0;
    attempts = 0;

    while (g_num_walls < num_segs && attempts < 300) {
        attempts++;
        wl = (uint8_t)(1 + _XL_RAND() % MAX_WALL_LEN);
        wh = (uint8_t)(_XL_RAND() % 2);

        if (wh) {
            if (XSize > wl) {
                wx = (uint8_t)(_XL_RAND() % (uint8_t)(XSize - wl));
            } else {
                wx = 0;
            }
            wy = (uint8_t)(1 + _XL_RAND() % (uint8_t)(YSize - 2));
        } else {
            wx = (uint8_t)(_XL_RAND() % (uint8_t)(XSize - 1));
            if (YSize > 2 + wl) {
                wy = (uint8_t)(1 + _XL_RAND() % (uint8_t)(YSize - 1 - wl));
            } else {
                wy = 1;
            }
        }

        if (wall_gap_ok(wx, wy, wl, wh)) {
            g_walls[g_num_walls].x = wx;
            g_walls[g_num_walls].y = wy;
            g_walls[g_num_walls].len = wl;
            g_walls[g_num_walls].horiz = wh;
            g_num_walls++;
        }
    }
}

/* ===================== Enemy Placement ===================== */
static uint8_t is_valid_tank_pos(uint8_t x, uint8_t y)
{
    uint8_t i, j;

    /* Bounds: tank is 2x2, must be in game area (y >= 1) */
    if (x + 1 >= XSize) {
        return 0;
    }
    if (y < 1 || y + 1 >= YSize) {
        return 0;
    }

    /* Check no wall tiles in the 2x2 area */
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (is_wall_tile((uint8_t)(x + i), (uint8_t)(y + j))) {
                return 0;
            }
        }
    }

    /* Don't place on top of player */
    if (x == g_player.x && y == g_player.y) {
        return 0;
    }
    /* Check overlap with player's 2x2 */
    if (x < (uint8_t)(g_player.x + 2) && (uint8_t)(x + 2) > g_player.x &&
        y < (uint8_t)(g_player.y + 2) && (uint8_t)(y + 2) > g_player.y) {
        return 0;
    }

    /* Don't overlap existing enemies */
    for (i = 0; i < g_num_enemies; i++) {
        if (g_enemies[i].alive) {
            uint8_t ox = g_enemies[i].x;
            uint8_t oy = g_enemies[i].y;
            if (x < (uint8_t)(ox + 2) && (uint8_t)(x + 2) > ox &&
                y < (uint8_t)(oy + 2) && (uint8_t)(y + 2) > oy) {
                return 0;
            }
        }
    }

    return 1;
}

static void place_enemy(uint8_t idx)
{
    uint8_t attempts;
    uint8_t ex, ey;
    uint8_t dir;

    attempts = 0;
    while (attempts < 100) {
        attempts++;
        ex = (uint8_t)(1 + _XL_RAND() % (uint8_t)(XSize - 3));
        ey = (uint8_t)(1 + _XL_RAND() % (uint8_t)(YSize - 3));
        dir = (uint8_t)(_XL_RAND() % 8);

        if (is_valid_tank_pos(ex, ey)) {
            g_enemies[idx].x = ex;
            g_enemies[idx].y = ey;
            g_enemies[idx].dir = dir;
            g_enemies[idx].hp = TANK_HP;
            g_enemies[idx].alive = 1;
            return;
        }
    }
    /* Fallback: place at a safe spot */
    g_enemies[idx].x = (uint8_t)(XSize - 4);
    g_enemies[idx].y = 2;
    g_enemies[idx].dir = 6;
    g_enemies[idx].hp = TANK_HP;
    g_enemies[idx].alive = 1;
}

/* ===================== Item Spawning ===================== */
static void spawn_item(void)
{
    uint8_t attempts;
    uint8_t ix, iy;

    g_item_alive = 0;
    attempts = 0;
    while (attempts < 50) {
        attempts++;
        ix = (uint8_t)(_XL_RAND() % (XSize - 1));
        iy = (uint8_t)(1 + _XL_RAND() % (uint8_t)(YSize - 2));

        if (!is_wall_tile(ix, iy)) {
            /* Don't spawn on top of player or enemies */
            uint8_t on_entity = 0;
            uint8_t e;
            if (ix >= g_player.x && ix <= (uint8_t)(g_player.x + 1) &&
                iy >= g_player.y && iy <= (uint8_t)(g_player.y + 1)) {
                on_entity = 1;
            }
            for (e = 0; e < g_num_enemies; e++) {
                if (g_enemies[e].alive) {
                    if (ix >= g_enemies[e].x && ix <= (uint8_t)(g_enemies[e].x + 1) &&
                        iy >= g_enemies[e].y && iy <= (uint8_t)(g_enemies[e].y + 1)) {
                        on_entity = 1;
                        break;
                    }
                }
            }
            if (!on_entity) {
                g_item_x = ix;
                g_item_y = iy;
                g_item_alive = 1;
                return;
            }
        }
    }
    g_item_alive = 0;
}

/* ===================== Tank Movement ===================== */
static uint8_t tank_blocked(uint8_t nx, uint8_t ny, uint8_t skip_enemy)
{
    uint8_t i, j;

    /* Bounds */
    if (nx + 1 >= XSize || ny < 1 || ny + 1 >= YSize) {
        return 1;
    }

    /* Wall check */
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (is_wall_tile((uint8_t)(nx + i), (uint8_t)(ny + j))) {
                return 1;
            }
        }
    }

    /* Player collision (for enemies moving) */
    if (skip_enemy != 255) {
        if (nx == g_player.x && ny == g_player.y) {
            return 1;
        }
        /* Check 2x2 overlap with player */
        if (nx < (uint8_t)(g_player.x + 2) && (uint8_t)(nx + 2) > g_player.x &&
            ny < (uint8_t)(g_player.y + 2) && (uint8_t)(ny + 2) > g_player.y) {
            return 1;
        }
    }

    /* Enemy collision */
    for (i = 0; i < g_num_enemies; i++) {
        if (g_enemies[i].alive && i != skip_enemy) {
            uint8_t ox = g_enemies[i].x;
            uint8_t oy = g_enemies[i].y;
            if (nx < (uint8_t)(ox + 2) && (uint8_t)(nx + 2) > ox &&
                ny < (uint8_t)(oy + 2) && (uint8_t)(ny + 2) > oy) {
                return 1;
            }
        }
    }

    return 0;
}

static void move_tank(Tank *t, uint8_t dir, uint8_t skip_enemy)
{
    short nx, ny;
    nx = (short)t->x + DX[dir];
    ny = (short)t->y + DY[dir];
    if (nx < 0 || ny < 0 || nx > 255 || ny > 255) {
        return;
    }
    if (tank_blocked((uint8_t)nx, (uint8_t)ny, skip_enemy)) {
        return;
    }
    sb_erase_tank(t);
    t->x = (uint8_t)nx;
    t->y = (uint8_t)ny;
}

/* ===================== Bullet Firing ===================== */
static void fire_bullet(Tank *t, uint8_t from_player)
{
    uint8_t i;
    short bx, by;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!g_bullets[i].alive) {
            bx = (short)t->x + DX[t->dir];
            by = (short)t->y + DY[t->dir];
            if (bx < 0 || by < 0 || bx > 255 || by > 255) {
                return;
            }
            g_bullets[i].x = (uint8_t)bx;
            g_bullets[i].y = (uint8_t)by;
            g_bullets[i].dir = t->dir;
            g_bullets[i].alive = 1;
            g_bullets[i].from_player = from_player;
            return;
        }
    }
}

static void reset_bullets(void)
{
    uint8_t i;
    for (i = 0; i < MAX_BULLETS; i++) {
        g_bullets[i].alive = 0;
    }
}

/* ===================== Player Update ===================== */
static void update_player(uint8_t input)
{
    if (!g_player.alive) {
        return;
    }

    if (_XL_LEFT(input)) {
        g_player.dir = (uint8_t)((g_player.dir + 7) % 8);
    } else if (_XL_RIGHT(input)) {
        g_player.dir = (uint8_t)((g_player.dir + 1) % 8);
    }

    if (_XL_UP(input)) {
        move_tank(&g_player, g_player.dir, 255);
    }
    if (_XL_DOWN(input)) {
        move_tank(&g_player, (uint8_t)((g_player.dir + 4) % 8), 255);
    }

    if (_XL_FIRE(input) && g_fire_cd == 0 && g_player_ammo > 0) {
        fire_bullet(&g_player, 1);
        g_player_ammo--;
        g_fire_cd = FIRE_COOLDOWN;
        _XL_SHOOT_SOUND();
    }
}

/* ===================== Enemy AI ===================== */
static void update_enemies(void)
{
    uint8_t i;
    uint8_t d;
    uint8_t best_dir;
    uint8_t best_dist;
    short pdx, pdy;
    uint8_t cur_dist;
    uint8_t ndx, ndy;

    for (i = 0; i < g_num_enemies; i++) {
        if (!g_enemies[i].alive) {
            continue;
        }

        /* Movement cooldown */
        if (g_enemy_mcd[i] > 0) {
            g_enemy_mcd[i]--;
        } else {
            /* Find best direction toward player */
            best_dist = 255;
            best_dir = g_enemies[i].dir;
            for (d = 0; d < 8; d++) {
                ndx = (uint8_t)((short)g_enemies[i].x + DX[d]);
                ndy = (uint8_t)((short)g_enemies[i].y + DY[d]);
                if (ndx + 1 >= XSize || ndy < 1 || ndy + 1 >= YSize) {
                    continue;
                }
                if (tank_blocked(ndx, ndy, i)) {
                    continue;
                }
                pdx = (short)g_player.x - (short)ndx;
                if (pdx < 0) { pdx = (short)(-pdx); }
                pdy = (short)g_player.y - (short)ndy;
                if (pdy < 0) { pdy = (short)(-pdy); }
                cur_dist = (uint8_t)(pdx + pdy);
                if (cur_dist < best_dist) {
                    best_dist = cur_dist;
                    best_dir = d;
                }
            }
            /* Add some randomness */
            if (_XL_RAND() % 10 < 3) {
                best_dir = (uint8_t)(_XL_RAND() % 8);
            }
            g_enemies[i].dir = best_dir;
            move_tank(&g_enemies[i], best_dir, i);
            g_enemy_mcd[i] = ENEMY_MOVE_CD;
        }

        /* Firing */
        if (g_enemy_cd[i] > 0) {
            g_enemy_cd[i]--;
        } else {
            /* Face toward player roughly */
            pdx = (short)g_player.x - (short)g_enemies[i].x;
            pdy = (short)g_player.y - (short)g_enemies[i].y;
            for (d = 0; d < 8; d++) {
                if ((DX[d] > 0 && pdx > 0) || (DX[d] < 0 && pdx < 0) || DX[d] == 0) {
                    if ((DY[d] > 0 && pdy > 0) || (DY[d] < 0 && pdy < 0) || DY[d] == 0) {
                        g_enemies[i].dir = d;
                        break;
                    }
                }
            }
            fire_bullet(&g_enemies[i], 0);
            g_enemy_cd[i] = (uint8_t)(ENEMY_FIRE_CD + _XL_RAND() % 5);
            _XL_SHOOT_SOUND();
        }
    }
}

/* ===================== Bullet Update ===================== */
static void update_bullets(void)
{
    uint8_t i;
    short nx, ny;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!g_bullets[i].alive) {
            continue;
        }

        nx = (short)g_bullets[i].x + DX[g_bullets[i].dir];
        ny = (short)g_bullets[i].y + DY[g_bullets[i].dir];

        /* Off screen? */
        if (nx < 0 || ny < 0 || nx >= (short)XSize || ny >= (short)(YSize - 1)) {
            g_bullets[i].alive = 0;
            continue;
        }

        /* Hit wall? */
        if (is_wall_tile((uint8_t)nx, (uint8_t)ny)) {
            g_bullets[i].alive = 0;
            _XL_TICK_SOUND();
            continue;
        }

        /* Hit player? (if enemy bullet) */
        if (!g_bullets[i].from_player && g_player.alive) {
            if (nx >= g_player.x && nx <= (uint8_t)(g_player.x + 1) &&
                ny >= g_player.y && ny <= (uint8_t)(g_player.y + 1)) {
                g_bullets[i].alive = 0;
                g_player.hp--;
                _XL_PING_SOUND();
                if (g_player.hp == 0) {
                    g_player.alive = 0;
                    g_game_over = 1;
                    _XL_EXPLOSION_SOUND();
                }
                continue;
            }
        }

        /* Hit enemy? (if player bullet) */
        if (g_bullets[i].from_player) {
            uint8_t hit = 0;
            uint8_t e;
            for (e = 0; e < g_num_enemies; e++) {
                if (g_enemies[e].alive) {
                    if (nx >= g_enemies[e].x &&
                        nx <= (uint8_t)(g_enemies[e].x + 1) &&
                        ny >= g_enemies[e].y &&
                        ny <= (uint8_t)(g_enemies[e].y + 1)) {
                        g_bullets[i].alive = 0;
                        g_enemies[e].hp--;
                        _XL_PING_SOUND();
                        if (g_enemies[e].hp == 0) {
                            g_enemies[e].alive = 0;
                            g_player_ammo += KILL_REWARD;
                            g_score += 100;
                            _XL_EXPLOSION_SOUND();
                            if (count_alive_enemies() == 0) {
                                g_level_done = 1;
                                g_score += 500;
                            }
                        }
                        hit = 1;
                        break;
                    }
                }
            }
            if (hit) {
                continue;
            }
        }

        /* Move bullet */
        sb_erase(g_bullets[i].x, g_bullets[i].y);
        g_bullets[i].x = (uint8_t)nx;
        g_bullets[i].y = (uint8_t)ny;
    }
}

static uint8_t count_alive_enemies(void)
{
    uint8_t i;
    uint8_t count;
    count = 0;
    for (i = 0; i < g_num_enemies; i++) {
        if (g_enemies[i].alive) {
            count++;
        }
    }
    return count;
}

/* ===================== Item Update ===================== */
static void update_item(void)
{
    if (!g_item_alive || !g_player.alive) {
        return;
    }
    if (g_item_x >= g_player.x && g_item_x <= (uint8_t)(g_player.x + 1) &&
        g_item_y >= g_player.y && g_item_y <= (uint8_t)(g_player.y + 1)) {
        g_item_alive = 0;
        g_player_ammo += ITEM_REWARD;
        _XL_TOCK_SOUND();
    }
}

/* ===================== Screen Buffer Helpers ===================== */
static void sb_draw(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id, uint8_t sb_id)
{
    if (x >= XSize || y >= YSize) {
        return;
    }
    if (g_screen[y][x] == sb_id) {
        return;
    }
    if (g_screen[y][x] != SB_EMPTY) {
        _XL_DELETE(x, y);
    }
    _XL_DRAW(x, y, tile_id, color_id);
    g_screen[y][x] = sb_id;
}

static void sb_erase(uint8_t x, uint8_t y)
{
    if (x >= XSize || y >= YSize) {
        return;
    }
    if (g_screen[y][x] == SB_EMPTY) {
        return;
    }
    _XL_DELETE(x, y);
    g_screen[y][x] = SB_EMPTY;
}

static void sb_draw_tank(Tank *t, uint8_t tile_id, uint8_t color_id, uint8_t sb_id)
{
    uint8_t i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            sb_draw((uint8_t)(t->x + i), (uint8_t)(t->y + j), tile_id, color_id, sb_id);
        }
    }
}

static void sb_erase_tank(Tank *t)
{
    uint8_t i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            sb_erase((uint8_t)(t->x + i), (uint8_t)(t->y + j));
        }
    }
}

/* ===================== Drawing ===================== */
static void draw_hud(void)
{
    uint8_t i;
    uint8_t alive_count;

    /* Clear HUD row */
    for (i = 0; i < XSize; i++) {
        sb_erase(i, 0);
    }

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "SCORE:");
    _XL_PRINTD(6, 0, 4, g_score);

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(15, 0, "AMMO:");
    _XL_PRINTD(20, 0, 2, g_player_ammo);

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(25, 0, "LVL:");
    _XL_PRINTD(29, 0, 2, g_level);

    alive_count = count_alive_enemies();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(33, 0, "EN:");
    _XL_PRINTD(36, 0, 2, alive_count);

    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(40, 0, "HP:");
    _XL_PRINTD(43, 0, 1, g_player.hp);
}

static void draw_screen(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t b;

    /* Draw walls */
    for (i = 0; i < g_num_walls; i++) {
        if (g_walls[i].horiz) {
            for (j = 0; j < g_walls[i].len; j++) {
                sb_draw((uint8_t)(g_walls[i].x + j), g_walls[i].y, _TILE_2, _XL_CYAN, SB_WALL);
            }
        } else {
            for (j = 0; j < g_walls[i].len; j++) {
                sb_draw(g_walls[i].x, (uint8_t)(g_walls[i].y + j), _TILE_2, _XL_CYAN, SB_WALL);
            }
        }
    }

    /* Draw player */
    if (g_player.alive) {
        sb_draw_tank(&g_player, _TILE_0, _XL_WHITE, SB_PLAYER);
    }

    /* Draw enemies */
    for (i = 0; i < g_num_enemies; i++) {
        if (g_enemies[i].alive) {
            sb_draw_tank(&g_enemies[i], _TILE_1, _XL_RED, SB_ENEMY);
        }
    }

    /* Draw bullets */
    for (b = 0; b < MAX_BULLETS; b++) {
        if (g_bullets[b].alive) {
            if (g_bullets[b].from_player) {
                sb_draw(g_bullets[b].x, g_bullets[b].y, _TILE_3, _XL_YELLOW, SB_BULLET);
            } else {
                sb_draw(g_bullets[b].x, g_bullets[b].y, _TILE_3, _XL_MAGENTA, SB_BULLET);
            }
        }
    }

    /* Draw item */
    if (g_item_alive) {
        sb_draw(g_item_x, g_item_y, _TILE_4, _XL_MAGENTA, SB_ITEM);
    }

    /* Draw HUD */
    draw_hud();
}

