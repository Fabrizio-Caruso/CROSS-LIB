#include "cross_lib.h"

#define MAX_ENEMIES 8
#define MAX_BULLETS 16

#define PLAYER_TYPE 0
#define ENEMY_TYPE 1

#define LEFT_DIR 0
#define RIGHT_DIR 1

#define STATE_PLAY 0
#define STATE_GAME_OVER 1

#define TOP_ROW 1

#define SHIELD_DURATION 30
#define SHIELD_COOLDOWN 60

#define FIRE_COOLDOWN 3

#define SPAWN_MIN 20
#define SPAWN_VAR 40

struct bullet {
    uint8_t active;
    uint8_t x;
    uint8_t y;
};

struct enemy {
    uint8_t active;
    uint8_t y;
    uint16_t vx;
    uint8_t dir;
    uint8_t timer;
    uint8_t fire_timer;
};

static struct enemy enemies[MAX_ENEMIES];
static struct bullet player_bullets[MAX_BULLETS];
static struct bullet enemy_bullets[MAX_BULLETS];

static uint8_t state;
static uint8_t player_active;
static uint16_t player_vx;
static uint8_t player_y;
static uint8_t lives;
static uint16_t score;
static uint16_t last_score;
static uint8_t last_lives;

static uint8_t shield_frames;
static uint8_t shield_cooldown;
static uint8_t shield_drawn;

static uint8_t fire_cooldown;
static uint8_t spawn_timer;

static uint8_t enemy_count;
static uint8_t player_bullet_count;
static uint8_t enemy_bullet_count;

static uint8_t prev_down;

static uint8_t sprite_left(uint16_t vx)
{
    return (uint8_t)(vx / 2);
}

static uint8_t sprite_right(uint16_t vx)
{
    return (uint8_t)(vx / 2 + 1);
}

static uint8_t bullet_x_from_virtual(uint16_t vx)
{
    if ((vx % 2) == 0) {
        return (uint8_t)(vx / 2);
    } else {
        return (uint8_t)(vx / 2 + 1);
    }
}

static uint8_t sprite_tile(uint8_t type, uint16_t vx, uint8_t side)
{
    if (type == PLAYER_TYPE) {
        if ((vx % 2) == 0) {
            if (side == 0) return _TILE_0;
            return _TILE_1;
        } else {
            if (side == 0) return _TILE_2;
            return _TILE_3;
        }
    } else {
        if ((vx % 2) == 0) {
            if (side == 0) return _TILE_4;
            return _TILE_5;
        } else {
            if (side == 0) return _TILE_6;
            return _TILE_7;
        }
    }
}

static uint8_t sprite_color(uint8_t type)
{
    if (type == PLAYER_TYPE) {
        return _XL_GREEN;
    } else {
        return _XL_RED;
    }
}

static void sprite_draw(uint8_t type, uint16_t vx, uint8_t y)
{
    uint8_t lx;
    uint8_t rx;
    uint8_t tl;
    uint8_t tr;
    uint8_t color;

    if (y >= YSize) return;

    lx = sprite_left(vx);
    rx = sprite_right(vx);

    if (lx >= XSize || rx >= XSize) return;

    color = sprite_color(type);
    tl = sprite_tile(type, vx, 0);
    tr = sprite_tile(type, vx, 1);

    _XL_DRAW(lx, y, tl, color);
    _XL_DRAW(rx, y, tr, color);
}

static void sprite_delete(uint8_t type, uint16_t vx, uint8_t y)
{
    uint8_t lx;
    uint8_t rx;

    if (y >= YSize) return;

    lx = sprite_left(vx);
    rx = sprite_right(vx);

    if (lx >= XSize || rx >= XSize) return;

    _XL_DELETE(lx, y);
    _XL_DELETE(rx, y);
}

static void bullet_draw(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
{
    if (x < XSize && y < YSize) {
        _XL_DRAW(x, y, tile, color);
    }
}

static void bullet_delete(uint8_t x, uint8_t y)
{
    if (x < XSize && y < YSize) {
        _XL_DELETE(x, y);
    }
}

static uint8_t sprite_x_overlap(uint16_t a, uint16_t b)
{
    uint8_t al;
    uint8_t ar;
    uint8_t bl;
    uint8_t br;

    al = (uint8_t)(a / 2);
    ar = (uint8_t)(a / 2 + 1);
    bl = (uint8_t)(b / 2);
    br = (uint8_t)(b / 2 + 1);

    if (al <= br && bl <= ar) {
        return 1;
    }

    return 0;
}

static uint8_t message_y(void)
{
    return (uint8_t)(YSize / 2);
}

static void clear_message(void)
{
    uint8_t i;
    uint8_t x;
    char msg[10];

    if (XSize >= 9) {
        x = (uint8_t)((XSize - 9) / 2);
    } else {
        x = 0;
    }

    _XL_SET_TEXT_COLOR(_XL_WHITE);

    if (XSize >= 9) {
        for (i = 0; i < 9; i++) {
            msg[i] = ' ';
        }
        msg[9] = 0;
        _XL_PRINT(x, message_y(), msg);
    } else {
        for (i = 0; i < 4; i++) {
            msg[i] = ' ';
        }
        msg[4] = 0;
        _XL_PRINT(0, message_y(), msg);
    }
}

static void show_game_over(void)
{
    uint8_t x;
    uint8_t y;

    y = message_y();
    _XL_SET_TEXT_COLOR(_XL_WHITE);

    if (XSize >= 9) {
        x = (uint8_t)((XSize - 9) / 2);
        _XL_PRINT(x, y, "GAME OVER");
    } else {
        _XL_PRINT(0, y, "OVER");
    }
}

static void update_hud(void)
{
    if (score != last_score || lives != last_lives) {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0, 0, 5, score);
        _XL_PRINTD((uint8_t)(XSize - 1), 0, 1, lives);
        last_score = score;
        last_lives = lives;
    }
}

static void add_score(uint8_t points)
{
    uint16_t ns;

    ns = score + points;
    if (ns < score) {
        ns = 0xFFFF;
    }

    score = ns;
    update_hud();
}

static uint16_t max_virtual_x(void)
{
    return (uint16_t)(2 * (XSize - 2));
}

static uint16_t center_virtual_x(void)
{
    uint8_t left;

    left = (uint8_t)(XSize / 2 - 1);
    return (uint16_t)(left * 2);
}

static uint8_t player_bullet_at(uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (player_bullets[i].active &&
            player_bullets[i].x == x &&
            player_bullets[i].y == y) {
            return 1;
        }
    }

    return 0;
}

static uint8_t enemy_bullet_at(uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (enemy_bullets[i].active &&
            enemy_bullets[i].x == x &&
            enemy_bullets[i].y == y) {
            return 1;
        }
    }

    return 0;
}

static void remove_player_bullet(uint8_t i)
{
    if (!player_bullets[i].active) return;

    bullet_delete(player_bullets[i].x, player_bullets[i].y);

    player_bullets[i].active = 0;
    if (player_bullet_count > 0) {
        player_bullet_count = player_bullet_count - 1;
    }
}

static uint8_t remove_player_bullet_at(uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (player_bullets[i].active &&
            player_bullets[i].x == x &&
            player_bullets[i].y == y) {
            remove_player_bullet(i);
            return 1;
        }
    }

    return 0;
}

static void remove_enemy_bullet(uint8_t i)
{
    if (!enemy_bullets[i].active) return;

    bullet_delete(enemy_bullets[i].x, enemy_bullets[i].y);

    enemy_bullets[i].active = 0;
    if (enemy_bullet_count > 0) {
        enemy_bullet_count = enemy_bullet_count - 1;
    }
}

static uint8_t remove_enemy_bullet_at(uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (enemy_bullets[i].active &&
            enemy_bullets[i].x == x &&
            enemy_bullets[i].y == y) {
            remove_enemy_bullet(i);
            return 1;
        }
    }

    return 0;
}

static uint8_t enemy_at_index(uint8_t i, uint8_t x, uint8_t y)
{
    uint8_t lx;
    uint8_t rx;

    if (!enemies[i].active) return 0;
    if (enemies[i].y != y) return 0;

    lx = sprite_left(enemies[i].vx);
    rx = sprite_right(enemies[i].vx);

    if (x == lx || x == rx) {
        return 1;
    }

    return 0;
}

static uint8_t enemy_at(uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_at_index(i, x, y)) {
            return 1;
        }
    }

    return 0;
}

static uint8_t player_at(uint8_t x, uint8_t y)
{
    uint8_t lx;
    uint8_t rx;

    if (!player_active) return 0;
    if (y != player_y) return 0;

    lx = sprite_left(player_vx);
    rx = sprite_right(player_vx);

    if (x == lx || x == rx) {
        return 1;
    }

    return 0;
}

static uint8_t shield_at(uint8_t x, uint8_t y)
{
    uint8_t lx;
    uint8_t rx;

    if (!shield_drawn) return 0;
    if (player_y < 1) return 0;

    if (y != (player_y - 1)) return 0;

    lx = sprite_left(player_vx);
    rx = sprite_right(player_vx);

    if (x == lx || x == rx) {
        return 1;
    }

    return 0;
}

static void remove_enemy(uint8_t i, uint8_t award)
{
    if (!enemies[i].active) return;

    sprite_delete(ENEMY_TYPE, enemies[i].vx, enemies[i].y);

    enemies[i].active = 0;
    if (enemy_count > 0) {
        enemy_count = enemy_count - 1;
    }

    if (award) {
        add_score(10);
        _XL_PING_SOUND();
    }
}

static uint8_t remove_enemy_at(uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_at_index(i, x, y)) {
            remove_enemy(i, 1);
            return 1;
        }
    }

    return 0;
}

static void clear_shield_cells(uint16_t vx)
{
    uint8_t y;
    uint8_t lx;
    uint8_t rx;

    if (player_y < 1) return;

    y = player_y - 1;
    lx = sprite_left(vx);
    rx = sprite_right(vx);

    if (lx < XSize && y < YSize) {
        remove_enemy_at(lx, y);
        remove_player_bullet_at(lx, y);
        remove_enemy_bullet_at(lx, y);
    }

    if (rx < XSize && y < YSize && rx != lx) {
        remove_enemy_at(rx, y);
        remove_player_bullet_at(rx, y);
        remove_enemy_bullet_at(rx, y);
    }
}

static void shield_draw(uint16_t vx)
{
    uint8_t y;
    uint8_t lx;
    uint8_t rx;

    if (player_y < 1) return;

    y = player_y - 1;
    lx = sprite_left(vx);
    rx = sprite_right(vx);

    if (lx < XSize && y < YSize) {
        _XL_DRAW(lx, y, _TILE_10, _XL_CYAN);
    }

    if (rx < XSize && y < YSize && rx != lx) {
        _XL_DRAW(rx, y, _TILE_10, _XL_CYAN);
    }
}

static void shield_delete(uint16_t vx)
{
    uint8_t y;
    uint8_t lx;
    uint8_t rx;

    if (player_y < 1) return;

    y = player_y - 1;
    lx = sprite_left(vx);
    rx = sprite_right(vx);

    if (lx < XSize && y < YSize) {
        _XL_DELETE(lx, y);
    }

    if (rx < XSize && y < YSize && rx != lx) {
        _XL_DELETE(rx, y);
    }
}

static void shield_update(uint16_t old_vx, uint16_t new_vx)
{
    uint8_t old_l;
    uint8_t old_r;
    uint8_t new_l;
    uint8_t new_r;
    uint8_t y;

    if (!shield_drawn) return;
    if (player_y < 1) return;

    y = player_y - 1;

    old_l = sprite_left(old_vx);
    old_r = sprite_right(old_vx);
    new_l = sprite_left(new_vx);
    new_r = sprite_right(new_vx);

    if (old_l != new_l && old_l != new_r) {
        bullet_delete(old_l, y);
    }

    if (old_r != new_l && old_r != new_r) {
        bullet_delete(old_r, y);
    }

    if (new_l != old_l && new_l != old_r) {
        if (new_l < XSize && y < YSize) {
            remove_enemy_at(new_l, y);
            remove_player_bullet_at(new_l, y);
            remove_enemy_bullet_at(new_l, y);
            bullet_draw(new_l, y, _TILE_10, _XL_CYAN);
        }
    }

    if (new_r != old_l && new_r != old_r) {
        if (new_r < XSize && y < YSize) {
            remove_enemy_at(new_r, y);
            remove_player_bullet_at(new_r, y);
            remove_enemy_bullet_at(new_r, y);
            bullet_draw(new_r, y, _TILE_10, _XL_CYAN);
        }
    }
}

static void create_player_bullet(uint8_t x, uint8_t y)
{
    uint8_t i;

    if (player_bullet_count >= MAX_BULLETS) return;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!player_bullets[i].active) {
            player_bullets[i].active = 1;
            player_bullets[i].x = x;
            player_bullets[i].y = y;
            player_bullet_count = player_bullet_count + 1;
            bullet_draw(x, y, _TILE_8, _XL_YELLOW);
            return;
        }
    }
}

static void create_enemy_bullet(uint8_t x, uint8_t y)
{
    uint8_t i;

    if (enemy_bullet_count >= MAX_BULLETS) return;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!enemy_bullets[i].active) {
            enemy_bullets[i].active = 1;
            enemy_bullets[i].x = x;
            enemy_bullets[i].y = y;
            enemy_bullet_count = enemy_bullet_count + 1;
            bullet_draw(x, y, _TILE_9, _XL_MAGENTA);
            return;
        }
    }
}

static void player_hit(void)
{
    uint8_t i;

    if (state != STATE_PLAY) return;

    if (lives > 0) {
        lives = lives - 1;
    }

    update_hud();
    _XL_EXPLOSION_SOUND();

    if (shield_drawn) {
        shield_delete(player_vx);
        shield_drawn = 0;
    }

    shield_frames = 0;
    shield_cooldown = SHIELD_COOLDOWN;

    if (player_active) {
        sprite_delete(PLAYER_TYPE, player_vx, player_y);
        player_active = 0;
    }

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            remove_enemy(i, 0);
        }
    }

    for (i = 0; i < MAX_BULLETS; i++) {
        remove_player_bullet(i);
        remove_enemy_bullet(i);
    }

    if (lives == 0) {
        state = STATE_GAME_OVER;
        show_game_over();
    } else {
        _XL_SLEEP(1);

        player_vx = center_virtual_x();
        player_y = (uint8_t)(YSize - 1);
        player_active = 1;

        sprite_draw(PLAYER_TYPE, player_vx, player_y);
    }
}

static uint8_t player_move_blocked(uint16_t vx)
{
    uint8_t i;
    uint8_t lx;
    uint8_t rx;

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active &&
            enemies[i].y == player_y &&
            sprite_x_overlap(vx, enemies[i].vx)) {
            return 1;
        }
    }

    lx = sprite_left(vx);
    rx = sprite_right(vx);

    for (i = 0; i < MAX_BULLETS; i++) {
        if (enemy_bullets[i].active &&
            enemy_bullets[i].y == player_y &&
            (enemy_bullets[i].x == lx || enemy_bullets[i].x == rx)) {
            return 1;
        }
    }

    return 0;
}

static void spawn_enemy(void)
{
    uint8_t i;
    uint8_t dir;
    uint8_t x;
    uint8_t top;
    uint16_t vx;
    uint16_t maxv;

    top = TOP_ROW;

    if (top >= player_y) return;
    if (enemy_count >= MAX_ENEMIES) return;

    maxv = max_virtual_x();

    vx = (uint16_t)(_XL_RAND() % (maxv + 1));
    dir = (uint8_t)(_XL_RAND() & 1u);

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            break;
        }
    }

    if (i == MAX_ENEMIES) return;

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active &&
            enemies[i].y == top &&
            sprite_x_overlap(vx, enemies[i].vx)) {
            return;
        }
    }

    x = sprite_left(vx);
    if (player_bullet_at(x, top) || enemy_bullet_at(x, top)) {
        return;
    }

    x = sprite_right(vx);
    if (player_bullet_at(x, top) || enemy_bullet_at(x, top)) {
        return;
    }

    enemies[i].active = 1;
    enemies[i].y = top;
    enemies[i].vx = vx;
    enemies[i].dir = dir;
    enemies[i].timer = 0;
    enemies[i].fire_timer = (uint8_t)(80 + (_XL_RAND() % 80));

    enemy_count = enemy_count + 1;

    sprite_draw(ENEMY_TYPE, vx, top);
}

static void update_enemies(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t moved;
    uint8_t overlap;
    uint8_t hit_player;
    uint8_t old_y;
    uint8_t new_y;
    uint8_t new_timer;
    uint8_t lx;
    uint8_t rx;
    uint16_t maxv;
    uint16_t old_vx;
    uint16_t new_vx;
    uint8_t new_dir;
    struct enemy *e;

    maxv = max_virtual_x();

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;

        e = &enemies[i];

        old_vx = e->vx;
        old_y = e->y;

        new_dir = e->dir;
        new_vx = e->vx;

        if (new_dir == RIGHT_DIR) {
            if (new_vx < maxv) {
                new_vx = new_vx + 1;
            } else {
                new_dir = LEFT_DIR;
            }
        } else {
            if (new_vx > 0) {
                new_vx = new_vx - 1;
            } else {
                new_dir = RIGHT_DIR;
            }
        }

        new_timer = e->timer;
        new_y = e->y;

        if (new_timer == 0) {
            new_y = e->y + 1;
            new_timer = 2;
        } else {
            new_timer = new_timer - 1;
        }

        overlap = 0;

        if (new_y < YSize) {
            for (j = 0; j < MAX_ENEMIES; j++) {
                if (j == i) continue;

                if (enemies[j].active &&
                    enemies[j].y == new_y &&
                    sprite_x_overlap(new_vx, enemies[j].vx)) {
                    overlap = 1;
                    break;
                }
            }
        }

        if (overlap) {
            new_vx = old_vx;
            new_y = old_y;
            new_timer = e->timer;

            if (new_dir == RIGHT_DIR) {
                new_dir = LEFT_DIR;
            } else {
                new_dir = RIGHT_DIR;
            }
        }

        lx = sprite_left(new_vx);
        rx = sprite_right(new_vx);

        if (new_y < YSize &&
            (player_bullet_at(lx, new_y) || player_bullet_at(rx, new_y))) {
            remove_enemy(i, 1);

            if (player_bullet_at(lx, new_y)) {
                remove_player_bullet_at(lx, new_y);
            }

            if (player_bullet_at(rx, new_y)) {
                remove_player_bullet_at(rx, new_y);
            }

            if (state != STATE_PLAY) return;
            continue;
        }

        if (new_y < YSize &&
            (enemy_bullet_at(lx, new_y) || enemy_bullet_at(rx, new_y))) {
            new_vx = old_vx;
            new_y = old_y;
            new_timer = e->timer;

            if (new_dir == RIGHT_DIR) {
                new_dir = LEFT_DIR;
            } else {
                new_dir = RIGHT_DIR;
            }
        }

        if (player_y > 0 &&
            shield_drawn &&
            new_y == (player_y - 1) &&
            sprite_x_overlap(new_vx, player_vx)) {
            remove_enemy(i, 1);
            if (state != STATE_PLAY) return;
            continue;
        }

        if (new_y >= player_y) {
            hit_player = 0;

            if (sprite_x_overlap(new_vx, player_vx)) {
                hit_player = 1;
            }

            if (hit_player) {
                if (shield_drawn) {
                    remove_enemy(i, 1);
                    if (state != STATE_PLAY) return;
                } else {
                    player_hit();
                    if (state != STATE_PLAY) return;
                }
            } else {
                remove_enemy(i, 0);
            }

            continue;
        }

        if (new_y >= YSize) {
            remove_enemy(i, 0);
            continue;
        }

        e->dir = new_dir;
        e->timer = new_timer;
        e->vx = new_vx;
        e->y = new_y;

        moved = (new_vx != old_vx) || (new_y != old_y);

        if (moved) {
            sprite_delete(ENEMY_TYPE, old_vx, old_y);
            sprite_draw(ENEMY_TYPE, new_vx, new_y);
        }
    }
}

static void update_player_bullets(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t hit;
    uint8_t old_x;
    uint8_t old_y;
    uint8_t new_x;
    uint8_t new_y;
    struct bullet *b;

    for (i = 0; i < MAX_BULLETS; i++) {
        b = &player_bullets[i];

        if (!b->active) continue;

        old_x = b->x;
        old_y = b->y;
        new_x = old_x;

        if (old_y == 0) {
            remove_player_bullet(i);
            continue;
        }

        new_y = old_y - 1;

        if (new_y < TOP_ROW) {
            remove_player_bullet(i);
            continue;
        }

        hit = 0;

        for (j = 0; j < MAX_BULLETS; j++) {
            if (enemy_bullets[j].active &&
                enemy_bullets[j].x == new_x &&
                enemy_bullets[j].y == new_y) {
                remove_enemy_bullet(j);
                remove_player_bullet(i);
                add_score(5);
                _XL_TOCK_SOUND();
                hit = 1;
                break;
            }
        }

        if (hit) continue;

        for (j = 0; j < MAX_ENEMIES; j++) {
            if (enemies[j].active && enemy_at_index(j, new_x, new_y)) {
                remove_enemy(j, 1);
                remove_player_bullet(i);
                hit = 1;
                break;
            }
        }

        if (hit) continue;

        bullet_delete(old_x, old_y);

        b->x = new_x;
        b->y = new_y;

        bullet_draw(new_x, new_y, _TILE_8, _XL_YELLOW);
    }
}

static void update_enemy_bullets(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t hit;
    uint8_t old_x;
    uint8_t old_y;
    uint8_t new_x;

    uint8_t new_y;
    uint8_t plx;
    uint8_t prx;
    uint8_t slx;
    uint8_t srx;
    struct bullet *b;

    for (i = 0; i < MAX_BULLETS; i++) {
        b = &enemy_bullets[i];

        if (!b->active) continue;

        old_x = b->x;
        old_y = b->y;
        new_x = old_x;
        new_y = old_y + 1;

        if (new_y >= YSize) {
            remove_enemy_bullet(i);
            continue;
        }

        plx = sprite_left(player_vx);
        prx = sprite_right(player_vx);
        slx = plx;
        srx = prx;

        if (shield_drawn &&
            new_y == (player_y - 1) &&
            (new_x == slx || new_x == srx)) {
            remove_enemy_bullet(i);
            _XL_TOCK_SOUND();
            continue;
        }

        if (player_active &&
            new_y == player_y &&
            (new_x == plx || new_x == prx)) {
            player_hit();
            if (state != STATE_PLAY) return;
            continue;
        }

        hit = 0;

        for (j = 0; j < MAX_BULLETS; j++) {
            if (player_bullets[j].active &&
                player_bullets[j].x == new_x &&
                player_bullets[j].y == new_y) {
                remove_enemy_bullet(i);
                remove_player_bullet(j);
                add_score(5);
                _XL_TOCK_SOUND();
                hit = 1;
                break;
            }
        }

        if (hit) continue;

        if (enemy_at(new_x, new_y)) {
            remove_enemy_bullet(i);
            continue;
        }

        bullet_delete(old_x, old_y);

        b->x = new_x;
        b->y = new_y;

        bullet_draw(new_x, new_y, _TILE_9, _XL_MAGENTA);
    }
}

static void update_enemy_firing(void)
{
    uint8_t i;
    uint8_t x;
    uint8_t y;
    uint8_t delay;
    struct enemy *e;

    for (i = 0; i < MAX_ENEMIES; i++) {
        e = &enemies[i];

        if (!e->active) continue;

        if (e->fire_timer > 0) {
            e->fire_timer = e->fire_timer - 1;
        }

        if (e->fire_timer == 0) {
            x = bullet_x_from_virtual(e->vx);
            y = (uint8_t)(e->y + 1);
            delay = 10;

            if (y < YSize &&
                !enemy_at(x, y) &&
                !player_bullet_at(x, y) &&
                !enemy_bullet_at(x, y) &&
                !shield_at(x, y)) {
                create_enemy_bullet(x, y);
                delay = (uint8_t)(60 + (_XL_RAND() % 60));
            }

            e->fire_timer = delay;
        }
    }
}

static void reset_game(void)
{
    uint8_t i;

    state = STATE_PLAY;

    score = 0;
    last_score = 0xFFFF;
    last_lives = 0xFF;
    lives = 3;

    player_active = 1;
    player_vx = center_virtual_x();
    player_y = (uint8_t)(YSize - 1);

    shield_frames = 0;
    shield_cooldown = 0;
    shield_drawn = 0;

    fire_cooldown = 0;
    spawn_timer = SPAWN_MIN;

    enemy_count = 0;
    player_bullet_count = 0;
    enemy_bullet_count = 0;

    prev_down = 0;

    for (i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = 0;
    }

    for (i = 0; i < MAX_BULLETS; i++) {
        player_bullets[i].active = 0;
        enemy_bullets[i].active = 0;
    }

    sprite_draw(PLAYER_TYPE, player_vx, player_y);
    update_hud();
}

int main(void)
{
    uint8_t input;
    uint8_t down;
    uint16_t maxv;
    uint16_t old_vx;
    uint16_t new_vx;
    uint8_t px;
    uint8_t py;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    _XL_CLEAR_SCREEN();
    reset_game();

    while (1) {
        if (state == STATE_GAME_OVER) {
            _XL_WAIT_FOR_INPUT();
            _XL_CLEAR_SCREEN();
            reset_game();
            continue;
        }

        input = _XL_INPUT();
        down = _XL_DOWN(input);

        if (shield_cooldown > 0) {
            shield_cooldown = shield_cooldown - 1;
        }

        if (shield_drawn) {
            if (shield_frames > 0) {
                shield_frames = shield_frames - 1;
            }

            if (shield_frames == 0) {
                shield_delete(player_vx);
                shield_drawn = 0;
                shield_cooldown = SHIELD_COOLDOWN;
                _XL_ZAP_SOUND();
            }
        } else if (down && !prev_down && shield_cooldown == 0) {
            shield_frames = SHIELD_DURATION;
            shield_drawn = 1;
            shield_draw(player_vx);
            _XL_ZAP_SOUND();
        }

        prev_down = down;

        if (_XL_LEFT(input)) {
            if (player_vx > 0) {
                old_vx = player_vx;
                new_vx = old_vx - 1;

                if (!player_move_blocked(new_vx)) {
                    if (shield_drawn) {
                        shield_update(old_vx, new_vx);
                    }

                    sprite_delete(PLAYER_TYPE, old_vx, player_y);
                    player_vx = new_vx;
                    sprite_draw(PLAYER_TYPE, player_vx, player_y);
                }
            }
        } else if (_XL_RIGHT(input)) {
            maxv = max_virtual_x();
            old_vx = player_vx;

            if (old_vx < maxv) {
                new_vx = old_vx + 1;

                if (!player_move_blocked(new_vx)) {
                    if (shield_drawn) {
                        shield_update(old_vx, new_vx);
                    }

                    sprite_delete(PLAYER_TYPE, old_vx, player_y);
                    player_vx = new_vx;
                    sprite_draw(PLAYER_TYPE, player_vx, player_y);
                }
            }
        }

        if (fire_cooldown > 0) {
            fire_cooldown = fire_cooldown - 1;
        }

        if (_XL_FIRE(input) && fire_cooldown == 0 && !shield_drawn) {
            px = bullet_x_from_virtual(player_vx);
            py = (uint8_t)(player_y - 1);

            if (py >= 1 &&
                !enemy_at(px, py) &&
                !enemy_bullet_at(px, py) &&
                !player_bullet_at(px, py)) {
                create_player_bullet(px, py);
                fire_cooldown = FIRE_COOLDOWN;
                _XL_SHOOT_SOUND();
            }
        }

        if (spawn_timer > 0) {
            spawn_timer = spawn_timer - 1;
        }

        if (spawn_timer == 0) {
            spawn_enemy();
            spawn_timer = (uint8_t)(SPAWN_MIN + (_XL_RAND() % SPAWN_VAR));
        }

        update_player_bullets();

        if (state == STATE_PLAY) {
            update_enemies();
        }

        if (state == STATE_PLAY) {
            update_enemy_firing();
        }

        if (state == STATE_PLAY) {
            update_enemy_bullets();
        }

        update_hud();

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}

