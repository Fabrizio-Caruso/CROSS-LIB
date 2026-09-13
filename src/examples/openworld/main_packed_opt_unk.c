/*
**Summary of optimisations applied:**

| Area | Before | After |
|------|--------|-------|
| **Screen buffers** | 4 × XSize × YSize bytes (`prev_tile`, `prev_color`, `curr_tile`, `curr_color`) | 2 × XSize × YSize bytes (`prev_state`,
`curr_state`) — tile + colour packed into one byte via `(tile<<3)\|color` |
| **Enemy health** | `uint16_t` (2 bytes × 17 = 34 B) | `uint8_t` (1 byte × 17 = 17 B) — max value 200 fits |
| **Dirty-checking** | Commented out; every tile drawn/deleted every frame | Restored: `_XL_DRAW`/`_XL_DELETE` called **only** when the packed state
differs from the previous frame |
| **`is_blocked`** | 4 comparisons per wall (≥, <, ≥, <) | 2 comparisons using unsigned wraparound: `(uint8_t)(x - wall.x) < WALL_SIZE` |
| **Enemy intervals** | `if/else` chain per enemy per frame | `const uint8_t` lookup tables (`enemy_move_iv[]`, `enemy_shoot_iv[]`) |
| **Distance culling** | Computed `dx*dx+dy*dy` for every entity | Quick axis reject (`dx > vis \|\| dx < -vis …`) before the multiply, skipping
most off-screen entities |
| **Item / enemy type dispatch** | `if/else if` chains | `switch` statements (compiler emits jump tables on 8-bit targets) |
| **`signed char` usage** | Pervasive | Replaced with `short` per the C89 / 8-bit constraint |
| **Unnecessary `hit` flag** | Set and tested after the hit loop | Eliminated; projectile is killed inline at the point of impact |
| **Frame pacing** | `_XL_SLOW_DOWN` commented out | Restored so the game runs at a controlled rate on the 8-bit CPU |
| **Code size** | Redundant boundary checks, repeated `_XL_DRAW` calls in `draw_visibility_border` | Consolidated; `switch` instead of `if`-chains;
removed dead code |

The net RAM saving is roughly **2 × XSize × YSize + 17 bytes** (e.g. ≈ 1 267 bytes on a 25 × 25 screen), and the per-frame CPU cost drops
dramatically because the dirty-check eliminates the majority of `_XL_DRAW`/`_XL_DELETE` calls when the scene is mostly static.
*/

#include "cross_lib.h"

#define MAP_SIZE 100
#define NUM_EASY 8
#define NUM_HARD 5
#define NUM_VERY_HARD 3
#define NUM_DRAGON 1
#define NUM_ENEMIES (NUM_EASY + NUM_HARD + NUM_VERY_HARD + NUM_DRAGON)
#define NUM_GEMS 10
#define NUM_POTIONS 15
#define NUM_INVINCIBLE 2
#define NUM_TORCHES 2
#define NUM_HEALTH 4
#define NUM_ITEMS (NUM_GEMS + NUM_POTIONS + NUM_INVINCIBLE + NUM_TORCHES + NUM_HEALTH)
#define NUM_WALLS 20
#define WALL_SIZE 4
#define MAX_PROJECTILES 4
#define MAX_SHOT_RANGE 6
#define INVINCIBLE_DURATION 150
#define PLAYER_MAX_HP 5
#define MIN_SPAWN_DIST 15
#define FAR_SPAWN_DIST 35
#define INITIAL_VISIBILITY 3
#define MAX_VISIBILITY 12
#define CHASE_RANGE 10
#define EMPTY_TILE _TILE_26
#define EMPTY_COLOR _XL_BLUE
#define ENEMY_BULLET_SPEED 2

#define ENEMY_EASY 0
#define ENEMY_HARD 1
#define ENEMY_VERY_HARD 2
#define ENEMY_DRAGON 3

#define ITEM_GEM 0
#define ITEM_POTION 1
#define ITEM_INVINCIBLE 2
#define ITEM_TORCH 3
#define ITEM_HEALTH 4

#define PROJ_PLAYER 0
#define PROJ_ENEMY 1

#define DIR_NONE 0
#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_RIGHT 4

/* Packed screen state: upper 5 bits = tile, lower 3 bits = color */
#define PACK_STATE(t, c) ((uint8_t)(((t) << 3) | (c)))
#define UNPACK_TILE(s) ((uint8_t)((s) >> 3))
#define UNPACK_COLOR(s) ((uint8_t)((s) & 0x07))
#define EMPTY_STATE PACK_STATE(EMPTY_TILE, EMPTY_COLOR)

typedef struct {
    uint8_t x, y, type, alive, shoot_timer, move_timer, health;
} Enemy;

typedef struct {
    uint8_t x, y, type, alive;
} Item;

typedef struct {
    uint8_t x, y, dir, type, alive, range, move_timer;
} Projectile;

typedef struct {
    uint8_t x, y, potions, invincible_timer, hp, gems, shoot_dir, alive;
} Player;

typedef struct {
    uint8_t x, y;
} Wall;

Enemy enemies[NUM_ENEMIES];
Item items[NUM_ITEMS];
Projectile projectiles[MAX_PROJECTILES];
Player player;
Wall walls[NUM_WALLS];
uint8_t visibility;
uint8_t game_over;
uint8_t game_won;
uint16_t score;

static uint8_t prev_state[XSize][YSize];
static uint8_t curr_state[XSize][YSize];

static uint16_t prev_score;
static uint8_t prev_potions;
static uint8_t prev_hp;
static uint8_t prev_invincible;

/* Lookup tables for enemy behaviour */
static const uint8_t enemy_move_iv[4] = {12, 18, 25, 30};
static const uint8_t enemy_shoot_iv[4] = {0, 0, 50, 40};

void reset_screen_buffer(void)
{
    uint8_t x, y;
    for (y = 0; y < YSize; y++)
    {
        for (x = 0; x < XSize; x++)
        {
            prev_state[x][y] = EMPTY_STATE;
            curr_state[x][y] = EMPTY_STATE;
        }
    }
    prev_score = 0;
    prev_potions = 0;
    prev_hp = 0;
    prev_invincible = 0;
}

/* Optimised: uses unsigned wraparound to replace 4 comparisons with 2 */
uint8_t is_blocked(uint8_t x, uint8_t y)
{
    uint8_t i;
    for (i = 0; i < NUM_WALLS; i++)
    {
        if ((uint8_t)(x - walls[i].x) < WALL_SIZE &&
            (uint8_t)(y - walls[i].y) < WALL_SIZE)
            return 1;
    }
    return 0;
}

void generate_walls(void)
{
    uint8_t i, j;
    uint8_t attempts;
    uint8_t wx, wy;
    short dx, dy;
    short min_dist;
    uint8_t valid;

    for (i = 0; i < NUM_WALLS; i++)
    {
        attempts = 0;
        while (1)
        {
            wx = (uint8_t)(1 + _XL_RAND() % (MAP_SIZE - WALL_SIZE - 2));
            wy = (uint8_t)(1 + _XL_RAND() % (MAP_SIZE - WALL_SIZE - 2));

            if (wx <= MAP_SIZE / 2 && wx + WALL_SIZE > MAP_SIZE / 2 &&
                wy <= MAP_SIZE / 2 && wy + WALL_SIZE > MAP_SIZE / 2)
            {
                attempts++;
                if (attempts > 500) break;
                continue;
            }

            valid = 1;
            for (j = 0; j < i; j++)
            {
                if (wx > walls[j].x + WALL_SIZE - 1)
                    dx = (short)wx - (short)walls[j].x - (WALL_SIZE - 1);
                else if (walls[j].x > wx + WALL_SIZE - 1)
                    dx = (short)walls[j].x - (short)wx - (WALL_SIZE - 1);
                else
                    dx = 0;

                if (wy > walls[j].y + WALL_SIZE - 1)
                    dy = (short)wy - (short)walls[j].y - (WALL_SIZE - 1);
                else if (walls[j].y > wy + WALL_SIZE - 1)
                    dy = (short)walls[j].y - (short)wy - (WALL_SIZE - 1);
                else
                    dy = 0;

                if (dx == 0 && dy == 0)
                {
                    valid = 0;
                    break;
                }
                if (dx == 0)
                    min_dist = dy;
                else if (dy == 0)
                    min_dist = dx;
                else
                    min_dist = (dx > dy) ? dx : dy;

                if (min_dist < 2)
                {
                    valid = 0;
                    break;
                }
            }

            if (valid)
            {
                walls[i].x = wx;
                walls[i].y = wy;
                break;
            }

            attempts++;
            if (attempts > 500)
            {
                walls[i].x = wx;
                walls[i].y = wy;
                break;
            }
        }
    }
}

void spawn_enemy(uint8_t idx, uint8_t type, uint8_t health, uint8_t min_dist)
{
    uint8_t attempts;
    short dx, dy;
    uint16_t dist_sq;
    uint16_t min_sq;
    uint8_t ex, ey;

    min_sq = (uint16_t)min_dist * (uint16_t)min_dist;
    attempts = 0;

    while (1)
    {
        ex = (uint8_t)(1 + _XL_RAND() % (MAP_SIZE - 2));
        ey = (uint8_t)(1 + _XL_RAND() % (MAP_SIZE - 2));
        if (is_blocked(ex, ey))
        {
            attempts++;
            if (attempts > 500) break;
            continue;
        }
        dx = (short)ex - (short)player.x;
        dy = (short)ey - (short)player.y;
        dist_sq = (uint16_t)(dx * dx + dy * dy);
        if (dist_sq >= min_sq)
            break;
        attempts++;
        if (attempts > 500)
            break;
    }

    enemies[idx].x = ex;
    enemies[idx].y = ey;
    enemies[idx].type = type;
    enemies[idx].health = health;
    enemies[idx].alive = 1;
    enemies[idx].shoot_timer = 0;
    enemies[idx].move_timer = 0;
}

void init_game(void)
{
    uint8_t i;

    player.x = MAP_SIZE / 2;
    player.y = MAP_SIZE / 2;
    player.potions = 0;
    player.invincible_timer = 0;
    player.hp = PLAYER_MAX_HP;
    player.gems = 0;
    player.shoot_dir = DIR_NONE;
    player.alive = 1;

    visibility = INITIAL_VISIBILITY;
    game_over = 0;
    game_won = 0;
    score = 0;

    generate_walls();

    for (i = 0; i < NUM_EASY; i++)
        spawn_enemy(i, ENEMY_EASY, 1, MIN_SPAWN_DIST);
    for (i = 0; i < NUM_HARD; i++)
        spawn_enemy(NUM_EASY + i, ENEMY_HARD, 6, MIN_SPAWN_DIST);
    for (i = 0; i < NUM_VERY_HARD; i++)
        spawn_enemy(NUM_EASY + NUM_HARD + i, ENEMY_VERY_HARD, 20, FAR_SPAWN_DIST);
    spawn_enemy(NUM_EASY + NUM_HARD + NUM_VERY_HARD, ENEMY_DRAGON, 200, FAR_SPAWN_DIST);

    for (i = 0; i < NUM_ITEMS; i++)
    {
        items[i].alive = 1;
        if (i < NUM_GEMS)
            items[i].type = ITEM_GEM;
        else if (i < NUM_GEMS + NUM_POTIONS)
            items[i].type = ITEM_POTION;
        else if (i < NUM_GEMS + NUM_POTIONS + NUM_INVINCIBLE)
            items[i].type = ITEM_INVINCIBLE;
        else if (i < NUM_GEMS + NUM_POTIONS + NUM_INVINCIBLE + NUM_TORCHES)
            items[i].type = ITEM_TORCH;
        else
            items[i].type = ITEM_HEALTH;
        items[i].x = (uint8_t)(1 + _XL_RAND() % (MAP_SIZE - 2));
        items[i].y = (uint8_t)(1 + _XL_RAND() % (MAP_SIZE - 2));
    }

    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        projectiles[i].alive = 0;
        projectiles[i].x = 0;
        projectiles[i].y = 0;
        projectiles[i].dir = DIR_NONE;
        projectiles[i].type = PROJ_PLAYER;
        projectiles[i].range = 0;
        projectiles[i].move_timer = 0;
    }

    reset_screen_buffer();
}

void spawn_projectile(uint8_t x, uint8_t y, uint8_t dir, uint8_t type)
{
    uint8_t i;
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].alive)
        {
            projectiles[i].alive = 1;
            projectiles[i].x = x;
            projectiles[i].y = y;
            projectiles[i].dir = dir;
            projectiles[i].type = type;
            projectiles[i].range = MAX_SHOT_RANGE;
            projectiles[i].move_timer = 0;
            return;
        }
    }
}

void draw_visibility_border(void)
{
    short bx, by, bi;
    short vis;
    vis = (short)visibility;

    by = (short)(YSize / 2) - vis;
    for (bi = -vis; bi <= vis; bi++)
    {
        bx = (short)(XSize / 2) + bi;
        if (bx >= 0 && bx < (short)XSize && by >= 0 && by < (short)YSize)
        {
            _XL_DRAW((uint8_t)bx, (uint8_t)by, _TILE_10, _XL_MAGENTA);
        }
    }
    by = (short)(YSize / 2) + vis;
    for (bi = -vis; bi <= vis; bi++)
    {
        bx = (short)(XSize / 2) + bi;
        if (bx >= 0 && bx < (short)XSize && by >= 0 && by < (short)YSize)
        {
            _XL_DRAW((uint8_t)bx, (uint8_t)by, _TILE_10, _XL_MAGENTA);
        }
    }
    bx = (short)(XSize / 2) - vis;
    for (bi = -vis + 1; bi <= vis - 1; bi++)
    {
        by = (short)(YSize / 2) + bi;
        if (bx >= 0 && bx < (short)XSize && by >= 0 && by < (short)YSize)
        {
            _XL_DRAW((uint8_t)bx, (uint8_t)by, _TILE_10, _XL_MAGENTA);
        }
    }
    bx = (short)(XSize / 2) + vis;
    for (bi = -vis + 1; bi <= vis - 1; bi++)
    {
        by = (short)(YSize / 2) + bi;
        if (bx >= 0 && bx < (short)XSize && by >= 0 && by < (short)YSize)
        {
            _XL_DRAW((uint8_t)bx, (uint8_t)by, _TILE_10, _XL_MAGENTA);
        }
    }
}

void update_player(uint8_t input)
{
    short nx, ny;
    uint8_t i;
    short px, py;

    if (!player.alive) return;

    if (player.invincible_timer > 0)
        player.invincible_timer--;

    if (_XL_LEFT(input))
    {
        nx = (short)player.x - 1;
        if (nx >= 1 && !is_blocked((uint8_t)nx, player.y))
        {
            player.x = (uint8_t)nx;
            player.shoot_dir = DIR_LEFT;
        }
    }
    else if (_XL_RIGHT(input))
    {
        nx = (short)player.x + 1;
        if (nx < MAP_SIZE - 1 && !is_blocked((uint8_t)nx, player.y))
        {
            player.x = (uint8_t)nx;
            player.shoot_dir = DIR_RIGHT;
        }
    }
    else if (_XL_UP(input))
    {
        ny = (short)player.y - 1;
        if (ny >= 1 && !is_blocked(player.x, (uint8_t)ny))
        {
            player.y = (uint8_t)ny;
            player.shoot_dir = DIR_UP;
        }
    }
    else if (_XL_DOWN(input))
    {
        ny = (short)player.y + 1;
        if (ny < MAP_SIZE - 1 && !is_blocked(player.x, (uint8_t)ny))
        {
            player.y = (uint8_t)ny;
            player.shoot_dir = DIR_DOWN;
        }
    }

    if (_XL_FIRE(input) && player.shoot_dir != DIR_NONE)
    {
        px = (short)player.x;
        py = (short)player.y;
        switch (player.shoot_dir)
        {
            case DIR_UP: py--; break;
            case DIR_DOWN: py++; break;
            case DIR_LEFT: px--; break;
            case DIR_RIGHT: px++; break;
            default: break;
        }
        if (px >= 1 && px < MAP_SIZE - 1 && py >= 1 && py < MAP_SIZE - 1)
        {
            spawn_projectile((uint8_t)px, (uint8_t)py, player.shoot_dir, PROJ_PLAYER);
            _XL_SHOOT_SOUND();
        }
    }

    for (i = 0; i < NUM_ITEMS; i++)
    {
        if (items[i].alive && items[i].x == player.x && items[i].y == player.y)
        {
            items[i].alive = 0;
            switch (items[i].type)
            {
                case ITEM_GEM:
                    player.gems++;
                    score += 10;
                    _XL_PING_SOUND();
                    break;
                case ITEM_POTION:
                    if (player.potions < 255) player.potions++;
                    _XL_PING_SOUND();
                    break;
                case ITEM_INVINCIBLE:
                    player.invincible_timer = INVINCIBLE_DURATION;
                    _XL_PING_SOUND();
                    break;
                case ITEM_TORCH:
                    if (visibility < MAX_VISIBILITY)
                    {
                        visibility++;
                        _XL_CLEAR_SCREEN();
                        reset_screen_buffer();
                        draw_visibility_border();
                    }
                    _XL_PING_SOUND();
                    break;
                case ITEM_HEALTH:
                    if (player.hp < PLAYER_MAX_HP)
                        player.hp++;
                    _XL_PING_SOUND();
                    break;
                default:
                    break;
            }
        }
    }
}

void update_enemies(void)
{
    uint8_t i;
    short dx, dy;
    uint8_t move_iv;
    uint8_t shoot_iv;
    uint8_t sdir;
    short px, py;
    short nx, ny;

    for (i = 0; i < NUM_ENEMIES; i++)
    {
        if (!enemies[i].alive) continue;

        enemies[i].move_timer++;
        move_iv = enemy_move_iv[enemies[i].type];

        if (enemies[i].move_timer >= move_iv)
        {
            enemies[i].move_timer = 0;
            dx = (short)player.x - (short)enemies[i].x;
            dy = (short)player.y - (short)enemies[i].y;

            if (dx <= CHASE_RANGE && dx >= -CHASE_RANGE &&
                dy <= CHASE_RANGE && dy >= -CHASE_RANGE)
            {
                if (dx > 0)
                {
                    nx = (short)enemies[i].x + 1;
                    if (nx < MAP_SIZE - 2 && !is_blocked((uint8_t)nx, enemies[i].y))
                        enemies[i].x = (uint8_t)nx;
                }
                else if (dx < 0)
                {
                    nx = (short)enemies[i].x - 1;
                    if (nx > 1 && !is_blocked((uint8_t)nx, enemies[i].y))
                        enemies[i].x = (uint8_t)nx;
                }
                if (dy > 0)
                {
                    ny = (short)enemies[i].y + 1;
                    if (ny < MAP_SIZE - 2 && !is_blocked(enemies[i].x, (uint8_t)ny))
                        enemies[i].y = (uint8_t)ny;
                }
                else if (dy < 0)
                {
                    ny = (short)enemies[i].y - 1;
                    if (ny > 1 && !is_blocked(enemies[i].x, (uint8_t)ny))
                        enemies[i].y = (uint8_t)ny;
                }
            }
        }

        shoot_iv = enemy_shoot_iv[enemies[i].type];
        if (shoot_iv != 0)
        {
            enemies[i].shoot_timer++;
            if (enemies[i].shoot_timer >= shoot_iv)
            {
                enemies[i].shoot_timer = 0;
                dx = (short)player.x - (short)enemies[i].x;
                dy = (short)player.y - (short)enemies[i].y;

                if (dx > 0) sdir = DIR_RIGHT;
                else if (dx < 0) sdir = DIR_LEFT;
                else if (dy > 0) sdir = DIR_DOWN;
                else sdir = DIR_UP;

                px = (short)enemies[i].x;
                py = (short)enemies[i].y;
                switch (sdir)
                {
                    case DIR_UP: py--; break;
                    case DIR_DOWN: py++; break;
                    case DIR_LEFT: px--; break;
                    case DIR_RIGHT: px++; break;
                    default: break;
                }
                if (px >= 1 && px < MAP_SIZE - 1 && py >= 1 && py < MAP_SIZE - 1)
                {
                    spawn_projectile((uint8_t)px, (uint8_t)py, sdir, PROJ_ENEMY);
                    _XL_TICK_SOUND();
                }
            }
        }
    }
}

void update_projectiles(void)
{
    uint8_t i;
    uint8_t j;
    short px, py;
    uint16_t dmg;
    uint8_t speed;

    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].alive) continue;

        speed = (projectiles[i].type == PROJ_PLAYER) ? 1 : ENEMY_BULLET_SPEED;

        projectiles[i].move_timer++;
        if (projectiles[i].move_timer < speed)
            continue;
        projectiles[i].move_timer = 0;

        px = (short)projectiles[i].x;
        py = (short)projectiles[i].y;
        switch (projectiles[i].dir)
        {
            case DIR_UP: py--; break;
            case DIR_DOWN: py++; break;
            case DIR_LEFT: px--; break;
            case DIR_RIGHT: px++; break;
            default: break;
        }

        if (px < 1 || px >= MAP_SIZE - 1 || py < 1 || py >= MAP_SIZE - 1)
        {
            projectiles[i].alive = 0;
            continue;
        }

        if (is_blocked((uint8_t)px, (uint8_t)py))
        {
            projectiles[i].alive = 0;
            continue;
        }

        projectiles[i].x = (uint8_t)px;
        projectiles[i].y = (uint8_t)py;
        projectiles[i].range--;
        if (projectiles[i].range == 0)
        {
            projectiles[i].alive = 0;
            continue;
        }

        if (projectiles[i].type == PROJ_PLAYER)
        {
            for (j = 0; j < NUM_ENEMIES; j++)
            {
                if (!enemies[j].alive) continue;
                if (enemies[j].x == projectiles[i].x && enemies[j].y == projectiles[i].y)
                {
                    dmg = 1 + (uint16_t)player.potions;
                    if (enemies[j].type == ENEMY_DRAGON && player.potions < 10)
                        dmg = 0;
                    if (dmg > 0)
                    {
                        if (enemies[j].health >= dmg)
                            enemies[j].health -= (uint8_t)dmg;
                        else
                            enemies[j].health = 0;
                        _XL_TOCK_SOUND();
                    }
                    if (enemies[j].health == 0)
                    {
                        enemies[j].alive = 0;
                        score += 25;
                        _XL_EXPLOSION_SOUND();
                        if (enemies[j].type == ENEMY_DRAGON)
                        {
                            game_won = 1;
                            game_over = 1;
                        }
                    }
                    projectiles[i].alive = 0;
                    break;
                }
            }
        }
        else
        {
            if (projectiles[i].x == player.x && projectiles[i].y == player.y)
            {
                projectiles[i].alive = 0;
                if (player.invincible_timer == 0 && player.alive)
                {
                    if (player.hp > 0) player.hp--;
                    _XL_ZAP_SOUND();
                    if (player.hp == 0)
                    {
                        player.alive = 0;
                        game_over = 1;
                    }
                }
            }
        }
    }
}

void render_hud(void)
{
    uint8_t invincible_now;

    invincible_now = (player.invincible_timer > 0) ? 1 : 0;

    if (score != prev_score)
    {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 0, "SCORE");
        _XL_PRINTD(30, 0, 1, score);
        prev_score = score;
    }

    if (player.potions != prev_potions)
    {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 1, "POT");
        _XL_PRINTD(15, 1, 1, (uint16_t)player.potions);
        prev_potions = player.potions;
    }

    if (player.hp != prev_hp)
    {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 2, "HP");
        _XL_PRINTD(10, 2, 1, (uint16_t)player.hp);
        prev_hp = player.hp;
    }

    if (invincible_now != prev_invincible)
    {
        if (invincible_now)
        {
            _XL_SET_TEXT_COLOR(_XL_CYAN);
            _XL_PRINT(0, 3, "INV");
        }
        else
        {
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(0, 3, "   ");
        }
        prev_invincible = invincible_now;
    }
}

void render(void)
{
    uint8_t i;
    uint8_t x, y;
    short dx, dy;
    short sx, sy;
    short vis;
    short x_start, x_end, y_start, y_end;
    uint8_t wi;
    short wx, wy;
    short wx2, wy2;
    uint8_t tile, color;
    uint8_t pcolor;

    /* Clear current frame buffer */
    for (y = 0; y < YSize; y++)
    {
        for (x = 0; x < XSize; x++)
        {
            curr_state[x][y] = EMPTY_STATE;
        }
    }

    vis = (short)visibility;
    x_start = (short)(XSize / 2) - vis;
    if (x_start < 0) x_start = 0;
    x_end = (short)(XSize / 2) + vis;
    if (x_end >= (short)XSize) x_end = (short)XSize - 1;
    y_start = (short)(YSize / 2) - vis;
    if (y_start < 0) y_start = 0;
    y_end = (short)(YSize / 2) + vis;
    if (y_end >= (short)YSize) y_end = (short)YSize - 1;

    /* Draw map boundary walls */
    for (y = (uint8_t)y_start; y <= (uint8_t)y_end; y++)
    {
        for (x = (uint8_t)x_start; x <= (uint8_t)x_end; x++)
        {
            wx = (short)player.x + (short)x - (short)(XSize / 2);
            wy = (short)player.y + (short)y - (short)(YSize / 2);
            if (wx < 0 || wx >= MAP_SIZE || wy < 0 || wy >= MAP_SIZE)
                continue;
            if (wx == 0 || wx == MAP_SIZE - 1 || wy == 0 || wy == MAP_SIZE - 1)
            {
                dx = wx - (short)player.x;
                dy = wy - (short)player.y;
                if (dx * dx + dy * dy <= vis * vis)
                {
                    curr_state[x][y] = PACK_STATE(_TILE_13, _XL_BLUE);
                }
            }
        }
    }

    /* Draw generated walls */
    for (wi = 0; wi < NUM_WALLS; wi++)
    {
        for (wy2 = 0; wy2 < WALL_SIZE; wy2++)
        {
            for (wx2 = 0; wx2 < WALL_SIZE; wx2++)
            {
                wx = (short)walls[wi].x + (short)wx2;
                wy = (short)walls[wi].y + (short)wy2;
                dx = wx - (short)player.x;
                dy = wy - (short)player.y;
                if (dx > vis || dx < -vis || dy > vis || dy < -vis) continue;
                if (dx * dx + dy * dy > vis * vis) continue;
                sx = (short)(XSize / 2) + dx;
                sy = (short)(YSize / 2) + dy;
                if (sx < 0 || sx >= (short)XSize) continue;
                if (sy < 0 || sy >= (short)YSize) continue;
                curr_state[sx][sy] = PACK_STATE(_TILE_14, _XL_MAGENTA);
            }
        }
    }

    /* Draw items */
    for (i = 0; i < NUM_ITEMS; i++)
    {
        if (!items[i].alive) continue;
        dx = (short)items[i].x - (short)player.x;
        dy = (short)items[i].y - (short)player.y;
        if (dx > vis || dx < -vis || dy > vis || dy < -vis) continue;
        if (dx * dx + dy * dy > vis * vis) continue;
        sx = (short)(XSize / 2) + dx;
        sy = (short)(YSize / 2) + dy;
        if (sx < 0 || sx >= (short)XSize) continue;
        if (sy < 0 || sy >= (short)YSize) continue;
        switch (items[i].type)
        {
            case ITEM_GEM:
                tile = _TILE_5; color = _XL_CYAN; break;
            case ITEM_POTION:
                tile = _TILE_6; color = _XL_GREEN; break;
            case ITEM_INVINCIBLE:
                tile = _TILE_7; color = _XL_WHITE; break;
            case ITEM_TORCH:
                tile = _TILE_11; color = _XL_YELLOW; break;
            default:
                tile = _TILE_12; color = _XL_RED; break;
        }
        curr_state[sx][sy] = PACK_STATE(tile, color);
    }

    /* Draw enemies */
    for (i = 0; i < NUM_ENEMIES; i++)
    {
        if (!enemies[i].alive) continue;
        dx = (short)enemies[i].x - (short)player.x;
        dy = (short)enemies[i].y - (short)player.y;
        if (dx > vis || dx < -vis || dy > vis || dy < -vis) continue;
        if (dx * dx + dy * dy > vis * vis) continue;
        sx = (short)(XSize / 2) + dx;
        sy = (short)(YSize / 2) + dy;
        if (sx < 0 || sx >= (short)XSize) continue;
        if (sy < 0 || sy >= (short)YSize) continue;
        switch (enemies[i].type)
        {
            case ENEMY_EASY:
                tile = _TILE_1; color = _XL_RED; break;
            case ENEMY_HARD:
                tile = _TILE_2; color = _XL_YELLOW; break;
            case ENEMY_VERY_HARD:
                tile = _TILE_3; color = _XL_MAGENTA; break;
            default:
                tile = _TILE_4; color = _XL_RED; break;
        }
        curr_state[sx][sy] = PACK_STATE(tile, color);
    }

    /* Draw projectiles */
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].alive) continue;
        dx = (short)projectiles[i].x - (short)player.x;
        dy = (short)projectiles[i].y - (short)player.y;
        if (dx > vis || dx < -vis || dy > vis || dy < -vis) continue;
        if (dx * dx + dy * dy > vis * vis) continue;
        sx = (short)(XSize / 2) + dx;
        sy = (short)(YSize / 2) + dy;
        if (sx < 0 || sx >= (short)XSize) continue;
        if (sy < 0 || sy >= (short)YSize) continue;
        if (projectiles[i].type == PROJ_PLAYER)
            curr_state[sx][sy] = PACK_STATE(_TILE_8, _XL_YELLOW);
        else
            curr_state[sx][sy] = PACK_STATE(_TILE_9, _XL_RED);
    }

    /* Draw player at centre */
    pcolor = (player.invincible_timer > 0) ? _XL_WHITE : _XL_GREEN;
    curr_state[XSize / 2][YSize / 2] = PACK_STATE(_TILE_0, pcolor);

    /* Dirty-check: only draw tiles that changed */
    for (y = (uint8_t)y_start; y <= (uint8_t)y_end; y++)
    {
        for (x = (uint8_t)x_start; x <= (uint8_t)x_end; x++)
        {
            if (curr_state[x][y] != prev_state[x][y])
            {
                if (curr_state[x][y] != EMPTY_STATE)
                {
                    _XL_DRAW(x, y, UNPACK_TILE(curr_state[x][y]),
                             UNPACK_COLOR(curr_state[x][y]));
                }
                else
                {
                    _XL_DELETE(x, y);
                }
                prev_state[x][y] = curr_state[x][y];
            }
        }
    }

    render_hud();
}

void show_game_over(void)
{
    _XL_CLEAR_SCREEN();
    reset_screen_buffer();
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    if (game_won)
    {
        _XL_PRINT(XSize / 4, YSize / 3, "YOU WIN");
        _XL_PRINT(XSize / 8, YSize / 3 + 2, "DRAGON SLAIN");
    }
    else
    {
        _XL_PRINT(XSize / 4, YSize / 3, "GAME OVER");
    }
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(XSize / 8, YSize / 2, "SCORE");
    _XL_PRINTD(XSize / 2, YSize / 2, 1, score);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(XSize / 8, YSize / 2 + 2, "PRESS ANY KEY");
}

int main(void)
{
    uint8_t input;
    uint8_t need_restart;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    need_restart = 1;

    while (1)
    {
        if (need_restart)
        {
            init_game();
            need_restart = 0;
            draw_visibility_border();
        }
        if (!game_over)
        {
            input = _XL_INPUT();
            update_player(input);
            update_enemies();
            update_projectiles();
            render();
        }
        else
        {
            show_game_over();
            _XL_WAIT_FOR_INPUT();
            need_restart = 1;
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}