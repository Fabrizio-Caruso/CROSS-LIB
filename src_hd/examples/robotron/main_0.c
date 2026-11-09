#include "cross_lib.h"

#define MAX_ENEMIES 6
#define MAX_BULLETS 8

uint8_t player_x;
uint8_t player_y;
uint8_t player_dir;          /* 0 = left, 1 = right */
short lives;
uint16_t score;
uint8_t enemy_x[MAX_ENEMIES];
uint8_t enemy_y[MAX_ENEMIES];
uint8_t enemy_active[MAX_ENEMIES];
uint8_t bullet_x[MAX_BULLETS];
uint8_t bullet_y[MAX_BULLETS];
short bullet_dx[MAX_BULLETS];
short bullet_dy[MAX_BULLETS];
uint8_t bullet_active[MAX_BULLETS];
uint8_t last_dx;
uint8_t last_dy;

void init_game(void)
{
    uint8_t i;
    player_x = XSize / 2;
    player_y = YSize / 2;
    player_dir = 1;
    lives = 3;
    score = 0;
    last_dx = 0;
    last_dy = 0;

    for (i = 0; i < MAX_ENEMIES; i++) {
        enemy_active[i] = 0;
    }
    for (i = 0; i < MAX_BULLETS; i++) {
        bullet_active[i] = 0;
    }

    /* spawn a few enemies */
    for (i = 0; i < 3; i++) {
        enemy_active[i] = 1;
        enemy_x[i] = (uint8_t)((_XL_RAND() % XSize));
        enemy_y[i] = (uint8_t)((_XL_RAND() % (YSize - 2)));
    }
}

void update_game(void)
{
    uint8_t inp;
    uint8_t i, j;
    short dx, dy;

    inp = _XL_INPUT();

    if (_XL_LEFT(inp)) {
        if (player_x > 0) player_x--;
        last_dx = 255; /* left flag */
        last_dy = 0;
        player_dir = 0;
    }
    if (_XL_RIGHT(inp)) {
        if (player_x < XSize - 1) player_x++;
        last_dx = 0;
        last_dy = 0;
        player_dir = 1;
    }
    if (_XL_UP(inp)) {
        if (player_y > 0) player_y--;
        last_dy = 255;
    }
    if (_XL_DOWN(inp)) {
        if (player_y < YSize - 2) player_y++;
        last_dy = 0;
    }

    /* fire */
    if (_XL_FIRE(inp)) {
        for (i = 0; i < MAX_BULLETS; i++) {
            if (!bullet_active[i]) {
                bullet_active[i] = 1;
                bullet_x[i] = player_x;
                bullet_y[i] = player_y;
                bullet_dx[i] = (player_dir == 0) ? -1 : (player_dir == 1) ? 1 : 0;
                if (last_dy == 255) bullet_dy[i] = -1;
                else if (last_dy == 0 && _XL_DOWN(inp)) bullet_dy[i] = 1;
                else bullet_dy[i] = 0;
                if (bullet_dx[i]==0 && bullet_dy[i]==0) bullet_dy[i] = -1;
                _XL_SHOOT_SOUND();
                break;
            }
        }
    }

    /* bullets */
    for (i = 0; i < MAX_BULLETS; i++) {
        if (bullet_active[i]) {
            bullet_x[i] += bullet_dx[i];
            bullet_y[i] += bullet_dy[i];
            if (bullet_x[i] >= XSize || bullet_y[i] >= YSize) {
                bullet_active[i] = 0;
                continue;
            }
            for (j = 0; j < MAX_ENEMIES; j++) {
                if (enemy_active[j] &&
                    bullet_x[i] == enemy_x[j] &&
                    (bullet_y[i] == enemy_y[j] || bullet_y[i] == enemy_y[j] + 1)) {
                    enemy_active[j] = 0;
                    bullet_active[i] = 0;
                    score += 10;
                    _XL_TOCK_SOUND();
                }
            }
        }
    }

    /* enemies */
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (!enemy_active[i]) continue;
        dx = (player_x > enemy_x[i]) ? 1 : (player_x < enemy_x[i]) ? -1 : 0;
        dy = (player_y > enemy_y[i]) ? 1 : (player_y < enemy_y[i]) ? -1 : 0;
        if (enemy_x[i] + dx < XSize && enemy_x[i] + dx > 0) enemy_x[i] += dx;
        if (enemy_y[i] + dy < YSize - 1 && enemy_y[i] + dy > 0) enemy_y[i] += dy;

        if (enemy_x[i] == player_x &&
            (enemy_y[i] == player_y || enemy_y[i] + 1 == player_y ||
             enemy_y[i] == player_y + 1)) {
            lives--;
            enemy_active[i] = 0;
            _XL_EXPLOSION_SOUND();
        }
    }

    /* occasional respawn */
    if (_XL_RAND() % 100 < 2) {
        for (i = 0; i < MAX_ENEMIES; i++) {
            if (!enemy_active[i]) {
                enemy_active[i] = 1;
                enemy_x[i] = (uint8_t)((_XL_RAND() % XSize));
                enemy_y[i] = (uint8_t)((_XL_RAND() % (YSize - 2)));
                break;
            }
        }
    }
}

void draw_game(void)
{
    uint8_t i;
    uint8_t top_tile, bottom_tile;

    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "SCORE");
    _XL_PRINTD(6, 0, 4, score);
    _XL_PRINT(XSize - 10, 0, "LIVES");
    _XL_PRINTD(XSize - 4, 0, 2, lives);

    if (player_dir == 0) {
        top_tile = _TILE_10;
        bottom_tile = _TILE_11;
    } else {
        top_tile = _TILE_12;
        bottom_tile = _TILE_13;
    }
    _XL_DRAW(player_x, player_y, top_tile, _XL_CYAN);
    _XL_DRAW(player_x, player_y + 1, bottom_tile, _XL_CYAN);

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_active[i]) {
            _XL_DRAW(enemy_x[i], enemy_y[i], _TILE_20, _XL_RED);
            _XL_DRAW(enemy_x[i], enemy_y[i] + 1, _TILE_21, _XL_RED);
        }
    }

    for (i = 0; i < MAX_BULLETS; i++) {
        if (bullet_active[i]) {
            _XL_DRAW(bullet_x[i], bullet_y[i], _TILE_0, _XL_YELLOW);
        }
    }
}

int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    init_game();

    while (1) {
        update_game();
        draw_game();
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

        if (lives <= 0) {
            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(XSize / 2 - 4, YSize / 2, "GAME OVER");
            _XL_WAIT_FOR_INPUT();
            init_game();
        }
    }
    return 0;
}
