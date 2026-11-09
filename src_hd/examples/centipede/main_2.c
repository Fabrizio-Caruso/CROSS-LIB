#include "cross_lib.h"

#define MAX_SEGS     12
#define MAX_PIECES   12
#define MAX_BULLETS  6
#define FULL_LEN     12
#define LIFE_THRESH  12000
#define PLAYER_ROW   ((uint8_t)(YSize - 1))
#define AREA_TOP     ((uint8_t)(YSize - 4))
#define RESPAWN_WAIT 60

static uint8_t mush_hp[YSize][XSize];
static uint8_t mush_poison[YSize][XSize];

static uint8_t seg_x[MAX_SEGS];
static uint8_t seg_y[MAX_SEGS];
static uint8_t seg_next[MAX_SEGS];
static uint8_t seg_is_head[MAX_SEGS];
static uint8_t seg_poison[MAX_SEGS];
static uint8_t seg_alive[MAX_SEGS];

static uint8_t piece_head[MAX_PIECES];
static uint8_t piece_dir[MAX_PIECES];
static uint8_t piece_active[MAX_PIECES];
static uint8_t piece_in_area[MAX_PIECES];
static uint8_t num_pieces;

static uint8_t pl_x;
static uint8_t pl_alive;
static uint8_t respawn_timer;

static uint8_t bul_x[MAX_BULLETS];
static uint8_t bul_y[MAX_BULLETS];
static uint8_t bul_active[MAX_BULLETS];

static uint8_t fl_x, fl_y, fl_hp, fl_active;
static uint8_t sp_x, sp_y, sp_dir, sp_zig, sp_active;
static uint8_t sc_x, sc_y, sc_dir, sc_active;

static uint16_t score;
static uint8_t lives;
static uint16_t next_life;
static uint8_t centi_round;
static uint8_t enemy_timer;
static uint8_t head_timer;

static void place_mushroom(uint8_t x, uint8_t y, uint8_t hp, uint8_t poison);
static void remove_mushroom(uint8_t x, uint8_t y);
static void clear_mushrooms(void);
static void seed_mushrooms(uint8_t n);
static void spawn_centipede(uint8_t len);
static void spawn_detached_head(uint8_t from_left);
static void move_piece(uint8_t pi);
static void update_centipede(void);
static void hit_segment(uint8_t idx);
static void kill_player(void);
static void fire_bullet(void);
static void update_bullets(void);
static void spawn_flea(void);
static void update_flea(void);
static void spawn_spider(void);
static void update_spider(void);
static void spawn_scorpion(void);
static void update_scorpion(void);
static void check_life(void);
static void reset_game(void);
static void draw_player(void);
static void erase_player(void);
static void next_centipede(void);
static void check_all_dead(void);
static void draw_hud(void);

/* ===== mushroom helpers ===== */

static void place_mushroom(uint8_t x, uint8_t y, uint8_t hp, uint8_t poison)
{
    if (x >= XSize || y >= YSize) return;
    mush_hp[y][x] = hp;
    mush_poison[y][x] = poison;
    _XL_DRAW(x, y, (uint8_t)(poison ? _TILE_2 : _TILE_1),
             (uint8_t)(poison ? _XL_MAGENTA : _XL_GREEN));
}

static void remove_mushroom(uint8_t x, uint8_t y)
{
    if (x >= XSize || y >= YSize) return;
    _XL_DELETE(x, y);
    mush_hp[y][x] = 0;
    mush_poison[y][x] = 0;
}

static void clear_mushrooms(void)
{
    uint8_t x, y;
    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            if (mush_hp[y][x] != 0) {
                _XL_DELETE(x, y);
                mush_hp[y][x] = 0;
                mush_poison[y][x] = 0;
            }
        }
    }
}

static void seed_mushrooms(uint8_t n)
{
    uint8_t i;
    uint16_t rx, ry;
    for (i = 0; i < n; i++) {
        rx = (uint16_t)(_XL_RAND() % (uint16_t)XSize);
        ry = (uint16_t)(_XL_RAND() % (uint16_t)AREA_TOP);
        if (mush_hp[ry][rx] == 0) {
            place_mushroom((uint8_t)rx, (uint8_t)ry, 4, 0);
        }
    }
}

/* ===== centipede ===== */

static void spawn_centipede(uint8_t len)
{
    uint8_t i, start_x, dir;

    num_pieces = 0;
    for (i = 0; i < MAX_SEGS; i++) seg_alive[i] = 0;

    if (len > (uint8_t)(XSize - 1)) len = (uint8_t)(XSize - 1);
    if (len == 0) len = 1;
    start_x = (uint8_t)(_XL_RAND() % (uint16_t)(XSize - len));
    dir = (uint8_t)(_XL_RAND() & 1);

    for (i = 0; i < len; i++) {
        seg_x[i] = (uint8_t)(start_x + i);
        seg_y[i] = 0;
        seg_next[i] = (uint8_t)(i + 1 < len ? i + 1 : 0xFF);
        seg_is_head[i] = (i == 0) ? 1 : 0;
        seg_poison[i] = 0;
        seg_alive[i] = 1;
        _XL_DRAW(seg_x[i], 0, (i == 0) ? _TILE_4 : _TILE_5, _XL_RED);
    }

    piece_head[0] = 0;
    piece_dir[0] = dir;
    piece_active[0] = 1;
    piece_in_area[0] = 0;
    num_pieces = 1;
}

static void spawn_detached_head(uint8_t from_left)
{
    uint8_t i, pi, py;

    pi = 0xFF;
    for (i = 0; i < MAX_PIECES; i++) {
        if (!piece_active[i]) { pi = i; break; }
    }
    if (pi == 0xFF) return;

    for (i = 0; i < MAX_SEGS; i++) {
        if (!seg_alive[i]) break;
    }
    if (i >= MAX_SEGS) return;

    seg_x[i] = from_left ? 0 : (uint8_t)(XSize - 1);
    py = (uint8_t)(AREA_TOP + (_XL_RAND() % (uint16_t)(YSize - AREA_TOP)));
    if (py >= YSize) py = (uint8_t)(YSize - 1);
    seg_y[i] = py;
    seg_next[i] = 0xFF;
    seg_is_head[i] = 1;
    seg_poison[i] = 0;
    seg_alive[i] = 1;
    _XL_DRAW(seg_x[i], py, _TILE_4, _XL_RED);

    piece_head[pi] = i;
    piece_dir[pi] = from_left ? 1 : 0;
    piece_active[pi] = 1;
    piece_in_area[pi] = 1;
    num_pieces++;
}

static void move_piece(uint8_t pi)
{
    uint8_t hi, si, nsi, nx, ny, ox, oy, tile;
    uint8_t descend;

    if (!piece_active[pi]) return;
    hi = piece_head[pi];
    if (!seg_alive[hi]) { piece_active[pi] = 0; return; }

    nx = seg_x[hi];
    ny = seg_y[hi];
    descend = 0;

    if (piece_in_area[pi]) {
        if (piece_dir[pi]) {
            nx++;
            if (nx >= XSize) nx = 0;
        } else {
            if (nx == 0) nx = (uint8_t)(XSize - 1);
            else nx--;
        }
    } else {
        if (piece_dir[pi]) {
            nx++;
            if (nx >= XSize) { descend = 1; nx = seg_x[hi]; }
        } else {
            if (nx == 0) { descend = 1; }
            else nx--;
        }
        if (!descend && nx < XSize && ny < YSize && mush_hp[ny][nx] != 0) {
            descend = 1;
            nx = seg_x[hi];
        }
        if (descend) {
            ny++;
            piece_dir[pi] = (uint8_t)(1 - piece_dir[pi]);
            if (ny >= YSize) ny = (uint8_t)(YSize - 1);
            if (ny >= AREA_TOP) piece_in_area[pi] = 1;
            if (nx < XSize && ny < YSize && mush_hp[ny][nx] != 0) {
                ny++;
                if (ny >= YSize) ny = (uint8_t)(YSize - 1);
                if (ny >= AREA_TOP) piece_in_area[pi] = 1;
            }
        }
    }

    si = hi;
    {
        uint8_t cur_x = nx;
        uint8_t cur_y = ny;
        while (si != 0xFF && seg_alive[si]) {
            nsi = seg_next[si];
            ox = seg_x[si];
            oy = seg_y[si];
            seg_x[si] = cur_x;
            seg_y[si] = cur_y;
            if (ox != seg_x[si] || oy != seg_y[si]) {
                if (seg_is_head[si]) tile = seg_poison[si] ? _TILE_10 : _TILE_4;
                else tile = seg_poison[si] ? _TILE_11 : _TILE_5;
                _XL_DELETE(ox, oy);
                _XL_DRAW(seg_x[si], seg_y[si], tile, _XL_RED);
            }
            cur_x = ox;
            cur_y = oy;
            si = nsi;
        }
    }
}

static void update_centipede(void)
{
    uint8_t pi;
    for (pi = 0; pi < MAX_PIECES; pi++) {
        if (piece_active[pi]) move_piece(pi);
    }
}

/* ===== hit segment ===== */

static void hit_segment(uint8_t idx)
{
    uint8_t pi, hi, nsi, prev, j, tile, new_pi;

    pi = 0xFF;
    for (j = 0; j < MAX_PIECES; j++) {
        if (!piece_active[j]) continue;
        hi = piece_head[j];
        while (hi != 0xFF && seg_alive[hi]) {
            if (hi == idx) { pi = j; break; }
            hi = seg_next[hi];
        }
        if (pi != 0xFF) break;
    }
    if (pi == 0xFF) return;

    if (seg_is_head[idx]) {
        score += 100;
        _XL_PING_SOUND();
        _XL_DELETE(seg_x[idx], seg_y[idx]);
        nsi = seg_next[idx];
        seg_alive[idx] = 0;
        seg_is_head[idx] = 0;

        if (nsi != 0xFF && seg_alive[nsi]) {
            seg_is_head[nsi] = 1;
            piece_head[pi] = nsi;
            tile = seg_poison[nsi] ? _TILE_10 : _TILE_4;
            _XL_DRAW(seg_x[nsi], seg_y[nsi], tile, _XL_RED);
        } else {
            piece_active[pi] = 0;
        }
        place_mushroom(seg_x[idx], seg_y[idx], 4, seg_poison[idx]);
    } else {
        score += 10;
        _XL_PING_SOUND();
        _XL_DELETE(seg_x[idx], seg_y[idx]);

        prev = 0xFF;
        hi = piece_head[pi];
        while (hi != 0xFF && hi != idx) {
            prev = hi;
            hi = seg_next[hi];
        }
        nsi = seg_next[idx];
        seg_alive[idx] = 0;

        if (prev != 0xFF) {
            seg_next[prev] = 0xFF;
        }

        if (nsi != 0xFF && seg_alive[nsi]) {
            new_pi = 0xFF;
            for (j = 0; j < MAX_PIECES; j++) {
                if (!piece_active[j]) { new_pi = j; break; }
            }
            if (new_pi != 0xFF) {
                seg_is_head[nsi] = 1;
                piece_head[new_pi] = nsi;
                piece_dir[new_pi] = piece_dir[pi];
                piece_active[new_pi] = 1;
                piece_in_area[new_pi] = piece_in_area[pi];
                num_pieces++;
                tile = seg_poison[nsi] ? _TILE_10 : _TILE_4;
                _XL_DRAW(seg_x[nsi], seg_y[nsi], tile, _XL_RED);
            }
        }
        place_mushroom(seg_x[idx], seg_y[idx], 4, seg_poison[idx]);
    }
}

/* ===== player ===== */

static void draw_player(void)
{
    if (pl_alive) _XL_DRAW(pl_x, PLAYER_ROW, _TILE_3, _XL_WHITE);
}

static void erase_player(void)
{
    if (pl_alive) _XL_DELETE(pl_x, PLAYER_ROW);
}

static void kill_player(void)
{
    uint8_t x, y;
    erase_player();
    pl_alive = 0;
    lives--;
    respawn_timer = RESPAWN_WAIT;
    _XL_EXPLOSION_SOUND();
    for (y = 0; y < YSize; y++) {
        for (x = 0; x < XSize; x++) {
            if (mush_hp[y][x] != 0) {
                if (mush_poison[y][x] != 0) {
                    mush_poison[y][x] = 0;
                    _XL_DELETE(x, y);
                    _XL_DRAW(x, y, _TILE_1, _XL_GREEN);
                    score += 5;
                }
            }
        }
    }
}

/* ===== bullets ===== */

static void fire_bullet(void)
{
    uint8_t i;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bul_active[i]) {
            bul_x[i] = pl_x;
            bul_y[i] = (uint8_t)(PLAYER_ROW - 1);
            bul_active[i] = 1;
            _XL_DRAW(bul_x[i], bul_y[i], _TILE_9, _XL_WHITE);
            _XL_SHOOT_SOUND();
            return;
        }
    }
}

static void update_bullets(void)
{
    uint8_t i, j, old_y;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bul_active[i]) continue;

        old_y = bul_y[i];
        if (bul_y[i] == 0) {
            _XL_DELETE(bul_x[i], 0);
            bul_active[i] = 0;
            continue;
        }
        bul_y[i]--;

        {
            uint8_t hit = 0;
            for (j = 0; j < MAX_SEGS; j++) {
                if (seg_alive[j] && seg_x[j] == bul_x[i] && seg_y[j] == bul_y[i]) {
                    hit_segment(j);
                    hit = 1;
                    break;
                }
            }
            if (hit) {
                _XL_DELETE(bul_x[i], old_y);
                bul_active[i] = 0;
                continue;
            }
        }
        if (bul_x[i] < XSize && bul_y[i] < YSize && mush_hp[bul_y[i]][bul_x[i]] != 0) {
            mush_hp[bul_y[i]][bul_x[i]]--;
            if (mush_hp[bul_y[i]][bul_x[i]] == 0) {
                remove_mushroom(bul_x[i], bul_y[i]);
                score++;
                _XL_TICK_SOUND();
            } else {
                _XL_TICK_SOUND();
            }
            _XL_DELETE(bul_x[i], old_y);
            bul_active[i] = 0;
            continue;
        }
        if (fl_active && fl_x == bul_x[i] && fl_y == bul_y[i]) {
            fl_hp--;
            if (fl_hp == 0) {
                _XL_DELETE(fl_x, fl_y);
                fl_active = 0;
                score += 200;
                _XL_PING_SOUND();
            } else {
                _XL_PING_SOUND();
            }
            _XL_DELETE(bul_x[i], old_y);
            bul_active[i] = 0;
            continue;
        }
        if (sp_active && sp_x == bul_x[i] && sp_y == bul_y[i]) {
            uint8_t range;
            range = (uint8_t)(PLAYER_ROW - sp_y);
            if (range <= 3) score += 300;
            else if (range <= 6) score += 600;
            else score += 900;
            _XL_DELETE(sp_x, sp_y);
            sp_active = 0;
            _XL_PING_SOUND();
            _XL_DELETE(bul_x[i], old_y);
            bul_active[i] = 0;
            continue;
        }
        if (sc_active && sc_x == bul_x[i] && sc_y == bul_y[i]) {
            score += 1000;
            _XL_DELETE(sc_x, sc_y);
            sc_active = 0;
            _XL_PING_SOUND();
            _XL_DELETE(bul_x[i], old_y);
            bul_active[i] = 0;
            continue;
        }

        _XL_DELETE(bul_x[i], old_y);
        _XL_DRAW(bul_x[i], bul_y[i], _TILE_9, _XL_WHITE);
    }
}

/* ===== flea ===== */

static void spawn_flea(void)
{
    if (fl_active) return;
    fl_x = (uint8_t)(_XL_RAND() % (uint16_t)XSize);
    fl_y = 0;
    fl_hp = 2;
    fl_active = 1;
    _XL_DRAW(fl_x, 0, _TILE_6, _XL_CYAN);
}

static void update_flea(void)
{
    uint8_t old_y, cnt, x, y;
    if (!fl_active) return;

    old_y = fl_y;
    fl_y++;
    if (fl_y >= YSize) {
        _XL_DELETE(fl_x, old_y);
        fl_active = 0;
        return;
    }
    if (fl_x < XSize && fl_y < YSize && mush_hp[fl_y][fl_x] != 0) {
        _XL_DELETE(fl_x, old_y);
        fl_active = 0;
        return;
    }

    if (fl_y >= AREA_TOP) {
        cnt = 0;
        for (y = AREA_TOP; y < YSize; y++) {
            for (x = 0; x < XSize; x++) {
                if (mush_hp[y][x] != 0) cnt++;
            }
        }
        if (cnt < 5 && (fl_y % 3) == 0 && mush_hp[fl_y][fl_x] == 0) {
            place_mushroom(fl_x, fl_y, 4, 0);
        }
    }

    _XL_DELETE(fl_x, old_y);
    _XL_DRAW(fl_x, fl_y, _TILE_6, _XL_CYAN);

    if (fl_y == PLAYER_ROW && fl_x == pl_x && pl_alive) {
        kill_player();
        fl_active = 0;
    }
}

/* ===== spider ===== */

static void spawn_spider(void)
{
    if (sp_active) return;
    sp_x = (uint8_t)(_XL_RAND() % (uint16_t)XSize);
    sp_y = (uint8_t)(AREA_TOP + (_XL_RAND() % (uint16_t)(YSize - AREA_TOP)));
    if (sp_y >= YSize) sp_y = AREA_TOP;
    sp_dir = (uint8_t)(_XL_RAND() & 1);
    sp_zig = 0;
    sp_active = 1;
    _XL_DRAW(sp_x, sp_y, _TILE_7, _XL_YELLOW);
}

static void update_spider(void)
{
    uint8_t nx, ny, jump_amt;
    if (!sp_active) return;

    nx = sp_x;
    ny = sp_y;

    /* move horizontally */
    if (sp_dir) { nx++; if (nx >= XSize) nx = 0; }
    else { if (nx == 0) nx = (uint8_t)(XSize - 1); else nx--; }

    /* jump much higher every 5 horizontal steps */
    sp_zig++;
    if (sp_zig >= 5) {
        sp_zig = 0;
        jump_amt = (uint8_t)(2 + (_XL_RAND() % 3)); /* jump 2 to 4 rows */
        if (sp_dir) {
            ny += jump_amt;
            if (ny >= YSize) ny = (uint8_t)(YSize - 1);
        } else {
            if (ny < jump_amt) ny = 0;
            else ny -= jump_amt;
            if (ny < AREA_TOP) ny = AREA_TOP;
        }
        sp_dir = (uint8_t)(1 - sp_dir);
    }

    if (nx < XSize && ny < YSize && mush_hp[ny][nx] != 0) {
        remove_mushroom(nx, ny);
    }
    _XL_DELETE(sp_x, sp_y);
    sp_x = nx;
    sp_y = ny;
    _XL_DRAW(sp_x, sp_y, _TILE_7, _XL_YELLOW);

    if (sp_x == pl_x && sp_y == PLAYER_ROW && pl_alive) {
        kill_player();
        sp_active = 0;
    }
}

/* ===== scorpion ===== */

static void spawn_scorpion(void)
{
    if (sc_active) return;
    sc_x = (uint8_t)(_XL_RAND() % (uint16_t)XSize);
    sc_y = (uint8_t)(AREA_TOP + (_XL_RAND() % (uint16_t)(YSize - AREA_TOP)));
    if (sc_y >= YSize) sc_y = AREA_TOP;
    sc_dir = (uint8_t)(_XL_RAND() & 1);
    sc_active = 1;
    _XL_DRAW(sc_x, sc_y, _TILE_8, _XL_BLUE);
}

static void update_scorpion(void)
{
    uint8_t nx;
    if (!sc_active) return;

    nx = sc_x;
    if (sc_dir) { nx++; if (nx >= XSize) nx = 0; }
    else { if (nx == 0) nx = (uint8_t)(XSize - 1); else nx--; }

    if (mush_hp[sc_y][nx] != 0 && mush_poison[sc_y][nx] == 0) {
        mush_poison[sc_y][nx] = 1;
        _XL_DELETE(nx, sc_y);
        _XL_DRAW(nx, sc_y, _TILE_2, _XL_MAGENTA);
    }
    _XL_DELETE(sc_x, sc_y);
    sc_x = nx;
    _XL_DRAW(sc_x, sc_y, _TILE_8, _XL_BLUE);

    if (sc_x == pl_x && sc_y == PLAYER_ROW && pl_alive) {
        kill_player();
        sc_active = 0;
    }
}

/* ===== game flow ===== */

static void check_life(void)
{
    if (score >= next_life) {
        lives++;
        next_life += LIFE_THRESH;
        _XL_ZAP_SOUND();
    }
}

static void next_centipede(void)
{
    uint8_t len, detached, i, j;

    for (i = 0; i < MAX_SEGS; i++) {
        if (seg_alive[i]) {
            _XL_DELETE(seg_x[i], seg_y[i]);
            seg_alive[i] = 0;
        }
    }
    for (j = 0; j < MAX_PIECES; j++) piece_active[j] = 0;
    num_pieces = 0;

    centi_round++;
    len = (uint8_t)(FULL_LEN - centi_round);
    if (len < 1) len = 1;

    spawn_centipede(len);

    detached = (uint8_t)(FULL_LEN - len);
    if (detached > (uint8_t)(MAX_PIECES - 1)) detached = (uint8_t)(MAX_PIECES - 1);
    for (i = 0; i < detached; i++) {
        spawn_detached_head((uint8_t)(i & 1));
    }
}

static void check_all_dead(void)
{
    uint8_t i, all;
    all = 1;
    for (i = 0; i < MAX_SEGS; i++) {
        if (seg_alive[i]) { all = 0; break; }
    }
    if (all && centi_round > 0) {
        next_centipede();
    }
}

static void reset_game(void)
{
    uint8_t i, j;
    _XL_CLEAR_SCREEN();
    score = 0;
    lives = 3;
    next_life = LIFE_THRESH;
    centi_round = 0;
    pl_x = (uint8_t)(XSize / 2);
    pl_alive = 1;
    respawn_timer = 0;
    for (i = 0; i < MAX_BULLETS; i++) bul_active[i] = 0;
    fl_active = 0;
    sp_active = 0;
    sc_active = 0;
    num_pieces = 0;
    enemy_timer = 0;
    head_timer = 0;
    for (i = 0; i < MAX_SEGS; i++) seg_alive[i] = 0;
    for (j = 0; j < MAX_PIECES; j++) piece_active[j] = 0;

    clear_mushrooms();
    seed_mushrooms(20);
    draw_player();
    spawn_centipede(FULL_LEN);
    draw_hud();
}

static void draw_hud(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "SCORE");
    _XL_PRINTD(6, 0, 5, score);
    _XL_PRINT(0, 1, "LIVES");
    _XL_PRINTD(5, 1, 1, lives);
}

/* ===== main ===== */

int main(void)
{
    uint8_t input;
    uint8_t i;
    uint8_t any_in_area;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    reset_game();

    while (1) {
        input = _XL_INPUT();
        if (pl_alive) {
            if (_XL_LEFT(input)) {
                if (pl_x > 0) { erase_player(); pl_x--; draw_player(); }
            }
            if (_XL_RIGHT(input)) {
                if (pl_x < XSize - 1) { erase_player(); pl_x++; draw_player(); }
            }
            if (_XL_FIRE(input)) {
                fire_bullet();
            }
        }

        if (!pl_alive && respawn_timer > 0) {
            respawn_timer--;
            if (respawn_timer == 0) {
                if (lives > 0) {
                    pl_alive = 1;
                    draw_player();
                }
            }
        }

        update_bullets();
        update_centipede();

        if (pl_alive) {
            for (i = 0; i < MAX_SEGS; i++) {
                if (seg_alive[i] && seg_x[i] == pl_x && seg_y[i] == PLAYER_ROW) {
                    kill_player();
                    break;
                }
            }
        }

        enemy_timer++;
        if (enemy_timer >= 100) {
            enemy_timer = 0;
            if (!fl_active && (_XL_RAND() & 1)) spawn_flea();
            if (!sp_active && (_XL_RAND() & 1)) spawn_spider();
            if (!sc_active && centi_round > 1) spawn_scorpion();
        }

        any_in_area = 0;
        for (i = 0; i < MAX_PIECES; i++) {
            if (piece_active[i] && piece_in_area[i]) any_in_area = 1;
        }
        if (any_in_area) {
            head_timer++;
            if (head_timer >= 150) {
                head_timer = 0;
                spawn_detached_head((uint8_t)(_XL_RAND() & 1));
            }
        }

        update_flea();
        update_spider();
        update_scorpion();

        check_all_dead();
        check_life();

        if (lives == 0) {
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(0, (uint8_t)(YSize / 2), "GAME OVER");
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(0, (uint8_t)(YSize / 2 + 2), "SCORE");
            _XL_PRINTD(6, (uint8_t)(YSize / 2 + 2), 5, score);
            _XL_PRINT(0, (uint8_t)(YSize / 2 + 4), "PRESS FIRE TO RESTART");
            _XL_ZAP_SOUND();
            _XL_WAIT_FOR_INPUT();
            reset_game();
        }

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(6, 0, 5, score);
        _XL_PRINTD(5, 1, 1, lives);

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}