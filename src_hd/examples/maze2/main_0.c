#include "cross_lib.h"

#define MAZE_SIZE 20
#define MAX_LEVELS 20
#define MAX_MONSTERS 20
#define VISION_INITIAL 4
#define VISION_TORCH 7
#define MONSTER_MOVE_INTERVAL 5
#define ROOMS 10

uint8_t maze[MAZE_SIZE][MAZE_SIZE];
uint8_t player_x, player_y;
uint8_t player_dir;
uint8_t key_x, key_y;
uint8_t torch_x, torch_y;
uint8_t exit_x, exit_y;
uint8_t _has_key;
uint8_t has_torch;
uint8_t vision_radius;
uint8_t level;
uint8_t num_monsters;
uint8_t monster_x[MAX_MONSTERS];
uint8_t monster_y[MAX_MONSTERS];
uint8_t monster_alive[MAX_MONSTERS];
uint8_t bullet_x, bullet_y;
uint8_t bullet_dir;
uint8_t bullet_active;
uint8_t game_state;
uint8_t move_counter;
uint16_t score;
uint8_t visited[ROOMS][ROOMS];
uint8_t dfs_stack_x[100];
uint8_t dfs_stack_y[100];
uint8_t dfs_stack_top;

void generate_maze(void)
{
    uint8_t i, j;
    uint8_t cx, cy;
    uint8_t found;
    uint8_t start_dir;
    uint8_t d;

    for (i = 0; i < MAZE_SIZE; i++)
        for (j = 0; j < MAZE_SIZE; j++)
            maze[i][j] = 0;
    for (i = 0; i < ROOMS; i++)
        for (j = 0; j < ROOMS; j++)
            visited[i][j] = 0;

    dfs_stack_top = 0;
    dfs_stack_x[0] = 0;
    dfs_stack_y[0] = 0;
    visited[0][0] = 1;
    maze[0][0] = 1;
    dfs_stack_top = 1;

    while (dfs_stack_top > 0) {
        cx = dfs_stack_x[dfs_stack_top - 1];
        cy = dfs_stack_y[dfs_stack_top - 1];
        found = 0;
        start_dir = (uint8_t)(_XL_RAND() % 4);

        for (i = 0; i < 4; i++) {
            d = (start_dir + i) % 4;
            if (d == 0 && cy > 0 && !visited[cx][cy - 1]) {
                maze[cx * 2][cy * 2 - 1] = 1;
                maze[cx * 2][cy * 2 - 2] = 1;
                visited[cx][cy - 1] = 1;
                dfs_stack_x[dfs_stack_top] = cx;
                dfs_stack_y[dfs_stack_top] = cy - 1;
                dfs_stack_top++;
                found = 1;
                break;
            } else if (d == 1 && cx < ROOMS - 1 && !visited[cx + 1][cy]) {
                maze[cx * 2 + 1][cy * 2] = 1;
                maze[cx * 2 + 2][cy * 2] = 1;
                visited[cx + 1][cy] = 1;
                dfs_stack_x[dfs_stack_top] = cx + 1;
                dfs_stack_y[dfs_stack_top] = cy;
                dfs_stack_top++;
                found = 1;
                break;
            } else if (d == 2 && cy < ROOMS - 1 && !visited[cx][cy + 1]) {
                maze[cx * 2][cy * 2 + 1] = 1;
                maze[cx * 2][cy * 2 + 2] = 1;
                visited[cx][cy + 1] = 1;
                dfs_stack_x[dfs_stack_top] = cx;
                dfs_stack_y[dfs_stack_top] = cy + 1;
                dfs_stack_top++;
                found = 1;
                break;
            } else if (d == 3 && cx > 0 && !visited[cx - 1][cy]) {
                maze[cx * 2 - 1][cy * 2] = 1;
                maze[cx * 2 - 2][cy * 2] = 1;
                visited[cx - 1][cy] = 1;
                dfs_stack_x[dfs_stack_top] = cx - 1;
                dfs_stack_y[dfs_stack_top] = cy;
                dfs_stack_top++;
                found = 1;
                break;
            }
        }

        if (!found) {
            dfs_stack_top--;
        }
    }
}

uint8_t is_path(uint8_t x, uint8_t y)
{
    if (x >= MAZE_SIZE || y >= MAZE_SIZE) return 0;
    return maze[x][y];
}

uint8_t manhattan_dist(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    short dx = (short)x1 - (short)x2;
    short dy = (short)y1 - (short)y2;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return (uint8_t)(dx + dy);
}

uint8_t pos_taken(uint8_t px, uint8_t py)
{
    uint8_t i;
    if (px == 0 && py == 0) return 1;
    if (px == exit_x && py == exit_y) return 1;
    if (px == key_x && py == key_y) return 1;
    if (px == torch_x && py == torch_y) return 1;
    for (i = 0; i < num_monsters; i++) {
        if (monster_alive[i] && monster_x[i] == px && monster_y[i] == py) return 1;
    }
    return 0;
}

void place_items(void)
{
    uint8_t i;
    uint8_t rx, ry;
    uint8_t ok;

    exit_x = 18;
    exit_y = 18;

    do {
        rx = (uint8_t)(_XL_RAND() % ROOMS);
        ry = (uint8_t)(_XL_RAND() % ROOMS);
        ok = 1;
        if (rx == 0 && ry == 0) ok = 0;
        if (rx == ROOMS - 1 && ry == ROOMS - 1) ok = 0;
    } while (!ok);
    key_x = rx * 2;
    key_y = ry * 2;

    do {
        rx = (uint8_t)(_XL_RAND() % ROOMS);
        ry = (uint8_t)(_XL_RAND() % ROOMS);
        ok = 1;
        if (rx == 0 && ry == 0) ok = 0;
        if (rx == ROOMS - 1 && ry == ROOMS - 1) ok = 0;
        if (rx * 2 == key_x && ry * 2 == key_y) ok = 0;
    } while (!ok);
    torch_x = rx * 2;
    torch_y = ry * 2;

    for (i = 0; i < num_monsters; i++) {
        do {
            rx = (uint8_t)(_XL_RAND() % ROOMS);
            ry = (uint8_t)(_XL_RAND() % ROOMS);
            ok = 1;
            if (pos_taken(rx * 2, ry * 2)) ok = 0;
        } while (!ok);
        monster_x[i] = rx * 2;
        monster_y[i] = ry * 2;
        monster_alive[i] = 1;
    }
}

void move_monsters(void)
{
    uint8_t i;
    uint8_t dx, dy;
    uint8_t nx, ny;
    uint8_t moved;
    uint8_t d;

    for (i = 0; i < num_monsters; i++) {
        if (!monster_alive[i]) continue;

        dx = 0;
        dy = 0;
        if (player_x > monster_x[i]) dx = 1;
        else if (player_x < monster_x[i]) dx = 255;
        if (player_y > monster_y[i]) dy = 1;
        else if (player_y < monster_y[i]) dy = 255;

        moved = 0;

        if (dx != 0) {
            if (dx == 1) nx = monster_x[i] + 1;
            else nx = monster_x[i] - 1;
            ny = monster_y[i];
            if (nx < MAZE_SIZE && is_path(nx, ny)) {
                monster_x[i] = nx;
                moved = 1;
            }
        }
        if (!moved && dy != 0) {
            nx = monster_x[i];
            if (dy == 1) ny = monster_y[i] + 1;
            else ny = monster_y[i] - 1;
            if (ny < MAZE_SIZE && is_path(nx, ny)) {
                monster_y[i] = ny;
                moved = 1;
            }
        }
        if (!moved) {
            d = (uint8_t)(_XL_RAND() % 4);
            if (d == 0) {
                nx = monster_x[i];
                if (monster_y[i] > 0) ny = monster_y[i] - 1;
                else ny = 0;
                if (ny < MAZE_SIZE && is_path(nx, ny)) monster_y[i] = ny;
            } else if (d == 1) {
                if (monster_x[i] < MAZE_SIZE - 1) nx = monster_x[i] + 1;
                else nx = 0;
                ny = monster_y[i];
                if (nx < MAZE_SIZE && is_path(nx, ny)) monster_x[i] = nx;
            } else if (d == 2) {
                nx = monster_x[i];
                if (monster_y[i] < MAZE_SIZE - 1) ny = monster_y[i] + 1;
                else ny = 0;
                if (ny < MAZE_SIZE && is_path(nx, ny)) monster_y[i] = ny;
            } else {
                if (monster_x[i] > 0) nx = monster_x[i] - 1;
                else nx = 0;
                ny = monster_y[i];
                if (nx < MAZE_SIZE && is_path(nx, ny)) monster_x[i] = nx;
            }
        }
    }
}

void move_bullet(void)
{
    uint8_t nx, ny;
    uint8_t i;

    if (!bullet_active) return;

    if (bullet_dir == 0) {
        nx = bullet_x;
        if (bullet_y > 0) ny = bullet_y - 1;
        else ny = 0;
    } else if (bullet_dir == 1) {
        if (bullet_x < MAZE_SIZE - 1) nx = bullet_x + 1;
        else nx = 0;
        ny = bullet_y;
    } else if (bullet_dir == 2) {
        nx = bullet_x;
        if (bullet_y < MAZE_SIZE - 1) ny = bullet_y + 1;
        else ny = 0;
    } else {
        if (bullet_x > 0) nx = bullet_x - 1;
        else nx = 0;
        ny = bullet_y;
    }

    if (nx >= MAZE_SIZE || ny >= MAZE_SIZE || !is_path(nx, ny)) {
        bullet_active = 0;
        return;
    }

    for (i = 0; i < num_monsters; i++) {
        if (monster_alive[i] && monster_x[i] == nx && monster_y[i] == ny) {
            monster_alive[i] = 0;
            bullet_active = 0;
            score += 10;
            _XL_PING_SOUND();
            return;
        }
    }

    bullet_x = nx;
    bullet_y = ny;
}

void draw_screen(void)
{
    uint8_t x, y;
    uint8_t dist;
    uint8_t i;

    _XL_CLEAR_SCREEN();

    for (y = 0; y < MAZE_SIZE; y++) {
        for (x = 0; x < MAZE_SIZE; x++) {
            dist = manhattan_dist(x, y, player_x, player_y);
            if (dist > vision_radius) continue;
            if (maze[x][y] == 0) {
                _XL_DRAW(x, y, _TILE_0, _XL_BLUE);
            } else {
                _XL_DRAW(x, y, _TILE_1, _XL_WHITE);
            }
        }
    }

    if (manhattan_dist(key_x, key_y, player_x, player_y) <= vision_radius) {
        _XL_DRAW(key_x, key_y, _TILE_3, _XL_YELLOW);
    }

    if (manhattan_dist(torch_x, torch_y, player_x, player_y) <= vision_radius) {
        _XL_DRAW(torch_x, torch_y, _TILE_4, _XL_MAGENTA);
    }

    if (manhattan_dist(exit_x, exit_y, player_x, player_y) <= vision_radius) {
        if (_has_key) {
            _XL_DRAW(exit_x, exit_y, _TILE_6, _XL_GREEN);
        } else {
            _XL_DRAW(exit_x, exit_y, _TILE_5, _XL_RED);
        }
    }

    for (i = 0; i < num_monsters; i++) {
        if (monster_alive[i] && manhattan_dist(monster_x[i], monster_y[i], player_x, player_y) <= vision_radius) {
            _XL_DRAW(monster_x[i], monster_y[i], _TILE_7, _XL_RED);
        }
    }

    if (bullet_active) {
        _XL_DRAW(bullet_x, bullet_y, _TILE_8, _XL_CYAN);
    }

    _XL_DRAW(player_x, player_y, _TILE_2, _XL_GREEN);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "LVL ");
    _XL_PRINTD(4, 0, 1, level);
    _XL_PRINT(8, 0, "SCORE ");
    _XL_PRINTD(14, 0, 1, score);

    if (_has_key) {
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(0, 1, "KEY");
    }
    if (has_torch) {
        _XL_SET_TEXT_COLOR(_XL_MAGENTA);
        _XL_PRINT(4, 1, "TORCH");
    }
}

void init_level(uint8_t lvl)
{
    uint8_t i;

    level = lvl;
    num_monsters = lvl;
    if (num_monsters > MAX_MONSTERS) num_monsters = MAX_MONSTERS;

    player_x = 0;
    player_y = 0;
    player_dir = 1;
    _has_key = 0;
    has_torch = 0;
    vision_radius = VISION_INITIAL;
    bullet_active = 0;
    move_counter = 0;
    game_state = 0;

    generate_maze();
    place_items();
}

int main(void)
{
    uint8_t input;
    uint8_t i;
    uint8_t nx, ny;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    score = 0;

    while (1) {
        init_level(1);

        while (game_state == 0) {
            input = _XL_INPUT();

            if (_XL_LEFT(input)) {
                player_dir = 3;
                if (player_x > 0) nx = player_x - 1;
                else nx = 0;
                ny = player_y;
                if (is_path(nx, ny)) {
                    player_x = nx;
                    move_counter++;
                }
            } else if (_XL_RIGHT(input)) {
                player_dir = 1;
                if (player_x < MAZE_SIZE - 1) nx = player_x + 1;
                else nx = 0;
                ny = player_y;
                if (is_path(nx, ny)) {
                    player_x = nx;
                    move_counter++;
                }
            } else if (_XL_UP(input)) {
                player_dir = 0;
                nx = player_x;
                if (player_y > 0) ny = player_y - 1;
                else ny = 0;
                if (is_path(nx, ny)) {
                    player_y = ny;
                    move_counter++;
                }
            } else if (_XL_DOWN(input)) {
                player_dir = 2;
                nx = player_x;
                if (player_y < MAZE_SIZE - 1) ny = player_y + 1;
                else ny = 0;
                if (is_path(nx, ny)) {
                    player_y = ny;
                    move_counter++;
                }
            }

            if (_XL_FIRE(input) && !bullet_active) {
                if (player_dir == 0) {
                    bullet_x = player_x;
                    if (player_y > 0) bullet_y = player_y - 1;
                    else bullet_y = 0;
                } else if (player_dir == 1) {
                    if (player_x < MAZE_SIZE - 1) bullet_x = player_x + 1;
                    else bullet_x = 0;
                    bullet_y = player_y;
                } else if (player_dir == 2) {
                    bullet_x = player_x;
                    if (player_y < MAZE_SIZE - 1) bullet_y = player_y + 1;
                    else bullet_y = 0;
                } else {
                    if (player_x > 0) bullet_x = player_x - 1;
                    else bullet_x = 0;
                    bullet_y = player_y;
                }
                bullet_dir = player_dir;
                if (is_path(bullet_x, bullet_y)) {
                    bullet_active = 1;
                    _XL_SHOOT_SOUND();
                }
            }

            move_bullet();

            if (move_counter % MONSTER_MOVE_INTERVAL == 0 && move_counter > 0) {
                move_monsters();
            }

            if (player_x == key_x && player_y == key_y) {
                _has_key = 1;
                score += 50;
                _XL_TOCK_SOUND();
            }
            if (player_x == torch_x && player_y == torch_y) {
                has_torch = 1;
                vision_radius = VISION_TORCH;
                score += 25;
                _XL_TOCK_SOUND();
            }

            if (player_x == exit_x && player_y == exit_y) {
                if (_has_key) {
                    score += 100;
                    _XL_ZAP_SOUND();
                    if (level >= MAX_LEVELS) {
                        game_state = 3;
                    } else {
                        game_state = 1;
                    }
                }
            }

            for (i = 0; i < num_monsters; i++) {
                if (monster_alive[i] && monster_x[i] == player_x && monster_y[i] == player_y) {
                    game_state = 2;
                    _XL_EXPLOSION_SOUND();
                    break;
                }
            }

            draw_screen();

            if (game_state == 1) {
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(2, 10, "LEVEL COMPLETE");
                _XL_SLEEP(2);
                init_level(level + 1);
            }

            if (game_state == 2) {
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(2, 10, "GAME OVER");
                _XL_SLEEP(2);
                break;
            }

            if (game_state == 3) {
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(2, 10, "YOU WIN!");
                _XL_SLEEP(3);
                break;
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }

    return 0;
}