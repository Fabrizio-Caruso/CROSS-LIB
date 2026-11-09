#include "cross_lib.h"

#define MAZE_W 20
#define MAZE_H 20
#define MAX_MONSTERS 20
#define MAX_LEVELS 20
#define MONSTER_MOVE_INTERVAL 4
#define ROOMS_X 10
#define ROOMS_Y 10

#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

void generate_maze(uint8_t maze[MAZE_H][MAZE_W], uint8_t level) {
    uint8_t i, j;
    uint8_t stack_x[100];
    uint8_t stack_y[100];
    uint8_t stack_top;
    uint8_t visited[100];
    uint8_t rx, ry;
    uint8_t nrx, nry;
    uint8_t dirs[4];
    uint8_t n;
    uint8_t d;
    uint8_t dir;
    uint8_t r;
    uint8_t ci, cj;
    uint8_t neighbors;
    uint8_t extra;

    for (i = 0; i < MAZE_H; i++) {
        for (j = 0; j < MAZE_W; j++) {
            maze[i][j] = 1;
        }
    }

    /* --- Recursive backtracking on 10x10 room grid --- */
    for (i = 0; i < 100; i++) {
        visited[i] = 0;
    }

    maze[0][0] = 0;
    visited[0] = 1;
    stack_x[0] = 0;
    stack_y[0] = 0;
    stack_top = 1;

    while (stack_top > 0) {
        rx = stack_x[stack_top - 1];
        ry = stack_y[stack_top - 1];

        n = 0;
        if (ry > 0 && visited[ry * ROOMS_X + rx] == 0) dirs[n++] = 0;
        if (ry < ROOMS_Y - 1 && visited[(ry + 1) * ROOMS_X + rx] == 0) dirs[n++] = 1;
        if (rx > 0 && visited[ry * ROOMS_X + rx - 1] == 0) dirs[n++] = 2;
        if (rx < ROOMS_X - 1 && visited[ry * ROOMS_X + rx + 1] == 0) dirs[n++] = 3;

        if (n == 0) {
            stack_top--;
        } else {
            d = _XL_RAND() % n;
            dir = dirs[d];

            if (dir == 0) {
                nry = ry - 1;
                nrx = rx;
                maze[ry * 2 - 1][rx * 2] = 0;
                maze[nry * 2][rx * 2] = 0;
            } else if (dir == 1) {
                nry = ry + 1;
                nrx = rx;
                maze[ry * 2 + 1][rx * 2] = 0;
                maze[nry * 2][rx * 2] = 0;
            } else if (dir == 2) {
                nry = ry;
                nrx = rx - 1;
                maze[ry * 2][rx * 2 - 1] = 0;
                maze[ry * 2][nrx * 2] = 0;
            } else {
                nry = ry;
                nrx = rx + 1;
                maze[ry * 2][rx * 2 + 1] = 0;
                maze[ry * 2][nrx * 2] = 0;
            }

            visited[nry * ROOMS_X + nrx] = 1;
            stack_x[stack_top] = nrx;
            stack_y[stack_top] = nry;
            stack_top++;
        }
    }

    /* --- Wall removal pass 1: open walls with >= 2 open neighbours --- */
    for (i = 1; i < MAZE_H - 1; i++) {
        for (j = 1; j < MAZE_W - 1; j++) {
            if (maze[i][j] == 1) {
                neighbors = 0;
                if (maze[i - 1][j] == 0) neighbors++;
                if (maze[i + 1][j] == 0) neighbors++;
                if (maze[i][j - 1] == 0) neighbors++;
                if (maze[i][j + 1] == 0) neighbors++;
                if (neighbors >= 2) {
                    maze[i][j] = 0;
                }
            }
        }
    }

    /* --- Wall removal pass 2: open walls with >= 3 open neighbours --- */
    for (i = 1; i < MAZE_H - 1; i++) {
        for (j = 1; j < MAZE_W - 1; j++) {
            if (maze[i][j] == 1) {
                neighbors = 0;
                if (maze[i - 1][j] == 0) neighbors++;
                if (maze[i + 1][j] == 0) neighbors++;
                if (maze[i][j - 1] == 0) neighbors++;
                if (maze[i][j + 1] == 0) neighbors++;
                if (neighbors >= 3) {
                    maze[i][j] = 0;
                }
            }
        }
    }

    /* --- Random 2x2 open rooms for variety --- */
    extra = 3 + level / 3;
    for (i = 0; i < extra; i++) {
        r = _XL_RAND() % (MAZE_H * MAZE_W);
        ci = r / MAZE_W;
        cj = r % MAZE_W;
        if (ci < MAZE_H - 2 && cj < MAZE_W - 2) {
            maze[ci][cj] = 0;
            maze[ci][cj + 1] = 0;
            maze[ci + 1][cj] = 0;
            maze[ci + 1][cj + 1] = 0;
        }
    }

    /* --- Guarantee exit (19,19) is reachable from last room (18,18) --- */
    maze[18][19] = 0;
    maze[19][18] = 0;
    maze[19][19] = 0;
    maze[0][0] = 0;
}

void flood_fill(uint8_t maze[MAZE_H][MAZE_W], uint8_t visited[MAZE_H][MAZE_W]) {
    uint8_t qx[400];
    uint8_t qy[400];
    uint16_t head, tail;
    uint8_t cx, cy;
    uint8_t i, j;

    for (i = 0; i < MAZE_H; i++) {
        for (j = 0; j < MAZE_W; j++) {
            visited[i][j] = 0;
        }
    }

    head = 0;
    tail = 0;
    qx[tail] = 0;
    qy[tail] = 0;
    tail++;
    visited[0][0] = 1;

    while (head < tail) {
        cx = qx[head];
        cy = qy[head];
        head++;

        if (cx < MAZE_W - 1 && maze[cy][cx + 1] == 0 && visited[cy][cx + 1] == 0) {
            visited[cy][cx + 1] = 1;
            qx[tail] = cx + 1;
            qy[tail] = cy;
            tail++;
        }
        if (cx > 0 && maze[cy][cx - 1] == 0 && visited[cy][cx - 1] == 0) {
            visited[cy][cx - 1] = 1;
            qx[tail] = cx - 1;
            qy[tail] = cy;
            tail++;
        }
        if (cy < MAZE_H - 1 && maze[cy + 1][cx] == 0 && visited[cy + 1][cx] == 0) {
            visited[cy + 1][cx] = 1;
            qx[tail] = cx;
            qy[tail] = cy + 1;
            tail++;
        }
        if (cy > 0 && maze[cy - 1][cx] == 0 && visited[cy - 1][cx] == 0) {
            visited[cy - 1][cx] = 1;
            qx[tail] = cx;
            qy[tail] = cy - 1;
            tail++;
        }
    }
}

void place_items(uint8_t visited[MAZE_H][MAZE_W], uint8_t *key_x, uint8_t *key_y,
                 uint8_t *torch_x, uint8_t *torch_y,
                 uint8_t *mon_x, uint8_t *mon_y, uint8_t num_monsters) {
    uint8_t i, j;
    uint8_t r;
    uint8_t attempts;
    uint8_t m;
    uint8_t found;

    found = 0;
    attempts = 0;
    while (!found && attempts < 100) {
        r = _XL_RAND() % (MAZE_H * MAZE_W);
        i = r / MAZE_W;
        j = r % MAZE_W;
        if (visited[i][j] == 1 && !(i == 0 && j == 0) && !(i == MAZE_H - 1 && j == MAZE_W - 1)) {
            *key_x = j;
            *key_y = i;
            found = 1;
        }
        attempts++;
    }
    if (!found) {
        *key_x = 1;
        *key_y = 1;
    }

    found = 0;
    attempts = 0;
    while (!found && attempts < 100) {
        r = _XL_RAND() % (MAZE_H * MAZE_W);
        i = r / MAZE_W;
        j = r % MAZE_W;
        if (visited[i][j] == 1 && !(i == 0 && j == 0) && !(i == MAZE_H - 1 && j == MAZE_W - 1)
            && !(i == *key_y && j == *key_x)) {
            *torch_x = j;
            *torch_y = i;
            found = 1;
        }
        attempts++;
    }
    if (!found) {
        *torch_x = (*key_x + 1) % MAZE_W;
        *torch_y = *key_y;
    }

    for (m = 0; m < num_monsters; m++) {
        found = 0;
        attempts = 0;
        while (!found && attempts < 100) {
            r = _XL_RAND() % (MAZE_H * MAZE_W);
            i = r / MAZE_W;
            j = r % MAZE_W;
            if (visited[i][j] == 1 && !(i == 0 && j == 0)
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
            mon_x[m] = (1 + m) % (MAZE_W - 2);
            mon_y[m] = (1 + m) % (MAZE_H - 2);
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
    uint8_t visited[MAZE_H][MAZE_W];
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
            flood_fill(maze, visited);

            px = 0;
            py = 0;
            has_key = 0;
            has_torch = 0;
            num_monsters = level;
            facing = DIR_RIGHT;
            mon_timer = 0;

            place_items(visited, &key_x, &key_y, &torch_x, &torch_y,
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