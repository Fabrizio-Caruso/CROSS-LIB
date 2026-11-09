#include "cross_lib.h"

// Game constants
#define ROOMS_PER_LEVEL 64
#define MAX_ENEMIES 8
#define ROOM_WIDTH 16
#define ROOM_HEIGHT 16
#define NUM_PATTERNS 42 // Total number of unique room layouts

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

// Helper: Get layout type for a room index (cycles through 42 types)
static uint8_t getRoomType(uint8_t roomIndex) {
    return (roomIndex % NUM_PATTERNS);
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

    uint8_t cx = ROOM_WIDTH / 2;
    uint8_t cy = ROOM_HEIGHT / 2;

    // --- Patterns 0-15 (Original) ---
    
    if (roomType == 0) return 0; // Open Room

    if (roomType == 1) { // Central Pillar
        if ((x >= cx - 1 && x <= cx + 1) && (y >= cy - 1 && y <= cy + 1)) return 1;
        return 0;
    }

    if (roomType == 2) { // Cross with gaps
        if (x == cx && y != cy && y != cy - 1 && y != cy + 1) return 1;
        if (y == cy && x != cx && x != cx - 1 && x != cx + 1) return 1;
        return 0;
    }

    if (roomType == 3) { // Diagonal Sparse
        if ((x + y) % 8 == 0 && x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    if (roomType == 4) { // Checkerboard Sparse
        if ((x % 4 == 2) && (y % 4 == 2)) return 1;
        return 0;
    }

    if (roomType == 5) { // Vertical Corridors
        if ((x == 3 || x == 7 || x == 11) && y > 2 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    if (roomType == 6) { // Horizontal Corridors
        if ((y == 3 || y == 7 || y == 11) && x > 2 && x < ROOM_WIDTH - 3) return 1;
        return 0;
    }

    if (roomType == 7) { // Central Cross Thick
        if (x == cx && y >= cy - 3 && y <= cy + 3) return 1;
        if (y == cy && x >= cx - 3 && x <= cx + 3) return 1;
        return 0;
    }

    if (roomType == 8) { // Diagonal Split
        if ((x + y) == (ROOM_WIDTH - 2)) {
            if (x % 3 != 0 && y % 3 != 0) return 1;
        }
        return 0;
    }

    if (roomType == 9) { // Side Walls Gaps
        if (x <= 2 && y % 3 != 0 && y > 1 && y < ROOM_HEIGHT - 2) return 1;
        if (x >= ROOM_WIDTH - 3 && y % 3 != 0 && y > 1 && y < ROOM_HEIGHT - 2) return 1;
        return 0;
    }

    if (roomType == 10) { // Four Pillars
        if ((x >= 3 && x <= 5 && y >= 3 && y <= 5)) return 1;
        if ((x >= cx + 1 && x <= cx + 3 && y >= 3 && y <= 5)) return 1;
        if ((x >= 3 && x <= 5 && y >= cy + 1 && y <= cy + 3)) return 1;
        if ((x >= cx + 1 && x <= cx + 3 && y >= cy + 1 && y <= cy + 3)) return 1;
        return 0;
    }

    if (roomType == 11) { // Random Hash
        uint8_t hash = (x * 7 + y * 13) % 16;
        if (hash < 4 && x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    if (roomType == 12) { // Thick Cross Center
        if ((x >= cx - 1 && x <= cx + 1) && y == cy) return 1;
        if ((y >= cy - 1 && y <= cy + 1) && x == cx) return 1;
        return 0;
    }

    if (roomType == 13) { // Diagonal Stripes
        uint8_t diff = (x > y) ? (x - y) : (y - x);
        if (diff % 6 < 2 && x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    if (roomType == 14) { // Concentric Rings
        uint8_t outerDistX = x >= cx - 3 ? (x - (cx - 3)) : ((cx - 3) - x);
        uint8_t outerDistY = y >= cy - 3 ? (y - (cy - 3)) : ((cy - 3) - y);
        if ((outerDistX == 3 && outerDistY < 3) || (outerDistY == 3 && outerDistX < 3)) {
            uint8_t perimeterPos = (x + y) % 4;
            if (perimeterPos != 0) return 1;
        }
        return 0;
    }

    if (roomType == 15) { // Zigzag
        uint8_t zig = (y / 2) % 2;
        if ((x % 4 == 0 && zig == 1) || (x % 4 == 2 && zig == 0)) {
             if (x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        }
        return 0;
    }

    // --- Patterns 16-41 (New) ---

    if (roomType == 16) { // Top Wall with Gaps
        if (y <= 2 && x % 4 != 0 && x > 1 && x < ROOM_WIDTH - 2) return 1;
        return 0;
    }

    if (roomType == 17) { // Bottom Wall with Gaps
        if (y >= ROOM_HEIGHT - 3 && x % 4 != 0 && x > 1 && x < ROOM_WIDTH - 2) return 1;
        return 0;
    }

    if (roomType == 18) { // Left Wall with Gaps
        if (x <= 2 && y % 4 != 0 && y > 1 && y < ROOM_HEIGHT - 2) return 1;
        return 0;
    }

    if (roomType == 19) { // Right Wall with Gaps
        if (x >= ROOM_WIDTH - 3 && y % 4 != 0 && y > 1 && y < ROOM_HEIGHT - 2) return 1;
        return 0;
    }

    if (roomType == 20) { // Top-Left L Shape
        if ((x <= 3 && y <= 3)) return 1;
        return 0;
    }

    if (roomType == 21) { // Bottom-Right L Shape
        if ((x >= ROOM_WIDTH - 4 && y >= ROOM_HEIGHT - 4)) return 1;
        return 0;
    }

    if (roomType == 22) { // Top-Right L Shape
        if ((x >= ROOM_WIDTH - 4 && y <= 3)) return 1;
        return 0;
    }

    if (roomType == 23) { // Bottom-Left L Shape
        if ((x <= 3 && y >= ROOM_HEIGHT - 4)) return 1;
        return 0;
    }

    if (roomType == 24) { // Central Diamond
        uint8_t dx = x > cx ? (x - cx) : (cx - x);
        uint8_t dy = y > cy ? (y - cy) : (cy - y);
        if ((dx + dy) <= 3 && (dx + dy) >= 1) return 1; // Diamond shape with hole in center? No, filled diamond.
        // Let's make it a hollow diamond border for variety
        if ((dx + dy) == 3) return 1; 
        return 0;
    }

    if (roomType == 25) { // Vertical Bars Sparse
        if (x % 6 == 3 && y > 2 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    if (roomType == 26) { // Horizontal Bars Sparse
        if (y % 6 == 3 && x > 2 && x < ROOM_WIDTH - 3) return 1;
        return 0;
    }

    if (roomType == 27) { // Grid of Dots
        if ((x % 5 == 2) && (y % 5 == 2)) return 1;
        return 0;
    }

    if (roomType == 28) { // Diagonal Crosses
        if (((x + y) % 7 == 3) || ((x - y + ROOM_WIDTH) % 7 == 3)) {
             if (x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        }
        return 0;
    }

    if (roomType == 29) { // Inner Box Border
        uint8_t bx = cx - 4, by = cy - 4;
        uint8_t bw = 8, bh = 8;
        if ((x >= bx && x <= bx + bw) || (y >= by && y <= by + bh)) {
            if (x == bx || x == bx + bw || y == by || y == by + bh) return 1;
        }
        return 0;
    }

    if (roomType == 30) { // Outer Box Border
        uint8_t bx = 2, by = 2;
        uint8_t bw = ROOM_WIDTH - 5, bh = ROOM_HEIGHT - 5;
        if ((x >= bx && x <= bx + bw) || (y >= by && y <= by + bh)) {
            if (x == bx || x == bx + bw || y == by || y == by + bh) return 1;
        }
        return 0;
    }

    if (roomType == 31) { // Random Walls (Dense)
        uint8_t hash = (x * 11 + y * 7) % 20;
        if (hash < 5 && x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    if (roomType == 32) { // Random Walls (Sparse)
        uint8_t hash = (x * 5 + y * 17) % 30;
        if (hash < 4 && x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    if (roomType == 33) { // Vertical Splits
        if ((x == cx - 2 || x == cx + 2) && y > 2 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    if (roomType == 34) { // Horizontal Splits
        if ((y == cy - 2 || y == cy + 2) && x > 2 && x < ROOM_WIDTH - 3) return 1;
        return 0;
    }

    if (roomType == 35) { // Corner Blocks
        if ((x <= 4 && y <= 4)) return 1;
        if ((x >= ROOM_WIDTH - 5 && y >= ROOM_HEIGHT - 5)) return 1;
        return 0;
    }

    if (roomType == 36) { // Diagonal Blocks
        if ((x + y <= 8) || (x + y >= ROOM_WIDTH + ROOM_HEIGHT - 9)) {
             if (x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        }
        return 0;
    }

    if (roomType == 37) { // Central Plus Thick
        if ((x >= cx - 2 && x <= cx + 2) && y == cy) return 1;
        if ((y >= cy - 2 && y <= cy + 2) && x == cx) return 1;
        return 0;
    }

    if (roomType == 38) { // Scattered Pillars
        if ((x % 7 == 3 && y % 7 == 3)) return 1;
        if ((x % 7 == 5 && y % 7 == 5)) return 1;
        return 0;
    }

    if (roomType == 39) { // Wavy Walls
        uint8_t wave = (y / 2) % 4;
        if ((x % 6 == wave) && x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        return 0;
    }

    if (roomType == 40) { // Inverted Ls
        if ((x <= 3 && y >= ROOM_HEIGHT - 4)) return 1;
        if ((x >= ROOM_WIDTH - 4 && y <= 3)) return 1;
        return 0;
    }

    if (roomType == 41) { // Complex Mix
        uint8_t hash = (x * 3 + y * 5) % 25;
        if (hash < 6 && x > 2 && y > 2 && x < ROOM_WIDTH - 3 && y < ROOM_HEIGHT - 3) return 1;
        // Add a central obstacle
        if ((x >= cx - 1 && x <= cx + 1) && (y >= cy - 1 && y <= cy + 1)) return 1;
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
    g_game.currentRoom = 36;
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
    uint16_t roomNum = (uint16_t)(g_game.currentRoom);
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
