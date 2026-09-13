#include "cross_lib.h"

/* Screen dimensions (must respect 0<=x<=XSize, 0<=y<=YSize) */
// #define XSize 60
// #define YSize 15

/* Game constants */
#define MAX_ENEMIES 8
#define MAX_BULLETS 10

/* Map to Cross-Lib tile constants as requested */
#define TILE_EMPTY _TILE_0
#define TILE_ENEMY _TILE_1
#define TILE_BULLET _TILE_2
#define TILE_PLAYER _TILE_3

/* Structs for game entities */
typedef struct {
    uint8_t x, y;
    uint8_t dx, dy; /* 0: none, 1: +1, 2: -1 */
    uint8_t timer;
} Enemy;

typedef struct {
    uint8_t x, y;
    uint8_t dy;     /* 1: down, 2: up */
    uint8_t is_player;
} Bullet;

/* Double-buffered grid for diff rendering */
static uint8_t prev[XSize+1][YSize+1];
static uint8_t curr[XSize+1][YSize+1];

/* Game state */
static Enemy enemies[MAX_ENEMIES];
static Bullet bullets[MAX_BULLETS];
static uint8_t p_x, p_y;

/* Resets all state and clears screen */
void reset_game(void) {
    _XL_CLEAR_SCREEN();
    
    for(uint8_t i=0; i<=XSize; i++) {
        for(uint8_t j=0; j<=YSize; j++) {
            prev[i][j] = TILE_EMPTY;
            curr[i][j] = TILE_EMPTY;
        }
    }

    /* Player starts near bottom center */
    p_x = XSize / 2;
    p_y = YSize - 2;

    /* Spawn enemies */
    for(uint8_t i=0; i<MAX_ENEMIES; i++) {
        enemies[i].x = _XL_RAND() % XSize;
        enemies[i].y = 2 + (_XL_RAND() % 5);
        /* Random horizontal variation (0,1,2) */
        enemies[i].dx = _XL_RAND() % 3;
        /* Downward bias with slight diagonal variation */
        enemies[i].dy = (_XL_RAND() % 2) + 1; 
        enemies[i].timer = _XL_RAND() % 40 + 15;
    }

    /* Clear bullets */
    for(uint8_t i=0; i<MAX_BULLETS; i++) {
        bullets[i].x = 0;
        bullets[i].y = 0;
    }

    /* Draw initial player */
    curr[p_x][p_y] = TILE_PLAYER;
}

/* Main game loop frame */
void game_frame(void) {
    uint8_t inp = _XL_INPUT();

    /* --- Player Movement (Horizontal) --- */
    if(_XL_LEFT(inp)) {
        curr[p_x][p_y] = TILE_EMPTY;
        if(p_x > 0) p_x--;
    } else if(_XL_RIGHT(inp)) {
        curr[p_x][p_y] = TILE_EMPTY;
        if(p_x < XSize) p_x++;
    }
    curr[p_x][p_y] = TILE_PLAYER;

    /* --- Enemy Movement & AI --- */
    for(uint8_t i=0; i<MAX_ENEMIES; i++) {
        curr[enemies[i].x][enemies[i].y] = TILE_EMPTY;

        /* Reverse direction on screen bounds */
        if(enemies[i].x == 0 || enemies[i].x == XSize) {
            if(enemies[i].dx == 0) enemies[i].dx = 1; else if(enemies[i].dx == 1) enemies[i].dx = 2;
        }
        if(enemies[i].y == 0 || enemies[i].y == YSize) {
            if(enemies[i].dy == 0) enemies[i].dy = 1; else if(enemies[i].dy == 1) enemies[i].dy = 2;
        }

        /* Apply movement */
        if(enemies[i].dx == 1) enemies[i].x++;
        else if(enemies[i].dx == 2) {
            if(enemies[i].x > 0) enemies[i].x--;
        }

        if(enemies[i].dy == 1) enemies[i].y++;
        else if(enemies[i].dy == 2) {
            if(enemies[i].y > 0) enemies[i].y--;
        }

        /* Clamp to bounds */
        if(enemies[i].x > XSize) enemies[i].x = XSize;
        if(enemies[i].y > YSize) enemies[i].y = YSize;

        /* Game Over if enemy reaches player row */
        if(enemies[i].y >= p_y) {
            _XL_EXPLOSION_SOUND();
            reset_game();
            return;
        }

        curr[enemies[i].x][enemies[i].y] = TILE_ENEMY;
    }

    /* --- Shooting Logic --- */
    /* Player shoots upwards on FIRE */
    if(_XL_FIRE(inp)) {
        for(uint8_t i=0; i<MAX_BULLETS; i++) {
            if(bullets[i].x == 0 && bullets[i].y == 0) {
                bullets[i].x = p_x;
                bullets[i].y = p_y - 1;
                bullets[i].dy = 2; /* Up */
                bullets[i].is_player = 1;
                break;
            }
        }
    }

    /* Enemies shoot downwards randomly */
    for(uint8_t i=0; i<MAX_ENEMIES; i++) {
        enemies[i].timer--;
        if(enemies[i].timer <= 0) {
            enemies[i].timer = _XL_RAND() % 30 + 15;
            for(uint8_t j=0; j<MAX_BULLETS; j++) {
                if(bullets[j].x == 0 && bullets[j].y == 0) {
                    bullets[j].x = enemies[i].x;
                    bullets[j].y = enemies[i].y + 1;
                    bullets[j].dy = 1; /* Down */
                    bullets[j].is_player = 0;
                    break;
                }
            }
        }
    }

    /* --- Bullet Update & Collision --- */
    for(uint8_t i=0; i<MAX_BULLETS; i++) {
        if(bullets[i].x == 0 && bullets[i].y == 0) continue;

        curr[bullets[i].x][bullets[i].y] = TILE_EMPTY;
        
        /* Move bullet safely with unsigned arithmetic */
        if(bullets[i].dy == 1) { /* Down */
            if(bullets[i].y >= YSize) { bullets[i].x = 0; bullets[i].y = 0; continue; }
            bullets[i].y++;
        } else { /* Up (dy==2) */
            if(bullets[i].y == 0) { bullets[i].x = 0; bullets[i].y = 0; continue; }
            bullets[i].y--;
        }

        curr[bullets[i].x][bullets[i].y] = TILE_BULLET;

        /* Collision: Enemy bullet hits player -> Game Over */
        if(!bullets[i].is_player && bullets[i].y >= p_y) {
            _XL_EXPLOSION_SOUND();
            reset_game();
            return;
        }

        /* Collision: Player bullet hits enemy */
        if(bullets[i].is_player) {
            for(uint8_t j=0; j<MAX_ENEMIES; j++) {
                if(bullets[i].x == enemies[j].x && bullets[i].y == enemies[j].y) {
                    _XL_PING_SOUND();
                    bullets[i].x = 0; /* Destroy bullet */
                    bullets[i].y = 0;

                    /* Respawn enemy at random horizontal position away from player */
                    uint8_t nx = enemies[j].x;
                    while(nx >= p_x && nx < XSize) nx++;
                    if(nx > XSize) nx = 0;
                    
                    enemies[j].x = nx;
                    enemies[j].y = 2 + (_XL_RAND() % 5);
                    curr[enemies[j].x][enemies[j].y] = TILE_ENEMY;
                    break;
                }
            }
        }
    }

    /* --- Diff Rendering: Only draw/delete changed tiles --- */
    for(uint8_t y=0; y<=YSize; y++) {
        for(uint8_t x=0; x<=XSize; x++) {
            uint8_t old = prev[x][y];
            uint8_t new = curr[x][y];

            if(old != new) {
                if(old != TILE_EMPTY) _XL_DELETE(x, y);
                if(new != TILE_EMPTY) {
                    uint8_t color = _XL_WHITE;
                    if(new == TILE_ENEMY) color = _XL_RED;
                    else if(new == TILE_PLAYER) color = _XL_CYAN;
                    else if(new == TILE_BULLET) color = _XL_YELLOW;
                    _XL_DRAW(x, y, new, color);
                }
            }
        }
    }

    /* Swap buffers for next frame */
    for(uint8_t y=0; y<=YSize; y++) {
        for(uint8_t x=0; x<=XSize; x++) {
            prev[x][y] = curr[x][y];
        }
    }

    /* Frame pacing */
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 2);
}

/* Infinite main loop with automatic restart on game over */
int main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    reset_game();

    while(1) {
        game_frame();
    }
}
