#include "cross_lib.h"

/* ============================================================
   PAC-MAN CLONE — Cross-Lib API only, no heap, uint8_t/uint16_t
   Ghosts do NOT erase dots. Dots remain visible when ghosts move away.
   ============================================================ */

#define MW 20
#define MH 15

#define T_EMPTY   0
#define T_WALL    1
#define T_DOT     2
#define T_PELLET 3

#define DIR_NONE  0
#define DIR_LEFT   1
#define DIR_RIGHT  2
#define DIR_UP     3
#define DIR_DOWN   4

#define NGHOSTS 4

#define PACMAN_TILE_RIGHT _TILE_0
#define PACMAN_TILE_LEFT  _TILE_1
#define PACMAN_TILE_UP    _TILE_2
#define PACMAN_TILE_DOWN  _TILE_3
#define GHOST_TILE_0      _TILE_4
#define GHOST_TILE_1      _TILE_5
#define DOT_TILE          _TILE_6
#define PELLET_TILE       _TILE_7
#define WALL_TILE         _TILE_8

 


#define PACMAN_COLOR _XL_YELLOW

/* ---- open maze: short pillars, wide corridors ---- */

static uint8_t maze_layout[MH][MW] = {
{ T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL}, /* 0 */
{ T_WALL,T_EMPTY,T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_EMPTY,T_WALL}, /* 1 */
{ T_WALL,T_DOT,   T_DOT,   T_WALL,T_DOT,   T_DOT,   T_DOT,   T_WALL,T_DOT,   T_DOT,   T_DOT,   T_DOT,   T_WALL,T_DOT,   T_DOT,   T_WALL,T_DOT,   T_DOT,   T_DOT,  T_WALL}, /* 2 */
{ T_WALL,T_PELLET,T_EMPTY,T_EMPTY,T_DOT,   T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_PELLET,T_WALL}, /* 3 */
{ T_WALL,T_DOT,   T_EMPTY,T_EMPTY,T_WALL,T_EMPTY,T_DOT,   T_EMPTY,T_WALL,T_WALL,T_EMPTY,T_DOT,   T_EMPTY,T_EMPTY,T_DOT,   T_EMPTY,T_EMPTY,T_WALL,T_EMPTY,T_DOT }, /* 4 */
{ T_WALL,T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,  T_WALL}, /* 5 */
{ T_WALL,T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_EMPTY,T_WALL}, /* 6 */
{ T_WALL,T_PELLET,T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_DOT,   T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_DOT,   T_PELLET,T_WALL}, /* 7 */
{ T_WALL,T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_EMPTY,T_WALL}, /* 8 */
{ T_WALL,T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,   T_EMPTY,T_DOT,  T_WALL}, /* 9 */
{ T_WALL,T_DOT,   T_EMPTY,T_EMPTY,T_WALL,T_EMPTY,T_DOT,   T_EMPTY,T_WALL,T_WALL,T_EMPTY,T_DOT,   T_EMPTY,T_EMPTY,T_DOT,   T_EMPTY,T_EMPTY,T_WALL,T_EMPTY,T_DOT }, /*10 */
{ T_WALL,T_PELLET,T_EMPTY,T_EMPTY,T_DOT,   T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_PELLET,T_WALL}, /*11 */
{ T_WALL,T_DOT,   T_DOT,   T_WALL,T_DOT,   T_DOT,   T_DOT,   T_WALL,T_DOT,   T_DOT,   T_DOT,   T_DOT,   T_WALL,T_DOT,   T_DOT,   T_WALL,T_DOT,   T_DOT,   T_DOT,  T_WALL}, /*12 */
{ T_WALL,T_EMPTY,T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_EMPTY,T_DOT,   T_DOT,   T_EMPTY,T_WALL}, /*13 */
{ T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL,T_WALL} /*14 */
};


/* ---- runtime state ---- */

static uint8_t g_maze[MH][MW];

static uint8_t px, py;
static uint8_t pdir;
static uint8_t pending_dir;
static uint8_t lives;

static uint8_t gx[NGHOSTS];
static uint8_t gy[NGHOSTS];
static uint8_t gdir[NGHOSTS];
static uint8_t fright[NGHOSTS];

static uint8_t prev_px, prev_py;
static uint8_t prev_gx[NGHOSTS];
static uint8_t prev_gy[NGHOSTS];
static uint8_t first_draw = 1;

static uint8_t ghost_clr[NGHOSTS] = { _XL_RED, _XL_GREEN, _XL_MAGENTA, _XL_WHITE };

static uint16_t score;
static uint16_t dots_left;
static uint8_t fright_tick;

static uint8_t pacman_tile;
static uint8_t ghost_tile;

static uint8_t counter;

/* ---- helpers ---- */

static void init_maze(void)
{
    uint8_t r, c;
    dots_left = 0;
    for (r = 0; r < MH; r++) {
        for (c = 0; c < MW; c++) {
            g_maze[r][c] = maze_layout[r][c];
            if (g_maze[r][c] == T_DOT || g_maze[r][c] == T_PELLET) dots_left++;
        }
    }
}

static int is_wall(uint8_t x, uint8_t y)
{
    if (x >= MW || y >= MH) return 1;
    if (g_maze[y][x] == T_WALL) return 1;
    return 0;
}


/* ---- full render (once at start / after resets) ---- */

static void render_full(void)
{
    uint8_t r, c, i;
    for (r = 0; r < MH; r++) {
        for (c = 0; c < MW; c++) {
            if (g_maze[r][c] == T_WALL) {
                _XL_DRAW(c, r, WALL_TILE, _XL_BLUE);
            } else if (g_maze[r][c] == T_DOT) {
                _XL_DRAW(c, r, DOT_TILE, _XL_YELLOW);
            } else if (g_maze[r][c] == T_PELLET) {
                _XL_DRAW(c, r, PELLET_TILE, _XL_MAGENTA);
            } else //if (g_maze[r][c] == T_EMPTY) 
            {
                _XL_DELETE(c, r);
            }
        }
    }
    _XL_DRAW(px, py, pacman_tile, PACMAN_COLOR);
    for (i = 0; i < NGHOSTS; i++) {
        if (fright[i]) {
            _XL_DRAW(gx[i], gy[i], ghost_tile, _XL_CYAN);
        } else {
            _XL_DRAW(gx[i], gy[i], ghost_tile, ghost_clr[i]);
        }
    }
}


/* ---- incremental render: only changed tiles ----
   Key fix: when a ghost leaves a cell containing a dot/pellet in g_maze,
   we must re-draw the dot BEFORE deleting the old ghost position.
   When a ghost arrives at a new cell, we draw the ghost on top of any dot
   that is still present (dot remains in g_maze until Pac-Man eats it). */

static void render_delta(void)
{
    uint8_t i;

    /* Player: delete old position only if no dot exists there anymore */
    if (prev_px != px || prev_py != py) {
        /* If a dot is still in g_maze at the player's old cell, redraw it */
        if (g_maze[prev_py][prev_px] == T_DOT) {
            _XL_DRAW(prev_px, prev_py, DOT_TILE, _XL_YELLOW);
        } else if (g_maze[prev_py][prev_px] == T_PELLET) {
            _XL_DRAW(prev_px, prev_py, PELLET_TILE, _XL_MAGENTA);
        } else
        {
            _XL_DELETE(prev_px, prev_py);
        }
    }

    /* Ghosts */
    if(counter&1)
    {
        ghost_tile = GHOST_TILE_0;
    }
    else
    {
        ghost_tile = GHOST_TILE_1;
    }
    for (i = 0; i < NGHOSTS; i++) {
        if (prev_gx[i] != gx[i] || prev_gy[i] != gy[i]) {
            /* Delete the ghost's old position */
            _XL_DELETE(prev_gx[i], prev_gy[i]);

            /* If a dot/pellet still exists in g_maze at that cell, redraw it */
            if (g_maze[prev_gy[i]][prev_gx[i]] == T_DOT) {
                _XL_DRAW(prev_gx[i], prev_gy[i], DOT_TILE, _XL_YELLOW);
            } else if (g_maze[prev_gy[i]][prev_gx[i]] == T_PELLET) {
                _XL_DRAW(prev_gx[i], prev_gy[i], PELLET_TILE, _XL_MAGENTA);
            }

            /* Draw the ghost at its new position */
            if (fright[i]) {
                _XL_DRAW(gx[i], gy[i], ghost_tile, _XL_CYAN);
            } else {
                _XL_DRAW(gx[i], gy[i], ghost_tile, ghost_clr[i]);
            }
        } else {
            /* Ghost didn't move: just redraw (colour may have changed) */
            if (fright[i]) {
                _XL_DRAW(gx[i], gy[i], ghost_tile, _XL_CYAN);
            } else {
                _XL_DRAW(gx[i], gy[i], ghost_tile, ghost_clr[i]);
            }
        }
    }

    prev_px = px;
    prev_py = py;
    for (i = 0; i < NGHOSTS; i++) {
        prev_gx[i] = gx[i];
        prev_gy[i] = gy[i];
    }

}


/* ---- score display ---- */

static void draw_score(void)
{
    static char buf[6];
    uint16_t tmp = score;
    uint8_t n = 0;
    uint8_t p;

    while (tmp > 0 && n < 5) {
        buf[n] = (char)((uint8_t)(tmp % 10) + '0');
        tmp /= 10;
        n++;
    }
    if (n == 0) {
        buf[0] = '0';
        n = 1;
    }
    buf[n] = '\0';

    for (p = 0; p < n / 2; p++) {
        char t = buf[p];
        buf[p] = buf[n - 1 - p];
        buf[n - 1 - p] = t;
    }

    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(2, MH + 2, buf);
}


/* ---- input ---- */

static void read_input(uint8_t *dir)
{
    uint8_t inp = _XL_INPUT();
    if (_XL_LEFT(inp)) {
        *dir = DIR_LEFT;
        pacman_tile = PACMAN_TILE_LEFT;
    } else if (_XL_RIGHT(inp)) {
        *dir = DIR_RIGHT;
        pacman_tile = PACMAN_TILE_RIGHT;
    } else if (_XL_UP(inp)) {
        *dir = DIR_UP;
        pacman_tile = PACMAN_TILE_UP;
    } else if (_XL_DOWN(inp)) {
        *dir = DIR_DOWN;
        pacman_tile = PACMAN_TILE_DOWN;
    } else {
        *dir = DIR_NONE;
    }
    // if(*dir != DIR_NONE)
    // {
        // _XL_DRAW(px,py,PACMAN_TILE,PACMAN_COLOR);
    // }
}


/* ---- player movement (only Pac-Man eats dots) ---- */

#define FRIGHT_DURATION 50

static void try_move_player(uint8_t dir)
{
    uint8_t nx, ny;

    if (dir == DIR_LEFT) {
        nx = px - 1; ny = py;
    } else if (dir == DIR_RIGHT) {
        nx = px + 1; ny = py;
    } else if (dir == DIR_UP) {
        nx = px; ny = py - 1;
    } else if (dir == DIR_DOWN) {
        nx = px; ny = py + 1;
    } else {
        return;
    }

    if (!is_wall(nx, ny)) {
        /* Only Pac-Man deletes dots */
        if (g_maze[ny][nx] == T_DOT || g_maze[ny][nx] == T_PELLET) {
            score++;
            dots_left--;
            _XL_TICK_SOUND();
            _XL_DELETE(nx, ny);  /* remove the dot from screen */
            if (g_maze[ny][nx] == T_PELLET) {
                fright_tick = FRIGHT_DURATION;
                { uint8_t i; for (i = 0; i < NGHOSTS; i++) fright[i] = 1; }
                _XL_ZAP_SOUND();
            }
        }
        g_maze[ny][nx] = T_EMPTY;
        px = nx;
        py = ny;
        pdir = dir;
    }
}

static void update_player(void)
{
    uint8_t dir;
    read_input(&dir);

    if (dir != DIR_NONE && dir != pdir) {
        uint8_t nx, ny;
        if (dir == DIR_LEFT)      { nx = px - 1; ny = py; }
        else if (dir == DIR_RIGHT){ nx = px + 1; ny = py; }
        else if (dir == DIR_UP)   { nx = px;    ny = py - 1; }
        else                      { nx = px;    ny = py + 1; }

        if (!is_wall(nx, ny)) {
            try_move_player(dir);
        } else {
            pending_dir = dir;
        }
    } else if (dir != DIR_NONE && dir == pdir) {
        try_move_player(pdir);
    } else if (pdir != DIR_NONE) {
        try_move_player(pdir);
    }

    if (pending_dir != DIR_NONE && pending_dir != pdir) {
        uint8_t nx, ny;
        if (pending_dir == DIR_LEFT)      { nx = px - 1; ny = py; }
        else if (pending_dir == DIR_RIGHT){ nx = px + 1; ny = py; }
        else if (pending_dir == DIR_UP)   { nx = px;    ny = py - 1; }
        else                              { nx = px;    ny = py + 1; }

        if (!is_wall(nx, ny)) {
            try_move_player(pending_dir);
        }
        pending_dir = DIR_NONE;
    }
}


/* ---- ghost AI (ghosts do NOT modify g_maze or delete dots) ---- */

static void move_ghost(uint8_t i)
{
    uint8_t nx, ny;
    uint8_t d;
    uint16_t min_dist;
    uint8_t best_d;
    uint8_t dirs[4];
    uint8_t k;

    dirs[0] = DIR_LEFT;
    dirs[1] = DIR_RIGHT;
    dirs[2] = DIR_UP;
    dirs[3] = DIR_DOWN;

    if (fright[i]) {
        uint16_t r = _XL_RAND();
        d = (uint8_t)(r % 4) + 1;

        if (d == DIR_LEFT)      { nx = gx[i] - 1; ny = gy[i]; }
        else if (d == DIR_RIGHT){ nx = gx[i] + 1; ny = gy[i]; }
        else if (d == DIR_UP)   { nx = gx[i];    ny = gy[i] - 1; }
        else                    { nx = gx[i];    ny = gy[i] + 1; }

        /* Ghosts only check walls — dots stay in g_maze */
        if (!is_wall(nx, ny)) {
            uint8_t rev;
            if (d == DIR_LEFT)      rev = DIR_RIGHT;
            else if (d == DIR_RIGHT)rev = DIR_LEFT;
            else if (d == DIR_UP)   rev = DIR_DOWN;
            else                    rev = DIR_UP;

            if (gdir[i] != DIR_NONE && d == rev) {
                uint8_t alt;
                for (k = 0; k < 4; k++) {
                    alt = dirs[k];
                    if (alt == gdir[i]) continue;
                    if (alt == rev) continue;
                    if (alt == d) continue;

                    if (alt == DIR_LEFT)      { nx = gx[i] - 1; ny = gy[i]; }
                    else if (alt == DIR_RIGHT){ nx = gx[i] + 1; ny = gy[i]; }
                    else if (alt == DIR_UP)   { nx = gx[i];    ny = gy[i] - 1; }
                    else                      { nx = gx[i];    ny = gy[i] + 1; }

                    if (!is_wall(nx, ny)) {
                        d = alt;
                        break;
                    }
                }
            }
            gx[i] = nx;
            gy[i] = ny;
            gdir[i] = d;
        }
        return;
    }

    /* chase mode */
    min_dist = 0xFFFF;
    best_d = gdir[i];

    for (k = 0; k < 4; k++) {
        uint8_t dd = dirs[k];
        uint16_t dist;
        uint8_t cx, cy;

        if (gdir[i] != DIR_NONE) {
            uint8_t rev;
            if (dd == DIR_LEFT)      rev = DIR_RIGHT;
            else if (dd == DIR_RIGHT)rev = DIR_LEFT;
            else if (dd == DIR_UP)   rev = DIR_DOWN;
            else                     rev = DIR_UP;
            if (dd == rev) continue;
        }

        if (dd == DIR_LEFT)      { cx = gx[i] - 1; cy = gy[i]; }
        else if (dd == DIR_RIGHT){ cx = gx[i] + 1; cy = gy[i]; }
        else if (dd == DIR_UP)   { cx = gx[i];    cy = gy[i] - 1; }
        else                     { cx = gx[i];    cy = gy[i] + 1; }

        /* Ghosts only check walls, not dots */
        if (is_wall(cx, cy)) continue;

        dist = 0;
        if (cx >= px) dist += (uint16_t)(cx - px);
        else           dist += (uint16_t)((uint8_t)(px - cx));
        if (cy >= py) dist += (uint16_t)(cy - py);
        else           dist += (uint16_t)((uint8_t)(py - cy));

        dist += (_XL_RAND() % 3);

        if (dist < min_dist) {
            min_dist = dist;
            best_d = dd;
        }
    }

    d = best_d;
    if (d == DIR_LEFT)      { nx = gx[i] - 1; ny = gy[i]; }
    else if (d == DIR_RIGHT){ nx = gx[i] + 1; ny = gy[i]; }
    else if (d == DIR_UP)   { nx = gx[i];    ny = gy[i] - 1; }
    else                    { nx = gx[i];    ny = gy[i] + 1; }

    /* Ghosts do NOT modify g_maze */
    if (is_wall(nx, ny)) {
        uint8_t fb;
        if (d == DIR_LEFT)      fb = DIR_RIGHT;
        else if (d == DIR_RIGHT)fb = DIR_LEFT;
        else if (d == DIR_UP)   fb = DIR_DOWN;
        else                    fb = DIR_UP;

        if (fb == DIR_LEFT)      { nx = gx[i] - 1; ny = gy[i]; }
        else if (fb == DIR_RIGHT){ nx = gx[i] + 1; ny = gy[i]; }
        else if (fb == DIR_UP)   { nx = gx[i];    ny = gy[i] - 1; }
        else                     { nx = gx[i];    ny = gy[i] + 1; }

        if (!is_wall(nx, ny)) d = fb;
        else return;
    }

    gx[i] = nx;
    gy[i] = ny;
    gdir[i] = d;
}

static void update_ghosts(void)
{
    uint8_t i;
    for (i = 0; i < NGHOSTS; i++) {
        move_ghost(i);
    }
}


/* ---- collision ---- */

static int ghost_hit_player(void)
{
    uint8_t i;
    for (i = 0; i < NGHOSTS; i++) {
        if (gx[i] == px && gy[i] == py) return 1;
    }
    return 0;
}


/* ---- fright countdown ---- */

static void tick_fright(void)
{
    if (fright_tick > 0) {
        fright_tick--;
        if (fright_tick == 0) {
            uint8_t i;
            for (i = 0; i < NGHOSTS; i++) fright[i] = 0;
        }
    }
}


/* ---- reset ---- */

static void reset_positions(void)
{
    px = 10; py = 7; pdir = DIR_NONE; pending_dir = DIR_NONE;

    gx[0] = 9;  gy[0] = 2; gdir[0] = DIR_LEFT;
    gx[1] = 10; gy[1] = 2; gdir[1] = DIR_RIGHT;
    gx[2] = 8;  gy[2] = 11; gdir[2] = DIR_UP;
    gx[3] = 11; gy[3] = 11; gdir[3] = DIR_DOWN;

    { uint8_t i; for (i = 0; i < NGHOSTS; i++) fright[i] = 0; }
    fright_tick = 0;
    first_draw = 1;
}


/* ---- main ---- */

int main(void)
{
    
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    for(;;)
    {
        _XL_CLEAR_SCREEN();
        lives = 3;
        score = 0;
        first_draw = 1;

        init_maze();
        reset_positions();
        render_full();
        draw_score();

        prev_px = px;
        prev_py = py;
        
        pacman_tile = PACMAN_TILE_RIGHT;
        ghost_tile = GHOST_TILE_0;
        
        { uint8_t i; for (i = 0; i < NGHOSTS; i++) { prev_gx[i] = gx[i]; prev_gy[i] = gy[i]; } }

        first_draw = 0;
        counter = 0;
        for (;;) {
            update_player();
            tick_fright();
            ++counter;
            if(!(counter&3))
            {
                update_ghosts();
            }
            if (ghost_hit_player()) {
                if (fright[0]) {
                    score += 50;
                    _XL_EXPLOSION_SOUND();
                    gx[0] = 10; gy[0] = 2; gdir[0] = DIR_LEFT;
                    first_draw = 1;
                } else {
                    lives--;
                    _XL_EXPLOSION_SOUND();
                    if (lives == 0) {
                        _XL_SET_TEXT_COLOR(_XL_RED);
                        _XL_PRINT(5, MH + 3, "GAME OVER");
                        _XL_SLEEP(3);
                        break;
                    }
                    reset_positions();
                }
            }

            if (dots_left == 0) {
                _XL_SET_TEXT_COLOR(_XL_GREEN);
                _XL_PRINT(5, MH + 3, "YOU WIN");
                _XL_TOCK_SOUND();
                _XL_SLEEP(2);
                break;
            }

            if (first_draw) {
                render_full();
                prev_px = px;
                prev_py = py;
                { uint8_t i; for (i = 0; i < NGHOSTS; i++) { prev_gx[i] = gx[i]; prev_gy[i] = gy[i]; } }
                first_draw = 0;
            } else {
                render_delta();
            }
            _XL_DRAW(px,py,pacman_tile,PACMAN_COLOR);

            draw_score();
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }
    return 0;
}
