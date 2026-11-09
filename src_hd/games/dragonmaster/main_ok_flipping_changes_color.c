#include "cross_lib.h"
#include "images.h"

#define MAX_DRAGONS 64
#define MAP_W       64
#define MAP_H       64

typedef struct
{
    uint8_t x;
    uint8_t y;
    uint8_t facing;
    uint8_t alive;
} Dragon;

static Dragon dragons[MAX_DRAGONS];

static uint8_t passable[MAP_W][MAP_H];

static uint16_t bfs_queue[MAP_W * MAP_H];
static uint16_t bfs_dist[MAP_W * MAP_H];

static uint8_t initial_dragons;

static uint8_t player_x, player_y;
static uint8_t wizard_x, wizard_y;

static uint8_t game_over;
static uint8_t game_over_drawn;
static uint8_t scene_drawn;

static uint16_t frames;
static uint16_t wizard_timer;
static uint8_t wizard_interval;

static uint16_t flip_timer;
static uint16_t respawn_timer;

static uint8_t score;
static uint8_t fire_held;

/* --------------------------------------------------------------------------------------- */
/* Drawing helpers                                                                          */
/* --------------------------------------------------------------------------------------- */

static void draw_castle(void)
{
    _XL_DRAW(XSize/2-1,YSize/2,  CASTLE_NW_TILE,_XL_WHITE);
    _XL_DRAW(XSize/2,  YSize/2,  CASTLE_NE_TILE,_XL_WHITE);
    _XL_DRAW(XSize/2-1,YSize/2+1,CASTLE_SW_TILE,_XL_WHITE);
    _XL_DRAW(XSize/2,YSize/2+1,  CASTLE_SE_TILE,_XL_WHITE);
    _XL_DRAW(XSize/2-2,YSize/2+1, BRIDGE_DOWN_TILE,_XL_WHITE);
}

static void draw_left_dragon(uint8_t x, uint8_t y, uint8_t color)
{
    _XL_DRAW(x-1,y,  LEFT_NW_TILE,color);
    _XL_DRAW(x,  y,  LEFT_NE_TILE,color);
    _XL_DRAW(x-1,y+1,LEFT_SW_TILE,color);
    _XL_DRAW(x,y+1,  LEFT_SE_TILE,color);
}

static void draw_right_dragon(uint8_t x, uint8_t y, uint8_t color)
{
    _XL_DRAW(x-1,y,  RIGHT_NW_TILE,color);
    _XL_DRAW(x,  y,  RIGHT_NE_TILE,color);
    _XL_DRAW(x-1,y+1,RIGHT_SW_TILE,color);
    _XL_DRAW(x,y+1,  RIGHT_SE_TILE,color);
}

static void draw_wall(void)
{
    uint8_t i;

    for(i=0;i<XSize;++i)
    {
        _XL_DRAW(i,0,WALL_TILE, _XL_WHITE);
        _XL_DRAW(i,YSize-1,WALL_TILE, _XL_WHITE);
    }

    for(i=0;i<YSize;++i)
    {
        _XL_DRAW(0,i,WALL_TILE, _XL_WHITE);
        _XL_DRAW(XSize-1,i,WALL_TILE, _XL_WHITE);
    }
}

static void draw_player(uint8_t x, uint8_t y)
{
    _XL_DRAW(x,y,PLAYER_TILE, _XL_WHITE);
}

/* --------------------------------------------------------------------------------------- */
/* Small helpers                                                                            */
/* --------------------------------------------------------------------------------------- */

static uint8_t in_bounds(short x, short y)
{
    if (x < 0 || y < 0) return 0;
    if (x >= (short)XSize || y >= (short)YSize) return 0;
    return 1;
}

static uint8_t is_wall(short x, short y)
{
    if (x == 0 || y == 0 || x == (short)XSize - 1 || y == (short)YSize - 1)
        return 1;
    return 0;
}

static uint8_t is_castle(short x, short y)
{
    short cx = (short)XSize / 2;
    short cy = (short)YSize / 2;

    if ((x == cx - 1 || x == cx) && (y == cy || y == cy + 1))
        return 1;

    return 0;
}

static uint8_t basic_free(short x, short y)
{
    if (!in_bounds(x, y)) return 0;
    if (is_wall(x, y)) return 0;
    if (is_castle(x, y)) return 0;
    return 1;
}

static uint8_t adjacent4(short x1, short y1, short x2, short y2)
{
    short dx = x1 - x2;
    short dy = y1 - y2;

    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    return (dx + dy) == 1;
}

static uint8_t dragon_tile_equals(const Dragon *d, short x, short y)
{
    if (!d->alive) return 0;

    if ((y == (short)d->y || y == (short)d->y + 1) &&
        (x == (short)d->x - 1 || x == (short)d->x))
    {
        return 1;
    }

    return 0;
}

static uint8_t tile_is_dragon(short x, short y)
{
    uint8_t i;

    for (i = 0; i < MAX_DRAGONS; ++i)
    {
        if (dragon_tile_equals(&dragons[i], x, y))
            return 1;
    }

    return 0;
}

static void rebuild_passable(void)
{
    uint8_t x, y, i, j;
    short xs[4], ys[4];

    for (y = 0; y < (uint8_t)YSize && y < MAP_H; ++y)
    {
        for (x = 0; x < (uint8_t)XSize && x < MAP_W; ++x)
        {
            passable[y][x] = (!is_wall((short)x, (short)y) &&
                              !is_castle((short)x, (short)y)) ? 1 : 0;
        }
    }

    for (i = 0; i < MAX_DRAGONS; ++i)
    {
        if (!dragons[i].alive)
            continue;

        xs[0] = (short)dragons[i].x - 1;
        xs[1] = (short)dragons[i].x;
        xs[2] = (short)dragons[i].x - 1;
        xs[3] = (short)dragons[i].x;

        ys[0] = (short)dragons[i].y;
        ys[1] = (short)dragons[i].y;
        ys[2] = (short)dragons[i].y + 1;
        ys[3] = (short)dragons[i].y + 1;

        for (j = 0; j < 4; ++j)
        {
            if (xs[j] >= 0 && ys[j] >= 0 &&
                xs[j] < (short)XSize && ys[j] < (short)YSize &&
                xs[j] < MAP_W && ys[j] < MAP_H)
            {
                passable[ys[j]][xs[j]] = 0;
            }
        }
    }
}

static uint8_t can_move(short x, short y)
{
    if (x < 0 || y < 0) return 0;
    if (x >= (short)XSize || y >= (short)YSize) return 0;
    if (x >= MAP_W || y >= MAP_H) return 0;

    return passable[y][x] != 0;
}

/* --------------------------------------------------------------------------------------- */
/* Incremental drawing helpers                                                              */
/* --------------------------------------------------------------------------------------- */

static void draw_background(void)
{
    draw_wall();
    draw_castle();
}

static void draw_wizard(void)
{
    _XL_DRAW(wizard_x, wizard_y, CHASE_TILE, _XL_WHITE);
}

static void draw_dragon(const Dragon *d)
{
    if (!d->alive)
        return;

    if (d->facing == 0)
        draw_left_dragon(d->x, d->y, _XL_RED);
    else
        draw_right_dragon(d->x, d->y, _XL_WHITE);
}

static void delete_dragon(uint8_t i)
{
    const Dragon *d = &dragons[i];
    uint8_t xl = (uint8_t)((short)d->x - 1);
    uint8_t yh = (uint8_t)(d->y + 1);

    _XL_DELETE(xl, d->y);
    _XL_DELETE(d->x, d->y);
    _XL_DELETE(xl, yh);
    _XL_DELETE(d->x, yh);
}

/* --------------------------------------------------------------------------------------- */
/* Input / timing helpers                                                                   */
/* --------------------------------------------------------------------------------------- */

/*
 * FIXED: The original code compared the raw input byte k against the return
 * value of _XL_UP(k), _XL_DOWN(k), etc.  The API returns non-zero when the
 * corresponding key is pressed; you simply test the return value.
 */
static uint8_t get_direction(uint8_t k)
{
    uint8_t d = 0;

    if (_XL_UP(k))   d |= 1;
    if (_XL_DOWN(k)) d |= 2;
    if (_XL_LEFT(k)) d |= 4;
    if (_XL_RIGHT(k))d |= 8;

    return d;
}

/* --------------------------------------------------------------------------------------- */
/* Player movement                                                                          */
/* --------------------------------------------------------------------------------------- */

static void move_player(uint8_t dir)
{
    short dx = 0;
    short dy = 0;
    short nx;
    short ny;
    uint8_t old_x;
    uint8_t old_y;

    old_x = player_x;
    old_y = player_y;

    if (dir & 1)
    {
        dx = 0;
        dy = -1;
    }
    else if (dir & 2)
    {
        dx = 0;
        dy = 1;
    }
    else if (dir & 4)
    {
        dx = -1;
        dy = 0;
    }
    else if (dir & 8)
    {
        dx = 1;
        dy = 0;
    }
    else
    {
        return;
    }

    nx = (short)player_x + dx;
    ny = (short)player_y + dy;

    if (!can_move(nx, ny))
        return;

    if (nx == (short)wizard_x && ny == (short)wizard_y)
    {
        game_over = 1;
        return;
    }

    player_x = (uint8_t)nx;
    player_y = (uint8_t)ny;

    _XL_DELETE(old_x, old_y);
    draw_player(player_x, player_y);
}

/* --------------------------------------------------------------------------------------- */
/* Wizard movement (BFS chase)                                                              */
/* --------------------------------------------------------------------------------------- */

static short wizard_dirs[8][2] =
{
    {-1, -1},
    { 1, -1},
    {-1,  1},
    { 1,  1},
    {-1,  0},
    { 1,  0},
    { 0, -1},
    { 0,  1}
};

static void wizard_step(void)
{
    uint16_t i;
    uint16_t head;
    uint16_t tail;
    uint16_t cur;
    uint16_t idx;
    uint16_t wiz_idx;
    uint16_t best;
    uint8_t cx;
    uint8_t cy;
    short nx;
    short ny;
    short bestx;
    short besty;

    if (wizard_x == player_x && wizard_y == player_y)
    {
        game_over = 1;
        return;
    }

    head = 0;
    tail = 0;

    for (i = 0; i < MAP_W * MAP_H; ++i)
    {
        bfs_dist[i] = 0xFFFF;
    }

    if (!can_move((short)player_x, (short)player_y))
        return;

    bfs_dist[player_y * MAP_W + player_x] = 0;
    bfs_queue[tail++] = (uint16_t)(player_y * MAP_W + player_x);

    while (head < tail)
    {
        cur = bfs_queue[head++];

        cx = (uint8_t)(cur % MAP_W);
        cy = (uint8_t)(cur / MAP_W);

        if (cx == wizard_x && cy == wizard_y)
            break;

        for (i = 0; i < 8; ++i)
        {
            nx = (short)cx + wizard_dirs[i][0];
            ny = (short)cy + wizard_dirs[i][1];

            if (!can_move(nx, ny))
                continue;

            idx = (uint16_t)((uint16_t)ny * MAP_W + (uint16_t)nx);

            if (bfs_dist[idx] != 0xFFFF)
                continue;

            bfs_dist[idx] = bfs_dist[cur] + 1;
            bfs_queue[tail++] = idx;
        }
    }

    wiz_idx = (uint16_t)((uint16_t)wizard_y * MAP_W + (uint16_t)wizard_x);

    if (bfs_dist[wiz_idx] == 0xFFFF)
        return;

    best = 0xFFFF;
    bestx = (short)wizard_x;
    besty = (short)wizard_y;

    for (i = 0; i < 8; ++i)
    {
        nx = (short)wizard_x + wizard_dirs[i][0];
        ny = (short)wizard_y + wizard_dirs[i][1];

        if (!can_move(nx, ny))
            continue;

        idx = (uint16_t)((uint16_t)ny * MAP_W + (uint16_t)nx);

        if (bfs_dist[idx] < best)
        {
            best = bfs_dist[idx];
            bestx = nx;
            besty = ny;
        }
    }

    if (best == 0xFFFF)
        return;

    _XL_DELETE(wizard_x, wizard_y);

    wizard_x = (uint8_t)bestx;
    wizard_y = (uint8_t)besty;

    draw_wizard();

    if (wizard_x == player_x && wizard_y == player_y)
        game_over = 1;
}

/* --------------------------------------------------------------------------------------- */
/* Dragon logic                                                                             */
/* --------------------------------------------------------------------------------------- */

static short dragon_side_x(const Dragon *d)
{
    return d->facing ? (short)d->x : (short)d->x - 1;
}

static uint8_t player_not_near_mouth(short x, short y, uint8_t facing)
{
    short mx = facing ? x : x - 1;
    short my = y;

    return !adjacent4((short)player_x, (short)player_y, mx, my);
}

static uint8_t dragon_spots_free(short x, short y)
{
    short xs[4];
    short ys[4];
    uint8_t i;

    xs[0] = x - 1;
    xs[1] = x;
    xs[2] = x - 1;
    xs[3] = x;

    ys[0] = y;
    ys[1] = y;
    ys[2] = y + 1;
    ys[3] = y + 1;

    for (i = 0; i < 4; ++i)
    {
        if (xs[i] < 1 || ys[i] < 1 ||
            xs[i] >= (short)XSize - 1 || ys[i] >= (short)YSize - 1)
        {
            return 0;
        }

        if (is_castle(xs[i], ys[i]))
            return 0;

        if (xs[i] == (short)player_x && ys[i] == (short)player_y)
            return 0;

        if (xs[i] == (short)wizard_x && ys[i] == (short)wizard_y)
            return 0;

        if (tile_is_dragon(xs[i], ys[i]))
            return 0;
    }

    return 1;
}

static void spawn_dragon(void)
{
    uint8_t i;
    uint8_t attempt;
    short x;
    short y;
    short facing;

    if (XSize < 6 || YSize < 6)
        return;

    for (attempt = 0; attempt < 24; ++attempt)
    {
        x = 4 + (short)(_XL_RAND() % (uint16_t)(XSize - 6));
        y = 4 + (short)(_XL_RAND() % (uint16_t)(YSize - 6));
        facing = (short)(_XL_RAND() & 1);

        if (!dragon_spots_free(x, y))
            continue;

        if (!player_not_near_mouth(x, y, (uint8_t)facing))
        {
            facing ^= 1;

            if (!player_not_near_mouth(x, y, (uint8_t)facing))
                continue;
        }

        for (i = 0; i < MAX_DRAGONS; ++i)
        {
            if (!dragons[i].alive)
            {
                dragons[i].x = (uint8_t)x;
                dragons[i].y = (uint8_t)y;
                dragons[i].facing = (uint8_t)facing;
                dragons[i].alive = 1;

                draw_dragon(&dragons[i]);

                return;
            }
        }
    }
}

static void update_flip(void)
{
    uint8_t alive;
    uint8_t i;
    uint16_t pick;

    alive = 0;

    for (i = 0; i < MAX_DRAGONS; ++i)
    {
        if (dragons[i].alive)
            ++alive;
    }

    if (flip_timer == 0)
    {
        if (alive > 0)
        {
            pick = (uint16_t)(_XL_RAND() % (uint16_t)alive);

            for (i = 0; i < MAX_DRAGONS; ++i)
            {
                if (dragons[i].alive)
                {
                    if (pick == 0)
                    {
                        dragons[i].facing ^= 1;
                        draw_dragon(&dragons[i]);
                        break;
                    }
                    --pick;
                }
            }
        }

        flip_timer = (uint16_t)(3 + _XL_RAND() % 8);
    }
    else
    {
        --flip_timer;
    }
}

static void update_respawn(void)
{
    uint8_t alive;
    uint8_t i;

    alive = 0;

    for (i = 0; i < MAX_DRAGONS; ++i)
    {
        if (dragons[i].alive)
            ++alive;
    }

    if (alive < initial_dragons)
    {
        if (respawn_timer == 0)
        {
            spawn_dragon();
            respawn_timer = 90;
        }
        else
        {
            --respawn_timer;
        }
    }
    else
    {
        respawn_timer = 0;
    }
}

static void kill_dragons(void)
{
    uint8_t i;
    short bx;
    short by;

    for (i = 0; i < MAX_DRAGONS; ++i)
    {
        if (!dragons[i].alive)
            continue;

        bx = dragon_side_x(&dragons[i]);
        by = (short)dragons[i].y + 1;

        if (adjacent4((short)player_x, (short)player_y, bx, by))
        {
            dragons[i].alive = 0;

            delete_dragon(i);

            if (score < 255)
                ++score;

            respawn_timer = 90;
        }
    }
}

static void check_death(void)
{
    uint8_t i;
    short mx;
    short my;

    if (wizard_x == player_x && wizard_y == player_y)
    {
        game_over = 1;
        return;
    }

    for (i = 0; i < MAX_DRAGONS; ++i)
    {
        if (!dragons[i].alive)
            continue;

        mx = dragon_side_x(&dragons[i]);
        my = (short)dragons[i].y;

        if (adjacent4((short)player_x, (short)player_y, mx, my))
        {
            game_over = 1;
            return;
        }
    }
}

/* --------------------------------------------------------------------------------------- */
/* Reset                                                                                    */
/* --------------------------------------------------------------------------------------- */

static void reset_game(void)
{
    uint8_t i;
    short x;
    short y;
    uint8_t found;

    if (scene_drawn)
    {
        _XL_DELETE(player_x, player_y);
        _XL_DELETE(wizard_x, wizard_y);

        for (i = 0; i < MAX_DRAGONS; ++i)
        {
            if (dragons[i].alive)
                delete_dragon(i);
        }
    }

    game_over = 0;
    game_over_drawn = 0;
    fire_held = 0;
    score = 0;
    frames = 0;
    wizard_timer = 0;
    wizard_interval = 12;

    flip_timer = 90;
    respawn_timer = 0;

    for (i = 0; i < MAX_DRAGONS; ++i)
    {
        dragons[i].alive = 0;
    }

    initial_dragons = (uint8_t)(((short)XSize / 5) * ((short)YSize / 10));

    if (initial_dragons == 0)
        initial_dragons = 1;

    if (initial_dragons > MAX_DRAGONS)
        initial_dragons = MAX_DRAGONS;

    player_x = (uint8_t)((short)XSize / 2 - 1);
    player_y = 2;

    if (!basic_free((short)player_x, (short)player_y))
    {
        player_x = 1;
        player_y = 1;

        if (!basic_free((short)player_x, (short)player_y))
        {
            found = 0;

            for (y = 1; y < (short)YSize - 1 && !found; ++y)
            {
                for (x = 1; x < (short)XSize - 1; ++x)
                {
                    if (basic_free(x, y))
                    {
                        player_x = (uint8_t)x;
                        player_y = (uint8_t)y;
                        found = 1;
                        break;
                    }
                }
            }

            if (!found)
            {
                player_x = 1;
                player_y = 1;
            }
        }
    }

    wizard_x = 1;
    wizard_y = (YSize > 2) ? (uint8_t)(YSize - 2) : 1;

    if (!basic_free((short)wizard_x, (short)wizard_y) ||
        (wizard_x == player_x && wizard_y == player_y))
    {
        wizard_x = (XSize > 2) ? (uint8_t)(XSize - 2) : 1;
        wizard_y = (YSize > 2) ? (uint8_t)(YSize - 2) : 1;
    }

    if (!basic_free((short)wizard_x, (short)wizard_y) ||
        (wizard_x == player_x && wizard_y == player_y))
    {
        wizard_x = (XSize > 2) ? (uint8_t)(XSize - 2) : 1;
        wizard_y = 1;
    }

    if (!basic_free((short)wizard_x, (short)wizard_y) ||
        (wizard_x == player_x && wizard_y == player_y))
    {
        wizard_x = 1;
        wizard_y = 1;
    }

    if (wizard_x == player_x && wizard_y == player_y)
    {
        found = 0;

        for (y = 1; y < (short)YSize - 1 && !found; ++y)
        {
            for (x = 1; x < (short)XSize - 1; ++x)
            {
                if (basic_free(x, y) &&
                    !(x == (short)player_x && y == (short)player_y))
                {
                    wizard_x = (uint8_t)x;
                    wizard_y = (uint8_t)y;
                    found = 1;
                    break;
                }
            }
        }

        if (!found)
        {
            wizard_x = player_x;
            wizard_y = player_y;
        }
    }

    draw_background();

    for (i = 0; i < initial_dragons; ++i)
    {
        spawn_dragon();
    }

    draw_wizard();
    draw_player(player_x, player_y);

    scene_drawn = 1;
}

/* --------------------------------------------------------------------------------------- */
/* Main game loop                                                                           */
/* --------------------------------------------------------------------------------------- */

int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_SOUND();
    _XL_INIT_INPUT();

    _XL_CLEAR_SCREEN();

    reset_game();

    while (1)
    {
        if (game_over)
        {
            if (!game_over_drawn)
            {
                _XL_DELETE(player_x, player_y);
                _XL_DRAW(player_x, player_y, PLAYER_TILE, _XL_RED);
                game_over_drawn = 1;
            }

            _XL_SLEEP(1);
            _XL_WAIT_FOR_INPUT();

            reset_game();

            continue;
        }

        ++frames;

        if ((frames % 2) == 0 && wizard_interval > 3)
        {
            --wizard_interval;
        }

        update_flip();
        update_respawn();
        rebuild_passable();

        {
            uint8_t input;
            uint8_t fire;
            uint8_t fire_pressed;

            input = (uint8_t)_XL_INPUT();
            fire = (uint8_t)_XL_FIRE(input);

            fire_pressed = (fire != 0) && (fire_held == 0);
            fire_held = fire;

            move_player(get_direction(input));

            if (game_over)
                continue;

            if (fire_pressed)
                kill_dragons();

            ++wizard_timer;

            if (wizard_timer >= wizard_interval)
            {
                wizard_timer = 0;
                wizard_step();
            }

            if (game_over)
                continue;

            check_death();
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}



