#include "cross_lib.h"

#define MAZE_W 20
#define MAZE_H 20
#define MAX_MONSTERS 20
#define MAX_LEVELS 20
#define MONSTER_MOVE_INTERVAL 4
#define ROOMS 10

#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

void generate_maze(uint8_t maze[MAZE_H][MAZE_W], uint8_t level) {
    uint8_t active[100];
    uint8_t active_count;
    uint8_t visited[100];
    uint8_t ri, rj;
    uint8_t nri, nrj;
    uint8_t dirs[4];
    uint8_t n, d;
    uint8_t i, j;
    uint8_t r;
    uint8_t loops;
    uint8_t li, lj;

    /* All walls */
    for (i = 0; i < MAZE_H; i++) {
        for (j = 0; j < MAZE_W; j++) {
            maze[i][j] = 1;
        }
    }

    /* Init visited for 10x10 room grid */
    for (i = 0; i < 100; i++) {
        visited[i] = 0;
    }

    /* Randomized Prim's: start from room (0,0) at cell (0,0) */
    maze[0][0] = 0;
    visited[0] = 1;
    active[0] = 0;
    active_count = 1;

    while (active_count > 0) {
        r = _XL_RAND() % active_count;
        ri = active[r] / ROOMS;
        rj = active[r] % ROOMS;

        /* Remove from active (swap with last) */
        active[r] = active[active_count - 1];
        active_count--;

        /* Collect unvisited neighbours */
        n = 0;
        if (ri > 0 && !visited[(ri - 1) * ROOMS + rj]) dirs[n++] = 0;
        if (ri < ROOMS - 1 && !visited[(ri + 1) * ROOMS + rj]) dirs[n++] = 1;
        if (rj > 0 && !visited[ri * ROOMS + (rj - 1)]) dirs[n++] = 2;
        if (rj < ROOMS - 1 && !visited[ri * ROOMS + (rj + 1)]) dirs[n++] = 3;

        if (n > 0) {
            d = _XL_RAND() % n;

            if (dirs[d] == 0) {
                nri = ri - 1;
                nrj = rj;
                maze[ri * 2 - 1][rj * 2] = 0;
                maze[nri * 2][rj * 2] = 0;
            } else if (dirs[d] == 1) {
                nri = ri + 1;
                nrj = rj;
                maze[ri * 2 + 1][rj * 2] = 0;
                maze[nri * 2][rj * 2] = 0;
            } else if (dirs[d] == 2) {
                nri = ri;
                nrj = rj - 1;
                maze[ri * 2][rj * 2 - 1] = 0;
                maze[ri * 2][nrj * 2] = 0;
            } else {
                nri = ri;
                nrj = rj + 1;
                maze[ri * 2][rj * 2 + 1] = 0;
                maze[ri * 2][nrj * 2] = 0;
            }

            visited[nri * ROOMS + nrj] = 1;
            active[active_count] = nri * ROOMS + nrj;
            active_count++;
        }
    }

    /* Add loops: open ~50% of remaining walls for many alternative routes */
    loops = 100 + level * 2;
    for (i = 0; i < loops; i++) {
        r = _XL_RAND() % (MAZE_H * MAZE_W);
        li = r / MAZE_W;
        lj = r % MAZE_W;
        if (maze[li][lj] == 1) {
            maze[li][lj] = 0;
        }
    }

    /* Guarantee exit (19,19) connects to last room (18,18) */
    maze[18][19] = 0;
    maze[19][18] = 0;
    maze[19][19] = 0;
    maze[0][0] = 0;
}

void place_items(uint8_t maze[MAZE_H][MAZE_W], uint8_t *key_x, uint8_t *key_y,
                 uint8_t *torch_x, uint8_t *torch_y,
                 uint8_t *mon_x, uint8_t *mon_y, uint8_t num_monsters) {
    uint8_t i, j;
    uint8_t r;
    uint8_t attempts;
    uint8_t m;
    uint8_t found;

    /* Key: any open cell that is not start or exit */
    found = 0;
    attempts = 0;
    while (!found && attempts < 100) {
        r = _XL_RAND() % (MAZE_H * MAZE_W);
        i = r / MAZE_W;
        j = r % MAZE_W;
        if (maze[i][j] == 0 && !(i == 0 && j == 0) && !(i == MAZE_H - 1 && j == MAZE_W - 1)) {
            *key_x = j;
            *key_y = i;
            found = 1;
        }
        attempts++;
    }
    if (!found) {
        *key_x = 2;
        *key_y = 2;
    }

    /* Torch: any open cell, not start/exit/key */
    found = 0;
    attempts = 0;
    while (!found && attempts < 100) {
        r = _XL_RAND() % (MAZE_H * MAZE_W);
        i = r / MAZE_W;
        j = r % MAZE_W;
        if (maze[i][j] == 0 && !(i == 0 && j == 0) && !(i == MAZE_H - 1 && j == MAZE_W - 1)
            && !(i == *key_y && j == *key_x)) {
            *torch_x = j;
            *torch_y = i;
            found = 1;
        }
        attempts++;
    }
    if (!found) {
        *torch_x = (*key_x + 2) % MAZE_W;
        *torch_y = (*key_y + 2) % MAZE_H;
    }

    /* Monsters: any open cell, not start/exit/key/torch */
    for (m = 0; m < num_monsters; m++) {
        found = 0;
        attempts = 0;
        while (!found && attempts < 100) {
            r = _XL_RAND() % (MAZE_H * MAZE_W);
            i = r / MAZE_W;
            j = r % MAZE_W;
            if (maze[i][j] == 0 && !(i == 0 && j == 0)
                && !(i == MAZE_H - 1 && j == MAZE_W - 1)
                && !(i == *key_y && j == *key_x)
                && !(i == *torch_y && j == *torch_x)) {
                mon_x[m] = j;
                mon_y[m] = i;
                found = 1;
            }
            attempts++;
        }
        if (!found) {
            mon_x[m] = (2 + m) % (MAZE_W - 2);
            mon_y[m] = (2 + m) % (MAZE_H - 2);
        }
    }
}

void move_monsters(uint8_t maze[MAZE_H][MAZE_W], uint8_t *mon_x, uint8_t *mon_y,
                   uint8_t num_monsters, uint8_t px, uint8_t py) {
    uint8_t m;

    for (m = 0; m < num_monsters; m++) {
        if (px > mon_x[m] && mon_x[m] < MAZE_W - 1 && maze[mon_y[m]][mon_x[m] + 1] == 0) {
            mon_x[m]++;
        } else if (px < mon_x[m] && mon_x[m] > 0 && maze[mon_y[m]][mon_x[m] - 1] == 0) {
            mon_x[m]--;
        } else if (py > mon_y[m] && mon_y[m] < MAZE_H - 1 && maze[mon_y[m] + 1][mon_x[m]] == 0) {
            mon_y[m]++;
        } else if (py < mon_y[m] && mon_y[m] > 0 && maze[mon_y[m] - 1][mon_x[m]] == 0) {
            mon_y[m]--;
        }
    }
}

uint8_t shoot(uint8_t maze[MAZE_H][MAZE_W], uint8_t px, uint8_t py, uint8_t dir,
              uint8_t *mon_x, uint8_t *mon_y, uint8_t *num_monsters) {
    uint8_t x, y;
    uint8_t m;
    uint8_t hit;

    x = px;
    y = py;

    while (1) {
        if (dir == DIR_UP) {
            if (y == 0) break;
            y--;
        } else if (dir == DIR_DOWN) {
            if (y == MAZE_H - 1) break;
            y++;
        } else if (dir == DIR_LEFT) {
            if (x == 0) break;
            x--;
        } else {
            if (x == MAZE_W - 1) break;
            x++;
        }

        if (maze[y][x] == 1) break;

        hit = 0;
        for (m = 0; m < *num_monsters; m++) {
            if (mon_x[m] == x && mon_y[m] == y) {
                mon_x[m] = mon_x[*num_monsters - 1];
                mon_y[m] = mon_y[*num_monsters - 1];
                (*num_monsters)--;
                hit = 1;
                break;
            }
        }
        if (hit) break;
    }

    return hit;
}

void draw_screen(uint8_t maze[MAZE_H][MAZE_W], uint8_t px, uint8_t py,
                 uint8_t key_x, uint8_t key_y, uint8_t torch_x, uint8_t torch_y,
                 uint8_t *mon_x, uint8_t *mon_y, uint8_t num_monsters,
                 uint8_t has_torch, uint8_t has_key,
                 uint8_t level, uint8_t lives, uint16_t score) {
    uint8_t i, j;
    uint8_t radius;
    uint8_t dx, dy;
    uint8_t dist;
    uint8_t tile;
    uint8_t color;
    uint8_t m;

    radius = has_torch ? 8 : 4;

    _XL_CLEAR_SCREEN();

    for (i = 0; i < MAZE_H; i++) {
        for (j = 0; j < MAZE_W; j++) {
            dx = (px > j) ? (px - j) : (j - px);
            dy = (py > i) ? (py - i) : (i - py);
            dist = dx + dy;

            if (dist > radius) continue;

            if (maze[i][j] == 0) {
                tile = _TILE_1;
                color = _XL_CYAN;
            } else {
                tile = _TILE_0;
                color = _XL_BLUE;
            }

            if (i == key_y && j == key_x && !has_key) {
                tile = _TILE_3;
                color = _XL_YELLOW;
            }
            if (i == torch_y && j == torch_x && !has_torch) {
                tile = _TILE_7;
                color = _XL_YELLOW;
            }
            if (i == MAZE_H - 1 && j == MAZE_W - 1) {
                if (has_key) {
                    tile = _TILE_5;
                    color = _XL_GREEN;
                } else {
                    tile = _TILE_4;
                    color = _XL_RED;
                }
            }

            for (m = 0; m < num_monsters; m++) {
                if (mon_x[m] == j && mon_y[m] == i) {
                    tile = _TILE_6;
                    color = _XL_RED;
                    break;
                }
            }

            _XL_DRAW(j, i, tile, color);
        }
    }

    _XL_DRAW(px, py, _TILE_2, _XL_GREEN);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "L");
    _XL_PRINTD(1, 0, 1, level);
    _XL_PRINT(3, 0, "S");
    _XL_PRINTD(4, 0, 1, score);
    _XL_PRINT(8, 0, "L");
    _XL_PRINTD(9, 0, 1, lives);
}

int main(void) {
    uint8_t maze[MAZE_H][MAZE_W];
    uint8_t px, py;
    uint8_t key_x, key_y;
    uint8_t torch_x, torch_y;
    uint8_t mon_x[MAX_MONSTERS];
    uint8_t mon_y[MAX_MONSTERS];
    uint8_t num_monsters;
    uint8_t has_key;
    uint8_t has_torch;
    uint8_t level;
    uint8_t lives;
    uint16_t score;
    uint8_t input;
    uint8_t facing;
    uint8_t hit;
    uint8_t m;
    uint8_t player_dead;
    uint8_t level_done;
    uint8_t mon_timer;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        lives = 3;
        score = 0;
        level = 1;

        while (level <= MAX_LEVELS) {
            generate_maze(maze, level);

            px = 0;
            py = 0;
            has_key = 0;
            has_torch = 0;
            num_monsters = level;
            facing = DIR_RIGHT;
            mon_timer = 0;

            place_items(maze, &key_x, &key_y, &torch_x, &torch_y,
                        mon_x, mon_y, num_monsters);

            _XL_CLEAR_SCREEN();
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(5, 5, "LEVEL");
            _XL_PRINTD(10, 5, 1, level);
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 5);

            player_dead = 0;
            level_done = 0;

            while (!player_dead && !level_done) {
                draw_screen(maze, px, py, key_x, key_y, torch_x, torch_y,
                            mon_x, mon_y, num_monsters,
                            has_torch, has_key, level, lives, score);

                input = _XL_INPUT();

                if (_XL_UP(input)) {
                    facing = DIR_UP;
                    if (py > 0 && maze[py - 1][px] == 0) {
                        py--;
                        _XL_TICK_SOUND();
                    }
                } else if (_XL_DOWN(input)) {
                    facing = DIR_DOWN;
                    if (py < MAZE_H - 1 && maze[py + 1][px] == 0) {
                        py++;
                        _XL_TICK_SOUND();
                    }
                } else if (_XL_LEFT(input)) {
                    facing = DIR_LEFT;
                    if (px > 0 && maze[py][px - 1] == 0) {
                        px--;
                        _XL_TICK_SOUND();
                    }
                } else if (_XL_RIGHT(input)) {
                    facing = DIR_RIGHT;
                    if (px < MAZE_W - 1 && maze[py][px + 1] == 0) {
                        px++;
                        _XL_TICK_SOUND();
                    }
                }

                if (_XL_FIRE(input)) {
                    hit = shoot(maze, px, py, facing, mon_x, mon_y, &num_monsters);
                    _XL_SHOOT_SOUND();
                    if (hit) {
                        score += 10;
                    }
                }

                if (px == key_x && py == key_y && !has_key) {
                    has_key = 1;
                    _XL_PING_SOUND();
                    score += 50;
                }

                if (px == torch_x && py == torch_y && !has_torch) {
                    has_torch = 1;
                    _XL_PING_SOUND();
                    score += 50;
                }

                if (px == MAZE_W - 1 && py == MAZE_H - 1 && has_key) {
                    level_done = 1;
                    _XL_ZAP_SOUND();
                    score += 100;
                }

                mon_timer++;
                if (mon_timer >= MONSTER_MOVE_INTERVAL) {
                    mon_timer = 0;
                    move_monsters(maze, mon_x, mon_y, num_monsters, px, py);
                }

                for (m = 0; m < num_monsters; m++) {
                    if (mon_x[m] == px && mon_y[m] == py) {
                        player_dead = 1;
                        _XL_EXPLOSION_SOUND();
                        break;
                    }
                }

                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
            }

            if (player_dead) {
                lives--;
                if (lives == 0) {
                    _XL_CLEAR_SCREEN();
                    _XL_SET_TEXT_COLOR(_XL_RED);
                    _XL_PRINT(2, 5, "GAME OVER");
                    _XL_SET_TEXT_COLOR(_XL_WHITE);
                    _XL_PRINT(2, 7, "SCORE");
                    _XL_PRINTD(7, 7, 1, score);
                    _XL_PRINT(2, 9, "PRESS ANY KEY");
                    _XL_WAIT_FOR_INPUT();
                    break;
                }
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_PRINT(2, 5, "HIT");
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 3);
            } else {
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(2, 5, "LEVEL DONE");
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 2);
                level++;
            }
        }

        _XL_CLEAR_SCREEN();
        if (level > MAX_LEVELS) {
            _XL_SET_TEXT_COLOR(_XL_GREEN);
            _XL_PRINT(2, 5, "YOU WIN");
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(2, 7, "SCORE");
            _XL_PRINTD(7, 7, 1, score);
        } else {
            _XL_SET_TEXT_COLOR(_XL_RED);
            _XL_PRINT(2, 5, "GAME OVER");
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINT(2, 7, "SCORE");
            _XL_PRINTD(7, 7, 1, score);
        }
        _XL_PRINT(2, 9, "PRESS ANY KEY");
        _XL_WAIT_FOR_INPUT();
    }

    return 0;
}