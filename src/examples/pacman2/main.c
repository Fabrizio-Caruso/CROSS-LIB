#include "cross_lib.h"

#define P_T0 _TILE_0
#define P_T1 _TILE_1
#define P_T2 _TILE_2
#define P_T3 _TILE_3

#define G_T0 _TILE_4
#define G_T1 _TILE_5
#define G_T2 _TILE_6
#define G_T3 _TILE_7

#define PEL_T0 _TILE_8
#define BORDER_T _TILE_26

#define MAX_P 64
#define ENEMIES 4
#define PLAY_TOP 2

static void draw_block(uint8_t x, uint8_t y, uint8_t t0, uint8_t t1,
                       uint8_t t2, uint8_t t3, uint8_t col)
{
    _XL_DRAW(x-1, y-1, t0, col);
    _XL_DRAW(x,   y-1, t1, col);
    _XL_DRAW(x-1, y,   t2, col);
    _XL_DRAW(x,   y,   t3, col);
}

static int block_hits_pellet(short bx, short by, uint8_t px[], uint8_t py[],
                             uint8_t alive[], short n)
{
    short i, dx, dy;
    for (i = 0; i < n; i++) {
        if (!alive[i]) continue;
        dx = px[i] - bx;  if (dx < -1 || dx > 0) continue;
        dy = py[i] - by;  if (dy < -1 || dy > 0) continue;
        return 1;
    }
    return 0;
}

int main(void)
{
    short px, py, old_px, old_py;
    short ex[ENEMIES], ey[ENEMIES];
    short old_ex[ENEMIES], old_ey[ENEMIES];
    uint8_t ecol[ENEMIES];
    uint8_t pellet_x[MAX_P];
    uint8_t pellet_y[MAX_P];
    uint8_t pellet_alive[MAX_P];
    uint8_t score;
    uint8_t lives;
    uint16_t pcount;
    short i, j, k, t;
    uint8_t input;
    short game_over;
    short level_done;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    ecol[0] = _XL_RED; ecol[1] = _XL_MAGENTA;
    ecol[2] = _XL_CYAN; ecol[3] = _XL_BLUE;

    while (1)
    {
        score = 0; lives = 3; pcount = 28;
        px = 4; py = PLAY_TOP + 3;
        ex[0] = XSize - 6; ey[0] = PLAY_TOP + 4;
        ex[1] = XSize - 6; ey[1] = YSize - 6;
        ex[2] = 4;         ey[2] = YSize - 6;
        ex[3] = 4;         ey[3] = PLAY_TOP + 4;

        for (i = 0; i < MAX_P; i++) pellet_alive[i] = 0;
        for (i = 0; i < pcount; i++) {
            short ok, tries;
            tries = 0;
            do {
                ok = 1;
                pellet_x[i] = (uint8_t)((_XL_RAND() % (XSize - 4)) + 2);
                pellet_y[i] = (uint8_t)((_XL_RAND() % (YSize - PLAY_TOP - 3))
                                        + PLAY_TOP + 1);
                if (block_hits_pellet(px, py, pellet_x, pellet_y,
                                      pellet_alive, i)) ok = 0;
                for (j = 0; j < ENEMIES; j++) {
                    if (block_hits_pellet(ex[j], ey[j],
                                          pellet_x, pellet_y, pellet_alive, i))
                        ok = 0;
                }
                for (j = 0; j < i; j++) {
                    if (pellet_x[i] == pellet_x[j] && pellet_y[i] == pellet_y[j])
                        ok = 0;
                }
                tries++;
            } while (!ok && tries < 2000);
            pellet_alive[i] = 1;
        }

        _XL_CLEAR_SCREEN();
        for (i = 0; i < XSize; i++) {
            _XL_DRAW((uint8_t)i, PLAY_TOP-1, BORDER_T, _XL_WHITE);
            _XL_DRAW((uint8_t)i, YSize-2, BORDER_T, _XL_WHITE);
        }
        for (j = PLAY_TOP; j < YSize-2; j++) {
            _XL_DRAW(0, (uint8_t)j, BORDER_T, _XL_WHITE);
            _XL_DRAW(XSize-1, (uint8_t)j, BORDER_T, _XL_WHITE);
        }
        for (i = 0; i < pcount; i++)
            if (pellet_alive[i]) _XL_DRAW(pellet_x[i], pellet_y[i], PEL_T0, _XL_WHITE);
        draw_block((uint8_t)px, (uint8_t)py, P_T0,P_T1,P_T2,P_T3,_XL_YELLOW);
        for (i = 0; i < ENEMIES; i++)
            draw_block((uint8_t)ex[i], (uint8_t)ey[i], G_T0,G_T1,G_T2,G_T3,ecol[i]);

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0,0,3,score);
        _XL_PRINTD(XSize-6,0,1,lives);

        game_over = 0; level_done = 0;
        while (!game_over && lives > 0 && !level_done)
        {
            old_px = px; old_py = py;
            for (i = 0; i < ENEMIES; i++) { old_ex[i]=ex[i]; old_ey[i]=ey[i]; }

            input = _XL_INPUT();
            if (_XL_LEFT(input))  px -= 1;
            if (_XL_RIGHT(input)) px += 1;
            if (_XL_UP(input))    py -= 1;
            if (_XL_DOWN(input))  py += 1;
            if (px < 2) px = 2; if (py < PLAY_TOP+1) py = PLAY_TOP+1;
            if (px > XSize-3) px = XSize-3; if (py > YSize-3) py = YSize-3;

            for (i = 0; i < ENEMIES; i++) {
                short nx, ny; short d = _XL_RAND() & 3;
                nx = ex[i]; ny = ey[i];
                if (d == 0) nx -= 1; if (d == 1) nx += 1;
                if (d == 2) ny -= 1; if (d == 3) ny += 1;
                if (nx < 2) nx = 2; if (ny < PLAY_TOP+1) ny = PLAY_TOP+1;
                if (nx > XSize-3) nx = XSize-3; if (ny > YSize-3) ny = YSize-3;
                if (!block_hits_pellet(nx, ny, pellet_x, pellet_y,
                                       pellet_alive, pcount)) {
                    ex[i] = nx; ey[i] = ny;
                }
            }

            if (old_px != px || old_py != py) {
                _XL_DELETE(old_px-1, old_py-1); _XL_DELETE(old_px, old_py-1);
                _XL_DELETE(old_px-1, old_py);   _XL_DELETE(old_px, old_py);
                draw_block((uint8_t)px, (uint8_t)py, P_T0,P_T1,P_T2,P_T3,_XL_YELLOW);
            }
            for (i = 0; i < ENEMIES; i++) {
                if (old_ex[i] != ex[i] || old_ey[i] != ey[i]) {
                    short ox = old_ex[i], oy = old_ey[i];
                    if (!block_hits_pellet(ox, oy, pellet_x, pellet_y,
                                           pellet_alive, pcount)) {
                        _XL_DELETE(ox-1, oy-1); _XL_DELETE(ox, oy-1);
                        _XL_DELETE(ox-1, oy);   _XL_DELETE(ox, oy);
                    }
                    draw_block((uint8_t)ex[i], (uint8_t)ey[i],
                               G_T0,G_T1,G_T2,G_T3,ecol[i]);
                }
            }

            // for (i = 0; i < pcount; i++) {
                // if (pellet_alive[i] && pellet_x[i] == (uint8_t)px &&
                    // pellet_y[i] == (uint8_t)py) {
                    // pellet_alive[i] = 0;
                    // _XL_DELETE(pellet_x[i], pellet_y[i]);
                    // score += 10; _XL_TICK_SOUND();
                    // _XL_PRINTD(0,0,3,score);
                // }
            // }

            /* --- replace the old pellet eating loop with this --- */
            for (i = 0; i < pcount; i++) {
                if (!pellet_alive[i]) continue;
                if (pellet_x[i] >= (uint8_t)(px-1) && pellet_x[i] <= (uint8_t)px &&
                    pellet_y[i] >= (uint8_t)(py-1) && pellet_y[i] <= (uint8_t)py) {
                    pellet_alive[i] = 0;
                    _XL_DELETE(pellet_x[i], pellet_y[i]);
                    score += 10;
                    _XL_TICK_SOUND();
                    _XL_PRINTD(0,0,3,score);
                }
            }

            for (i = 0; i < ENEMIES; i++) {
                if (px == ex[i] && py == ey[i]) {
                    lives--; _XL_EXPLOSION_SOUND();
                    _XL_PRINTD(XSize-6,0,1,lives);
                    if (lives == 0) { game_over = 1; break; }
                    /* restart level, keep only active pellets */
                    px = 4; py = PLAY_TOP + 3;
                    ex[0] = XSize - 6; ey[0] = PLAY_TOP + 4;
                    ex[1] = XSize - 6; ey[1] = YSize - 6;
                    ex[2] = 4;         ey[2] = YSize - 6;
                    ex[3] = 4;         ey[3] = PLAY_TOP + 4;
                    score = 0;
                    _XL_CLEAR_SCREEN();
                    for (j = 0; j < XSize; j++) {
                        _XL_DRAW((uint8_t)j, PLAY_TOP-1, BORDER_T, _XL_WHITE);
                        _XL_DRAW((uint8_t)j, YSize-2, BORDER_T, _XL_WHITE);
                    }
                    for (j = PLAY_TOP; j < YSize-2; j++) {
                        _XL_DRAW(0, (uint8_t)j, BORDER_T, _XL_WHITE);
                        _XL_DRAW(XSize-1, (uint8_t)j, BORDER_T, _XL_WHITE);
                    }
                    for (j = 0; j < pcount; j++)
                        if (pellet_alive[j])
                            _XL_DRAW(pellet_x[j], pellet_y[j], PEL_T0, _XL_WHITE);
                    draw_block((uint8_t)px, (uint8_t)py,
                               P_T0,P_T1,P_T2,P_T3,_XL_YELLOW);
                    for (j = 0; j < ENEMIES; j++)
                        draw_block((uint8_t)ex[j], (uint8_t)ey[j],
                                   G_T0,G_T1,G_T2,G_T3,ecol[j]);
                    _XL_PRINTD(0,0,3,score);
                    _XL_PRINTD(XSize-6,0,1,lives);
                    break;
                }
            }

            level_done = 1;
            for (i = 0; i < pcount; i++) {
                if (pellet_alive[i]) { level_done = 0; break; }
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        if (lives == 0) {
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(2, YSize/2, "GAME OVER");
        } else {
            _XL_SET_TEXT_COLOR(_XL_GREEN);
            _XL_PRINT(2, YSize/2, "LEVEL CLEAR");
        }
        _XL_WAIT_FOR_INPUT();
    }
    return 0;
}
