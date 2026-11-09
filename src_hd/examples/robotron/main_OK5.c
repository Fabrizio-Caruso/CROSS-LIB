#include "cross_lib.h"

#define MAX_ENEMIES 24
#define MAX_BULLETS 8
#define ENEMY_MOVE_CHANCE 20
#define UI_ROW 0
#define PLAY_TOP 1

uint8_t player_x, player_y, player_dir, last_move_dir;
uint8_t player_prev_x, player_prev_y, player_prev_dir;
short lives;
uint16_t score, score_last;
uint8_t enemy_x[MAX_ENEMIES], enemy_y[MAX_ENEMIES];
uint8_t enemy_active[MAX_ENEMIES];
uint8_t enemy_prev_x[MAX_ENEMIES], enemy_prev_y[MAX_ENEMIES];
uint8_t enemy_prev_active[MAX_ENEMIES];
uint8_t bullet_x[MAX_BULLETS], bullet_y[MAX_BULLETS];
short bullet_dx[MAX_BULLETS], bullet_dy[MAX_BULLETS];
uint8_t bullet_active[MAX_BULLETS];

static void draw_player(uint8_t x, uint8_t y, uint8_t dir)
{
    uint8_t t0, t1;
    if (dir == 0) { t0 = _TILE_10; t1 = _TILE_11; }
    else          { t0 = _TILE_12; t1 = _TILE_13; }
    _XL_DRAW(x, y, t0, _XL_CYAN);
    _XL_DRAW(x, y + 1, t1, _XL_CYAN);
}
static void del_player(uint8_t x, uint8_t y)
{
    _XL_DELETE(x, y);
    _XL_DELETE(x, y + 1);
}
static void draw_enemy(uint8_t x, uint8_t y)
{
    _XL_DRAW(x, y, _TILE_20, _XL_RED);
    _XL_DRAW(x, y + 1, _TILE_21, _XL_RED);
}
static void del_enemy(uint8_t x, uint8_t y)
{
    _XL_DELETE(x, y);
    _XL_DELETE(x, y + 1);
}

void init_game(void)
{
    uint8_t i, side;
    player_x = XSize / 2;
    player_y = PLAY_TOP + (YSize - PLAY_TOP - 2) / 2;
    player_dir = 1;
    last_move_dir = 3;
    lives = 3;
    score = 0;
    score_last = 0xFFFF;
    player_prev_x = player_x;
    player_prev_y = player_y;
    player_prev_dir = player_dir;

    for (i = 0; i < MAX_ENEMIES; i++) {
        enemy_active[i] = 0;
        enemy_prev_active[i] = 0;
    }
    for (i = 0; i < MAX_BULLETS; i++) bullet_active[i] = 0;

    for (i = 0; i < 12; i++) {
        enemy_active[i] = 1;
        side = _XL_RAND() % 4;
        if (side == 0) { enemy_x[i] = 0;               enemy_y[i] = PLAY_TOP + (uint8_t)(_XL_RAND() % (YSize - PLAY_TOP - 1)); }
        else if (side == 1){ enemy_x[i] = XSize - 1;   enemy_y[i] = PLAY_TOP + (uint8_t)(_XL_RAND() % (YSize - PLAY_TOP - 1)); }
        else if (side == 2){ enemy_x[i] = (uint8_t)(_XL_RAND() % XSize); enemy_y[i] = PLAY_TOP; }
        else               { enemy_x[i] = (uint8_t)(_XL_RAND() % XSize); enemy_y[i] = YSize - 2; }
        enemy_prev_x[i] = enemy_x[i];
        enemy_prev_y[i] = enemy_y[i];
        enemy_prev_active[i] = 1;
        draw_enemy(enemy_x[i], enemy_y[i]);
    }
    draw_player(player_x, player_y, player_dir);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(UI_ROW, 0, "SCORE");
    _XL_PRINTD(6, UI_ROW, 4, score);
}

void update_game(void)
{
    uint8_t inp, i, j, k;
    short dx, dy;
    short bdx[3], bdy[3];
    uint8_t changed;

    if (score != score_last) {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(6, UI_ROW, 4, score);
        score_last = score;
    }

    inp = _XL_INPUT();

    if (_XL_LEFT(inp)) { if (player_x > 0) player_x--; player_dir = 0; last_move_dir = 2; }
    if (_XL_RIGHT(inp)){ if (player_x < XSize - 1) player_x++; player_dir = 1; last_move_dir = 3; }
    if (_XL_UP(inp))   { if (player_y > PLAY_TOP) player_y--; last_move_dir = 0; }
    if (_XL_DOWN(inp)) { if (player_y < YSize - 2) player_y++; last_move_dir = 1; }

    changed = (player_x != player_prev_x || player_y != player_prev_y || player_dir != player_prev_dir);
    if (changed) {
        del_player(player_prev_x, player_prev_y);
        draw_player(player_x, player_y, player_dir);
        player_prev_x = player_x;
        player_prev_y = player_y;
        player_prev_dir = player_dir;
    }

    if (_XL_FIRE(inp)) {
        if (last_move_dir == 2) { bdx[0] = -1; bdy[0] = 0; bdx[1] = -1; bdy[1] = -1; bdx[2] = -1; bdy[2] = 1; }
        else if (last_move_dir == 3){ bdx[0] = 1; bdy[0] = 0; bdx[1] = 1; bdy[1] = -1; bdx[2] = 1; bdy[2] = 1; }
        else if (last_move_dir == 0){ bdx[0] = 0; bdy[0] = -1; bdx[1] = -1; bdy[1] = -1; bdx[2] = 1; bdy[2] = -1; }
        else { bdx[0] = 0; bdy[0] = 1; bdx[1] = -1; bdy[1] = 1; bdx[2] = 1; bdy[2] = 1; }
        for (k = 0; k < 3; k++) {
            for (i = 0; i < MAX_BULLETS; i++) {
                if (!bullet_active[i]) {
                    bullet_active[i] = 1;
                    bullet_x[i] = player_x + bdx[k];
                    bullet_y[i] = player_y + bdy[k];
                    bullet_dx[i] = bdx[k];
                    bullet_dy[i] = bdy[k];
                    _XL_DRAW(bullet_x[i], bullet_y[i], _TILE_0, _XL_YELLOW);
                    _XL_SHOOT_SOUND();
                    break;
                }
            }
        }
    }

    for (i = 0; i < MAX_BULLETS; i++) {
        if (bullet_active[i]) {
            uint8_t oldx = bullet_x[i], oldy = bullet_y[i];
            bullet_x[i] += bullet_dx[i];
            bullet_y[i] += bullet_dy[i];
            _XL_DELETE(oldx, oldy);
            if (bullet_x[i] < XSize && bullet_y[i] < YSize) {
                _XL_DRAW(bullet_x[i], bullet_y[i], _TILE_0, _XL_YELLOW);
            } else {
                bullet_active[i] = 0;
            }
            for (j = 0; j < MAX_ENEMIES; j++) {
                if (enemy_active[j] && bullet_x[i] == enemy_x[j] &&
                    (bullet_y[i] == enemy_y[j] || bullet_y[i] == enemy_y[j] + 1)) {
                    del_enemy(enemy_x[j], enemy_y[j]);
                    enemy_active[j] = 0;
                    bullet_active[i] = 0;
                    _XL_DELETE(bullet_x[i], bullet_y[i]);
                    score += 10;
                    _XL_TOCK_SOUND();
                }
            }
        }
    }

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (!enemy_active[i]) continue;
        if (_XL_RAND() % ENEMY_MOVE_CHANCE == 0) {
            dx = (player_x > enemy_x[i]) ? 1 : (player_x < enemy_x[i]) ? -1 : 0;
            dy = (player_y > enemy_y[i]) ? 1 : (player_y < enemy_y[i]) ? -1 : 0;
            if (enemy_x[i] + dx < XSize && enemy_x[i] + dx > 0) enemy_x[i] += dx;
            if (enemy_y[i] + dy < YSize - 1 && enemy_y[i] + dy >= PLAY_TOP) enemy_y[i] += dy;
        }
    }

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_active[i] != enemy_prev_active[i]) {
            if (enemy_prev_active[i]) del_enemy(enemy_prev_x[i], enemy_prev_y[i]);
            if (enemy_active[i]) draw_enemy(enemy_x[i], enemy_y[i]);
            enemy_prev_active[i] = enemy_active[i];
            enemy_prev_x[i] = enemy_x[i];
            enemy_prev_y[i] = enemy_y[i];
        } else if (enemy_active[i]) {
            if (enemy_x[i] != enemy_prev_x[i] || enemy_y[i] != enemy_prev_y[i]) {
                del_enemy(enemy_prev_x[i], enemy_prev_y[i]);
                draw_enemy(enemy_x[i], enemy_y[i]);
                enemy_prev_x[i] = enemy_x[i];
                enemy_prev_y[i] = enemy_y[i];
            }
        }
    }

    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_active[i] && enemy_x[i] == player_x &&
            (enemy_y[i] == player_y || enemy_y[i] + 1 == player_y)) {
            lives--;
            del_enemy(enemy_x[i], enemy_y[i]);
            enemy_active[i] = 0;
            _XL_EXPLOSION_SOUND();
        }
    }

    {
        uint8_t spawn_thresh = 3 + (score / 40);
        if (spawn_thresh > 30) spawn_thresh = 30;
        if (_XL_RAND() % 100 < spawn_thresh) {
            for (i = 0; i < MAX_ENEMIES; i++) {
                if (!enemy_active[i]) {
                    uint8_t side = _XL_RAND() % 4;
                    if (side == 0) { enemy_x[i] = 0;               enemy_y[i] = PLAY_TOP + (uint8_t)(_XL_RAND() % (YSize - PLAY_TOP - 1)); }
                    else if (side == 1){ enemy_x[i] = XSize - 1;   enemy_y[i] = PLAY_TOP + (uint8_t)(_XL_RAND() % (YSize - PLAY_TOP - 1)); }
                    else if (side == 2){ enemy_x[i] = (uint8_t)(_XL_RAND() % XSize); enemy_y[i] = PLAY_TOP; }
                    else               { enemy_x[i] = (uint8_t)(_XL_RAND() % XSize); enemy_y[i] = YSize - 2; }
                    enemy_active[i] = 1;
                    draw_enemy(enemy_x[i], enemy_y[i]);
                    enemy_prev_x[i] = enemy_x[i];
                    enemy_prev_y[i] = enemy_y[i];
                    enemy_prev_active[i] = 1;
                    break;
                }
            }
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
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

        if (lives <= 0) {
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(XSize / 2 - 4, YSize / 2, "GAME OVER");
            _XL_WAIT_FOR_INPUT();
            init_game();
        }
    }
    return 0;
}
