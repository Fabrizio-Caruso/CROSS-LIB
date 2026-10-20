#include "cross_lib.h"

/* ===== Layout ===== */
#define BAR_ROW0 2
#define BAR_GAP 3
#define KEG_X0 0
#define KEG_X1 1
#define SURF_X0 2
#define DOOR_X0_OFFSET 2

#define BAR_COUNT 4
#define MAX_CUST 4
#define MAX_MUGS 4
#define MAX_TIPS 4

/* ===== States ===== */
#define CUST_WALK 0
#define CUST_DRINK 1
#define CUST_SLIDE_BACK 2
#define CUST_SLIDE_MUG 3
#define CUST_DIZZY 4

#define BT_NORMAL 0
#define BT_FILLING 1

#define GS_PLAYING 0
#define GS_CLEAR 1
#define GS_OVER 2

/* ===== Timing ===== */
#define TICK_RATE 20
#define DANCER_DUR 120
#define TIP_LIFE 250
#define CLEAR_DUR 90

/* ===== Structs ===== */
typedef struct {
    uint8_t active;
    uint8_t x;
    uint8_t state;
    uint8_t timer;
    uint8_t steps_left;
} Customer;

typedef struct {
    uint8_t active;
    uint8_t x;
    uint8_t full;
    uint8_t dir;
    uint8_t timer;
} Mug;

typedef struct {
    uint8_t active;
    uint8_t x;
    uint8_t timer;
} Tip;

/* ===== Globals ===== */
uint16_t g_score;
uint8_t g_lives;
uint8_t g_level;
uint8_t g_state;
uint8_t g_bart_bar;
uint8_t g_bart_x;
uint8_t g_bart_state;
uint8_t g_dancers;
uint8_t g_dancer_timer;
uint8_t g_clear_timer;
uint8_t g_tick;
uint8_t g_fire_was;
uint8_t g_spawn_timer;
uint8_t g_max_cust;
uint8_t g_spawned;

uint16_t g_prev_score;
uint8_t g_prev_lives;
uint8_t g_prev_level;
uint8_t g_prev_bart_bar;
uint8_t g_prev_bart_x;
uint8_t g_prev_bart_state;
uint8_t g_prev_dancers;

Customer g_cust[BAR_COUNT][MAX_CUST];
Mug g_mug[BAR_COUNT][MAX_MUGS];
Tip g_tip[BAR_COUNT][MAX_TIPS];

/* ===== Helpers ===== */
static uint8_t bar_row(uint8_t bar)
{
    return (uint8_t)(BAR_ROW0 + bar * (BAR_GAP + 1));
}

static uint8_t door_x0(void)
{
    return (uint8_t)(XSize - DOOR_X0_OFFSET);
}

static uint8_t door_x1(void)
{
    return (uint8_t)(XSize - 1);
}

static uint8_t surf_x1(void)
{
    return (uint8_t)(XSize - DOOR_X0_OFFSET - 1);
}

static uint8_t cust_speed(void)
{
    uint8_t s;
    s = (uint8_t)(TICK_RATE - g_level * 2);
    if (s < 5)
        s = 5;
    return s;
}

static uint8_t slide_back_dist(void)
{
    uint8_t d;
    d = (uint8_t)(12 - g_level);
    if (d < 3)
        d = 3;
    return d;
}

/* ===== Static drawing ===== */
static void draw_static(void)
{
    uint8_t b, x;
    for (b = 0; b < BAR_COUNT; b++) {
        uint8_t row;
        row = bar_row(b);
        for (x = SURF_X0; x <= surf_x1(); x++) {
            _XL_DRAW(x, row, _TILE_0, _XL_BLUE);
        }
        _XL_DRAW(KEG_X0, row, _TILE_2, _XL_GREEN);
        _XL_DRAW(KEG_X1, row, _TILE_2, _XL_GREEN);
        _XL_DRAW(door_x0(), row, _TILE_3, _XL_YELLOW);
        _XL_DRAW(door_x1(), row, _TILE_3, _XL_YELLOW);
    }
}

/* ===== Restore surface tile ===== */
static void restore_surf(uint8_t x, uint8_t row)
{
    if (x >= SURF_X0 && x <= surf_x1()) {
        _XL_DRAW(x, row, _TILE_0, _XL_BLUE);
    }
}

/* ===== Entity slot finders ===== */
static uint8_t find_free_cust(uint8_t bar)
{
    uint8_t i;
    for (i = 0; i < MAX_CUST; i++) {
        if (!g_cust[bar][i].active)
            return i;
    }
    return 255;
}

static uint8_t find_free_mug(uint8_t bar)
{
    uint8_t i;
    for (i = 0; i < MAX_MUGS; i++) {
        if (!g_mug[bar][i].active)
            return i;
    }
    return 255;
}

static uint8_t find_free_tip(uint8_t bar)
{
    uint8_t i;
    for (i = 0; i < MAX_TIPS; i++) {
        if (!g_tip[bar][i].active)
            return i;
    }
    return 255;
}

static uint8_t count_cust(void)
{
    uint8_t b, i, n;
    n = 0;
    for (b = 0; b < BAR_COUNT; b++) {
        for (i = 0; i < MAX_CUST; i++) {
            if (g_cust[b][i].active)
                n++;
        }
    }
    return n;
}

/* ===== Reset ===== */
static void reset_entities(void)
{
    uint8_t b, i;
    for (b = 0; b < BAR_COUNT; b++) {
        for (i = 0; i < MAX_CUST; i++) {
            g_cust[b][i].active = 0;
            g_cust[b][i].x = 0;
            g_cust[b][i].state = CUST_WALK;
            g_cust[b][i].timer = 0;
            g_cust[b][i].steps_left = 0;
        }
        for (i = 0; i < MAX_MUGS; i++) {
            g_mug[b][i].active = 0;
            g_mug[b][i].x = 0;
            g_mug[b][i].full = 0;
            g_mug[b][i].dir = 0;
            g_mug[b][i].timer = 0;
        }
        for (i = 0; i < MAX_TIPS; i++) {
            g_tip[b][i].active = 0;
            g_tip[b][i].x = 0;
            g_tip[b][i].timer = 0;
        }
    }
}

static void reset_game(void)
{
    g_score = 0;
    g_lives = 3;
    g_level = 1;
    g_state = GS_PLAYING;
    g_bart_bar = 0;
    g_bart_x = KEG_X1;
    g_bart_state = BT_NORMAL;
    g_dancers = 0;
    g_dancer_timer = 0;
    g_clear_timer = 0;
    g_tick = 0;
    g_fire_was = 0;
    g_spawn_timer = 0;
    g_max_cust = 1;
    g_spawned = 0;
    g_prev_score = 0;
    g_prev_lives = 0;
    g_prev_level = 0;
    g_prev_bart_bar = 255;
    g_prev_bart_x = 0;
    g_prev_bart_state = 255;
    g_prev_dancers = 0;
    reset_entities();
    _XL_CLEAR_SCREEN();
    draw_static();
    g_prev_lives = 3;
    g_prev_level = 1;
}

static void next_screen(void)
{
    g_level++;
    if (g_level <= 3)
        g_max_cust = 1;
    else if (g_level <= 5)
        g_max_cust = 2;
    else if (g_level <= 7)
        g_max_cust = 3;
    else
        g_max_cust = 4;
    g_state = GS_PLAYING;
    g_bart_x = KEG_X1;
    g_bart_state = BT_NORMAL;
    g_dancers = 0;
    g_dancer_timer = 0;
    g_spawn_timer = 0;
    g_spawned = 0;
    g_prev_bart_bar = 255;
    g_prev_bart_state = 255;
    g_prev_dancers = 0;
    reset_entities();
    _XL_CLEAR_SCREEN();
    draw_static();
}

/* ===== Lose life ===== */
static void lose_life(void)
{
    g_lives--;
    _XL_EXPLOSION_SOUND();
    if (g_lives == 0) {
        g_state = GS_OVER;
    }
}

/* ===== Spawning ===== */
static void try_spawn_customer(void)
{
    uint8_t bar, idx, r, ti;
    bar = (uint8_t)(_XL_RAND() % BAR_COUNT);
    idx = find_free_cust(bar);
    if (idx == 255)
        return;
    g_cust[bar][idx].active = 1;
    g_cust[bar][idx].x = door_x0() - 1;
    g_cust[bar][idx].state = CUST_WALK;
    g_cust[bar][idx].timer = 0;
    g_cust[bar][idx].steps_left = 0;
    g_spawned = 1;
    r = (uint8_t)(_XL_RAND() % 100);
    if (r < 15) {
        ti = find_free_tip(bar);
        if (ti != 255) {
            g_tip[bar][ti].active = 1;
            g_tip[bar][ti].x = (uint8_t)(SURF_X0 + 2 + (uint8_t)(_XL_RAND() % (surf_x1() - SURF_X0 - 2)));
            g_tip[bar][ti].timer = TIP_LIFE;
        }
    }
}

static void try_spawn_tip(void)
{
    uint8_t bar, ti;
    bar = (uint8_t)(_XL_RAND() % BAR_COUNT);
    ti = find_free_tip(bar);
    if (ti == 255)
        return;
    g_tip[bar][ti].active = 1;
    g_tip[bar][ti].x = (uint8_t)(SURF_X0 + 2 + (uint8_t)(_XL_RAND() % (surf_x1() - SURF_X0 - 2)));
    g_tip[bar][ti].timer = TIP_LIFE;
}

/* ===== Update customers ===== */
static void update_customers(void)
{
    uint8_t b, i, spd, row;
    spd = cust_speed();

    for (b = 0; b < BAR_COUNT; b++) {
        row = bar_row(b);
        for (i = 0; i < MAX_CUST; i++) {
            Customer *c;
            c = &g_cust[b][i];
            if (!c->active)
                continue;

            switch (c->state) {
                case CUST_WALK:
                    c->timer++;
                    if (c->timer >= spd) {
                        c->timer = 0;
                        if (c->x > SURF_X0) {
                            restore_surf(c->x, row);
                            c->x--;
                            _XL_DRAW(c->x, row, _TILE_5, _XL_RED);
                        } else {
                            restore_surf(c->x, row);
                            c->active = 0;
                            lose_life();
                        }
                    }
                    break;

                case CUST_DRINK:
                    c->timer++;
                    if (c->timer >= 30) {
                        c->state = CUST_SLIDE_MUG;
                        c->timer = 0;
                    }
                    break;

                case CUST_SLIDE_BACK:
                    c->timer++;
                    if (c->timer >= 4) {
                        c->timer = 0;
                        if (c->x < door_x0() - 1) {
                            restore_surf(c->x, row);
                            c->x++;
                            _XL_DRAW(c->x, row, _TILE_5, _XL_RED);
                        } else {
                            restore_surf(c->x, row);
                            c->active = 0;
                            g_score += 100;
                            continue;
                        }
                    }
                    c->steps_left--;
                    if (c->steps_left == 0) {
                        c->state = CUST_DRINK;
                        c->timer = 0;
                    }
                    break;

                case CUST_SLIDE_MUG:
                    c->timer++;
                    if (c->timer >= 3) {
                        c->timer = 0;
                        {
                            uint8_t mi;
                            mi = find_free_mug(b);
                            if (mi != 255) {
                                g_mug[b][mi].active = 1;
                                g_mug[b][mi].x = c->x;
                                g_mug[b][mi].full = 0;
                                g_mug[b][mi].dir = 1;
                                g_mug[b][mi].timer = 0;
                                _XL_DRAW(c->x, row, _TILE_7, _XL_WHITE);
                            }
                        }
                        c->state = CUST_WALK;
                        c->timer = 0;
                    }
                    break;

                case CUST_DIZZY:
                    break;

                default:
                    break;
            }
        }
    }
}

/* ===== Update mugs ===== */
static void update_mugs(void)
{
    uint8_t b, i, ci, row;
    for (b = 0; b < BAR_COUNT; b++) {
        row = bar_row(b);
        for (i = 0; i < MAX_MUGS; i++) {
            Mug *m;
            m = &g_mug[b][i];
            if (!m->active)
                continue;
            m->timer++;
            if (m->timer >= 3) {
                m->timer = 0;
                if (m->dir == 0) {
                    if (m->x < door_x0() - 1) {
                        restore_surf(m->x, row);
                        m->x++;
                        _XL_DRAW(m->x, row, _TILE_6, _XL_YELLOW);
                        for (ci = 0; ci < MAX_CUST; ci++) {
                            Customer *c;
                            c = &g_cust[b][ci];
                            if (c->active && c->state == CUST_WALK) {
                                if (m->x >= c->x && m->x <= c->x + 1) {
                                    restore_surf(m->x, row);
                                    m->active = 0;
                                    c->state = CUST_SLIDE_BACK;
                                    c->timer = 0;
                                    c->steps_left = slide_back_dist();
                                    g_score += 50;
                                    _XL_PING_SOUND();
                                    break;
                                }
                            }
                        }
                    } else {
                        restore_surf(m->x, row);
                        m->active = 0;
                        lose_life();
                    }
                } else {
                    if (m->x > SURF_X0) {
                        restore_surf(m->x, row);
                        m->x--;
                        _XL_DRAW(m->x, row, _TILE_7, _XL_WHITE);
                    } else {
                        restore_surf(m->x, row);
                        m->active = 0;
                        if (g_bart_bar == b && g_bart_x <= SURF_X0) {
                            g_score += 20;
                            _XL_TICK_SOUND();
                        } else {
                            lose_life();
                        }
                    }
                }
            }
        }
    }
}

/* ===== Update tips ===== */
static void update_tips(void)
{
    uint8_t b, i, row, j;
    for (b = 0; b < BAR_COUNT; b++) {
        row = bar_row(b);
        for (i = 0; i < MAX_TIPS; i++) {
            Tip *t;
            t = &g_tip[b][i];
            if (!t->active)
                continue;
            t->timer--;
            if (t->timer == 0) {
                restore_surf(t->x, row);
                t->active = 0;
                continue;
            }
            if (g_bart_bar == b && g_bart_x >= t->x - 1 &&
                g_bart_x <= t->x + 1) {
                restore_surf(t->x, row);
                t->active = 0;
                g_score += 200;
                g_dancers = 1;
                g_dancer_timer = DANCER_DUR;
                _XL_ZAP_SOUND();
                for (j = 0; j < MAX_CUST; j++) {
                    Customer *c;
                    c = &g_cust[b][j];
                    if (c->active && c->state == CUST_WALK) {
                        c->state = CUST_DIZZY;
                        _XL_DRAW(c->x, row, _TILE_5, _XL_MAGENTA);
                    }
                }
            }
        }
    }
}

/* ===== Update dancers ===== */
static void update_dancers(void)
{
    uint8_t b, i, x, row;
    row = (uint8_t)(BAR_ROW0 + BAR_COUNT * (BAR_GAP + 1) + 1);

    if (!g_dancers) {
        if (g_prev_dancers) {
            for (x = 4; x < XSize - 4; x += 2) {
                _XL_DELETE(x, row);
            }
            g_prev_dancers = 0;
        }
        return;
    }

    g_dancer_timer--;
    if (g_dancer_timer == 0) {
        g_dancers = 0;
        for (x = 4; x < XSize - 4; x += 2) {
            _XL_DELETE(x, row);
        }
        g_prev_dancers = 0;
        for (b = 0; b < BAR_COUNT; b++) {
            for (i = 0; i < MAX_CUST; i++) {
                Customer *c;
                c = &g_cust[b][i];
                if (c->active && c->state == CUST_DIZZY) {
                    c->state = CUST_WALK;
                    c->timer = 0;
                    _XL_DRAW(c->x, bar_row(b), _TILE_5, _XL_RED);
                }
            }
        }
        return;
    }

    if (!g_prev_dancers) {
        for (x = 4; x < XSize - 4; x += 2) {
            _XL_DRAW(x, row, _TILE_9, _XL_MAGENTA);
        }
        _XL_SET_TEXT_COLOR(_XL_MAGENTA);
        _XL_PRINT(10, row + 1, "DANCERS");
        g_prev_dancers = 1;
    }
}

/* ===== Update bartender ===== */
static void update_bartender(uint8_t input)
{
    uint8_t fire;
    uint8_t new_row;

    fire = _XL_FIRE(input);

    if (g_bart_bar != g_prev_bart_bar || g_bart_x != g_prev_bart_x ||
        g_bart_state != g_prev_bart_state) {
        if (g_prev_bart_bar != 255) {
            uint8_t old_row;
            old_row = bar_row(g_prev_bart_bar);
            _XL_DELETE(g_prev_bart_x, old_row);
        }
        new_row = bar_row(g_bart_bar);
        if (g_bart_state == BT_FILLING) {
            _XL_DRAW(g_bart_x, new_row, _TILE_4, _XL_MAGENTA);
        } else {
            _XL_DRAW(g_bart_x, new_row, _TILE_4, _XL_WHITE);
        }
        g_prev_bart_bar = g_bart_bar;
        g_prev_bart_x = g_bart_x;
        g_prev_bart_state = g_bart_state;
    }

    if (g_bart_state == BT_FILLING) {
        if (!fire) {
            g_bart_state = BT_NORMAL;
            {
                uint8_t mi;
                mi = find_free_mug(g_bart_bar);
                if (mi != 255) {
                    g_mug[g_bart_bar][mi].active = 1;
                    g_mug[g_bart_bar][mi].x = SURF_X0;
                    g_mug[g_bart_bar][mi].full = 1;
                    g_mug[g_bart_bar][mi].dir = 0;
                    g_mug[g_bart_bar][mi].timer = 0;
                    _XL_DRAW(SURF_X0, bar_row(g_bart_bar), _TILE_6, _XL_YELLOW);
                }
            }
        }
        g_fire_was = fire;
        return;
    }

    if (fire && !g_fire_was) {
        g_bart_x = KEG_X1;
        g_bart_state = BT_FILLING;
        _XL_SHOOT_SOUND();
        g_fire_was = fire;
        return;
    }

    if (g_bart_state == BT_NORMAL) {
        if (_XL_LEFT(input)) {
            if (g_bart_x > SURF_X0)
                g_bart_x--;
        }
        if (_XL_RIGHT(input)) {
            if (g_bart_x < surf_x1())
                g_bart_x++;
        }
        if (_XL_UP(input)) {
            g_bart_bar--;
            if (g_bart_bar >= BAR_COUNT)
                g_bart_bar = 0;
            _XL_TICK_SOUND();
        }
        if (_XL_DOWN(input)) {
            g_bart_bar++;
            if (g_bart_bar >= BAR_COUNT)
                g_bart_bar = 0;
            _XL_TICK_SOUND();
        }
    }

    g_fire_was = fire;
}

/* ===== Spawning logic ===== */
static void update_spawning(void)
{
    uint8_t interval, r;
    g_spawn_timer++;
    interval = (uint8_t)(60 - g_level * 5);
    if (interval < 10)
        interval = 10;
    if (g_spawn_timer >= interval) {
        g_spawn_timer = 0;
        try_spawn_customer();
    }
    if (g_tick % 100 == 0) {
        r = (uint8_t)(_XL_RAND() % 100);
        if (r < 20)
            try_spawn_tip();
    }
}

/* ===== Check screen clear ===== */
static void check_clear(void)
{
    uint8_t b, i, any_mug;
    if (!g_spawned)
        return;
    if (count_cust() == 0 && g_state == GS_PLAYING) {
        any_mug = 0;
        for (b = 0; b < BAR_COUNT; b++) {
            for (i = 0; i < MAX_MUGS; i++) {
                if (g_mug[b][i].active)
                    any_mug = 1;
            }
        }
        if (!any_mug) {
            g_state = GS_CLEAR;
            g_clear_timer = CLEAR_DUR;
            _XL_SET_TEXT_COLOR(_XL_YELLOW);
            _XL_PRINT(8, 14, "BAR CLEARED");
        }
    }
}

/* ===== Update clear ===== */
static void update_clear(void)
{
    if (g_state == GS_CLEAR) {
        g_clear_timer--;
        if (g_clear_timer == 0) {
            next_screen();
        }
    }
}

/* ===== HUD update ===== */
static void update_hud(void)
{
    if (g_score != g_prev_score || g_lives != g_prev_lives ||
        g_level != g_prev_level) {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 0, "SCORE ");
        _XL_PRINTD(6, 0, 5, g_score);
        _XL_PRINT(14, 0, "LIVES ");
        _XL_PRINTD(20, 0, 1, g_lives);
        _XL_PRINT(23, 0, "LV ");
        _XL_PRINTD(26, 0, 2, g_level);
        g_prev_score = g_score;
        g_prev_lives = g_lives;
        g_prev_level = g_level;
    }
}

/* ===== Game over display ===== */
static void show_game_over(void)
{
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(8, 10, "GAME OVER");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(10, 13, "PRESS FIRE");
}

/* ===== Main ===== */
int main(void)
{
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    reset_game();

    for (;;) {
        input = _XL_INPUT();

        if (g_state == GS_OVER) {
            if (_XL_FIRE(input)) {
                _XL_WAIT_FOR_INPUT();
                _XL_CLEAR_SCREEN();
                draw_static();
                reset_game();
                continue;
            }
            show_game_over();
            update_hud();
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            continue;
        }

        if (g_state == GS_PLAYING) {
            update_bartender(input);
            g_tick++;
            update_customers();
            update_mugs();
            update_tips();
            update_dancers();
            update_spawning();
            check_clear();
        }

        if (g_state == GS_CLEAR) {
            update_clear();
        }

        update_hud();
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}


