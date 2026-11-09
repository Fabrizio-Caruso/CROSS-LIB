#include "cross_lib.h"

/*
   Assumption: cross_lib.h already defines uint8_t and uint16_t.
   If your toolchain does not provide them, uncomment these two lines:

   typedef unsigned char uint8_t;
   typedef unsigned short uint16_t;
*/

#define MAX_BULLETS 99
#define MAX_ENEMIES 8
#define MAX_ITEMS 8
#define FIRE_COOLDOWN 6
#define FAST_FIRE_COOLDOWN 2
#define ENEMY_INTERVAL 24
#define ITEM_INTERVAL 90
#define ITEM_MOVE_EVERY 3
#define POWERUP_DURATION 60
#define INVINCIBLE_DURATION 45
#define TRIPLE_DURATION 60
#define START_BULLETS 9
#define BULLET_RESTORE 9

/* Tile IDs */
#define PLAYER_TILE_ID _TILE_1
#define BULLET_TILE_ID _TILE_2
#define ENEMY_TILE_ID _TILE_3
#define EXPLOSION_TILE_ID _TILE_4
#define ITEM_BULLET_TILE_ID _TILE_5
#define ITEM_FIRERATE_TILE_ID _TILE_6
#define ITEM_INVINCIBLE_TILE_ID _TILE_7
#define ITEM_TRIPLE_TILE_ID _TILE_8

/* Colors */
#define PLAYER_COLOR_ID _XL_CYAN
#define BULLET_COLOR_ID _XL_YELLOW
#define ENEMY_COLOR_ID _XL_RED
#define EXPLOSION_COLOR_ID _XL_MAGENTA
#define ITEM_BULLET_COLOR_ID _XL_GREEN
#define ITEM_FIRERATE_COLOR_ID _XL_WHITE
#define ITEM_INVINCIBLE_COLOR_ID _XL_BLUE
#define ITEM_TRIPLE_COLOR_ID _XL_YELLOW

#define BLANK_TILE ((uint8_t)_TILE_0)

/* Screen-state dimensions clamped to uint8_t range. */
#define XL_CAP_X (((XSize)) > 256 ? 256 : ((XSize)))
#define STATE_W ((((XL_CAP_X)) < 1) ? 1 : (XL_CAP_X))
#define XL_CAP_Y (((YSize)) > 256 ? 256 : ((YSize)))
#define STATE_H ((((XL_CAP_Y)) < 1) ? 1 : (XL_CAP_Y))

/* Playable coordinate limits. */
#define GAME_MAX_X ((STATE_W) - 1)
#define GAME_MAX_Y ((STATE_H) - 1)

/* HUD occupies the bottom row (YSize-1). */
#define HUD_ROW ((uint8_t)GAME_MAX_Y)

/* Playable vertical range: player cannot go past YSize-2. */
#define PLAY_TOP ((((GAME_MAX_Y)) >= 2) ? 1 : 0)
#define PLAY_BOTTOM ((((GAME_MAX_Y)) >= 2) ? ((uint8_t)(GAME_MAX_Y) - 1) : ((uint8_t)PLAY_TOP))

/* Player start position (left tile of the 2-wide ship). */
#define PLAYER_START_X (1)
#define PLAYER_START_Y (((PLAY_TOP) + (PLAY_BOTTOM)) / 2)

/* Item types */
#define ITEM_TYPE_BULLETS 1
#define ITEM_TYPE_FIRERATE 2
#define ITEM_TYPE_INVINCIBLE 3
#define ITEM_TYPE_TRIPLE 4

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t tile_id;
    uint8_t color_id;
    uint8_t alive;
} entity_t;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t type;
    uint8_t tile_id;
    uint8_t color_id;
    uint8_t alive;
} item_t;

static uint8_t prev_tile[STATE_H][STATE_W];
static uint8_t prev_color[STATE_H][STATE_W];

static entity_t bullets[MAX_BULLETS];
static entity_t enemies[MAX_ENEMIES];
static item_t items[MAX_ITEMS];

static uint8_t player_x;
static uint8_t player_y;
static uint8_t player_alive;
static uint8_t bullet_count;

static uint16_t score;
static uint16_t last_score;
static uint16_t last_bullet_display;
static uint16_t fire_cooldown;
static uint16_t spawn_timer;
static uint16_t item_spawn_timer;
static uint16_t item_move_timer;
static uint16_t firerate_timer;
static uint16_t invincible_timer;
static uint16_t triple_timer;

static uint8_t game_over;
static uint8_t game_over_shown;
static uint8_t explosion_shown;

/* Previous-frame positions for erase-on-change. */
static uint8_t old_player_x;
static uint8_t old_player_y;
static uint8_t old_player_alive;
static uint8_t old_bullet_x[MAX_BULLETS];
static uint8_t old_bullet_y[MAX_BULLETS];
static uint8_t old_bullet_alive[MAX_BULLETS];
static uint8_t old_enemy_x[MAX_ENEMIES];
static uint8_t old_enemy_y[MAX_ENEMIES];
static uint8_t old_enemy_alive[MAX_ENEMIES];
static uint8_t old_item_x[MAX_ITEMS];
static uint8_t old_item_y[MAX_ITEMS];
static uint8_t old_item_alive[MAX_ITEMS];

static void reset_screen_state(void);
static void set_cell(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id);
static void erase_cell(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id);
static uint8_t random_play_row(void);
static uint8_t find_dead_bullet(void);
static uint8_t find_dead_enemy(void);
static uint8_t find_dead_item(void);
static void spawn_enemy(void);
static void spawn_item(void);
static void reset_game(void);
static void update_game(uint8_t input);
static void draw_changed_tiles(void);
static void update_hud(void);
static void wait_frames(uint8_t frames);
static void show_ready(void);
static void show_game_over(void);
static void fire_bullets(uint8_t idx);

/* Reset the internal screen-state table to blank. */
static void reset_screen_state(void)
{
    uint16_t y;
    uint16_t x;

    for (y = 0; y < (uint16_t)STATE_H; ++y) {
        for (x = 0; x < (uint16_t)STATE_W; ++x) {
            prev_tile[y][x] = (uint8_t)BLANK_TILE;
            prev_color[y][x] = 0;
        }
    }
}

/* Draw a tile only if the stored state for that cell is different. */
static void set_cell(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id)
{
    if ((uint16_t)x >= (uint16_t)STATE_W ||
        (uint16_t)y >= (uint16_t)STATE_H) {
        return;
    }

    if (tile_id == (uint8_t)BLANK_TILE && color_id == 0) {
        if (prev_tile[y][x] != (uint8_t)BLANK_TILE ||
            prev_color[y][x] != 0) {
            _XL_DELETE(x, y);
        }
        prev_tile[y][x] = (uint8_t)BLANK_TILE;
        prev_color[y][x] = 0;
        return;
    }

    if (prev_tile[y][x] == tile_id &&
        prev_color[y][x] == color_id) {
        return;
    }

    _XL_DRAW(x, y, tile_id, color_id);
    prev_tile[y][x] = tile_id;
    prev_color[y][x] = color_id;
}

/* Delete a tile only if the stored state for that cell still matches it. */
static void erase_cell(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id)
{
    if ((uint16_t)x >= (uint16_t)STATE_W ||
        (uint16_t)y >= (uint16_t)STATE_H) {
        return;
    }

    if (tile_id == (uint8_t)BLANK_TILE && color_id == 0) {
        return;
    }

    if (prev_tile[y][x] == tile_id &&
        prev_color[y][x] == color_id) {
        _XL_DELETE(x, y);
        prev_tile[y][x] = (uint8_t)BLANK_TILE;
        prev_color[y][x] = 0;
    }
}

/* Random row inside the playable area. */
static uint8_t random_play_row(void)
{
    uint16_t span;

    span = (uint16_t)((PLAY_BOTTOM) - (PLAY_TOP) + 1);
    if (span > 0) {
        return (uint8_t)((PLAY_TOP) + (_XL_RAND() % span));
    }
    return (uint8_t)PLAY_TOP;
}

static uint8_t find_dead_bullet(void)
{
    uint16_t i;
    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        if (!bullets[i].alive) {
            return (uint8_t)i;
        }
    }
    return (uint8_t)MAX_BULLETS;
}

static uint8_t find_dead_enemy(void)
{
    uint16_t i;
    for (i = 0; i < (uint16_t)MAX_ENEMIES; ++i) {
        if (!enemies[i].alive) {
            return (uint8_t)i;
        }
    }
    return (uint8_t)MAX_ENEMIES;
}

static uint8_t find_dead_item(void)
{
    uint16_t i;
    for (i = 0; i < (uint16_t)MAX_ITEMS; ++i) {
        if (!items[i].alive) {
            return (uint8_t)i;
        }
    }
    return (uint8_t)MAX_ITEMS;
}

/* Spawn one enemy on the right edge (2 tiles wide). */
static void spawn_enemy(void)
{
    uint16_t i;
    uint8_t idx;
    uint8_t y;

    if ((uint16_t)GAME_MAX_X < 1) {
        return;
    }

    idx = find_dead_enemy();
    if ((uint16_t)idx >= (uint16_t)MAX_ENEMIES) {
        return;
    }

    y = random_play_row();

    /* Avoid spawning in a row already used by another live enemy. */
    for (i = 0; i < (uint16_t)MAX_ENEMIES; ++i) {
        if (enemies[i].alive && enemies[i].y == y) {
            return;
        }
    }

    enemies[idx].x = (uint8_t)(GAME_MAX_X - 1);
    enemies[idx].y = y;
    enemies[idx].tile_id = ENEMY_TILE_ID;
    enemies[idx].color_id = ENEMY_COLOR_ID;
    enemies[idx].alive = 1;

    _XL_TOCK_SOUND();
}

/* Spawn one power-up item on the right edge. */
static void spawn_item(void)
{
    uint8_t idx;
    uint8_t y;
    uint8_t type;

    if ((uint16_t)GAME_MAX_X < 1) {
        return;
    }

    idx = find_dead_item();
    if ((uint16_t)idx >= (uint16_t)MAX_ITEMS) {
        return;
    }

    y = random_play_row();
    type = (uint8_t)((_XL_RAND() % 4) + 1);

    items[idx].x = (uint8_t)GAME_MAX_X;
    items[idx].y = y;
    items[idx].type = type;
    items[idx].alive = 1;

    switch (type) {
        case ITEM_TYPE_BULLETS:
            items[idx].tile_id = ITEM_BULLET_TILE_ID;
            items[idx].color_id = ITEM_BULLET_COLOR_ID;
            break;
        case ITEM_TYPE_FIRERATE:
            items[idx].tile_id = ITEM_FIRERATE_TILE_ID;
            items[idx].color_id = ITEM_FIRERATE_COLOR_ID;
            break;
        case ITEM_TYPE_INVINCIBLE:
            items[idx].tile_id = ITEM_INVINCIBLE_TILE_ID;
            items[idx].color_id = ITEM_INVINCIBLE_COLOR_ID;
            break;
        case ITEM_TYPE_TRIPLE:
            items[idx].tile_id = ITEM_TRIPLE_TILE_ID;
            items[idx].color_id = ITEM_TRIPLE_COLOR_ID;
            break;
        default:
            items[idx].tile_id = ITEM_BULLET_TILE_ID;
            items[idx].color_id = ITEM_BULLET_COLOR_ID;
            break;
    }
}

/* Reset all game state for a new run. */
static void reset_game(void)
{
    uint16_t i;

    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        bullets[i].x = 0;
        bullets[i].y = 0;
        bullets[i].tile_id = (uint8_t)BLANK_TILE;
        bullets[i].color_id = 0;
        bullets[i].alive = 0;
        old_bullet_x[i] = 0;
        old_bullet_y[i] = 0;
        old_bullet_alive[i] = 0;
    }

    for (i = 0; i < (uint16_t)MAX_ENEMIES; ++i) {
        enemies[i].x = 0;
        enemies[i].y = 0;
        enemies[i].tile_id = (uint8_t)BLANK_TILE;
        enemies[i].color_id = 0;
        enemies[i].alive = 0;
        old_enemy_x[i] = 0;
        old_enemy_y[i] = 0;
        old_enemy_alive[i] = 0;
    }

    for (i = 0; i < (uint16_t)MAX_ITEMS; ++i) {
        items[i].x = 0;
        items[i].y = 0;
        items[i].type = 0;
        items[i].tile_id = (uint8_t)BLANK_TILE;
        items[i].color_id = 0;
        items[i].alive = 0;
        old_item_x[i] = 0;
        old_item_y[i] = 0;
        old_item_alive[i] = 0;
    }

    old_player_x = 0;
    old_player_y = 0;
    old_player_alive = 0;

    player_alive = 1;
    game_over = 0;
    explosion_shown = 0;

    fire_cooldown = 0;
    spawn_timer = ENEMY_INTERVAL;
    item_spawn_timer = ITEM_INTERVAL;
    item_move_timer = 0;
    firerate_timer = 0;
    invincible_timer = 0;
    triple_timer = 0;

    score = 0;
    last_score = (uint16_t)65535;
    last_bullet_display = (uint16_t)65535;
    bullet_count = START_BULLETS;

    player_x = (uint8_t)PLAYER_START_X;
    player_y = (uint8_t)PLAYER_START_Y;
}

/* Fire bullets from the current player position. */
static void fire_bullets(uint8_t idx)
{
    uint16_t cooldown;
    uint8_t bx;
    uint8_t by;

    cooldown = (firerate_timer > 0) ? FAST_FIRE_COOLDOWN : FIRE_COOLDOWN;

    if (bullet_count == 0) {
        return;
    }

    --bullet_count;
    fire_cooldown = cooldown;

    bx = (uint8_t)((uint16_t)player_x + 2);
    if ((uint16_t)bx > (uint16_t)GAME_MAX_X) {
        bx = (uint8_t)GAME_MAX_X;
    }
    by = player_y;

    if (triple_timer > 0) {
        /* Triple fire: 3 bullets, counts as 1 shot. */
        {
            uint8_t idx2, idx3;
            uint8_t by_up, by_dn;

            idx2 = find_dead_bullet();
            idx3 = find_dead_bullet();

            by_up = (player_y > PLAY_TOP) ? (uint8_t)(player_y - 1) : player_y;
            by_dn = (player_y < PLAY_BOTTOM) ? (uint8_t)(player_y + 1) : player_y;

            if ((uint16_t)idx < (uint16_t)MAX_BULLETS) {
                bullets[idx].x = bx;
                bullets[idx].y = by;
                bullets[idx].tile_id = BULLET_TILE_ID;
                bullets[idx].color_id = BULLET_COLOR_ID;
                bullets[idx].alive = 1;
            }
            if ((uint16_t)idx2 < (uint16_t)MAX_BULLETS) {
                bullets[idx2].x = bx;
                bullets[idx2].y = by_up;
                bullets[idx2].tile_id = BULLET_TILE_ID;
                bullets[idx2].color_id = BULLET_COLOR_ID;
                bullets[idx2].alive = 1;
            }
            if ((uint16_t)idx3 < (uint16_t)MAX_BULLETS) {
                bullets[idx3].x = bx;
                bullets[idx3].y = by_dn;
                bullets[idx3].tile_id = BULLET_TILE_ID;
                bullets[idx3].color_id = BULLET_COLOR_ID;
                bullets[idx3].alive = 1;
            }
        }
    } else {
        /* Single bullet. */
        if ((uint16_t)idx < (uint16_t)MAX_BULLETS) {
            bullets[idx].x = bx;
            bullets[idx].y = by;
            bullets[idx].tile_id = BULLET_TILE_ID;
            bullets[idx].color_id = BULLET_COLOR_ID;
            bullets[idx].alive = 1;
        }
    }

    _XL_SHOOT_SOUND();
}

/* Update logical game state for one frame. */
static void update_game(uint8_t input)
{
    uint16_t i;
    uint16_t j;
    uint8_t idx;

    /* Timers. */
    if (fire_cooldown > 0) --fire_cooldown;
    if (firerate_timer > 0) --firerate_timer;
    if (invincible_timer > 0) --invincible_timer;
    if (triple_timer > 0) --triple_timer;

    if (spawn_timer > 0) {
        --spawn_timer;
    } else {
        spawn_enemy();
        spawn_timer = ENEMY_INTERVAL;
    }

    if (item_spawn_timer > 0) {
        --item_spawn_timer;
    } else {
        spawn_item();
        item_spawn_timer = ITEM_INTERVAL;
    }

    /* Item slow movement: advance every ITEM_MOVE_EVERY frames. */
    item_move_timer++;
    if (item_move_timer >= ITEM_MOVE_EVERY) {
        item_move_timer = 0;
        for (i = 0; i < (uint16_t)MAX_ITEMS; ++i) {
            if (!items[i].alive) continue;
            if ((uint16_t)items[i].x > 0) {
                --items[i].x;
            } else {
                items[i].alive = 0; /* escaped off left edge */
            }
        }
    }

    /* Remember previous-frame state. */
    old_player_alive = player_alive;
    old_player_x = player_x;
    old_player_y = player_y;

    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        old_bullet_x[i] = bullets[i].x;
        old_bullet_y[i] = bullets[i].y;
        old_bullet_alive[i] = bullets[i].alive;
    }
    for (i = 0; i < (uint16_t)MAX_ENEMIES; ++i) {
        old_enemy_x[i] = enemies[i].x;
        old_enemy_y[i] = enemies[i].y;
        old_enemy_alive[i] = enemies[i].alive;
    }
    for (i = 0; i < (uint16_t)MAX_ITEMS; ++i) {
        old_item_x[i] = items[i].x;
        old_item_y[i] = items[i].y;
        old_item_alive[i] = items[i].alive;
    }

    /* Player movement and firing. */
    if (player_alive) {
        if (_XL_LEFT(input)) {
            if ((uint16_t)player_x > 0) --player_x;
        } else if (_XL_RIGHT(input)) {
            if ((uint16_t)player_x + 2 <= (uint16_t)GAME_MAX_X) ++player_x;
        } else if (_XL_UP(input)) {
            if ((uint16_t)player_y > (uint16_t)PLAY_TOP) --player_y;
        } else if (_XL_DOWN(input)) {
            if ((uint16_t)player_y < (uint16_t)PLAY_BOTTOM) ++player_y;
        }

        if (_XL_FIRE(input) && fire_cooldown == 0 && bullet_count > 0) {
            idx = find_dead_bullet();
            if ((uint16_t)idx < (uint16_t)MAX_BULLETS) {
                fire_bullets(idx);
            }
        }
    }

    /* Move bullets right. */
    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        if (!bullets[i].alive) continue;
        if ((uint16_t)bullets[i].x + 1 <= (uint16_t)GAME_MAX_X) {
            ++bullets[i].x;
        } else {
            bullets[i].alive = 0;
        }
    }

    /* Bullet vs enemy collision (enemy is 2 wide: ex, ex+1). */
    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        if (!bullets[i].alive) continue;
        for (j = 0; j < (uint16_t)MAX_ENEMIES; ++j) {
            if (!enemies[j].alive) continue;
            if (bullets[i].y != enemies[j].y) continue;

            /* Bullet at bx overlaps enemy [ex, ex+1] if bx==ex or bx==ex+1.
               Also check crossing: bullet was left of enemy and now right. */
            if ((bullets[i].x == enemies[j].x) ||
                (bullets[i].x == (uint8_t)(enemies[j].x + 1)) ||
                (old_bullet_alive[i] && old_enemy_alive[j] &&
                 ((uint16_t)old_bullet_x[i] < (uint16_t)enemies[j].x &&
                  (uint16_t)bullets[i].x > (uint16_t)enemies[j].x + 1))) {
                bullets[i].alive = 0;
                enemies[j].alive = 0;
                if (score < (uint16_t)50000) {
                    score += 10;
                    if (score > (uint16_t)50000) score = (uint16_t)50000;
                }
                _XL_PING_SOUND();
                break;
            }
        }
    }

    /* Move enemies left (2 tiles wide). */
    for (i = 0; i < (uint16_t)MAX_ENEMIES; ++i) {
        if (!enemies[i].alive) continue;
        if ((uint16_t)enemies[i].x > 0) {
            --enemies[i].x;
        } else {
            enemies[i].alive = 0; /* escaped */
        }
    }

    /* Bullet vs enemy collision again (after enemy moved). */
    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        if (!bullets[i].alive) continue;
        for (j = 0; j < (uint16_t)MAX_ENEMIES; ++j) {
            if (!enemies[j].alive) continue;
            if (bullets[i].y != enemies[j].y) continue;
            if ((bullets[i].x == enemies[j].x) ||
                (bullets[i].x == (uint8_t)(enemies[j].x + 1))) {
                bullets[i].alive = 0;
                enemies[j].alive = 0;
                if (score < (uint16_t)50000) {
                    score += 10;
                    if (score > (uint16_t)50000) score = (uint16_t)50000;
                }
                _XL_PING_SOUND();
                break;
            }
        }
    }

    /* Player vs enemy collision (both 2 wide).
       Player occupies [player_x, player_x+1], enemy [ex, ex+1].
       Overlap if py==ey and player_x <= ex+1 and ex <= player_x+1. */
    if (player_alive && !game_over && invincible_timer == 0) {
        for (j = 0; j < (uint16_t)MAX_ENEMIES; ++j) {
            if (!enemies[j].alive) continue;
            if (player_y != enemies[j].y) continue;

            if ((uint16_t)player_x <= (uint16_t)enemies[j].x + 1 &&
                (uint16_t)enemies[j].x <= (uint16_t)player_x + 1) {
                game_over = 1;
                player_alive = 0;
                _XL_EXPLOSION_SOUND();
                break;
            }
        }
    }

    /* Player vs item pickup (item is 1 tile, player is 2 wide).
       Overlap if item.y == player_y and (item.x == player_x or item.x == player_x+1). */
    if (player_alive) {
        for (i = 0; i < (uint16_t)MAX_ITEMS; ++i) {
            if (!items[i].alive) continue;
            if (items[i].y != player_y) continue;

            if ((items[i].x == player_x) ||
                (items[i].x == (uint8_t)(player_x + 1))) {
                /* Picked up! */
                items[i].alive = 0;

                if (score < (uint16_t)50000) {
                    score += 5;
                    if (score > (uint16_t)50000) score = (uint16_t)50000;
                }

                switch (items[i].type) {
                    case ITEM_TYPE_BULLETS:
                        bullet_count = BULLET_RESTORE;
                        break;
                    case ITEM_TYPE_FIRERATE:
                        firerate_timer = POWERUP_DURATION;
                        break;
                    case ITEM_TYPE_INVINCIBLE:
                        invincible_timer = INVINCIBLE_DURATION;
                        break;
                    case ITEM_TYPE_TRIPLE:
                        triple_timer = TRIPLE_DURATION;
                        break;
                    default:
                        break;
                }

                _XL_PING_SOUND();
            }
        }
    }
}

/* Update the visible screen state (only changed tiles). */
static void draw_changed_tiles(void)
{
    uint16_t i;

    /* Player ship (2 tiles wide) or explosion. */
    if (player_alive) {
        uint8_t c = PLAYER_COLOR_ID;
        /* Flash when invincible. */
        if (invincible_timer > 0 && (invincible_timer & 1) == 0) {
            c = _XL_WHITE;
        }
        set_cell(player_x, player_y, PLAYER_TILE_ID, c);
        set_cell((uint8_t)(player_x + 1), player_y, PLAYER_TILE_ID, c);

        if (old_player_alive) {
            if (old_player_x != player_x || old_player_y != player_y) {
                erase_cell(old_player_x, old_player_y,
                           PLAYER_TILE_ID, PLAYER_COLOR_ID);
                erase_cell((uint8_t)(old_player_x + 1), old_player_y,
                           PLAYER_TILE_ID, PLAYER_COLOR_ID);
            }
        }
    } else {
        if (!explosion_shown) {
            set_cell(player_x, player_y,
                     EXPLOSION_TILE_ID, EXPLOSION_COLOR_ID);
            set_cell((uint8_t)(player_x + 1), player_y,
                     EXPLOSION_TILE_ID, EXPLOSION_COLOR_ID);
            explosion_shown = 1;
        }
        if (old_player_alive) {
            if (old_player_x != player_x || old_player_y != player_y) {
                erase_cell(old_player_x, old_player_y,
                           PLAYER_TILE_ID, PLAYER_COLOR_ID);
                erase_cell((uint8_t)(old_player_x + 1), old_player_y,
                           PLAYER_TILE_ID, PLAYER_COLOR_ID);
            }
        }
    }

    /* Bullets (1 tile each). */
    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        if (bullets[i].alive) {
            set_cell(bullets[i].x, bullets[i].y,
                     BULLET_TILE_ID, BULLET_COLOR_ID);
        }
        if (old_bullet_alive[i] &&
            ((old_bullet_x[i] != bullets[i].x ||
              old_bullet_y[i] != bullets[i].y) ||
             !bullets[i].alive)) {
            erase_cell(old_bullet_x[i], old_bullet_y[i],
                       BULLET_TILE_ID, BULLET_COLOR_ID);
        }
    }

    /* Enemies (2 tiles wide). */
    for (i = 0; i < (uint16_t)MAX_ENEMIES; ++i) {
        if (enemies[i].alive) {
            set_cell(enemies[i].x, enemies[i].y,
                     ENEMY_TILE_ID, ENEMY_COLOR_ID);
            set_cell((uint8_t)(enemies[i].x + 1), enemies[i].y,
                     ENEMY_TILE_ID, ENEMY_COLOR_ID);
        }
        if (old_enemy_alive[i] &&
            ((old_enemy_x[i] != enemies[i].x ||
              old_enemy_y[i] != enemies[i].y) ||
             !enemies[i].alive)) {
            erase_cell(old_enemy_x[i], old_enemy_y[i],
                       ENEMY_TILE_ID, ENEMY_COLOR_ID);
            erase_cell((uint8_t)(old_enemy_x[i] + 1), old_enemy_y[i],
                       ENEMY_TILE_ID, ENEMY_COLOR_ID);
        }
    }

    /* Items (1 tile each). */
    for (i = 0; i < (uint16_t)MAX_ITEMS; ++i) {
        if (items[i].alive) {
            set_cell(items[i].x, items[i].y,
                     items[i].tile_id, items[i].color_id);
        }
        if (old_item_alive[i] &&
            ((old_item_x[i] != items[i].x ||
              old_item_y[i] != items[i].y) ||
             !items[i].alive)) {
            erase_cell(old_item_x[i], old_item_y[i],
                       items[i].tile_id, items[i].color_id);
        }
    }
}

/* Update HUD: score and bullet count at the bottom row (YSize-1). */
static void update_hud(void)
{
    if (STATE_H < 2 || STATE_W < 12) {
        return;
    }

    if (score != last_score) {
        last_score = score;
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0, HUD_ROW, 1, score);
    }

    if (bullet_count != last_bullet_display) {
        last_bullet_display = bullet_count;
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD(STATE_W - 4, HUD_ROW, 1, bullet_count);
    }
}

/* Wait a number of frames. */
static void wait_frames(uint8_t frames)
{
    uint16_t i;
    for (i = 0; i < (uint16_t)frames; ++i) {
        _XL_SLOW_DOWN((uint16_t)_XL_SLOW_DOWN_FACTOR);
    }
}

static void show_ready(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    if (STATE_W >= 5) {
        _XL_PRINT(0, 0, "READY");
    } else {
        _XL_PRINT(0, 0, "R");
    }
}

static void show_game_over(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    if (STATE_W >= 9) {
        _XL_PRINT(0, 0, "GAME OVER");
    } else {
        _XL_PRINT(0, 0, "OVER");
    }
}

int main(void)
{
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    reset_screen_state();
    _XL_CLEAR_SCREEN();
    reset_game();

    if (STATE_H > 1) {
        show_ready();
    }

    _XL_WAIT_FOR_INPUT();

    reset_screen_state();
    _XL_CLEAR_SCREEN();
    last_score = (uint16_t)65535;
    last_bullet_display = (uint16_t)65535;

    while (1) {
        input = _XL_INPUT();

        if (!game_over) {
            update_game(input);
            draw_changed_tiles();
            update_hud();
        } else {
            if (!game_over_shown) {
                game_over_shown = 1;
                if (STATE_H > 1) {
                    show_game_over();
                }
                wait_frames(30);
            }

            _XL_WAIT_FOR_INPUT();

            reset_screen_state();
            _XL_CLEAR_SCREEN();
            reset_game();
            last_score = (uint16_t)65535;
            last_bullet_display = (uint16_t)65535;
            game_over_shown = 0;

            if (STATE_H > 1) {
                show_ready();
            }

            _XL_WAIT_FOR_INPUT();
        }

        _XL_SLOW_DOWN((uint16_t)_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}