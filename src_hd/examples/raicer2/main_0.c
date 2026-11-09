#include "cross_lib.h"

#define ROAD_L 1
#define ROAD_R (XSize - 2)
#define ROAD_W (ROAD_R - ROAD_L + 1)
#define PLAYER_Y (YSize - 4)
#define MAX_ENEMIES 8
#define OBSTACLE_TILE _TILE_6
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

static void draw_car(Car *c, uint8_t top_tile, uint8_t bot_tile)
{
    uint8_t i;
    for (i = 0; i < c->h; i++) {
        if ((uint16_t)c->y + i >= YSize) break;
        if (i == 0) {
            uint8_t j;
            for (j = 0; j < c->w; j++) {
                _XL_DRAW(c->x + j, c->y + i, top_tile, _XL_RED);
            }
        } else {
            uint8_t j;
            for (j = 0; j < c->w; j++) {
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

static uint8_t row_left_edge(uint8_t y, uint8_t *left_blk, uint8_t *right_blk)
{
    if (left_blk[y] > 0 && left_blk[y] < ROAD_W) return ROAD_L + left_blk[y];
    if (right_blk[y] > 0 && right_blk[y] < ROAD_W) return ROAD_R - right_blk[y] + 1;
    return ROAD_L;
}

static uint8_t row_right_edge(uint8_t y, uint8_t *left_blk, uint8_t *right_blk)
{
    if (right_blk[y] > 0 && right_blk[y] < ROAD_W) return ROAD_R - right_blk[y];
    if (left_blk[y] > 0 && left_blk[y] < ROAD_W) return ROAD_L + left_blk[y] - 1;
    return ROAD_R;
}

static uint8_t is_blocked(uint8_t x, uint8_t y, uint8_t *left_blk, uint8_t *right_blk,
                          uint8_t *cent_l, uint8_t *cent_w)
{
    if (x < ROAD_L || x > ROAD_R) return 1;
    if (left_blk[y] > 0 && left_blk[y] < ROAD_W && x >= ROAD_L && x < ROAD_L + left_blk[y]) return 1;
    if (right_blk[y] > 0 && right_blk[y] < ROAD_W && x > ROAD_R - right_blk[y] && x <= ROAD_R) return 1;
    if (cent_w[y] > 0 && cent_l[y] >= ROAD_L && cent_l[y] <= ROAD_R) {
        uint8_t cl = cent_l[y];
        uint8_t cr = cent_l[y] + cent_w[y] - 1;
        if (cr > ROAD_R) cr = ROAD_R;
        if (x >= cl && x <= cr) return 1;
    }
    return 0;
}

static void draw_obstacles(uint8_t y, uint8_t *left_blk, uint8_t *right_blk,
                           uint8_t *cent_l, uint8_t *cent_w)
{
    uint8_t x;
    for (x = ROAD_L; x <= ROAD_R; x++) {
        if (is_blocked(x, y, left_blk, right_blk, cent_l, cent_w)) {
            _XL_DRAW(x, y, OBSTACLE_TILE, _XL_YELLOW);
        }
    }
}

static void erase_obstacles(uint8_t y, uint8_t *left_blk, uint8_t *right_blk,
                            uint8_t *cent_l, uint8_t *cent_w)
{
    uint8_t x;
    for (x = ROAD_L; x <= ROAD_R; x++) {
        if (is_blocked(x, y, left_blk, right_blk, cent_l, cent_w)) {
            _XL_DELETE(x, y);
        }
    }
}

static void gen_row(uint8_t y, uint8_t *left_blk, uint8_t *right_blk,
                    uint8_t *cent_l, uint8_t *cent_w)
{
    uint16_t r;
    left_blk[y] = 0;
    right_blk[y] = 0;
    cent_l[y] = ROAD_L;
    cent_w[y] = 0;
    if (ROAD_W < 5) return;
    r = _XL_RAND();
    if (r % 10 == 0 && ROAD_W >= 7) {
        uint8_t bw;
        bw = (uint8_t)(2 + _XL_RAND() % 3);
        cent_l[y] = (uint8_t)(ROAD_L + 1 + _XL_RAND() % (ROAD_W - 2));
        if ((uint16_t)cent_l[y] + bw > ROAD_R) {
            uint8_t maxw = (uint8_t)(ROAD_R - cent_l[y] + 1);
            if (bw > maxw) bw = maxw;
        }
        cent_w[y] = bw;
    } else if (r % 7 == 0 && ROAD_W >= 5) {
        left_blk[y] = (uint8_t)(2 + _XL_RAND() % 3);
        if (left_blk[y] > ROAD_W - 2) left_blk[y] = ROAD_W - 2;
    } else if (r % 7 == 1 && ROAD_W >= 5) {
        right_blk[y] = (uint8_t)(2 + _XL_RAND() % 3);
        if (right_blk[y] > ROAD_W - 2) right_blk[y] = ROAD_W - 2;
    }
}

static void shift_rows(uint8_t *left_blk, uint8_t *right_blk,
                       uint8_t *cent_l, uint8_t *cent_w)
{
    uint8_t y;
    for (y = YSize - 1; y > 0; y--) {
        left_blk[y] = left_blk[y - 1];
        right_blk[y] = right_blk[y - 1];
        cent_l[y] = cent_l[y - 1];
        cent_w[y] = cent_w[y - 1];
    }
}

static void spawn_enemy(Car *enemies, uint8_t *count, uint8_t y,
                        uint8_t *left_blk, uint8_t *right_blk,
                        uint8_t *cent_l, uint8_t *cent_w)
{
    uint16_t r;
    uint8_t w, h, x, i;
    if (*count >= MAX_ENEMIES) return;
    r = _XL_RAND();
    if (r % 5 == 0) {
        w = 2;
        h = 3;
    } else {
        w = 1;
        h = 2;
    }
    x = row_left_edge(y, left_blk, right_blk);
    r = _XL_RAND();
    if (r % 4 == 0) {
        uint8_t re;
        re = row_right_edge(y, left_blk, right_blk);
        if ((uint16_t)re - w + 1 >= x) {
            x = (uint8_t)(re - w + 1);
        }
    } else {
        uint8_t max_x;
        max_x = row_right_edge(y, left_blk, right_blk);
        if ((uint16_t)x + w > max_x + 1) return;
        x = (uint8_t)(x + _XL_RAND() % (max_x - x + 2 - w));
    }
    for (i = 0; i < h; i++) {
        uint8_t yy;
        yy = (uint8_t)(y + i);
        if (yy >= YSize) return;
        if (is_blocked(x, yy, left_blk, right_blk, cent_l, cent_w)) return;
    }
    enemies[*count].x = x;
    enemies[*count].y = y;
    enemies[*count].w = w;
    enemies[*count].h = h;
    (*count)++;
}

static uint8_t cars_overlap(uint8_t ax, uint8_t ay, uint8_t aw, uint8_t ah,
                            uint8_t bx, uint8_t by, uint8_t bw, uint8_t bh)
{
    if (ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by) return 1;
    return 0;
}

static void game_loop(void)
{
    uint8_t left_blk[YSize];
    uint8_t right_blk[YSize];
    uint8_t cent_l[YSize];
    uint8_t cent_w[YSize];
    Car enemies[MAX_ENEMIES];
    uint8_t enemy_count;
    uint8_t player_x;
    uint8_t i, j;
    uint16_t frame;
    uint16_t score;
    uint8_t game_over;
    uint8_t input;

    _XL_CLEAR_SCREEN();
    for (i = 0; i < YSize; i++) {
        left_blk[i] = 0;
        right_blk[i] = 0;
        cent_l[i] = ROAD_L;
        cent_w[i] = 0;
    }
    player_x = (uint8_t)(ROAD_L + ROAD_W / 2);
    enemy_count = 0;
    frame = 0;
    score = 0;
    game_over = 0;

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "RACING");
    _XL_SLEEP(1);
    _XL_DELETE(0, 0);
    for (i = 0; i < 4 && i < XSize; i++) {
        _XL_DELETE(i, 0);
    }

    while (!game_over) {
        frame++;
        if (frame % 3 == 0) {
            uint8_t y;
            for (y = 1; y < YSize - 1; y++) {
                erase_obstacles(y, left_blk, right_blk, cent_l, cent_w);
            }
            shift_rows(left_blk, right_blk, cent_l, cent_w);
            gen_row(0, left_blk, right_blk, cent_l, cent_w);
            for (y = 1; y < YSize - 1; y++) {
                draw_obstacles(y, left_blk, right_blk, cent_l, cent_w);
            }
        }

        if (frame % 8 == 0) {
            spawn_enemy(enemies, &enemy_count, 0, left_blk, right_blk, cent_l, cent_w);
        }

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
                uint8_t can_move;
                uint8_t k;
                old_x = enemies[idx].x;
                old_y = enemies[idx].y;
                erase_car(&enemies[idx]);

                if (frame % 12 == 0 && _XL_RAND() % 3 == 0) {
                    new_x = old_x;
                    if (_XL_RAND() % 2 == 0) {
                        if (old_x > ROAD_L) new_x = old_x - 1;
                    } else {
                        if ((uint16_t)old_x + enemies[idx].w < ROAD_R) new_x = old_x + 1;
                    }
                } else {
                    new_x = old_x;
                }

                can_move = 1;
                for (k = 0; k < enemies[idx].h && can_move; k++) {
                    uint8_t ny;
                    ny = (uint8_t)(old_y + 1 + k);
                    if (ny >= YSize) { can_move = 0; break; }
                    {
                        uint8_t m;
                        for (m = 0; m < enemies[idx].w && can_move; m++) {
                            if (is_blocked(new_x + m, ny, left_blk, right_blk, cent_l, cent_w)) {
                                can_move = 0;
                            }
                        }
                    }
                }

                if (can_move) {
                    enemies[idx].x = new_x;
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
            uint8_t k;
            uint8_t ok;
            ok = 1;
            for (k = 0; k < 2 && ok; k++) {
                uint8_t py;
                py = (uint8_t)(PLAYER_Y + k);
                if (is_blocked(player_x - 1, py, left_blk, right_blk, cent_l, cent_w)) ok = 0;
            }
            for (k = 0; k < enemy_count && ok; k++) {
                if (cars_overlap(player_x - 1, PLAYER_Y, 1, 2,
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
            uint8_t k;
            uint8_t ok;
            ok = 1;
            for (k = 0; k < 2 && ok; k++) {
                uint8_t py;
                py = (uint8_t)(PLAYER_Y + k);
                if (is_blocked(player_x + 1, py, left_blk, right_blk, cent_l, cent_w)) ok = 0;
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

        for (i = 0; i < enemy_count && !game_over; i++) {
            uint8_t k;
            for (k = 0; k < enemies[i].w; k++) {
                if (is_blocked(enemies[i].x + k, PLAYER_Y, left_blk, right_blk, cent_l, cent_w)) {
                    game_over = 1;
                    break;
                }
                if (game_over) break;
            }
        }

        score++;
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0, YSize - 1, 1, score);

        if (game_over) {
            uint8_t e;
            for (e = 0; e < enemy_count; e++) {
                erase_car(&enemies[e]);
            }
            _XL_DELETE(player_x, PLAYER_Y);
            _XL_DELETE(player_x, (uint8_t)(PLAYER_Y + 1));
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(0, YSize / 2 - 1, "GAME OVER");
            _XL_SLEEP(2);
            for (i = 0; i < 9 && i < XSize; i++) {
                _XL_DELETE(i, YSize / 2 - 1);
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
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
