#include "cross_lib.h"

/* Unit types */
#define UNIT_NONE        0
#define UNIT_INFANTRY    1
#define UNIT_TANK        2
#define UNIT_HELICOPTER  3

/* Costs */
#define COST_INFANTRY     1
#define COST_TANK         5
#define COST_HELICOPTER   3

/* HP */
#define HP_INFANTRY       1
#define HP_TANK           3
#define HP_HELICOPTER     1

/* Move ticks */
#define MOVE_INFANTRY     2
#define MOVE_TANK         4
#define MOVE_HELICOPTER   1

/* Shoot ticks */
#define SHOOT_INFANTRY    3
#define SHOOT_TANK        5
#define SHOOT_HELICOPTER  2

/* Budget */
#define INITIAL_BUDGET    99
#define BUDGET_INTERVAL   20
#define BONUS_INFANTRY    1
#define BONUS_TANK        3
#define BONUS_HELICOPTER  2

/* Limits */
#define MAX_UNITS         6
#define MAX_ENEMY_UNITS   6
#define MAX_PROJ          8
#define MAX_ENEMY_PROJ    8
#define WALL_HP           199

/* Tiles */
#define TILE_INF      _TILE_1
#define TILE_TANK     _TILE_2
#define TILE_HELI     _TILE_3
#define TILE_PROJ     _TILE_4
#define TILE_E_INF    _TILE_5
#define TILE_E_TANK   _TILE_6
#define TILE_E_HELI   _TILE_7
#define TILE_E_PROJ   _TILE_8
#define TILE_WALL     _TILE_0
#define TILE_CURSOR   _TILE_9

/* Colors */
#define C_PLAYER  _XL_CYAN
#define C_ENEMY   _XL_RED
#define C_WALL    _XL_WHITE
#define C_HUD     _XL_YELLOW
#define C_SEL     _XL_GREEN
#define C_PROJ    _XL_YELLOW
#define C_EPROJ   _XL_MAGENTA

typedef struct {
    uint8_t type;
    uint8_t x;
    uint8_t y;
    uint8_t hp;
    uint8_t m_timer;
    uint8_t s_timer;
} Unit;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t dir;
} Proj;

Unit p_units[MAX_UNITS];
Unit e_units[MAX_ENEMY_UNITS];
Proj p_proj[MAX_PROJ];
Proj e_proj[MAX_ENEMY_PROJ];

uint8_t budget;
uint8_t bud_timer;
uint8_t e_wall_hp;
uint8_t p_wall_hp;
uint8_t sel_type;
uint8_t sel_x;
uint8_t state;
uint8_t e_spawn_t;
uint8_t p_uc;
uint8_t e_uc;
uint8_t p_pc;
uint8_t e_pc;
uint8_t tick_ct;
uint8_t hud_dirty;

uint8_t get_cost(uint8_t t) {
    if (t == UNIT_INFANTRY) return COST_INFANTRY;
    if (t == UNIT_TANK) return COST_TANK;
    if (t == UNIT_HELICOPTER) return COST_HELICOPTER;
    return 0;
}

uint8_t get_hp(uint8_t t) {
    if (t == UNIT_INFANTRY) return HP_INFANTRY;
    if (t == UNIT_TANK) return HP_TANK;
    if (t == UNIT_HELICOPTER) return HP_HELICOPTER;
    return 1;
}

uint8_t get_move(uint8_t t) {
    if (t == UNIT_INFANTRY) return MOVE_INFANTRY;
    if (t == UNIT_TANK) return MOVE_TANK;
    if (t == UNIT_HELICOPTER) return MOVE_HELICOPTER;
    return 2;
}

uint8_t get_shoot(uint8_t t) {
    if (t == UNIT_INFANTRY) return SHOOT_INFANTRY;
    if (t == UNIT_TANK) return SHOOT_TANK;
    if (t == UNIT_HELICOPTER) return SHOOT_HELICOPTER;
    return 3;
}

uint8_t get_tile(uint8_t t, uint8_t en) {
    if (en) {
        if (t == UNIT_INFANTRY) return TILE_E_INF;
        if (t == UNIT_TANK) return TILE_E_TANK;
        if (t == UNIT_HELICOPTER) return TILE_E_HELI;
    } else {
        if (t == UNIT_INFANTRY) return TILE_INF;
        if (t == UNIT_TANK) return TILE_TANK;
        if (t == UNIT_HELICOPTER) return TILE_HELI;
    }
    return _TILE_0;
}

void draw_unit(uint8_t x, uint8_t y, uint8_t t, uint8_t en) {
    _XL_DRAW(x, y, get_tile(t, en), en ? C_ENEMY : C_PLAYER);
}

void draw_pproj(uint8_t x, uint8_t y) {
    _XL_DRAW(x, y, TILE_PROJ, C_PROJ);
}

void draw_eproj(uint8_t x, uint8_t y) {
    _XL_DRAW(x, y, TILE_E_PROJ, C_EPROJ);
}

void draw_hud(void) {
    _XL_SET_TEXT_COLOR(C_HUD);
    _XL_PRINT(0, 0, "EW:");
    _XL_PRINTD(3, 0, 1, e_wall_hp);
    _XL_PRINT(0, YSize - 1, "PW:");
    _XL_PRINTD(3, YSize - 1, 1, p_wall_hp);
    _XL_PRINT(6, YSize - 1, "BUD:");
    _XL_PRINTD(10, YSize - 1, 2, budget);
    _XL_SET_TEXT_COLOR(C_SEL);
    _XL_PRINT(0, YSize - 2, "TYPE:");
    if (sel_type == UNIT_INFANTRY)
        _XL_PRINT(5, YSize - 2, "INF");
    else if (sel_type == UNIT_TANK)
        _XL_PRINT(5, YSize - 2, "TANK");
    else
        _XL_PRINT(5, YSize - 2, "HELI");
    _XL_PRINT(10, YSize - 2, "COL:");
    _XL_PRINTD(14, YSize - 2, 2, sel_x);
}

void init_game(void) {
    uint8_t i;
    budget = INITIAL_BUDGET;
    bud_timer = 0;
    e_wall_hp = WALL_HP;
    p_wall_hp = WALL_HP;
    sel_type = UNIT_INFANTRY;
    sel_x = XSize / 2;
    state = 0;
    e_spawn_t = 0;
    p_uc = 0;
    e_uc = 0;
    p_pc = 0;
    e_pc = 0;
    tick_ct = 0;
    hud_dirty = 1;

    for (i = 0; i < MAX_UNITS; i++) {
        p_units[i].type = UNIT_NONE;
    }
    for (i = 0; i < MAX_ENEMY_UNITS; i++) {
        e_units[i].type = UNIT_NONE;
    }
    for (i = 0; i < MAX_PROJ; i++) {
        p_proj[i].x = 0;
        p_proj[i].y = 0xFF;
        p_proj[i].dir = 0;
    }
    for (i = 0; i < MAX_ENEMY_PROJ; i++) {
        e_proj[i].x = 0;
        e_proj[i].y = 0xFF;
        e_proj[i].dir = 0;
    }

    _XL_CLEAR_SCREEN();

    for (i = 0; i < XSize; i++) {
        _XL_DRAW(i, 0, TILE_WALL, C_WALL);
        _XL_DRAW(i, YSize - 1, TILE_WALL, C_WALL);
    }

    draw_hud();
}

void spawn_enemy(void) {
    uint8_t i;
    uint8_t t;
    uint8_t x;

    if (e_uc >= MAX_ENEMY_UNITS) return;

    t = 1 + (uint8_t)(_XL_RAND() % 3);
    x = (uint8_t)(_XL_RAND() % XSize);

    for (i = 0; i < MAX_ENEMY_UNITS; i++) {
        if (e_units[i].type == UNIT_NONE) {
            e_units[i].type = t;
            e_units[i].x = x;
            e_units[i].y = 1;
            e_units[i].hp = get_hp(t);
            e_units[i].m_timer = 0;
            e_units[i].s_timer = 0;
            e_uc++;
            draw_unit(x, 1, t, 1);
            return;
        }
    }
}

void deploy_unit(void) {
    uint8_t i;
    uint8_t c;
    uint8_t dy;

    c = get_cost(sel_type);
    if (budget < c) return;
    if (p_uc >= MAX_UNITS) return;

    dy = YSize - 2;

    for (i = 0; i < MAX_UNITS; i++) {
        if (p_units[i].type == UNIT_NONE) {
            p_units[i].type = sel_type;
            p_units[i].x = sel_x;
            p_units[i].y = dy;
            p_units[i].hp = get_hp(sel_type);
            p_units[i].m_timer = 0;
            p_units[i].s_timer = 0;
            p_uc++;
            budget -= c;
            draw_unit(sel_x, dy, sel_type, 0);
            _XL_PING_SOUND();
            hud_dirty = 1;
            return;
        }
    }
}

void player_shoot(uint8_t ui) {
    uint8_t i;
    uint8_t d;
    uint8_t sx;
    uint8_t sy;

    if (p_pc >= MAX_PROJ) return;

    d = 0;
    if (p_units[ui].type == UNIT_HELICOPTER) {
        d = (uint8_t)(_XL_RAND() % 3);
    }

    sx = p_units[ui].x;
    sy = p_units[ui].y;
    if (sy > 1) sy--;
    else sy = 1;

    for (i = 0; i < MAX_PROJ; i++) {
        if (p_proj[i].y == 0xFF) {
            p_proj[i].x = sx;
            p_proj[i].y = sy;
            p_proj[i].dir = d;
            p_pc++;
            draw_pproj(sx, sy);
            _XL_SHOOT_SOUND();
            return;
        }
    }
}

void enemy_shoot(uint8_t ui) {
    uint8_t i;
    uint8_t d;
    uint8_t sx;
    uint8_t sy;

    if (e_pc >= MAX_ENEMY_PROJ) return;

    d = 0;
    if (e_units[ui].type == UNIT_HELICOPTER) {
        d = (uint8_t)(_XL_RAND() % 3);
    }

    sx = e_units[ui].x;
    sy = e_units[ui].y;
    if (sy < YSize - 2) sy++;
    else sy = YSize - 2;

    for (i = 0; i < MAX_ENEMY_PROJ; i++) {
        if (e_proj[i].y == 0xFF) {
            e_proj[i].x = sx;
            e_proj[i].y = sy;
            e_proj[i].dir = d;
            e_pc++;
            draw_eproj(sx, sy);
            return;
        }
    }
}

void remove_p_unit(uint8_t i) {
    _XL_DELETE(p_units[i].x, p_units[i].y);
    p_units[i].type = UNIT_NONE;
    p_uc--;
}

void remove_e_unit(uint8_t i) {
    _XL_DELETE(e_units[i].x, e_units[i].y);
    e_units[i].type = UNIT_NONE;
    e_uc--;
}

void update_player_units(void) {
    uint8_t i;

    for (i = 0; i < MAX_UNITS; i++) {
        if (p_units[i].type == UNIT_NONE) continue;

        p_units[i].m_timer++;
        p_units[i].s_timer++;

        if (p_units[i].m_timer >= get_move(p_units[i].type)) {
            p_units[i].m_timer = 0;
            _XL_DELETE(p_units[i].x, p_units[i].y);
            if (p_units[i].y > 1) {
                p_units[i].y--;
                draw_unit(p_units[i].x, p_units[i].y, p_units[i].type, 0);
            } else {
                /* Hit enemy wall */
                e_wall_hp--;
                remove_p_unit(i);
                _XL_EXPLOSION_SOUND();
                hud_dirty = 1;
                if (e_wall_hp == 0) state = 1;
            }
        }

        if (p_units[i].type != UNIT_NONE && p_units[i].s_timer >= get_shoot(p_units[i].type)) {
            p_units[i].s_timer = 0;
            player_shoot(i);
        }
    }
}

void update_enemy_units(void) {
    uint8_t i;

    for (i = 0; i < MAX_ENEMY_UNITS; i++) {
        if (e_units[i].type == UNIT_NONE) continue;

        e_units[i].m_timer++;
        e_units[i].s_timer++;

        if (e_units[i].m_timer >= get_move(e_units[i].type)) {
            e_units[i].m_timer = 0;
            _XL_DELETE(e_units[i].x, e_units[i].y);
            if (e_units[i].y < YSize - 2) {
                e_units[i].y++;
                draw_unit(e_units[i].x, e_units[i].y, e_units[i].type, 1);
            } else {
                /* Hit player wall */
                p_wall_hp--;
                remove_e_unit(i);
                _XL_ZAP_SOUND();
                hud_dirty = 1;
                if (p_wall_hp == 0) state = 2;
            }
        }

        if (e_units[i].type != UNIT_NONE && e_units[i].s_timer >= get_shoot(e_units[i].type)) {
            e_units[i].s_timer = 0;
            enemy_shoot(i);
        }
    }
}

void update_player_proj(void) {
    uint8_t i;
    uint8_t j;
    uint8_t nx;
    uint8_t ny;
    uint8_t hit;
    uint8_t bonus;
    uint8_t et;

    for (i = 0; i < MAX_PROJ; i++) {
        if (p_proj[i].y == 0xFF) continue;

        _XL_DELETE(p_proj[i].x, p_proj[i].y);

        nx = p_proj[i].x;
        ny = p_proj[i].y;

        if (p_proj[i].dir == 1) {
            if (nx > 0) nx--;
        } else if (p_proj[i].dir == 2) {
            if (nx < XSize - 1) nx++;
        }
        if (ny > 1) ny--;

        /* Hit enemy wall */
        if (ny <= 1) {
            e_wall_hp--;
            p_proj[i].y = 0xFF;
            p_pc--;
            _XL_EXPLOSION_SOUND();
            hud_dirty = 1;
            if (e_wall_hp == 0) state = 1;
            continue;
        }

        /* Collision with enemy units */
        hit = 0;
        for (j = 0; j < MAX_ENEMY_UNITS; j++) {
            if (e_units[j].type == UNIT_NONE) continue;
            if (e_units[j].x == nx && e_units[j].y == ny) {
                et = e_units[j].type;
                e_units[j].hp--;
                if (e_units[j].hp == 0) {
                    if (et == UNIT_INFANTRY) bonus = BONUS_INFANTRY;
                    else if (et == UNIT_TANK) bonus = BONUS_TANK;
                    else bonus = BONUS_HELICOPTER;
                    budget += bonus;
                    remove_e_unit(j);
                    _XL_TOCK_SOUND();
                    hud_dirty = 1;
                } else {
                    _XL_TICK_SOUND();
                }
                hit = 1;
                break;
            }
        }

        if (hit) {
            p_proj[i].y = 0xFF;
            p_pc--;
            continue;
        }

        p_proj[i].x = nx;
        p_proj[i].y = ny;
        draw_pproj(nx, ny);
    }
}

void update_enemy_proj(void) {
    uint8_t i;
    uint8_t j;
    uint8_t nx;
    uint8_t ny;
    uint8_t hit;

    for (i = 0; i < MAX_ENEMY_PROJ; i++) {
        if (e_proj[i].y == 0xFF) continue;

        _XL_DELETE(e_proj[i].x, e_proj[i].y);

        nx = e_proj[i].x;
        ny = e_proj[i].y;

        if (e_proj[i].dir == 1) {
            if (nx > 0) nx--;
        } else if (e_proj[i].dir == 2) {
            if (nx < XSize - 1) nx++;
        }
        if (ny < YSize - 2) ny++;

        /* Hit player wall */
        if (ny >= YSize - 2) {
            p_wall_hp--;
            e_proj[i].y = 0xFF;
            e_pc--;
            _XL_ZAP_SOUND();
            hud_dirty = 1;
            if (p_wall_hp == 0) state = 2;
            continue;
        }

        /* Collision with player units */
        hit = 0;
        for (j = 0; j < MAX_UNITS; j++) {
            if (p_units[j].type == UNIT_NONE) continue;
            if (p_units[j].x == nx && p_units[j].y == ny) {
                p_units[j].hp--;
                if (p_units[j].hp == 0) {
                    remove_p_unit(j);
                    _XL_EXPLOSION_SOUND();
                } else {
                    _XL_TICK_SOUND();
                }
                hit = 1;
                break;
            }
        }

        if (hit) {
            e_proj[i].y = 0xFF;
            e_pc--;
            continue;
        }

        e_proj[i].x = nx;
        e_proj[i].y = ny;
        draw_eproj(nx, ny);
    }
}

void draw_cursor(void) {
    _XL_DRAW(sel_x, 1, TILE_CURSOR, C_SEL);
}

void game_over_screen(uint8_t won) {
    uint8_t input;

    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(won ? _XL_GREEN : _XL_RED);
    if (won) {
        _XL_PRINT(0, YSize / 2, "VICTORY!");
        _XL_PRINT(0, YSize / 2 + 2, "ENEMY WALL DESTROYED");
    } else {
        _XL_PRINT(0, YSize / 2, "DEFEAT!");
        _XL_PRINT(0, YSize / 2 + 2, "YOUR WALL IS GONE");
    }
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, YSize / 2 + 4, "PRESS FIRE TO RESTART");

    while (1) {
        input = _XL_INPUT();
        if (_XL_FIRE(input)) break;
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

void main(void) {
    uint8_t input;
    uint8_t i;
    uint8_t old_sel_x;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    init_game();
    draw_cursor();

    while (1) {
        if (state != 0) {
            game_over_screen(state == 1);
            init_game();
            draw_cursor();
            continue;
        }

        /* Input */
        input = _XL_INPUT();

        if (_XL_LEFT(input)) {
            old_sel_x = sel_x;
            if (sel_x > 0) sel_x--;
            if (sel_x != old_sel_x) {
                _XL_DELETE(old_sel_x, 1);
                draw_cursor();
            }
        }
        if (_XL_RIGHT(input)) {
            old_sel_x = sel_x;
            if (sel_x < XSize - 1) sel_x++;
            if (sel_x != old_sel_x) {
                _XL_DELETE(old_sel_x, 1);
                draw_cursor();
            }
        }
        if (_XL_UP(input)) {
            if (sel_type == UNIT_INFANTRY) sel_type = UNIT_TANK;
            else if (sel_type == UNIT_TANK) sel_type = UNIT_HELICOPTER;
            else sel_type = UNIT_INFANTRY;
            hud_dirty = 1;
            _XL_TICK_SOUND();
        }
        if (_XL_DOWN(input)) {
            if (sel_type == UNIT_HELICOPTER) sel_type = UNIT_TANK;
            else if (sel_type == UNIT_TANK) sel_type = UNIT_INFANTRY;
            else sel_type = UNIT_HELICOPTER;
            hud_dirty = 1;
            _XL_TICK_SOUND();
        }
        if (_XL_FIRE(input)) {
            deploy_unit();
        }

        /* Game update */
        tick_ct++;

        /* Budget grows slowly */
        bud_timer++;
        if (bud_timer >= BUDGET_INTERVAL) {
            bud_timer = 0;
            budget++;
            hud_dirty = 1;
        }

        /* Spawn enemies */
        e_spawn_t++;
        if (e_spawn_t >= 15) {
            e_spawn_t = 0;
            spawn_enemy();
        }

        /* Update units */
        update_player_units();
        update_enemy_units();

        /* Update projectiles */
        update_player_proj();
        update_enemy_proj();

        /* Redraw HUD if dirty */
        if (hud_dirty) {
            draw_hud();
            hud_dirty = 0;
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}