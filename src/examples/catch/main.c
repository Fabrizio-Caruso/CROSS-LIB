
#include "cross_lib.h"

/* 
 * Simple Game: Infinite Tile Collector
 * Description: The player moves with arrow keys and collects items to increase the score.
 * Constraints: No heap usage (static variables), prefers uint8_t/uint16_t, uses provided API.
 */

/* Global State Variables (Static - no heap allocation) */
static uint16_t score = 0;       /* Score counter */
static uint8_t player_x = 0;     /* Player X coordinate */
static uint8_t player_y = 0;     /* Player Y coordinate */
static uint8_t item_active = 0;  /* Flag for collected object status */
static uint8_t item_x = 0;       /* Item X coordinate */
static uint8_t item_y = 0;       /* Item Y coordinate */
static uint16_t spawn_timer = 0; /* Timer to control spawning frequency */

int main(void) {
    /* Initialize the system modules */
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    /* Set initial player position (middle-bottom of screen) */
    player_x = (XSize / 2);
    player_y = (YSize - 1); 
    
    /* Initial clear */
    _XL_CLEAR_SCREEN();

    /* Main Game Loop */
    while (1) {
        /* Poll input device to get key states */
        uint8_t input = _XL_INPUT();

        /* Handle Player Movement with Boundary Checks */
        if (_XL_LEFT(input)) {
            player_x--;
            if (player_x < 0) {
                player_x = 0;
            }
        } else if (_XL_RIGHT(input)) {
            player_x++;
            if (player_x >= XSize) {
                player_x = XSize - 1; /* Clamp to max valid coordinate */
            }
        } else if (_XL_UP(input)) {
            player_y--;
            if (player_y < 0) {
                player_y = 0;
            }
        } else if (_XL_DOWN(input)) {
            player_y++;
            if (player_y >= YSize) {
                player_y = YSize - 1; /* Clamp to max valid coordinate */
            }
        }

        /* Game Logic: Spawn Item */
        /* Ensure a new item spawns if not currently active and timer allows it */
        if (item_active == 0 && spawn_timer <= 0) {
            item_active = 1;
            /* Spawn at random coordinates within valid range */
            item_x = (_XL_RAND() % XSize);
            item_y = (_XL_RAND() % YSize);
            /* Set timer to wait before next automatic spawn */
            spawn_timer = _XL_SLOW_DOWN_FACTOR * 30; 
        } else if (item_active) {
            spawn_timer--;
        }

        /* Check for Collision between Player and Item */
        if (item_active && player_x == item_x && player_y == item_y) {
            score++;
            _XL_PING_SOUND(); /* Play positive sound on collection */

            /* Respawn logic: Clear current item and reset timer immediately 
               for an endless play style */
            item_active = 0;
            spawn_timer = 0;
        }

        /* Clear Screen for the new frame to prevent ghosting */
        _XL_CLEAR_SCREEN();

        /* Draw Player (Blue Tile) */
        _XL_DRAW(player_x, player_y, _TILE_1, _XL_BLUE);

        /* Draw Item if Active (Red Tile) */
        if (item_active) {
            _XL_DRAW(item_x, item_y, _TILE_2, _XL_RED);
        }

        /* Display Score using white text */
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0, YSize - 1, 8, score);

        /* Slow down the loop to control game speed */
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}
