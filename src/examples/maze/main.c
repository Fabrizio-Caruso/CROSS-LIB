#include "cross_lib.h"

#define MAZE_SIZE 20
#define VIEW_DIST 4
#define NUM_LEVELS 20
#define MONSTER_MOVE_INTERVAL 6

// Tiles
#define T_FLOOR   _TILE_0
#define T_WALL    _TILE_1
#define T_PLAYER  _TILE_2
#define T_MONSTER _TILE_3
#define T_EXIT    _TILE_4
#define T_BULLET  _TILE_5
#define T_DARK    _TILE_6

// Colors
#define C_FLOOR   _XL_BLUE
#define C_WALL    _XL_YELLOW
#define C_PLAYER  _XL_CYAN
#define C_MONSTER _XL_RED
#define C_EXIT    _XL_GREEN
#define C_BULLET  _XL_MAGENTA
#define C_DARK    _XL_BLUE
#define C_TEXT    _XL_WHITE

// Globals
uint8_t maze[MAZE_SIZE][MAZE_SIZE];
uint8_t player_x, player_y;
uint8_t player_dir; // 0=up 1=right 2=down 3=left
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
uint8_t frame_count;

const int8_t dx[4] = {0, 1, 0, -1};
const int8_t dy[4] = {-1, 0, 1, 0};

// --- Maze Generation (guaranteed solvable) ---
void generate_maze(void) {
    uint8_t i, j;
    uint8_t cx, cy;
    uint8_t path_x[40];
    uint8_t path_y[40];
    uint8_t path_len;
    uint8_t step;

    // Fill all with walls
    for (i = 0; i < MAZE_SIZE; i++)
        for (j = 0; j < MAZE_SIZE; j++)
            maze[i][j] = 1;

    // Generate random monotonic path from (0,0) to (19,19)
    // Only moves right (+x) or down (+y)
    cx = 0;
    cy = 0;
    path_len = 0;
    path_x[0] = 0;
    path_y[0] = 0;
    path_len = 1;

    while (cx < MAZE_SIZE - 1 || cy < MAZE_SIZE - 1) {
        if (cx < MAZE_SIZE - 1 && cy < MAZE_SIZE - 1) {
            // Choose randomly between right and down
            if ((_XL_RAND() % 2) == 0)
                cx++;
            else
                cy++;
        } else if (cx < MAZE_SIZE - 1) {
            cx++;
        } else {
            cy++;
        }
        path_x[path_len] = cx;
        path_y[path_len] = cy;
        path_len++;
    }

    // Mark path as floor
    for (step = 0; step < path_len; step++) {
        maze[path_x[step]][path_y[step]] = 0;
    }

    // Add extra random floor cells for variety (25% chance)
    for (i = 0; i < MAZE_SIZE; i++) {
        for (j = 0; j < MAZE_SIZE; j++) {
            if (maze[i][j] == 1) {
                if ((_XL_RAND() % 100) < 25) {
                    maze[i][j] = 0;
                }
            }
        }
    }

    // Ensure start and exit are floor
    maze[0][0] = 0;
    maze[MAZE_SIZE - 1][MAZE_SIZE - 1] = 0;
}

// --- Place Monsters ---
void place_monsters(void) {
    uint8_t i;
    uint8_t mx, my;
    uint8_t valid;

    num_monsters = level;
    if (num_monsters > MAZE_SIZE) num_monsters = MAZE_SIZE;

    for (i = 0; i < num_monsters; i++) {
        do {
            mx = (_XL_RAND() % MAZE_SIZE);
            my = (_XL_RAND() % MAZE_SIZE);
            valid = 1;
            // Not on wall
            if (maze[mx][my] == 1) valid = 0;
            // Not on player start
            if (mx == 0 && my == 0) valid = 0;
            // Not on exit
            if (mx == MAZE_SIZE - 1 && my == MAZE_SIZE - 1) valid = 0;
            // Not on another monster
            for (uint8_t k = 0; k < i; k++) {
                if (mx == monster_x[k] && my == monster_y[k]) valid = 0;
            }
            // Not too close to start (at least 3 tiles away)
            if (mx < 3 && my < 3) valid = 0;
        } while (!valid);

        monster_x[i] = mx;
        monster_y[i] = my;
    }
}

// --- Draw Screen ---
void draw_screen(void) {
    uint8_t i, j;
    uint8_t view_min_x, view_max_x;
    uint8_t view_min_y, view_max_y;
    uint8_t screen_off_x, screen_off_y;
    uint8_t sx, sy;
    uint8_t dist;
    uint8_t tile, color;

    _XL_CLEAR_SCREEN();

    // HUD
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "LEVEL ");
    _XL_PRINTD(6, 0, 1, level);
    _XL_PRINT(10, 0, "SCORE ");
    _XL_PRINTD(16, 0, 1, score);

    screen_off_x = XSize / 2 - VIEW_DIST;
    screen_off_y = YSize / 2 - VIEW_DIST;

    view_min_x = (player_x > VIEW_DIST) ? (player_x - VIEW_DIST) : 0;
    view_max_x = (player_x + VIEW_DIST < MAZE_SIZE - 1) ? (player_x + VIEW_DIST) : (MAZE_SIZE - 1);
    view_min_y = (player_y > VIEW_DIST) ? (player_y - VIEW_DIST) : 0;
    view_max_y = (player_y + VIEW_DIST < MAZE_SIZE - 1) ? (player_y + VIEW_DIST) : (MAZE_SIZE - 1);

    for (i = view_min_x; i <= view_max_x; i++) {
        for (j = view_min_y; j <= view_max_y; j++) {
            sx = screen_off_x + (i - view_min_x);
            sy = screen_off_y + (j - view_min_y);

            if (sx > XSize || sy > YSize) continue;

            // Check distance for fog of war
            dist = 0;
            if (i > player_x) dist = i - player_x; else dist = player_x - i;
            if (j > player_y) { if (j - player_y > dist) dist = j - player_y; }
            else { if (player_y - j > dist) dist = player_y - j; }

            if (dist > VIEW_DIST) {
                // Outside visibility - draw dark
                _XL_DRAW(sx, sy, T_DARK, C_DARK);
                continue;
            }

            // Determine what to draw
            tile = T_FLOOR;
            color = C_FLOOR;

            if (maze[i][j] == 1) {
                tile = T_WALL;
                color = C_WALL;
            }

            // Exit
            if (i == MAZE_SIZE - 1 && j == MAZE_SIZE - 1) {
                tile = T_EXIT;
                color = C_EXIT;
            }

            // Monsters
            for (uint8_t m = 0; m < num_monsters; m++) {
                if (monster_x[m] == i && monster_y[m] == j) {
                    tile = T_MONSTER;
                    color = C_MONSTER;
                }
            }

            // Bullet
            if (bullet_active && bullet_x == i && bullet_y == j) {
                tile = T_BULLET;
                color = C_BULLET;
            }

            // Player (drawn last so it's on top)
            if (i == player_x && j == player_y) {
                tile = T_PLAYER;
                color = C_PLAYER;
            }

            _XL_DRAW(sx, sy, tile, color);
        }
    }
}

// --- Move Player ---
void move_player(uint8_t input) {
    uint8_t nx, ny;
    uint8_t moved;

    moved = 0;

    if (_XL_UP(input)) {
        player_dir = 0;
        nx = player_x;
        ny = (player_y > 0) ? (player_y - 1) : player_y;
        if (maze[nx][ny] == 0) {
            player_x = nx;
            player_y = ny;
            moved = 1;
        }
    } else if (_XL_RIGHT(input)) {
        player_dir = 1;
        nx = (player_x < MAZE_SIZE - 1) ? (player_x + 1) : player_x;
        ny = player_y;
        if (maze[nx][ny] == 0) {
            player_x = nx;
            player_y = ny;
            moved = 1;
        }
    } else if (_XL_DOWN(input)) {
        player_dir = 2;
        nx = player_x;
        ny = (player_y < MAZE_SIZE - 1) ? (player_y + 1) : player_y;
        if (maze[nx][ny] == 0) {
            player_x = nx;
            player_y = ny;
            moved = 1;
        }
    } else if (_XL_LEFT(input)) {
        player_dir = 3;
        nx = (player_x > 0) ? (player_x - 1) : player_x;
        ny = player_y;
        if (maze[nx][ny] == 0) {
            player_x = nx;
            player_y = ny;
            moved = 1;
        }
    }

    if (moved) {
        _XL_TICK_SOUND();
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

	nx = bullet_x + dx[bullet_dir];   // was: player_x
	ny = bullet_y + dy[bullet_dir];   // was: player_y

    // Out of bounds
    if (nx >= MAZE_SIZE || ny >= MAZE_SIZE) {
        bullet_active = 0;
        return;
    }

    // Hit wall
    if (maze[nx][ny] == 1) {
        bullet_active = 0;
        return;
    }

    // Hit monster?
    hit_monster = 0;
    for (uint8_t m = 0; m < num_monsters; m++) {
        if (monster_x[m] == nx && monster_y[m] == ny) {
            // Kill monster: swap with last
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

// --- Move Monsters (slow, greedy toward player) ---
void move_monsters(void) {
    uint8_t i;
    uint8_t nx, ny;
    uint8_t best_dir;
    uint8_t best_dist;
    uint8_t cur_dist;
    uint8_t d;
    uint8_t tx, ty;

    for (i = 0; i < num_monsters; i++) {
        // Try to move toward player
        // Calculate current distance
        tx = player_x;
        ty = player_y;

        best_dist = 255;
        best_dir = 0;

        for (d = 0; d < 4; d++) {
            nx = monster_x[i] + dx[d];
            ny = monster_y[i] + dy[d];

            if (nx >= MAZE_SIZE || ny >= MAZE_SIZE) continue;
            if (maze[nx][ny] == 1) continue;

            // Don't move onto another monster
            uint8_t blocked = 0;
            for (uint8_t k = 0; k < num_monsters; k++) {
                if (k != i && monster_x[k] == nx && monster_y[k] == ny) {
                    blocked = 1;
                    break;
                }
            }
            if (blocked) continue;

            // Calculate distance to player
            if (nx > tx) cur_dist = nx - tx; else cur_dist = tx - nx;
            if (ty > ny) { if (ty - ny > cur_dist) cur_dist = ty - ny; }
            else { if (ny > ty) { if (ny - ty > cur_dist) cur_dist = ny - ty; } }

            if (cur_dist < best_dist) {
                best_dist = cur_dist;
                best_dir = d;
            }
        }

        // If we found a better direction, move
        if (best_dist < 255) {
            nx = monster_x[i] + dx[best_dir];
            ny = monster_y[i] + dy[best_dir];
            monster_x[i] = nx;
            monster_y[i] = ny;
        }
    }
}

// --- Check Collisions ---
void check_collisions(void) {
    uint8_t i;

    // Monster on player?
    for (i = 0; i < num_monsters; i++) {
        if (monster_x[i] == player_x && monster_y[i] == player_y) {
            player_dead = 1;
            _XL_ZAP_SOUND();
            return;
        }
    }

    // Player at exit?
    if (player_x == MAZE_SIZE - 1 && player_y == MAZE_SIZE - 1) {
        level_complete = 1;
        _XL_PING_SOUND();
    }
}

// --- Next Level ---
// void next_level(void) {
    // level++;
    // if (level > NUM_LEVELS) {
        // game_over = 1; // Win condition
        // return;
    // }
    // player_x = 0;
    // player_y = 0;
    // player_dir = 1;
    // bullet_active = 0;
    // monster_timer = 0;
    // generate_maze();
    // place_monsters();
// }


void next_level(void) {
    level++;
    if (level > NUM_LEVELS) {
        game_over = 1;
        return;
    }

    // Reset per-level state
    level_complete = 0;
    player_dead = 0;
    bullet_active = 0;
    monster_timer = 0;

    player_x = 0;
    player_y = 0;
    player_dir = 1;

    generate_maze();
    place_monsters();
}
// --- Game Over / Win Screen ---
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

    _XL_PRINT(XSize / 4, YSize / 2 + 2, "PRESS ANY KEY");
    _XL_WAIT_FOR_INPUT();
}

// --- Main ---
int main(void) {
    uint8_t input;
    uint8_t fire_pressed;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    // Initial state
    level = 1;
    score = 0;
    game_over = 0;
    level_complete = 0;
    player_dead = 0;
    monster_timer = 0;
    frame_count = 0;
    bullet_active = 0;

    player_x = 0;
    player_y = 0;
    player_dir = 1;

    generate_maze();
    place_monsters();

    // Title
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(XSize / 4, YSize / 4, "MAZE RUNNER");
    _XL_PRINT(XSize / 4, YSize / 4 + 2, "20 LEVELS");
    _XL_PRINT(XSize / 4, YSize / 4 + 4, "FIND THE EXIT");
    _XL_PRINT(XSize / 4, YSize / 4 + 6, "AVOID MONSTERS");
    _XL_PRINT(XSize / 4, YSize / 4 + 8, "PRESS ANY KEY");
    _XL_WAIT_FOR_INPUT();

    while (!game_over) {
        // Draw
        draw_screen();

        // Input
        input = _XL_INPUT();

        // Move player
        move_player(input);

        // Shoot
        fire_pressed = _XL_FIRE(input);
        if (fire_pressed && !bullet_active) {
            shoot();
        }

        // Move bullet
        move_bullet();

        // Monster timer
        monster_timer++;
        if (monster_timer >= MONSTER_MOVE_INTERVAL) {
            monster_timer = 0;
            move_monsters();
        }

        // Check collisions
        check_collisions();

        if (player_dead) {
            show_end_screen();
            break;
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

        // Slow down for game pacing
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}
