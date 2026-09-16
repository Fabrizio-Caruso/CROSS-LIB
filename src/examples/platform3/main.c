#include "cross_lib.h"

#define MAX_PLATFORMS  8
#define MAX_ELEVATORS  7
#define MAX_ENEMIES    16
#define MAX_ITEMS      20
#define MAX_BULLETS    5
#define MAX_LIVES      3
#define INVINC_FRAMES  240
#define JUMP_HEIGHT    5
#define MAX_FALL_SPEED 1

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
static uint8_t   cell[160][160];

/* Player state (2x2 sprite, top-left at px,py) */
static uint8_t  px, py;
static uint8_t  pdir;          /* 0=left, 1=right */
static uint8_t  jumping;
static uint8_t  jump_count;
static uint8_t  in_elev;
static uint8_t  elev_idx;
static uint8_t  invincible;
static uint16_t inv_timer;

static uint16_t score;
static uint8_t  lives;
static uint8_t  game_state;    /* 0=playing, 1=dead_pause, 2=game_over, 3=level_complete */
static uint16_t frame;
static uint8_t  items_collected;
static uint8_t  total_items;
static uint8_t  plat_count;
static uint8_t  elev_count;
static uint8_t  enemy_count;
static uint8_t  item_count;

/* ---------- helpers ---------- */
static uint8_t rnd(uint8_t lo, uint8_t hi) {
    if (hi <= lo) return lo;
    return lo + (uint8_t)(_XL_RAND() % (uint16_t)(hi - lo));
}

/*
 * set_cell – write a cell.
 *  - Static cells (PLATFORM, ELEVATOR) are never overwritten.
 *  - Item / Special cells are never overwritten by PLAYER or ENEMY.
 *  - Dynamic cells may be overwritten by other dynamic cells.
 */
static void set_cell(uint8_t x, uint8_t y, uint8_t what) {
    if (x >= (uint8_t)XSize || y >= (uint8_t)YSize) return;

    uint8_t old = cell[x][y];

    /* Protect static geometry from being clobbered */
    if (old == CELL_PLATFORM || old == CELL_ELEVATOR) return;

    /* Protect items from player / enemy sprites */
    if ((what == CELL_PLAYER || what == CELL_ENEMY) &&
        (old == CELL_ITEM || old == CELL_SPECIAL)) return;

    if (old != what) {
        if (old != CELL_EMPTY) _XL_DELETE(x, y);
        cell[x][y] = what;
        if (what != CELL_EMPTY) {
            uint8_t tile, color;
            switch (what) {
                case CELL_PLATFORM: tile=_TILE_0; color=_XL_CYAN;
                    break;
                case CELL_ELEVATOR: tile=_TILE_0; color=_XL_GREEN;
                    break;
                case CELL_PLAYER:   tile=_TILE_5; color=_XL_WHITE;
                    break;
                case CELL_ENEMY:    tile=_TILE_4; color=_XL_RED;
                    break;
                case CELL_ITEM:     tile=_TILE_1; color=_XL_YELLOW;
                    break;
                case CELL_SPECIAL:  tile=_TILE_1; color=_XL_MAGENTA;
                    break;
                case CELL_BULLET:   tile=_TILE_6; color=_XL_RED;
                    break;
                default:            tile=_TILE_0; color=_XL_WHITE;
                    break;
            }
            _XL_DRAW(x, y, tile, color);
        }
    }
}

/*
 * clear_cell – erase a cell.
 *  - Static cells (PLATFORM, ELEVATOR) are never cleared.
 *  - Everything else may be cleared.
 */
static void clear_cell(uint8_t x, uint8_t y) {
    if (x >= (uint8_t)XSize || y >= (uint8_t)YSize) return;
    uint8_t cur = cell[x][y];
    if (cur == CELL_PLATFORM || cur == CELL_ELEVATOR) return;
    if (cur != CELL_EMPTY) {
        cell[x][y] = CELL_EMPTY;
        _XL_DELETE(x, y);
    }
}

/* Force-clear a cell (used when the player legitimately collects an item). */
static void force_clear_cell(uint8_t x, uint8_t y) {
    if (x >= (uint8_t)XSize || y >= (uint8_t)YSize) return;
    if (cell[x][y] != CELL_EMPTY) {
        cell[x][y] = CELL_EMPTY;
        _XL_DELETE(x, y);
    }
}

static void draw_2x2(uint8_t x, uint8_t y, uint8_t type) {
    set_cell(x, y, type);
    set_cell((uint8_t)(x+1), y, type);
    set_cell(x, (uint8_t)(y+1), type);
    set_cell((uint8_t)(x+1), (uint8_t)(y+1), type);
}

static void erase_2x2(uint8_t x, uint8_t y) {
    clear_cell(x, y);
    clear_cell((uint8_t)(x+1), y);
    clear_cell(x, (uint8_t)(y+1));
    clear_cell((uint8_t)(x+1), (uint8_t)(y+1));
}

/* ---------- level generation ---------- */
static void gen_level(void) {
    uint8_t i, j;
    uint8_t min_w;

    min_w = (uint8_t)(XSize / 4);
    if (min_w < 8) min_w = 8;
    if (min_w > (uint8_t)(XSize / 2)) min_w = (uint8_t)(XSize / 2);

    plat_count = 0;
    elev_count = 0;
    enemy_count = 0;
    item_count = 0;
    total_items = 0;
    items_collected = 0;

    /* --- Bottom platform (full width, no enemies) --- */
    plats[0].x = 0;
    plats[0].y = (uint8_t)(YSize - 1);
    plats[0].width = (uint8_t)XSize;
    plat_count = 1;

    /* --- Generate upper platforms with guaranteed overlap --- */
    for (i = 1; i < MAX_PLATFORMS; i++) {
        uint8_t gap = rnd(14, 22);
        uint8_t new_y = plats[i-1].y - gap;
        if (new_y < 5) break;

        uint8_t w = rnd(min_w, (uint8_t)(XSize / 2));
        if (w > (uint8_t)XSize) w = (uint8_t)XSize;
        uint8_t max_x = (uint8_t)(XSize - w);
        uint8_t nx = rnd(0, max_x);

        /* Ensure at least 4-cell overlap with platform below */
        uint8_t below_l = plats[i-1].x;
        uint8_t below_r = (uint8_t)(plats[i-1].x + plats[i-1].width - 1);
        uint8_t new_l = nx;
        uint8_t new_r = (uint8_t)(nx + w - 1);

        if (new_r < (uint8_t)(below_l + 3) || new_l > (uint8_t)(below_r - 3)) {
            uint8_t ol = (new_l > below_l) ? new_l : below_l;
            uint8_t orr = (new_r < below_r) ? new_r : below_r;
            if (orr - ol >= 4) {
                /* Already overlapping enough */
            } else {
                nx = (uint8_t)(below_l + rnd(0, (uint8_t)(below_r - below_l - 2)));
                if ((uint8_t)(nx + w) > (uint8_t)XSize) nx = (uint8_t)(XSize - w);
                new_l = nx;
                new_r = (uint8_t)(nx + w - 1);
            }
        }

        plats[i].x = nx;
        plats[i].y = new_y;
        plats[i].width = w;
        plat_count++;

        /* Place elevator in the overlap zone */
        {
            uint8_t ol = (new_l > below_l) ? new_l : below_l;
            uint8_t orr = (new_r < below_r) ? new_r : below_r;
            uint8_t ex;
            if (orr - ol >= 4) {
                ex = rnd(ol, (uint8_t)(orr - 2));
            } else {
                ex = ol;
            }
            elevs[elev_count].x = ex;
            elevs[elev_count].y_top = new_y;
            elevs[elev_count].y_bot = plats[i-1].y;
            elev_count++;
        }
    }

    /* --- Draw platforms --- */
    for (i = 0; i < plat_count; i++) {
        for (j = 0; j < plats[i].width; j++) {
            set_cell((uint8_t)(plats[i].x + j), plats[i].y, CELL_PLATFORM);
        }
    }

    /* --- Draw elevators (2-wide columns between platforms) --- */
    for (i = 0; i < elev_count; i++) {
        for (j = (uint8_t)(elevs[i].y_top + 1); j < elevs[i].y_bot; j++) {
            set_cell(elevs[i].x, j, CELL_ELEVATOR);
            set_cell((uint8_t)(elevs[i].x + 1), j, CELL_ELEVATOR);
        }
    }

    /* --- Place items: at least 1 per platform --- */
    for (i = 0; i < plat_count; i++) {
        uint8_t cnt = (i == 0) ? 1 : rnd(1, 2);
        for (j = 0; j < cnt; j++) {
            items[item_count].x = rnd(plats[i].x, (uint8_t)(plats[i].x + plats[i].width - 1));
            items[item_count].y = (uint8_t)(plats[i].y - 1);
            items[item_count].type = 0;
            items[item_count].active = 1;
            set_cell(items[item_count].x, items[item_count].y, CELL_ITEM);
            item_count++;
            total_items++;
        }
    }

    /* --- Place one special (invincibility) item on a random upper platform --- */
    if (plat_count > 1) {
        uint8_t sp = rnd(1, (uint8_t)(plat_count - 1));
        items[item_count].x = rnd(plats[sp].x, (uint8_t)(plats[sp].x + plats[sp].width - 1));
        items[item_count].y = (uint8_t)(plats[sp].y - 1);
        items[item_count].type = 1;
        items[item_count].active = 1;
        set_cell(items[item_count].x, items[item_count].y, CELL_SPECIAL);
        item_count++;
        total_items++;
    }

    /* --- Place enemies: at least 1 on every platform except the bottom --- */
    for (i = 1; i < plat_count; i++) {
        uint8_t cnt = rnd(1, 2);
        for (j = 0; j < cnt; j++) {
            uint8_t ex;
            if (plats[i].width < 4) {
                ex = plats[i].x;
            } else {
                ex = rnd(plats[i].x, (uint8_t)(plats[i].x + plats[i].width - 2));
            }
            enemies[enemy_count].x = ex;
            enemies[enemy_count].y = (uint8_t)(plats[i].y - 2);
            enemies[enemy_count].dir = (uint8_t)(_XL_RAND() & 1);
            enemies[enemy_count].plat = i;
            enemies[enemy_count].active = 1;
            draw_2x2(ex, enemies[enemy_count].y, CELL_ENEMY);
            enemy_count++;
        }
    }
}

/* ---------- player drawing ---------- */
static void draw_player(void) {
    draw_2x2(px, py, CELL_PLAYER);
}

static void erase_player(void) {
    erase_2x2(px, py);
}

/* ---------- check if player is on a platform ---------- */
static uint8_t on_platform(uint8_t cx, uint8_t cy) {
    uint8_t i;
    for (i = 0; i < plat_count; i++) {
        if (cy == plats[i].y) {
            if (cx >= plats[i].x && cx < (uint8_t)(plats[i].x + plats[i].width))
                return 1;
        }
    }
    return 0;
}

/*
 * in_elevator_shaft – returns the elevator index or 0xFF.
 *
 * The player is "in the elevator" when:
 *   - their x is within the 2-wide elevator column, AND
 *   - their feet row (cy = py+2) is strictly above y_top (not yet at the
 *     upper platform) and at or below y_bot (so they can enter from the
 *     lower platform).
 *
 * This lets the player walk in from the lower platform (cy == y_bot) and
 * exit at the upper platform (cy == y_top → no longer in elevator).
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

/* ---------- fire bullet ---------- */
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

/* ---------- move bullets ---------- */
static void move_bullets(void) {
    uint8_t i, e;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;

        clear_cell(bullets[i].x, bullets[i].y);
        bullets[i].y--;
        if (bullets[i].y == 0) {
            bullets[i].active = 0;
            continue;
        }
        set_cell(bullets[i].x, bullets[i].y, CELL_BULLET);

        /* Hit enemy */
        for (e = 0; e < enemy_count; e++) {
            if (!enemies[e].active) continue;
            if ((bullets[i].x == enemies[e].x ||
                 bullets[i].x == (uint8_t)(enemies[e].x + 1)) &&
                (bullets[i].y == enemies[e].y ||
                 bullets[i].y == (uint8_t)(enemies[e].y + 1))) {
                erase_2x2(enemies[e].x, enemies[e].y);
                enemies[e].active = 0;
                clear_cell(bullets[i].x, bullets[i].y);
                bullets[i].active = 0;
                score += 50;
                _XL_PING_SOUND();
                break;
            }
        }
    }
}

/* ---------- move enemies (patrol) ---------- */
static void move_enemies(void) {
    uint8_t i;
    for (i = 0; i < enemy_count; i++) {
        if (!enemies[i].active) continue;

        {
            uint8_t nx = enemies[i].x;
            uint8_t p = enemies[i].plat;
            uint8_t plat_l = plats[p].x;
            uint8_t plat_r = (uint8_t)(plats[p].x + plats[p].width - 1);

            if (enemies[i].dir == 0) {
                if (nx > plat_l) nx--;
                else enemies[i].dir = 1;
            } else {
                if ((uint8_t)(nx + 1) < plat_r) nx++;
                else enemies[i].dir = 0;
            }

            erase_2x2(enemies[i].x, enemies[i].y);
            enemies[i].x = nx;
            draw_2x2(nx, enemies[i].y, CELL_ENEMY);
        }
    }
}

/* ---------- player vs enemy collision ---------- */
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

/* ---------- player vs item collision (call BEFORE player draws) ---------- */
static void check_items(void) {
    uint8_t i;
    for (i = 0; i < item_count; i++) {
        if (!items[i].active) continue;
        if ((items[i].x == px || items[i].x == (uint8_t)(px+1)) &&
            (items[i].y == py || items[i].y == (uint8_t)(py+1))) {
            /* Only clear if the cell still holds the item (not already
               overwritten by a dynamic entity). */
            uint8_t c = cell[items[i].x][items[i].y];
            if (c == CELL_ITEM || c == CELL_SPECIAL)
                force_clear_cell(items[i].x, items[i].y);
            items[i].active = 0;
            items_collected++;
            if (items[i].type == 1) {
                invincible = 1;
                inv_timer = INVINC_FRAMES;
                _XL_TOCK_SOUND();
            } else {
                score += 10;
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

/* ---------- start / reset player ---------- */
static void reset_player(void) {
    uint8_t i;
    px = (uint8_t)(XSize / 2);
    py = plats[0].y - 2;
    pdir = 1;
    jumping = 0;
    jump_count = 0;
    in_elev = 0;
    invincible = 0;
    inv_timer = 0;

    erase_player();
    for (i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            clear_cell(bullets[i].x, bullets[i].y);
            bullets[i].active = 0;
        }
    }
    draw_player();
}

/* ---------- start level ---------- */
static void start_level(void) {
    uint8_t i, j;

    for (i = 0; i < XSize; i++)
        for (j = 0; j < YSize; j++)
            cell[i][j] = CELL_EMPTY;
    _XL_CLEAR_SCREEN();

    score = 0;
    lives = MAX_LIVES;
    game_state = 0;
    frame = 0;

    gen_level();
    reset_player();
    draw_hud();
}

/* ---------- main game loop ---------- */
static void game_loop(void) {
    uint8_t input;
    uint8_t grounded;
    uint8_t elev;

    while (game_state == 0) {
        input = _XL_INPUT();

        /* --- Collect items BEFORE player draws (prevents cell conflict) --- */
        check_items();

        /* --- Player horizontal movement (allowed even in elevator so
              the player can walk in / out) --- */
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

        /* --- Jump (only when grounded and NOT in elevator) --- */
        if (_XL_UP(input) && !jumping && grounded && !in_elev) {
            jumping = 1;
            jump_count = JUMP_HEIGHT;
        }

        /* --- Fire --- */
        if (_XL_FIRE(input)) {
            fire_bullet();
        }

        /* --- Elevator logic ---
         *
         * The player is in the elevator when their feet (py+2) are
         * strictly above y_top and at-or-below y_bot.
         *   • Entry:  standing on lower platform → py+2 == y_bot  ✓
         *   • Exit:   reaching upper platform   → py+2 == y_top  ✗ (no longer in elevator)
         *
         * Up:   py+2 > y_top   → can still rise
         * Down: py+2 < y_bot   → can still descend
         */
        in_elev = 0;
        elev = in_elevator_shaft(px, (uint8_t)(py + 2));
        if (elev != 0xFF) {
            in_elev = 1;
            elev_idx = elev;

            if (_XL_UP(input)) {
                /* Rise until feet reach the upper platform (py+2 == y_top) */
                if ((uint8_t)(py + 2) > elevs[elev_idx].y_top) {
                    erase_player();
                    py--;
                    draw_player();
                }
            } else if (_XL_DOWN(input)) {
                /* Descend until feet reach the lower platform (py+2 == y_bot) */
                if ((uint8_t)(py + 2) < elevs[elev_idx].y_bot) {
                    erase_player();
                    py++;
                    draw_player();
                }
            }
            /* No gravity while in elevator */
            jumping = 0;
        } else {
            /* --- Gravity / Jumping --- */
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

            /* Check grounded: is there a platform at py+2? */
            grounded = 0;
            if (on_platform(px, (uint8_t)(py + 2)) ||
                on_platform((uint8_t)(px + 1), (uint8_t)(py + 2))) {
                grounded = 1;
            }

            if (!grounded && !jumping) {
                /* Fall */
                erase_player();
                py++;
                draw_player();

                /* Fell off screen */
                if (py >= YSize) {
                    lives--;
                    if (lives == 0) {
                        game_state = 2;
                        return;
                    }
                    reset_player();
                    draw_hud();
                    return;
                }
            }
        }

        /* Recompute grounded for next frame if not in elevator */
        if (!in_elev) {
            grounded = 0;
            if (on_platform(px, (uint8_t)(py + 2)) ||
                on_platform((uint8_t)(px + 1), (uint8_t)(py + 2))) {
                grounded = 1;
            }
        }

        /* --- Move enemies --- */
        move_enemies();

        /* --- Move bullets --- */
        move_bullets();

        /* --- Player vs enemy --- */
        if (invincible) {
            inv_timer--;
            if (inv_timer == 0) invincible = 0;
        } else if (player_hit_enemy()) {
            lives--;
            if (lives == 0) {
                game_state = 2;
                return;
            }
            reset_player();
            draw_hud();
            return;
        }

        /* --- Check win --- */
        if (items_collected >= total_items) {
            game_state = 3;
            return;
        }

        /* --- HUD --- */
        draw_hud();

        frame++;
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

/* ---------- main ---------- */
int main(void) {
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        /* Title screen */
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