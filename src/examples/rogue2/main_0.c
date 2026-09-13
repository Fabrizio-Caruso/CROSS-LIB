#include "cross_lib.h"

#define MAP_W 15
#define MAP_H 10
#define OX ((XSize - MAP_W) / 2)
#define OY ((YSize - MAP_H) / 2 + 3)
#define VIS_R 2
#define MAX_ENEMIES 4
#define MAX_BULLETS 4
#define ENEMY_INTERVAL 3

static uint8_t map_data[MAP_W][MAP_H];
static uint8_t item_map[MAP_W][MAP_H];
static uint8_t prev_tile[MAP_W][MAP_H];
static uint8_t prev_color[MAP_W][MAP_H];

static uint8_t px, py;
static uint8_t has_gun;
static uint8_t invincible;
static uint16_t score;
static uint8_t lives;
static short last_dx, last_dy;

static uint8_t ex[MAX_ENEMIES];
static uint8_t ey[MAX_ENEMIES];
static uint8_t enemy_count;

static uint8_t bx[MAX_BULLETS];
static uint8_t by[MAX_BULLETS];
static short bdx[MAX_BULLETS];
static short bdy[MAX_BULLETS];
static uint8_t bullet_count;

static void init_map(void)
{
    uint8_t x, y;
    for (y = 0; y < MAP_H; y++) {
        for (x = 0; x < MAP_W; x++) {
            if (y == 0 || y == MAP_H - 1 || x == 0 || x == MAP_W - 1)
                map_data[x][y] = 1;
            else
                map_data[x][y] = 0;
        }
    }
    map_data[3][2]=1; map_data[4][2]=1; map_data[5][2]=1;
    map_data[9][2]=1; map_data[10][2]=1; map_data[11][2]=1;
    map_data[3][3]=1; map_data[9][3]=1;
    map_data[3][4]=1; map_data[4][4]=1; map_data[5][4]=1;
    map_data[9][4]=1; map_data[10][4]=1; map_data[11][4]=1;
    map_data[3][5]=1; map_data[9][5]=1;
    map_data[3][6]=1; map_data[4][6]=1; map_data[5][6]=1;
    map_data[9][6]=1; map_data[10][6]=1; map_data[11][6]=1;
    map_data[MAP_W-2][1] = 2;
}

static void init_items(void)
{
    uint8_t x, y;
    for (y = 0; y < MAP_H; y++)
        for (x = 0; x < MAP_W; x++)
            item_map[x][y] = 0;
    item_map[7][4] = 1;
    item_map[12][3] = 2;
}

static void init_enemies(void)
{
    enemy_count = MAX_ENEMIES;
    ex[0]=5; ey[0]=7;
    ex[1]=9; ey[1]=3;
    ex[2]=12;ey[2]=6;
    ex[3]=4; ey[3]=5;
}

static void reset_game(void)
{
    px=1; py=1;
    has_gun=0; invincible=0;
    score=0; lives=3;
    last_dx=1; last_dy=0;
    bullet_count=0;
    init_map();
    init_items();
    init_enemies();
}

static void clear_prev(void)
{
    uint8_t x, y;
    for (y = 0; y < MAP_H; y++)
        for (x = 0; x < MAP_W; x++) {
            prev_tile[x][y] = 255;
            prev_color[x][y] = 255;
        }
}

static uint8_t is_visible(uint8_t cx, uint8_t cy)
{
    short dx = (short)cx - (short)px;
    short dy = (short)cy - (short)py;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return ((uint8_t)(dx + dy) <= VIS_R);
}

static uint8_t is_wall(uint8_t x, uint8_t y)
{
    return map_data[x][y] == 1;
}

static void move_enemies(void)
{
    uint8_t i;
    short dx, dy;
    for (i = 0; i < enemy_count; i++) {
        if ((short)(px - ex[i]) > 0) dx = 1;
        else if ((short)(px - ex[i]) < 0) dx = -1;
        else dx = 0;
        if ((short)(py - ey[i]) > 0) dy = 1;
        else if ((short)(py - ey[i]) < 0) dy = -1;
        else dy = 0;
        if (dx != 0 && !is_wall((uint8_t)(ex[i]+dx), ey[i])) {
            ex[i] = (uint8_t)(ex[i]+dx);
        } else if (dy != 0 && !is_wall(ex[i], (uint8_t)(ey[i]+dy))) {
            ey[i] = (uint8_t)(ey[i]+dy);
        }
    }
}

static void move_bullets(void)
{
    uint8_t i, j;
    for (i = 0; i < bullet_count; i++) {
        bx[i] = (uint8_t)(bx[i] + bdx[i]);
        by[i] = (uint8_t)(by[i] + bdy[i]);
        if (is_wall(bx[i], by[i])) {
            for (j = i; j < bullet_count - 1; j++) {
                bx[j]=bx[j+1]; by[j]=by[j+1];
                bdx[j]=bdx[j+1]; bdy[j]=bdy[j+1];
            }
            bullet_count--;
            i--;
        } else {
            for (j = 0; j < enemy_count; j++) {
                if (bx[i]==ex[j] && by[i]==ey[j]) {
                    for (; j<enemy_count-1; j++) { ex[j]=ex[j+1]; ey[j]=ey[j+1]; }
                    enemy_count--;
                    score += 50;
                    _XL_PING_SOUND();
                    break;
                }
            }
        }
    }
}

static void check_collisions(void)
{
    uint8_t i;
    for (i = 0; i < enemy_count; i++) {
        if (px==ex[i] && py==ey[i]) {
            if (!invincible) {
                lives--;
                _XL_EXPLOSION_SOUND();
                if (lives > 0) { px=1; py=1; }
            } else {
                for (; i<enemy_count-1; i++) { ex[i]=ex[i+1]; ey[i]=ey[i+1]; }
                enemy_count--;
                score += 50;
                _XL_PING_SOUND();
            }
        }
    }
}

static void check_items(void)
{
    if (item_map[px][py]==1 && !has_gun) {
        has_gun=1; item_map[px][py]=0;
        _XL_TICK_SOUND();
    } else if (item_map[px][py]==2) {
        invincible=5; item_map[px][py]=0;
        _XL_TOCK_SOUND();
    }
}

static void render(void)
{
    uint8_t x, y, sx, sy;
    uint8_t tile, color;
    uint8_t i;
    for (y = 0; y < MAP_H; y++) {
        for (x = 0; x < MAP_W; x++) {
            sx = OX + x;
            sy = OY + y;
            if (!is_visible(x, y)) {
                tile = _TILE_1;
                color = _XL_BLUE;
            } else {
                tile = 0; color = 0;
                for (i = 0; i < bullet_count; i++) {
                    if (bx[i]==x && by[i]==y) { tile=_TILE_2; color=_XL_YELLOW; break; }
                }
                if (!tile) {
                    for (i = 0; i < enemy_count; i++) {
                        if (ex[i]==x && ey[i]==y) { tile=_TILE_7; color=_XL_RED; break; }
                    }
                }
                if (!tile && item_map[x][y]) {
                    if (item_map[x][y]==1) { tile=_TILE_9; color=_XL_CYAN; }
                    else { tile=_TILE_11; color=_XL_MAGENTA; }
                }
                if (!tile && px==x && py==y) {
                    tile = _TILE_5;
                    color = invincible ? _XL_YELLOW : _XL_GREEN;
                }
                if (!tile) {
                    if (map_data[x][y]==1) { tile=_TILE_1; color=_XL_WHITE; }
                    else if (map_data[x][y]==2) { tile=_TILE_3; color=_XL_YELLOW; }
                    else { tile=_TILE_0; color=_XL_BLUE; }
                }
            }
            if (tile != prev_tile[x][y] || color != prev_color[x][y]) {
                _XL_DRAW(sx, sy, tile, color);
                prev_tile[x][y] = tile;
                prev_color[x][y] = color;
            }
        }
    }
}

static void render_ui(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 1, "SCORE");
    _XL_PRINTD(OX + MAP_W - 4, 1, 1, score);
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(2, 3, "LIVES");
    _XL_PRINTD(OX + MAP_W - 4, 3, 1, lives);
    if (has_gun) {
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(2, 5, "GUN: YES");
    } else {
        _XL_SET_TEXT_COLOR(_XL_BLUE);
        _XL_PRINT(2, 5, "GUN: NO ");
    }
    if (invincible) {
        _XL_SET_TEXT_COLOR(_XL_MAGENTA);
        _XL_PRINT(OX + MAP_W - 8, 5, "INVINCIBLE");
    } else {
        _XL_SET_TEXT_COLOR(_XL_BLUE);
        _XL_PRINT(OX + MAP_W - 8, 5, "          ");
    }
}

int main(void)
{
    uint8_t input;
    uint8_t frame_count;
    uint8_t game_over;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        reset_game();
        clear_prev();
        frame_count = 0;
        game_over = 0;

        render();
        render_ui();

        while (!game_over) {
            input = _XL_INPUT();

            if (_XL_LEFT(input)) {
                last_dx=-1; last_dy=0;
                if (px > 0 && !is_wall(px-1, py)) px--;
            } else if (_XL_RIGHT(input)) {
                last_dx=1; last_dy=0;
                if ((uint8_t)(px+1) < MAP_W && !is_wall(px+1, py)) px++;
            } else if (_XL_UP(input)) {
                last_dx=0; last_dy=-1;
                if (py > 0 && !is_wall(px, py-1)) py--;
            } else if (_XL_DOWN(input)) {
                last_dx=0; last_dy=1;
                if ((uint8_t)(py+1) < MAP_H && !is_wall(px, py+1)) py++;
            }

            if (has_gun && _XL_FIRE(input) && bullet_count < MAX_BULLETS) {
                bx[bullet_count] = px;
                by[bullet_count] = py;
                bdx[bullet_count] = last_dx;
                bdy[bullet_count] = last_dy;
                bullet_count++;
                _XL_SHOOT_SOUND();
            }

            move_bullets();

            frame_count++;
            if (frame_count >= ENEMY_INTERVAL) {
                frame_count = 0;
                move_enemies();
            }

            if (invincible > 0) invincible--;

            check_collisions();
            check_items();

            if (map_data[px][py] == 2) {
                game_over = 1;
                score += 100;
                _XL_ZAP_SOUND();
            }
            if (lives == 0) {
                game_over = 1;
                _XL_EXPLOSION_SOUND();
            }

            render();
            render_ui();
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT((XSize-20)/2, YSize/2, "GAME OVER");
        _XL_WAIT_FOR_INPUT();
    }

    return 0;
}
