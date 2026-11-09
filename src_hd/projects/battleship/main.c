#include "cross_lib.h"

#define MAP_SIZE 10
#define MAP_OX 2
#define MAP_OY 2

#define T_WATER    _TILE_0
#define T_SHIP     _TILE_1
#define T_HIT      _TILE_2
#define T_MISS     _TILE_3
#define T_CURSOR   _TILE_5
#define T_PLACE_OK _TILE_6
#define T_PLACE_BAD _TILE_7

#define MAX_SHIPS 5
#define MAX_LEN   4

#define D_EASY 1
#define D_MED  2
#define D_HARD 3

/* ---------- Globals ---------- */
uint8_t pgrid[MAP_SIZE][MAP_SIZE];
uint8_t egrid[MAP_SIZE][MAP_SIZE];
uint8_t pshot[MAP_SIZE][MAP_SIZE];
uint8_t eshot[MAP_SIZE][MAP_SIZE];

uint8_t pcells[MAX_SHIPS][MAX_LEN][2];
uint8_t ecells[MAX_SHIPS][MAX_LEN][2];
uint8_t plen[MAX_SHIPS];
uint8_t elen[MAX_SHIPS];
uint8_t phits[MAX_SHIPS];
uint8_t ehits[MAX_SHIPS];

uint8_t nships;
uint8_t slens[MAX_SHIPS];
uint8_t cx, cy;
uint8_t in_player_view;
uint8_t diff;
uint8_t sel;

/* Placement state */
uint8_t ship_idx;
uint8_t ship_h;
uint8_t ship_cx, ship_cy;

/* ---------- Helpers ---------- */

uint8_t in_map(uint8_t x, uint8_t y)
{
    if (x >= MAP_OX && x < (uint8_t)(MAP_OX + MAP_SIZE) &&
        y >= MAP_OY && y < (uint8_t)(MAP_OY + MAP_SIZE))
        return 1;
    return 0;
}

/* ---------- Cell rendering (direct, no buffer) ---------- */

void draw_pcell(uint8_t r, uint8_t c)
{
    uint8_t sx, sy, tile, color;
    sx = (uint8_t)(MAP_OX + c);
    sy = (uint8_t)(MAP_OY + r);
    if (pshot[r][c] == 2)      { tile = T_HIT;  color = _XL_RED; }
    else if (pshot[r][c] == 1) { tile = T_MISS; color = _XL_CYAN; }
    else if (pgrid[r][c] == 1) { tile = T_SHIP; color = _XL_WHITE; }
    else                       { tile = T_WATER; color = _XL_BLUE; }
    _XL_DRAW(sx, sy, tile, color);
}

void draw_ecell(uint8_t r, uint8_t c)
{
    uint8_t sx, sy, tile, color;
    sx = (uint8_t)(MAP_OX + c);
    sy = (uint8_t)(MAP_OY + r);
    if (eshot[r][c] == 2)      { tile = T_HIT;  color = _XL_RED; }
    else if (eshot[r][c] == 1) { tile = T_MISS; color = _XL_CYAN; }
    else                       { tile = T_WATER; color = _XL_BLUE; }
    _XL_DRAW(sx, sy, tile, color);
}

/* Base tile for player grid (ignores current placement overlay) */
void draw_pcell_base(uint8_t r, uint8_t c)
{
    uint8_t sx, sy;
    sx = (uint8_t)(MAP_OX + c);
    sy = (uint8_t)(MAP_OY + r);
    if (pgrid[r][c] == 1) _XL_DRAW(sx, sy, T_SHIP, _XL_WHITE);
    else                  _XL_DRAW(sx, sy, T_WATER, _XL_BLUE);
}

void render_full_player(void)
{
    uint8_t i, j;
    for (i = 0; i < MAP_SIZE; i++)
        for (j = 0; j < MAP_SIZE; j++)
            draw_pcell(i, j);
}

void render_full_enemy(void)
{
    uint8_t i, j;
    for (i = 0; i < MAP_SIZE; i++)
        for (j = 0; j < MAP_SIZE; j++)
            draw_ecell(i, j);
}

/* ---------- Cursor (no traces) ---------- */

void erase_cursor(uint8_t x, uint8_t y)
{
    if (in_map(x, y)) {
        uint8_t r = (uint8_t)(y - MAP_OY);
        uint8_t c = (uint8_t)(x - MAP_OX);
        if (in_player_view) draw_pcell(r, c);
        else                draw_ecell(r, c);
    } else {
        _XL_DELETE(x, y);
    }
}

void draw_cursor_at(uint8_t x, uint8_t y)
{
    if (in_map(x, y)) {
        _XL_DRAW(x, y, T_CURSOR, _XL_YELLOW);
    } else {
        _XL_CHAR(x, y, '>');
    }
}

void move_cursor(uint8_t ox, uint8_t oy, uint8_t nx, uint8_t ny)
{
    erase_cursor(ox, oy);
    draw_cursor_at(nx, ny);
}

/* ---------- Ship placement ---------- */

void ship_cell(uint8_t i, uint8_t *r, uint8_t *c)
{
    if (ship_h == 0) {
        *r = ship_cy;
        *c = (uint8_t)(ship_cx + i);
    } else {
        *r = (uint8_t)(ship_cy + i);
        *c = ship_cx;
    }
}

uint8_t ship_valid(void)
{
    uint8_t i, r, c, slen;
    slen = slens[ship_idx];
    for (i = 0; i < slen; i++) {
        ship_cell(i, &r, &c);
        if (r >= MAP_SIZE || c >= MAP_SIZE || pgrid[r][c] != 0)
            return 0;
    }
    return 1;
}

void erase_placement_ship(void)
{
    uint8_t i, r, c, slen;
    slen = slens[ship_idx];
    for (i = 0; i < slen; i++) {
        ship_cell(i, &r, &c);
        if (r < MAP_SIZE && c < MAP_SIZE)
            draw_pcell_base(r, c);
    }
}

void draw_placement_ship(void)
{
    uint8_t i, r, c, sx, sy, slen, valid;
    slen = slens[ship_idx];
    valid = ship_valid();
    for (i = 0; i < slen; i++) {
        ship_cell(i, &r, &c);
        if (r < MAP_SIZE && c < MAP_SIZE) {
            sx = (uint8_t)(MAP_OX + c);
            sy = (uint8_t)(MAP_OY + r);
            if (valid) _XL_DRAW(sx, sy, T_PLACE_OK, _XL_GREEN);
            else       _XL_DRAW(sx, sy, T_PLACE_BAD, _XL_RED);
        }
    }
}

void place_ship_confirm(void)
{
    uint8_t i, r, c, slen;
    slen = slens[ship_idx];
    for (i = 0; i < slen; i++) {
        ship_cell(i, &r, &c);
        pgrid[r][c] = 1;
        pcells[ship_idx][i][0] = c;
        pcells[ship_idx][i][1] = r;
    }
    plen[ship_idx] = slen;
    phits[ship_idx] = 0;
}

/* ---------- AI ship placement ---------- */

void place_one_ai(uint8_t grid[MAP_SIZE][MAP_SIZE],
                  uint8_t cells[MAX_SHIPS][MAX_LEN][2],
                  uint8_t len_arr[MAX_SHIPS],
                  uint8_t sid, uint8_t slen)
{
    uint8_t placed, x, y, dir, i, tx, ty, ok, guard;

    guard = 0;
    do {
        placed = 0;
        x   = (uint8_t)(_XL_RAND() % MAP_SIZE);
        y   = (uint8_t)(_XL_RAND() % MAP_SIZE);
        dir = (uint8_t)(_XL_RAND() % 2);

        ok = 1;
        for (i = 0; i < slen; i++) {
            tx = x;
            ty = y;
            if (dir == 0) tx = (uint8_t)(x + i);
            else          ty = (uint8_t)(y + i);
            if (tx >= MAP_SIZE || ty >= MAP_SIZE || grid[ty][tx] != 0) {
                ok = 0;
                break;
            }
        }

        if (ok) {
            for (i = 0; i < slen; i++) {
                tx = x;
                ty = y;
                if (dir == 0) tx = (uint8_t)(x + i);
                else          ty = (uint8_t)(y + i);
                grid[ty][tx] = 1;
                cells[sid][i][0] = tx;
                cells[sid][i][1] = ty;
            }
            len_arr[sid] = slen;
            placed = 1;
        }
        guard++;
    } while (!placed && guard < 500);
}

/* ---------- Game state ---------- */

void setup_difficulty(uint8_t d)
{
    diff = d;
    if (d == D_EASY) {
        nships = 3;
        slens[0] = 2; slens[1] = 2; slens[2] = 1;
    } else if (d == D_MED) {
        nships = 4;
        slens[0] = 3; slens[1] = 2; slens[2] = 2; slens[3] = 1;
    } else {
        nships = 5;
        slens[0] = 4; slens[1] = 3; slens[2] = 2; slens[3] = 2; slens[4] = 1;
    }
}

void init_game(void)
{
    uint8_t i, j;
    for (i = 0; i < MAP_SIZE; i++)
        for (j = 0; j < MAP_SIZE; j++) {
            pgrid[i][j]  = 0;
            egrid[i][j]  = 0;
            pshot[i][j]  = 0;
            eshot[i][j]  = 0;
        }
    for (i = 0; i < MAX_SHIPS; i++) {
        phits[i] = 0;
        ehits[i] = 0;
        plen[i]  = 0;
        elen[i]  = 0;
    }
    for (i = 0; i < nships; i++) {
        place_one_ai(egrid, ecells, elen, i, slens[i]);
    }
    cx = (uint8_t)(MAP_OX + MAP_SIZE / 2);
    cy = (uint8_t)(MAP_OY + MAP_SIZE / 2);
    in_player_view = 0;
}

/* ---------- Ship hit tracking ---------- */

uint8_t find_ship(uint8_t cells[MAX_SHIPS][MAX_LEN][2],
                  uint8_t len_arr[MAX_SHIPS],
                  uint8_t mx, uint8_t my)
{
    uint8_t i, j;
    for (i = 0; i < nships; i++)
        for (j = 0; j < len_arr[i]; j++)
            if (cells[i][j][0] == mx && cells[i][j][1] == my)
                return i;
    return 255;
}

uint8_t all_sunk(uint8_t hits[MAX_SHIPS], uint8_t len_arr[MAX_SHIPS])
{
    uint8_t i;
    for (i = 0; i < nships; i++)
        if (hits[i] < len_arr[i]) return 0;
    return 1;
}

/* ---------- Labels & status ---------- */

void render_labels(void)
{
    uint8_t i, ch;
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    for (i = 0; i < MAP_SIZE; i++) {
        ch = (char)('A' + i);
        _XL_CHAR((uint8_t)(MAP_OX + i), (uint8_t)(MAP_OY - 1), ch);
    }
    for (i = 0; i < MAP_SIZE; i++) {
        ch = (char)('1' + i);
        _XL_CHAR((uint8_t)(MAP_OX - 1), (uint8_t)(MAP_OY + i), ch);
    }
}

void render_status_gameplay(void)
{
    uint8_t i, sunk_e, sunk_p, sy;
    sunk_e = 0;
    sunk_p = 0;
    for (i = 0; i < nships; i++) {
        if (ehits[i] >= elen[i]) sunk_e++;
        if (phits[i] >= plen[i]) sunk_p++;
    }
    sy = (uint8_t)(MAP_OY + MAP_SIZE + 1);

    _XL_SET_TEXT_COLOR(_XL_GREEN);
    if (in_player_view)
        _XL_PRINT(MAP_OX, sy, "YOUR MAP");
    else
        _XL_PRINT(MAP_OX, sy, "ENEMY MAP");

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    sy = (uint8_t)(MAP_OY + MAP_SIZE + 2);
    _XL_PRINT(MAP_OX, sy, "E:S ");
    _XL_PRINTD((uint8_t)(MAP_OX + 5), sy, 1, sunk_e);
    _XL_PRINT((uint8_t)(MAP_OX + 6), sy, "/");
    _XL_PRINTD((uint8_t)(MAP_OX + 7), sy, 1, nships);
    _XL_PRINT((uint8_t)(MAP_OX + 9), sy, " Y:S ");
    _XL_PRINTD((uint8_t)(MAP_OX + 14), sy, 1, sunk_p);
    _XL_PRINT((uint8_t)(MAP_OX + 15), sy, "/");
    _XL_PRINTD((uint8_t)(MAP_OX + 16), sy, 1, nships);
}

void render_status_placement(void)
{
    uint8_t sy;
    sy = (uint8_t)(MAP_OY + MAP_SIZE + 1);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(MAP_OX, sy, "PLACE SHIP ");
    _XL_PRINTD((uint8_t)(MAP_OX + 12), sy, 1, (uint16_t)(ship_idx + 1));
    _XL_PRINT((uint8_t)(MAP_OX + 13), sy, " OF ");
    _XL_PRINTD((uint8_t)(MAP_OX + 16), sy, 1, (uint16_t)nships);
    _XL_PRINT((uint8_t)(MAP_OX + 17), sy, " LEN ");
    _XL_PRINTD((uint8_t)(MAP_OX + 22), sy, 1, slens[ship_idx]);

    sy = (uint8_t)(MAP_OY + MAP_SIZE + 2);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    if (ship_h == 0)
        _XL_PRINT(MAP_OX, sy, "HORIZONTAL  L/R MOVE  UP ROT  FIRE OK");
    else
        _XL_PRINT(MAP_OX, sy, "VERTICAL    L/R MOVE  UP ROT  FIRE OK");
}

/* ---------- Title screen ---------- */

void render_title(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 0, "BATTLESHIP");

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(2, 2, "SELECT LEVEL");

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    if (sel == 0) _XL_PRINT(2, 4, ">EASY");
    else          _XL_PRINT(3, 4, " EASY");

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    if (sel == 1) _XL_PRINT(2, 5, ">MEDIUM");
    else          _XL_PRINT(3, 5, " MEDIUM");

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    if (sel == 2) _XL_PRINT(2, 6, ">HARD");
    else          _XL_PRINT(3, 6, " HARD");

    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(2, 8, "FIRE TO START");
}

void title_screen(void)
{
    uint8_t input;
    sel = 0;
    while (1) {
        _XL_CLEAR_SCREEN();
        render_title();
        input = _XL_INPUT();
        if (_XL_UP(input) || _XL_LEFT(input)) {
            if (sel > 0) sel--;
            _XL_TICK_SOUND();
        } else if (_XL_DOWN(input) || _XL_RIGHT(input)) {
            if (sel < 2) sel++;
            _XL_TICK_SOUND();
        } else if (_XL_FIRE(input)) {
            setup_difficulty((uint8_t)(sel + 1));
            return;
        }
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

/* ---------- Placement phase ---------- */

void placement_phase(void)
{
    uint8_t input, valid, i, r, c, slen;

    ship_idx = 0;
    ship_h   = 0;
    ship_cx  = 0;
    ship_cy  = 0;

    while (ship_idx < nships) {
        _XL_CLEAR_SCREEN();
        render_labels();
        render_status_placement();

        /* Draw all already-placed ships and water */
        for (i = 0; i < MAP_SIZE; i++) {
            uint8_t j;
            for (j = 0; j < MAP_SIZE; j++)
                draw_pcell_base(i, j);
        }

        /* Draw current ship being placed */
        draw_placement_ship();

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

        input = _XL_INPUT();
        if (_XL_LEFT(input)) {
            if (ship_cx > 0) {
                erase_placement_ship();
                ship_cx--;
                draw_placement_ship();
                _XL_TICK_SOUND();
            }
        } else if (_XL_RIGHT(input)) {
            slen = slens[ship_idx];
            if (ship_h == 0) {
                if ((uint8_t)(ship_cx + slen) <= MAP_SIZE) {
                    erase_placement_ship();
                    ship_cx++;
                    draw_placement_ship();
                    _XL_TICK_SOUND();
                }
            } else {
                if (ship_cx < (uint8_t)(MAP_SIZE - 1)) {
                    erase_placement_ship();
                    ship_cx++;
                    draw_placement_ship();
                    _XL_TICK_SOUND();
                }
            }
        } else if (_XL_DOWN(input)) {
            slen = slens[ship_idx];
            if (ship_h == 1) {
                if ((uint8_t)(ship_cy + slen) <= MAP_SIZE) {
                    erase_placement_ship();
                    ship_cy++;
                    draw_placement_ship();
                    _XL_TICK_SOUND();
                }
            } else {
                if (ship_cy < (uint8_t)(MAP_SIZE - 1)) {
                    erase_placement_ship();
                    ship_cy++;
                    draw_placement_ship();
                    _XL_TICK_SOUND();
                }
            }
        } else if (_XL_UP(input)) {
            /* Rotate */
            {
                uint8_t old_h = ship_h;
                uint8_t old_cx = ship_cx;
                uint8_t old_cy = ship_cy;
                erase_placement_ship();
                ship_h = (uint8_t)(1 - ship_h);
                /* Adjust position to keep within bounds */
                slen = slens[ship_idx];
                if (ship_h == 0) {
                    if ((uint8_t)(ship_cx + slen) > MAP_SIZE)
                        ship_cx = (uint8_t)(MAP_SIZE - slen);
                    if (ship_cy >= MAP_SIZE)
                        ship_cy = 0;
                } else {
                    if ((uint8_t)(ship_cy + slen) > MAP_SIZE)
                        ship_cy = (uint8_t)(MAP_SIZE - slen);
                    if (ship_cx >= MAP_SIZE)
                        ship_cx = 0;
                }
                draw_placement_ship();
                render_status_placement();
                _XL_TICK_SOUND();
            }
        } else if (_XL_FIRE(input)) {
            valid = ship_valid();
            if (valid) {
                place_ship_confirm();
                ship_idx++;
                if (ship_idx < nships) {
                    ship_h  = 0;
                    ship_cx = 0;
                    ship_cy = 0;
                    _XL_PING_SOUND();
                }
            } else {
                _XL_TOCK_SOUND();
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

/* ---------- Gameplay ---------- */

void player_shoot(uint8_t mx, uint8_t my)
{
    uint8_t sid, sx, sy;

    if (eshot[my][mx] != 0) return;

    sx = (uint8_t)(MAP_OX + mx);
    sy = (uint8_t)(MAP_OY + my);

    if (egrid[my][mx] == 1) {
        eshot[my][mx] = 2;
        sid = find_ship(ecells, elen, mx, my);
        if (sid != 255) ehits[sid]++;
        _XL_DRAW(sx, sy, T_HIT, _XL_RED);
        _XL_PING_SOUND();
    } else {
        eshot[my][mx] = 1;
        _XL_DRAW(sx, sy, T_MISS, _XL_CYAN);
        _XL_TOCK_SOUND();
    }
}

void enemy_shoot(void)
{
    uint8_t mx, my, sid, tries;

    mx = (uint8_t)(_XL_RAND() % MAP_SIZE);
    my = (uint8_t)(_XL_RAND() % MAP_SIZE);
    tries = 0;
    while (pshot[my][mx] != 0 && tries < 200) {
        mx = (uint8_t)(_XL_RAND() % MAP_SIZE);
        my = (uint8_t)(_XL_RAND() % MAP_SIZE);
        tries++;
    }
    if (pshot[my][mx] != 0) return;

    if (pgrid[my][mx] == 1) {
        pshot[my][mx] = 2;
        sid = find_ship(pcells, plen, mx, my);
        if (sid != 255) phits[sid]++;
        _XL_SHOOT_SOUND();
    } else {
        pshot[my][mx] = 1;
        _XL_TICK_SOUND();
    }
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
}

void show_gameover(uint8_t player_won)
{
    _XL_CLEAR_SCREEN();
    if (player_won) {
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(2, 4, "YOU WIN");
        _XL_EXPLOSION_SOUND();
    } else {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(2, 4, "YOU LOSE");
        _XL_ZAP_SOUND();
    }
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 6, "PRESS FIRE TO RESTART");
    _XL_WAIT_FOR_INPUT();
}

void game_screen(void)
{
    uint8_t input, mx, my, player_won, game_over;

    init_game();
    _XL_CLEAR_SCREEN();
    render_labels();
    render_full_enemy();
    render_status_gameplay();
    draw_cursor_at(cx, cy);

    game_over  = 0;
    player_won = 0;

    while (!game_over) {
        input = _XL_INPUT();

        if (_XL_LEFT(input)) {
            if (cx > 0) {
                move_cursor(cx, cy, (uint8_t)(cx - 1), cy);
                cx--;
            }
        } else if (_XL_RIGHT(input)) {
            if (cx < (uint8_t)(XSize - 1)) {
                move_cursor(cx, cy, (uint8_t)(cx + 1), cy);
                cx++;
            }
        } else if (_XL_UP(input)) {
            if (cy > 0) {
                move_cursor(cx, cy, cx, (uint8_t)(cy - 1));
                cy--;
            }
        } else if (_XL_DOWN(input)) {
            if (cy < (uint8_t)(YSize - 1)) {
                move_cursor(cx, cy, cx, (uint8_t)(cy + 1));
                cy++;
            }
        } else if (_XL_FIRE(input)) {
            if (in_map(cx, cy)) {
                /* Fire at enemy */
                mx = (uint8_t)(cx - MAP_OX);
                my = (uint8_t)(cy - MAP_OY);
                player_shoot(mx, my);
                render_status_gameplay();

                if (all_sunk(ehits, elen)) {
                    game_over  = 1;
                    player_won = 1;
                    break;
                }

                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                enemy_shoot();
                render_status_gameplay();

                if (all_sunk(phits, plen)) {
                    game_over  = 1;
                    player_won = 0;
                    break;
                }
            } else {
                /* Toggle view: outside the map */
                if (in_player_view) {
                    in_player_view = 0;
                    render_full_enemy();
                    render_status_gameplay();
                } else {
                    in_player_view = 1;
                    render_full_player();
                    render_status_gameplay();
                }
                _XL_TICK_SOUND();
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    show_gameover(player_won);
}

/* ---------- Main ---------- */

int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        title_screen();
        placement_phase();
        game_screen();
    }

    return 0;
}