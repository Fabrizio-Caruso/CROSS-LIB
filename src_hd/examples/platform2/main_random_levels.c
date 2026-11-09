#include "cross_lib.h"

#define MAX_PLATFORMS 8
#define MAX_ENEMIES 6
#define MAX_ITEMS 14
#define MAX_ELEVATORS 7
#define MAX_BULLETS 3
#define DIAMOND_COUNT 8

typedef struct { uint8_t x, y; } Pos;
typedef struct { uint8_t x1, x2, y; } Platform;
typedef struct { uint8_t x, y, dir, alive; } Enemy;
typedef struct { uint8_t x, y, type, collected; } Item;
typedef struct { uint8_t x, yl, yu; } Elevator;
typedef struct { uint8_t x, y, vx, active; } Bullet;

static Platform platforms[MAX_PLATFORMS];
static Enemy enemies[MAX_ENEMIES];
static Item items[MAX_ITEMS];
static Elevator elevators[MAX_ELEVATORS];
static Bullet bullets[MAX_BULLETS];
static uint8_t plat_count, enemy_count, item_count, elev_count;
static uint8_t player_x, player_y, facing;
static uint8_t diamonds_collected, gun_ammo, invincible_timer, freeze_timer;
static uint8_t door_x, door_y, door_open;
static uint16_t score;
static uint16_t level_num;
static uint16_t rng_state;

static void seed_rng(uint16_t s) { rng_state = s + 7919; }
static uint16_t my_rand(void) {
    rng_state = (uint16_t)(rng_state * 3421 + 523);
    return rng_state;
}

static void safe_delete(uint8_t x, uint8_t y) {
    uint8_t i;
    _XL_DELETE(x, y);
    for (i = 0; i < elev_count; i++) {
        if (x == elevators[i].x && y > elevators[i].yl && y < elevators[i].yu) {
            _XL_DRAW(x, y, _TILE_5, _XL_CYAN);
            return;
        }
    }
    for (i = 0; i < item_count; i++) {
        if (!items[i].collected && items[i].x == x && items[i].y == y) {
            uint8_t t;
            switch (items[i].type) {
                case 0: t = _TILE_6; break;
                case 1: t = _TILE_7; break;
                case 2: t = _TILE_8; break;
                default: t = _TILE_9; break;
            }
            _XL_DRAW(x, y, t, _XL_YELLOW);
            return;
        }
    }
}

static void draw_platform(Platform *p) {
    uint8_t x;
    for (x = p->x1; x <= p->x2; x++) _XL_DRAW(x, p->y, _TILE_4, _XL_WHITE);
}

static void erase_platform(Platform *p) {
    uint8_t x;
    for (x = p->x1; x <= p->x2; x++) _XL_DELETE(x, p->y);
}

static void draw_elevators(void) {
    uint8_t i, y;
    for (i = 0; i < elev_count; i++) {
        for (y = (uint8_t)(elevators[i].yl + 1); y < elevators[i].yu; y++) {
            _XL_DRAW(elevators[i].x, y, _TILE_5, _XL_CYAN);
        }
    }
}

static void erase_elevators(void) {
    uint8_t i, y;
    for (i = 0; i < elev_count; i++) {
        for (y = (uint8_t)(elevators[i].yl + 1); y < elevators[i].yu; y++) {
            _XL_DELETE(elevators[i].x, y);
        }
    }
}

static void draw_items(void) {
    uint8_t i;
    for (i = 0; i < item_count; i++) {
        if (!items[i].collected) {
            uint8_t t;
            switch (items[i].type) {
                case 0: t = _TILE_6; break;
                case 1: t = _TILE_7; break;
                case 2: t = _TILE_8; break;
                default: t = _TILE_9; break;
            }
            _XL_DRAW(items[i].x, items[i].y, t, _XL_YELLOW);
        }
    }
}

static void draw_door(void) {
    uint8_t t;
    if (door_open) t = _TILE_11; else t = _TILE_10;
    _XL_DRAW(door_x, (uint8_t)(door_y - 2), t, _XL_MAGENTA);
    _XL_DRAW(door_x, (uint8_t)(door_y - 1), t, _XL_MAGENTA);
}

static void draw_player(void) {
    uint8_t c;
    if (invincible_timer > 0) c = _XL_YELLOW; else c = _XL_GREEN;
    _XL_DRAW(player_x, (uint8_t)(player_y - 2), _TILE_0, c);
    _XL_DRAW(player_x, (uint8_t)(player_y - 1), _TILE_1, c);
}

static void erase_player(void) {
    safe_delete(player_x, (uint8_t)(player_y - 2));
    safe_delete(player_x, (uint8_t)(player_y - 1));
}

static void draw_enemies(void) {
    uint8_t i;
    for (i = 0; i < enemy_count; i++) {
        if (!enemies[i].alive) continue;
        uint8_t c;
        if (freeze_timer > 0) c = _XL_CYAN; else c = _XL_RED;
        _XL_DRAW(enemies[i].x, (uint8_t)(enemies[i].y - 2), _TILE_2, c);
        _XL_DRAW(enemies[i].x, (uint8_t)(enemies[i].y - 1), _TILE_3, c);
    }
}

static void erase_enemies(void) {
    uint8_t i;
    for (i = 0; i < enemy_count; i++) {
        if (!enemies[i].alive) continue;
        safe_delete(enemies[i].x, (uint8_t)(enemies[i].y - 2));
        safe_delete(enemies[i].x, (uint8_t)(enemies[i].y - 1));
    }
}

static void erase_bullets(void) {
    uint8_t i;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        safe_delete(bullets[i].x, bullets[i].y);
    }
}

static void clear_screen(void) {
    uint8_t i;
    for (i = 0; i < plat_count; i++) erase_platform(&platforms[i]);
    erase_elevators();
    erase_player();
    erase_enemies();
    erase_bullets();
    _XL_DELETE(door_x, (uint8_t)(door_y - 2));
    _XL_DELETE(door_x, (uint8_t)(door_y - 1));
}

static uint8_t on_platform(uint8_t x, uint8_t feet_y) {
    uint8_t i;
    for (i = 0; i < plat_count; i++) {
        if (feet_y == platforms[i].y && x >= platforms[i].x1 && x <= platforms[i].x2) return 1;
    }
    return 0;
}

static uint8_t bullet_hits_enemy(uint8_t bx, uint8_t by, uint8_t ei) {
    if (!enemies[ei].alive) return 0;
    if (bx == enemies[ei].x &&
        ((by == (uint8_t)(enemies[ei].y - 2)) ||
         (by == (uint8_t)(enemies[ei].y - 1)))) {
        return 1;
    }
    return 0;
}

static uint8_t enemies_overlap(uint8_t ea, uint8_t eb) {
    if (!enemies[ea].alive || !enemies[eb].alive) return 0;
    if (enemies[ea].y != enemies[eb].y) return 0;
    if (enemies[ea].x == enemies[eb].x) return 1;
    return 0;
}

static void kill_enemy(uint8_t i) {
    safe_delete(enemies[i].x, (uint8_t)(enemies[i].y - 2));
    safe_delete(enemies[i].x, (uint8_t)(enemies[i].y - 1));
    enemies[i].alive = 0;
}

static void move_bullets(void) {
    uint8_t i, j;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;

        for (j = 0; j < enemy_count; j++) {
            if (bullet_hits_enemy(bullets[i].x, bullets[i].y, j)) {
                kill_enemy(j);
                safe_delete(bullets[i].x, bullets[i].y);
                bullets[i].active = 0;
                score += 50;
                _XL_PING_SOUND();
                break;
            }
        }
        if (!bullets[i].active) continue;

        safe_delete(bullets[i].x, bullets[i].y);
        if (bullets[i].vx == 1) {
            bullets[i].x++;
            if (bullets[i].x >= XSize - 1) { bullets[i].active = 0; continue; }
        } else {
            if (bullets[i].x <= 1) { bullets[i].active = 0; continue; }
            bullets[i].x--;
        }
        _XL_DRAW(bullets[i].x, bullets[i].y, _TILE_12, _XL_WHITE);

        for (j = 0; j < enemy_count; j++) {
            if (bullet_hits_enemy(bullets[i].x, bullets[i].y, j)) {
                kill_enemy(j);
                safe_delete(bullets[i].x, bullets[i].y);
                bullets[i].active = 0;
                score += 50;
                _XL_PING_SOUND();
                break;
            }
        }
    }
}

static void move_enemies(void) {
    uint8_t i, j;
    if (freeze_timer > 0) return;
    for (i = 0; i < enemy_count; i++) {
        if (!enemies[i].alive) continue;

        for (j = 0; j < enemy_count; j++) {
            if (j == i) continue;
            if (enemies_overlap(i, j)) {
                enemies[i].dir ^= 1;
                break;
            }
        }

        safe_delete(enemies[i].x, (uint8_t)(enemies[i].y - 2));
        safe_delete(enemies[i].x, (uint8_t)(enemies[i].y - 1));

        {
            uint8_t nx = enemies[i].x;
            if (enemies[i].dir == 0) {
                if (nx > 0) nx--; else enemies[i].dir = 1;
            } else {
                if ((uint8_t)(nx + 1) < XSize - 1) nx++; else enemies[i].dir = 0;
            }
            {
                uint8_t ok, pi;
                ok = 0;
                for (pi = 0; pi < plat_count; pi++) {
                    if (enemies[i].y == platforms[pi].y && nx >= platforms[pi].x1 && nx <= platforms[pi].x2) { ok = 1; break; }
                }
                if (ok) {
                    uint8_t conflict = 0;
                    for (j = 0; j < enemy_count && !conflict; j++) {
                        if (j == i || !enemies[j].alive) continue;
                        if (nx == enemies[j].x && enemies[i].y == enemies[j].y) conflict = 1;
                    }
                    if (conflict) enemies[i].dir ^= 1;
                }
                if (!ok) enemies[i].dir ^= 1;
                else enemies[i].x = nx;
            }
        }

        _XL_DRAW(enemies[i].x, (uint8_t)(enemies[i].y - 2), _TILE_2, _XL_RED);
        _XL_DRAW(enemies[i].x, (uint8_t)(enemies[i].y - 1), _TILE_3, _XL_RED);

        for (j = i + 1; j < enemy_count; j++) {
            if (!enemies[j].alive) continue;
            if (enemies_overlap(i, j)) {
                enemies[i].dir ^= 1;
                break;
            }
        }
    }
}

static uint8_t check_collision(void) {
    uint8_t i;
    for (i = 0; i < enemy_count; i++) {
        if (!enemies[i].alive) continue;
        if (player_x == enemies[i].x &&
            ((player_y - 1 == (uint8_t)(enemies[i].y - 2)) ||
             (player_y - 1 == (uint8_t)(enemies[i].y - 1)) ||
             (player_y == (uint8_t)(enemies[i].y - 2)) ||
             (player_y == (uint8_t)(enemies[i].y - 1)))) {
            return 1;
        }
    }
    return 0;
}

static void check_items(void) {
    uint8_t i;
    for (i = 0; i < item_count; i++) {
        if (items[i].collected) continue;
        if (player_x == items[i].x && player_y - 1 == items[i].y) {
            _XL_DELETE(items[i].x, items[i].y);
            items[i].collected = 1;
            switch (items[i].type) {
                case 0: diamonds_collected++; score += 25; _XL_PING_SOUND(); break;
                case 1: gun_ammo = 3; _XL_TICK_SOUND(); break;
                case 2: invincible_timer = 60; _XL_TOCK_SOUND(); break;
                default: freeze_timer = 90; _XL_TOCK_SOUND(); break;
            }
        }
    }
}

static void game_over_screen(void) {
    clear_screen();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(2, (uint8_t)(YSize / 4), "GAME OVER");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(2, (uint8_t)(YSize / 4 + 3), 1, score);
    _XL_SLEEP(2);
}

static void win_screen(void) {
    clear_screen();
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(2, (uint8_t)(YSize / 4), "YOU WIN!");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(2, (uint8_t)(YSize / 4 + 3), 1, score);
    _XL_SLEEP(2);
}

static void generate_level(uint16_t lvl) {
    uint8_t i;
    uint8_t num_plats;
    uint8_t item_idx;
    uint8_t guns, invs, frzs;
    short sx;

    seed_rng((uint16_t)(lvl * 7919 + 31));
    plat_count = 0;
    enemy_count = 0;
    item_count = 0;
    elev_count = 0;
    diamonds_collected = 0;
    gun_ammo = 0;
    invincible_timer = 0;
    freeze_timer = 0;
    door_open = 0;
    score = (uint16_t)(lvl * 100);

    for (i = 0; i < MAX_BULLETS; i++) bullets[i].active = 0;

    num_plats = (uint8_t)(4 + my_rand() % 3);
    if (num_plats > MAX_PLATFORMS - 1) num_plats = (uint8_t)(MAX_PLATFORMS - 1);

    platforms[plat_count].x1 = 1;
    platforms[plat_count].x2 = (uint8_t)(XSize - 2);
    platforms[plat_count].y = (uint8_t)(YSize - 2);
    plat_count++;

    for (i = 1; i < num_plats; i++) {
        uint8_t w, x1, y, prev_x1, prev_x2, prev_y;

        prev_x1 = platforms[i - 1].x1;
        prev_x2 = platforms[i - 1].x2;
        prev_y = platforms[i - 1].y;
        sx = (short)prev_x2 - (short)prev_x1 + 1;

        w = (uint8_t)(sx / 3 + my_rand() % (sx / 2));
        if (w < 6) w = 6;
        if ((short)w > sx - 1) w = (uint8_t)sx - 1;

        x1 = (uint8_t)(prev_x1 + my_rand() % ((uint16_t)(sx - w)));
        y = (uint8_t)((YSize - 2) - i * ((YSize - 6) / num_plats));
        if (y < 4) y = 4;

        platforms[plat_count].x1 = x1;
        platforms[plat_count].x2 = (uint8_t)(x1 + w);
        platforms[plat_count].y = y;
        plat_count++;

        {
            uint8_t ov_start, ov_end;
            ov_start = prev_x1;
            if (platforms[i].x1 > ov_start) ov_start = platforms[i].x1;
            ov_end = prev_x2;
            if (platforms[i].x2 < ov_end) ov_end = platforms[i].x2;
            if (ov_start <= ov_end && elev_count < MAX_ELEVATORS) {
                sx = (short)ov_end - (short)ov_start + 1;
                {
                    uint8_t ex;
                    ex = (uint8_t)(ov_start + my_rand() % ((uint16_t)sx));
                    elevators[elev_count].x = ex;
                    elevators[elev_count].yl = platforms[i].y;
                    elevators[elev_count].yu = prev_y;
                    elev_count++;
                }
            }
        }
    }

    /* --- Items --- */
    item_idx = 0;

    /* Diamonds: random platforms above bottom */
    for (i = 0; i < DIAMOND_COUNT && item_idx < MAX_ITEMS; i++) {
        uint8_t pi, px;
        pi = (uint8_t)(1 + my_rand() % (plat_count - 1));
        sx = (short)platforms[pi].x2 - (short)platforms[pi].x1;
        px = (uint8_t)(platforms[pi].x1 + my_rand() % ((uint16_t)sx + 1));
        items[item_idx].x = px;
        items[item_idx].y = (uint8_t)(platforms[pi].y - 1);
        items[item_idx].type = 0;
        items[item_idx].collected = 0;
        item_idx++;
    }

    /* Guns: placed on the 1st or 2nd platform above bottom */
    guns = (uint8_t)(1 + my_rand() % 2);
    for (i = 0; i < guns && item_idx < MAX_ITEMS; i++) {
        uint8_t pi, px;
        pi = (uint8_t)(1 + my_rand() % 2); /* index 1 or 2 */
        sx = (short)platforms[pi].x2 - (short)platforms[pi].x1;
        px = (uint8_t)(platforms[pi].x1 + my_rand() % ((uint16_t)sx + 1));
        items[item_idx].x = px;
        items[item_idx].y = (uint8_t)(platforms[pi].y - 1);
        items[item_idx].type = 1;
        items[item_idx].collected = 0;
        item_idx++;
    }

    /* Invincibility */
    invs = (uint8_t)(1 + my_rand() % 2);
    for (i = 0; i < invs && item_idx < MAX_ITEMS; i++) {
        uint8_t pi, px;
        pi = (uint8_t)(1 + my_rand() % (plat_count - 1));
        sx = (short)platforms[pi].x2 - (short)platforms[pi].x1;
        px = (uint8_t)(platforms[pi].x1 + my_rand() % ((uint16_t)sx + 1));
        items[item_idx].x = px;
        items[item_idx].y = (uint8_t)(platforms[pi].y - 1);
        items[item_idx].type = 2;
        items[item_idx].collected = 0;
        item_idx++;
    }

    /* Freeze */
    frzs = (uint8_t)(1 + my_rand() % 2);
    for (i = 0; i < frzs && item_idx < MAX_ITEMS; i++) {
        uint8_t pi, px;
        pi = (uint8_t)(1 + my_rand() % (plat_count - 1));
        sx = (short)platforms[pi].x2 - (short)platforms[pi].x1;
        px = (uint8_t)(platforms[pi].x1 + my_rand() % ((uint16_t)sx + 1));
        items[item_idx].x = px;
        items[item_idx].y = (uint8_t)(platforms[pi].y - 1);
        items[item_idx].type = 3;
        items[item_idx].collected = 0;
        item_idx++;
    }

    item_count = item_idx;

    /* --- Enemies: every platform above bottom gets at least one enemy.
       Platforms longer than XSize/3 get a second enemy. --- */

    /* First pass: one enemy per platform above bottom */
    for (i = 1; i < num_plats && enemy_count < MAX_ENEMIES; i++) {
        uint8_t ex;
        sx = (short)platforms[i].x2 - (short)platforms[i].x1;
        ex = (uint8_t)(platforms[i].x1 + my_rand() % ((uint16_t)sx + 1));
        enemies[enemy_count].x = ex;
        enemies[enemy_count].y = platforms[i].y;
        if (my_rand() & 1) enemies[enemy_count].dir = 0; else enemies[enemy_count].dir = 1;
        enemies[enemy_count].alive = 1;
        enemy_count++;
    }

    /* Second pass: add second enemy to platforms longer than XSize/3 */
    for (i = 1; i < num_plats && enemy_count < MAX_ENEMIES; i++) {
        uint8_t plat_w, ex2;
        plat_w = (uint8_t)(platforms[i].x2 - platforms[i].x1 + 1);
        if ((short)plat_w > XSize / 3) {
            sx = (short)platforms[i].x2 - (short)platforms[i].x1;
            ex2 = (uint8_t)(platforms[i].x1 + my_rand() % ((uint16_t)sx + 1));
            enemies[enemy_count].x = ex2;
            enemies[enemy_count].y = platforms[i].y;
            if (my_rand() & 1) enemies[enemy_count].dir = 0; else enemies[enemy_count].dir = 1;
            enemies[enemy_count].alive = 1;
            enemy_count++;
        }
    }

    /* Door on top platform */
    {
        uint8_t top_pi;
        top_pi = (uint8_t)(num_plats - 1);
        door_x = platforms[top_pi].x2;
        door_y = platforms[top_pi].y;
    }

    player_x = (uint8_t)((platforms[0].x1 + platforms[0].x2) / 2);
    player_y = platforms[0].y;
    facing = 0;
}

static void draw_level(void) {
    uint8_t i;
    for (i = 0; i < plat_count; i++) draw_platform(&platforms[i]);
    draw_elevators();
    draw_items();
    draw_door();
    draw_player();
    draw_enemies();
}

static void play_level(void) {
    uint8_t input;
    uint8_t game_over;
    uint8_t level_complete;
    uint8_t i;
    short px;

    _XL_CLEAR_SCREEN();
    generate_level(level_num);
    draw_level();

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(1, 0, 1, (uint16_t)level_num);
    _XL_PRINTD((uint8_t)(XSize - 5), 0, 1, score);

    game_over = 0;
    level_complete = 0;

    while (!game_over && !level_complete) {
        input = _XL_INPUT();

        if (invincible_timer > 0) invincible_timer--;
        if (freeze_timer > 0) freeze_timer--;

        /* Check collision BEFORE enemies move */
        if ((invincible_timer == 0) && check_collision()) {
            game_over = 1;
        }

        move_bullets();
        move_enemies();

        /* Check collision AFTER enemies move */
        if (!game_over && (invincible_timer == 0) && check_collision()) {
            game_over = 1;
        }

        px = (short)player_x;
        if (_XL_LEFT(input)) {
            px--;
            facing = 1;
        } else if (_XL_RIGHT(input)) {
            px++;
            facing = 0;
        }
        if (px < 1) px = 1;
        if (px > (short)(XSize - 2)) px = XSize - 2;

        if ((uint8_t)px != player_x) {
            erase_player();
            player_x = (uint8_t)px;
            draw_player();
        }

        /* Check collision AFTER player moves */
        if (!game_over && (invincible_timer == 0) && check_collision()) {
            game_over = 1;
        }

        if (_XL_UP(input)) {
            for (i = 0; i < elev_count; i++) {
                if (player_x == elevators[i].x && player_y == elevators[i].yu) {
                    erase_player();
                    player_y = elevators[i].yl;
                    draw_player();
                    _XL_TICK_SOUND();
                    break;
                }
            }
        } else if (_XL_DOWN(input)) {
            for (i = 0; i < elev_count; i++) {
                if (player_x == elevators[i].x && player_y == elevators[i].yl) {
                    erase_player();
                    player_y = elevators[i].yu;
                    draw_player();
                    _XL_TICK_SOUND();
                    break;
                }
            }
        }

        /* Gravity: find the NEAREST lower platform below the player */
        if (!on_platform(player_x, player_y)) {
            uint8_t found = 0;
            uint8_t best_y = 0;
            for (i = 0; i < plat_count; i++) {
                if (player_x >= platforms[i].x1 && player_x <= platforms[i].x2) {
                    if (platforms[i].y > player_y) {
                        if (!found || platforms[i].y < best_y) {
                            best_y = platforms[i].y;
                            found = 1;
                        }
                    }
                }
            }
            if (found) {
                erase_player();
                while (player_y < best_y) player_y++;
                draw_player();
            }
        }

        check_items();

        if (_XL_FIRE(input) && gun_ammo > 0) {
            uint8_t fired = 0;
            for (i = 0; i < MAX_BULLETS && !fired; i++) {
                if (!bullets[i].active) {
                    bullets[i].x = player_x;
                    bullets[i].y = (uint8_t)(player_y - 2);
                    if (facing == 0) bullets[i].vx = 1; else bullets[i].vx = 0;
                    bullets[i].active = 1;
                    _XL_DRAW(bullets[i].x, bullets[i].y, _TILE_12, _XL_WHITE);
                    fired = 1;
                }
            }
            if (fired) {
                gun_ammo--;
                _XL_SHOOT_SOUND();
            }
        }

        if (!door_open && diamonds_collected >= DIAMOND_COUNT) {
            door_open = 1;
            safe_delete(door_x, (uint8_t)(door_y - 2));
            safe_delete(door_x, (uint8_t)(door_y - 1));
            _XL_DRAW(door_x, (uint8_t)(door_y - 2), _TILE_11, _XL_MAGENTA);
            _XL_DRAW(door_x, (uint8_t)(door_y - 1), _TILE_11, _XL_MAGENTA);
            _XL_PING_SOUND();
        }

        if (door_open && player_x == door_x && player_y == door_y) {
            level_complete = 1;
        }

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(1, 0, 1, (uint16_t)level_num);
        _XL_PRINTD((uint8_t)(XSize - 5), 0, 1, score);
        {
            uint8_t dx;
            dx = (uint8_t)(XSize / 2 - 4);
            _XL_PRINTD(dx, 0, 1, diamonds_collected);
        }

        if (game_over) {
            game_over_screen();
        } else if (level_complete) {
            level_num++;
            if (level_num > 30) {
                win_screen();
                level_num = 1;
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

int main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();
    level_num = 1;
    while (1) {
        play_level();
    }
    return 0;
}
