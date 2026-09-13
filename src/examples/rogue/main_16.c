#include "cross_lib.h"

// Game constants
#define ROOMS_PER_LEVEL 64
#define MAX_ENEMIES 8
#define ROOM_WIDTH 16
#define ROOM_HEIGHT 16

// Tile IDs (mapped to Cross-Lib tiles)
#define TILE_FLOOR _TILE_0
#define TILE_WALL _TILE_1
#define TILE_PLAYER _TILE_2
#define TILE_ENEMY _TILE_3
#define TILE_EXIT _TILE_5 // Using a distinct tile for exit

// Colors
#define COLOR_FLOOR _XL_BLUE
#define COLOR_WALL _XL_WHITE
#define COLOR_PLAYER _XL_GREEN
#define COLOR_ENEMY _XL_RED
#define COLOR_EXIT _XL_CYAN
#define COLOR_TEXT _XL_WHITE

// Structures
typedef struct {
    uint8_t x;
    uint8_t y;
} Position;

typedef struct {
    Position pos;
    uint8_t alive;
} Enemy;

typedef struct {
    Position playerPos;
    Enemy enemies[MAX_ENEMIES];
    uint8_t numEnemies;
    uint16_t score;
    uint8_t currentRoom;
    uint8_t gameOver;
    uint8_t won;
} GameState;

// Global game state
static GameState g_game;

// Forward declarations to fix implicit declaration errors
static void initEnemiesForRoom(uint8_t roomIndex);
static uint8_t isWalkable(uint8_t roomIndex, Position pos);
static uint8_t hasEnemy(Position pos);
static uint8_t isExit(uint8_t roomIndex, Position pos);

// Helper: Get layout type for a room index (cycles through 16 types)
static uint8_t getRoomType(uint8_t roomIndex) {
    return (roomIndex % 16);
}

// Check if position is within the grid bounds
static uint8_t inBounds(Position pos) {
    return (pos.x < ROOM_WIDTH && pos.y < ROOM_HEIGHT);
}

// Procedural Room Generation Logic
// Returns 1 if wall, 0 if floor.
// Ensures borders are walls and specific patterns for variety.
// CRITICAL: Start (1,1) and Exit (W-2, H-2) are forced to be walkable.
static uint8_t getTileAt(uint8_t roomType, Position pos) {
    // Borders are always walls
    if (pos.x == 0 || pos.y == 0 || 
        pos.x == ROOM_WIDTH - 1 || pos.y == ROOM_HEIGHT - 1) {
        return 1;
    }

    uint8_t x = pos.x;
    uint8_t y = pos.y;

    // Force Start and Exit to be walkable (Floor)
    if ((x == 1 && y == 1) || 
        (x == ROOM_WIDTH - 2 && y == ROOM_HEIGHT - 2)) {
        return 0;
    }

    // Pattern 0: Open Room (No internal walls)
    if (roomType == 0) return 0;

    // Pattern 1: Central Pillar (2x2 block in center)
    if (roomType == 1) {
        uint8_t cx = ROOM_WIDTH / 2 - 1;
        uint8_t cy = ROOM_HEIGHT / 2 - 1;
        if ((x >= cx && x <= cx + 1) && (y >= cy && y <= cy + 1)) return 1;
        return 0;
    }

    // Pattern 2: Four Quadrant Walls (Cross with gaps)
    if (roomType == 2) {
        uint8_t midX = ROOM_WIDTH / 2;
        uint8_t midY = ROOM_HEIGHT / 2;
        // Vertical wall in middle, gap at center
        if (x == midX && y != midY && y != midY - 1 && y != midY + 1) return 1;
        // Horizontal wall in middle, gap at center
        if (y == midY && x != midX && x != midX - 1 && x != midX + 1) return 1;
        return 0;
    }

    // Pattern 3: Diagonal Walls (Sparse)
    if (roomType == 3) {
        if ((x + y) % 8 == 0 && x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    // Pattern 4: Checkerboard Obstacles (Sparse, every 4th tile)
    if (roomType == 4) {
        if ((x % 4 == 2) && (y % 4 == 2)) return 1;
        return 0;
    }

    // Pattern 5: Vertical Corridors (Walls at x=3,7,11 - avoiding exit column W-2=14)
    if (roomType == 5) {
        // Walls at x=3, 7, 11. Exit is at x=14, so it's safe.
        if ((x == 3 || x == 7 || x == 11) && y > 2 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    // Pattern 6: Horizontal Corridors (Walls at y=3,7,11 - avoiding exit row H-2=14)
    if (roomType == 6) {
        // Walls at y=3, 7, 11. Exit is at y=14, so it's safe.
        if ((y == 3 || y == 7 || y == 11) && x > 2 && x < ROOM_WIDTH - 3) return 1;
        return 0;
    }

    // Pattern 7: Central Cross (Plus sign in the middle)
    if (roomType == 7) {
        uint8_t cx = ROOM_WIDTH / 2;
        uint8_t cy = ROOM_HEIGHT / 2;
        
        // Vertical bar of the cross
        if (x == cx && y >= cy - 3 && y <= cy + 3) return 1;
        // Horizontal bar of the cross
        if (y == cy && x >= cx - 3 && x <= cx + 3) return 1;
        
        return 0;
    }

    // Pattern 8: Diagonal Split (New Design)
    if (roomType == 8) {
        uint8_t diagSum = x + y;
        // Diagonal line roughly from (W-2, 0) to (0, H-2)
        if ((x + y) == (ROOM_WIDTH - 2)) {
            // Gap every 3 tiles along the diagonal
            if (x % 3 != 0 && y % 3 != 0) return 1;
        }
        return 0;
    }

    // Pattern 9: Left and Right Walls (with gaps every 3 tiles)
    if (roomType == 9) {
        if (x <= 2 && y % 3 != 0 && y > 1 && y < ROOM_HEIGHT - 2) return 1;
        if (x >= ROOM_WIDTH - 3 && y % 3 != 0 && y > 1 && y < ROOM_HEIGHT - 2) return 1;
        return 0;
    }

    // Pattern 10: Four Pillars (New Design)
    if (roomType == 10) {
        uint8_t midX = ROOM_WIDTH / 2;
        uint8_t midY = ROOM_HEIGHT / 2;
        
        // Top-Left Pillar
        if (x >= 3 && x <= 5 && y >= 3 && y <= 5) return 1;
        // Top-Right Pillar
        if (x >= midX + 1 && x <= midX + 3 && y >= 3 && y <= 5) return 1;
        // Bottom-Left Pillar
        if (x >= 3 && x <= 5 && y >= midY + 1 && y <= midY + 3) return 1;
        // Bottom-Right Pillar
        if (x >= midX + 1 && x <= midX + 3 && y >= midY + 1 && y <= midY + 3) return 1;
        
        return 0;
    }

    // Pattern 11: Deterministic "Random" obstacles
    if (roomType == 11) {
        uint8_t hash = (x * 7 + y * 13) % 16;
        if (hash < 4 && x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    // Pattern 12: Cross shape in center (thicker)
    if (roomType == 12) {
        uint8_t cx = ROOM_WIDTH / 2;
        uint8_t cy = ROOM_HEIGHT / 2;
        if ((x >= cx - 1 && x <= cx + 1) && y == cy) return 1;
        if ((y >= cy - 1 && y <= cy + 1) && x == cx) return 1;
        return 0;
    }

    // Pattern 13: Diagonal stripes (sparse)
    if (roomType == 13) {
        uint8_t diff = (x > y) ? (x - y) : (y - x);
        if (diff % 6 < 2 && x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    // Pattern 14: Concentric Rings (New Design)
    // Creates nested rectangular walls with gaps to ensure passability.
    if (roomType == 14) {
        uint8_t cx = ROOM_WIDTH / 2;
        uint8_t cy = ROOM_HEIGHT / 2;
        
        // Outer ring: 3 tiles from center
        uint8_t outerDistX = x >= cx - 3 ? (x - (cx - 3)) : ((cx - 3) - x);
        uint8_t outerDistY = y >= cy - 3 ? (y - (cy - 3)) : ((cy - 3) - y);
        
        // Inner ring: 1 tile from center
        uint8_t innerDistX = x >= cx - 1 ? (x - (cx - 1)) : ((cx - 1) - x);
        uint8_t innerDistY = y >= cy - 1 ? (y - (cy - 1)) : ((cy - 1) - y);
        
        // Draw outer ring walls (with gaps at corners to allow passage)
        if ((outerDistX == 3 && outerDistY < 3) || (outerDistY == 3 && outerDistX < 3)) {
            // Gap every 4 tiles on the perimeter of this ring
            uint8_t perimeterPos = (x + y) % 4;
            if (perimeterPos != 0) return 1;
        }
        
        // Draw inner ring walls
        if ((innerDistX == 1 && innerDistY < 1) || (innerDistY == 1 && innerDistX < 1)) {
             // Gap every 2 tiles on the perimeter of this ring
            uint8_t perimeterPos = (x + y) % 2;
            if (perimeterPos != 0) return 1;
        }
        
        return 0;
    }

    // Pattern 15: Zigzag walls
    if (roomType == 15) {
        uint8_t zig = (y / 2) % 2;
        if ((x % 4 == 0 && zig == 1) || (x % 4 == 2 && zig == 0)) {
             if (x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        }
        return 0;
    }

    // Default: Open
    return 0;
}

// Check if a tile at position is walkable (floor or exit, not wall)
static uint8_t isWalkable(uint8_t roomIndex, Position pos) {
    if (!inBounds(pos)) return 0;
    
    uint8_t type = getRoomType(roomIndex);
    uint8_t tile = getTileAt(type, pos);
    
    // Tile 1 is wall, Tile 0 is floor.
    return (tile == 0);
}

// Check if position has an enemy
static uint8_t hasEnemy(Position pos) {
    for (uint8_t i = 0; i < g_game.numEnemies; i++) {
        if (g_game.enemies[i].alive && 
            g_game.enemies[i].pos.x == pos.x && 
            g_game.enemies[i].pos.y == pos.y) {
            return 1;
        }
    }
    return 0;
}

// Check if position is the exit.
// Exit is placed at (Width-2, Height-2), which is inside the border walls.
static uint8_t isExit(uint8_t roomIndex, Position pos) {
    return (pos.x == ROOM_WIDTH - 2 && pos.y == ROOM_HEIGHT - 2);
}

// Helper to draw the background tile at a specific position
static void drawBackgroundTile(Position pos) {
    uint8_t type = getRoomType(g_game.currentRoom);
    uint8_t isWall = getTileAt(type, pos);
    
    if (isWall) {
        _XL_DRAW(pos.x, pos.y, TILE_WALL, COLOR_WALL);
    } else {
        if (isExit(g_game.currentRoom, pos)) {
            _XL_DRAW(pos.x, pos.y, TILE_EXIT, COLOR_EXIT);
        } else {
            _XL_DRAW(pos.x, pos.y, TILE_FLOOR, COLOR_FLOOR);
        }
    }
}

// Initialize enemies for a specific room
static void initEnemiesForRoom(uint8_t roomIndex) {
    g_game.numEnemies = 0;
    
    // Number of enemies increases with room index, capped at MAX_ENEMIES
    uint8_t numToSpawn = (roomIndex / 10) + 2;
    if (numToSpawn > MAX_ENEMIES) numToSpawn = MAX_ENEMIES;
    
    for (uint8_t i = 0; i < numToSpawn; i++) {
        uint8_t attempts = 0;
        Position pos;
        
        // Try to find a valid spawn location
        do {
            // Random position within inner area (1 to Size-2)
            uint8_t maxOffsetX = ROOM_WIDTH - 2; 
            uint8_t maxOffsetY = ROOM_HEIGHT - 2;
            
            pos.x = 1 + (_XL_RAND() % maxOffsetX);
            pos.y = 1 + (_XL_RAND() % maxOffsetY);
            attempts++;
            
        } while ((pos.x == g_game.playerPos.x && pos.y == g_game.playerPos.y) || 
                 isExit(roomIndex, pos) || 
                 !isWalkable(roomIndex, pos) || 
                 hasEnemy(pos) ||
                 attempts > 50);
        
        if (attempts <= 50) {
            g_game.enemies[i].pos = pos;
            g_game.enemies[i].alive = 1;
            g_game.numEnemies++;
        }
    }
}

// Initialize a new game
static void initGame(void) {
    g_game.playerPos.x = 1;
    g_game.playerPos.y = 1;
    g_game.currentRoom = 0;
    g_game.score = 0;
    g_game.gameOver = 0;
    g_game.won = 0;
    
    // Initialize enemies for the first room
    initEnemiesForRoom(0);
}

// Draw the initial full screen (only called on start or room change)
static void drawInitialScreen(void) {
    _XL_CLEAR_SCREEN();
    
    // Draw all background tiles
    for (uint8_t y = 0; y < ROOM_HEIGHT; y++) {
        for (uint8_t x = 0; x < ROOM_WIDTH; x++) {
            Position pos = {x, y};
            drawBackgroundTile(pos);
        }
    }
    
    // Draw enemies
    for (uint8_t i = 0; i < g_game.numEnemies; i++) {
        if (g_game.enemies[i].alive) {
            _XL_DRAW(g_game.enemies[i].pos.x, g_game.enemies[i].pos.y, TILE_ENEMY, COLOR_ENEMY);
        }
    }
    
    // Draw player on top
    _XL_DRAW(g_game.playerPos.x, g_game.playerPos.y, TILE_PLAYER, COLOR_PLAYER);
    
    // 3. Draw UI (Score and Room)
    _XL_SET_TEXT_COLOR(COLOR_TEXT);
    uint8_t uiY = ROOM_HEIGHT + 1;
    _XL_PRINT(0, uiY, "SCORE:");
    _XL_PRINTD(6, uiY, 8, g_game.score);
    _XL_PRINT(0, uiY + 1, "ROOM:");
    uint16_t roomNum = (uint16_t)(g_game.currentRoom + 0);
    _XL_PRINTD(5, uiY + 1, 8, roomNum);
}

// Move player and check for collisions/exit
static void movePlayer(int8_t dx, int8_t dy) {
    Position newPos;
    
    // Calculate new position
    if (dx > 0) {
        if (g_game.playerPos.x >= ROOM_WIDTH - 1) return; // Hit right wall/boundary
        newPos.x = g_game.playerPos.x + 1;
    } else if (dx < 0) {
        if (g_game.playerPos.x <= 0) return; // Hit left wall/boundary
        newPos.x = g_game.playerPos.x - 1;
    } else {
        newPos.x = g_game.playerPos.x;
    }

    if (dy > 0) {
        if (g_game.playerPos.y >= ROOM_HEIGHT - 1) return; // Hit bottom wall/boundary
        newPos.y = g_game.playerPos.y + 1;
    } else if (dy < 0) {
        if (g_game.playerPos.y <= 0) return; // Hit top wall/boundary
        newPos.y = g_game.playerPos.y - 1;
    } else {
        newPos.y = g_game.playerPos.y;
    }

    // Check walkability
    if (!isWalkable(g_game.currentRoom, newPos)) return;
    
    // Check enemy collision
    if (hasEnemy(newPos)) {
        g_game.gameOver = 1;
        _XL_EXPLOSION_SOUND();
        return;
    }

    // --- OPTIMIZED DRAWING LOGIC ---
    // 1. Restore background at old position
    drawBackgroundTile(g_game.playerPos);
    
    // 2. Draw player at new position
    g_game.playerPos = newPos;
    _XL_DRAW(newPos.x, newPos.y, TILE_PLAYER, COLOR_PLAYER);

    // Check exit AFTER moving and drawing
    if (isExit(g_game.currentRoom, newPos)) {
        // Move to next room
        g_game.playerPos.x = 1;
        g_game.playerPos.y = 1;
        
        // Increment room index BEFORE checking win condition or initializing enemies
        g_game.currentRoom++;
        
        if (g_game.currentRoom >= ROOMS_PER_LEVEL) {
            g_game.won = 1;
            g_game.gameOver = 1;
            _XL_ZAP_SOUND();
            return;
        }
        
        // Initialize enemies for new room using the NEW currentRoom index
        initEnemiesForRoom(g_game.currentRoom);
        _XL_PING_SOUND();
        
        // Redraw entire screen for new room layout and entities
        drawInitialScreen();
    }
}

// Move enemies towards player (simple AI)
static void moveEnemies(void) {
    for (uint8_t i = 0; i < g_game.numEnemies; i++) {
        if (!g_game.enemies[i].alive) continue;
        
        Position ePos = g_game.enemies[i].pos;
        Position pPos = g_game.playerPos;
        
        // Determine direction to move towards player
        int8_t dx = 0, dy = 0;
        
        if (pPos.x > ePos.x) dx = 1;
        else if (pPos.x < ePos.x) dx = -1;
        
        if (pPos.y > ePos.y) dy = 1;
        else if (pPos.y < ePos.y) dy = -1; // FIXED: Compare pPos.y with ePos.y
        
        Position tryPos;
        uint8_t moved = 0;
        
        if (dx != 0) {
            if (dx > 0) {
                if (ePos.x < ROOM_WIDTH - 1) {
                    tryPos.x = ePos.x + 1;
                    tryPos.y = ePos.y;
                    if (isWalkable(g_game.currentRoom, tryPos)) {
                        // --- OPTIMIZED DRAWING LOGIC FOR ENEMY ---
                        // 1. Restore background at old position
                        drawBackgroundTile(ePos);
                        
                        // 2. Update position and draw enemy at new pos
                        g_game.enemies[i].pos = tryPos;
                        _XL_DRAW(tryPos.x, tryPos.y, TILE_ENEMY, COLOR_ENEMY);
                        moved = 1;
                    }
                }
            } else {
                if (ePos.x > 0) {
                    tryPos.x = ePos.x - 1;
                    tryPos.y = ePos.y;
                    if (isWalkable(g_game.currentRoom, tryPos)) {
                        // --- OPTIMIZED DRAWING LOGIC FOR ENEMY ---
                        drawBackgroundTile(ePos);
                        
                        g_game.enemies[i].pos = tryPos;
                        _XL_DRAW(tryPos.x, tryPos.y, TILE_ENEMY, COLOR_ENEMY);
                        moved = 1;
                    }
                }
            }
        }
        
        // If horizontal didn't move or wasn't possible, try vertical
        if (!moved && dy != 0) {
             Position vTryPos;
             if (dy > 0) {
                 if (ePos.y < ROOM_HEIGHT - 1) {
                     vTryPos.x = g_game.enemies[i].pos.x; // Use updated x if horizontal moved, or original if not
                     vTryPos.y = ePos.y + 1;
                     if (isWalkable(g_game.currentRoom, vTryPos)) {
                         // --- OPTIMIZED DRAWING LOGIC FOR ENEMY ---
                         drawBackgroundTile(ePos);
                         
                         g_game.enemies[i].pos = vTryPos;
                         _XL_DRAW(vTryPos.x, vTryPos.y, TILE_ENEMY, COLOR_ENEMY);
                     }
                 }
             } else {
                 if (ePos.y > 0) {
                     vTryPos.x = g_game.enemies[i].pos.x;
                     vTryPos.y = ePos.y - 1;
                     if (isWalkable(g_game.currentRoom, vTryPos)) {
                         // --- OPTIMIZED DRAWING LOGIC FOR ENEMY ---
                         drawBackgroundTile(ePos);
                         
                         g_game.enemies[i].pos = vTryPos;
                         _XL_DRAW(vTryPos.x, vTryPos.y, TILE_ENEMY, COLOR_ENEMY);
                     }
                 }
             }
        }
        
        // Check collision with player after move
        Position currentEnemyPos = g_game.enemies[i].pos;
        if (currentEnemyPos.x == pPos.x && currentEnemyPos.y == pPos.y) {
            g_game.gameOver = 1;
            _XL_EXPLOSION_SOUND();
            return;
        }
    }
}

int main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();
    
    while (1) {
        initGame();
        
        // Draw initial state for the first room
        drawInitialScreen();
        
        // Game loop for one playthrough
        uint8_t frameCount = 0;
        
        while (1 || !g_game.gameOver) {
            uint8_t input = _XL_INPUT();
            
            if (_XL_LEFT(input)) {
                movePlayer(-1, 0);
                _XL_TICK_SOUND();
            } else if (_XL_RIGHT(input)) {
                movePlayer(1, 0);
                _XL_TICK_SOUND();
            } else if (_XL_UP(input)) {
                movePlayer(0, -1);
                _XL_TICK_SOUND();
            } else if (_XL_DOWN(input)) {
                movePlayer(0, 1);
                _XL_TICK_SOUND();
            }
            
            // Move enemies every few frames for slower movement feel
            frameCount++;
            if (frameCount % 3 == 0) {
                moveEnemies();
            }
            
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
        
        // Show game over message
        _XL_SET_TEXT_COLOR(COLOR_ENEMY);
        uint8_t msgY = ROOM_HEIGHT + 4;
        if (g_game.won) {
            _XL_PRINT(0, msgY, "YOU WIN!");
        } else {
            _XL_PRINT(0, msgY, "GAME OVER");
        }
        
        // Wait for input to restart
        _XL_WAIT_FOR_INPUT();
    }
    
    return 0;
}
