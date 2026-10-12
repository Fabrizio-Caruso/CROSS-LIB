#include "cross_lib.h"

/*
 * Galaxian-style vertical arcade shooter.
 *
 * Tile assignments (3x3 = 9 tiles per entity):
 *   Player:         _TILE_0  .. _TILE_8
 *   Enemy Type 0:   _TILE_9  .. _TILE_17  (Scout)
 *   Enemy Type 1:   _TILE_18 .. _TILE_26  (Fighter)
 *   Enemy Type 2:   _TILE_27 .. _TILE_35  (Boss)
 *   Player bullet:  _TILE_36
 *   Enemy bullet:   _TILE_37
 */

#define PLAYER_TILE_BASE  0
#define ENEMY0_TILE_BASE  9
#define ENEMY1_TILE_BASE  18
#define ENEMY2_TILE_BASE  27
#define P_BULLET_TILE     36
#define E_BULLET_TILE     37

#define MAX_ENEMIES   9
#define MAX_P_BULLETS 3
#define MAX_E_BULLETS 4
#define NUM_LIVES     3


static uint8_t tile_map[] = {
    _TILE_0,  _TILE_1,  _TILE_2,  _TILE_3,  _TILE_4,  _TILE_5,  _TILE_6,  _TILE_7,  _TILE_8,
    _TILE_9,  _TILE_10, _TILE_11, _TILE_12, _TILE_13, _TILE_14, _TILE_15, _TILE_16, _TILE_17,
    _TILE_18, _TILE_19, _TILE_20, _TILE_21, _TILE_22, _TILE_23, _TILE_24, _TILE_25, _TILE_26,
    _TILE_27, _TILE_28, _TILE_29, _TILE_30, _TILE_31, _TILE_32, _TILE_33, _TILE_34, _TILE_35,
    _TILE_36,
    _TILE_37
};

/* ---------- structures ---------- */

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t active;
} Bullet;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t type;    /* 0 = Scout, 1 = Fighter, 2 = Boss */
    uint8_t active;
    uint8_t dir;     /* 0 = left, 1 = right */
    uint8_t timer;   /* frame counter for periodic actions */
} Enemy;

/* ---------- helpers ---------- */

void draw_entity(uint8_t ex, uint8_t ey, uint8_t tile_base, uint8_t color)
{
    uint8_t r, c;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            _XL_DRAW(ex + c, ey + r, tile_map[tile_base + r * 3 + c], color);
        }
    }
}

void delete_entity(uint8_t ex, uint8_t ey)
{
    uint8_t r, c;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            _XL_DELETE(ex + c, ey + r);
        }
    }
}

uint8_t enemy_tile_base(uint8_t type)
{
    if (type == 0) return ENEMY0_TILE_BASE;
    if (type == 1) return ENEMY1_TILE_BASE;
    return ENEMY2_TILE_BASE;
}

uint8_t enemy_color(uint8_t type)
{
    if (type == 0) return _XL_RED;
    if (type == 1) return _XL_YELLOW;
    return _XL_GREEN;
}

uint8_t bullet_hits_enemy(uint8_t bx, uint8_t by, uint8_t ex, uint8_t ey)
{
    /* bullet at (bx,by) vs 3x3 entity at (ex,ey) */
    uint8_t rx, ry;
    if (bx < ex || bx > (uint8_t)(ex + 2)) return 0;
    if (by < ey || by > (uint8_t)(ey + 2)) return 0;
    rx = bx - ex;
    ry = by - ey;
    (void)rx;
    (void)ry;
    return 1;
}

uint8_t entities_overlap(uint8_t ax, uint8_t ay, uint8_t bx, uint8_t by)
{
    /* two 3x3 entities overlap? */
    if (ax > (uint8_t)(bx + 2)) return 0;
    if (bx > (uint8_t)(ax + 2)) return 0;
    if (ay > (uint8_t)(by + 2)) return 0;
    if (by > (uint8_t)(ay + 2)) return 0;
    return 1;
}

void spawn_wave(Enemy *enemies, uint8_t wave)
{
    uint8_t i, count, spacing, x;
    uint8_t types[9];

    /* number of enemies this wave (capped) */
    count = (uint8_t)(wave + 2);
    if (count > MAX_ENEMIES) count = MAX_ENEMIES;
    /* also cap so they fit horizontally */
    if (count > (uint8_t)((XSize - 1) / 3)) count = (uint8_t)((XSize - 1) / 3);
    if (count < 1) count = 1;

    /* assign types round-robin: 0,1,2,0,1,2,... */
    for (i = 0; i < count; i++) {
        types[i] = (uint8_t)(i % 3);
    }

    /* even horizontal spacing */
    spacing = (uint8_t)((XSize - 3) / (count + 1));
    if (spacing < 3) spacing = 3;

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (i < count) {
            x = (uint8_t)((i + 1) * spacing);
            if (x > (uint8_t)(XSize - 3)) x = (uint8_t)(XSize - 3);
            enemies[i].x = x;
            enemies[i].y = 1;
            enemies[i].type = types[i];
            enemies[i].active = 1;
            enemies[i].dir = (i % 2);
            enemies[i].timer = 0;
        } else {
            enemies[i].active = 0;
        }
    }
}

/* ---------- main ---------- */

int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    /* persistent game variables (declared once for C89) */
    uint8_t px, py, lives;
    uint16_t score;
    uint8_t wave;
    uint8_t frame;
    uint8_t game_over;
    uint8_t input;
    uint8_t old_px;
    uint8_t i, j;
    uint8_t all_dead;
    uint8_t fired;
    uint8_t hit;
    uint8_t old_score_hi, old_score_lo;
    uint8_t old_lives;
    uint8_t old_ey;
    uint8_t old_ex;
    uint8_t enemy_speed;
    uint8_t move_down;
    uint8_t fire_enemy;
    uint8_t bi;

    Bullet p_bullets[MAX_P_BULLETS];
    Bullet e_bullets[MAX_E_BULLETS];
    Enemy enemies[MAX_ENEMIES];

    /* infinite outer loop: game restarts after completion */
    for (;;) {

        /* ---- reset state ---- */
        px = (uint8_t)(XSize / 2 - 1);
        if (px > (uint8_t)(XSize - 3)) px = (uint8_t)(XSize - 3);
        py = (uint8_t)(YSize - 4);
        lives = NUM_LIVES;
        score = 0;
        wave = 1;
        frame = 0;
        game_over = 0;
        old_score_hi = 0;
        old_score_lo = 0;
        old_lives = NUM_LIVES;

        for (i = 0; i < MAX_P_BULLETS; i++) {
            p_bullets[i].active = 0;
            p_bullets[i].x = 0;
            p_bullets[i].y = 0;
        }
        for (i = 0; i < MAX_E_BULLETS; i++) {
            e_bullets[i].active = 0;
            e_bullets[i].x = 0;
            e_bullets[i].y = 0;
        }

        spawn_wave(enemies, wave);

        /* ---- initial render ---- */
        _XL_CLEAR_SCREEN();
        draw_entity(px, py, PLAYER_TILE_BASE, _XL_CYAN);
        for (i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                draw_entity(enemies[i].x, enemies[i].y,
                            enemy_tile_base(enemies[i].type),
                            enemy_color(enemies[i].type));
            }
        }
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0, 0, 5, 0);
        _XL_PRINTD((uint8_t)(XSize - 2), 0, 1, NUM_LIVES);

        /* ==================== GAME LOOP ==================== */
        while (!game_over) {

            frame++;
            input = _XL_INPUT();
            old_px = px;

            /* ---------- player movement ---------- */
            if (_XL_LEFT(input)) {
                if (px > 0) px--;
            } else if (_XL_RIGHT(input)) {
                if (px < (uint8_t)(XSize - 3)) px++;
            }
            if (px != old_px) {
                delete_entity(old_px, py);
                draw_entity(px, py, PLAYER_TILE_BASE, _XL_CYAN);
            }

            /* ---------- player fire ---------- */
            if (_XL_FIRE(input) && py > 0) {
                fired = 0;
                for (i = 0; i < MAX_P_BULLETS && !fired; i++) {
                    if (!p_bullets[i].active) {
                        p_bullets[i].x = (uint8_t)(px + 1);
                        p_bullets[i].y = (uint8_t)(py - 1);
                        p_bullets[i].active = 1;
                        _XL_DRAW(p_bullets[i].x, p_bullets[i].y,
                                 tile_map[P_BULLET_TILE], _XL_WHITE);
                        _XL_SHOOT_SOUND();
                        fired = 1;
                    }
                }
            }

            /* ---------- update player bullets ---------- */
            for (i = 0; i < MAX_P_BULLETS; i++) {
                if (p_bullets[i].active) {
                    _XL_DELETE(p_bullets[i].x, p_bullets[i].y);
                    p_bullets[i].y--;
                    if (p_bullets[i].y <= 0) {
                        p_bullets[i].active = 0;
                    } else {
                        _XL_DRAW(p_bullets[i].x, p_bullets[i].y,
                                 tile_map[P_BULLET_TILE], _XL_WHITE);
                    }
                }
            }

            /* ---------- update enemy bullets ---------- */
            for (i = 0; i < MAX_E_BULLETS; i++) {
                if (e_bullets[i].active) {
                    _XL_DELETE(e_bullets[i].x, e_bullets[i].y);
                    e_bullets[i].y++;
                    if (e_bullets[i].y >= (uint8_t)(YSize - 1)) {
                        e_bullets[i].active = 0;
                    } else {
                        _XL_DRAW(e_bullets[i].x, e_bullets[i].y,
                                 tile_map[E_BULLET_TILE], _XL_MAGENTA);
                    }
                }
            }

            /* ---------- update enemies ---------- */
            for (i = 0; i < MAX_ENEMIES; i++) {
                if (!enemies[i].active) continue;

                old_ex = enemies[i].x;
                old_ey = enemies[i].y;
                enemies[i].timer++;

                /* movement per type */
                if (enemies[i].type == 0) {
                    /* Scout: horizontal every frame, down every 4 */
                    if (enemies[i].dir == 0) {
                        if (enemies[i].x > 0) enemies[i].x--;
                        else enemies[i].dir = 1;
                    } else {
                        if (enemies[i].x < (uint8_t)(XSize - 3)) enemies[i].x++;
                        else enemies[i].dir = 0;
                    }
                    move_down = (enemies[i].timer >= 4);
                    if (move_down) enemies[i].timer = 0;
                    if (move_down) enemies[i].y++;
                } else if (enemies[i].type == 1) {
                    /* Fighter: zigzag (flip every 3), down every 3, fire every 10 */
                    if (enemies[i].timer >= 3) {
                        enemies[i].dir = (uint8_t)(!enemies[i].dir);
                    }
                    if (enemies[i].dir == 0) {
                        if (enemies[i].x > 0) enemies[i].x--;
                    } else {
                        if (enemies[i].x < (uint8_t)(XSize - 3)) enemies[i].x++;
                    }
                    move_down = (enemies[i].timer >= 6);
                    if (move_down) enemies[i].y++;
                    fire_enemy = (enemies[i].timer >= 10);
                    if (fire_enemy) {
                        enemies[i].timer = 0;
                        /* fire enemy bullet */
                        for (j = 0; j < MAX_E_BULLETS; j++) {
                            if (!e_bullets[j].active) {
                                e_bullets[j].x = (uint8_t)(enemies[i].x + 1);
                                e_bullets[j].y = (uint8_t)(enemies[i].y + 3);
                                e_bullets[j].active = 1;
                                _XL_DRAW(e_bullets[j].x, e_bullets[j].y,
                                         tile_map[E_BULLET_TILE], _XL_MAGENTA);
                                break;
                            }
                        }
                    }
                } else {
                    /* Boss: horizontal every 2 frames, down every 5, fire every 7 */
                    if (enemies[i].timer >= 2) {
                        if (enemies[i].dir == 0) {
                            if (enemies[i].x > 0) enemies[i].x--;
                            else enemies[i].dir = 1;
                        } else {
                            if (enemies[i].x < (uint8_t)(XSize - 3)) enemies[i].x++;
                            else enemies[i].dir = 0;
                        }
                    }
                    move_down = (enemies[i].timer >= 10);
                    if (move_down) enemies[i].y++;
                    fire_enemy = (enemies[i].timer >= 14);
                    if (fire_enemy) {
                        enemies[i].timer = 0;
                        for (j = 0; j < MAX_E_BULLETS; j++) {
                            if (!e_bullets[j].active) {
                                e_bullets[j].x = (uint8_t)(enemies[i].x + 1);
                                e_bullets[j].y = (uint8_t)(enemies[i].y + 3);
                                e_bullets[j].active = 1;
                                _XL_DRAW(e_bullets[j].x, e_bullets[j].y,
                                         tile_map[E_BULLET_TILE], _XL_MAGENTA);
                                break;
                            }
                        }
                    }
                }

                /* remove if reached player row */
                if (enemies[i].y >= (uint8_t)(YSize - 5)) {
                    enemies[i].active = 0;
                    delete_entity(old_ex, old_ey);
                    lives--;
                    _XL_EXPLOSION_SOUND();
                    continue;
                }

                /* redraw if moved */
                if (enemies[i].x != old_ex || enemies[i].y != old_ey) {
                    delete_entity(old_ex, old_ey);
                    draw_entity(enemies[i].x, enemies[i].y,
                                enemy_tile_base(enemies[i].type),
                                enemy_color(enemies[i].type));
                }
            }

            /* ---------- collisions: player bullets vs enemies ---------- */
            for (i = 0; i < MAX_P_BULLETS; i++) {
                if (!p_bullets[i].active) continue;
                for (j = 0; j < MAX_ENEMIES; j++) {
                    if (!enemies[j].active) continue;
                    hit = bullet_hits_enemy(p_bullets[i].x, p_bullets[i].y,
                                            enemies[j].x, enemies[j].y);
                    if (hit) {
                        p_bullets[i].active = 0;
                        _XL_DELETE(p_bullets[i].x, p_bullets[i].y);
                        enemies[j].active = 0;
                        delete_entity(enemies[j].x, enemies[j].y);
                        if (enemies[j].type == 0) score += 10;
                        else if (enemies[j].type == 1) score += 20;
                        else score += 50;
                        _XL_PING_SOUND();
                        break;
                    }
                }
            }

            /* ---------- collisions: enemy bullets vs player ---------- */
            for (i = 0; i < MAX_E_BULLETS; i++) {
                if (!e_bullets[i].active) continue;
                hit = bullet_hits_enemy(e_bullets[i].x, e_bullets[i].y,
                                        px, py);
                if (hit) {
                    e_bullets[i].active = 0;
                    _XL_DELETE(e_bullets[i].x, e_bullets[i].y);
                    lives--;
                    _XL_ZAP_SOUND();
                    break;
                }
            }

            /* ---------- collisions: enemies vs player (body) ---------- */
            for (i = 0; i < MAX_ENEMIES; i++) {
                if (!enemies[i].active) continue;
                if (entities_overlap(enemies[i].x, enemies[i].y, px, py)) {
                    enemies[i].active = 0;
                    delete_entity(enemies[i].x, enemies[i].y);
                    lives--;
                    _XL_EXPLOSION_SOUND();
                    break;
                }
            }

            /* ---------- check lives ---------- */
            if (lives < old_lives) {
                old_lives = lives;
                _XL_SET_TEXT_COLOR(_XL_WHITE);
                _XL_PRINTD((uint8_t)(XSize - 2), 0, 1, lives);
                if (lives == 0) {
                    game_over = 1;
                }
            }

            /* ---------- update score display ---------- */
            {
                uint8_t new_hi, new_lo;
                new_hi = (uint8_t)((score >> 8) & 0xFF);
                new_lo = (uint8_t)(score & 0xFF);
                if (new_hi != old_score_hi || new_lo != old_score_lo) {
                    old_score_hi = new_hi;
                    old_score_lo = new_lo;
                    _XL_SET_TEXT_COLOR(_XL_WHITE);
                    _XL_PRINTD(0, 0, 5, score);
                }
            }

            /* ---------- wave complete? ---------- */
            all_dead = 1;
            for (i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].active) {
                    all_dead = 0;
                    break;
                }
            }
            if (all_dead && !game_over) {
                wave++;
                _XL_SLEEP(1);
                spawn_wave(enemies, wave);
                for (i = 0; i < MAX_ENEMIES; i++) {
                    if (enemies[i].active) {
                        draw_entity(enemies[i].x, enemies[i].y,
                                    enemy_tile_base(enemies[i].type),
                                    enemy_color(enemies[i].type));
                    }
                }
                _XL_PING_SOUND();
            }

            /* ---------- frame delay ---------- */
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        /* ==================== GAME OVER ==================== */
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT((uint8_t)(XSize / 4), (uint8_t)(YSize / 2), "GAME OVER");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT((uint8_t)(XSize / 4), (uint8_t)(YSize / 2 + 2), "SCORE");
        _XL_PRINTD((uint8_t)(XSize / 4 + 5), (uint8_t)(YSize / 2 + 2), 5, score);
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT((uint8_t)(XSize / 4), (uint8_t)(YSize / 2 + 4), "WAVE");
        _XL_PRINTD((uint8_t)(XSize / 4 + 5), (uint8_t)(YSize / 2 + 4), 2, wave);
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT((uint8_t)(XSize / 4), (uint8_t)(YSize / 2 + 6), "PRESS ANY KEY");
        _XL_WAIT_FOR_INPUT();
    }

    return 0;
}

