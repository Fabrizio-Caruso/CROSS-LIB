#include "cross_lib.h"

#define NUM_LEVELS 32
#define MAX_ALIENS 6
#define START_LIVES 3

/* --- static maps --- */
static uint8_t map0 [XSize][YSize];
static uint8_t map1 [XSize][YSize];
static uint8_t map2 [XSize][YSize];
static uint8_t map3 [XSize][YSize];
static uint8_t map4 [XSize][YSize];
static uint8_t map5 [XSize][YSize];
static uint8_t map6 [XSize][YSize];
static uint8_t map7 [XSize][YSize];
static uint8_t map8 [XSize][YSize];
static uint8_t map9 [XSize][YSize];
static uint8_t map10[XSize][YSize];
static uint8_t map11[XSize][YSize];
static uint8_t map12[XSize][YSize];
static uint8_t map13[XSize][YSize];
static uint8_t map14[XSize][YSize];
static uint8_t map15[XSize][YSize];
static uint8_t map16[XSize][YSize];
static uint8_t map17[XSize][YSize];
static uint8_t map18[XSize][YSize];
static uint8_t map19[XSize][YSize];
static uint8_t map20[XSize][YSize];
static uint8_t map21[XSize][YSize];
static uint8_t map22[XSize][YSize];
static uint8_t map23[XSize][YSize];
static uint8_t map24[XSize][YSize];
static uint8_t map25[XSize][YSize];
static uint8_t map26[XSize][YSize];
static uint8_t map27[XSize][YSize];
static uint8_t map28[XSize][YSize];
static uint8_t map29[XSize][YSize];
static uint8_t map30[XSize][YSize];
static uint8_t map31[XSize][YSize];

static uint8_t (*active_map)[YSize];

/* --- player --- */
uint8_t px, py;
uint8_t lives;

/* --- aliens --- */
uint8_t ax[MAX_ALIENS], ay[MAX_ALIENS];
uint8_t num_alien;

/* --- bullet --- */
uint8_t bx, by;
uint8_t bullet_active;
int8_t b_dir_x, b_dir_y;
int8_t bb_dx, bb_dy;

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

    /* --- map1: two vertical bars at thirds --- */
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

    /* --- map5: concentric frame with gaps + inner cross --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map5[x][y] = 0;
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
        uint8_t dx1 = 2 + y * 3 / YSize;
        uint8_t dx2 = XSize - 3 - y * 3 / YSize;
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
    for (x = 4; x < XSize - 4; x++)
        map8[x][YSize / 2] = 1;
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
    for (y = 6; y < YSize - 6; y++)
        map9[XSize / 2][y] = 1;
    for (x = 8; x < XSize - 8; x++)
        if (!(x > XSize / 2 - 5 && x < XSize / 2 + 5))
            map9[x][YSize / 2] = 1;

    /* --- map10: H-shape (two verticals + horizontal bar) --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map10[x][y] = 0;
    for (y = 2; y < YSize - 2; y++) {
        map10[XSize / 4][y] = 1;
        map10[3 * XSize / 4][y] = 1;
    }
    for (x = XSize / 4; x <= 3 * XSize / 4 && x < XSize; x++)
        map10[x][YSize / 2] = 1;

    /* --- map11: three vertical bars at quarters --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map11[x][y] = 0;
    for (uint8_t q = 1; q <= 3; q++) {
        uint8_t cx = q * XSize / 4;
        for (y = 2; y < YSize - 2; y++)
            map11[cx][y] = 1;
    }

    /* --- map12: checkerboard of 3x3 blocks --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map12[x][y] = 0;
    for (uint8_t bx2 = 5; bx2 < XSize - 4; bx2 += 9) {
        for (uint8_t by2 = 3; by2 < YSize - 3; by2 += 7) {
            uint8_t on = ((bx2 / 9 + by2 / 7) % 2);
            if (on) {
                map12[bx2][by2] = 1;
                map12[bx2 + 1][by2] = 1;
                map12[bx2 + 2][by2] = 1;
                if (by2 + 1 < YSize) {
                    map12[bx2][by2 + 1] = 1;
                    map12[bx2 + 1][by2 + 1] = 1;
                    map12[bx2 + 2][by2 + 1] = 1;
                }
            }
        }
    }

    /* --- map13: S-curve corridor --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map13[x][y] = 0;
    for (x = 4; x < XSize - 4; x++) {
        if (x > XSize / 2 && x < XSize * 3 / 4) continue;
        map13[x][YSize / 3] = 1;
    }
    for (x = XSize * 3 / 8; x < XSize - 4; x++) {
        if (x > XSize / 2 && x < XSize * 5 / 6) continue;
        map13[x][YSize / 2] = 1;
    }
    for (x = 4; x < XSize - 4; x++) {
        if (x > XSize * 3 / 8 && x < XSize * 5 / 6) continue;
        map13[x][2 * YSize / 3] = 1;
    }

    /* --- map14: two concentric rectangles with gaps --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map14[x][y] = 0;
    /* outer rect */
    for (x = 3; x < XSize - 3; x++) {
        if (x > XSize / 2 - 5 && x < XSize / 2 + 5) continue;
        map14[x][2] = 1;
        map14[x][YSize - 3] = 1;
    }
    for (y = 2; y < YSize - 2; y++) {
        if (y > YSize / 2 - 4 && y < YSize / 2 + 4) continue;
        map14[3][y] = 1;
        map14[XSize - 4][y] = 1;
    }
    /* inner rect */
    for (x = XSize / 3; x < 2 * XSize / 3; x++) {
        if (x > XSize / 2 - 3 && x < XSize / 2 + 3) continue;
        map14[x][YSize / 4] = 1;
        map14[x][3 * YSize / 4] = 1;
    }
    for (y = YSize / 4; y <= 3 * YSize / 4; y++) {
        if (y > YSize / 2 - 2 && y < YSize / 2 + 2) continue;
        map14[XSize / 3][y] = 1;
        map14[2 * XSize / 3][y] = 1;
    }

    /* --- map15: vertical zigzag column in center --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map15[x][y] = 0;
    for (y = 2; y < YSize - 2; y += 4) {
        uint8_t off = ((y / 4) % 2 == 0) ? XSize / 3 : 2 * XSize / 3;
        map15[off][y] = 1;
        if (off + 1 < XSize) map15[off + 1][y] = 1;
    }

    /* --- map16: four diagonal segments forming a diamond --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map16[x][y] = 0;
    uint8_t hw = XSize / 4, hh = YSize / 4;
    /* top-left to center */
    for (uint8_t t = 0; t <= hh; t++)
        map16[hw + t * hw / hh][hh - t] = 1;
    /* top-right to center */
    for (uint8_t t = 0; t <= hh; t++)
        map16[XSize - 1 - hw - t * hw / hh][hh - t] = 1;
    /* bottom-left to center */
    for (uint8_t t = 0; t <= YSize - 2 - hh; t++)
        map16[hw + t * hw / (YSize - 2 - hh)][hh + 1 + t] = 1;
    /* bottom-right to center */
    for (uint8_t t = 0; t <= YSize - 2 - hh; t++)
        map16[XSize - 1 - hw - t * hw / (YSize - 2 - hh)][hh + 1 + t] = 1;

    /* --- map17: four horizontal bars at fifths --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map17[x][y] = 0;
    for (uint8_t f = 1; f <= 4; f++) {
        uint8_t row = f * YSize / 5;
        for (x = 2; x < XSize - 2; x++)
            map17[x][row] = 1;
    }

    /* --- map18: four L-shaped corners pointing inward --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map18[x][y] = 0;
    uint8_t arm = XSize / 5;
    /* top-left L */
    for (x = 3; x < 3 + arm && x < XSize - 2; x++) map18[x][3] = 1;
    for (y = 3; y < 3 + arm && y < YSize - 2; y++)   map18[3][y] = 1;
    /* top-right L */
    for (x = XSize - 4 - arm; x < XSize - 3; x++) map18[x][3] = 1;
    for (y = 3; y < 3 + arm && y < YSize - 2; y++)   map18[XSize - 4][y] = 1;
    /* bottom-left L */
    for (x = 3; x < 3 + arm && x < XSize - 2; x++) map18[x][YSize - 4] = 1;
    for (y = YSize - 4 - arm; y < YSize - 3; y++)   map18[3][y] = 1;
    /* bottom-right L */
    for (x = XSize - 4 - arm; x < XSize - 3; x++) map18[x][YSize - 4] = 1;
    for (y = YSize - 4 - arm; y < YSize - 3; y++)   map18[XSize - 4][y] = 1;

    /* --- map19: vertical spine with horizontal branches --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map19[x][y] = 0;
    for (y = 2; y < YSize - 2; y++)
        map19[XSize / 2][y] = 1;
    for (uint8_t q = 1; q <= 3; q++) {
        uint8_t row = q * YSize / 4;
        for (x = XSize / 4; x < XSize - XSize / 4 && x < XSize; x++)
            map19[x][row] = 1;
    }

    /* --- map20: two offset vertical bars staggered --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map20[x][y] = 0;
    for (y = 2; y < YSize / 2; y++) {
        map20[XSize / 3][y] = 1;
        map20[2 * XSize / 3][y] = 1;
    }
    for (y = YSize / 2; y < YSize - 2; y++) {
        map20[XSize / 4][y] = 1;
        map20[3 * XSize / 4][y] = 1;
    }

    /* --- map21: thick cross (wider arms) --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map21[x][y] = 0;
    for (y = 3; y < YSize - 3; y++) {
        map21[XSize / 2 - 2][y] = 1;
        map21[XSize / 2 - 1][y] = 1;
        map21[XSize / 2][y]     = 1;
        map21[XSize / 2 + 1][y] = 1;
        map21[XSize / 2 + 2][y] = 1;
    }
    for (x = 3; x < XSize - 3; x++) {
        map21[x][YSize / 2 - 1] = 1;
        map21[x][YSize / 2]     = 1;
        map21[x][YSize / 2 + 1] = 1;
    }

    /* --- map22: scattered single dots in rows --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map22[x][y] = 0;
    for (uint8_t row = 4; row < YSize - 3; row += 5) {
        uint8_t offset = (row % 10 == 4) ? 6 : 2 * XSize / 3;
        map22[offset][row] = 1;
        if (offset + 7 < XSize) map22[offset + 7][row] = 1;
        if (offset + 14 < XSize) map22[offset + 14][row] = 1;
    }

    /* --- map23: three horizontal bars at uneven positions --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map23[x][y] = 0;
    for (x = 4; x < XSize - 4; x++) {
        map23[x][YSize / 5]       = 1;
        map23[x][YSize / 2]       = 1;
        map23[x][4 * YSize / 5]   = 1;
    }

    /* --- map24: three nested rectangles with gaps --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map24[x][y] = 0;
    /* outer */
    for (x = 2; x < XSize - 2; x++) {
        if (x > XSize / 2 - 5 && x < XSize / 2 + 5) continue;
        map24[x][1] = 1;
        map24[x][YSize - 2] = 1;
    }
    for (y = 1; y < YSize - 1; y++) {
        if (y > YSize / 2 - 3 && y < YSize / 2 + 3) continue;
        map24[2][y] = 1;
        map24[XSize - 3][y] = 1;
    }
    /* middle */
    for (x = XSize / 4; x < 3 * XSize / 4; x++) {
        if (x > XSize / 2 - 4 && x < XSize / 2 + 4) continue;
        map24[x][YSize / 4] = 1;
        map24[x][3 * YSize / 4] = 1;
    }
    for (y = YSize / 4; y <= 3 * YSize / 4; y++) {
        if (y > YSize / 2 - 2 && y < YSize / 2 + 2) continue;
        map24[XSize / 4][y] = 1;
        map24[3 * XSize / 4][y] = 1;
    }
    /* inner */
    for (x = XSize / 2 - 5; x <= XSize / 2 + 5 && x < XSize; x++)
        if (!(x > XSize / 2 - 2 && x < XSize / 2 + 2))
            map24[x][YSize / 2] = 1;

    /* --- map25: parallel diagonal stripes --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map25[x][y] = 0;
    for (uint8_t d = 0; d < YSize + XSize / 3; d += 6) {
        uint16_t sx = d, sy = 0;
        while (sy < YSize - 2 && sx < XSize - 2) {
            map25[sx][sy] = 1;
            if (sx + 1 < XSize) map25[sx + 1][sy] = 1;
            sy++;
            sx++;
        }
    }

    /* --- map26: vertical comb pattern --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map26[x][y] = 0;
    /* spine on left third */
    for (y = 2; y < YSize - 2; y++)
        map26[XSize / 3][y] = 1;
    /* teeth going right every 4 rows */
    for (uint8_t row = 4; row < YSize - 3; row += 4) {
        uint8_t len = ((row / 4) % 2 == 0) ? XSize / 5 : XSize / 8;
        for (x = XSize / 3 + 1; x < XSize / 3 + 1 + len && x < XSize - 2; x++)
            map26[x][row] = 1;
    }

    /* --- map27: parallel diagonals going same direction --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map27[x][y] = 0;
    for (uint8_t off = 4; off < XSize - 4; off += 10) {
        uint16_t cx = off, cy = 2;
        while (cy < YSize - 2 && cx < XSize - 2) {
            map27[cx][cy] = 1;
            if (cx + 1 < XSize) map27[cx + 1][cy] = 1;
            cy++;
            cx++;
        }
    }

    /* --- map28: frame border + inner cross --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map28[x][y] = 0;
    /* border */
    for (x = 2; x < XSize - 2; x++) {
        if (x > XSize / 2 - 6 && x < XSize / 2 + 6) continue;
        map28[x][2] = 1;
        map28[x][YSize - 3] = 1;
    }
    for (y = 2; y < YSize - 2; y++) {
        if (y > YSize / 2 - 4 && y < YSize / 2 + 4) continue;
        map28[2][y] = 1;
        map28[XSize - 3][y] = 1;
    }
    /* inner cross */
    for (y = 5; y < YSize - 5; y++) {
        if (y > YSize / 2 - 3 && y < YSize / 2 + 3) continue;
        map28[XSize / 2][y] = 1;
    }
    for (x = XSize / 4; x < 3 * XSize / 4 && x < XSize; x++) {
        if (x > XSize / 2 - 3 && x < XSize / 2 + 3) continue;
        map28[x][YSize / 2] = 1;
    }

    /* --- map29: three vertical bars with gaps at different heights --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map29[x][y] = 0;
    /* bar at 1/4: gap in upper half */
    for (y = YSize / 2 + 2; y < YSize - 2; y++)
        map29[XSize / 4][y] = 1;
    /* bar at center: full */
    for (y = 2; y < YSize - 2; y++)
        map29[XSize / 2][y] = 1;
    /* bar at 3/4: gap in lower half */
    for (y = 2; y < YSize / 2 - 2; y++)
        map29[3 * XSize / 4][y] = 1;

    /* --- map30: descending staircase from left to right --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map30[x][y] = 0;
    uint8_t steps = 6;
    uint8_t step_w = XSize / steps;
    uint8_t step_h = (YSize - 4) / steps;
    for (uint8_t s = 0; s < steps && s * step_w + step_w < XSize; s++) {
        uint16_t sx = s * step_w;
        uint16_t sy = 2 + s * step_h;
        /* horizontal tread */
        for (x = sx; x < sx + step_w && x < XSize - 2; x++)
            map30[x][sy] = 1;
        /* vertical riser */
        if (s > 0) {
            uint8_t ry = 2 + s * step_h;
            for (y = 2 + (s - 1) * step_h + 1; y <= ry && y < YSize - 2; y++)
                map30[sx][y] = 1;
        }
    }

    /* --- map31: scattered small blocks in a loose grid --- */
    for (x = 0; x < XSize; x++)
        for (y = 0; y < YSize; y++)
            map31[x][y] = 0;
    for (uint8_t bx2 = 5; bx2 < XSize - 4; bx2 += 7) {
        for (uint8_t by2 = 4; by2 < YSize - 3; by2 += 5) {
            /* pseudo-random via bit pattern */
            if (((bx2 * 7 + by2 * 13) % 5) != 0) continue;
            map31[bx2][by2] = 1;
            if (bx2 + 1 < XSize - 4) {
                map31[bx2 + 1][by2] = 1;
                map31[bx2][by2 + 1] = 1;
            }
        }
    }

    active_map = map0;
}

void setup_level(void) {
    uint8_t i, attempts;

    build_maps();

    switch (level_idx % NUM_LEVELS) {
        case 0:  active_map = map0;  break;
        case 1:  active_map = map1;  break;
        case 2:  active_map = map2;  break;
        case 3:  active_map = map3;  break;
        case 4:  active_map = map4;  break;
        case 5:  active_map = map5;  break;
        case 6:  active_map = map6;  break;
        case 7:  active_map = map7;  break;
        case 8:  active_map = map8;  break;
        case 9:  active_map = map9;  break;
        case 10: active_map = map10; break;
        case 11: active_map = map11; break;
        case 12: active_map = map12; break;
        case 13: active_map = map13; break;
        case 14: active_map = map14; break;
        case 15: active_map = map15; break;
        case 16: active_map = map16; break;
        case 17: active_map = map17; break;
        case 18: active_map = map18; break;
        case 19: active_map = map19; break;
        case 20: active_map = map20; break;
        case 21: active_map = map21; break;
        case 22: active_map = map22; break;
        case 23: active_map = map23; break;
        case 24: active_map = map24; break;
        case 25: active_map = map25; break;
        case 26: active_map = map26; break;
        case 27: active_map = map27; break;
        case 28: active_map = map28; break;
        case 29: active_map = map29; break;
        default: active_map = map30; break; /* or map31 */
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
