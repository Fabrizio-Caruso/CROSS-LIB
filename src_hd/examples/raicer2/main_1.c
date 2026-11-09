#include "cross_lib.h"

#define ROAD_L 1
#define ROAD_R (XSize - 2)
#define ROAD_W (ROAD_R - ROAD_L + 1)
#define PLAYER_Y (YSize - 4)
#define MAX_ENEMIES 8
#define MAX_BLOCKS 16
#define OBSTACLE_TILE _TILE_6
#define BORDER_TILE _TILE_5
#define PLAYER_TOP _TILE_0
#define PLAYER_BOT _TILE_1
#define ENEMY_TOP _TILE_2
#define ENEMY_BOT _TILE_3

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
} Car;

typedef struct {
    uint8_t x;
    uint8_t top;
    uint8_t height;
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
        _XL_DRAW(0, y, BORDER_TILE, _XL_WHITE);
        _XL_DRAW(XSize - 1, y, BORDER_TILE, _XL_WHITE);
    }
}

static void erase_borders(void)
{
    uint8_t y;
    for (y = 0; y < YSize; y++) {
        _XL_DELETE(0, y);
        _XL_DELETE(XSize - 1, y);
    }
}

static uint8_t is_blocked(uint8_t x, uint8_t y, Block *blocks, uint8_t block_count)
{
    uint8_t i;
    if (x == 0 || x >= XSize - 1) return 1;
    for (i = 0; i < block_count; i++) {
        if (blocks[i].x == x && y >= blocks[i].top &&
            y < (uint8_t)(blocks[i].top + blocks[i].height)) {
            return 1;
        }
    }
    return 0;
}

static void draw_block(Block *b)
{
    uint8_t i;
    for (i = 0; i < b->height; i++) {
        if ((uint16_t)b->top + i >= YSize) break;
        _XL_DRAW(b->x, (uint8_t)(b->top + i), OBSTACLE_TILE, _XL_YELLOW);
    }
}

static void erase_block(Block *b)
{
    uint8_t i;
    for (i = 0; i < b->height; i++) {
        if ((uint16_t)b->top + i >= YSize) break;
        _XL_DELETE(b->x, (uint8_t)(b->top + i));
    }
}

static void spawn_block(Block *blocks, uint8_t *block_count)
{
    uint16_t r;
    uint8_t h;
    if (*block_count >= MAX_BLOCKS) return;
    r = _XL_RAND();
    h = (uint8_t)(3 + _XL_RAND() % 6);
    if (h > YSize - 2) h = YSize - 2;
    blocks[*block_count].top = 0;
    blocks[*block_count].height = h;
    if (r % 4 == 0 && ROAD_W >= 5) {
        uint8_t min_x, max_x;
        min_x = ROAD_L + 1;
        max_x = ROAD_R - 2;
        if (min_x <= max_x) {
            blocks[*block_count].x = (uint8_t)(min_x + _XL_RAND() % (max_x - min_x + 1));
        } else {
            blocks[*block_count].x = ROAD_L;
        }
    } else if (r % 2 == 0) {
        blocks[*block_count].x = ROAD_L;
    } else {
        blocks[*block_count].x = ROAD_R;
    }
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
    uint8_t i, j, k;
    uint16_t frame;
    uint16_t score;
    uint8_t game_over;
    uint8_t input;

    _XL_CLEAR_SCREEN();
    draw_borders();
    block_count = 0;
    enemy_count = 0;
    player_x = (uint8_t)(ROAD_L + ROAD_W / 2);
    frame = 0;
    score = 0;
    game_over = 0;

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(ROAD_L, YSize / 2, "RACING");
    _XL_SLEEP(1);
    for (i = 0; i < 6 && i <= ROAD_R - ROAD_L; i++) {
        _XL_DELETE((uint8_t)(ROAD_L + i), YSize / 2);
    }

    while (!game_over) {
        frame++;

        if (frame % 4 == 0) {
            for (i = block_count; i > 0; i--) {
                uint8_t idx;
                idx = i - 1;
                erase_block(&blocks[idx]);
                blocks[idx].top++;
                draw_block(&blocks[idx]);
                if (blocks[idx].top >= YSize) {
                    blocks[idx] = blocks[block_count - 1];
                    block_count--;
                }
            }
        }

        if (frame % 20 == 0 && ROAD_W >= 4) {
            spawn_block(blocks, &block_count);
        }

        if (frame % 8 == 0) {
            uint16_t r;
            uint8_t w, h, x, max_x, min_x;
            uint8_t ok;
            if (enemy_count >= MAX_ENEMIES) goto skip_spawn;
            r = _XL_RAND();
            if (r % 5 == 0) {
                w = 2;
                h = 3;
            } else {
                w = 1;
                h = 2;
            }
            min_x = ROAD_L;
            max_x = (uint8_t)(ROAD_R - w + 1);
            if (max_x < min_x) goto skip_spawn;
            x = (uint8_t)(min_x + _XL_RAND() % (max_x - min_x + 1));
            ok = 1;
            for (k = 0; k < h && ok; k++) {
                uint8_t m;
                for (m = 0; m < w && ok; m++) {
                    if (is_blocked(x + m, k, blocks, block_count)) ok = 0;
                }
            }
            if (!ok) goto skip_spawn;
            enemies[enemy_count].x = x;
            enemies[enemy_count].y = 0;
            enemies[enemy_count].w = w;
            enemies[enemy_count].h = h;
            enemy_count++;
        }
skip_spawn:

        for (i = enemy_count; i > 0; i--) {
            uint8_t idx;
            idx = i - 1;
            if ((uint16_t)enemies[idx].y + enemies[idx].h >= YSize) {
                erase_car(&enemies[idx]);
                enemies[idx] = enemies[enemy_count - 1];
                enemy_count--;
            } else {
                uint8_t old_x, old_y;
                uint8_t new_x;
                uint8_t can_move_down;
                uint8_t can_move_diag;
                uint8_t do_diag;
                uint8_t dir;
                old_x = enemies[idx].x;
                old_y = enemies[idx].y;

                if (frame % 12 == 0 && _XL_RAND() % 3 == 0) {
                    do_diag = 1;
                    dir = (uint8_t)(_XL_RAND() % 2);
                } else {
                    do_diag = 0;
                    dir = 0;
                }

                can_move_down = 1;
                for (k = 0; k < enemies[idx].h && can_move_down; k++) {
                    uint8_t ny;
                    ny = (uint8_t)(old_y + 1 + k);
                    if (ny >= YSize) { can_move_down = 0; break; }
                    {
                        uint8_t m;
                        for (m = 0; m < enemies[idx].w && can_move_down; m++) {
                            if (is_blocked(old_x + m, ny, blocks, block_count)) {
                                can_move_down = 0;
                            }
                        }
                    }
                }

                new_x = old_x;
                if (do_diag) {
                    if (dir == 0 && old_x > ROAD_L) {
                        new_x = old_x - 1;
                    } else if (dir == 1 && (uint16_t)old_x + enemies[idx].w < XSize - 1) {
                        new_x = old_x + 1;
                    }
                }

                can_move_diag = 0;
                if (do_diag && new_x != old_x) {
                    uint8_t m2, k2;
                    can_move_diag = 1;
                    for (k2 = 0; k2 < enemies[idx].h && can_move_diag; k2++) {
                        uint8_t ny2;
                        ny2 = (uint8_t)(old_y + 1 + k2);
                        if (ny2 >= YSize) { can_move_diag = 0; break; }
                        for (m2 = 0; m2 < enemies[idx].w && can_move_diag; m2++) {
                            if (is_blocked(new_x + m2, ny2, blocks, block_count)) {
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
                    enemies[idx].x = old_x;
                    enemies[idx].y = old_y;
                }

                draw_car(&enemies[idx], ENEMY_TOP, ENEMY_BOT);
            }
        }

        input = _XL_INPUT();
        if (_XL_LEFT(input) && player_x > ROAD_L) {
            uint8_t ok;
            ok = 1;
            for (k = 0; k < 2 && ok; k++) {
                uint8_t py;
                py = (uint8_t)(PLAYER_Y + k);
                if (is_blocked(player_x - 1, py, blocks, block_count)) ok = 0;
            }
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
            for (k = 0; k < 2 && ok; k++) {
                uint8_t py;
                py = (uint8_t)(PLAYER_Y + k);
                if (is_blocked((uint8_t)(player_x + 1), py, blocks, block_count)) ok = 0;
            }
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

        score++;
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(ROAD_L, YSize - 1, 1, score);

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
            for (i = 0; i < 9 && (uint16_t)ROAD_L + i <= ROAD_R; i++) {
                _XL_DELETE((uint8_t)(ROAD_L + i), YSize / 2);
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
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
