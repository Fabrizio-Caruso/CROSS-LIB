#include "cross_lib.h"

/* ============================================================
 *  HORIZONTAL SHOOTER - Cross-Lib / ANSI C89
 *  Player uses virtual y (vpy): display at vpy/2, 2x2 tiles.
 *  Even vpy → tile set A, Odd vpy → tile set B.
 *  Play area: rows TOP_ROW .. BOT_ROW (inclusive).
 * ============================================================ */

#define MAX_BULLETS     16
#define MAX_ENEMIES     16
#define MAX_EBULLETS    12
#define MAX_POWERUPS     4
#define FIRE_COOLDOWN    4
#define INVINCIBLE_TICKS 300
#define ENEMY_FIRE_CD   12

/* Play-area vertical bounds (real screen rows) */
#define TOP_ROW         2
#define BOT_ROW         (YSize - 2)

/* Virtual-y bounds derived from play area */
#define MIN_VPY         4
#define MAX_VPY         (uint8_t)(2 * (YSize - 3) + 1)

/* Tile / colour assignments */
/* Player 2x2 – EVEN virtual y */
#define T_PLAYER_E_TL  _TILE_1
#define T_PLAYER_E_TR  _TILE_6
#define T_PLAYER_E_BL  _TILE_0
#define T_PLAYER_E_BR  _TILE_26
/* Player 2x2 – ODD virtual y */
#define T_PLAYER_O_TL  _TILE_22
#define T_PLAYER_O_TR  _TILE_23
#define T_PLAYER_O_BL  _TILE_24
#define T_PLAYER_O_BR  _TILE_25

#define C_PLAYER     _XL_CYAN
#define T_ENEMY_L    _TILE_2
#define T_ENEMY_R    _TILE_7
#define C_ENEMY      _XL_RED

/* Bullet tiles: differ by origin parity */
#define T_BULLET_E   _TILE_3
#define T_BULLET_O   _TILE_8
#define C_BULLET     _XL_YELLOW

#define T_POWERUP    _TILE_4
#define C_POWERUP    _XL_GREEN
#define T_EBULLET    _TILE_5
#define C_EBULLET    _XL_MAGENTA

/* Power-up types */
#define PU_DOUBLE     0
#define PU_SPREAD     1
#define PU_INVINCIBLE 2
#define PU_POINTS     3

/* Screen buffer */
static uint8_t scr[XSize][YSize];

/* ---------- entity arrays ---------- */

typedef struct {
    uint8_t x, y;
    int8_t  dx, dy;
    uint8_t tile;
    uint8_t active;
} Bullet;

typedef struct {
    uint8_t x, y;
    int8_t  dx, dy;
    uint8_t fire_cd;
    uint8_t active;
} Enemy;

typedef struct {
    uint8_t x, y;
    uint8_t type;
    uint8_t active;
} PowerUp;

static Bullet   bullets[MAX_BULLETS];
static Enemy    enemies[MAX_ENEMIES];
static Bullet   ebullets[MAX_EBULLETS];
static PowerUp  powerups[MAX_POWERUPS];

/* ---------- player state ----------
 * vpy is the virtual y; screen row = vpy / 2.
 * Player occupies 2x2 at (px, vpy/2) .. (px+1, vpy/2+1).
 */
static uint8_t px, vpy;
static uint8_t lives;
static uint8_t power_level;
static uint8_t fire_timer;
static uint16_t invincible;
static uint16_t score;
static uint8_t tick;

/* ---------- helpers ---------- */

static void clear_scr(void)
{
    uint8_t x, y;
    for (x = 0; x < XSize; x++) {
        for (y = 0; y < YSize; y++) {
            if (scr[x][y] != 0) {
                _XL_DELETE(x, y);
                scr[x][y] = 0;
            }
        }
    }
}

static void set_cell(uint8_t x, uint8_t y, uint8_t tile, uint8_t color)
{
    if (x >= XSize || y >= YSize) return;
    if (scr[x][y] != tile) {
        _XL_DRAW(x, y, tile, color);
        scr[x][y] = tile;
    }
}

static void clear_cell(uint8_t x, uint8_t y)
{
    if (x >= XSize || y >= YSize) return;
    if (scr[x][y] != 0) {
        _XL_DELETE(x, y);
        scr[x][y] = 0;
    }
}

/* Draw 2x2 player at virtual position (x, vpy).
 * Screen rows are vpy/2 and vpy/2+1.
 * Tile set depends on parity of vpy. */
static void draw_player(uint8_t x, uint8_t vpy, uint8_t color)
{
    uint8_t sy = (uint8_t)(vpy / 2);
    if ((vpy & 1) == 0) {
        set_cell(x,     sy,     T_PLAYER_E_TL, color);
        set_cell(x + 1, sy,     T_PLAYER_E_TR, color);
        set_cell(x,     sy + 1, T_PLAYER_E_BL, color);
        set_cell(x + 1, sy + 1, T_PLAYER_E_BR, color);
    } else {
        set_cell(x,     sy,     T_PLAYER_O_TL, color);
        set_cell(x + 1, sy,     T_PLAYER_O_TR, color);
        set_cell(x,     sy + 1, T_PLAYER_O_BL, color);
        set_cell(x + 1, sy + 1, T_PLAYER_O_BR, color);
    }
}

/* Clear all 4 screen cells of the 2x2 player at virtual (x, vpy) */
static void clear_player(uint8_t x, uint8_t vpy)
{
    uint8_t sy = (uint8_t)(vpy / 2);
    clear_cell(x,     sy);
    clear_cell(x + 1, sy);
    clear_cell(x,     sy + 1);
    clear_cell(x + 1, sy + 1);
}

/* Clear old player cells NOT inside the new 2x2 at (nx, nvpy) */
static void clear_player_diff(uint8_t ox, uint8_t ovpy,
                              uint8_t nx, uint8_t nvpy)
{
    uint8_t oy = (uint8_t)(ovpy / 2);
    uint8_t ny = (uint8_t)(nvpy / 2);
    uint8_t cx, cy;
    for (cy = 0; cy < 2; cy++) {
        for (cx = 0; cx < 2; cx++) {
            uint8_t cellx = ox + cx;
            uint8_t celly = oy + cy;
            if (cellx < nx || cellx > (uint8_t)(nx + 1) ||
                celly < ny || celly > (uint8_t)(ny + 1)) {
                clear_cell(cellx, celly);
            }
        }
    }
}

/* Draw 2-cell horizontal enemy at (x,y) */
static void draw_enemy(uint8_t x, uint8_t y)
{
    set_cell(x,     y, T_ENEMY_L, C_ENEMY);
    set_cell(x + 1, y, T_ENEMY_R, C_ENEMY);
}

static void clear_enemy(uint8_t x, uint8_t y)
{
    clear_cell(x,     y);
    clear_cell(x + 1, y);
}

static void clear_enemy_diff(uint8_t ox, uint8_t oy,
                             uint8_t nx, uint8_t ny)
{
    if (!((ox == nx || ox == (uint8_t)(nx + 1)) && oy == ny)) {
        clear_cell(ox, oy);
    }
    if (!((ox + 1 == nx || ox + 1 == (uint8_t)(nx + 1)) && oy == ny)) {
        clear_cell(ox + 1, oy);
    }
}

/* ---------- game reset ---------- */

static void reset_game(void)
{
    uint8_t i;
    px = 2;
    vpy = MIN_VPY;         /* start at top of play area */
    lives = 3;
    power_level = 1;
    fire_timer = 0;
    invincible = 0;
    score = 0;
    tick = 0;

    for (i = 0; i < MAX_BULLETS; i++)   bullets[i].active = 0;
    for (i = 0; i < MAX_ENEMIES; i++)   enemies[i].active = 0;
    for (i = 0; i < MAX_EBULLETS; i++)  ebullets[i].active = 0;
    for (i = 0; i < MAX_POWERUPS; i++)  powerups[i].active = 0;

    clear_scr();
}

/* ---------- spawn helpers ---------- */

static uint8_t find_bullet_slot(void)
{
    uint8_t i;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) return i;
    }
    return 255;
}

static uint8_t find_enemy_slot(void)
{
    uint8_t i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) return i;
    }
    return 255;
}

static uint8_t find_ebullet_slot(void)
{
    uint8_t i;
    for (i = 0; i < MAX_EBULLETS; i++) {
        if (!ebullets[i].active) return i;
    }
    return 255;
}

static uint8_t find_powerup_slot(void)
{
    uint8_t i;
    for (i = 0; i < MAX_POWERUPS; i++) {
        if (!powerups[i].active) return i;
    }
    return 255;
}

/* ---------- fire player bullets ----------
 * Even vpy → bullet tile T_BULLET_E, primary y = sy (top of 2x2).
 * Odd  vpy → bullet tile T_BULLET_O, primary y = sy+1 (bottom of 2x2).
 * Bullets keep their tile for the entire lifespan. */

static void fire_player(void)
{
    uint8_t slot;
    uint8_t sy = (uint8_t)(vpy / 2);
    uint8_t btile;
    uint8_t primary_y;
    uint8_t secondary_y;

    if ((vpy & 1) == 0) {
        btile = T_BULLET_E;
        primary_y   = sy;       /* top of 2x2 */
        secondary_y = sy + 1;   /* bottom of 2x2 */
    } else {
        btile = T_BULLET_O;
        primary_y   = sy + 1;   /* bottom of 2x2 */
        secondary_y = sy;       /* top of 2x2 */
    }

    /* Level 1: single straight from primary row */
    slot = find_bullet_slot();
    if (slot < MAX_BULLETS && px + 2 < XSize &&
        primary_y >= TOP_ROW && primary_y <= BOT_ROW) {
        bullets[slot].x = px + 2;
        bullets[slot].y = primary_y;
        bullets[slot].dx = 1;
        bullets[slot].dy = 0;
        bullets[slot].tile = btile;
        bullets[slot].active = 1;
    }

    if (power_level >= 2) {
        slot = find_bullet_slot();
        if (slot < MAX_BULLETS && px + 2 < XSize &&
            secondary_y >= TOP_ROW && secondary_y <= BOT_ROW) {
            bullets[slot].x = px + 2;
            bullets[slot].y = secondary_y;
            bullets[slot].dx = 1;
            bullets[slot].dy = 0;
            bullets[slot].tile = btile;
            bullets[slot].active = 1;
        }
    }

    if (power_level >= 3) {
        slot = find_bullet_slot();
        if (slot < MAX_BULLETS && px + 2 < XSize &&
            primary_y > TOP_ROW) {
            bullets[slot].x = px + 2;
            bullets[slot].y = primary_y;
            bullets[slot].dx = 1;
            bullets[slot].dy = -1;
            bullets[slot].tile = btile;
            bullets[slot].active = 1;
        }
    }

    if (power_level >= 4) {
        slot = find_bullet_slot();
        if (slot < MAX_BULLETS && px + 2 < XSize &&
            secondary_y < BOT_ROW) {
            bullets[slot].x = px + 2;
            bullets[slot].y = secondary_y;
            bullets[slot].dx = 1;
            bullets[slot].dy = 1;
            bullets[slot].tile = btile;
            bullets[slot].active = 1;
        }
    }

    _XL_SHOOT_SOUND();
}

/* ---------- spawn enemy ---------- */

static void spawn_enemy(void)
{
    uint8_t slot, r;
    int8_t dy;
    uint8_t span;

    slot = find_enemy_slot();
    if (slot >= MAX_ENEMIES) return;

    enemies[slot].x = XSize - 2;
    span = (uint8_t)(BOT_ROW - TOP_ROW + 1);
    r = (uint8_t)(_XL_RAND() % span);
    enemies[slot].y = (uint8_t)(TOP_ROW + r);

    if (_XL_RAND() % 10 < 3) {
        dy = (_XL_RAND() & 1) ? -1 : 1;
        enemies[slot].dy = dy;
    } else {
        enemies[slot].dy = 0;
    }
    enemies[slot].dx = -1;
    enemies[slot].fire_cd = ENEMY_FIRE_CD + (uint8_t)(_XL_RAND() % 10);
    enemies[slot].active = 1;
}

/* ---------- spawn power-up ---------- */

static void spawn_powerup(void)
{
    uint8_t slot, r;
    uint8_t span;

    slot = find_powerup_slot();
    if (slot >= MAX_POWERUPS) return;

    powerups[slot].x = XSize - 1;
    span = (uint8_t)(BOT_ROW - TOP_ROW + 1);
    r = (uint8_t)(_XL_RAND() % span);
    powerups[slot].y = (uint8_t)(TOP_ROW + r);
    powerups[slot].type = (uint8_t)(_XL_RAND() % 4);
    powerups[slot].active = 1;
}

/* ---------- main game tick ---------- */

static void update(void)
{
    uint8_t input, i, j;
    uint8_t old_px, old_vpy;
    uint8_t sy;
    uint16_t r;
    int8_t ady;

    tick++;
    sy = (uint8_t)(vpy / 2);

    /* --- player movement (clamped to play area) --- */
    old_px = px;
    old_vpy = vpy;

    input = _XL_INPUT();
    if (_XL_UP(input) && vpy > MIN_VPY)
        vpy--;
    else if (_XL_DOWN(input) && vpy < MAX_VPY)
        vpy++;
    if (_XL_LEFT(input) && px > 0)
        px--;
    else if (_XL_RIGHT(input) && (uint8_t)(px + 1) < (uint8_t)(XSize - 1))
        px++;

    /* diff-clear only when screen position changed */
    if (old_px != px || (old_vpy / 2) != (vpy / 2)) {
        clear_player_diff(old_px, old_vpy, px, vpy);
    }

    /* --- player fire --- */
    if (fire_timer > 0) fire_timer--;
    if (_XL_FIRE(input) && fire_timer == 0) {
        fire_player();
        fire_timer = FIRE_COOLDOWN;
    }

    /* --- invincibility countdown --- */
    if (invincible > 0) invincible--;

    /* --- move player bullets (removed when leaving play area) --- */
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        {
            short nx = (short)bullets[i].x + (short)bullets[i].dx;
            short ny = (short)bullets[i].y + (short)bullets[i].dy;
            if (nx < 0 || nx >= (short)XSize ||
                ny < (short)TOP_ROW || ny > (short)BOT_ROW) {
                clear_cell(bullets[i].x, bullets[i].y);
                bullets[i].active = 0;
            } else {
                clear_cell(bullets[i].x, bullets[i].y);
                bullets[i].x = (uint8_t)nx;
                bullets[i].y = (uint8_t)ny;
            }
        }
    }

    /* --- move enemy bullets (removed when leaving play area) --- */
    for (i = 0; i < MAX_EBULLETS; i++) {
        if (!ebullets[i].active) continue;
        {
            short nx = (short)ebullets[i].x + (short)ebullets[i].dx;
            short ny = (short)ebullets[i].y + (short)ebullets[i].dy;
            if (nx < 0 || nx >= (short)XSize ||
                ny < (short)TOP_ROW || ny > (short)BOT_ROW) {
                clear_cell(ebullets[i].x, ebullets[i].y);
                ebullets[i].active = 0;
            } else {
                clear_cell(ebullets[i].x, ebullets[i].y);
                ebullets[i].x = (uint8_t)nx;
                ebullets[i].y = (uint8_t)ny;
            }
        }
    }

    /* --- move enemies: every 2 ticks --- */
    if ((tick & 1) == 0) {
        for (i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].active) continue;
            {
                short nx, ny;
                ady = enemies[i].dy;
                /* bounce within play area */
                if (ady != 0 &&
                    (enemies[i].y <= TOP_ROW || enemies[i].y >= BOT_ROW)) {
                    ady = 0;
                }
                nx = (short)enemies[i].x + (short)enemies[i].dx;
                ny = (short)enemies[i].y + (short)ady;

                if (nx < 0 || nx >= (short)(XSize - 1) ||
                    ny < (short)TOP_ROW || ny > (short)BOT_ROW) {
                    clear_enemy(enemies[i].x, enemies[i].y);
                    enemies[i].active = 0;
                } else {
                    clear_enemy_diff(enemies[i].x, enemies[i].y,
                                    (uint8_t)nx, (uint8_t)ny);
                    enemies[i].x = (uint8_t)nx;
                    enemies[i].y = (uint8_t)ny;
                    draw_enemy(enemies[i].x, enemies[i].y);
                }
            }

            if (enemies[i].active) {
                if (enemies[i].fire_cd > 0) {
                    enemies[i].fire_cd--;
                } else {
                    j = find_ebullet_slot();
                    if (j < MAX_EBULLETS && enemies[i].x > 0) {
                        ebullets[j].x = enemies[i].x - 1;
                        ebullets[j].y = enemies[i].y;
                        ebullets[j].dx = -1;
                        if (_XL_RAND() % 5 == 0) {
                            ebullets[j].dy = (_XL_RAND() & 1) ? -1 : 1;
                        } else {
                            ebullets[j].dy = 0;
                        }
                        ebullets[j].tile = T_EBULLET;
                        ebullets[j].active = 1;
                    }
                    enemies[i].fire_cd = ENEMY_FIRE_CD + (uint8_t)(_XL_RAND() % 8);
                }
            }
        }
    }

    /* --- move power-ups: every 3 ticks --- */
    if ((tick % 3) == 0) {
        for (i = 0; i < MAX_POWERUPS; i++) {
            if (!powerups[i].active) continue;
            clear_cell(powerups[i].x, powerups[i].y);
            if (powerups[i].x > 0) {
                powerups[i].x--;
            } else {
                powerups[i].active = 0;
            }
        }
    }

    /* --- collisions: player bullets vs enemies --- */
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        for (j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].active) continue;
            if ((bullets[i].x == enemies[j].x ||
                 bullets[i].x == (uint8_t)(enemies[j].x + 1))
                && bullets[i].y == enemies[j].y) {
                clear_cell(bullets[i].x, bullets[i].y);
                bullets[i].active = 0;
                clear_enemy(enemies[j].x, enemies[j].y);
                enemies[j].active = 0;
                score += 10;
                _XL_PING_SOUND();
                break;
            }
        }
    }

    /* --- collisions: enemy bullets vs player (2x2 area) --- */
    if (invincible == 0) {
        sy = (uint8_t)(vpy / 2);
        for (i = 0; i < MAX_EBULLETS; i++) {
            if (!ebullets[i].active) continue;
            if ((ebullets[i].x == px || ebullets[i].x == (uint8_t)(px + 1))
                && (ebullets[i].y == sy || ebullets[i].y == (uint8_t)(sy + 1))) {
                clear_cell(ebullets[i].x, ebullets[i].y);
                ebullets[i].active = 0;
                lives--;
                power_level = 1;
                invincible = INVINCIBLE_TICKS;
                _XL_EXPLOSION_SOUND();
                break;
            }
        }
    }

    /* --- collisions: enemies vs player (2x1 vs 2x2) --- */
    if (invincible == 0) {
        sy = (uint8_t)(vpy / 2);
        for (i = 0; i < MAX_ENEMIES; i++) {
            if (!enemies[i].active) continue;
            if ((enemies[i].y == sy || enemies[i].y == (uint8_t)(sy + 1))
                && enemies[i].x <= (uint8_t)(px + 1)
                && (uint8_t)(enemies[i].x + 1) >= px) {
                clear_enemy(enemies[i].x, enemies[i].y);
                enemies[i].active = 0;
                lives--;
                power_level = 1;
                invincible = INVINCIBLE_TICKS;
                _XL_EXPLOSION_SOUND();
                break;
            }
        }
    }

    /* --- collisions: player vs power-ups (2x2) --- */
    {
        sy = (uint8_t)(vpy / 2);
        for (i = 0; i < MAX_POWERUPS; i++) {
            if (!powerups[i].active) continue;
            if ((powerups[i].x == px || powerups[i].x == (uint8_t)(px + 1))
                && (powerups[i].y == sy || powerups[i].y == (uint8_t)(sy + 1))) {
                clear_cell(powerups[i].x, powerups[i].y);
                powerups[i].active = 0;
                switch (powerups[i].type) {
                    case PU_DOUBLE:
                        if (power_level < 2) power_level++;
                        else score += 5;
                        break;
                    case PU_SPREAD:
                        if (power_level < 4)
                            power_level = (power_level >= 3) ? 4 : 3;
                        else score += 10;
                        break;
                    case PU_INVINCIBLE:
                        invincible = INVINCIBLE_TICKS;
                        break;
                    case PU_POINTS:
                        score += 50;
                        break;
                }
                _XL_TOCK_SOUND();
            }
        }
    }

    /* --- spawn logic --- */
    r = _XL_RAND();
    if (r % 20 == 0) {
        spawn_enemy();
    }
    if (r % 150 == 0) {
        spawn_powerup();
    }

    /* --- render all active entities --- */
    for (i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active && bullets[i].x < XSize
            && bullets[i].y >= TOP_ROW && bullets[i].y <= BOT_ROW) {
            set_cell(bullets[i].x, bullets[i].y, bullets[i].tile, C_BULLET);
        }
    }
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active && (uint8_t)(enemies[i].x + 1) < XSize
            && enemies[i].y >= TOP_ROW && enemies[i].y <= BOT_ROW) {
            draw_enemy(enemies[i].x, enemies[i].y);
        }
    }
    for (i = 0; i < MAX_EBULLETS; i++) {
        if (ebullets[i].active && ebullets[i].x < XSize
            && ebullets[i].y >= TOP_ROW && ebullets[i].y <= BOT_ROW) {
            set_cell(ebullets[i].x, ebullets[i].y, T_EBULLET, C_EBULLET);
        }
    }
    for (i = 0; i < MAX_POWERUPS; i++) {
        if (powerups[i].active && powerups[i].x < XSize
            && powerups[i].y >= TOP_ROW && powerups[i].y <= BOT_ROW) {
            set_cell(powerups[i].x, powerups[i].y, T_POWERUP, C_POWERUP);
        }
    }

    /* player – blink when invincible */
    if (invincible > 0) {
        if ((invincible / 4) % 2 == 0) {
            draw_player(px, vpy, _XL_WHITE);
        } else {
            clear_player(px, vpy);
        }
    } else {
        draw_player(px, vpy, C_PLAYER);
    }

    /* --- HUD (rows 0-1, above play area) --- */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    {
        char buf[16];
        uint8_t k;
        uint16_t s = score;

        if (s >= 10000) {
            buf[0] = '0' + (char)(s / 10000);
            buf[1] = '0' + (char)((s / 1000) % 10);
            buf[2] = '0' + (char)((s / 100) % 10);
            buf[3] = '0' + (char)((s / 10) % 10);
            buf[4] = '0' + (char)(s % 10);
            buf[5] = '\0';
        } else if (s >= 1000) {
            buf[0] = '0' + (char)((s / 1000) % 10);
            buf[1] = '0' + (char)((s / 100) % 10);
            buf[2] = '0' + (char)((s / 10) % 10);
            buf[3] = '0' + (char)(s % 10);
            buf[4] = '\0';
        } else if (s >= 100) {
            buf[0] = '0' + (char)((s / 100) % 10);
            buf[1] = '0' + (char)((s / 10) % 10);
            buf[2] = '0' + (char)(s % 10);
            buf[3] = '\0';
        } else if (s >= 10) {
            buf[0] = '0' + (char)((s / 10) % 10);
            buf[1] = '0' + (char)(s % 10);
            buf[2] = '\0';
        } else {
            buf[0] = '0' + (char)s;
            buf[1] = '\0';
        }
        _XL_PRINT(0, 0, buf);

        /* life icons: small 2-tile player at row 1 (within HUD zone) */
        for (k = 0; k < lives && k < 5; k++) {
            set_cell(2 + k * 2,     1, T_PLAYER_E_TL, C_PLAYER);
            set_cell(2 + k * 2 + 1, 1, T_PLAYER_E_TR, C_PLAYER);
        }
        for (k = lives; k < 5; k++) {
            clear_cell(2 + k * 2,     1);
            clear_cell(2 + k * 2 + 1, 1);
        }

        _XL_SET_TEXT_COLOR(_XL_GREEN);
        {
            char pbuf[8];
            pbuf[0] = 'P';
            pbuf[1] = '0' + (char)power_level;
            pbuf[2] = '\0';
            _XL_PRINT(XSize - 4, 1, pbuf);
        }
    }
}

/* ---------- game over screen ---------- */

static void show_game_over(void)
{
    uint8_t input;
    clear_scr();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(2, YSize / 2 - 1, "GAME OVER");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    {
        char buf[16];
        uint16_t s = score;
        if (s >= 1000) {
            buf[0] = '0' + (char)(s / 1000);
            buf[1] = '0' + (char)((s / 100) % 10);
            buf[2] = '0' + (char)((s / 10) % 10);
            buf[3] = '0' + (char)(s % 10);
            buf[4] = '\0';
        } else if (s >= 100) {
            buf[0] = '0' + (char)(s / 100);
            buf[1] = '0' + (char)((s / 10) % 10);
            buf[2] = '0' + (char)(s % 10);
            buf[3] = '\0';
        } else if (s >= 10) {
            buf[0] = '0' + (char)(s / 10);
            buf[1] = '0' + (char)(s % 10);
            buf[2] = '\0';
        } else {
            buf[0] = '0' + (char)s;
            buf[1] = '\0';
        }
        buf[4] = '\0';
        _XL_PRINT(2, YSize / 2, buf);
    }
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(1, YSize / 2 + 2, "FIRE TO RESTART");

    while (1) {
        input = _XL_INPUT();
        if (_XL_FIRE(input)) break;
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

/* ---------- main ---------- */

int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        reset_game();
        while (lives > 0) {
            update();
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
        show_game_over();
    }

    return 0;
}

