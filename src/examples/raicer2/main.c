#include "cross_lib.h"

#if XSize>=38
    #define ROAD_W 15
#elif XSize>=32
    #define ROAD_W 13
#else
    #define ROAD_W 11
#endif
#define ROAD_L ((XSize - ROAD_W) / 2)
#define ROAD_R (ROAD_L + ROAD_W - 1)
#define CENTER_X (ROAD_L + ROAD_W / 2)
#define PLAYER_Y (YSize - 4)
#define MAX_ENEMIES 8
#define MAX_BLOCKS 16
#define OBSTACLE_TILE _TILE_6
#define BORDER_TILE _TILE_5
#define PLAYER_TOP _TILE_0
#define PLAYER_BOT _TILE_1
#define ENEMY_TOP _TILE_2
#define ENEMY_BOT _TILE_3
#define BIG_ENEMY_TOP _TILE_4
#define BIG_ENEMY_BOT _TILE_7

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
} Car;

typedef struct {
    uint8_t x;
    uint8_t height;
    uint8_t entered;
    uint8_t top;
} Block;

static void draw_car(Car *c, uint8_t top_tile, uint8_t bot_tile)
{
    uint8_t i, j;
    for (i = 0; i < c->h; i++) {
        if ((uint16_t)c->y + i >= YSize) break;
        for (j = 0; j < c->w; j++) {
            if (i == 0) {
                _XL_DRAW(c->x + j, c->y, top_tile, _XL_RED);
            } else {
                _XL_DRAW(c->x + j, c->y + i, bot_tile, _XL_CYAN);
            }
        }
    }
}

static void erase_car(Car *c)
{
    uint8_t i, j;
    for (i = 0; i < c->h; i++) {
        if ((uint16_t)c->y + i >= YSize) break;
        for (j = 0; j < c->w; j++) {
            _XL_DELETE(c->x + j, c->y + i);
        }
    }
}

static void draw_borders(void)
{
    uint8_t y;
    for (y = 0; y < YSize; y++) {
        _XL_DRAW((uint8_t)(ROAD_L - 1), y, BORDER_TILE, _XL_WHITE);
        _XL_DRAW((uint8_t)(ROAD_R + 1), y, BORDER_TILE, _XL_WHITE);
    }
}

static void erase_borders(void)
{
    uint8_t y;
    for (y = 0; y < YSize; y++) {
        _XL_DELETE((uint8_t)(ROAD_L - 1), y);
        _XL_DELETE((uint8_t)(ROAD_R + 1), y);
    }
}

static uint8_t is_blocked(uint8_t x, uint8_t y, Block *blocks, uint8_t block_count)
{
    uint8_t i;
    if (x < ROAD_L || x > ROAD_R) return 1;
    for (i = 0; i < block_count; i++) {
        if (blocks[i].x != x) continue;
        if (blocks[i].entered < blocks[i].height) {
            if (y < blocks[i].entered) return 1;
        } else {
            if (y >= blocks[i].top && y < (uint8_t)(blocks[i].top + blocks[i].height)) return 1;
        }
    }
    return 0;
}

static uint8_t has_active_at(Block *blocks, uint8_t block_count, uint8_t x)
{
    uint8_t i;
    for (i = 0; i < block_count; i++) {
        if (blocks[i].x == x) return 1;
    }
    return 0;
}

static void spawn_lateral_pair(Block *blocks, uint8_t *block_count)
{
    uint8_t h;
    if (*block_count + 2 > MAX_BLOCKS) return;
    if (has_active_at(blocks, *block_count, ROAD_L)) return;
    h = (uint8_t)(10 + _XL_RAND() % 16);
    blocks[*block_count].x = ROAD_L;
    blocks[*block_count].height = h;
    blocks[*block_count].entered = 0;
    blocks[*block_count].top = 0;
    (*block_count)++;
    blocks[*block_count].x = ROAD_R;
    blocks[*block_count].height = h;
    blocks[*block_count].entered = 0;
    blocks[*block_count].top = 0;
    (*block_count)++;
}

static void spawn_central(Block *blocks, uint8_t *block_count)
{
    uint8_t h;
    if (*block_count + 1 > MAX_BLOCKS) return;
    if (has_active_at(blocks, *block_count, CENTER_X)) return;
    h = (uint8_t)(10 + _XL_RAND() % 16);
    blocks[*block_count].x = CENTER_X;
    blocks[*block_count].height = h;
    blocks[*block_count].entered = 0;
    blocks[*block_count].top = 0;
    (*block_count)++;
}

static uint8_t cars_overlap(uint8_t ax, uint8_t ay, uint8_t aw, uint8_t ah,
                            uint8_t bx, uint8_t by, uint8_t bw, uint8_t bh)
{
    if (ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by) return 1;
    return 0;
}

static void game_loop(void)
{
    Block blocks[MAX_BLOCKS];
    Car enemies[MAX_ENEMIES];
    uint8_t block_count;
    uint8_t enemy_count;
    uint8_t player_x;
    uint8_t i, k;
    uint16_t frame;
    uint16_t score;
    uint8_t game_over;
    uint8_t input;
    uint16_t slowdown;

    _XL_CLEAR_SCREEN();
    draw_borders();
    block_count = 0;
    enemy_count = 0;
    player_x = CENTER_X;
    frame = 0;
    score = 0;
    game_over = 0;

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(ROAD_L, YSize / 2, "RACING");
    _XL_DRAW(player_x, PLAYER_Y, PLAYER_TOP, _XL_GREEN);
    _XL_DRAW(player_x, (uint8_t)(PLAYER_Y + 1), PLAYER_BOT, _XL_GREEN);
    _XL_SLEEP(1);
    for (i = 0; i < ROAD_W; i++) {
        _XL_DELETE((uint8_t)(ROAD_L + i), YSize / 2);
    }
    slowdown = _XL_SLOW_DOWN_FACTOR;
    
    while (!game_over) {
        frame++;
        if(!(frame&7))
        {
            score++;
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINTD(ROAD_L, YSize - 1, 5, score);
            if(slowdown>_XL_SLOW_DOWN_FACTOR/4)
            {
                slowdown-= _XL_SLOW_DOWN_FACTOR/100;
            }
            #if !defined(_XL_SLOW_DOWN_FACTOR)
            else if(slowdown>_XL_SLOW_DOWN/8)
            {
                --slowdown;
            }
            #endif
        }
        
        if (frame % 4 == 0) {
            for (i = block_count; i > 0; i--) {
                uint8_t idx;
                idx = i - 1;
                if (blocks[idx].entered < blocks[idx].height) {
                    _XL_DRAW(blocks[idx].x, blocks[idx].entered, OBSTACLE_TILE, _XL_YELLOW);
                    blocks[idx].entered++;
                } else {
                    uint8_t old_top;
                    uint8_t new_bottom;
                    old_top = blocks[idx].top;
                    new_bottom = (uint8_t)(old_top + blocks[idx].height);
                    _XL_DELETE(blocks[idx].x, old_top);
                    if (new_bottom < YSize) {
                        _XL_DRAW(blocks[idx].x, new_bottom, OBSTACLE_TILE, _XL_YELLOW);
                    }
                    blocks[idx].top++;
                }
                if (blocks[idx].entered >= blocks[idx].height && blocks[idx].top >= YSize) {
                    blocks[idx] = blocks[block_count - 1];
                    block_count--;
                }
            }
        }

        if (frame % 60 == 0) {
            uint16_t r;
            r = _XL_RAND();
            if (r % 2 == 0) {
                spawn_lateral_pair(blocks, &block_count);
            } else {
                spawn_central(blocks, &block_count);
            }
        }

        if (frame % 8 == 0 && enemy_count < MAX_ENEMIES) {
            uint16_t r;
            uint8_t w, h, x, max_x, ok;
            r = _XL_RAND();
            if (r % 5 == 0) {
                w = 2;
                h = 3;
            } else {
                w = 1;
                h = 2;
            }
            max_x = (uint8_t)(ROAD_R - w + 1);
            x = (uint8_t)(ROAD_L + _XL_RAND() % (max_x - ROAD_L + 1));
            ok = 1;
            {
                uint8_t m, n;
                for (n = 0; n < h && ok; n++) {
                    for (m = 0; m < w && ok; m++) {
                        if (is_blocked((uint8_t)(x + m), n, blocks, block_count)) ok = 0;
                    }
                }
            }
            if (ok) {
                enemies[enemy_count].x = x;
                enemies[enemy_count].y = 0;
                enemies[enemy_count].w = w;
                enemies[enemy_count].h = h;
                enemy_count++;
            }
        }

        for (i = enemy_count; i > 0; i--) {
            uint8_t idx;
            uint8_t old_x, old_y;
            uint8_t new_x;
            uint8_t can_move_down;
            uint8_t can_move_diag;
            uint8_t do_diag;
            uint8_t dir;
            uint8_t m2, k2;
            idx = i - 1;
            if ((uint16_t)enemies[idx].y + enemies[idx].h >= YSize) {
                erase_car(&enemies[idx]);
                enemies[idx] = enemies[enemy_count - 1];
                enemy_count--;
                continue;
            }

            old_x = enemies[idx].x;
            old_y = enemies[idx].y;

            if (frame % 12 == 0 && _XL_RAND() % 3 == 0 && old_y < YSize - 6) {
                do_diag = 1;
                dir = (uint8_t)(_XL_RAND() % 2);
            } else {
                do_diag = 0;
                dir = 0;
            }

            can_move_down = 1;
            for (k2 = 0; k2 < enemies[idx].h && can_move_down; k2++) {
                uint8_t ny;
                ny = (uint8_t)(old_y + 1 + k2);
                if (ny >= YSize) { can_move_down = 0; break; }
                for (m2 = 0; m2 < enemies[idx].w && can_move_down; m2++) {
                    if (is_blocked((uint8_t)(old_x + m2), ny, blocks, block_count)) {
                        can_move_down = 0;
                    }
                }
            }

            new_x = old_x;
            if (do_diag) {
                if (dir == 0 && old_x > ROAD_L) {
                    new_x = (uint8_t)(old_x - 1);
                } else if (dir == 1 && (uint16_t)old_x + enemies[idx].w <= ROAD_R) {
                    new_x = (uint8_t)(old_x + 1);
                }
            }

            can_move_diag = 0;
            if (do_diag && new_x != old_x) {
                can_move_diag = 1;
                for (k2 = 0; k2 < enemies[idx].h && can_move_diag; k2++) {
                    uint8_t ny2;
                    ny2 = (uint8_t)(old_y + 1 + k2);
                    if (ny2 >= YSize) { can_move_diag = 0; break; }
                    for (m2 = 0; m2 < enemies[idx].w && can_move_diag; m2++) {
                        if (is_blocked((uint8_t)(new_x + m2), ny2, blocks, block_count)) {
                            can_move_diag = 0;
                        }
                    }
                }
            }

            erase_car(&enemies[idx]);

            if (can_move_down && do_diag && can_move_diag) {
                enemies[idx].x = new_x;
                enemies[idx].y = (uint8_t)(old_y + 1);
            } else if (can_move_down) {
                enemies[idx].x = old_x;
                enemies[idx].y = (uint8_t)(old_y + 1);
            } else {
                uint8_t escaped;
                uint8_t esc_dir;
                uint8_t esc_x;
                uint8_t m3, k3, e2;
                escaped = 0;
                esc_dir = (uint8_t)(_XL_RAND() % 2);

                if (esc_dir == 0 && old_x > ROAD_L) {
                    esc_x = (uint8_t)(old_x - 1);
                    can_move_diag = 1;
                    for (k3 = 0; k3 < enemies[idx].h && can_move_diag; k3++) {
                        uint8_t ny3;
                        ny3 = (uint8_t)(old_y + k3);
                        if (ny3 >= YSize) { can_move_diag = 0; break; }
                        for (m3 = 0; m3 < enemies[idx].w && can_move_diag; m3++) {
                            if (is_blocked((uint8_t)(esc_x + m3), ny3, blocks, block_count)) {
                                can_move_diag = 0;
                            }
                        }
                    }
                    if (can_move_diag) {
                        for (e2 = 0; e2 < enemy_count && can_move_diag; e2++) {
                            if (e2 == idx) continue;
                            if (cars_overlap(esc_x, old_y, enemies[idx].w, enemies[idx].h,
                                            enemies[e2].x, enemies[e2].y,
                                            enemies[e2].w, enemies[e2].h)) {
                                can_move_diag = 0;
                            }
                        }
                    }
                    if (can_move_diag) escaped = 1;
                }

                if (!escaped && (uint16_t)old_x + enemies[idx].w <= ROAD_R) {
                    esc_x = (uint8_t)(old_x + 1);
                    can_move_diag = 1;
                    for (k3 = 0; k3 < enemies[idx].h && can_move_diag; k3++) {
                        uint8_t ny3;
                        ny3 = (uint8_t)(old_y + k3);
                        if (ny3 >= YSize) { can_move_diag = 0; break; }
                        for (m3 = 0; m3 < enemies[idx].w && can_move_diag; m3++) {
                            if (is_blocked((uint8_t)(esc_x + m3), ny3, blocks, block_count)) {
                                can_move_diag = 0;
                            }
                        }
                    }
                    if (can_move_diag) {
                        for (e2 = 0; e2 < enemy_count && can_move_diag; e2++) {
                            if (e2 == idx) continue;
                            if (cars_overlap(esc_x, old_y, enemies[idx].w, enemies[idx].h,
                                            enemies[e2].x, enemies[e2].y,
                                            enemies[e2].w, enemies[e2].h)) {
                                can_move_diag = 0;
                            }
                        }
                    }
                    if (can_move_diag) escaped = 1;
                }

                if (!escaped && esc_dir == 1 && old_x > ROAD_L) {
                    esc_x = (uint8_t)(old_x - 1);
                    can_move_diag = 1;
                    for (k3 = 0; k3 < enemies[idx].h && can_move_diag; k3++) {
                        uint8_t ny3;
                        ny3 = (uint8_t)(old_y + k3);
                        if (ny3 >= YSize) { can_move_diag = 0; break; }
                        for (m3 = 0; m3 < enemies[idx].w && can_move_diag; m3++) {
                            if (is_blocked((uint8_t)(esc_x + m3), ny3, blocks, block_count)) {
                                can_move_diag = 0;
                            }
                        }
                    }
                    if (can_move_diag) {
                        for (e2 = 0; e2 < enemy_count && can_move_diag; e2++) {
                            if (e2 == idx) continue;
                            if (cars_overlap(esc_x, old_y, enemies[idx].w, enemies[idx].h,
                                            enemies[e2].x, enemies[e2].y,
                                            enemies[e2].w, enemies[e2].h)) {
                                can_move_diag = 0;
                            }
                        }
                    }
                    if (can_move_diag) escaped = 1;
                }

                if (escaped) {
                    enemies[idx].x = esc_x;
                    enemies[idx].y = old_y;
                } else {
                    enemies[idx].x = old_x;
                    enemies[idx].y = old_y;
                }
            }

            if (enemies[idx].w == 2) {
                draw_car(&enemies[idx], BIG_ENEMY_TOP, BIG_ENEMY_BOT);
            } else {
                draw_car(&enemies[idx], ENEMY_TOP, ENEMY_BOT);
            }
        }

        input = _XL_INPUT();
        if (_XL_LEFT(input) && player_x > ROAD_L) {
            uint8_t ok;
            ok = 1;
            for (k = 0; k < enemy_count && ok; k++) {
                if (cars_overlap((uint8_t)(player_x - 1), PLAYER_Y, 1, 2,
                                enemies[k].x, enemies[k].y, enemies[k].w, enemies[k].h)) {
                    ok = 0;
                }
            }
            if (ok) {
                _XL_DELETE(player_x, PLAYER_Y);
                _XL_DELETE(player_x, (uint8_t)(PLAYER_Y + 1));
                player_x--;
                _XL_DRAW(player_x, PLAYER_Y, PLAYER_TOP, _XL_GREEN);
                _XL_DRAW(player_x, (uint8_t)(PLAYER_Y + 1), PLAYER_BOT, _XL_GREEN);
            }
        } else if (_XL_RIGHT(input) && (uint16_t)player_x + 1 <= ROAD_R) {
            uint8_t ok;
            ok = 1;
            for (k = 0; k < enemy_count && ok; k++) {
                if (cars_overlap((uint8_t)(player_x + 1), PLAYER_Y, 1, 2,
                                enemies[k].x, enemies[k].y, enemies[k].w, enemies[k].h)) {
                    ok = 0;
                }
            }
            if (ok) {
                _XL_DELETE(player_x, PLAYER_Y);
                _XL_DELETE(player_x, (uint8_t)(PLAYER_Y + 1));
                player_x++;
                _XL_DRAW(player_x, PLAYER_Y, PLAYER_TOP, _XL_GREEN);
                _XL_DRAW(player_x, (uint8_t)(PLAYER_Y + 1), PLAYER_BOT, _XL_GREEN);
            }
        }

        for (i = 0; i < enemy_count && !game_over; i++) {
            if (cars_overlap(player_x, PLAYER_Y, 1, 2,
                            enemies[i].x, enemies[i].y, enemies[i].w, enemies[i].h)) {
                game_over = 1;
            }
        }

        for (k = 0; k < 2 && !game_over; k++) {
            uint8_t py;
            py = (uint8_t)(PLAYER_Y + k);
            if (is_blocked(player_x, py, blocks, block_count)) {
                game_over = 1;
            }
        }



        if (game_over) {
            uint8_t e;
            for (e = 0; e < enemy_count; e++) {
                erase_car(&enemies[e]);
            }
            _XL_DELETE(player_x, PLAYER_Y);
            _XL_DELETE(player_x, (uint8_t)(PLAYER_Y + 1));
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(ROAD_L, YSize / 2, "GAME OVER");
            _XL_SLEEP(2);
            for (i = 0; i < ROAD_W; i++) {
                _XL_DELETE((uint8_t)(ROAD_L + i), YSize / 2);
            }
        }

        _XL_SLOW_DOWN(slowdown);
    }

    erase_borders();
}


int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();
    while (1) {
        game_loop();
    }
    return 0;
}
