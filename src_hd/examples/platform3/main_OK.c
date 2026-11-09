#include "cross_lib.h"

#define MAX_PLATFORMS   8
#define MAX_ELEVATORS   7
#define MAX_ENEMIES     16
#define MAX_ITEMS       20
#define MAX_BULLETS     5
#define MAX_LIVES       3
#define INVINC_FRAMES   240
#define JUMP_HEIGHT     5
#define MIN_PLATFORMS   3

#define CELL_EMPTY      0
#define CELL_PLATFORM   1
#define CELL_ELEVATOR   2
#define CELL_PLAYER     3
#define CELL_ENEMY      4
#define CELL_ITEM       5
#define CELL_SPECIAL    6
#define CELL_BULLET     7

typedef struct {
    uint8_t x, y, width;
} Platform;

typedef struct {
    uint8_t x, y_top, y_bot;
} Elevator;

typedef struct {
    uint8_t x, y;
    uint8_t dir;      /* 0=left, 1=right */
    uint8_t plat;
    uint8_t active;
} Enemy;

typedef struct {
    uint8_t x, y;
    uint8_t type;     /* 0=normal, 1=special */
    uint8_t active;
} Item;

typedef struct {
    uint8_t x, y;
    uint8_t active;
} Bullet;

static Platform  plats[MAX_PLATFORMS];
static Elevator  elevs[MAX_ELEVATORS];
static Enemy     enemies[MAX_ENEMIES];
static Item      items[MAX_ITEMS];
static Bullet    bullets[MAX_BULLETS];

/* Current visible cell. */
static uint8_t   cell[160][160];

/* Permanent geometry underneath dynamic sprites. */
static uint8_t   base_cell[160][160];

/* Player state: 2x2 sprite, top-left at px,py. */
static uint8_t   px, py;
static uint8_t   pdir;          /* 0=left, 1=right */
static uint8_t   jumping;
static uint8_t   jump_count;
static uint8_t   in_elev;
static uint8_t   elev_idx;
static uint8_t   invincible;
static uint16_t  inv_timer;

static uint16_t  score;
static uint8_t   lives;
static uint8_t   game_state;    /* 0=playing, 2=game_over, 3=level_complete */
static uint16_t  frame;
static uint8_t   items_collected;
static uint8_t   total_items;
static uint8_t   plat_count;
static uint8_t   elev_count;
static uint8_t   enemy_count;
static uint8_t   item_count;

/* ---------- helpers ---------- */

static uint8_t rnd(uint8_t lo, uint8_t hi) {
    if (hi <= lo) return lo;
    return lo + (uint8_t)(_XL_RAND() % (uint16_t)(hi - lo));
}

static void draw_base_cell(uint8_t x, uint8_t y) {
    uint8_t b = base_cell[x][y];

    if (b != CELL_EMPTY && cell[x][y] != b) {
        cell[x][y] = b;

        switch (b) {
            case CELL_PLATFORM:
                _XL_DRAW(x, y, _TILE_0, _XL_CYAN);
                break;
            case CELL_ELEVATOR:
                _XL_DRAW(x, y, _TILE_0, _XL_GREEN);
                break;
            default:
                break;
        }
    }
}

/*
 * set_cell - write a cell.
 *
 * Dynamic entities can temporarily cover platform/elevator cells.
 * Items and special items are protected from being overwritten by
 * player/enemy/bullet sprites unless explicitly force-cleared.
 */
static void set_cell(uint8_t x, uint8_t y, uint8_t what) {
    uint8_t old;

    if (x >= (uint8_t)XSize || y >= (uint8_t)YSize) return;

    old = cell[x][y];

    /*
     * Items and specials should not be erased by other dynamic sprites.
     * The player collects them explicitly in check_items().
     */
    if ((old == CELL_ITEM || old == CELL_SPECIAL) && what != old) {
        return;
    }

    if (old == what) return;

    if (old != CELL_EMPTY) _XL_DELETE(x, y);

    cell[x][y] = what;

    /* Platform/elevator are the permanent base layer. */
    if (what == CELL_PLATFORM || what == CELL_ELEVATOR) {
        base_cell[x][y] = what;
    }

    if (what != CELL_EMPTY) {
        uint8_t tile, color;

        switch (what) {
            case CELL_PLATFORM:
                tile = _TILE_0;
                color = _XL_CYAN;
                break;
            case CELL_ELEVATOR:
                tile = _TILE_0;
                color = _XL_GREEN;
                break;
            case CELL_PLAYER:
                tile = _TILE_5;
                color = _XL_WHITE;
                break;
            case CELL_ENEMY:
                tile = _TILE_4;
                color = _XL_RED;
                break;
            case CELL_ITEM:
                tile = _TILE_1;
                color = _XL_YELLOW;
                break;
            case CELL_SPECIAL:
                tile = _TILE_1;
                color = _XL_MAGENTA;
                break;
            case CELL_BULLET:
                tile = _TILE_6;
                color = _XL_RED;
                break;
            default:
                tile = _TILE_0;
                color = _XL_WHITE;
                break;
        }

        _XL_DRAW(x, y, tile, color);
    }
}

/*
 * clear_cell - erase a cell and restore base geometry underneath it.
 */
static void clear_cell(uint8_t x, uint8_t y) {
    if (x >= (uint8_t)XSize || y >= (uint8_t)YSize) return;

    if (cell[x][y] == CELL_EMPTY) return;

    cell[x][y] = CELL_EMPTY;
    _XL_DELETE(x, y);

    draw_base_cell(x, y);
}

/*
 * force_clear_cell - erase a collectible or other forced cell.
 */
static void force_clear_cell(uint8_t x, uint8_t y) {
    clear_cell(x, y);
}

/*
 * erase_entity_2x2 - erase only cells that actually contain the given type.
 *
 * This is important so enemies do not erase items: if an item blocked
 * drawing of the enemy on a cell, that cell still contains CELL_ITEM and
 * must be left untouched.
 */
static void erase_entity_cell(uint8_t x, uint8_t y, uint8_t type) {
    if (x >= (uint8_t)XSize || y >= (uint8_t)YSize) return;

    if (cell[x][y] == type) {
        cell[x][y] = CELL_EMPTY;
        _XL_DELETE(x, y);
        draw_base_cell(x, y);
    }
}

static void erase_entity_2x2(uint8_t x, uint8_t y, uint8_t type) {
    uint8_t y2;

    y2 = (uint8_t)(y + 1);
    if (y2 >= (uint8_t)YSize) y2 = (uint8_t)(YSize - 1);

    erase_entity_cell(x, y, type);
    erase_entity_cell((uint8_t)(x + 1), y, type);
    erase_entity_cell(x, y2, type);
    erase_entity_cell((uint8_t)(x + 1), y2, type);
}

/*
 * draw_2x2 - draw a 2x2 sprite.
 */
static void draw_2x2(uint8_t x, uint8_t y, uint8_t type) {
    uint8_t y2;

    y2 = (uint8_t)(y + 1);
    if (y2 >= (uint8_t)YSize) y2 = (uint8_t)(YSize - 1);

    set_cell(x, y, type);
    set_cell((uint8_t)(x + 1), y, type);
    set_cell(x, y2, type);
    set_cell((uint8_t)(x + 1), y2, type);
}

/* ---------- level helpers ---------- */

static uint8_t in_elevator_x(uint8_t xx) {
    uint8_t i;

    for (i = 0; i < elev_count; i++) {
        if (xx == elevs[i].x || xx == (uint8_t)(elevs[i].x + 1)) {
            return 1;
        }
    }

    return 0;
}

static uint8_t on_platform(uint8_t cx, uint8_t cy) {
    uint8_t i;

    for (i = 0; i < plat_count; i++) {
        if (cy == plats[i].y) {
            if (cx >= plats[i].x &&
                cx < (uint8_t)(plats[i].x + plats[i].width)) {
                return 1;
            }
        }
    }

    return 0;
}

static uint8_t enemy_on_platform(uint8_t e) {
    if (!enemies[e].active) return 0;

    return on_platform(enemies[e].x, (uint8_t)(enemies[e].y + 2)) ||
           on_platform((uint8_t)(enemies[e].x + 1), (uint8_t)(enemies[e].y + 2));
}

static void deactivate_enemy(uint8_t e) {
    if (!enemies[e].active) return;

    erase_entity_2x2(enemies[e].x, enemies[e].y, CELL_ENEMY);
    enemies[e].active = 0;
}

/*
 * in_elevator_shaft - returns elevator index or 0xFF.
 */
static uint8_t in_elevator_shaft(uint8_t cx, uint8_t cy) {
    uint8_t i;

    for (i = 0; i < elev_count; i++) {
        if ((cx == elevs[i].x || cx == (uint8_t)(elevs[i].x + 1)) &&
            cy > elevs[i].y_top && cy <= elevs[i].y_bot) {
            return i;
        }
    }

    return 0xFF;
}

/* ---------- level generation ---------- */

static void gen_level(void) {
    uint8_t i, j;
    uint8_t min_w;
    uint8_t gap;
    uint8_t max_gap;
    uint8_t w;
    uint8_t max_x;
    uint8_t nx;
    uint8_t below_l, below_r;
    uint8_t new_l, new_r;
    uint8_t ol, orr;
    uint8_t ex;
    uint8_t cnt;
    uint8_t ix, sx, exx;
    uint8_t tries;

    min_w = (uint8_t)(XSize / 4);
    if (min_w < 8) min_w = 8;
    if (min_w > (uint8_t)(XSize / 2)) min_w = (uint8_t)(XSize / 2);

    plat_count = 0;
    elev_count = 0;
    enemy_count = 0;
    item_count = 0;
    total_items = 0;
    items_collected = 0;

    /* Clear dynamic objects from previous levels. */
    for (i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = 0;
    }
    for (i = 0; i < MAX_ITEMS; i++) {
        items[i].active = 0;
    }
    for (i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }

    /* Bottom platform. */
    plats[0].x = 0;
    plats[0].y = (uint8_t)(YSize - 1);
    plats[0].width = (uint8_t)XSize;
    plat_count = 1;

    /*
     * Upper platforms.
     *
     * Ladders are made shorter by using a smaller vertical gap between
     * adjacent platforms:
     *   old gaps were about 14..22 cells
     *   new gaps are about 6..9 cells
     */
    for (i = 1; i < MAX_PLATFORMS; i++) {
        max_gap = 9;

        if ((uint8_t)(plats[i - 1].y - 5) > max_gap) {
            gap = rnd(6, max_gap);
        } else {
            gap = (uint8_t)(plats[i - 1].y - 4);
            if (gap < 3) break;
        }

        w = rnd(min_w, (uint8_t)(XSize / 2));
        if (w > (uint8_t)XSize) w = (uint8_t)XSize;

        max_x = (uint8_t)(XSize - w);
        nx = rnd(0, max_x);

        below_l = plats[i - 1].x;
        below_r = (uint8_t)(plats[i - 1].x + plats[i - 1].width - 1);
        new_l = nx;
        new_r = (uint8_t)(nx + w - 1);

        /* Ensure at least 4 cells of horizontal overlap. */
        if (new_r < (uint8_t)(below_l + 3) ||
            new_l > (uint8_t)(below_r - 3)) {

            ol = (new_l > below_l) ? new_l : below_l;
            orr = (new_r < below_r) ? new_r : below_r;

            if (orr < ol || (orr - ol) < 4) {
                if ((uint8_t)(below_r - below_l) >= 2) {
                    nx = (uint8_t)(below_l +
                                   rnd(0, (uint8_t)(below_r - below_l - 2)));
                } else {
                    nx = below_l;
                }

                if ((uint8_t)(nx + w) > (uint8_t)XSize) {
                    nx = (uint8_t)(XSize - w);
                }

                new_l = nx;
                new_r = (uint8_t)(nx + w - 1);
            }
        }

        plats[i].x = nx;
        plats[i].y = (uint8_t)(plats[i - 1].y - gap);
        plats[i].width = w;
        plat_count++;

        /* Place elevator in the overlapping region. */
        {
            ol = (new_l > below_l) ? new_l : below_l;
            orr = (new_r < below_r) ? new_r : below_r;

            if (orr >= ol && (orr - ol) >= 4) {
                ex = rnd(ol, (uint8_t)(orr - 2));
            } else {
                ex = ol;
            }

            elevs[elev_count].x = ex;
            elevs[elev_count].y_top = plats[i].y;
            elevs[elev_count].y_bot = plats[i - 1].y;
            elev_count++;
        }

        /* Stop once at least three platforms exist and height is low. */
        if (plat_count >= MIN_PLATFORMS &&
            plats[plat_count - 1].y <= 8) {
            break;
        }
    }

    /* Guarantee at least three platforms when screen height allows it. */
    while (plat_count < MIN_PLATFORMS) {
        uint8_t new_y = (uint8_t)(plats[plat_count - 1].y - 4);

        if (new_y < 2) break;

        plats[plat_count].x = plats[0].x;
        plats[plat_count].width = plats[0].width;
        plats[plat_count].y = new_y;
        plat_count++;
    }

    /* Draw platforms. */
    for (i = 0; i < plat_count; i++) {
        for (j = 0; j < plats[i].width; j++) {
            set_cell((uint8_t)(plats[i].x + j), plats[i].y, CELL_PLATFORM);
        }
    }

    /* Draw shorter elevators. */
    for (i = 0; i < elev_count; i++) {
        for (j = (uint8_t)(elevs[i].y_top + 1); j < elevs[i].y_bot; j++) {
            set_cell(elevs[i].x, j, CELL_ELEVATOR);
            set_cell((uint8_t)(elevs[i].x + 1), j, CELL_ELEVATOR);
        }
    }

    /* Place items: at least one per platform. */
    for (i = 0; i < plat_count; i++) {
        cnt = (i == 0) ? 1 : rnd(1, 2);

        for (j = 0; j < cnt; j++) {
            tries = 0;
            ix = plats[i].x;

            /* Prefer positions not inside elevator columns. */
            while (tries < 8 && in_elevator_x(ix)) {
                ix = rnd(plats[i].x,
                         (uint8_t)(plats[i].x + plats[i].width - 1));
                tries++;
            }

            items[item_count].x = ix;
            items[item_count].y = (uint8_t)(plats[i].y - 1);
            items[item_count].type = 0;
            items[item_count].active = 1;

            set_cell(items[item_count].x, items[item_count].y, CELL_ITEM);
            item_count++;
            total_items++;
        }
    }

    /* Place one special invincibility item on an upper platform. */
    if (plat_count > 1) {
        uint8_t sp;

        sp = rnd(1, (uint8_t)(plat_count - 1));

        tries = 0;
        sx = plats[sp].x;

        while (tries < 8 && in_elevator_x(sx)) {
            sx = rnd(plats[sp].x,
                     (uint8_t)(plats[sp].x + plats[sp].width - 1));
            tries++;
        }

        items[item_count].x = sx;
        items[item_count].y = (uint8_t)(plats[sp].y - 1);
        items[item_count].type = 1;
        items[item_count].active = 1;

        set_cell(items[item_count].x, items[item_count].y, CELL_SPECIAL);
        item_count++;
        total_items++;
    }

    /*
     * Place exactly one enemy on every platform except the bottom one.
     *
     * The important fix is that enemies[enemy_count].active must be set
     * to 1 before calling enemy_on_platform(), because that helper first
     * checks whether the enemy is active.
     */
    for (i = 1; i < plat_count; i++) {
        tries = 0;

        if (plats[i].width < 4) {
            exx = plats[i].x;
        } else {
            exx = rnd(plats[i].x,
                      (uint8_t)(plats[i].x + plats[i].width - 2));

            /* Prefer positions not inside elevator columns. */
            while (tries < 8 && in_elevator_x(exx)) {
                exx = rnd(plats[i].x,
                          (uint8_t)(plats[i].x + plats[i].width - 2));
                tries++;
            }
        }

        enemies[enemy_count].x = exx;
        enemies[enemy_count].y = (uint8_t)(plats[i].y - 2);
        enemies[enemy_count].dir = (uint8_t)(_XL_RAND() & 1U);
        enemies[enemy_count].plat = i;

        /* Mark active before validating the position. */
        enemies[enemy_count].active = 1;

        if (enemy_on_platform(enemy_count)) {
            draw_2x2(exx, enemies[enemy_count].y, CELL_ENEMY);
        } else {
            enemies[enemy_count].active = 0;
        }

        enemy_count++;
    }
}

/* ---------- player drawing ---------- */

static void draw_player(void) {
    draw_2x2(px, py, CELL_PLAYER);
}

static void erase_player(void) {
    erase_entity_2x2(px, py, CELL_PLAYER);
}

/* ---------- bullets ---------- */

static void fire_bullet(void) {
    uint8_t i;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = (uint8_t)(px + 1);
            bullets[i].y = py;
            bullets[i].active = 1;

            set_cell(bullets[i].x, bullets[i].y, CELL_BULLET);
            _XL_SHOOT_SOUND();
            return;
        }
    }
}

static void move_bullets(void) {
    uint8_t i, e;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;

        /* Erase only if this cell still belongs to the bullet. */
        erase_entity_cell(bullets[i].x, bullets[i].y, CELL_BULLET);

        bullets[i].y--;

        if (bullets[i].y == 0) {
            bullets[i].active = 0;
            continue;
        }

        set_cell(bullets[i].x, bullets[i].y, CELL_BULLET);

        /* Hit enemy. */
        for (e = 0; e < enemy_count; e++) {
            if (!enemies[e].active) continue;

            if ((bullets[i].x == enemies[e].x ||
                 bullets[i].x == (uint8_t)(enemies[e].x + 1)) &&
                (bullets[i].y == enemies[e].y ||
                 bullets[i].y == (uint8_t)(enemies[e].y + 1))) {

                erase_entity_2x2(enemies[e].x, enemies[e].y, CELL_ENEMY);
                enemies[e].active = 0;

                /* Erase only if the bullet cell is still a bullet. */
                erase_entity_cell(bullets[i].x, bullets[i].y, CELL_BULLET);
                bullets[i].active = 0;

                score += 50U;
                _XL_PING_SOUND();
                break;
            }
        }
    }
}

/* ---------- enemies ---------- */

static void move_enemies(void) {
    uint8_t i, nx;
    uint8_t p;
    uint8_t plat_l, plat_r;

    for (i = 0; i < enemy_count; i++) {
        if (!enemies[i].active) continue;

        /* Enemies not on a platform should not be active. */
        if (!enemy_on_platform(i)) {
            deactivate_enemy(i);
            continue;
        }

        p = enemies[i].plat;
        plat_l = plats[p].x;
        plat_r = (uint8_t)(plats[p].x + plats[p].width - 1);

        nx = enemies[i].x;

        if (enemies[i].dir == 0) {
            if (nx > plat_l) {
                nx--;
            } else {
                enemies[i].dir = 1;
            }
        } else {
            if ((uint8_t)(nx + 1) < plat_r) {
                nx++;
            } else {
                enemies[i].dir = 0;
            }
        }

        /*
         * Erase old enemy position using type-aware clearing.
         * This prevents the enemy from erasing items or special cells
         * that blocked its sprite on the previous frame.
         */
        erase_entity_2x2(enemies[i].x, enemies[i].y, CELL_ENEMY);

        enemies[i].x = nx;
        draw_2x2(nx, enemies[i].y, CELL_ENEMY);
    }
}

/* ---------- player collisions ---------- */

static uint8_t player_hit_enemy(void) {
    uint8_t i;

    for (i = 0; i < enemy_count; i++) {
        if (!enemies[i].active) continue;

        if (px < (uint8_t)(enemies[i].x + 2) &&
            (uint8_t)(px + 2) > enemies[i].x &&
            py < (uint8_t)(enemies[i].y + 2) &&
            (uint8_t)(py + 2) > enemies[i].y) {
            return 1;
        }
    }

    return 0;
}

/* Call before drawing the player so items can be collected cleanly. */
static void check_items(void) {
    uint8_t i;

    for (i = 0; i < item_count; i++) {
        if (!items[i].active) continue;

        if ((items[i].x == px || items[i].x == (uint8_t)(px + 1)) &&
            (items[i].y == py || items[i].y == (uint8_t)(py + 1))) {

            if (cell[items[i].x][items[i].y] == CELL_ITEM ||
                cell[items[i].x][items[i].y] == CELL_SPECIAL) {
                force_clear_cell(items[i].x, items[i].y);
            }

            items[i].active = 0;
            items_collected++;

            if (items[i].type == 1) {
                invincible = 1;
                inv_timer = INVINC_FRAMES;
                _XL_TOCK_SOUND();
            } else {
                score += 10U;
                _XL_TICK_SOUND();
            }
        }
    }
}

/* ---------- HUD ---------- */

static void draw_hud(void) {
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "S:");
    _XL_PRINTD(2, 0, 4, score);
    _XL_PRINT(8, 0, "L:");
    _XL_PRINTD(10, 0, 1, lives);
    _XL_PRINT(14, 0, "I:");
    _XL_PRINTD(16, 0, 2, items_collected);
    _XL_PRINTD(18, 0, 2, (uint16_t)total_items);

    if (invincible) {
        _XL_SET_TEXT_COLOR(_XL_MAGENTA);
        _XL_PRINT(22, 0, "INV");
    }
}

/* ---------- reset / start ---------- */

static void reset_player(void) {
    uint8_t i;

    px = (uint8_t)(XSize / 2);
    py = (uint8_t)(plats[0].y - 2);
    pdir = 1;
    jumping = 0;
    jump_count = 0;
    in_elev = 0;
    invincible = 0;
    inv_timer = 0;

    erase_player();

    for (i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            /* Type-aware clear so we do not remove other objects. */
            erase_entity_cell(bullets[i].x, bullets[i].y, CELL_BULLET);
            bullets[i].active = 0;
        }
    }

    draw_player();
}

static void start_level(void) {
    uint8_t i, j;

    for (i = 0; i < XSize; i++) {
        for (j = 0; j < YSize; j++) {
            cell[i][j] = CELL_EMPTY;
            base_cell[i][j] = CELL_EMPTY;
        }
    }

    _XL_CLEAR_SCREEN();

    score = 0U;
    lives = MAX_LIVES;
    game_state = 0;
    frame = 0U;

    gen_level();
    reset_player();
    draw_hud();
}

/* ---------- main game loop ---------- */

static void game_loop(void) {
    uint8_t input;
    uint8_t grounded;
    uint8_t elev;

    /* Player starts on the bottom platform. */
    grounded = 1;

    while (game_state == 0) {
        input = _XL_INPUT();

        /* Collect items before drawing/moving the player. */
        check_items();

        /* Horizontal movement. */
        if (_XL_LEFT(input)) {
            if (px > 0) {
                erase_player();
                px--;
                pdir = 0;
                draw_player();
            }
        } else if (_XL_RIGHT(input)) {
            if ((uint8_t)(px + 1) < (uint8_t)(XSize - 1)) {
                erase_player();
                px++;
                pdir = 1;
                draw_player();
            }
        }

        /* Jump only when grounded and not in elevator. */
        if (_XL_UP(input) && !jumping && grounded && !in_elev) {
            jumping = 1;
            jump_count = JUMP_HEIGHT;
        }

        /* Fire. */
        if (_XL_FIRE(input)) {
            fire_bullet();
        }

        /* Elevator logic. */
        in_elev = 0;
        elev = in_elevator_shaft(px, (uint8_t)(py + 2));

        if (elev != 0xFF) {
            in_elev = 1;
            elev_idx = elev;

            if (_XL_UP(input)) {
                if ((uint8_t)(py + 2) > elevs[elev_idx].y_top) {
                    erase_player();
                    py--;
                    draw_player();
                }
            } else if (_XL_DOWN(input)) {
                if ((uint8_t)(py + 2) < elevs[elev_idx].y_bot) {
                    erase_player();
                    py++;
                    draw_player();
                }
            }

            jumping = 0;
        } else {
            /* Gravity / jump. */
            if (jumping) {
                if (jump_count > 0) {
                    erase_player();
                    py--;
                    jump_count--;
                    draw_player();
                } else {
                    jumping = 0;
                }
            }

            grounded = 0;
            if (on_platform(px, (uint8_t)(py + 2)) ||
                on_platform((uint8_t)(px + 1), (uint8_t)(py + 2))) {
                grounded = 1;
            }

            if (!grounded && !jumping) {
                erase_player();
                py++;
                draw_player();

                /* Fell off the bottom of the screen. */
                if (py >= YSize) {
                    lives--;

                    if (lives == 0) {
                        game_state = 2;
                        return;
                    }

                    reset_player();
                    draw_hud();

                    /* Stay in this level after losing a life. */
                    continue;
                }
            }
        }

        /* Recompute grounded for next frame. */
        if (!in_elev) {
            grounded = 0;
            if (on_platform(px, (uint8_t)(py + 2)) ||
                on_platform((uint8_t)(px + 1), (uint8_t)(py + 2))) {
                grounded = 1;
            }
        }

        move_enemies();
        move_bullets();

        /* Player vs enemy. */
        if (invincible) {
            inv_timer--;
            if (inv_timer == 0U) invincible = 0;
        } else if (player_hit_enemy()) {
            lives--;

            if (lives == 0) {
                game_state = 2;
                return;
            }

            reset_player();
            draw_hud();

            /* Stay in this level after losing a life. */
            continue;
        }

        /* Win check. */
        if (items_collected >= total_items) {
            game_state = 3;
            return;
        }

        draw_hud();
        frame++;
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

/* ---------- main ---------- */

int main(void) {
    (void)_XL_INPUT();

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        /* Title screen. */
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(12, 20, "PLATFORM");
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(8, 30, "COLLECT ALL ITEMS");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(8, 38, "PRESS FIRE TO START");
        _XL_WAIT_FOR_INPUT();

        start_level();
        game_loop();

        if (game_state == 3) {
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_GREEN);
            _XL_PRINT(10, 25, "LEVEL CLEAR!");
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(10, 35, "SCORE: ");
            _XL_PRINTD(17, 35, 4, score);
            _XL_PRINT(8, 45, "PRESS FIRE TO CONTINUE");
            _XL_WAIT_FOR_INPUT();

            start_level();
            game_loop();
        }

        if (game_state == 2) {
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(12, 25, "GAME OVER");
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(10, 35, "SCORE: ");
            _XL_PRINTD(17, 35, 4, score);
            _XL_PRINT(8, 45, "PRESS FIRE TO RESTART");
            _XL_WAIT_FOR_INPUT();
        }
    }

    return 0;
}
