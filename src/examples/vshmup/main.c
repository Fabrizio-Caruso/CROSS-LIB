#include "cross_lib.h"

/*
   Assumption: cross_lib.h already defines uint8_t and uint16_t.
   If your toolchain does not provide them, uncomment these two lines:

   typedef unsigned char uint8_t;
   typedef unsigned short uint16_t;
*/

#define MAX_BULLETS 1
#define MAX_ENEMIES 8
#define FIRE_COOLDOWN 6
#define ENEMY_INTERVAL 24

#define PLAYER_TILE_ID _TILE_1
#define BULLET_TILE_ID _TILE_2
#define ENEMY_TILE_ID _TILE_3
#define EXPLOSION_TILE_ID _TILE_4

#define PLAYER_COLOR_ID _XL_CYAN
#define BULLET_COLOR_ID _XL_YELLOW
#define ENEMY_COLOR_ID _XL_RED
#define EXPLOSION_COLOR_ID _XL_MAGENTA

/* Internal blank state.  Cross-Lib tile IDs are small, so this is safe. */
#define BLANK_TILE ((uint8_t)_TILE_0)

/*
   Keep the screen-state table inside the uint8_t coordinate range.
   The game coordinates used below are clamped to a safe subset of the
   documented 0 <= x <= XSize and 0 <= y <= YSize limits.
*/
#define XL_CAP_X (((XSize)) > 256 ? 256 : ((XSize)))
#define STATE_W ((((XL_CAP_X)) < 1) ? 1 : (XL_CAP_X))

#define XL_CAP_Y (((YSize)) > 256 ? 256 : ((YSize)))
#define STATE_H ((((XL_CAP_Y)) < 1) ? 1 : (XL_CAP_Y))

/* Playable coordinate limits. */
#define GAME_MAX_X ((STATE_W) - 1)
#define GAME_MAX_Y ((STATE_H) - 1)

/* Reserve the top row for HUD text when there is room for it. */
#define PLAY_TOP (((GAME_MAX_Y)) >= 1 ? 1 : 0)

#define PLAYER_START_X ((GAME_MAX_X) >= 2 ? 1 : 0)
#define PLAYER_START_Y (((PLAY_TOP) + (GAME_MAX_Y)) / 2)

static uint8_t prev_tile[STATE_H][STATE_W];
static uint8_t prev_color[STATE_H][STATE_W];

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t tile_id;
    uint8_t color_id;
    uint8_t alive;
} entity_t;

static entity_t bullets[MAX_BULLETS];
static entity_t enemies[MAX_ENEMIES];

static uint8_t player_x;
static uint8_t player_y;
static uint8_t player_alive;

static uint16_t score;
static uint16_t last_score;
static uint16_t fire_cooldown;
static uint16_t spawn_timer;

static uint8_t game_over;
static uint8_t game_over_shown;
static uint8_t explosion_shown;

/* Previous-frame positions, used to erase only cells that actually changed. */
static uint8_t old_player_x;
static uint8_t old_player_y;
static uint8_t old_player_alive;

static uint8_t old_bullet_x[MAX_BULLETS];
static uint8_t old_bullet_y[MAX_BULLETS];
static uint8_t old_bullet_alive[MAX_BULLETS];

static uint8_t old_enemy_x[MAX_ENEMIES];
static uint8_t old_enemy_y[MAX_ENEMIES];
static uint8_t old_enemy_alive[MAX_ENEMIES];

static void reset_screen_state(void);
static void set_cell(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id);
static void erase_cell(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id);
static uint8_t random_play_row(void);
static uint8_t find_dead_bullet(void);
static uint8_t find_dead_enemy(void);
static void spawn_enemy(void);
static void reset_game(void);
static void update_game(uint8_t input);
static void draw_changed_tiles(void);
static void update_hud(void);
static void wait_frames(uint8_t frames);
static void show_ready(void);
static void show_game_over(void);

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

/*
   Draw a tile only if the stored state for that cell is different.
   This prevents _XL_DRAW on unchanged tiles.
*/
static void set_cell(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id)
{
    if ((uint16_t)x >= (uint16_t)STATE_W ||
        (uint16_t)y >= (uint16_t)STATE_H) {
        return;
    }

    /* Blank state: update internal state only when needed. */
    if (tile_id == (uint8_t)BLANK_TILE && color_id == 0) {
        if (prev_tile[y][x] != (uint8_t)BLANK_TILE ||
            prev_color[y][x] != 0) {
            _XL_DELETE(x, y);
        }

        prev_tile[y][x] = (uint8_t)BLANK_TILE;
        prev_color[y][x] = 0;
        return;
    }

    /* If the cell already has exactly this tile/color, do nothing. */
    if (prev_tile[y][x] == tile_id &&
        prev_color[y][x] == color_id) {
        return;
    }

    _XL_DRAW(x, y, tile_id, color_id);

    prev_tile[y][x] = tile_id;
    prev_color[y][x] = color_id;
}

/*
   Delete a tile only if the stored state for that cell still matches it.
   This prevents _XL_DELETE on unchanged or already-overwritten tiles.
*/
static void erase_cell(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id)
{
    if ((uint16_t)x >= (uint16_t)STATE_W ||
        (uint16_t)y >= (uint16_t)STATE_H) {
        return;
    }

    /* Nothing to delete for blank state. */
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

    if ((uint16_t)(GAME_MAX_Y) >= (uint16_t)(PLAY_TOP)) {
        span = (uint16_t)((GAME_MAX_Y) - (PLAY_TOP) + 1);

        if (span > 0) {
            return (uint8_t)(_XL_RAND() % span);
        }
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

/* Spawn one enemy on the right edge, avoiding obvious overlaps. */
static void spawn_enemy(void)
{
    uint16_t i;
    uint8_t idx;
    uint8_t y;

    idx = find_dead_enemy();

    if ((uint16_t)idx >= (uint16_t)MAX_ENEMIES) {
        return;
    }

    y = random_play_row();

    /* Do not spawn directly on top of a live bullet. */
    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        if (bullets[i].alive &&
            bullets[i].x == (uint8_t)GAME_MAX_X &&
            bullets[i].y == y) {
            return;
        }
    }

    /* Do not spawn in a row already used by another live enemy. */
    for (i = 0; i < (uint16_t)MAX_ENEMIES; ++i) {
        if (enemies[i].alive && enemies[i].y == y) {
            return;
        }
    }

    enemies[idx].x = (uint8_t)GAME_MAX_X;
    enemies[idx].y = y;
    enemies[idx].tile_id = ENEMY_TILE_ID;
    enemies[idx].color_id = ENEMY_COLOR_ID;
    enemies[idx].alive = 1;

    _XL_TOCK_SOUND();
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

    old_player_x = 0;
    old_player_y = 0;
    old_player_alive = 0;

    player_alive = 1;
    game_over = 0;
    explosion_shown = 0;

    fire_cooldown = 0;
    spawn_timer = ENEMY_INTERVAL;

    score = 0;
    last_score = (uint16_t)65535; /* Force HUD update on first frame. */

    player_x = (uint8_t)PLAYER_START_X;
    player_y = (uint8_t)PLAYER_START_Y;
}

/* Update logical game state for one frame. */
static void update_game(uint8_t input)
{
    uint16_t i;
    uint16_t j;
    uint16_t a_old;
    uint16_t a_new;
    uint16_t b_old;
    uint16_t b_new;
    uint8_t idx;
    uint8_t nx;

    if (fire_cooldown > 0) {
        --fire_cooldown;
    }

    if (spawn_timer > 0) {
        --spawn_timer;
    } else {
        spawn_enemy();
    }

    /* Remember previous-frame state before changing anything. */
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

    /* Player movement and firing. */
    if (player_alive) {
        if (_XL_LEFT(input)) {
            if ((uint16_t)player_x > 0) {
                --player_x;
            }
        } else if (_XL_RIGHT(input)) {
            if ((uint16_t)player_x + 1 <= (uint16_t)GAME_MAX_X) {
                ++player_x;
            }
        } else if (_XL_UP(input)) {
            if ((uint16_t)player_y > (uint16_t)PLAY_TOP) {
                --player_y;
            }
        } else if (_XL_DOWN(input)) {
            if ((uint16_t)player_y + 1 <= (uint16_t)GAME_MAX_Y) {
                ++player_y;
            }
        }

        if (_XL_FIRE(input) &&
            fire_cooldown == 0 &&
            (uint16_t)player_x + 1 <= (uint16_t)GAME_MAX_X) {

            idx = find_dead_bullet();

            if ((uint16_t)idx < (uint16_t)MAX_BULLETS) {
                bullets[idx].x = (uint8_t)((uint16_t)player_x + 1);
                bullets[idx].y = player_y;
                bullets[idx].tile_id = BULLET_TILE_ID;
                bullets[idx].color_id = BULLET_COLOR_ID;
                bullets[idx].alive = 1;

                fire_cooldown = FIRE_COOLDOWN;
                _XL_SHOOT_SOUND();
            }
        }
    }

    /* Move bullets to the right. */
    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        if (!bullets[i].alive) {
            continue;
        }

        nx = bullets[i].x;

        if ((uint16_t)nx + 1 <= (uint16_t)GAME_MAX_X) {
            ++nx;
            bullets[i].x = nx;
        } else {
            /* Bullet left the playable area. */
            bullets[i].alive = 0;
        }
    }

    /* Bullet/enemy collisions, including one-step crossing cases. */
    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        if (!bullets[i].alive) {
            continue;
        }

        for (j = 0; j < (uint16_t)MAX_ENEMIES; ++j) {
            if (!enemies[j].alive) {
                continue;
            }

            if (bullets[i].y != enemies[j].y) {
                continue;
            }

            a_new = bullets[i].x;
            b_new = enemies[j].x;
            a_old = old_bullet_x[i];
            b_old = old_enemy_x[j];

            if ((a_new == b_new) ||
                (old_bullet_alive[i] &&
                 old_enemy_alive[j] &&
                 (((a_old < b_new) && (a_new > b_old)) ||
                  ((b_old < a_new) && (b_new > a_old))))) {

                bullets[i].alive = 0;
                enemies[j].alive = 0;

                if (score < (uint16_t)50000) {
                    score += 10;

                    if (score > (uint16_t)50000) {
                        score = (uint16_t)50000;
                    }
                }

                _XL_PING_SOUND();
                break;
            }
        }
    }


    /* Move enemies to the left. */
    for (i = 0; i < (uint16_t)MAX_ENEMIES; ++i) {
        if (!enemies[i].alive) {
            continue;
        }

        nx = enemies[i].x;

        if ((uint16_t)nx > 0) {
            --nx;
            enemies[i].x = nx;
        } else {
            /* Enemy escaped off the left edge. */
            enemies[i].alive = 0;
        }
    }

    /* Bullet/enemy collisions, including one-step crossing cases. */
    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        if (!bullets[i].alive) {
            continue;
        }

        for (j = 0; j < (uint16_t)MAX_ENEMIES; ++j) {
            if (!enemies[j].alive) {
                continue;
            }

            if (bullets[i].y != enemies[j].y) {
                continue;
            }

            a_new = bullets[i].x;
            b_new = enemies[j].x;
            a_old = old_bullet_x[i];
            b_old = old_enemy_x[j];

            if ((a_new == b_new) ||
                (old_bullet_alive[i] &&
                 old_enemy_alive[j] &&
                 (((a_old < b_new) && (a_new > b_old)) ||
                  ((b_old < a_new) && (b_new > a_old))))) {

                bullets[i].alive = 0;
                enemies[j].alive = 0;

                if (score < (uint16_t)50000) {
                    score += 10;

                    if (score > (uint16_t)50000) {
                        score = (uint16_t)50000;
                    }
                }

                _XL_PING_SOUND();
                break;
            }
        }
    }

    /* Player/enemy collision, including one-step crossing cases. */
    if (player_alive && !game_over) {
        for (j = 0; j < (uint16_t)MAX_ENEMIES; ++j) {
            if (!enemies[j].alive || player_y != enemies[j].y) {
                continue;
            }

            a_new = player_x;
            b_new = enemies[j].x;
            a_old = old_player_x;
            b_old = old_enemy_x[j];

            if ((a_new == b_new) ||
                (old_player_alive &&
                 old_enemy_alive[j] &&
                 (((a_old < b_new) && (a_new > b_old)) ||
                  ((b_old < a_new) && (b_new > a_old))))) {

                game_over = 1;
                player_alive = 0;
                _XL_EXPLOSION_SOUND();
                break;
            }
        }
    }
}

/*
   Update the visible screen state.
   This is where unchanged tiles are skipped.
*/
static void draw_changed_tiles(void)
{
    uint16_t i;

    /* Player ship, or explosion if the player just died. */
    if (player_alive) {
        set_cell(player_x, player_y, PLAYER_TILE_ID, PLAYER_COLOR_ID);

        if (old_player_alive &&
            (old_player_x != player_x || old_player_y != player_y)) {
            erase_cell(old_player_x, old_player_y,
                       PLAYER_TILE_ID, PLAYER_COLOR_ID);
        }
    } else {
        if (!explosion_shown) {
            set_cell(player_x, player_y,
                     EXPLOSION_TILE_ID, EXPLOSION_COLOR_ID);
            explosion_shown = 1;
        }

        if (old_player_alive &&
            (old_player_x != player_x || old_player_y != player_y)) {
            erase_cell(old_player_x, old_player_y,
                       PLAYER_TILE_ID, PLAYER_COLOR_ID);
        }
    }

    /* Bullets. */
    for (i = 0; i < (uint16_t)MAX_BULLETS; ++i) {
        if (bullets[i].alive) {
            set_cell(bullets[i].x, bullets[i].y,
                     BULLET_TILE_ID, BULLET_COLOR_ID);
        }

        /*
           Erase the previous cell only if this entity was alive last frame
           and either moved or died.  Newly spawned entities do not erase
           their dummy old position.
        */
        if (old_bullet_alive[i] &&
            ((old_bullet_x[i] != bullets[i].x ||
              old_bullet_y[i] != bullets[i].y) ||
             !bullets[i].alive)) {

            erase_cell(old_bullet_x[i], old_bullet_y[i],
                       BULLET_TILE_ID, BULLET_COLOR_ID);
        }
    }

    /* Enemies. */
    for (i = 0; i < (uint16_t)MAX_ENEMIES; ++i) {
        if (enemies[i].alive) {
            set_cell(enemies[i].x, enemies[i].y,
                     ENEMY_TILE_ID, ENEMY_COLOR_ID);
        }

        if (old_enemy_alive[i] &&
            ((old_enemy_x[i] != enemies[i].x ||
              old_enemy_y[i] != enemies[i].y) ||
             !enemies[i].alive)) {

            erase_cell(old_enemy_x[i], old_enemy_y[i],
                       ENEMY_TILE_ID, ENEMY_COLOR_ID);
        }
    }
}

/* Update score display only when the score value changes. */
static void update_hud(void)
{
    if (score != last_score && STATE_H > 1 && STATE_W >= 5) {
        last_score = score;
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0, 0, 1, score);
    }
}

/* Wait a number of frames using the Cross-Lib slowdown API. */
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

    if ((uint16_t)STATE_W >= 5) {
        _XL_PRINT(0, 0, "READY");
    } else {
        _XL_PRINT(0, 0, "R");
    }
}

static void show_game_over(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);

    if ((uint16_t)STATE_W >= 9) {
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

    /* Wait for the player to start. */
    _XL_WAIT_FOR_INPUT();

    /* Clear the READY screen before gameplay begins. */
    reset_screen_state();
    _XL_CLEAR_SCREEN();
    last_score = (uint16_t)65535;

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

                /* Let the player see the message before restarting. */
                wait_frames(30);
            }

            /* Wait for any key to restart. */
            _XL_WAIT_FOR_INPUT();

            reset_screen_state();
            _XL_CLEAR_SCREEN();
            reset_game();
            last_score = (uint16_t)65535;
            game_over_shown = 0;

            if (STATE_H > 1) {
                show_ready();
            }

            /* Wait for the player to start the next run. */
            _XL_WAIT_FOR_INPUT();
        }

        _XL_SLOW_DOWN((uint16_t)_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}
