#include <stdint.h>
#include "cross_lib.h"

#define NUM_PLATFORMS 4
#define NUM_ELEVATORS 6
#define NUM_ENEMIES   3
#define NUM_ITEMS     4
#define ELEVATOR_DELAY 5          /* frames before an elevator re-activates */

/* ===== screen state buffer ===== */
static uint8_t scr_tile[XSize + 1][YSize + 1];
static uint8_t scr_color[XSize + 1][YSize + 1];

static void place(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color_id)
{
    if (x > XSize || y > YSize) return;
    if (scr_tile[x][y] != tile_id || scr_color[x][y] != color_id) {
        _XL_DRAW(x, y, tile_id, color_id);
        scr_tile[x][y] = tile_id;
        scr_color[x][y] = color_id;
    }
}

static void remove_cell(uint8_t x, uint8_t y)
{
    if (x > XSize || y > YSize) return;
    if (scr_tile[x][y] != 0) {
        _XL_DELETE(x, y);
        scr_tile[x][y] = 0;
        scr_color[x][y] = 0;
    }
}

static void clear_screen(void)
{
    uint8_t x, y;
    for (x = 0; x <= XSize; x++) {
        for (y = 0; y <= YSize; y++) {
            remove_cell(x, y);
        }
    }
}

/* ===== static level geometry ===== */
struct platform_s { uint8_t floor_y; };
static struct platform_s platforms[NUM_PLATFORMS];

struct elevator_s {
    uint8_t x;
    uint8_t plat_lo, plat_hi;
};
static struct elevator_s elevators[NUM_ELEVATORS];

/* per-elevator cooldown (0 = active) */
static uint8_t elevator_timer[NUM_ELEVATORS];

/* ===== dynamic entities ===== */
static uint8_t player_x, player_y, player_facing, player_plat;
static uint16_t score;

struct enemy_s { uint8_t ex, ey, facing, plat; };
static struct enemy_s enemies[NUM_ENEMIES];

struct item_s { uint8_t ix, iy, collected; };
static struct item_s items[NUM_ITEMS];

/* prev-frame state for diff-based rendering */
static uint8_t prev_px, prev_py, prev_pf;
static struct enemy_s prev_enemies[NUM_ENEMIES];
static uint8_t prev_item_collected[NUM_ITEMS];

/* ===== static tile restoration ===== */
static void restore_static_tile(uint8_t x, uint8_t y)
{
    uint8_t p, e;
    for (p = 0; p < NUM_PLATFORMS; p++) {
        if (y == platforms[p].floor_y) {
            uint8_t elev_here = 0;
            for (e = 0; e < NUM_ELEVATORS; e++) {
                if (x == elevators[e].x &&
                    (y == platforms[elevators[e].plat_lo].floor_y ||
                     y == platforms[elevators[e].plat_hi].floor_y)) {
                    elev_here = 1;
                    break;
                }
            }
            place(x, y, elev_here ? _TILE_10 : _TILE_9,
                         elev_here ? _XL_BLUE : _XL_GREEN);
            return;
        }
    }
}

static void clear_entity_cell(uint8_t x, uint8_t y)
{
    if (x > XSize || y > YSize) return;
    {
        uint8_t p;
        for (p = 0; p < NUM_PLATFORMS; p++) {
            if (y == platforms[p].floor_y) {
                restore_static_tile(x, y);
                return;
            }
        }
    }
    remove_cell(x, y);
}

/* ===== initialisation ===== */
void init_game(void)
{
    uint8_t i;

    platforms[0].floor_y = (uint8_t)(YSize - 1);
    platforms[1].floor_y = (uint8_t)(3 * YSize / 5);
    platforms[2].floor_y = (uint8_t)(2 * YSize / 5);
    platforms[3].floor_y = 2;

    elevators[0].x = 1;                    elevators[0].plat_lo = 0; elevators[0].plat_hi = 1;
    elevators[1].x = (uint8_t)(XSize / 2);  elevators[1].plat_lo = 1; elevators[1].plat_hi = 2;
    elevators[2].x = (uint8_t)(XSize - 2);  elevators[2].plat_lo = 0; elevators[2].plat_hi = 1;
    elevators[3].x = (uint8_t)(XSize / 4);  elevators[3].plat_lo = 1; elevators[3].plat_hi = 2;
    elevators[4].x = (uint8_t)(3 * XSize / 4); elevators[4].plat_lo = 2; elevators[4].plat_hi = 3;
    elevators[5].x = (uint8_t)(XSize - 1);  elevators[5].plat_lo = 1; elevators[5].plat_hi = 2;

    for (i = 0; i < NUM_ELEVATORS; i++) elevator_timer[i] = 0;

    player_plat   = 0;
    player_x      = (uint8_t)(XSize / 2);
    player_y      = (uint8_t)(platforms[0].floor_y - 1);
    player_facing = 1;

    prev_px = player_x;
    prev_py = player_y;
    prev_pf = player_facing;

    score = 0;

    enemies[0].plat = 0; enemies[0].ex = (uint8_t)(XSize / 4);     enemies[0].ey = (uint8_t)(platforms[0].floor_y - 1); enemies[0].facing = 1;
    enemies[1].plat = 1; enemies[1].ex = (uint8_t)(3 * XSize / 4); enemies[1].ey = (uint8_t)(platforms[1].floor_y - 1); enemies[1].facing = 0;
    enemies[2].plat = 2; enemies[2].ex = (uint8_t)(XSize / 2);     enemies[2].ey = (uint8_t)(platforms[2].floor_y - 1); enemies[2].facing = 1;

    for (i = 0; i < NUM_ENEMIES; i++) prev_enemies[i] = enemies[i];

    items[0].ix = (uint8_t)(XSize / 5);       items[0].iy = (uint8_t)(platforms[1].floor_y - 1); items[0].collected = 0;
    items[1].ix = (uint8_t)(3 * XSize / 5);   items[1].iy = (uint8_t)(platforms[2].floor_y - 1); items[1].collected = 0;
    items[2].ix = (uint8_t)(XSize / 3);       items[2].iy = (uint8_t)(platforms[3].floor_y - 1); items[2].collected = 0;
    items[3].ix = (uint8_t)(2 * XSize / 3);   items[3].iy = (uint8_t)(platforms[1].floor_y - 1); items[3].collected = 0;

    for (i = 0; i < NUM_ITEMS; i++) prev_item_collected[i] = 0;
}

/* ===== static level rendering ===== */
void draw_static_level(void)
{
    uint8_t p, x, e;

    for (p = 0; p < NUM_PLATFORMS; p++) {
        for (x = 0; x <= XSize; x++) {
            place(x, platforms[p].floor_y, _TILE_9, _XL_GREEN);
        }
    }

    for (e = 0; e < NUM_ELEVATORS; e++) {
        place(elevators[e].x, platforms[elevators[e].plat_lo].floor_y, _TILE_10, _XL_BLUE);
        place(elevators[e].x, platforms[elevators[e].plat_hi].floor_y, _TILE_10, _XL_BLUE);
    }
}

/* ===== elevator logic with cooldown ===== */
void tick_elevators(void)
{
    uint8_t e;
    for (e = 0; e < NUM_ELEVATORS; e++) {
        if (elevator_timer[e] > 0) elevator_timer[e]--;
    }
}

void check_elevator(void)
{
    uint8_t e;
    for (e = 0; e < NUM_ELEVATORS; e++) {
        /* skip elevators still in cooldown */
        if (elevator_timer[e] != 0) continue;

        if (player_x == elevators[e].x && player_plat == elevators[e].plat_lo) {
            player_plat = elevators[e].plat_hi;
            player_y    = (uint8_t)(platforms[player_plat].floor_y - 1);
            elevator_timer[e] = ELEVATOR_DELAY;
            _XL_TICK_SOUND();
        } else if (player_x == elevators[e].x && player_plat == elevators[e].plat_hi) {
            player_plat = elevators[e].plat_lo;
            player_y    = (uint8_t)(platforms[player_plat].floor_y - 1);
            elevator_timer[e] = ELEVATOR_DELAY;
            _XL_TICK_SOUND();
        }
    }
}

/* ===== diff-based entity rendering ===== */
void render_player(void)
{
    uint8_t t_top, t_bot;

    if (prev_px != player_x || prev_py != player_y || prev_pf != player_facing) {
        clear_entity_cell(prev_px, prev_py);
        if (prev_py < YSize) clear_entity_cell(prev_px, (uint8_t)(prev_py + 1));

        t_top = (player_facing == 0) ? _TILE_0 : _TILE_2;
        t_bot = (player_facing == 0) ? _TILE_1 : _TILE_3;

        place(player_x, player_y, t_top, _XL_CYAN);
        if (player_y < YSize) place(player_x, (uint8_t)(player_y + 1), t_bot, _XL_CYAN);
    } else {
        t_top = (player_facing == 0) ? _TILE_0 : _TILE_2;
        t_bot = (player_facing == 0) ? _TILE_1 : _TILE_3;

        place(player_x, player_y, t_top, _XL_CYAN);
        if (player_y < YSize) place(player_x, (uint8_t)(player_y + 1), t_bot, _XL_CYAN);
    }

    prev_px = player_x;
    prev_py = player_y;
    prev_pf = player_facing;
}

void render_enemies(void)
{
    uint8_t i;
    for (i = 0; i < NUM_ENEMIES; i++) {
        if (prev_enemies[i].ex != enemies[i].ex ||
            prev_enemies[i].ey != enemies[i].ey ||
            prev_enemies[i].facing != enemies[i].facing) {

            clear_entity_cell(prev_enemies[i].ex, prev_enemies[i].ey);
            if (prev_enemies[i].ey < YSize)
                clear_entity_cell(prev_enemies[i].ex, (uint8_t)(prev_enemies[i].ey + 1));
        }

        {
            uint8_t t_top = (enemies[i].facing == 0) ? _TILE_4 : _TILE_6;
            uint8_t t_bot = (enemies[i].facing == 0) ? _TILE_5 : _TILE_7;

            place(enemies[i].ex, enemies[i].ey, t_top, _XL_RED);
            if (enemies[i].ey < YSize)
                place(enemies[i].ex, (uint8_t)(enemies[i].ey + 1), t_bot, _XL_RED);
        }

        prev_enemies[i] = enemies[i];
    }
}

void render_items(void)
{
    uint8_t i;
    for (i = 0; i < NUM_ITEMS; i++) {
        if (prev_item_collected[i] != items[i].collected) {
            if (items[i].collected) {
                clear_entity_cell(items[i].ix, items[i].iy);
            } else {
                place(items[i].ix, items[i].iy, _TILE_8, _XL_YELLOW);
            }
        } else if (!items[i].collected) {
            place(items[i].ix, items[i].iy, _TILE_8, _XL_YELLOW);
        }

        prev_item_collected[i] = items[i].collected;
    }
}

/* ===== main loop ===== */
int main(void)
{
    uint8_t input;
    int game_over;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    for (;;) {
        init_game();
        clear_screen();

        draw_static_level();
        render_player();
        render_enemies();
        render_items();

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 0, "SCORE");
        _XL_PRINTD(5, 0, 2, score);

        game_over = 0;

        while (!game_over) {
            input = _XL_INPUT();

            if (_XL_LEFT(input)) {
                if (player_x > 0) player_x--;
                player_facing = 0;
            } else if (_XL_RIGHT(input)) {
                if (player_x < XSize) player_x++;
                player_facing = 1;
            }

            /* tick elevator cooldowns, then check teleport */
            tick_elevators();
            check_elevator();

            /* enemies patrol */
            {
                uint8_t i;
                for (i = 0; i < NUM_ENEMIES; i++) {
                    if (enemies[i].facing == 0) {
                        if (enemies[i].ex > 0) enemies[i].ex--;
                        else enemies[i].facing = 1;
                    } else {
                        if (enemies[i].ex < XSize) enemies[i].ex++;
                        else enemies[i].facing = 0;
                    }
                }
            }

            /* collision */
            {
                uint8_t i;
                for (i = 0; i < NUM_ENEMIES; i++) {
                    if (player_plat == enemies[i].plat &&
                        player_x == enemies[i].ex &&
                        player_y == enemies[i].ey) {
                        game_over = 1;
                        _XL_EXPLOSION_SOUND();
                    }
                }
            }

            /* item pickup */
            {
                uint8_t i;
                for (i = 0; i < NUM_ITEMS; i++) {
                    if (!items[i].collected &&
                        player_x == items[i].ix &&
                        player_y == items[i].iy) {
                        items[i].collected = 1;
                        score++;
                        _XL_PING_SOUND();
                    }
                }
            }

            if (score >= NUM_ITEMS) game_over = 1;

            render_player();
            render_enemies();
            render_items();

            _XL_PRINTD(5, 0, 2, score);

            if (game_over) {
                _XL_SET_TEXT_COLOR(_XL_MAGENTA);
                _XL_PRINT((uint8_t)(player_x > XSize / 4 ? player_x - 6 : 1),
                          (uint8_t)(player_y < YSize - 2 ? player_y + 2 : YSize),
                          "GAME OVER");
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 3);
            } else {
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }
        }

        clear_screen();
    }
}
