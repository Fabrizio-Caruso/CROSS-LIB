#include "cross_lib.h"

/* Manual type definitions to avoid stdint.h */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

/* Tile IDs mapped to Cross-Lib constants */
#define TILE_EMPTY _TILE_0
#define TILE_ENEMY _TILE_1
#define TILE_PLAYER _TILE_2
#define TILE_BULLET _TILE_3

/* Game constants */
#define MAX_ENEMIES 4
#define MAX_BULLETS 4

/* Double-buffered tile grid for diff rendering */
uint8_t curr[XSize + 1][YSize + 1];
uint8_t prev[XSize + 1][YSize + 1];

/* Entity structures */
typedef struct {
    uint8_t x, y;
    uint8_t dir_x; /* 0 = move right, 1 = move left */
} Enemy;

typedef struct {
    uint8_t x, y;
    uint8_t dir_y; /* 0 = move up, 1 = move down */
    uint8_t is_player; /* 1 if fired by player, 0 if enemy */
} Bullet;

Enemy enemies[MAX_ENEMIES];
Bullet bullets[MAX_BULLETS];
uint8_t p_x;
#define PLAYER_Y (YSize-1)
uint8_t game_over = 0;

/* Initialize or reset all game state */
void reset_game(void) {
    for(uint8_t y=0; y<=YSize; y++) {
        for(uint8_t x=0; x<=XSize; x++) {
            curr[x][y] = TILE_EMPTY;
            prev[x][y] = TILE_EMPTY;
        }
    }

    /* Place player near bottom center */
    p_x = XSize / 2;
    // PLAYER_Y = YSize - 1;
    curr[p_x][PLAYER_Y] = TILE_PLAYER;

    /* Spawn enemies */
    for(uint8_t i=0; i<MAX_ENEMIES; i++) {
        enemies[i].x = (_XL_RAND() % XSize) + 1;
        enemies[i].y = i * 20 + 5;
        enemies[i].dir_x = _XL_RAND() % 2; /* Random initial direction */
    }

    /* Clear bullet array */
    for(uint8_t i=0; i<MAX_BULLETS; i++) {
        bullets[i].x = 0;
        bullets[i].y = 0;
    }

    _XL_CLEAR_SCREEN();
}

/* Main game logic & rendering frame */
void update(void) {
    uint8_t inp = _XL_INPUT();

    /* Player horizontal movement */
    if(_XL_LEFT(inp) && p_x > 0) {
        curr[p_x][PLAYER_Y] = TILE_EMPTY;
        p_x--;
    } else if(_XL_RIGHT(inp) && p_x < XSize) {
        curr[p_x][PLAYER_Y] = TILE_EMPTY;
        p_x++;
    }

    /* Player shooting */
    if(_XL_FIRE(inp)) {
        for(uint8_t i=0; i<MAX_BULLETS; i++) {
            if(bullets[i].x == 0 && bullets[i].y == 0) {
                curr[p_x][PLAYER_Y - 1] = TILE_EMPTY;
                bullets[i].x = p_x;
                bullets[i].y = PLAYER_Y - 1;
                bullets[i].dir_y = 0; /* Up */
                bullets[i].is_player = 1;
                curr[bullets[i].x][bullets[i].y] = TILE_BULLET;
                _XL_PING_SOUND();
                break;
            }
        }
    }

    /* Update enemies */
    for(uint8_t i=0; i<MAX_ENEMIES; i++) {
        /* Calculate next position */
        uint8_t nx = enemies[i].dir_x == 0 ? enemies[i].x + 1 : enemies[i].x - 1;
        uint8_t ny = enemies[i].y + 1;

        /* Bounce off horizontal walls */
        if(enemies[i].x == XSize-1) { enemies[i].dir_x = 1; nx = enemies[i].x - 1; }
        else if(enemies[i].x == 0) { enemies[i].dir_x = 0; nx = enemies[i].x + 1; }

        /* Respawn at top if off bottom screen */
        if(ny == PLAYER_Y) { enemies[i].y = 2; ny = 2; _XL_DELETE(enemies[i].x,PLAYER_Y);}

        /* Move enemy in grid */
        curr[enemies[i].x][enemies[i].y] = TILE_EMPTY;
        enemies[i].x = nx;
        enemies[i].y = ny;
        curr[enemies[i].x][enemies[i].y] = TILE_ENEMY;

        /* Enemy shooting at player (probabilistic) */
        if(!(_XL_RAND()&1)) { /* ~2% chance per frame */
            if(enemies[i].y < PLAYER_Y) {
                for(uint8_t j=0; j<MAX_BULLETS; j++) {
                    if(bullets[j].x == 0 && bullets[j].y == 0) {
                        curr[enemies[i].x][enemies[i].y] = TILE_EMPTY;
                        bullets[j].x = enemies[i].x;
                        bullets[j].y = enemies[i].y + 1;
                        bullets[j].dir_y = 1; /* Down */
                        bullets[j].is_player = 0;
                        curr[bullets[j].x][bullets[j].y] = TILE_BULLET;
                        break;
                    }
                }
            }
        }
    }

    /* Update bullets */
    for(uint8_t i=0; i<MAX_BULLETS; i++) {
        if(bullets[i].x == 0 && bullets[i].y == 0) continue;

        uint8_t bx = bullets[i].x;
        uint8_t by;

        /* Safe unsigned arithmetic for direction */
        if(bullets[i].dir_y == 0) { /* Moving up */
            if(bullets[i].y > 0) by = bullets[i].y - 1;
            else { curr[bullets[i].x][bullets[i].y] = TILE_EMPTY; bullets[i].x=0; bullets[i].y=0; continue; }
        } else { /* Moving down */
            by = bullets[i].y + 1;
        }

        /* Remove if off bottom screen */
        if(by > YSize) { curr[bullets[i].x][bullets[i].y] = TILE_EMPTY; bullets[i].x=0; bullets[i].y=0; continue; }

        /* Move bullet in grid */
        curr[bullets[i].x][bullets[i].y] = TILE_EMPTY;
        bullets[i].x = bx;
        bullets[i].y = by;
        curr[bullets[i].x][bullets[i].y] = TILE_BULLET;

        /* Collision: Player bullet hits enemy */
        if(bullets[i].is_player) {
            for(uint8_t j=0; j<MAX_ENEMIES; j++) {
                if(bullets[i].x == enemies[j].x && bullets[i].y == enemies[j].y) {
                    curr[bullets[i].x][bullets[i].y] = TILE_EMPTY;
                    bullets[i].x = 0; 
                    bullets[i].y = 0;

                    /* Respawn enemy slightly lower */
                    enemies[j].x = (_XL_RAND() % XSize) + 1;
                    enemies[j].y += 5;
                    if(enemies[j].y > YSize) enemies[j].y = 2;
                    curr[enemies[j].x][enemies[j].y] = TILE_ENEMY;
                    break;
                }
            }
        } else {
            /* Collision: Enemy bullet hits player */
            if(bullets[i].x == p_x && bullets[i].y == PLAYER_Y) {
                game_over = 1;
            }
        }
    }

    /* Ensure player tile is always rendered */
    curr[p_x][PLAYER_Y] = TILE_PLAYER;

    /* Diff Rendering: Only draw/delete tiles that changed since last frame */
    for(uint8_t y=0; y<=YSize; y++) {
        for(uint8_t x=0; x<=XSize; x++) {
            if(prev[x][y] != curr[x][y]) {
                /* Remove old tile if it was not empty */
                if(prev[x][y] != TILE_EMPTY) {
                    _XL_DELETE(x, y);
                }

                /* Draw new tile with appropriate color */
                if(curr[x][y] != TILE_EMPTY) {
                    uint8_t col = _XL_WHITE;
                    if(curr[x][y] == TILE_ENEMY) col = _XL_RED;
                    else if(curr[x][y] == TILE_PLAYER) col = _XL_CYAN;
                    else if(curr[x][y] == TILE_BULLET) col = _XL_YELLOW;

                    _XL_DRAW(x, y, curr[x][y], col);
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
    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR*4);
}

/* Infinite main loop with automatic restart */
int main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while(1) { /* Infinite loop as requested */
        reset_game();
        game_over = 0;

        while(!game_over) {
            update();
        }
		_XL_SET_TEXT_COLOR(_XL_RED);
		_XL_PRINT((XSize/2)-4,YSize/2,"GAME OVER");
        /* Brief pause before restarting */
        _XL_SLEEP(2);
		_XL_WAIT_FOR_INPUT();
    }
}
