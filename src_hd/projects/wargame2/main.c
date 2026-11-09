#include "cross_lib.h"

/* Unit types */
#define UNIT_INFANTRY 0
#define UNIT_TANK     1
#define UNIT_HELI     2
#define UNIT_PLANE    3
#define NUM_UNIT_TYPES 4

/* Unit stats - tanks are very slow */
#define INF_HP    3
#define INF_DMG   1
#define INF_SPEED 2
#define INF_COST  5

#define TANK_HP    25
#define TANK_DMG   4
#define TANK_SPEED 8
#define TANK_COST  50

#define HELI_HP    8
#define HELI_DMG   2
#define HELI_SPEED 2
#define HELI_COST  30

#define PLANE_HP    5
#define PLANE_DMG   5
#define PLANE_SPEED 1
#define PLANE_COST  40

/* Game constants */
#define WALL_MAX_HP    250
#define MAX_UNITS      16
#define LANES          5
#define INITIAL_BUDGET 100
#define BUDGET_TICK    4
#define BUDGET_INC     2
#define KILL_BONUS     5
#define WALL_HIT_BONUS 2
#define ENEMY_SPAWN_INTERVAL 32

#define WALL_ENEMY_ROW  0
#define WALL_PLAYER_ROW (YSize - 1)

typedef struct {
    uint8_t type;
    uint8_t hp;
    uint8_t x;
    uint8_t y;
    uint8_t frame;
    uint8_t active;
} Unit;

/* Global state */
Unit player_units[MAX_UNITS];
Unit enemy_units[MAX_UNITS];

uint8_t  player_wall_hp;
uint8_t  enemy_wall_hp;
uint16_t budget;
uint8_t  selected_type;
uint8_t  selected_lane;
uint8_t  budget_timer;
uint8_t  spawn_timer;
uint8_t  game_state;
uint8_t  tick_count;

/* Track what is on screen to avoid redundant draws */
uint8_t  drawn_enemy_wall_color;
uint8_t  drawn_player_wall_color;
uint8_t  prev_enemy_hp;
uint8_t  prev_player_hp;
uint16_t prev_budget;
uint8_t  prev_sel_type;
uint8_t  prev_sel_lane;

/* ---------- Helpers ---------- */

uint8_t get_hp(uint8_t t)
{
    if (t == UNIT_INFANTRY) return INF_HP;
    if (t == UNIT_TANK)     return TANK_HP;
    if (t == UNIT_HELI)     return HELI_HP;
    return PLANE_HP;
}

uint8_t get_dmg(uint8_t t)
{
    if (t == UNIT_INFANTRY) return INF_DMG;
    if (t == UNIT_TANK)     return TANK_DMG;
    if (t == UNIT_HELI)     return HELI_DMG;
    return PLANE_DMG;
}

uint8_t get_speed(uint8_t t)
{
    if (t == UNIT_INFANTRY) return INF_SPEED;
    if (t == UNIT_TANK)     return TANK_SPEED;
    if (t == UNIT_HELI)     return HELI_SPEED;
    return PLANE_SPEED;
}

uint8_t get_cost(uint8_t t)
{
    if (t == UNIT_INFANTRY) return INF_COST;
    if (t == UNIT_TANK)     return TANK_COST;
    if (t == UNIT_HELI)     return HELI_COST;
    return PLANE_COST;
}

char get_char(uint8_t t)
{
    if (t == UNIT_INFANTRY) return 'I';
    if (t == UNIT_TANK)     return 'T';
    if (t == UNIT_HELI)     return 'H';
    return 'P';
}

uint8_t lane_x(uint8_t lane)
{
    return (uint8_t)(1 + (lane + 1) * (XSize - 2) / (LANES + 1));
}

uint8_t wall_color(uint8_t hp)
{
    if (hp > 60) return _XL_GREEN;
    if (hp > 30) return _XL_YELLOW;
    if (hp > 0)  return _XL_RED;
    return _XL_BLUE;
}

/* ---------- Screen operations (direct, no buffer) ---------- */

void draw_wall_row(uint8_t row, uint8_t color)
{
    uint8_t x;
    for (x = 0; x < XSize; x++) {
        _XL_DRAW(x, row, _TILE_0, color);
    }
}

void delete_wall_row(uint8_t row)
{
    uint8_t x;
    for (x = 0; x < XSize; x++) {
        _XL_DELETE(x, row);
    }
}

void draw_unit(uint8_t x, uint8_t y, uint8_t type, uint8_t is_player)
{
    uint8_t col;
    col = is_player ? _XL_CYAN : _XL_MAGENTA;
    _XL_SET_TEXT_COLOR(col);
    _XL_CHAR(x, y, get_char(type));
}

void delete_unit(uint8_t x, uint8_t y)
{
    _XL_DELETE(x, y);
}

void draw_lane_marker(void)
{
    uint8_t x;
    uint8_t y;
    y = WALL_PLAYER_ROW;
    x = lane_x(selected_lane);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_CHAR(x, y, '^');
}

void draw_hud(void)
{
    /* Enemy wall HP - top right */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(1, WALL_ENEMY_ROW, "E:");
    _XL_PRINTD(3, WALL_ENEMY_ROW, 2, enemy_wall_hp);

    /* Player wall HP + budget + unit - bottom right */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(1, WALL_PLAYER_ROW, "P:");
    _XL_PRINTD(3, WALL_PLAYER_ROW, 2, player_wall_hp);
    _XL_PRINT(6, WALL_PLAYER_ROW, "B:");
    _XL_PRINTD(8, WALL_PLAYER_ROW, 3, budget);
    _XL_PRINT(12, WALL_PLAYER_ROW, "U:");
    _XL_CHAR(14, WALL_PLAYER_ROW, get_char(selected_type));
    _XL_PRINT(16, WALL_PLAYER_ROW, "L:");
    _XL_PRINTD(18, WALL_PLAYER_ROW, 1, selected_lane);
}

/* ---------- Setup / Reset ---------- */

void clear_all_units(void)
{
    uint8_t i;
    for (i = 0; i < MAX_UNITS; i++) {
        if (player_units[i].active) {
            delete_unit(player_units[i].x, player_units[i].y);
            player_units[i].active = 0;
        }
        if (enemy_units[i].active) {
            delete_unit(enemy_units[i].x, enemy_units[i].y);
            enemy_units[i].active = 0;
        }
    }
}

void setup_game(void)
{
    uint8_t i;

    clear_all_units();

    player_wall_hp = WALL_MAX_HP;
    enemy_wall_hp  = WALL_MAX_HP;
    budget         = INITIAL_BUDGET;
    selected_type  = UNIT_INFANTRY;
    selected_lane  = 2;
    budget_timer   = 0;
    spawn_timer    = 0;
    tick_count     = 0;
    game_state     = 1;

    /* Draw walls */
    drawn_enemy_wall_color = wall_color(enemy_wall_hp);
    drawn_player_wall_color = wall_color(player_wall_hp);
    draw_wall_row(WALL_ENEMY_ROW, drawn_enemy_wall_color);
    draw_wall_row(WALL_PLAYER_ROW, drawn_player_wall_color);

    /* Draw initial HUD */
    prev_enemy_hp = enemy_wall_hp;
    prev_player_hp = player_wall_hp;
    prev_budget = budget;
    prev_sel_type = selected_type;
    prev_sel_lane = selected_lane;
    draw_hud();
    draw_lane_marker();
}

/* ---------- Unit management ---------- */

uint8_t find_slot(Unit *arr)
{
    uint8_t i;
    for (i = 0; i < MAX_UNITS; i++) {
        if (!arr[i].active) return i;
    }
    return 255;
}

void deploy_player(void)
{
    uint8_t slot;
    uint8_t cost;
    uint8_t by;

    cost = get_cost(selected_type);
    if (budget < cost) return;
    slot = find_slot(player_units);
    if (slot == 255) return;

    by = (uint8_t)(WALL_PLAYER_ROW - 1);
    budget = (uint16_t)(budget - cost);

    player_units[slot].type   = selected_type;
    player_units[slot].hp     = get_hp(selected_type);
    player_units[slot].x      = lane_x(selected_lane);
    player_units[slot].y      = by;
    player_units[slot].frame  = 0;
    player_units[slot].active = 1;

    draw_unit(player_units[slot].x, player_units[slot].y, selected_type, 1);
    _XL_PING_SOUND();
}

void spawn_enemy(void)
{
    uint8_t slot;
    uint8_t type;
    uint8_t lane;

    slot = find_slot(enemy_units);
    if (slot == 255) return;

    type = (uint8_t)(_XL_RAND() % NUM_UNIT_TYPES);
    if (_XL_RAND() % 3 == 0) type = UNIT_INFANTRY;
    lane = (uint8_t)(_XL_RAND() % LANES);

    enemy_units[slot].type   = type;
    enemy_units[slot].hp     = get_hp(type);
    enemy_units[slot].x      = lane_x(lane);
    enemy_units[slot].y      = (uint8_t)(WALL_ENEMY_ROW + 1);
    enemy_units[slot].frame  = 0;
    enemy_units[slot].active = 1;

    draw_unit(enemy_units[slot].x, enemy_units[slot].y, type, 0);
}

/* ---------- Combat ---------- */

void damage_unit(Unit *u, uint8_t dmg)
{
    if (u->hp > dmg) u->hp = (uint8_t)(u->hp - dmg);
    else u->hp = 0;
}

void move_and_attack_player(void)
{
    uint8_t i, j;
    uint8_t spd;
    uint8_t dmg;
    uint8_t ex, ey;
    uint8_t dx, dy;
    uint8_t old_x, old_y;
    uint8_t top;

    top = (uint8_t)(WALL_ENEMY_ROW + 1);

    for (i = 0; i < MAX_UNITS; i++) {
        if (!player_units[i].active) continue;

        player_units[i].frame++;
        spd = get_speed(player_units[i].type);

        if (player_units[i].frame >= spd) {
            player_units[i].frame = 0;
            old_x = player_units[i].x;
            old_y = player_units[i].y;

            /* Move up toward enemy */
            if (player_units[i].y > top) {
                player_units[i].y--;
            }

            /* Attack enemy wall */
            if (player_units[i].y <= top) {
                player_units[i].y = top;
                dmg = get_dmg(player_units[i].type);
                if (enemy_wall_hp >= dmg) enemy_wall_hp = (uint8_t)(enemy_wall_hp - dmg);
                else enemy_wall_hp = 0;
                budget = (uint16_t)(budget + WALL_HIT_BONUS);
                _XL_SHOOT_SOUND();
            }

            /* Helicopter: shoot sideways/diagonally (range 2) */
            if (player_units[i].type == UNIT_HELI) {
                dmg = get_dmg(UNIT_HELI);
                for (j = 0; j < MAX_UNITS; j++) {
                    if (!enemy_units[j].active) continue;
                    ex = enemy_units[j].x;
                    ey = enemy_units[j].y;
                    if (ex > player_units[i].x) dx = (uint8_t)(ex - player_units[i].x);
                    else dx = 0;
                    if (ey >= player_units[i].y) dy = (uint8_t)(ey - player_units[i].y);
                    else dy = 0;
                    if (dx <= 2 && dy <= 2 && (dx > 0 || dy > 0)) {
                        damage_unit(&enemy_units[j], dmg);
                    }
                    if (ex < player_units[i].x) {
                        dx = (uint8_t)(player_units[i].x - ex);
                    } else {
                        dx = 0;
                    }
                    if (dx <= 2 && dy <= 2 && dx > 0) {
                        damage_unit(&enemy_units[j], dmg);
                    }
                }
            }

            /* Only redraw if position actually changed */
            if (old_x != player_units[i].x || old_y != player_units[i].y) {
                delete_unit(old_x, old_y);
                draw_unit(player_units[i].x, player_units[i].y,
                          player_units[i].type, 1);
            }
        }

        /* Melee: same cell */
        for (j = 0; j < MAX_UNITS; j++) {
            if (!enemy_units[j].active) continue;
            if (enemy_units[j].x == player_units[i].x &&
                enemy_units[j].y == player_units[i].y) {
                damage_unit(&enemy_units[j], get_dmg(player_units[i].type));
                damage_unit(&player_units[i], get_dmg(enemy_units[j].type));
                _XL_TOCK_SOUND();
            }
        }
    }
}

void move_and_attack_enemy(void)
{
    uint8_t i, j;
    uint8_t spd;
    uint8_t dmg;
    uint8_t old_x, old_y;
    uint8_t bot;

    bot = (uint8_t)(WALL_PLAYER_ROW - 1);

    for (i = 0; i < MAX_UNITS; i++) {
        if (!enemy_units[i].active) continue;

        enemy_units[i].frame++;
        spd = get_speed(enemy_units[i].type);

        if (enemy_units[i].frame >= spd) {
            enemy_units[i].frame = 0;
            old_x = enemy_units[i].x;
            old_y = enemy_units[i].y;

            /* Move down toward player */
            if (enemy_units[i].y < bot) {
                enemy_units[i].y++;
            }

            /* Attack player wall */
            if (enemy_units[i].y >= bot) {
                enemy_units[i].y = bot;
                dmg = get_dmg(enemy_units[i].type);
                if (player_wall_hp >= dmg) player_wall_hp = (uint8_t)(player_wall_hp - dmg);
                else player_wall_hp = 0;
                _XL_SHOOT_SOUND();
            }

            /* Only redraw if position actually changed */
            if (old_x != enemy_units[i].x || old_y != enemy_units[i].y) {
                delete_unit(old_x, old_y);
                draw_unit(enemy_units[i].x, enemy_units[i].y,
                          enemy_units[i].type, 0);
            }
        }

        /* Melee: same cell */
        for (j = 0; j < MAX_UNITS; j++) {
            if (!player_units[j].active) continue;
            if (player_units[j].x == enemy_units[i].x &&
                player_units[j].y == enemy_units[i].y) {
                damage_unit(&player_units[j], get_dmg(enemy_units[i].type));
            }
        }
    }
}

void cleanup_dead(void)
{
    uint8_t i;
    for (i = 0; i < MAX_UNITS; i++) {
        if (player_units[i].active && player_units[i].hp == 0) {
            delete_unit(player_units[i].x, player_units[i].y);
            player_units[i].active = 0;
        }
        if (enemy_units[i].active && enemy_units[i].hp == 0) {
            delete_unit(enemy_units[i].x, enemy_units[i].y);
            enemy_units[i].active = 0;
            budget = (uint16_t)(budget + KILL_BONUS);
        }
    }
}

/* ---------- HUD update (only when values change) ---------- */

void update_hud(void)
{
    uint8_t new_e_color;
    uint8_t new_p_color;

    /* Enemy wall color */
    new_e_color = wall_color(enemy_wall_hp);
    if (new_e_color != drawn_enemy_wall_color) {
        delete_wall_row(WALL_ENEMY_ROW);
        draw_wall_row(WALL_ENEMY_ROW, new_e_color);
        drawn_enemy_wall_color = new_e_color;
    }

    /* Player wall color */
    new_p_color = wall_color(player_wall_hp);
    if (new_p_color != drawn_player_wall_color) {
        delete_wall_row(WALL_PLAYER_ROW);
        draw_wall_row(WALL_PLAYER_ROW, new_p_color);
        drawn_player_wall_color = new_p_color;
    }

    /* HP text - delete old digits, draw new */
    if (enemy_wall_hp != prev_enemy_hp) {
        _XL_DELETE(3, WALL_ENEMY_ROW);
        _XL_DELETE(4, WALL_ENEMY_ROW);
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(3, WALL_ENEMY_ROW, 2, enemy_wall_hp);
        prev_enemy_hp = enemy_wall_hp;
    }

    if (player_wall_hp != prev_player_hp) {
        _XL_DELETE(3, WALL_PLAYER_ROW);
        _XL_DELETE(4, WALL_PLAYER_ROW);
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(3, WALL_PLAYER_ROW, 2, player_wall_hp);
        prev_player_hp = player_wall_hp;
    }

    /* Budget */
    if (budget != prev_budget) {
        _XL_DELETE(8, WALL_PLAYER_ROW);
        _XL_DELETE(9, WALL_PLAYER_ROW);
        _XL_DELETE(10, WALL_PLAYER_ROW);
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(8, WALL_PLAYER_ROW, 3, budget);
        prev_budget = budget;
    }

    /* Selected type */
    if (selected_type != prev_sel_type) {
        _XL_DELETE(14, WALL_PLAYER_ROW);
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_CHAR(14, WALL_PLAYER_ROW, get_char(selected_type));
        prev_sel_type = selected_type;
    }

    /* Selected lane */
    if (selected_lane != prev_sel_lane) {
        _XL_DELETE(lane_x(prev_sel_lane), WALL_PLAYER_ROW);
        _XL_DELETE(18, WALL_PLAYER_ROW);
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(18, WALL_PLAYER_ROW, 1, selected_lane);
        draw_lane_marker();
        prev_sel_lane = selected_lane;
    }
}

/* ---------- End screens ---------- */

void draw_end_screen(void)
{
    uint8_t cx;
    uint8_t cy;
    cx = (uint8_t)(XSize / 2);
    cy = (uint8_t)(YSize / 2);

    if (game_state == 2) {
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(2, cy, "VICTORY!");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, cy + 2, "ENEMY WALL DESTROYED");
    } else {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(2, cy, "DEFEAT");
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(2, cy + 2, "YOUR WALL FELL");
    }
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(2, cy + 4, "FIRE TO RESTART");
}

void draw_title(void)
{
    uint8_t cy;
    cy = (uint8_t)(YSize / 2);
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(1, cy - 4, "AUTO-BATTLER");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(1, cy - 2, "DESTROY ENEMY WALL");
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(1, cy, "I:INF T:TK H:HLI P:PLN");
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(1, cy + 2, "L/R:LANE U/D:UNIT F:DEPLOY");
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(1, cy + 4, "PRESS FIRE TO START");
}

/* ---------- Input ---------- */

void handle_input(void)
{
    uint8_t input;

    input = _XL_INPUT();

    if (game_state == 0) {
        if (_XL_FIRE(input)) {
            setup_game();
        }
        return;
    }

    if (game_state == 2 || game_state == 3) {
        if (_XL_FIRE(input)) {
            clear_all_units();
            delete_wall_row(WALL_ENEMY_ROW);
            delete_wall_row(WALL_PLAYER_ROW);
            game_state = 0;
        }
        return;
    }

    if (_XL_LEFT(input)) {
        if (selected_lane > 0) selected_lane--;
    }
    if (_XL_RIGHT(input)) {
        if (selected_lane < (uint8_t)(LANES - 1)) selected_lane++;
    }
    if (_XL_UP(input)) {
        selected_type = (uint8_t)((selected_type + 1) % NUM_UNIT_TYPES);
    }
    if (_XL_DOWN(input)) {
        if (selected_type > 0) selected_type--;
        else selected_type = (uint8_t)(NUM_UNIT_TYPES - 1);
    }
    if (_XL_FIRE(input)) {
        deploy_player();
    }
}

/* ---------- Game tick ---------- */

void game_tick(void)
{
    tick_count++;

    /* Budget grows */
    budget_timer++;
    if (budget_timer >= BUDGET_TICK) {
        budget_timer = 0;
        budget = (uint16_t)(budget + BUDGET_INC);
    }

    /* Enemy spawning (accelerates over time) */
    spawn_timer++;
    if (spawn_timer >= ENEMY_SPAWN_INTERVAL) {
        spawn_timer = 0;
        spawn_enemy();
        if (tick_count > 300) spawn_enemy();
        if (tick_count > 600) spawn_enemy();
    }

    move_and_attack_player();
    move_and_attack_enemy();
    cleanup_dead();
    update_hud();

    /* Win/lose check */
    if (enemy_wall_hp == 0 && game_state == 1) {
        game_state = 2;
        _XL_EXPLOSION_SOUND();
        draw_end_screen();
    }
    if (player_wall_hp == 0 && game_state == 1) {
        game_state = 3;
        _XL_EXPLOSION_SOUND();
        draw_end_screen();
    }
}

/* ---------- Main ---------- */

int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while(1)
    {
        game_state = 0;
        draw_title();
        
        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();
        _XL_CLEAR_SCREEN();
        
        while (1) {
            handle_input();

            if (game_state == 1) {
                game_tick();
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }

    return 0;
}