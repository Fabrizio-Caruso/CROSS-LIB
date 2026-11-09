#include "cross_lib.h"

#define MAP_SIZE 100
#define NUM_EASY 10
#define NUM_HARD 10
#define NUM_VERY_HARD 10
#define NUM_DRAGON 1
#define NUM_ENEMIES 31
#define NUM_GEMS 40
#define NUM_POTIONS 20
#define NUM_INVINCIBLE 5
#define NUM_TORCHES 5
#define NUM_HEALTH 5
#define NUM_ITEMS 75
#define MAX_PROJECTILES 30
#define MAX_SHOT_RANGE 6
#define INVINCIBLE_DURATION 150
#define PLAYER_MAX_HP 5
#define MIN_SPAWN_DIST 15
#define FAR_SPAWN_DIST 35
#define INITIAL_VISIBILITY 6
#define MAX_VISIBILITY 12
#define CHASE_RANGE 10
#define EMPTY_TILE 255
#define ENEMY_BULLET_SPEED 2

#define ENEMY_EASY 0
#define ENEMY_HARD 1
#define ENEMY_VERY_HARD 2
#define ENEMY_DRAGON 3

#define ITEM_GEM 0
#define ITEM_POTION 1
#define ITEM_INVINCIBLE 2
#define ITEM_TORCH 3
#define ITEM_HEALTH 4

#define PROJ_PLAYER 0
#define PROJ_ENEMY 1

#define DIR_NONE 0
#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_RIGHT 4

typedef struct {
    uint8_t x, y, type, alive, shoot_timer, move_timer;
    uint16_t health;
} Enemy;

typedef struct {
    uint8_t x, y, type, alive;
} Item;

typedef struct {
    uint8_t x, y, dir, type, alive, range, move_timer;
} Projectile;

typedef struct {
    uint8_t x, y, potions, invincible_timer, hp, gems, shoot_dir, alive;
} Player;

Enemy enemies[NUM_ENEMIES];
Item items[NUM_ITEMS];
Projectile projectiles[MAX_PROJECTILES];
Player player;
uint8_t visibility;
uint8_t game_over;
uint8_t game_won;
uint16_t score;

static uint8_t prev_tile[XSize][YSize];
static uint8_t prev_color[XSize][YSize];
static uint8_t curr_tile[XSize][YSize];
static uint8_t curr_color[XSize][YSize];

void reset_screen_buffer(void)
{
    uint8_t x, y;
    for (y = 0; y < YSize; y++)
    {
        for (x = 0; x < XSize; x++)
        {
            prev_tile[x][y] = EMPTY_TILE;
            prev_color[x][y] = 0;
        }
    }
}

void spawn_enemy(uint8_t idx, uint8_t type, uint16_t health, uint8_t min_dist)
{
    uint8_t attempts;
    short dx, dy;
    uint16_t dist_sq;
    uint16_t min_sq;
    uint8_t ex, ey;

    min_sq = (uint16_t)(min_dist * min_dist);
    attempts = 0;

    while (1)
    {
        ex = (uint8_t)(1 + _XL_RAND() % (MAP_SIZE - 2));
        ey = (uint8_t)(1 + _XL_RAND() % (MAP_SIZE - 2));
        dx = (short)ex - (short)player.x;
        dy = (short)ey - (short)player.y;
        dist_sq = (uint16_t)(dx * dx + dy * dy);
        if (dist_sq >= min_sq)
            break;
        attempts++;
        if (attempts > 200)
            break;
    }

    enemies[idx].x = ex;
    enemies[idx].y = ey;
    enemies[idx].type = type;
    enemies[idx].health = health;
    enemies[idx].alive = 1;
    enemies[idx].shoot_timer = 0;
    enemies[idx].move_timer = 0;
}

void init_game(void)
{
    uint8_t i;

    player.x = MAP_SIZE / 2;
    player.y = MAP_SIZE / 2;
    player.potions = 0;
    player.invincible_timer = 0;
    player.hp = PLAYER_MAX_HP;
    player.gems = 0;
    player.shoot_dir = DIR_NONE;
    player.alive = 1;

    visibility = INITIAL_VISIBILITY;

    game_over = 0;
    game_won = 0;
    score = 0;

    for (i = 0; i < NUM_EASY; i++)
        spawn_enemy(i, ENEMY_EASY, 1, MIN_SPAWN_DIST);
    for (i = 0; i < NUM_HARD; i++)
        spawn_enemy(NUM_EASY + i, ENEMY_HARD, 6, MIN_SPAWN_DIST);
    for (i = 0; i < NUM_VERY_HARD; i++)
        spawn_enemy(NUM_EASY + NUM_HARD + i, ENEMY_VERY_HARD, 20, FAR_SPAWN_DIST);
    spawn_enemy(NUM_EASY + NUM_HARD + NUM_VERY_HARD, ENEMY_DRAGON, 200, FAR_SPAWN_DIST);

    for (i = 0; i < NUM_ITEMS; i++)
    {
        items[i].alive = 1;
        if (i < NUM_GEMS)
            items[i].type = ITEM_GEM;
        else if (i < NUM_GEMS + NUM_POTIONS)
            items[i].type = ITEM_POTION;
        else if (i < NUM_GEMS + NUM_POTIONS + NUM_INVINCIBLE)
            items[i].type = ITEM_INVINCIBLE;
        else if (i < NUM_GEMS + NUM_POTIONS + NUM_INVINCIBLE + NUM_TORCHES)
            items[i].type = ITEM_TORCH;
        else
            items[i].type = ITEM_HEALTH;
        items[i].x = (uint8_t)(1 + _XL_RAND() % (MAP_SIZE - 2));
        items[i].y = (uint8_t)(1 + _XL_RAND() % (MAP_SIZE - 2));
    }

    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        projectiles[i].alive = 0;
        projectiles[i].x = 0;
        projectiles[i].y = 0;
        projectiles[i].dir = DIR_NONE;
        projectiles[i].type = PROJ_PLAYER;
        projectiles[i].range = 0;
        projectiles[i].move_timer = 0;
    }

    reset_screen_buffer();
}

void spawn_projectile(uint8_t x, uint8_t y, uint8_t dir, uint8_t type)
{
    uint8_t i;
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].alive)
        {
            projectiles[i].alive = 1;
            projectiles[i].x = x;
            projectiles[i].y = y;
            projectiles[i].dir = dir;
            projectiles[i].type = type;
            projectiles[i].range = MAX_SHOT_RANGE;
            projectiles[i].move_timer = 0;
            return;
        }
    }
}

void update_player(uint8_t input)
{
    short nx, ny;
    uint8_t i;
    short px, py;

    if (!player.alive) return;

    if (player.invincible_timer > 0)
        player.invincible_timer--;

    if (_XL_LEFT(input))
    {
        nx = (short)player.x - 1;
        if (nx >= 1)
        {
            player.x = (uint8_t)nx;
            player.shoot_dir = DIR_LEFT;
        }
    }
    else if (_XL_RIGHT(input))
    {
        nx = (short)player.x + 1;
        if (nx < MAP_SIZE - 1)
        {
            player.x = (uint8_t)nx;
            player.shoot_dir = DIR_RIGHT;
        }
    }
    else if (_XL_UP(input))
    {
        ny = (short)player.y - 1;
        if (ny >= 1)
        {
            player.y = (uint8_t)ny;
            player.shoot_dir = DIR_UP;
        }
    }
    else if (_XL_DOWN(input))
    {
        ny = (short)player.y + 1;
        if (ny < MAP_SIZE - 1)
        {
            player.y = (uint8_t)ny;
            player.shoot_dir = DIR_DOWN;
        }
    }

    if (_XL_FIRE(input) && player.shoot_dir != DIR_NONE)
    {
        px = (short)player.x;
        py = (short)player.y;
        switch (player.shoot_dir)
        {
            case DIR_UP: py--; break;
            case DIR_DOWN: py++; break;
            case DIR_LEFT: px--; break;
            case DIR_RIGHT: px++; break;
            default: break;
        }
        if (px >= 1 && px < MAP_SIZE - 1 && py >= 1 && py < MAP_SIZE - 1)
        {
            spawn_projectile((uint8_t)px, (uint8_t)py, player.shoot_dir, PROJ_PLAYER);
            _XL_SHOOT_SOUND();
        }
    }

    for (i = 0; i < NUM_ITEMS; i++)
    {
        if (items[i].alive && items[i].x == player.x && items[i].y == player.y)
        {
            items[i].alive = 0;
            if (items[i].type == ITEM_GEM)
            {
                player.gems++;
                score += 10;
                _XL_PING_SOUND();
            }
            else if (items[i].type == ITEM_POTION)
            {
                if (player.potions < 255) player.potions++;
                _XL_PING_SOUND();
            }
            else if (items[i].type == ITEM_INVINCIBLE)
            {
                player.invincible_timer = INVINCIBLE_DURATION;
                _XL_PING_SOUND();
            }
            else if (items[i].type == ITEM_TORCH)
            {
                if (visibility < MAX_VISIBILITY)
                    visibility++;
                _XL_PING_SOUND();
            }
            else if (items[i].type == ITEM_HEALTH)
            {
                if (player.hp < PLAYER_MAX_HP)
                    player.hp++;
                _XL_PING_SOUND();
            }
        }
    }
}

void update_enemies(void)
{
    uint8_t i;
    short dx, dy;
    uint8_t move_interval;
    uint8_t shoot_interval;
    uint8_t sdir;
    short px, py;

    for (i = 0; i < NUM_ENEMIES; i++)
    {
        if (!enemies[i].alive) continue;

        enemies[i].move_timer++;

        if (enemies[i].type == ENEMY_EASY) move_interval = 12;
        else if (enemies[i].type == ENEMY_HARD) move_interval = 18;
        else if (enemies[i].type == ENEMY_VERY_HARD) move_interval = 25;
        else move_interval = 30;

        if (enemies[i].move_timer >= move_interval)
        {
            enemies[i].move_timer = 0;
            dx = (short)player.x - (short)enemies[i].x;
            dy = (short)player.y - (short)enemies[i].y;

            if (dx > CHASE_RANGE || dx < -CHASE_RANGE || dy > CHASE_RANGE || dy < -CHASE_RANGE)
            {
                /* Too far, do not chase */
            }
            else
            {
                if (dx > 0)
                {
                    if (enemies[i].x < MAP_SIZE - 2) enemies[i].x++;
                }
                else if (dx < 0)
                {
                    if (enemies[i].x > 1) enemies[i].x--;
                }
                if (dy > 0)
                {
                    if (enemies[i].y < MAP_SIZE - 2) enemies[i].y++;
                }
                else if (dy < 0)
                {
                    if (enemies[i].y > 1) enemies[i].y--;
                }
            }
        }

        if (enemies[i].type == ENEMY_VERY_HARD || enemies[i].type == ENEMY_DRAGON)
        {
            if (enemies[i].type == ENEMY_DRAGON) shoot_interval = 40;
            else shoot_interval = 50;

            enemies[i].shoot_timer++;
            if (enemies[i].shoot_timer >= shoot_interval)
            {
                enemies[i].shoot_timer = 0;
                dx = (short)player.x - (short)enemies[i].x;
                dy = (short)player.y - (short)enemies[i].y;

                if (dx > 0) sdir = DIR_RIGHT;
                else if (dx < 0) sdir = DIR_LEFT;
                else if (dy > 0) sdir = DIR_DOWN;
                else sdir = DIR_UP;

                px = (short)enemies[i].x;
                py = (short)enemies[i].y;
                switch (sdir)
                {
                    case DIR_UP: py--; break;
                    case DIR_DOWN: py++; break;
                    case DIR_LEFT: px--; break;
                    case DIR_RIGHT: px++; break;
                    default: break;
                }
                if (px >= 1 && px < MAP_SIZE - 1 && py >= 1 && py < MAP_SIZE - 1)
                {
                    spawn_projectile((uint8_t)px, (uint8_t)py, sdir, PROJ_ENEMY);
                    _XL_TICK_SOUND();
                }
            }
        }
    }
}

void update_projectiles(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t hit;
    short px, py;
    uint16_t dmg;
    uint8_t speed;

    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].alive) continue;

        if (projectiles[i].type == PROJ_PLAYER)
            speed = 1;
        else
            speed = ENEMY_BULLET_SPEED;

        projectiles[i].move_timer++;
        if (projectiles[i].move_timer < speed)
            continue;
        projectiles[i].move_timer = 0;

        px = (short)projectiles[i].x;
        py = (short)projectiles[i].y;
        switch (projectiles[i].dir)
        {
            case DIR_UP: py--; break;
            case DIR_DOWN: py++; break;
            case DIR_LEFT: px--; break;
            case DIR_RIGHT: px++; break;
            default: break;
        }

        if (px < 1 || px >= MAP_SIZE - 1 || py < 1 || py >= MAP_SIZE - 1)
        {
            projectiles[i].alive = 0;
            continue;
        }

        projectiles[i].x = (uint8_t)px;
        projectiles[i].y = (uint8_t)py;
        projectiles[i].range--;
        if (projectiles[i].range == 0)
        {
            projectiles[i].alive = 0;
            continue;
        }

        hit = 0;
        if (projectiles[i].type == PROJ_PLAYER)
        {
            for (j = 0; j < NUM_ENEMIES; j++)
            {
                if (!enemies[j].alive) continue;
                if (enemies[j].x == projectiles[i].x && enemies[j].y == projectiles[i].y)
                {
                    hit = 1;
                    dmg = 1 + (uint16_t)player.potions;
                    if (enemies[j].type == ENEMY_DRAGON && player.potions < 10)
                    {
                        dmg = 0;
                    }
                    if (dmg > 0)
                    {
                        if (enemies[j].health >= dmg)
                            enemies[j].health -= dmg;
                        else
                            enemies[j].health = 0;
                        _XL_TOCK_SOUND();
                    }
                    if (enemies[j].health == 0)
                    {
                        enemies[j].alive = 0;
                        score += 25;
                        _XL_EXPLOSION_SOUND();
                        if (enemies[j].type == ENEMY_DRAGON)
                        {
                            game_won = 1;
                            game_over = 1;
                        }
                    }
                    break;
                }
            }
        }
        else
        {
            if (projectiles[i].x == player.x && projectiles[i].y == player.y)
            {
                hit = 1;
                if (player.invincible_timer == 0 && player.alive)
                {
                    if (player.hp > 0) player.hp--;
                    _XL_ZAP_SOUND();
                    if (player.hp == 0)
                    {
                        player.alive = 0;
                        game_over = 1;
                    }
                }
            }
        }

        if (hit)
            projectiles[i].alive = 0;
    }
}
void render(void)
{
    uint8_t i;
    short dx, dy;
    short sx, sy;
    short cx, cy;
    uint8_t tile, color;
    short vis;
    uint8_t ptile, pcolor;
    uint8_t x, y;
    short wx, wy;
    short x_start, x_end, y_start, y_end;

    for (y = 0; y < YSize; y++)
    {
        for (x = 0; x < XSize; x++)
        {
            curr_tile[x][y] = EMPTY_TILE;
            curr_color[x][y] = 0;
        }
    }

    cx = (short)XSize / 2;
    cy = (short)YSize / 2;
    vis = (short)visibility;

    x_start = cx - vis;
    if (x_start < 0) x_start = 0;
    x_end = cx + vis;
    if (x_end >= (short)XSize) x_end = (short)XSize - 1;
    y_start = cy - vis;
    if (y_start < 0) y_start = 0;
    y_end = cy + vis;
    if (y_end >= (short)YSize) y_end = (short)YSize - 1;

    /* Draw map walls at boundaries */
    for (y = 0; y < YSize; y++)
    {
        for (x = 0; x < XSize; x++)
        {
            wx = (short)player.x + (short)x - cx;
            wy = (short)player.y + (short)y - cy;
            if (wx < 0 || wx >= MAP_SIZE || wy < 0 || wy >= MAP_SIZE)
                continue;
            if (wx == 0 || wx == MAP_SIZE - 1 || wy == 0 || wy == MAP_SIZE - 1)
            {
                dx = wx - (short)player.x;
                dy = wy - (short)player.y;
                if (dx * dx + dy * dy <= vis * vis)
                {
                    curr_tile[x][y] = _TILE_13;
                    curr_color[x][y] = _XL_BLUE;
                }
            }
        }
    }

    /* Record items */
    for (i = 0; i < NUM_ITEMS; i++)
    {
        if (!items[i].alive) continue;
        dx = (short)items[i].x - (short)player.x;
        dy = (short)items[i].y - (short)player.y;
        if (dx * dx + dy * dy > vis * vis) continue;
        sx = cx + dx;
        sy = cy + dy;
        if (sx < 0 || sx >= (short)XSize) continue;
        if (sy < 0 || sy >= (short)YSize) continue;
        if (items[i].type == ITEM_GEM)
        {
            tile = _TILE_5;
            color = _XL_CYAN;
        }
        else if (items[i].type == ITEM_POTION)
        {
            tile = _TILE_6;
            color = _XL_GREEN;
        }
        else if (items[i].type == ITEM_INVINCIBLE)
        {
            tile = _TILE_7;
            color = _XL_WHITE;
        }
        else if (items[i].type == ITEM_TORCH)
        {
            tile = _TILE_11;
            color = _XL_YELLOW;
        }
        else
        {
            tile = _TILE_12;
            color = _XL_RED;
        }
        curr_tile[sx][sy] = tile;
        curr_color[sx][sy] = color;
    }

    /* Record enemies */
    for (i = 0; i < NUM_ENEMIES; i++)
    {
        if (!enemies[i].alive) continue;
        dx = (short)enemies[i].x - (short)player.x;
        dy = (short)enemies[i].y - (short)player.y;
        if (dx * dx + dy * dy > vis * vis) continue;
        sx = cx + dx;
        sy = cy + dy;
        if (sx < 0 || sx >= (short)XSize) continue;
        if (sy < 0 || sy >= (short)YSize) continue;
        if (enemies[i].type == ENEMY_EASY)
        {
            tile = _TILE_1;
            color = _XL_RED;
        }
        else if (enemies[i].type == ENEMY_HARD)
        {
            tile = _TILE_2;
            color = _XL_YELLOW;
        }
        else if (enemies[i].type == ENEMY_VERY_HARD)
        {
            tile = _TILE_3;
            color = _XL_MAGENTA;
        }
        else
        {
            tile = _TILE_4;
            color = _XL_RED;
        }
        curr_tile[sx][sy] = tile;
        curr_color[sx][sy] = color;
    }

    /* Record projectiles */
    for (i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!projectiles[i].alive) continue;
        dx = (short)projectiles[i].x - (short)player.x;
        dy = (short)projectiles[i].y - (short)player.y;
        if (dx * dx + dy * dy > vis * vis) continue;
        sx = cx + dx;
        sy = cy + dy;
        if (sx < 0 || sx >= (short)XSize) continue;
        if (sy < 0 || sy >= (short)YSize) continue;
        if (projectiles[i].type == PROJ_PLAYER)
        {
            tile = _TILE_8;
            color = _XL_YELLOW;
        }
        else
        {
            tile = _TILE_9;
            color = _XL_RED;
        }
        curr_tile[sx][sy] = tile;
        curr_color[sx][sy] = color;
    }

    /* Record player */
    ptile = _TILE_0;
    if (player.invincible_timer > 0)
        pcolor = _XL_WHITE;
    else
        pcolor = _XL_GREEN;
    curr_tile[cx][cy] = ptile;
    curr_color[cx][cy] = pcolor;

    /* Record visibility border */
    {
        short bx, by;
        short bi;
        by = cy - vis;
        for (bi = -vis; bi <= vis; bi++)
        {
            bx = cx + bi;
            if (bx >= 0 && bx < (short)XSize && by >= 0 && by < (short)YSize)
            {
                curr_tile[bx][by] = _TILE_10;
                curr_color[bx][by] = _XL_MAGENTA;
            }
        }
        by = cy + vis;
        for (bi = -vis; bi <= vis; bi++)
        {
            bx = cx + bi;
            if (bx >= 0 && bx < (short)XSize && by >= 0 && by < (short)YSize)
            {
                curr_tile[bx][by] = _TILE_10;
                curr_color[bx][by] = _XL_MAGENTA;
            }
        }
        bx = cx - vis;
        for (bi = -vis + 1; bi <= vis - 1; bi++)
        {
            by = cy + bi;
            if (bx >= 0 && bx < (short)XSize && by >= 0 && by < (short)YSize)
            {
                curr_tile[bx][by] = _TILE_10;
                curr_color[bx][by] = _XL_MAGENTA;
            }
        }
        bx = cx + vis;
        for (bi = -vis + 1; bi <= vis - 1; bi++)
        {
            by = cy + bi;
            if (bx >= 0 && bx < (short)XSize && by >= 0 && by < (short)YSize)
            {
                curr_tile[bx][by] = _TILE_10;
                curr_color[bx][by] = _XL_MAGENTA;
            }
        }
    }

    /* Compare and draw only changed tiles in the visible area */
    for (y = (uint8_t)y_start; y <= (uint8_t)y_end; y++)
    {
        for (x = (uint8_t)x_start; x <= (uint8_t)x_end; x++)
        {
            if (curr_tile[x][y] != prev_tile[x][y] || curr_color[x][y] != prev_color[x][y])
            {
                if (curr_tile[x][y] != EMPTY_TILE)
                {
                    _XL_DRAW(x, y, curr_tile[x][y], curr_color[x][y]);
                }
                else
                {
                    _XL_DELETE(x, y);
                }
            }
            prev_tile[x][y] = curr_tile[x][y];
            prev_color[x][y] = curr_color[x][y];
        }
    }

    /* HUD */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "SCORE");
    _XL_PRINTD(30, 0, 1, score);
    _XL_PRINT(0, 1, "POT");
    _XL_PRINTD(15, 1, 1, (uint16_t)player.potions);
    _XL_PRINT(0, 2, "HP");
    _XL_PRINTD(10, 2, 1, (uint16_t)player.hp);
    if (player.invincible_timer > 0)
    {
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(0, 3, "INV");
    }
    else
    {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 3, "   ");
    }
}


void show_game_over(void)
{
    _XL_CLEAR_SCREEN();
    reset_screen_buffer();
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    if (game_won)
    {
        _XL_PRINT(XSize / 4, YSize / 3, "YOU WIN");
        _XL_PRINT(XSize / 8, YSize / 3 + 2, "DRAGON SLAIN");
    }
    else
    {
        _XL_PRINT(XSize / 4, YSize / 3, "GAME OVER");
    }
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(XSize / 8, YSize / 2, "SCORE");
    _XL_PRINTD(XSize / 2, YSize / 2, 1, score);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(XSize / 8, YSize / 2 + 2, "PRESS ANY KEY");
}

int main(void)
{
    uint8_t input;
    uint8_t need_restart;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    need_restart = 1;

    while (1)
    {
        if (need_restart)
        {
            init_game();
            need_restart = 0;
        }

        if (!game_over)
        {
            input = _XL_INPUT();
            update_player(input);
            update_enemies();
            update_projectiles();
            render();
        }
        else
        {
            show_game_over();
            _XL_WAIT_FOR_INPUT();
            need_restart = 1;
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}

