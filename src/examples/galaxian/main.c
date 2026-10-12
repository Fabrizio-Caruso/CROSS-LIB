#include "cross_lib.h"

#define MAX_ENEMIES 3
#define MAX_BULLETS 16

#define PLAYER_TYPE 0
#define ENEMY_TYPE 1

#define LEFT_DIR 0
#define RIGHT_DIR 1

#define STATE_PLAY 0
#define STATE_GAME_OVER 1

#define SHIELD_DURATION 30
#define SHIELD_COOLDOWN 60
#define FIRE_COOLDOWN 3
#define ENEMY_FIRE_MIN 30
#define ENEMY_FIRE_VAR 30
#define DESCEND_INTERVAL 6

#define ENEMY_START_ROW 2

struct bullet {
    uint8_t active;
    uint8_t x;
    uint8_t y;
};

struct enemy {
    uint8_t active;
    uint8_t y;
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
static uint8_t descend_timer;

static uint16_t enemy_vx;
static uint8_t enemy_dir;
static uint8_t player_bullet_count;
static uint8_t enemy_bullet_count;
static uint8_t prev_down;

static void init_enemies(void);

static uint8_t in_bounds(uint8_t x, uint8_t y)
{
    if (x < XSize && y < YSize) return 1;
    return 0;
}

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
    return (uint8_t)((vx + 1) / 2);
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
    return _TILE_0;
}

static uint8_t sprite_color(uint8_t type)
{
    if (type == PLAYER_TYPE) return _XL_GREEN;
    return _XL_RED;
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
    if (in_bounds(x, y)) {
        _XL_DRAW(x, y, tile, color);
    }
}

static void bullet_delete(uint8_t x, uint8_t y)
{
    if (in_bounds(x, y)) {
        _XL_DELETE(x, y);
    }
}

static uint8_t message_y(void)
{
    return (uint8_t)(YSize / 2);
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
    if (ns < score) ns = 0xFFFF;
    score = ns;
    update_hud();
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
    player_bullets[i].x = 0;
    player_bullets[i].y = 0;
    if (player_bullet_count > 0) player_bullet_count--;
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
    enemy_bullets[i].x = 0;
    enemy_bullets[i].y = 0;
    if (enemy_bullet_count > 0) enemy_bullet_count--;
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

    lx = sprite_left(enemy_vx);
    rx = sprite_right(enemy_vx);

    if (x == lx || x == rx) return 1;
    return 0;
}

static uint8_t enemy_at(uint8_t x, uint8_t y)
{
    uint8_t i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_at_index(i, x, y)) return 1;
    }
    return 0;
}

static void kill_enemy(uint8_t i)
{
    if (!enemies[i].active) return;
    sprite_delete(ENEMY_TYPE, enemy_vx, enemies[i].y);
    enemies[i].active = 0;
}

static uint8_t kill_enemy_at(uint8_t x, uint8_t y)
{
    uint8_t i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_at_index(i, x, y)) {
            kill_enemy(i);
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

    if (x == lx || x == rx) return 1;
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

    if (x == lx || x == rx) return 1;
    return 0;
}

static void shield_draw_cells(uint16_t vx)
{
    uint8_t y;
    uint8_t lx;
    uint8_t rx;

    if (player_y < 1) return;

    y = player_y - 1;
    lx = sprite_left(vx);
    rx = sprite_right(vx);

    if (in_bounds(lx, y)) {
        remove_enemy_bullet_at(lx, y);
        remove_player_bullet_at(lx, y);
        kill_enemy_at(lx, y);
        _XL_DRAW(lx, y, _TILE_10, _XL_CYAN);
    }
    if (rx != lx && in_bounds(rx, y)) {
        remove_enemy_bullet_at(rx, y);
        remove_player_bullet_at(rx, y);
        kill_enemy_at(rx, y);
        _XL_DRAW(rx, y, _TILE_10, _XL_CYAN);
    }
}

static void shield_delete_cells(uint16_t vx)
{
    uint8_t y;
    uint8_t lx;
    uint8_t rx;

    if (player_y < 1) return;

    y = player_y - 1;
    lx = sprite_left(vx);
    rx = sprite_right(vx);

    if (in_bounds(lx, y)) _XL_DELETE(lx, y);
    if (rx != lx && in_bounds(rx, y)) _XL_DELETE(rx, y);
}

static void shield_clear_cells(uint16_t vx)
{
    uint8_t y;
    uint8_t lx;
    uint8_t rx;

    if (player_y < 1) return;

    y = player_y - 1;
    lx = sprite_left(vx);
    rx = sprite_right(vx);

    if (in_bounds(lx, y)) {
        remove_enemy_bullet_at(lx, y);
        remove_player_bullet_at(lx, y);
        kill_enemy_at(lx, y);
    }
    if (rx != lx && in_bounds(rx, y)) {
        remove_enemy_bullet_at(rx, y);
        remove_player_bullet_at(rx, y);
        kill_enemy_at(rx, y);
    }
}

static void shield_update(uint16_t old_vx, uint16_t new_vx)
{
    uint8_t y;
    uint8_t old_l;
    uint8_t old_r;
    uint8_t new_l;
    uint8_t new_r;
    uint8_t cell;

    if (!shield_drawn) return;
    if (player_y < 1) return;

    y = player_y - 1;
    old_l = sprite_left(old_vx);
    old_r = sprite_right(old_vx);
    new_l = sprite_left(new_vx);
    new_r = sprite_right(new_vx);

    cell = old_l;
    if (cell != new_l && cell != new_r && in_bounds(cell, y)) {
        _XL_DELETE(cell, y);
        remove_enemy_bullet_at(cell, y);
        remove_player_bullet_at(cell, y);
        kill_enemy_at(cell, y);
    }

    cell = old_r;
    if (cell != new_l && cell != new_r && cell != old_l && in_bounds(cell, y)) {
        _XL_DELETE(cell, y);
        remove_enemy_bullet_at(cell, y);
        remove_player_bullet_at(cell, y);
        kill_enemy_at(cell, y);
    }

    cell = new_l;
    if (cell != old_l && cell != old_r && in_bounds(cell, y)) {
        remove_enemy_bullet_at(cell, y);
        remove_player_bullet_at(cell, y);
        kill_enemy_at(cell, y);
        _XL_DRAW(cell, y, _TILE_10, _XL_CYAN);
    }

    cell = new_r;
    if (cell != old_l && cell != old_r && cell != new_l && in_bounds(cell, y)) {
        remove_enemy_bullet_at(cell, y);
        remove_player_bullet_at(cell, y);
        kill_enemy_at(cell, y);
        _XL_DRAW(cell, y, _TILE_10, _XL_CYAN);
    }
}

static uint8_t player_move_blocked(uint16_t vx)
{
    uint8_t i;
    uint8_t lx;
    uint8_t rx;
    uint8_t elx;
    uint8_t erx;
    uint8_t plx;
    uint8_t prx;

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active && enemies[i].y == player_y) {
            elx = sprite_left(enemy_vx);
            erx = sprite_right(enemy_vx);
            plx = sprite_left(vx);
            prx = sprite_right(vx);
            if (plx <= erx && elx <= prx) return 1;
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

static void create_player_bullet(uint8_t x, uint8_t y)
{
    uint8_t i;

    if (player_bullet_count >= MAX_BULLETS) return;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!player_bullets[i].active) {
            player_bullets[i].active = 1;
            player_bullets[i].x = x;
            player_bullets[i].y = y;
            player_bullet_count++;
            bullet_draw(x, y, _TILE_8, _XL_YELLOW);
            return;
        }
    }
}

static void create_enemy_bullet(uint8_t x, uint8_t y)
{
    uint8_t i;

    if (enemy_bullet_count >= MAX_BULLETS) return;
    if (!in_bounds(x, y)) return;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!enemy_bullets[i].active) {
            enemy_bullets[i].active = 1;
            enemy_bullets[i].x = x;
            enemy_bullets[i].y = y;
            enemy_bullet_count++;
            bullet_draw(x, y, _TILE_9, _XL_MAGENTA);
            return;
        }
    }
}

static void init_enemies(void)
{
    uint8_t i;

    enemy_vx = center_virtual_x();
    enemy_dir = RIGHT_DIR;
    descend_timer = 0;

    for (i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = 1;
        enemies[i].y = (uint8_t)(ENEMY_START_ROW + i * 2);
        enemies[i].fire_timer = (uint8_t)(ENEMY_FIRE_MIN + i * 10 + (_XL_RAND() % ENEMY_FIRE_VAR));
        sprite_draw(ENEMY_TYPE, enemy_vx, enemies[i].y);
    }
}

static void player_hit(void)
{
    uint8_t i;

    if (state != STATE_PLAY) return;

    if (lives > 0) lives--;
    update_hud();
    _XL_EXPLOSION_SOUND();

    if (shield_drawn) {
        shield_delete_cells(player_vx);
        shield_clear_cells(player_vx);
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
            sprite_delete(ENEMY_TYPE, enemy_vx, enemies[i].y);
            enemies[i].active = 0;
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

        init_enemies();
    }
}

static void update_enemies(void)
{
    uint8_t i;
    uint8_t old_y;
    uint8_t new_y;
    uint16_t maxv;
    uint16_t old_vx;
    uint16_t new_vx;

    maxv = max_virtual_x();
    old_vx = enemy_vx;

    if (enemy_dir == RIGHT_DIR) {
        if (enemy_vx < maxv) {
            enemy_vx = enemy_vx + 1;
        } else {
            enemy_dir = LEFT_DIR;
        }
    } else {
        if (enemy_vx > 0) {
            enemy_vx = enemy_vx - 1;
        } else {
            enemy_dir = RIGHT_DIR;
        }
    }

    new_vx = enemy_vx;

    descend_timer++;
    if (descend_timer >= DESCEND_INTERVAL) {
        descend_timer = 0;
    }

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;

        old_y = enemies[i].y;
        new_y = old_y;

        if (descend_timer == 0) {
            new_y = old_y + 1;
        }

        if (new_y >= player_y) {
            new_y = ENEMY_START_ROW;
        }

        enemies[i].y = new_y;

        sprite_delete(ENEMY_TYPE, old_vx, old_y);
        sprite_draw(ENEMY_TYPE, new_vx, new_y);
    }
}

static void update_enemy_firing(void)
{
    uint8_t i;
    uint8_t x;
    uint8_t y;
    uint8_t delay;

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;

        if (enemies[i].fire_timer > 0) {
            enemies[i].fire_timer--;
        }

        if (enemies[i].fire_timer == 0) {
            x = bullet_x_from_virtual(enemy_vx);
            y = (uint8_t)(enemies[i].y + 1);
            delay = 10;

            if (in_bounds(x, y) &&
                !player_at(x, y) &&
                !shield_at(x, y) &&
                !enemy_at(x, y) &&
                !player_bullet_at(x, y) &&
                !enemy_bullet_at(x, y)) {
                create_enemy_bullet(x, y);
                delay = (uint8_t)(ENEMY_FIRE_MIN + (_XL_RAND() % ENEMY_FIRE_VAR));
            }

            enemies[i].fire_timer = delay;
        }
    }
}

static void update_player_bullets(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t old_x;
    uint8_t old_y;
    uint8_t new_x;
    uint8_t new_y;
    uint8_t hit;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!player_bullets[i].active) continue;

        old_x = player_bullets[i].x;
        old_y = player_bullets[i].y;

        if (old_y == 0) {
            remove_player_bullet(i);
            continue;
        }

        new_x = old_x;
        new_y = old_y - 1;

        if (new_y < 1) {
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
            if (enemies[j].active &&
                (enemy_at_index(j, new_x, new_y) ||
                 enemy_at_index(j, new_x, old_y))) {
                kill_enemy(j);
                remove_player_bullet(i);
                add_score(10);
                _XL_PING_SOUND();

                /* respawn at top */
                enemies[j].active = 1;
                enemies[j].y = ENEMY_START_ROW;
                enemies[j].fire_timer = (uint8_t)(ENEMY_FIRE_MIN + (_XL_RAND() % ENEMY_FIRE_VAR));
                sprite_draw(ENEMY_TYPE, enemy_vx, ENEMY_START_ROW);

                hit = 1;
                break;
            }
        }

        if (hit) continue;

        bullet_delete(old_x, old_y);

        player_bullets[i].x = new_x;
        player_bullets[i].y = new_y;

        bullet_draw(new_x, new_y, _TILE_8, _XL_YELLOW);
    }
}

static void update_enemy_bullets(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t old_x;
    uint8_t old_y;
    uint8_t new_x;
    uint8_t new_y;
    uint8_t hit;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!enemy_bullets[i].active) continue;

        old_x = enemy_bullets[i].x;
        old_y = enemy_bullets[i].y;

        new_x = old_x;
        new_y = old_y + 1;

        if (new_y >= YSize) {
            remove_enemy_bullet(i);
            continue;
        }

        if (shield_at(new_x, new_y)) {
            remove_enemy_bullet(i);
            _XL_TOCK_SOUND();
            continue;
        }

        if (player_active && player_at(new_x, new_y)) {
            remove_enemy_bullet(i);
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

        enemy_bullets[i].x = new_x;
        enemy_bullets[i].y = new_y;

        bullet_draw(new_x, new_y, _TILE_9, _XL_MAGENTA);
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

    fire_cooldown = 5;
    descend_timer = 0;

    player_bullet_count = 0;
    enemy_bullet_count = 0;

    prev_down = 0;

    for (i = 0; i < MAX_BULLETS; i++) {
        player_bullets[i].active = 0;
        player_bullets[i].x = 0;
        player_bullets[i].y = 0;
        enemy_bullets[i].active = 0;
        enemy_bullets[i].x = 0;
        enemy_bullets[i].y = 0;
    }

    for (i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = 0;
    }

    sprite_draw(PLAYER_TYPE, player_vx, player_y);
    init_enemies();
    update_hud();
}

int main(void)
{
    uint8_t input;
    uint8_t down;
    uint8_t px;
    uint8_t py;
    uint16_t maxv;
    uint16_t old_vx;
    uint16_t new_vx;

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
            shield_cooldown--;
        }

        if (shield_drawn) {
            if (shield_frames > 0) {
                shield_frames--;
            }
            if (shield_frames == 0) {
                shield_delete_cells(player_vx);
                shield_clear_cells(player_vx);
                shield_drawn = 0;
                shield_cooldown = SHIELD_COOLDOWN;
                _XL_ZAP_SOUND();
            }
        } else if (down && !prev_down && shield_cooldown == 0) {
            shield_frames = SHIELD_DURATION;
            shield_drawn = 1;
            shield_draw_cells(player_vx);
            _XL_ZAP_SOUND();
        }

        prev_down = down;

        if (_XL_LEFT(input)) {
            old_vx = player_vx;
            if (old_vx > 0) {
                new_vx = old_vx - 1;
                if (!player_move_blocked(new_vx)) {
                    if (shield_drawn) shield_update(old_vx, new_vx);
                    sprite_delete(PLAYER_TYPE, old_vx, player_y);
                    player_vx = new_vx;
                    sprite_draw(PLAYER_TYPE, player_vx, player_y);
                }
            }
        } else if (_XL_RIGHT(input)) {
            old_vx = player_vx;
            maxv = max_virtual_x();
            if (old_vx < maxv) {
                new_vx = old_vx + 1;
                if (!player_move_blocked(new_vx)) {
                    if (shield_drawn) shield_update(old_vx, new_vx);
                    sprite_delete(PLAYER_TYPE, old_vx, player_y);
                    player_vx = new_vx;
                    sprite_draw(PLAYER_TYPE, player_vx, player_y);
                }
            }
        }

        if (fire_cooldown > 0) {
            fire_cooldown--;
        }

        if (_XL_FIRE(input) &&
            fire_cooldown == 0 &&
            !shield_drawn &&
            player_y > 1) {
            px = bullet_x_from_virtual(player_vx);
            py = player_y - 1;

            if (py >= 1 &&
                !enemy_at(px, py) &&
                !enemy_bullet_at(px, py) &&
                !player_bullet_at(px, py)) {
                create_player_bullet(px, py);
                fire_cooldown = FIRE_COOLDOWN;
                _XL_SHOOT_SOUND();
            }
        }

        update_enemies();

        if (state == STATE_PLAY) {
            update_enemy_firing();
        }

        if (state == STATE_PLAY) {
            update_player_bullets();
        }

        if (state == STATE_PLAY) {
            update_enemy_bullets();
        }

        update_hud();

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}