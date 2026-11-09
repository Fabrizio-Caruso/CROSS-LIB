#include "cross_lib.h"

#define MAX_ENEMIES 64
#define MAX_BULLETS 32
#define MAX_ITEMS   8
#define ENEMY_MOVE_CHANCE 20
#define UI_ROW 0
#define PLAY_TOP 2

uint8_t player_x, player_y, player_dir;
uint8_t player_prev_x, player_prev_y, player_prev_dir;
uint8_t last_move_dir;
uint8_t lives;
uint16_t score;
uint16_t score_last;
uint8_t enemy_x[MAX_ENEMIES], enemy_y[MAX_ENEMIES];
uint8_t enemy_active[MAX_ENEMIES];
uint8_t enemy_dir[MAX_ENEMIES];
uint8_t enemy_prev_x[MAX_ENEMIES], enemy_prev_y[MAX_ENEMIES];
uint8_t enemy_prev_active[MAX_ENEMIES];
uint8_t bullet_x[MAX_BULLETS], bullet_y[MAX_BULLETS];
short bullet_dx[MAX_BULLETS], bullet_dy[MAX_BULLETS];
uint8_t bullet_active[MAX_BULLETS];
uint8_t item_x[MAX_ITEMS], item_y[MAX_ITEMS];
uint8_t item_type[MAX_ITEMS];
uint8_t item_active[MAX_ITEMS];
uint8_t fire_mode;
uint8_t fire_upgrade_cnt;
uint16_t max_bullets_allowed;
uint8_t invincible_timer;
uint8_t fire_tick;

static void draw_player(uint8_t x, uint8_t y, uint8_t dir)
{
    uint8_t col;
    col = invincible_timer ? _XL_YELLOW : _XL_CYAN;
    if (dir == 0) {
        _XL_DRAW(x,     y,     _TILE_2, col);
        _XL_DRAW(x + 1, y,     _TILE_3, col);
        _XL_DRAW(x,     y + 1, _TILE_4, col);
        _XL_DRAW(x + 1, y + 1, _TILE_5, col);
    } else {
        _XL_DRAW(x,     y,     _TILE_6, col);
        _XL_DRAW(x + 1, y,     _TILE_7, col);
        _XL_DRAW(x,     y + 1, _TILE_8, col);
        _XL_DRAW(x + 1, y + 1, _TILE_9, col);
    }
}

static void del_player(uint8_t x, uint8_t y)
{
    _XL_DELETE(x,     y);
    _XL_DELETE(x + 1, y);
    _XL_DELETE(x,     y + 1);
    _XL_DELETE(x + 1, y + 1);
}

static void draw_enemy(uint8_t x, uint8_t y, uint8_t dir)
{
    if (dir == 0) {
        _XL_DRAW(x,     y,     _TILE_10, _XL_RED);
        _XL_DRAW(x + 1, y,     _TILE_11, _XL_RED);
        _XL_DRAW(x,     y + 1, _TILE_12, _XL_RED);
        _XL_DRAW(x + 1, y + 1, _TILE_13, _XL_RED);
    } else {
        _XL_DRAW(x,     y,     _TILE_14, _XL_RED);
        _XL_DRAW(x + 1, y,     _TILE_15, _XL_RED);
        _XL_DRAW(x,     y + 1, _TILE_16, _XL_RED);
        _XL_DRAW(x + 1, y + 1, _TILE_17, _XL_RED);
    }
}

static void del_enemy(uint8_t x, uint8_t y)
{
    _XL_DELETE(x,     y);
    _XL_DELETE(x + 1, y);
    _XL_DELETE(x,     y + 1);
    _XL_DELETE(x + 1, y + 1);
}

static void draw_item(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t col;
    if (t == 0) col = _XL_CYAN;
    else if (t == 1) col = _XL_YELLOW;
    else col = _XL_GREEN;
    _XL_DRAW(x, y, _TILE_18, col);
}

static void del_item(uint8_t x, uint8_t y)
{
    _XL_DELETE(x, y);
}

static uint8_t blocks_overlap(uint8_t ax, uint8_t ay, uint8_t bx, uint8_t by)
{
    if (ax > bx + 1 || bx > ax + 1) return 0;
    if (ay > by + 1 || by > ay + 1) return 0;
    return 1;
}

static uint8_t enemy_occupies(uint8_t x, uint8_t y)
{
    uint8_t i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_active[i]) {
            if (blocks_overlap(x, y, enemy_x[i], enemy_y[i])) return 1;
        }
    }
    return 0;
}

static uint8_t bullet_hits_enemy(uint8_t bx, uint8_t by, uint8_t ex, uint8_t ey)
{
    if (bx < ex || bx > ex + 1) return 0;
    if (by < ey || by > ey + 1) return 0;
    return 1;
}

static void check_player_enemy_collision(void)
{
    uint8_t i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (!enemy_active[i]) continue;
        if (!blocks_overlap(player_x, player_y, enemy_x[i], enemy_y[i])) continue;
        del_enemy(enemy_x[i], enemy_y[i]);
        enemy_active[i] = 0;
        if (invincible_timer) {
            score += 20;
            _XL_TOCK_SOUND();
        } else {
            if (lives > 0) lives--;
            _XL_EXPLOSION_SOUND();
        }
    }
}

static uint8_t count_active_bullets(void)
{
    uint8_t i;
    uint8_t c;
    c = 0;
    for (i = 0; i < MAX_BULLETS; i++) if (bullet_active[i]) c++;
    return c;
}

static void spawn_enemy(uint8_t idx)
{
    uint8_t side;
    uint8_t ex, ey;
    uint8_t span, yspan;

    span = XSize - 2;
    if (span < 1) span = 1;
    yspan = YSize - PLAY_TOP - 1;
    if (yspan < 1) yspan = 1;

    side = (uint8_t)(_XL_RAND() % 4);
    if (side == 0) {
        ex = 0;
        ey = PLAY_TOP + (uint8_t)(_XL_RAND() % yspan);
    } else if (side == 1) {
        ex = XSize - 2;
        ey = PLAY_TOP + (uint8_t)(_XL_RAND() % yspan);
    } else if (side == 2) {
        ex = (uint8_t)(_XL_RAND() % span);
        ey = PLAY_TOP;
    } else {
        ex = (uint8_t)(_XL_RAND() % span);
        ey = YSize - 2;
    }

    enemy_x[idx] = ex;
    enemy_y[idx] = ey;
    enemy_active[idx] = 1;
    enemy_dir[idx] = 1;
    enemy_prev_x[idx] = ex;
    enemy_prev_y[idx] = ey;
    enemy_prev_active[idx] = 1;
    draw_enemy(ex, ey, 1);
}

/* ---------- game init ---------- */

void init_game(void)
{
    uint8_t i;

    _XL_CLEAR_SCREEN();

    player_x = XSize / 2;
    if (player_x + 1 >= XSize) player_x = XSize - 2;
    player_y = PLAY_TOP + (uint8_t)((YSize - PLAY_TOP - 2) / 2);
    if (player_y + 1 >= YSize) player_y = YSize - 2;

    player_dir = 1;
    last_move_dir = 3;
    lives = 3;
    score = 0;
    score_last = 0xFFFF;
    fire_mode = 0;
    fire_upgrade_cnt = 0;
    max_bullets_allowed = 4;
    invincible_timer = 0;
    fire_tick = 0;

    for (i = 0; i < MAX_ENEMIES; i++) {
        enemy_active[i] = 0;
        enemy_prev_active[i] = 0;
        enemy_dir[i] = 1;
        enemy_prev_x[i] = 0;
        enemy_prev_y[i] = 0;
    }
    for (i = 0; i < MAX_BULLETS; i++) bullet_active[i] = 0;
    for (i = 0; i < MAX_ITEMS; i++) item_active[i] = 0;

    for (i = 0; i < 16; i++) {
        spawn_enemy(i);
    }

    draw_player(player_x, player_y, player_dir);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(0, UI_ROW, 4, score);
    _XL_PRINT(4, UI_ROW, "L");
    _XL_PRINTD(5, UI_ROW, 1, lives);
}

/* ---------- main update ---------- */

void update_game(void)
{
    uint8_t inp, i, j, k;
    short dx, dy;
    short bdx[3], bdy[3];
    uint8_t changed;
    uint8_t bx, by;
    uint8_t active_bullets;
    uint8_t nx, ny;
    uint8_t occupied;
    uint8_t ox, oy;
    uint8_t t_item;
    uint8_t rad;
    uint8_t dxx, dyy;
    uint8_t spawn_thresh;
    uint8_t hit_found;

    /* update score display */
    if (score != score_last) {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0, UI_ROW, 4, score);
        score_last = score;
    }
    if (invincible_timer) invincible_timer--;

    check_player_enemy_collision();

    /* --- player input --- */
    inp = _XL_INPUT();
    if (_XL_LEFT(inp)) {
        if (player_x > 0) player_x--;
        player_dir = 0;
        last_move_dir = 2;
        _XL_TICK_SOUND();
    }
    if (_XL_RIGHT(inp)) {
        if (player_x < XSize - 2) player_x++;
        player_dir = 1;
        last_move_dir = 3;
        _XL_TICK_SOUND();
    }
    if (_XL_UP(inp)) {
        if (player_y > PLAY_TOP) player_y--;
        last_move_dir = 0;
        _XL_TICK_SOUND();
    }
    if (_XL_DOWN(inp)) {
        if (player_y < YSize - 2) player_y++;
        last_move_dir = 1;
        _XL_TICK_SOUND();
    }

    /* redraw player only if it changed */
    changed = (player_x != enemy_prev_x[0]); /* placeholder, will fix below */
    /* actually compare against stored prev */
    changed = 0;
    /* We need separate prev tracking for player. Use a static or global. */
    /* Simpler: compare against a local static-like pattern via globals. */
    /* Let me just use globals player_prev_x etc. */

    /* Actually let me restructure: I'll add player_prev_x, player_prev_y, player_prev_dir as globals. */
    /* For now, use a simple approach: always check if position/direction differs from a stored value. */

    /* I'll handle this with dedicated globals below. For the code structure,
       let me just use the pattern directly. */

    /* --- player redraw (using globals) --- */
    /* (handled below with proper prev variables) */

    /* --- firing --- */
    active_bullets = count_active_bullets();
    if (_XL_FIRE(inp) && (fire_tick % 4 == 0) && active_bullets < max_bullets_allowed) {
        if (fire_mode) {
            if (last_move_dir == 2) {
                bdx[0] = -1; bdy[0] = 0;
                bdx[1] = -1; bdy[1] = -1;
                bdx[2] = -1; bdy[2] = 1;
            } else if (last_move_dir == 3) {
                bdx[0] = 1; bdy[0] = 0;
                bdx[1] = 1; bdy[1] = -1;
                bdx[2] = 1; bdy[2] = 1;
            } else if (last_move_dir == 0) {
                bdx[0] = 0;  bdy[0] = -1;
                bdx[1] = -1; bdy[1] = -1;
                bdx[2] = 1;  bdy[2] = -1;
            } else {
                bdx[0] = 0;  bdy[0] = 1;
                bdx[1] = -1; bdy[1] = 1;
                bdx[2] = 1;  bdy[2] = 1;
            }
        } else {
            bdx[0] = (last_move_dir == 2) ? -1 : (last_move_dir == 3) ? 1 : 0;
            bdy[0] = (last_move_dir == 0) ? -1 : (last_move_dir == 1) ? 1 : 0;
        }

        for (k = 0; k < (fire_mode ? 3 : 1); k++) {
            /* compute bullet spawn position relative to 2x2 player */
            if (last_move_dir == 2) {
                bx = player_x - 1;
                by = player_y + 1 + (uint8_t)(bdy[k]);
            } else if (last_move_dir == 3) {
                bx = player_x + 2;
                by = player_y + 1 + (uint8_t)(bdy[k]);
            } else if (last_move_dir == 0) {
                bx = player_x + 1 + (uint8_t)(bdx[k]);
                by = player_y - 1;
            } else {
                bx = player_x + 1 + (uint8_t)(bdx[k]);
                by = player_y + 2;
            }

            if (bx >= XSize || by >= YSize || by < PLAY_TOP) continue;

            /* check if bullet immediately hits an enemy */
            hit_found = 0;
            for (j = 0; j < MAX_ENEMIES; j++) {
                if (enemy_active[j] && bullet_hits_enemy(bx, by, enemy_x[j], enemy_y[j])) {
                    del_enemy(enemy_x[j], enemy_y[j]);
                    enemy_active[j] = 0;
                    score += 10;
                    _XL_TOCK_SOUND();
                    t_item = (uint8_t)(_XL_RAND() % 3);
                    for (i = 0; i < MAX_ITEMS; i++) {
                        if (!item_active[i]) {
                            item_active[i] = 1;
                            item_type[i] = t_item;
                            item_x[i] = enemy_x[j];
                            item_y[i] = enemy_y[j];
                            if (!enemy_occupies(item_x[i], item_y[i]))
                                draw_item(item_x[i], item_y[i], t_item);
                            break;
                        }
                    }
                    hit_found = 1;
                    break;
                }
            }
            if (hit_found) continue;

            for (i = 0; i < MAX_BULLETS; i++) {
                if (!bullet_active[i]) {
                    bullet_active[i] = 1;
                    bullet_x[i] = bx;
                    bullet_y[i] = by;
                    bullet_dx[i] = bdx[k];
                    bullet_dy[i] = bdy[k];
                    _XL_DRAW(bullet_x[i], bullet_y[i], _TILE_0, _XL_YELLOW);
                    _XL_SHOOT_SOUND();
                    break;
                }
            }
        }
    }
    fire_tick++;

    /* --- bullet movement --- */
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullet_active[i]) continue;
        ox = bullet_x[i];
        oy = bullet_y[i];
        bullet_x[i] = (uint8_t)(bullet_x[i] + bullet_dx[i]);
        bullet_y[i] = (uint8_t)(bullet_y[i] + bullet_dy[i]);

        /* delete old bullet tile if not overlapping player */
        if (!(ox == player_x || ox == player_x + 1) ||
            !(oy == player_y || oy == player_y + 1))
            _XL_DELETE(ox, oy);

        if (bullet_x[i] < XSize && bullet_y[i] >= PLAY_TOP && bullet_y[i] < YSize) {
            if (!(bullet_x[i] == player_x || bullet_x[i] == player_x + 1) ||
                !(bullet_y[i] == player_y || bullet_y[i] == player_y + 1))
                _XL_DRAW(bullet_x[i], bullet_y[i], _TILE_0, _XL_YELLOW);
        } else {
            bullet_active[i] = 0;
        }

        /* bullet vs enemy */
        for (j = 0; j < MAX_ENEMIES; j++) {
            if (enemy_active[j] &&
                bullet_hits_enemy(bullet_x[i], bullet_y[i],
                                  enemy_x[j], enemy_y[j])) {
                del_enemy(enemy_x[j], enemy_y[j]);
                enemy_active[j] = 0;
                bullet_active[i] = 0;
                if (!(bullet_x[i] == player_x || bullet_x[i] == player_x + 1) ||
                    !(bullet_y[i] == player_y || bullet_y[i] == player_y + 1))
                    _XL_DELETE(bullet_x[i], bullet_y[i]);
                score += 10;
                _XL_TOCK_SOUND();
                if (_XL_RAND() % 100 < 60) {
                    t_item = (uint8_t)(_XL_RAND() % 3);
                    for (k = 0; k < MAX_ITEMS; k++) {
                        if (!item_active[k]) {
                            item_active[k] = 1;
                            item_type[k] = t_item;
                            item_x[k] = enemy_x[j];
                            item_y[k] = enemy_y[j];
                            if (!enemy_occupies(item_x[k], item_y[k]))
                                draw_item(item_x[k], item_y[k], t_item);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }

    /* --- enemy AI movement --- */
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (!enemy_active[i]) continue;
        if (_XL_RAND() % ENEMY_MOVE_CHANCE != 0) continue;

        nx = enemy_x[i];
        ny = enemy_y[i];
        dx = (player_x > enemy_x[i]) ? 1 : (player_x < enemy_x[i]) ? -1 : 0;
        dy = (player_y > enemy_y[i]) ? 1 : (player_y < enemy_y[i]) ? -1 : 0;

        if (dx > 0 && enemy_x[i] + 1 < XSize - 1 && !enemy_occupies(enemy_x[i] + 1, enemy_y[i]))
            nx = enemy_x[i] + 1;
        else
            dx = 0;
        if (dx < 0 && enemy_x[i] > 0 && !enemy_occupies(enemy_x[i] - 1, enemy_y[i]))
            nx = enemy_x[i] - 1;
        else
            dx = 0;
        if (dy > 0 && enemy_y[i] + 1 < YSize - 1 && !enemy_occupies(enemy_x[i], enemy_y[i] + 1))
            ny = enemy_y[i] + 1;
        else
            dy = 0;
        if (dy < 0 && enemy_y[i] > PLAY_TOP && !enemy_occupies(enemy_x[i], enemy_y[i] - 1))
            ny = enemy_y[i] - 1;
        else
            dy = 0;

        /* check no other enemy overlaps the new position */
        occupied = 0;
        for (j = 0; j < MAX_ENEMIES; j++) {
            if (j != i && enemy_active[j]) {
                if (blocks_overlap(nx, ny, enemy_x[j], enemy_y[j])) {
                    occupied = 1;
                    break;
                }
            }
        }
        if (!occupied) {
            if (dx < 0) enemy_dir[i] = 0;
            else if (dx > 0) enemy_dir[i] = 1;
            enemy_x[i] = nx;
            enemy_y[i] = ny;
        }
    }

    check_player_enemy_collision();

    /* --- redraw enemies (only changed ones) --- */
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_active[i] != enemy_prev_active[i]) {
            if (enemy_prev_active[i]) del_enemy(enemy_prev_x[i], enemy_prev_y[i]);
            if (enemy_active[i]) draw_enemy(enemy_x[i], enemy_y[i], enemy_dir[i]);
            enemy_prev_active[i] = enemy_active[i];
            enemy_prev_x[i] = enemy_x[i];
            enemy_prev_y[i] = enemy_y[i];
        } else if (enemy_active[i]) {
            if (enemy_x[i] != enemy_prev_x[i] || enemy_y[i] != enemy_prev_y[i] ||
                enemy_dir[i] != enemy_dir[i]) {
                del_enemy(enemy_prev_x[i], enemy_prev_y[i]);
                draw_enemy(enemy_x[i], enemy_y[i], enemy_dir[i]);
                enemy_prev_x[i] = enemy_x[i];
                enemy_prev_y[i] = enemy_y[i];
            }
        }
    }

    /* --- item collection --- */
    for (i = 0; i < MAX_ITEMS; i++) {
        if (!item_active[i]) continue;
        if (blocks_overlap(player_x, player_y, item_x[i], item_y[i])) {
            del_item(item_x[i], item_y[i]);
            if (item_type[i] == 0) {
                invincible_timer = 120;
                _XL_PING_SOUND();
            } else if (item_type[i] == 1) {
                rad = XSize / 4;
                for (j = 0; j < MAX_ENEMIES; j++) {
                    if (enemy_active[j]) {
                        dxx = enemy_x[j] > item_x[i] ?
                              (uint8_t)(enemy_x[j] - item_x[i]) :
                              (uint8_t)(item_x[i] - enemy_x[j]);
                        dyy = enemy_y[j] > item_y[i] ?
                              (uint8_t)(enemy_y[j] - item_y[i]) :
                              (uint8_t)(item_y[i] - enemy_y[j]);
                        if (dxx <= rad && dyy <= rad) {
                            del_enemy(enemy_x[j], enemy_y[j]);
                            enemy_active[j] = 0;
                        }
                    }
                }
                _XL_ZAP_SOUND();
            } else {
                fire_upgrade_cnt++;
                if (fire_upgrade_cnt == 1) max_bullets_allowed = 8;
                else if (fire_upgrade_cnt == 2) max_bullets_allowed = 16;
                else if (fire_upgrade_cnt == 3) { max_bullets_allowed = 32; fire_mode = 1; }
                else score += 500;
                _XL_PING_SOUND();
            }
            item_active[i] = 0;
        }
    }

    /* --- spawn new enemies --- */
    spawn_thresh = 3 + (uint8_t)(score / 40);
    if (spawn_thresh > 30) spawn_thresh = 30;
    if (_XL_RAND() % 100 < spawn_thresh) {
        for (i = 0; i < MAX_ENEMIES; i++) {
            if (!enemy_active[i]) {
                spawn_enemy(i);
                break;
            }
        }
    }
}

/* ---------- player prev-state globals for redraw tracking ---------- */
uint8_t player_prev_x, player_prev_y, player_prev_dir;

/* ---------- main ---------- */

int main(void)
{
    uint8_t gox;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    init_game();
    player_prev_x = player_x;
    player_prev_y = player_y;
    player_prev_dir = player_dir;

    while (1) {
        uint8_t inp;
        uint8_t changed;

        update_game();

        /* redraw player only if it moved or turned */
        changed = (player_x != player_prev_x ||
                   player_y != player_prev_y ||
                   player_dir != player_prev_dir);
        if (changed) {
            del_player(player_prev_x, player_prev_y);
            draw_player(player_x, player_y, player_dir);
            player_prev_x = player_x;
            player_prev_y = player_y;
            player_prev_dir = player_dir;
        }

        /* update lives display */
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(5, UI_ROW, 1, lives);

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

        if (lives <= 0) {
            _XL_SET_TEXT_COLOR(_XL_MAGENTA);
            gox = (XSize > 12) ? (uint8_t)(XSize / 2 - 4) : 0;
            _XL_PRINT(gox, YSize / 2, "GAME OVER");
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINTD(0, YSize / 2 + 1, 4, score);
            _XL_ZAP_SOUND();
            _XL_WAIT_FOR_INPUT();
            init_game();
            player_prev_x = player_x;
            player_prev_y = player_y;
            player_prev_dir = player_dir;
        }
    }
    return 0;
}


