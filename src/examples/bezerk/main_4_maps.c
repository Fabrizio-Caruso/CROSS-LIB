#include "cross_lib.h"

#define NUM_LEVELS 4
#define MAX_ALIENS 6
#define START_LIVES 3

/* --- static maps (one per level) --- */
static uint8_t map0[XSize][YSize];
static uint8_t map1[XSize][YSize];
static uint8_t map2[XSize][YSize];
static uint8_t map3[XSize][YSize];
static uint8_t (*active_map)[YSize];

/* --- player --- */
uint8_t px, py;
uint8_t lives;

/* --- aliens --- */
uint8_t ax[MAX_ALIENS], ay[MAX_ALIENS];
uint8_t num_alien;

/* --- bullet (one active at a time) --- */
uint8_t bx, by;
uint8_t bullet_active;
int8_t b_dir_x, b_dir_y;   /* player facing (changes with movement) */
int8_t bb_dx, bb_dy;       /* bullet direction (fixed once fired) */

uint16_t score;
uint8_t level_idx;
uint8_t game_over;

void build_maps(void) {
    uint8_t x, y;

    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map0[x][y] = 0;
    for (y = 3; y < YSize - 3; y++)
        map0[XSize / 2][y] = 1;
    for (x = 3; x < XSize - 3; x++)
        map0[x][YSize / 2] = 1;

    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map1[x][y] = 0;
    for (y = 2; y < YSize - 2; y++) {
        map1[XSize / 3][y] = 1;
        map1[2 * XSize / 3][y] = 1;
    }

    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map2[x][y] = 0;
    for (y = 3; y < YSize / 2 - 1; y++) {
        map2[3][y] = 1;
        map2[XSize - 4][y] = 1;
    }
    for (y = YSize / 2 + 1; y < YSize - 3; y++) {
        map2[3][y] = 1;
        map2[XSize - 4][y] = 1;
    }
    for (x = XSize / 2 - 3; x <= XSize / 2 + 3 && x < XSize; x++)
        for (y = YSize / 2 - 2; y <= YSize / 2 + 2; y++)
            map2[x][y] = 1;

    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map3[x][y] = 0;
    for (y = 2; y < YSize - 2; y += 4) {
        uint8_t start = ((y / 4) % 2 == 0) ? XSize / 4 : 3 * XSize / 4;
        uint8_t len   = XSize / 3;
        for (x = start; x < start + len && x < XSize - 1; x++)
            map3[x][y] = 1;
    }

    active_map = map0;
}

void setup_level(void) {
    uint8_t i, attempts;

    build_maps();

    switch (level_idx % NUM_LEVELS) {
        case 0:   active_map = map0; break;
        case 1:   active_map = map1; break;
        case 2:   active_map = map2; break;
        default:  active_map = map3; break;
    }

    _XL_CLEAR_SCREEN();
    for (uint8_t y = 0; y < YSize; y++)
        for (uint8_t x = 0; x < XSize; x++)
            if (active_map[x][y])
                _XL_DRAW(x, y, _TILE_1, _XL_RED);

    px = 2;
    py = YSize - 3;
    b_dir_x = 0;
    b_dir_y = -1;
    _XL_DRAW(px, py, _TILE_2, _XL_CYAN);

    num_alien = 0;
    attempts = 0;
    for (i = 0; i < MAX_ALIENS && num_alien < MAX_ALIENS && attempts < 200; ) {
        uint16_t rx = _XL_RAND() % XSize;
        uint16_t ry = _XL_RAND() % YSize;
        if (!active_map[rx][ry] && !(rx == px && ry == py)) {
            ax[num_alien] = rx;
            ay[num_alien] = ry;
            _XL_DRAW(ax[num_alien], ay[num_alien], _TILE_3, _XL_GREEN);
            num_alien++;
        } else {
            attempts++;
        }
    }

    bullet_active = 0;
}

void fire_bullet(void) {
    if (bullet_active) return;

    /* spawn one cell ahead of player in current facing */
    uint8_t sx = px + (uint8_t)b_dir_x;
    uint8_t sy = py + (uint8_t)b_dir_y;

    if (sx >= XSize || sy >= YSize) return;
    if (active_map[sx][sy])         return;

    bx = sx;
    by = sy;
    bullet_active = 1;
    bb_dx = b_dir_x;   /* lock direction at fire time */
    bb_dy = b_dir_y;
    _XL_DRAW(bx, by, _TILE_4, _XL_YELLOW);
    _XL_SHOOT_SOUND();
}

void move_bullet(void) {
    if (!bullet_active) return;

    uint8_t nx = bx + (uint8_t)bb_dx;   /* use locked bullet direction */
    uint8_t ny = by + (uint8_t)bb_dy;

    if (nx >= XSize || ny >= YSize) {
        _XL_DELETE(bx, by);
        bullet_active = 0;
        return;
    }

    if (active_map[nx][ny]) {
        _XL_DELETE(bx, by);
        bullet_active = 0;
        return;
    }

    for (uint8_t i = 0; i < num_alien; i++) {
        if (nx == ax[i] && ny == ay[i]) {
            _XL_DELETE(bx, by);
            _XL_DELETE(ax[i], ay[i]);
            bullet_active = 0;
            score += 100;
            for (uint8_t j = i; j < num_alien - 1; j++) {
                ax[j] = ax[j + 1];
                ay[j] = ay[j + 1];
            }
            num_alien--;
            _XL_EXPLOSION_SOUND();

            if (num_alien == 0) {
                level_idx++;
                setup_level();
            }
            return;
        }
    }

    _XL_DELETE(bx, by);
    bx = nx;
    by = ny;
    _XL_DRAW(bx, by, _TILE_4, _XL_YELLOW);
}

void move_alien(uint8_t i) {
    uint16_t r = _XL_RAND() % 4;
    int8_t dx = 0, dy = 0;
    switch (r) {
        case 0: dx = -1; break;
        case 1: dx =  1; break;
        case 2: dy = -1; break;
        default:dy =  1; break;
    }

    uint8_t nx = ax[i] + (uint8_t)dx;
    uint8_t ny = ay[i] + (uint8_t)dy;

    if (nx >= XSize || ny >= YSize) return;
    if (active_map[nx][ny])         return;

    if (nx == px && ny == py) {
        lives--;
        _XL_DELETE(ax[i], ay[i]);
        _XL_DELETE(px, py);
        _XL_ZAP_SOUND();

        if (lives > 0) {
            setup_level();
        } else {
            game_over = 1;
        }
        return;
    }

    for (uint8_t j = 0; j < num_alien; j++) {
        if (j != i && nx == ax[j] && ny == ay[j]) return;
    }

    _XL_DELETE(ax[i], ay[i]);
    ax[i] = nx;
    ay[i] = ny;
    _XL_DRAW(ax[i], ay[i], _TILE_3, _XL_GREEN);
}

void update_player(uint8_t input) {
    int8_t dx = 0, dy = 0;

    if (_XL_LEFT(input))     { dx = -1; b_dir_x = -1; b_dir_y = 0; }
    else if (_XL_RIGHT(input)){ dx =  1; b_dir_x =  1; b_dir_y = 0; }
    else if (_XL_UP(input))   { dy = -1; b_dir_x = 0; b_dir_y = -1; }
    else if (_XL_DOWN(input)) { dy =  1; b_dir_x = 0; b_dir_y =  1; }

    if (dx == 0 && dy == 0) return;

    uint8_t nx = px + (uint8_t)dx;
    uint8_t ny = py + (uint8_t)dy;

    if (nx >= XSize || ny >= YSize) return;
    if (active_map[nx][ny])         return;

    _XL_DELETE(px, py);
    px = nx;
    py = ny;
    _XL_DRAW(px, py, _TILE_2, _XL_CYAN);
}

void game_over_screen(void) {
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(0, YSize / 2 - 1, "GAME OVER");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(XSize / 4, YSize / 2 + 1, 2, score);

    _XL_WAIT_FOR_INPUT();
}

int main(void) {
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        score = 0;
        lives = START_LIVES;
        level_idx = 0;
        game_over = 0;

        setup_level();

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0, YSize - 1, 2, score);

        while (!game_over) {
            input = _XL_INPUT();

            update_player(input);

            if (_XL_FIRE(input))
                fire_bullet();

            move_bullet();

            for (uint8_t i = 0; i < num_alien && !game_over; i++)
                move_alien(i);

            _XL_DELETE(0, YSize - 1);
            _XL_PRINTD(0, YSize - 1, 2, score);

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 3);
        }

        game_over_screen();
    }

    return 0;
}
