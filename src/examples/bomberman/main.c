/* Bomberman clone – Cross-Lib API, no heap, dirty-tile rendering */
#include "cross_lib.h"

#define GRID_W      20
#define GRID_H      18
#define BOMB_DELAY  18
#define NUM_ENEMIES  4
#define MAX_BOMBS    4
#define BOMB_POWER   5


/* Internal grid state */
#define G_EMPTY   0
#define G_WALL    1
#define G_BLOCK   2
#define G_BOMB    3
#define G_EXPLODE 4
#define G_PLAYER  5
#define G_ENEMY   6


static uint8_t grid[GRID_H][GRID_W];
static uint8_t prev_grid[GRID_H][GRID_W];

/* Active bomb list – only these are scanned each frame */
static uint8_t bomb_x[MAX_BOMBS];
static uint8_t bomb_y[MAX_BOMBS];
static uint8_t bomb_age[MAX_BOMBS];
static uint8_t bomb_active[MAX_BOMBS];

static uint8_t player_x, player_y;
static uint8_t enemy_x[NUM_ENEMIES];
static uint8_t enemy_y[NUM_ENEMIES];
static uint8_t enemy_alive[NUM_ENEMIES];
static uint8_t game_over = 0;
static uint8_t blocks_remaining;
static uint8_t explosion_active = 0;
static uint8_t frame_count = 0;

/* ───────────────────────────────────────────── */
/*  Initialisation                               */
/* ───────────────────────────────────────────── */

void init_game(void)
{
    uint8_t i, j;

    for (i = 0; i < GRID_H; i++) {
        for (j = 0; j < GRID_W; j++) {
            if (i == 0 || i == GRID_H - 1 || j == 0 || j == GRID_W - 1) {
                grid[i][j] = G_WALL;
            } else {
                if ((i & 1) && (j & 1)) {
                    if (i == GRID_H / 2 && j == GRID_W / 2) {
                        grid[i][j] = G_EMPTY;
                    } else {
                        grid[i][j] = G_BLOCK;
                    }
                } else {
                    grid[i][j] = G_EMPTY;
                }
            }
        }
    }

    player_x = GRID_W / 2;
    player_y = GRID_H / 2;
    grid[player_y][player_x] = G_PLAYER;

    enemy_x[0] = 3;   enemy_y[0] = 2;
    enemy_x[1] = 11;  enemy_y[1] = 2;
    enemy_x[2] = 3;   enemy_y[2] = 6;
    enemy_x[3] = 11;  enemy_y[3] = 6;

    for (i = 0; i < NUM_ENEMIES; i++) {
        grid[enemy_y[i]][enemy_x[i]] = G_ENEMY;
        enemy_alive[i] = 1;
    }

    blocks_remaining = 0;
    for (i = 1; i < GRID_H - 1; i++) {
        for (j = 1; j < GRID_W - 1; j++) {
            if (grid[i][j] == G_BLOCK) blocks_remaining++;
        }
    }

    /* Clear bomb list */
    for (i = 0; i < MAX_BOMBS; i++) {
        bomb_active[i] = 0;
        bomb_age[i] = 0;
    }

    for (i = 0; i < GRID_H; i++)
        for (j = 0; j < GRID_W; j++)
            prev_grid[i][j] = G_EMPTY;

    explosion_active = 0;
    frame_count = 0;
}

/* ───────────────────────────────────────────── */
/*  Rendering – dirty tiles only                 */
/* ───────────────────────────────────────────── */

void draw_game(void)
{
    uint8_t i, j;

    for (i = 0; i < GRID_H; i++) {
        for (j = 0; j < GRID_W; j++) {
            if (grid[i][j] != prev_grid[i][j]) {
                if (grid[i][j] == G_EMPTY) {
                    _XL_DELETE(j, i);
                } else {
                    if (grid[i][j] == G_WALL)
                        _XL_DRAW(j, i, _TILE_0, _XL_WHITE);
                    else if (grid[i][j] == G_BLOCK)
                        _XL_DRAW(j, i, _TILE_1, _XL_YELLOW);
                    else if (grid[i][j] == G_BOMB)
                        _XL_DRAW(j, i, _TILE_2, _XL_RED);
                    else if (grid[i][j] == G_EXPLODE)
                        _XL_DRAW(j, i, _TILE_3, _XL_MAGENTA);
                    else if (grid[i][j] == G_PLAYER)
                        _XL_DRAW(j, i, _TILE_4, _XL_GREEN);
                    else if (grid[i][j] == G_ENEMY)
                        _XL_DRAW(j, i, _TILE_5, _XL_BLUE);
                }
                prev_grid[i][j] = grid[i][j];
            }
        }
    }

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(0, GRID_H + 1, "BOMBBERMAN");

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(0, GRID_H + 2, 1, blocks_remaining);

    if (explosion_active) {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(GRID_W - 8, GRID_H + 1, "BOOM!");
    }
}

/* ───────────────────────────────────────────── */
/*  Bomb explosion                               */
/* ───────────────────────────────────────────── */
void explode_bomb(uint8_t bx, uint8_t by)
{
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    uint8_t d, e;

    /* Deactivate this bomb so it won't be processed again */
    uint8_t i;
    for (i = 0; i < MAX_BOMBS; i++) {
        if (bomb_active[i] && bomb_x[i] == bx && bomb_y[i] == by) {
            bomb_active[i] = 0;
        }
    }

    grid[by][bx] = G_EXPLODE;

    if (bx == player_x && by == player_y) {
        game_over = 1;
    }

    for (d = 0; d < 4 && !game_over; d++) {
        uint8_t cx = bx + (uint8_t)dx[d];
        uint8_t cy = by + (uint8_t)dy[d];
        uint8_t steps = 0;

        while (steps < BOMB_POWER &&
               cx >= 1 && cx <= GRID_W - 2 &&
               cy >= 1 && cy <= GRID_H - 2) {
            if (grid[cy][cx] == G_WALL) break;

            /* Chain reaction: detonate another bomb in the blast path */
            if (grid[cy][cx] == G_BOMB) {
                explode_bomb(cx, cy);
            }

            if (grid[cy][cx] == G_BLOCK && blocks_remaining > 0) {
                blocks_remaining--;
            }

            if (grid[cy][cx] == G_ENEMY) {
                for (e = 0; e < NUM_ENEMIES; e++) {
                    if (enemy_alive[e] && enemy_x[e] == cx && enemy_y[e] == cy) {
                        enemy_alive[e] = 0;
                        break;
                    }
                }
            }

            grid[cy][cx] = G_EXPLODE;

            if (cx == player_x && cy == player_y) {
                game_over = 1;
                break;
            }

            cx += (uint8_t)dx[d];
            cy += (uint8_t)dy[d];
            steps++;
        }
    }

    explosion_active = 1;
}


/* ───────────────────────────────────────────── */
/*  Bomb timer & explosion cleanup               */
/* ───────────────────────────────────────────── */

void process_bombs(void)
{
    uint8_t i;
    uint8_t had_explosion = explosion_active;

    /* Age only active bombs – no full-grid scan */
    for (i = 0; i < MAX_BOMBS; i++) {
        if (!bomb_active[i]) continue;

        bomb_age[i]++;
        if (bomb_age[i] >= BOMB_DELAY) {
            explode_bomb(bomb_x[i], bomb_y[i]);
            bomb_active[i] = 0;
        }
    }

    /* Clear explosion tiles only after they have been displayed */
    if (had_explosion && !game_over) {
        uint8_t r, c;
        for (r = 0; r < GRID_H; r++) {
            for (c = 0; c < GRID_W; c++) {
                if (grid[r][c] == G_EXPLODE) {
                    grid[r][c] = G_EMPTY;
                }
            }
        }
        explosion_active = 0;

        if (blocks_remaining == 0 && !game_over) {
            game_over = 1;
        }
    }
}

/* ───────────────────────────────────────────── */
/*  Enemy movement                               */
/* ───────────────────────────────────────────── */

void move_enemies(void)
{
    uint8_t e, nx, ny;
    int dx, dy;

    if (explosion_active || game_over) return;

    for (e = 0; e < NUM_ENEMIES && !game_over; e++) {
        if (!enemy_alive[e]) continue;

        dx = (int)player_x - (int)enemy_x[e];
        dy = (int)player_y - (int)enemy_y[e];

        nx = enemy_x[e];
        ny = enemy_y[e];

        if (_XL_RAND() & 1) {
            if (dx != 0) {
                nx += (uint8_t)(dx > 0 ? 1 : -1);
            } else if (dy != 0) {
                ny += (uint8_t)(dy > 0 ? 1 : -1);
            }
        } else {
            if (dy != 0 && abs(dx) <= abs(dy)) {
                ny += (uint8_t)(dy > 0 ? 1 : -1);
            } else if (dx != 0) {
                nx += (uint8_t)(dx > 0 ? 1 : -1);
            }
        }

        if (nx >= 1 && nx <= GRID_W - 2 && ny >= 1 && ny <= GRID_H - 2) {
            uint8_t target = grid[ny][nx];

            if (target == G_EMPTY || target == G_PLAYER) {
                grid[enemy_y[e]][enemy_x[e]] = G_EMPTY;
                grid[ny][nx] = G_ENEMY;
                enemy_x[e] = nx;
                enemy_y[e] = ny;

                if (target == G_PLAYER) {
                    game_over = 1;
                }
            }
        }
    }
}

/* ───────────────────────────────────────────── */
/*  Player input                                 */
/* ───────────────────────────────────────────── */

void handle_input(void)
{
    uint8_t input, nx, ny;

    // if (explosion_active) {
        // _XL_WAIT_FOR_INPUT();
        // return;
    // }

    input = _XL_INPUT();

    #define CLEAR_OLD() \
        do { \
            if (grid[player_y][player_x] != G_BOMB) \
                grid[player_y][player_x] = G_EMPTY; \
        } while (0)

    if (_XL_LEFT(input)) {
        nx = player_x - 1; ny = player_y;
        if (grid[ny][nx] == G_EMPTY) {
            CLEAR_OLD();
            grid[ny][nx] = G_PLAYER;
            player_x = nx;
        }
    } else if (_XL_RIGHT(input)) {
        nx = player_x + 1; ny = player_y;
        if (grid[ny][nx] == G_EMPTY) {
            CLEAR_OLD();
            grid[ny][nx] = G_PLAYER;
            player_x = nx;
        }
    } else if (_XL_UP(input)) {
        nx = player_x; ny = player_y - 1;
        if (grid[ny][nx] == G_EMPTY) {
            CLEAR_OLD();
            grid[ny][nx] = G_PLAYER;
            player_y = ny;
        }
    } else if (_XL_DOWN(input)) {
        nx = player_x; ny = player_y + 1;
        if (grid[ny][nx] == G_EMPTY) {
            CLEAR_OLD();
            grid[ny][nx] = G_PLAYER;
            player_y = ny;
        }
    }

    #undef CLEAR_OLD

    /* Place bomb – find a free slot in the active list */
    if (_XL_FIRE(input)) {
        uint8_t slot;
        for (slot = 0; slot < MAX_BOMBS; slot++) {
            if (!bomb_active[slot]) {
                grid[player_y][player_x] = G_BOMB;
                bomb_x[slot] = player_x;
                bomb_y[slot] = player_y;
                bomb_age[slot] = 0;
                bomb_active[slot] = 1;
                break;
            }
        }
    }
}

/* ───────────────────────────────────────────── */
/*  Main                                         */
/* ───────────────────────────────────────────── */

int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    init_game();
    _XL_CLEAR_SCREEN();

    while (!game_over) {
        draw_game();
        process_bombs();
        handle_input();
        frame_count++;

        if (!(frame_count&7)) {
            move_enemies();
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    _XL_CLEAR_SCREEN();
    if (blocks_remaining == 0 && !game_over) {
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(2, 3, "YOU WIN!");
        _XL_EXPLOSION_SOUND();
    } else {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(2, 3, "GAME OVER");
        _XL_ZAP_SOUND();
    }

    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 10);

    return 0;
}
