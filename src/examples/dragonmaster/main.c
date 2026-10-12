#include "cross_lib.h"
#include "images.h"

#define MAX_DRAGONS 32
#define MIN_DRAGONS  6

#define MAP_W       64
#define MAP_H       64

#define MIN_WIZARD_INTERVAL        8
#define INITIAL_WIZARD_INTERVAL  140

#define RESPAWN_TIME 240

#define INITIAL_LIVES 3

#define DRAGON_POINTS 5
#define KING_POINTS 1000

#define MAX_LIVES 9

#define INITIAL_LEVEL 1
#define LAST_LEVEL    4

#define BONUS_LIFE 1000

static uint8_t lives;
static uint16_t record;
static uint8_t bonus;

static uint8_t level;

static uint8_t extra_life_counter;

uint8_t player_color[] = {_XL_WHITE, _XL_CYAN};
uint8_t wizard_color[] = {_XL_WHITE, _XL_YELLOW};

typedef struct
{
    uint8_t x;
    uint8_t y;
    uint8_t facing;
    uint8_t alive;
    uint8_t color;
} Dragon;

static Dragon dragons[MAX_DRAGONS];

static uint8_t passable[MAP_W][MAP_H];

static uint8_t level_dragons;

static uint8_t player_x, player_y;
static uint8_t wizard_x, wizard_y;

// static uint8_t game_over;
// static uint8_t game_over_drawn;
static uint8_t scene_drawn;

static uint16_t frames;
static uint16_t wizard_timer;
static uint8_t wizard_interval;

static uint16_t flip_timer;
static uint16_t respawn_timer;

static uint16_t score;
// static uint8_t fire_held;

static uint8_t king_alive;

/* King dragon occupies a 2x2 area:
 *   (king_x, king_y)       (king_x+1, king_y)
 *   (king_x, king_y+1)     (king_x+1, king_y+1)
 */
static short king_x, king_y;

static uint16_t steps;

/* --------------------------------------------------------------------------------------- */
/* Drawing helpers                                                                          */
/* --------------------------------------------------------------------------------------- */

static void _draw_castle(uint8_t x, uint8_t y)
{
    _XL_DRAW(x-1,y,   CASTLE_NW_TILE,_XL_WHITE);
    _XL_DRAW(x,  y,   CASTLE_NE_TILE,_XL_WHITE);
    _XL_DRAW(x-1,y+1, CASTLE_SW_TILE,_XL_WHITE);
    _XL_DRAW(x,y+1,   CASTLE_SE_TILE,_XL_WHITE);
    _XL_DRAW(x-2,y+1, BRIDGE_UP_TILE,_XL_WHITE);
} 

static void draw_castle(void)
{
    _draw_castle(XSize/2,YSize/2);
    // _XL_DRAW(XSize/2-1,YSize/2,  CASTLE_NW_TILE,_XL_WHITE);
    // _XL_DRAW(XSize/2,  YSize/2,  CASTLE_NE_TILE,_XL_WHITE);
    // _XL_DRAW(XSize/2-1,YSize/2+1,CASTLE_SW_TILE,_XL_WHITE);
    // _XL_DRAW(XSize/2,YSize/2+1,  CASTLE_SE_TILE,_XL_WHITE);
    // _XL_DRAW(XSize/2-2,YSize/2+1, BRIDGE_UP_TILE,_XL_WHITE);
}

static void draw_left_dragon(uint8_t x, uint8_t y, uint8_t color)
{
    _XL_DRAW(x-1,y,  LEFT_NW_TILE,color);
    _XL_DRAW(x,  y,  LEFT_NE_TILE,color);
    _XL_DRAW(x-1,y+1,LEFT_SW_TILE,color);
    _XL_DRAW(x,y+1,  LEFT_SE_TILE,color);
}

static void draw_dead_dragon(uint8_t i)
{
    const Dragon *d = &dragons[i];
    uint8_t xl = (uint8_t)((short)d->x - 1);
    uint8_t yh = (uint8_t)(d->y + 1);

    _XL_DRAW(xl,d->y,     DEAD_BOSS_NW_TILE,_XL_WHITE);
    _XL_DRAW(d->x,  d->y, DEAD_BOSS_NE_TILE,_XL_WHITE);
    _XL_DRAW(xl,yh,       DEAD_BOSS_SW_TILE,_XL_WHITE);
    _XL_DRAW(d->x,yh,     DEAD_BOSS_SE_TILE,_XL_WHITE);
}

static void draw_dead_king_dragon(void)
{
    _XL_DRAW(king_x,king_y,     DEAD_BOSS_NW_TILE,_XL_WHITE);
    _XL_DRAW(king_x+1, king_y, DEAD_BOSS_NE_TILE,_XL_WHITE);
    _XL_DRAW(king_x,king_y+1,       DEAD_BOSS_SW_TILE,_XL_WHITE);
    _XL_DRAW(king_x+1,king_y+1,     DEAD_BOSS_SE_TILE,_XL_WHITE);
}



static void _draw_king_dragon(uint8_t king_x, uint8_t king_y)
{
    _XL_DRAW((uint8_t)king_x,       (uint8_t)king_y,   BOSS_NW_TILE,_XL_CYAN);
    _XL_DRAW((uint8_t)king_x + 1,   (uint8_t)king_y,   BOSS_NE_TILE, _XL_CYAN);
    _XL_DRAW((uint8_t)king_x,       (uint8_t)king_y+1, BOSS_SW_TILE,_XL_CYAN);
    _XL_DRAW((uint8_t)king_x + 1,   (uint8_t)king_y+1, BOSS_SE_TILE, _XL_CYAN);
}

static void draw_king_dragon(void)
{
    _draw_king_dragon(king_x, king_y);
    // _XL_DRAW((uint8_t)king_x,       (uint8_t)king_y,   BOSS_NW_TILE,_XL_CYAN);
    // _XL_DRAW((uint8_t)king_x + 1,   (uint8_t)king_y,   BOSS_NE_TILE, _XL_CYAN);
    // _XL_DRAW((uint8_t)king_x,       (uint8_t)king_y+1, BOSS_SW_TILE,_XL_CYAN);
    // _XL_DRAW((uint8_t)king_x + 1,   (uint8_t)king_y+1, BOSS_SE_TILE, _XL_CYAN);
}

static void delete_king_dragon(void)
{
    _XL_DELETE((uint8_t)king_x,       (uint8_t)king_y);
    _XL_DELETE((uint8_t)king_x + 1,   (uint8_t)king_y);
    _XL_DELETE((uint8_t)king_x,       (uint8_t)king_y+1);
    _XL_DELETE((uint8_t)king_x + 1,   (uint8_t)king_y+1);
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

static void draw_player_with_color(uint8_t x, uint8_t y, uint8_t color)
{
    _XL_DRAW(x,y,PLAYER_TILE, color);
}

/* --------------------------------------------------------------------------------------- */
/* Small helpers                                                                            */
/* --------------------------------------------------------------------------------------- */


static uint8_t is_wall(short x, short y)
{
    if ((x == 0) || (y == 0) || (x == (short)XSize - 1) || (y == (short)YSize - 1))
        return 1;
    return 0;
}

static uint8_t is_castle(short x, short y)
{
    short cx = (short)XSize / 2;
    short cy = (short)YSize / 2;

    if ((x == cx-2 || x == cx - 1 || x == cx) && (y == cy || y == cy + 1))
        return 1;

    return 0;
}

static uint8_t is_king(short x, short y)
{
    if (!king_alive)
        return 0;

    if ((x == king_x || x == king_x + 1) &&
        (y == king_y || y == king_y + 1))
        return 1;

    return 0;
}

static uint8_t basic_free(short x, short y)
{
    if (is_wall(x, y)) return 0;
    if (is_castle(x, y)) return 0;
    if (is_king(x, y)) return 0;
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

    for (i = 0; i < level_dragons; ++i)
    {
        if (dragon_tile_equals(&dragons[i], x, y))
            return 1;
    }

    return 0;
}


static void build_initial_passable(void)
{
    uint8_t x, y;
    // short xs[4], ys[4];

    for (y = 0; y < (uint8_t)YSize && y < MAP_H; ++y)
    {
        for (x = 0; x < (uint8_t)XSize && x < MAP_W; ++x)
        {
            passable[y][x] = (!is_wall((short)x, (short)y) &&
                              !is_castle((short)x, (short)y) &&
                              !is_king((short)x, (short)y)) ? 1 : 0;
        }
    }
}


static void update_single_dragon_passable(uint8_t i)
{
    uint8_t xs[4], ys[4];
    uint8_t j;

    xs[0] = dragons[i].x - 1;
    xs[1] = dragons[i].x;
    xs[2] = dragons[i].x - 1;
    xs[3] = dragons[i].x;

    ys[0] = dragons[i].y;
    ys[1] = dragons[i].y;
    ys[2] = dragons[i].y + 1;
    ys[3] = dragons[i].y + 1;

    for (j = 0; j < 4; ++j)
    {
        if (xs[j] >= 0 && ys[j] >= 0 &&
            xs[j] < (short)XSize && ys[j] < (short)YSize &&
            xs[j] < MAP_W && ys[j] < MAP_H)
        {
            passable[ys[j]][xs[j]] = !dragons[i].alive;
        }
    }
}


static void init_dragon_passable(void)
{
    uint8_t i;

    for (i = 0; i < level_dragons; ++i)
    {
        update_single_dragon_passable(i);
    }
}

static uint8_t can_move(short x, short y)
{
    if (x <= 0 || y <= 0) return 0;
    if (x >= (short)XSize-1 || y >= (short)YSize-1) return 0;
    if (x >= MAP_W || y >= MAP_H) return 0;

    return passable[y][x] != 0;
}

static uint8_t all_dragons_dead(void)
{
    uint8_t i;

    for (i = 0; i < level_dragons; ++i)
    {
        if (dragons[i].alive)
            return 0;
    }

    return 1;
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

static void draw_wizard_with_color(uint8_t color)
{
    _XL_DRAW(wizard_x, wizard_y, CHASE_TILE, color);
}

static void draw_dragon(const Dragon *d)
{
    if (!d->alive)
        return;

    if (d->facing == 0)
        draw_left_dragon(d->x, d->y, d->color);
    else
        draw_right_dragon(d->x, d->y, d->color);
    
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

    player_x = (uint8_t)nx;
    player_y = (uint8_t)ny;

    _XL_DELETE(old_x, old_y);
    ++steps;
}

/* --------------------------------------------------------------------------------------- */
/* Wizard movement (simple greedy + random fallback)                                        */
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

static void show_player_death(void)
{
    draw_player_with_color(player_x, player_y, _XL_WHITE);
    _XL_EXPLOSION_SOUND();
    _XL_SLEEP(1);
    _XL_TICK_SOUND();
    _XL_DRAW(player_x,player_y,DEAD_1_TILE, _XL_WHITE);
    _XL_SLEEP(1);
    _XL_TICK_SOUND();
    _XL_DRAW(player_x,player_y,DEAD_2_TILE, _XL_WHITE);
    _XL_SLEEP(1);
    _XL_DELETE(player_x,player_y);

}

static void reset_player(void)
{
    uint8_t found;
    uint8_t x,y;
    
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
}

static void display_lives(void)
{
    uint8_t i;
    
    for(i=1;i<=MAX_LIVES;++i)
    {
        if(i<=lives)
        {
            _XL_DRAW(XSize/2-3+i,0,PLAYER_TILE,_XL_WHITE);
        }
        else
        {
            _XL_DRAW(XSize/2-3+i,0,WALL_TILE,_XL_WHITE);
        }
    }
}

static void player_dies(void)
{
    --lives;
    show_player_death();
    reset_player();
    draw_player(player_x,player_y);
    display_lives();
}



static void reset_wizard(void)
{
    uint8_t x, y, found;
    
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
}


static void check_wizard_player_collision(void)
{
    if (wizard_x == player_x && wizard_y == player_y)
    {
        player_dies();
        reset_wizard();
    }
}


static void wizard_step(void)
{
    short dx;
    short dy;
    short nx;
    short ny;
    uint8_t i;
    uint8_t tried;

    /* Desired direction toward the player */
    dx = (short)player_x - (short)wizard_x;
    dy = (short)player_y - (short)wizard_y;

    if (dx > 0) dx = 1;
    else if (dx < 0) dx = -1;
    else dx = 0;

    if (dy > 0) dy = 1;
    else if (dy < 0) dy = -1;
    else dy = 0;

    nx = (short)wizard_x + dx;
    ny = (short)wizard_y + dy;

    if (can_move(nx, ny))
    {
        _XL_DELETE(wizard_x, wizard_y);
        wizard_x = (uint8_t)nx;
        wizard_y = (uint8_t)ny;
    }
    else
    {
        /* Blocked: pick a random passable neighbor */
        tried = 0;

        while (tried < 8)
        {
            i = (uint8_t)(_XL_RAND() % 8);
            nx = (short)wizard_x + wizard_dirs[i][0];
            ny = (short)wizard_y + wizard_dirs[i][1];

            if (can_move(nx, ny))
            {
                _XL_DELETE(wizard_x, wizard_y);
                wizard_x = (uint8_t)nx;
                wizard_y = (uint8_t)ny;
                break;
            }

            ++tried;
        }
    }
}

/* --------------------------------------------------------------------------------------- */
/* Dragon logic                                                                             */
/* --------------------------------------------------------------------------------------- */

static short dragon_side_x(const Dragon *d)
{
    return d->facing ? (short)d->x : (short)d->x - 1;
}

static short dragon_belly_x(const Dragon *d)
{
    return d->facing ? (short)d->x + 1 : (short)d->x - 2;
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

        if (is_king(xs[i], ys[i]))
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

        for (i = 0; i < level_dragons; ++i)
        {
            if (!dragons[i].alive)
            {
                dragons[i].x = (uint8_t)x;
                dragons[i].y = (uint8_t)y;
                dragons[i].facing = (uint8_t)facing;
                dragons[i].alive = 1;
                dragons[i].color = (uint8_t)(_XL_RAND() & 1) ? _XL_RED : _XL_WHITE;

                draw_dragon(&dragons[i]);
                update_single_dragon_passable(i);
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

    for (i = 0; i < level_dragons; ++i)
    {
        if (dragons[i].alive)
            ++alive;
    }

    if (flip_timer == 0)
    {
        if (alive > 0)
        {
            pick = (uint16_t)(_XL_RAND() % (uint16_t)alive);

            for (i = 0; i < level_dragons; ++i)
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

        flip_timer = (uint16_t)(30 + (_XL_RAND()&31));
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

    for (i = 0; i < level_dragons; ++i)
    {
        if (dragons[i].alive)
            ++alive;
    }

    if (alive < level_dragons)
    {
        if (respawn_timer == 0)
        {
            spawn_dragon();
            _XL_TOCK_SOUND();
            respawn_timer = RESPAWN_TIME;
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


static void display_score(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(0,0,4,score);
}

uint8_t compute_bonus(uint8_t steps)
{
    if(steps < XSize/2 * YSize)
    {
        bonus = 50;
    }
    else if(steps < (short) XSize * YSize)
    {
        bonus = 30;
    }
    else if(steps < 2 * (short) XSize * YSize)
    {
        bonus = 20;
    }
    else
    {
        bonus = 0;
    }
    return bonus;
}


static void handle_extra_life(void)
{
    if(score> BONUS_LIFE*extra_life_counter)
    {
        ++extra_life_counter;
        ++lives;
        display_lives();
        _XL_ZAP_SOUND();
    }
}


static void increase_score(uint8_t points)
{
    score+=points;
    display_score();
    handle_extra_life();
}


static void kill_dragons(void)
{
    uint8_t i;
    uint8_t killed;
    short bx;
    short by;

    killed = 0;

    /* Kill regular dragons adjacent to their mouth (bottom row) */
    for (i = 0; i < level_dragons; ++i)
    {
        if (!dragons[i].alive)
            continue;

        bx = dragon_belly_x(&dragons[i]);
        by = (short)dragons[i].y + 1;

        // if (adjacent4((short)player_x, (short)player_y, bx, by))
        if((player_x==bx) && (player_y==by))
        {
            dragons[i].alive = 0;

            draw_dead_dragon(i);
            _XL_SHOOT_SOUND();
            _XL_SLOW_DOWN(4*_XL_SLOW_DOWN_FACTOR);

            increase_score(DRAGON_POINTS);

            respawn_timer = 90;
            killed = 1;
            delete_dragon(i);
            update_single_dragon_passable(i);
        }
    }

    /* Kill king dragon only if all regular dragons are dead */
    if (king_alive && all_dragons_dead())
    {
        /* King mouth is the bottom row of its 2x2 area */
        // if (adjacent4((short)player_x, (short)player_y, king_x,     king_y + 1))
            
        
        if ((player_x == king_x-1) && (player_y == king_y + 1))
        {
            king_alive = 0;
            draw_dead_king_dragon();
            _XL_SHOOT_SOUND();
            _XL_SLOW_DOWN(10*_XL_SLOW_DOWN_FACTOR);

            _XL_DRAW(XSize/2-2,YSize/2+1, BRIDGE_DOWN_TILE,_XL_WHITE);
            
            
            killed = 1;
            // game_over = 1;
            delete_king_dragon();
            _XL_TICK_SOUND();
            _XL_DRAW(XSize/2-2,YSize/2+1, BRIDGE_DOWN_TILE,_XL_WHITE);
            _XL_SLEEP(1);
            _XL_TOCK_SOUND();
            _XL_DELETE(player_x, player_y);
            draw_player(player_x+1,player_y);
            _XL_SLEEP(1);
            _XL_TOCK_SOUND();
            _XL_DELETE(player_x+1, player_y);
            draw_player(player_x+2,player_y);
            _XL_SLEEP(1);
            _XL_TOCK_SOUND();
            _XL_DELETE(player_x+2, player_y);
            draw_player(player_x+3,player_y);
            _XL_SLEEP(1);
            _XL_TOCK_SOUND();
            _XL_DELETE(player_x+3, player_y);
            
            
            
            _XL_SLEEP(1);
            _XL_SET_TEXT_COLOR(_XL_CYAN);
            _XL_PRINT(XSize/2-6, YSize-5, "KING POINTS ");
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINTD(XSize/2-6+12, YSize-5, 3, KING_POINTS*level);

            increase_score(KING_POINTS*level);
            _XL_SLEEP(1);
            
            bonus = compute_bonus(steps);
            _XL_SET_TEXT_COLOR(_XL_CYAN);
            _XL_PRINT(XSize/2-3, YSize-3, "BONUS ");
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINTD(XSize/2+3, YSize-3, 2, bonus);
            _XL_SLEEP(1);
            
            increase_score(bonus);

            _XL_SLEEP(1);
            _XL_WAIT_FOR_INPUT();
            _XL_CLEAR_SCREEN();
        }
    }

    // if (killed)
    // {
        // init_dragon_passable();
    // }
}


static void check_dragon_player_collissions(void)
{
    uint8_t i;
    short mx;
    short my;

    if (wizard_x == player_x && wizard_y == player_y)
    {
        player_dies();
        return;
    }

    for (i = 0; i < level_dragons; ++i)
    {
        if (!dragons[i].alive)
            continue;

        mx = dragon_side_x(&dragons[i]);
        my = (short)dragons[i].y;

        if (adjacent4((short)player_x, (short)player_y, mx, my))
        {
            player_dies();
            return;
        }
    }

    /* King dragon kills the player on its top row (mouth side) */
    if (king_alive)
    {
        if (adjacent4((short)player_x, (short)player_y, king_x,     king_y))
        {
            player_dies();
            return;
        }
    }
}

/* --------------------------------------------------------------------------------------- */
/* Reset                                                                                    */
/* --------------------------------------------------------------------------------------- */



static void reset_game(void)
{
    level = INITIAL_LEVEL;
    lives = INITIAL_LIVES;
    score = 0;
}

static void init_level(void)
{
    uint8_t i;

    if (scene_drawn)
    {
        _XL_DELETE(player_x, player_y);
        _XL_DELETE(wizard_x, wizard_y);

        for (i = 0; i < level_dragons; ++i)
        {
            if (dragons[i].alive)
                delete_dragon(i);
        }

        if (king_alive)
            delete_king_dragon();
    }

    // game_over_drawn = 0;
    // fire_held = 0;
    frames = 0;
    wizard_timer = 0;
    wizard_interval = INITIAL_WIZARD_INTERVAL / level;

    flip_timer = 12;
    respawn_timer = 0;

    king_alive = 1;
    king_x = (short)XSize / 2 - 4;
    king_y = (short)YSize / 2;

    for (i = 0; i < level_dragons; ++i)
    {
        dragons[i].alive = 0;
    }

    level_dragons = (1+level) *(XSize / 16) * (YSize/8);

    if (level_dragons < MIN_DRAGONS)
        level_dragons = MIN_DRAGONS;

    if (level_dragons > MAX_DRAGONS)
        level_dragons = MAX_DRAGONS;

    reset_player();

    reset_wizard();

    draw_background();

    for (i = 0; i < level_dragons; ++i)
    {
        spawn_dragon();
    }

    draw_king_dragon();

    build_initial_passable();
    init_dragon_passable();

    draw_wizard();
    draw_player(player_x, player_y);

    scene_drawn = 1;
}

#if YSize<=20
    #define KING_Y (YSize-3)

#else
    #define KING_Y (YSize/2+8)
#endif

static void display_record(uint8_t x)
{
    
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(x,0,"HI");

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(x+2,0,4,record);
}


void intro_screen(void)
{
    _XL_CLEAR_SCREEN();
    
    draw_wall();
    _draw_castle(XSize/2, 1);
    
    display_record(XSize/2-3);

    #if YSize>=20
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(XSize/2-6, YSize/2-7, "DRAGONMASTER");


    _XL_SET_TEXT_COLOR(_XL_WHITE);

    _XL_PRINT(XSize/2-1, YSize/2-5, "BY");

    _XL_PRINT(XSize/2-7, YSize/2-3, "FABRIZIO CARUSO");
    
    _XL_PRINT(XSize/2-7, YSize/2+3, "ORIGINALLY BY");
    _XL_PRINT(XSize/2-6, YSize/2+5, "DAVID BERDAN");

    _XL_SET_TEXT_COLOR(_XL_YELLOW);

    _XL_PRINT(XSize/2-6, YSize/2-1, "AIDED BY AI");
    #else
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(XSize/2-6, 3, "DRAGONMASTER");


    _XL_SET_TEXT_COLOR(_XL_WHITE);

    _XL_PRINT(XSize/2-7, 5, "FABRIZIO CARUSO");
    
    _XL_PRINT(XSize/2-7, 8,  "ORIGINALLY BY");
    _XL_PRINT(XSize/2-6, 10, "DAVID BERDAN");

    _XL_SET_TEXT_COLOR(_XL_YELLOW);

    _XL_PRINT(XSize/2-6, 12, "AIDED BY AI");
    #endif

    _draw_king_dragon(XSize/2+1,KING_Y);
    draw_player(XSize/2-2,KING_Y+1);

    #if XSize>=23
    {
        uint8_t i;
        
        for(i=3;i<=YSize-1-3;i+=4)
        {
            draw_right_dragon(2,i,(i/4)&1 ? _XL_WHITE : _XL_RED);
            draw_left_dragon(XSize-1-1,i,(i/4)&1 ? _XL_RED : _XL_WHITE);
        }
    }
    #elif XSize>16

        draw_right_dragon(2,2,       _XL_WHITE);
        draw_left_dragon(XSize-1-1,YSize-1-3,_XL_RED);
        draw_right_dragon(2,YSize-1-3,       _XL_WHITE);
        draw_left_dragon(XSize-1-1,2,_XL_RED);
    #else
        draw_right_dragon(2,1,       _XL_WHITE);
        draw_left_dragon(XSize-1-1,YSize-1-2,_XL_RED);
        draw_right_dragon(2,YSize-1-2,       _XL_WHITE);
        draw_left_dragon(XSize-1-1,1,_XL_RED);
    #endif

    _XL_SLEEP(1);
    _XL_WAIT_FOR_INPUT();
    
    _XL_CLEAR_SCREEN();
}


void display_level(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    
    _XL_PRINTD(XSize/2-1,YSize-1,1,level);
}


/* --------------------------------------------------------------------------------------- */
/* Main game loop                                                                           */
/* --------------------------------------------------------------------------------------- */


int main(void)
{
    uint8_t input;
    uint8_t fire;
    
    _XL_INIT_GRAPHICS();
    _XL_INIT_SOUND();
    _XL_INIT_INPUT();

    _XL_CLEAR_SCREEN();
    record = 0;

    while (1)
    {
        intro_screen();
        
        reset_game();
        steps = 0;
        extra_life_counter = 1;

        while(lives && level<=LAST_LEVEL)
        {
            init_level();
            display_score();
            display_record(XSize-1-3-2);
            display_lives();
            display_level();
            while(lives && king_alive)
                {
                ++frames;
                draw_player_with_color(player_x, player_y,player_color[frames&1]);
                draw_wizard_with_color(wizard_color[frames&1]);

                if (wizard_interval > MIN_WIZARD_INTERVAL - level)
                {
                    --wizard_interval;
                }

                update_flip();
                update_respawn();
                check_dragon_player_collissions();

                input = (uint8_t)_XL_INPUT();
                fire = (uint8_t)_XL_FIRE(input);

                move_player(get_direction(input));
                check_dragon_player_collissions();

                if (!lives)
                    continue;

                if (fire)
                    kill_dragons();

                ++wizard_timer;

                check_wizard_player_collision();
                if (wizard_timer >= wizard_interval)
                {
                    wizard_timer = 0;
                    wizard_step();
                }
                check_wizard_player_collision();

                if (!lives)
                    continue;

                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                
            }
        

            if(king_alive)
            {
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_PRINT(XSize/2-4,0, "YOU LOST");
            }
            else
            {
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(XSize/2-4,0, "YOU WON");
            }
            ++level;
        }
        _XL_SLEEP(1);
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(XSize/2-4,YSize/2-2,   "GAME OVER");
        _XL_SLEEP(1);
        if(score>record)
        {
            record = score;
            _XL_SET_TEXT_COLOR(_XL_GREEN);
            _XL_PRINT(XSize/2-5,YSize-1, "NEW RECORD");
            display_record(XSize-1-3-2);
            _XL_SLEEP(1);
        }
        _XL_WAIT_FOR_INPUT();
        _XL_PRINT(XSize/2-4,YSize/2-2,   "          ");


    }

    return 0;
}

