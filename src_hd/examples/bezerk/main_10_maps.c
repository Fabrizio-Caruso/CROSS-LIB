#include "cross_lib.h"

#define NUM_LEVELS 10
#define MAX_ALIENS 6
#define START_LIVES 3

/* --- static maps (one per level) --- */
static uint8_t map0[XSize][YSize];
static uint8_t map1[XSize][YSize];
static uint8_t map2[XSize][YSize];
static uint8_t map3[XSize][YSize];
static uint8_t map4[XSize][YSize];
static uint8_t map5[XSize][YSize];
static uint8_t map6[XSize][YSize];
static uint8_t map7[XSize][YSize];
static uint8_t map8[XSize][YSize];
static uint8_t map9[XSize][YSize];
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
int8_t b_dir_x, b_dir_y;   /* player facing */
int8_t bb_dx, bb_dy;       /* locked bullet direction */

uint16_t score;
uint8_t level_idx;
uint8_t game_over;

void build_maps(void) {
    uint8_t x, y;

    /* --- map0: plus shape in center --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map0[x][y] = 0;
    for (y = 3; y < YSize - 3; y++)
        map0[XSize / 2][y] = 1;
    for (x = 3; x < XSize - 3; x++)
        map0[x][YSize / 2] = 1;

    /* --- map1: two vertical bars --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map1[x][y] = 0;
    for (y = 2; y < YSize - 2; y++) {
        map1[XSize / 3][y] = 1;
        map1[2 * XSize / 3][y] = 1;
    }

    /* --- map2: four corner pillars + center block --- */
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

    /* --- map3: horizontal zigzag rows --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map3[x][y] = 0;
    for (y = 2; y < YSize - 2; y += 4) {
        uint8_t start = ((y / 4) % 2 == 0) ? XSize / 4 : 3 * XSize / 4;
        uint8_t len   = XSize / 6;
        for (x = start; x < start + len && x < XSize - 1; x++)
            map3[x][y] = 1;
    }

    /* --- map4: two horizontal bars at thirds --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map4[x][y] = 0;
    for (x = 2; x < XSize - 2; x++) {
        map4[x][YSize / 3] = 1;
        map4[x][2 * YSize / 3] = 1;
    }

    /* --- map5: concentric frame with gaps --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map5[x][y] = 0;
    /* outer ring with gap in middle of each side */
    for (x = 2; x < XSize - 2; x++) {
        if (x > XSize / 2 - 4 && x < XSize / 2 + 4) continue;
        map5[x][3] = 1;
        map5[x][YSize - 4] = 1;
    }
    for (y = 3; y < YSize - 3; y++) {
        if (y > YSize / 2 - 3 && y < YSize / 2 + 3) continue;
        map5[3][y] = 1;
        map5[XSize - 4][y] = 1;
    }
    /* inner smaller ring */
    for (x = XSize / 4; x < 3 * XSize / 4; x++) {
        if (x > XSize / 2 - 3 && x < XSize / 2 + 3) continue;
        map5[x][YSize / 2] = 1;
    }
    for (y = YSize / 4; y < YSize - YSize / 4; y++) {
        if (y > YSize / 2 - 2 && y < YSize / 2 + 2) continue;
        map5[XSize / 2][y] = 1;
    }

    /* --- map6: two crossing diagonals --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map6[x][y] = 0;
    for (y = 2; y < YSize - 2; y++) {
        uint8_t dx1 = 2 + y * 3 / YSize;         /* left-to-right diag */
        uint8_t dx2 = XSize - 3 - y * 3 / YSize; /* right-to-left diag */
        if (dx1 < XSize) map6[dx1][y] = 1;
        if (dx2 < XSize) map6[dx2][y] = 1;
    }

    /* --- map7: grid of small blocks --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map7[x][y] = 0;
    for (uint8_t bx2 = 4; bx2 < XSize - 3; bx2 += 10) {
        for (uint8_t by2 = 4; by2 < YSize - 3; by2 += 6) {
            map7[bx2][by2] = 1;
            map7[bx2 + 1][by2] = 1;
            map7[bx2][by2 + 1] = 1;
            if (bx2 + 2 < XSize) map7[bx2 + 2][by2] = 1;
        }
    }

    /* --- map8: T-shaped corridors --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map8[x][y] = 0;
    /* horizontal spine */
    for (x = 4; x < XSize - 4; x++)
        map8[x][YSize / 2] = 1;
    /* vertical stems at quarters */
    for (uint8_t q = 1; q <= 3; q++) {
        uint8_t cx = q * XSize / 4;
        for (y = 2; y < YSize - 2; y++) {
            if (y == YSize / 2) continue;
            map8[cx][y] = 1;
        }
    }

    /* --- map9: open center with frame + cross --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map9[x][y] = 0;
    /* border frame (inset by 2) */
    for (x = 2; x < XSize - 2; x++) {
        if (x > XSize / 2 - 5 && x < XSize / 2 + 5) continue;
        map9[x][2] = 1;
        map9[x][YSize - 3] = 1;
    }
    for (y = 2; y < YSize - 2; y++) {
        if (y > YSize / 2 - 4 && y < YSize / 2 + 4) continue;
        map9[2][y] = 1;
        map9[XSize - 3][y] = 1;
    }
    /* cross through center */
    for (y = 6; y < YSize - 6; y++)
        map9[XSize / 2][y] = 1;
    for (x = 8; x < XSize - 8; x++)
        if (!(x > XSize / 2 - 5 && x < XSize / 2 + 5))
            map9[x][YSize / 2] = 1;

    active_map = map0;
}

void setup_level(void) {
    uint8_t i, attempts;

    build_maps();

    switch (level_idx % NUM_LEVELS) {
        case 0:   active_map = map0; break;
        case 1:   active_map = map1; break;
        case 2:   active_map = map2; break;
        case 3:   active_map = map3; break;
        case 4:   active_map = map4; break;
        case 5:   active_map = map5; break;
        case 6:   active_map = map6; break;
        case 7:   active_map = map7; break;
        case 8:   active_map = map8; break;
        default:  active_map = map9; break;
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
    for (i = 0; i < MAX_ALIENS && num_alien < MAX_ALIENS && attempts < 300; ) {
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

    uint8_t sx = px + (uint8_t)b_dir_x;
    uint8_t sy = py + (uint8_t)b_dir_y;

    if (sx >= XSize || sy >= YSize) return;
    if (active_map[sx][sy])         return;

    bx = sx;
    by = sy;
    bullet_active = 1;
    bb_dx = b_dir_x;
    bb_dy = b_dir_y;
    _XL_DRAW(bx, by, _TILE_4, _XL_YELLOW);
    _XL_SHOOT_SOUND();
}

void move_bullet(void) {
    if (!bullet_active) return;

    uint8_t nx = bx + (uint8_t)bb_dx;
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
