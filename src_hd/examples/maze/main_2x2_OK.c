#include "cross_lib.h"

#define MAZE_SIZE 20
#define NUM_LEVELS 20
#define NUM_GEMS 10
#define NUM_TORCHES 1
#define NUM_ITEMS (NUM_GEMS + NUM_TORCHES)
#define MONSTER_MOVE_INTERVAL 6
#define GEM_POINTS 25
#define START_LIVES 3
#define TILE_SCALE 2

// Tiles
#define T_FLOOR   _TILE_0
#define T_WALL    _TILE_1
#define T_PLAYER  _TILE_2
#define T_MONSTER _TILE_3
#define T_EXIT    _TILE_4
#define T_BULLET  _TILE_5
#define T_DARK    _TILE_6
#define T_GEM     _TILE_7
#define T_TORCH   _TILE_8

// Colors
#define C_FLOOR   _XL_BLUE
#define C_WALL    _XL_YELLOW
#define C_PLAYER  _XL_CYAN
#define C_MONSTER _XL_RED
#define C_EXIT    _XL_GREEN
#define C_BULLET  _XL_MAGENTA
#define C_DARK    _XL_BLUE
#define C_GEM     _XL_YELLOW
#define C_TORCH   _XL_RED

// Item types
#define ITEM_GEM   0
#define ITEM_TORCH 1

// Globals
uint8_t maze[MAZE_SIZE][MAZE_SIZE];
uint8_t reachable[MAZE_SIZE][MAZE_SIZE];
uint8_t player_x, player_y;
uint8_t player_dir;
uint8_t exit_x, exit_y;
uint8_t monster_x[MAZE_SIZE];
uint8_t monster_y[MAZE_SIZE];
uint8_t num_monsters;
uint8_t bullet_x, bullet_y;
uint8_t bullet_dir;
uint8_t bullet_active;
uint8_t level;
uint16_t score;
uint8_t game_over;
uint8_t level_complete;
uint8_t player_dead;
uint16_t monster_timer;
uint8_t lives;

// Item storage
uint8_t item_x[NUM_ITEMS];
uint8_t item_y[NUM_ITEMS];
uint8_t item_type[NUM_ITEMS];
uint8_t item_active[NUM_ITEMS];
uint8_t view_dist;

const int8_t dx[4] = {0, 1, 0, -1};
const int8_t dy[4] = {-1, 0, 1, 0};

// --- Draw a 2x2 block at screen position (sx, sy) ---
void draw_block(uint8_t sx, uint8_t sy, uint8_t tile, uint8_t color) {
    if (sx < XSize && sy < YSize)
        _XL_DRAW(sx, sy, tile, color);
    if (sx + 1 < XSize && sy < YSize)
        _XL_DRAW(sx + 1, sy, tile, color);
    if (sx < XSize && sy + 1 < YSize)
        _XL_DRAW(sx, sy + 1, tile, color);
    if (sx + 1 < XSize && sy + 1 < YSize)
        _XL_DRAW(sx + 1, sy + 1, tile, color);
}

// --- Pick a random exit corner ---
void pick_exit(void) {
    uint8_t r = _XL_RAND() % 3;
    if (r == 0) {
        exit_x = MAZE_SIZE - 1;
        exit_y = 0;
    } else if (r == 1) {
        exit_x = 0;
        exit_y = MAZE_SIZE - 1;
    } else {
        exit_x = MAZE_SIZE - 1;
        exit_y = MAZE_SIZE - 1;
    }
}

// --- Maze Generation ---
void generate_maze(void) {
    uint8_t i, j;
    uint8_t cx, cy;
    uint8_t path_x[40];
    uint8_t path_y[40];
    uint8_t path_len;
    uint8_t step;

    for (i = 0; i < MAZE_SIZE; i++)
        for (j = 0; j < MAZE_SIZE; j++)
            maze[i][j] = 1;

    cx = 0;
    cy = 0;
    path_len = 1;
    path_x[0] = 0;
    path_y[0] = 0;

    while (cx != exit_x || cy != exit_y) {
        if (cx != exit_x && cy != exit_y) {
            if ((_XL_RAND() % 2) == 0)
                cx += (exit_x > cx) ? 1 : -1;
            else
                cy += (exit_y > cy) ? 1 : -1;
        } else if (cx != exit_x) {
            cx += (exit_x > cx) ? 1 : -1;
        } else {
            cy += (exit_y > cy) ? 1 : -1;
        }
        path_x[path_len] = cx;
        path_y[path_len] = cy;
        path_len++;
    }

    for (step = 0; step < path_len; step++)
        maze[path_x[step]][path_y[step]] = 0;

    for (i = 0; i < MAZE_SIZE; i++)
        for (j = 0; j < MAZE_SIZE; j++)
            if (maze[i][j] == 1)
                if ((_XL_RAND() % 100) < 25)
                    maze[i][j] = 0;

    maze[0][0] = 0;
    maze[exit_x][exit_y] = 0;
}

// --- BFS flood fill from (0,0) ---
void flood_fill(void) {
    uint8_t queue_x[400];
    uint8_t queue_y[400];
    uint16_t head, tail;
    uint8_t i, j, d;
    uint8_t cx, cy, nx, ny;

    for (i = 0; i < MAZE_SIZE; i++)
        for (j = 0; j < MAZE_SIZE; j++)
            reachable[i][j] = 0;

    reachable[0][0] = 1;
    head = 0;
    tail = 0;
    queue_x[tail] = 0;
    queue_y[tail] = 0;
    tail++;

    while (head < tail) {
        cx = queue_x[head];
        cy = queue_y[head];
        head++;

        for (d = 0; d < 4; d++) {
            nx = cx + dx[d];
            ny = cy + dy[d];
            if (nx >= MAZE_SIZE || ny >= MAZE_SIZE) continue;
            if (reachable[nx][ny]) continue;
            if (maze[nx][ny] == 1) continue;
            reachable[nx][ny] = 1;
            queue_x[tail] = nx;
            queue_y[tail] = ny;
            tail++;
        }
    }
}

// --- Place Items ---
void place_items(void) {
    uint8_t i;
    uint8_t ix, iy;
    uint8_t valid;
    uint16_t tries;

    for (i = 0; i < NUM_ITEMS; i++) {
        item_type[i] = (i < NUM_GEMS) ? ITEM_GEM : ITEM_TORCH;
        item_active[i] = 1;

        tries = 0;
        do {
            ix = (_XL_RAND() % MAZE_SIZE);
            iy = (_XL_RAND() % MAZE_SIZE);
            valid = 1;

            if (!reachable[ix][iy]) valid = 0;
            if (maze[ix][iy] == 1) valid = 0;
            if (ix == 0 && iy == 0) valid = 0;
            if (ix == exit_x && iy == exit_y) valid = 0;
            if (ix < 3 && iy < 3) valid = 0;

            for (uint8_t k = 0; k < i; k++) {
                if (item_active[k] && item_x[k] == ix && item_y[k] == iy)
                    valid = 0;
            }

            tries++;
        } while (!valid && tries < 500);

        item_x[i] = ix;
        item_y[i] = iy;
    }
}

// --- Place Monsters ---
void place_monsters(void) {
    uint8_t i;
    uint8_t mx, my;
    uint8_t valid;
    uint16_t tries;

    num_monsters = level;
    if (num_monsters > MAZE_SIZE) num_monsters = MAZE_SIZE;

    for (i = 0; i < num_monsters; i++) {
        tries = 0;
        do {
            mx = (_XL_RAND() % MAZE_SIZE);
            my = (_XL_RAND() % MAZE_SIZE);
            valid = 1;

            if (!reachable[mx][my]) valid = 0;
            if (maze[mx][my] == 1) valid = 0;
            if (mx == 0 && my == 0) valid = 0;
            if (mx == exit_x && my == exit_y) valid = 0;
            if (mx < 3 && my < 3) valid = 0;

            for (uint8_t k = 0; k < i; k++) {
                if (mx == monster_x[k] && my == monster_y[k]) valid = 0;
            }

            tries++;
        } while (!valid && tries < 500);

        monster_x[i] = mx;
        monster_y[i] = my;
    }
}

// --- Respawn player at start ---
void respawn_player(void) {
    player_x = 0;
    player_y = 0;
    player_dir = 1;
    bullet_active = 0;
    monster_timer = 0;
    view_dist = 4;
    player_dead = 0;
    place_monsters();
}

// --- Draw Screen (2x2 blocks per cell) ---
void draw_screen(void) {
    uint8_t i, j;
    uint8_t view_min_x, view_max_x;
    uint8_t view_min_y, view_max_y;
    uint16_t screen_off_x, screen_off_y;
    uint16_t sx, sy;
    uint8_t ddx;
    uint8_t tile, color;

    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "LEVEL ");
    _XL_PRINTD(6, 0, 1, level);
    _XL_PRINT(10, 0, "SCORE ");
    _XL_PRINTD(16, 0, 1, score);
    _XL_PRINT(0, 1, "LIVES ");
    _XL_PRINTD(6, 1, 1, lives);

    // Screen offset: center minus view_dist * TILE_SCALE
    screen_off_x = XSize / 2 - view_dist * TILE_SCALE;
    screen_off_y = YSize / 2 - view_dist * TILE_SCALE;

    view_min_x = (player_x > view_dist) ? (player_x - view_dist) : 0;
    view_max_x = (player_x + view_dist < MAZE_SIZE - 1) ? (player_x + view_dist) : (MAZE_SIZE - 1);
    view_min_y = (player_y > view_dist) ? (player_y - view_dist) : 0;
    view_max_y = (player_y + view_dist < MAZE_SIZE - 1) ? (player_y + view_dist) : (MAZE_SIZE - 1);

    for (i = view_min_x; i <= view_max_x; i++) {
        for (j = view_min_y; j <= view_max_y; j++) {
            // Screen position (top-left of the 2x2 block)
            sx = screen_off_x + (uint16_t)(i - view_min_x) * TILE_SCALE;
            sy = screen_off_y + (uint16_t)(j - view_min_y) * TILE_SCALE;

            // Chebyshev distance
            if (i > player_x) ddx = i - player_x; else ddx = player_x - i;
            if (j > player_y) { if (j - player_y > ddx) ddx = j - player_y; }
            else { if (player_y - j > ddx) ddx = player_y - j; }

            if (ddx > view_dist) {
                draw_block(sx, sy, T_DARK, C_DARK);
                continue;
            }

            tile = T_FLOOR;
            color = C_FLOOR;

            if (maze[i][j] == 1) {
                tile = T_WALL;
                color = C_WALL;
            }

            if (i == exit_x && j == exit_y) {
                tile = T_EXIT;
                color = C_EXIT;
            }

            for (uint8_t m = 0; m < NUM_ITEMS; m++) {
                if (item_active[m] && item_x[m] == i && item_y[m] == j) {
                    if (item_type[m] == ITEM_GEM) {
                        tile = T_GEM;
                        color = C_GEM;
                    } else {
                        tile = T_TORCH;
                        color = C_TORCH;
                    }
                }
            }

            for (uint8_t m = 0; m < num_monsters; m++) {
                if (monster_x[m] == i && monster_y[m] == j) {
                    tile = T_MONSTER;
                    color = C_MONSTER;
                }
            }

            if (bullet_active && bullet_x == i && bullet_y == j) {
                tile = T_BULLET;
                color = C_BULLET;
            }

            if (i == player_x && j == player_y) {
                tile = T_PLAYER;
                color = C_PLAYER;
            }

            draw_block(sx, sy, tile, color);
        }
    }
}

// --- Move Player ---
void move_player(uint8_t input) {
    uint8_t nx, ny;

    if (_XL_UP(input)) {
        player_dir = 0;
        nx = player_x;
        ny = (player_y > 0) ? (player_y - 1) : player_y;
        if (maze[nx][ny] == 0) {
            player_x = nx;
            player_y = ny;
            _XL_TICK_SOUND();
        }
    } else if (_XL_RIGHT(input)) {
        player_dir = 1;
        nx = (player_x < MAZE_SIZE - 1) ? (player_x + 1) : player_x;
        ny = player_y;
        if (maze[nx][ny] == 0) {
            player_x = nx;
            player_y = ny;
            _XL_TICK_SOUND();
        }
    } else if (_XL_DOWN(input)) {
        player_dir = 2;
        nx = player_x;
        ny = (player_y < MAZE_SIZE - 1) ? (player_y + 1) : player_y;
        if (maze[nx][ny] == 0) {
            player_x = nx;
            player_y = ny;
            _XL_TICK_SOUND();
        }
    } else if (_XL_LEFT(input)) {
        player_dir = 3;
        nx = (player_x > 0) ? (player_x - 1) : player_x;
        ny = player_y;
        if (maze[nx][ny] == 0) {
            player_x = nx;
            player_y = ny;
            _XL_TICK_SOUND();
        }
    }
}

// --- Pick up items ---
void collect_items(void) {
    uint8_t i;

    for (i = 0; i < NUM_ITEMS; i++) {
        if (!item_active[i]) continue;
        if (item_x[i] == player_x && item_y[i] == player_y) {
            item_active[i] = 0;
            if (item_type[i] == ITEM_GEM) {
                score += GEM_POINTS;
                _XL_PING_SOUND();
            } else {
                view_dist = 8;
                _XL_PING_SOUND();
            }
        }
    }
}

// --- Shoot ---
void shoot(void) {
    bullet_x = player_x;
    bullet_y = player_y;
    bullet_dir = player_dir;
    bullet_active = 1;
    _XL_SHOOT_SOUND();
}

// --- Move Bullet ---
void move_bullet(void) {
    uint8_t nx, ny;
    uint8_t hit_monster;

    if (!bullet_active) return;

    nx = bullet_x + dx[bullet_dir];
    ny = bullet_y + dy[bullet_dir];

    if (nx >= MAZE_SIZE || ny >= MAZE_SIZE) {
        bullet_active = 0;
        return;
    }

    if (maze[nx][ny] == 1) {
        bullet_active = 0;
        return;
    }

    hit_monster = 0;
    for (uint8_t m = 0; m < num_monsters; m++) {
        if (monster_x[m] == nx && monster_y[m] == ny) {
            monster_x[m] = monster_x[num_monsters - 1];
            monster_y[m] = monster_y[num_monsters - 1];
            num_monsters--;
            score += 10;
            hit_monster = 1;
            _XL_EXPLOSION_SOUND();
            break;
        }
    }

    if (hit_monster) {
        bullet_active = 0;
        return;
    }

    bullet_x = nx;
    bullet_y = ny;
}

// --- Move Monsters ---
void move_monsters(void) {
    uint8_t i;
    uint8_t nx, ny;
    uint8_t best_dir;
    uint8_t best_dist;
    uint8_t cur_dist;
    uint8_t d;

    for (i = 0; i < num_monsters; i++) {
        best_dist = 255;
        best_dir = 0;

        for (d = 0; d < 4; d++) {
            nx = monster_x[i] + dx[d];
            ny = monster_y[i] + dy[d];

            if (nx >= MAZE_SIZE || ny >= MAZE_SIZE) continue;
            if (maze[nx][ny] == 1) continue;

            uint8_t blocked = 0;
            for (uint8_t k = 0; k < num_monsters; k++) {
                if (k != i && monster_x[k] == nx && monster_y[k] == ny) {
                    blocked = 1;
                    break;
                }
            }
            if (blocked) continue;

            if (player_x > nx) cur_dist = player_x - nx; else cur_dist = nx - player_x;
            if (player_y > ny) { if (player_y - ny > cur_dist) cur_dist = player_y - ny; }
            else { if (ny > player_y) { if (ny - player_y > cur_dist) cur_dist = ny - player_y; } }

            if (cur_dist < best_dist) {
                best_dist = cur_dist;
                best_dir = d;
            }
        }

        if (best_dist < 255) {
            monster_x[i] = monster_x[i] + dx[best_dir];
            monster_y[i] = monster_y[i] + dy[best_dir];
        }
    }
}

// --- Check Collisions ---
void check_collisions(void) {
    uint8_t i;

    for (i = 0; i < num_monsters; i++) {
        if (monster_x[i] == player_x && monster_y[i] == player_y) {
            player_dead = 1;
            _XL_ZAP_SOUND();
            return;
        }
    }

    if (player_x == exit_x && player_y == exit_y) {
        level_complete = 1;
        _XL_PING_SOUND();
    }
}

// --- Next Level ---
void next_level(void) {
    level++;
    if (level > NUM_LEVELS) {
        game_over = 1;
        return;
    }

    level_complete = 0;
    player_dead = 0;
    bullet_active = 0;
    monster_timer = 0;
    view_dist = 4;

    player_x = 0;
    player_y = 0;
    player_dir = 1;

    pick_exit();
    generate_maze();
    flood_fill();
    place_items();
    place_monsters();
}

// --- End Screen ---
void show_end_screen(void) {
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_WHITE);

    if (level > NUM_LEVELS) {
        _XL_PRINT(XSize / 4, YSize / 4, "YOU WIN!");
        _XL_PRINT(XSize / 4, YSize / 4 + 2, "FINAL SCORE ");
        _XL_PRINTD(XSize / 4 + 12, YSize / 4 + 2, 1, score);
    } else {
        _XL_PRINT(XSize / 4, YSize / 4, "GAME OVER");
        _XL_PRINT(XSize / 4, YSize / 4 + 2, "SCORE ");
        _XL_PRINTD(XSize / 4 + 6, YSize / 4 + 2, 1, score);
    }

    _XL_PRINT(XSize / 4, YSize / 2 + 2, "PRESS ANY KEY TO RESTART");
    _XL_WAIT_FOR_INPUT();
}

// --- Reset all game state ---
void reset_game(void) {
    level = 1;
    score = 0;
    game_over = 0;
    level_complete = 0;
    player_dead = 0;
    monster_timer = 0;
    bullet_active = 0;
    view_dist = 4;
    lives = START_LIVES;

    player_x = 0;
    player_y = 0;
    player_dir = 1;

    pick_exit();
    generate_maze();
    flood_fill();
    place_items();
    place_monsters();
}

// --- Main ---
int main(void) {
    uint8_t input;
    uint8_t fire_pressed;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        reset_game();

        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(XSize / 4, YSize / 4, "MAZE RUNNER");
        _XL_PRINT(XSize / 4, YSize / 4 + 2, "20 LEVELS");
        _XL_PRINT(XSize / 4, YSize / 4 + 4, "3 LIVES");
        _XL_PRINT(XSize / 4, YSize / 4 + 6, "FIND THE EXIT");
        _XL_PRINT(XSize / 4, YSize / 4 + 8, "AVOID MONSTERS");
        _XL_PRINT(XSize / 4, YSize / 4 + 10, "GRAB GEMS AND TORCH");
        _XL_PRINT(XSize / 4, YSize / 4 + 12, "PRESS ANY KEY");
        _XL_WAIT_FOR_INPUT();

        while (!game_over) {
            draw_screen();

            input = _XL_INPUT();

            move_player(input);
            collect_items();

            fire_pressed = _XL_FIRE(input);
            if (fire_pressed && !bullet_active) {
                shoot();
            }

            move_bullet();

            monster_timer++;
            if (monster_timer >= MONSTER_MOVE_INTERVAL) {
                monster_timer = 0;
                move_monsters();
            }

            check_collisions();

            if (player_dead) {
                lives--;
                if (lives > 0) {
                    respawn_player();
                    _XL_SLEEP(1);
                } else {
                    game_over = 1;
                    show_end_screen();
                    break;
                }
            }

            if (level_complete) {
                score += 50;
                _XL_SLEEP(1);
                next_level();
                if (game_over) {
                    show_end_screen();
                    break;
                }
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }

    return 0;
