#include "cross_lib.h"

/* Tile assignments */
#define T_GUN_TOP       _TILE_1
#define T_GUN_BOT       _TILE_2
#define T_DUCK_TOP      _TILE_3
#define T_DUCK_BOT      _TILE_4
#define T_HOWL_TOP      _TILE_5
#define T_HOWL_BOT      _TILE_6
#define T_RABBIT_TOP    _TILE_7
#define T_RABBIT_BOT    _TILE_8
#define T_BIRD_L        _TILE_9
#define T_BIRD_R        _TILE_10
#define T_EXTRA_TOP     _TILE_11
#define T_EXTRA_BOT     _TILE_12
#define T_BULLET        _TILE_13

/* Target types */
#define TYPE_DUCK       0
#define TYPE_HOWL       1
#define TYPE_RABBIT     2
#define TYPE_BIRD       3
#define TYPE_EXTRA      4

#define MAX_TARGETS     16
#define MAX_BULLETS     4
#define START_AMMO      250
#define BULLET_SPEED    2
#define TARGET_BASE_SPD 1
#define BIRD_SPEED      2
#define EXTRA_CHANCE    300
#define SPAWN_INTERVAL  4

/* Row Y positions (top tile of each target) */
#define ROW_0           (YSize - 18)
#define ROW_1           (YSize - 14)
#define ROW_2           (YSize - 10)
#define ROW_BIRD        (YSize - 6)
#define GUN_Y           (YSize - 3)

typedef struct {
    uint8_t active;
    uint8_t type;
    uint8_t x;
    uint8_t y;
    short   dir;
    uint8_t speed;
} Target;

typedef struct {
    uint8_t active;
    uint8_t x;
    uint8_t y;
} Bullet;

static void draw_target(const Target *t)
{
    if (t->type == TYPE_BIRD) {
        _XL_DRAW(t->x, t->y, T_BIRD_L, _XL_CYAN);
        _XL_DRAW(t->x + 1, t->y, T_BIRD_R, _XL_CYAN);
    } else if (t->type == TYPE_EXTRA) {
        _XL_DRAW(t->x, t->y, T_EXTRA_TOP, _XL_YELLOW);
        _XL_DRAW(t->x, t->y + 1, T_EXTRA_BOT, _XL_YELLOW);
    } else {
        uint8_t top_tile;
        uint8_t bot_tile;
        if (t->type == TYPE_DUCK) {
            top_tile = T_DUCK_TOP;
            bot_tile = T_DUCK_BOT;
        } else if (t->type == TYPE_HOWL) {
            top_tile = T_HOWL_TOP;
            bot_tile = T_HOWL_BOT;
        } else {
            top_tile = T_RABBIT_TOP;
            bot_tile = T_RABBIT_BOT;
        }
        _XL_DRAW(t->x, t->y, top_tile, _XL_GREEN);
        _XL_DRAW(t->x, t->y + 1, bot_tile, _XL_GREEN);
    }
}

static void delete_target(const Target *t)
{
    if (t->type == TYPE_BIRD) {
        _XL_DELETE(t->x, t->y);
        _XL_DELETE(t->x + 1, t->y);
    } else {
        _XL_DELETE(t->x, t->y);
        _XL_DELETE(t->x, t->y + 1);
    }
}

static uint8_t bullet_hits_target(const Bullet *b, const Target *t)
{
    if (t->type == TYPE_BIRD) {
        return (b->y == t->y &&
                (b->x == t->x || b->x == t->x + 1));
    } else {
        return (b->x == t->x &&
                (b->y == t->y || b->y == t->y + 1));
    }
}

static uint8_t get_row_y(uint8_t row_idx)
{
    if (row_idx == 0) return ROW_0;
    if (row_idx == 1) return ROW_1;
    if (row_idx == 2) return ROW_2;
    return ROW_BIRD;
}

static void spawn_target(Target *targets, uint8_t idx, uint8_t row_idx)
{
    uint8_t type;
    short   dir;
    uint8_t x;
    uint16_t r;

    targets[idx].active = 1;
    targets[idx].y      = get_row_y(row_idx);

    if (row_idx == 3) {
        /* ROW_BIRD: right to left */
        type  = TYPE_BIRD;
        dir   = -1;
        x     = XSize - 2;
        targets[idx].speed = BIRD_SPEED;
    } else {
        r = _XL_RAND();
        if (r < EXTRA_CHANCE) {
            type = TYPE_EXTRA;
        } else {
            type = (uint8_t)(r % 3);
        }
        targets[idx].speed = TARGET_BASE_SPD;

        /* ROW_0 and ROW_2: left to right */
        /* ROW_1: right to left */
        if (row_idx == 1) {
            dir = -1;
            x   = XSize - 2;
        } else {
            dir = 1;
            x   = 1;
        }
    }

    targets[idx].type  = type;
    targets[idx].x     = x;
    targets[idx].dir   = dir;
}

static void game_loop(void)
{
    Target targets[MAX_TARGETS];
    Bullet bullets[MAX_BULLETS];
    uint8_t gun_x;
    uint16_t ammo;
    uint16_t score;
    uint8_t i, j;
    uint8_t input;
    uint8_t fired;
    uint8_t game_over;
    uint16_t spawn_timer;

    for (i = 0; i < MAX_TARGETS; i++) {
        targets[i].active = 0;
    }
    for (i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }

    gun_x       = XSize / 2;
    ammo        = START_AMMO;
    score       = 0;
    game_over   = 0;
    spawn_timer = 0;

    _XL_DRAW(gun_x, GUN_Y, T_GUN_TOP, _XL_WHITE);
    _XL_DRAW(gun_x, GUN_Y + 1, T_GUN_BOT, _XL_WHITE);

    while (!game_over) {
        input = _XL_INPUT();

        /* Move gun */
        if (_XL_LEFT(input)) {
            if (gun_x > 0) {
                _XL_DELETE(gun_x, GUN_Y);
                _XL_DELETE(gun_x, GUN_Y + 1);
                gun_x--;
                _XL_DRAW(gun_x, GUN_Y, T_GUN_TOP, _XL_WHITE);
                _XL_DRAW(gun_x, GUN_Y + 1, T_GUN_BOT, _XL_WHITE);
            }
        } else if (_XL_RIGHT(input)) {
            if (gun_x < XSize - 1) {
                _XL_DELETE(gun_x, GUN_Y);
                _XL_DELETE(gun_x, GUN_Y + 1);
                gun_x++;
                _XL_DRAW(gun_x, GUN_Y, T_GUN_TOP, _XL_WHITE);
                _XL_DRAW(gun_x, GUN_Y + 1, T_GUN_BOT, _XL_WHITE);
            }
        }

        /* Fire */
        fired = 0;
        if (_XL_FIRE(input) && ammo > 0) {
            for (i = 0; i < MAX_BULLETS; i++) {
                if (!bullets[i].active) {
                    bullets[i].active = 1;
                    bullets[i].x      = gun_x;
                    bullets[i].y      = GUN_Y - 1;
                    _XL_DRAW(bullets[i].x, bullets[i].y, T_BULLET, _XL_WHITE);
                    fired = 1;
                    break;
                }
            }
            if (fired) {
                ammo--;
                _XL_SHOOT_SOUND();
            }
        }

        /* Update bullets */
        for (i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) continue;

            _XL_DELETE(bullets[i].x, bullets[i].y);

            if (bullets[i].y <= BULLET_SPEED) {
                bullets[i].active = 0;
            } else {
                bullets[i].y -= BULLET_SPEED;
                _XL_DRAW(bullets[i].x, bullets[i].y, T_BULLET, _XL_WHITE);

                /* Collision check */
                for (j = 0; j < MAX_TARGETS; j++) {
                    if (!targets[j].active) continue;
                    if (bullet_hits_target(&bullets[i], &targets[j])) {
                        uint8_t ttype = targets[j].type;
                        delete_target(&targets[j]);
                        _XL_DELETE(bullets[i].x, bullets[i].y);
                        targets[j].active = 0;
                        bullets[i].active = 0;

                        if (ttype == TYPE_EXTRA) {
                            ammo += START_AMMO;
                            _XL_TOCK_SOUND();
                        } else {
                            score += 10;
                            _XL_PING_SOUND();
                        }
                        break;
                    }
                }

            }
        }

        /* Update targets */
        for (i = 0; i < MAX_TARGETS; i++) {
            if (!targets[i].active) continue;

            delete_target(&targets[i]);

            if (targets[i].dir > 0) {
                uint8_t new_x = targets[i].x + targets[i].speed;
                uint8_t limit;
                if (targets[i].type == TYPE_BIRD) {
                    limit = XSize - 2;
                } else {
                    limit = XSize - 1;
                }
                if (new_x > limit) {
                    targets[i].active = 0;
                    continue;
                }
                targets[i].x = new_x;
            } else {
                if (targets[i].x < targets[i].speed) {
                    targets[i].active = 0;
                    continue;
                }
                targets[i].x -= targets[i].speed;
            }

            draw_target(&targets[i]);
        }

        /* Spawn */
        spawn_timer++;
        if (spawn_timer >= SPAWN_INTERVAL) {
            spawn_timer = 0;
            for (i = 0; i < MAX_TARGETS; i++) {
                if (!targets[i].active) {
                    uint8_t row_idx = (uint8_t)(_XL_RAND() % 4);
                    spawn_target(targets, i, row_idx);
                    draw_target(&targets[i]);
                    break;
                }
            }
        }

        /* HUD */
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 0, "SCORE");
        _XL_PRINTD(5, 0, 1, score);
        _XL_PRINT(XSize - 8, 0, "AMMO");
        _XL_PRINTD(XSize - 2, 0, 1, ammo);

        /* Game over check */
        if (ammo == 0) {
            uint8_t any_bullet = 0;
            for (i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) { any_bullet = 1; break; }
            }
            if (!any_bullet) {
                game_over = 1;
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    /* Game Over screen */
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(XSize / 4, YSize / 3, "GAME OVER");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_SLEEP(1);
    _XL_PRINT(2, YSize / 2 - 1, "FINAL SCORE");
    _XL_PRINTD(10, YSize / 2 - 1, 1, score);
    _XL_PRINT(2, YSize / 2 + 2, "PRESS ANY KEY");

    _XL_WAIT_FOR_INPUT();
}

int main(void)
{
    for (;;) {
        _XL_INIT_GRAPHICS();
        _XL_INIT_INPUT();
        _XL_INIT_SOUND();
        _XL_CLEAR_SCREEN();
        game_loop();
    }
    return 0;
}
