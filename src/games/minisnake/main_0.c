
#include "cross_lib.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BOARD_X_OFFSET 5
#define BOARD_Y_OFFSET 2
#define TILE_EMPTY 0
#define TILE_WALL 27
#define TILE_BLOCK 28

/* Tetromino definitions */
#define NUM_PIECES 7
#define PIECE_SIZE 4

static const uint8_t PIECES[NUM_PIECES][PIECE_SIZE][PIECE_SIZE] = {
    /* I */
    {{0,0,0,0},
     {1,1,1,1},
     {0,0,0,0},
     {0,0,0,0}},
    /* O */
    {{1,1,0,0},
     {1,1,0,0},
     {0,0,0,0},
     {0,0,0,0}},
    /* T */
    {{0,1,0,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
    /* S */
    {{0,1,1,0},
     {1,1,0,0},
     {0,0,0,0},
     {0,0,0,0}},
    /* Z */
    {{1,1,0,0},
     {0,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
    /* J */
    {{1,0,0,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
    /* L */
    {{0,0,1,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}}
};

static uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];
static uint8_t currentPiece[PIECE_SIZE][PIECE_SIZE];
static uint8_t pieceX, pieceY;
static uint8_t currentPieceType;
static uint8_t nextPieceType;
static uint16_t score;
static uint8_t gameOver;
static uint8_t dropCounter;

// void _XL_INIT_GRAPHICS(void);
// void _XL_INIT_INPUT(void);
// void _XL_INIT_SOUND(void);

void initBoard(void) {
    short i, j;
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = TILE_EMPTY;
        }
    }
}

void copyPiece(uint8_t pieceType) {
    short i, j;
    for (i = 0; i < PIECE_SIZE; i++) {
        for (j = 0; j < PIECE_SIZE; j++) {
            currentPiece[i][j] = PIECES[pieceType][i][j];
        }
    }
}

uint8_t checkCollision(short newX, short newY) {
    short i, j;
    for (i = 0; i < PIECE_SIZE; i++) {
        for (j = 0; j < PIECE_SIZE; j++) {
            if (currentPiece[i][j]) {
                short boardX = newX + j;
                short boardY = newY + i;
                if (boardX < 0 || boardX >= BOARD_WIDTH || 
                    boardY >= BOARD_HEIGHT) {
                    return 1;
                }
                if (boardY >= 0 && board[boardY][boardX] != TILE_EMPTY) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void lockPiece(void) {
    short i, j;
    for (i = 0; i < PIECE_SIZE; i++) {
        for (j = 0; j < PIECE_SIZE; j++) {
            if (currentPiece[i][j]) {
                short boardX = pieceX + j;
                short boardY = pieceY + i;
                if (boardY >= 0 && boardY < BOARD_HEIGHT && 
                    boardX >= 0 && boardX < BOARD_WIDTH) {
                    board[boardY][boardX] = currentPieceType + 1;
                }
            }
        }
    }
}

void clearLines(void) {
    short i, j, k;
    short linesCleared = 0;
    
    for (i = BOARD_HEIGHT - 1; i >= 0; i--) {
        short fullLine = 1;
        for (j = 0; j < BOARD_WIDTH; j++) {
            if (board[i][j] == TILE_EMPTY) {
                fullLine = 0;
                break;
            }
        }
        
        if (fullLine) {
            linesCleared++;
            for (k = i; k > 0; k--) {
                for (j = 0; j < BOARD_WIDTH; j++) {
                    board[k][j] = board[k-1][j];
                }
            }
            for (j = 0; j < BOARD_WIDTH; j++) {
                board[0][j] = TILE_EMPTY;
            }
            i++;
        }
    }
    
    if (linesCleared > 0) {
        score += (uint16_t)(linesCleared * 100);
        _XL_TOCK_SOUND();
    }
}

void spawnPiece(void) {
    currentPieceType = nextPieceType;
    nextPieceType = (uint8_t)(_XL_RAND() % NUM_PIECES);
    
    copyPiece(currentPieceType);
    pieceX = 3;
    pieceY = 0;
    
    if (checkCollision(pieceX, pieceY)) {
        gameOver = 1;
    }
}

void rotatePiece(void) {
    uint8_t temp[PIECE_SIZE][PIECE_SIZE];
    short i, j;
    
    for (i = 0; i < PIECE_SIZE; i++) {
        for (j = 0; j < PIECE_SIZE; j++) {
            temp[j][PIECE_SIZE - 1 - i] = currentPiece[i][j];
        }
    }
    
    for (i = 0; i < PIECE_SIZE; i++) {
        for (j = 0; j < PIECE_SIZE; j++) {
            currentPiece[i][j] = temp[i][j];
        }
    }
    
    if (checkCollision(pieceX, pieceY)) {
        /* Rotate back */
        for (i = 0; i < PIECE_SIZE; i++) {
            for (j = 0; j < PIECE_SIZE; j++) {
                temp[i][PIECE_SIZE - 1 - j] = currentPiece[j][i];
            }
        }
        for (i = 0; i < PIECE_SIZE; i++) {
            for (j = 0; j < PIECE_SIZE; j++) {
                currentPiece[i][j] = temp[i][j];
            }
        }
    }
}

void drawBoard(void) {
    short i, j;
    
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            uint8_t tile;
            uint8_t color;
            
            if (board[i][j] == TILE_EMPTY) {
                tile = _TILE_0;
                color = _XL_WHITE;
            } else {
                tile = _TILE_1 + ((board[i][j] - 1) % 4);
                color = _XL_CYAN + ((board[i][j] - 1) % 4);
            }
            
            _XL_DRAW(BOARD_X_OFFSET + j, BOARD_Y_OFFSET + i, tile, color);
        }
    }
}

void drawPiece(void) {
    short i, j;
    
    for (i = 0; i < PIECE_SIZE; i++) {
        for (j = 0; j < PIECE_SIZE; j++) {
            if (currentPiece[i][j]) {
                short boardX = pieceX + j;
                short boardY = pieceY + i;
                
                if (boardY >= 0) {
                    _XL_DRAW(BOARD_X_OFFSET + boardX, 
                            BOARD_Y_OFFSET + boardY,
                            _TILE_1 + (currentPieceType % 4),
                            _XL_CYAN + (currentPieceType % 4));
                }
            }
        }
    }
}

void erasePiece(void) {
    short i, j;
    
    for (i = 0; i < PIECE_SIZE; i++) {
        for (j = 0; j < PIECE_SIZE; j++) {
            if (currentPiece[i][j]) {
                short boardX = pieceX + j;
                short boardY = pieceY + i;
                
                if (boardY >= 0) {
                    _XL_DRAW(BOARD_X_OFFSET + boardX,
                            BOARD_Y_OFFSET + boardY,
                            _TILE_0,
                            _XL_WHITE);
                }
            }
        }
    }
}

void drawNextPiece(void) {
    short i, j;
    uint8_t previewX = BOARD_X_OFFSET + BOARD_WIDTH + 3;
    uint8_t previewY = BOARD_Y_OFFSET + 2;
    
    for (i = 0; i < PIECE_SIZE; i++) {
        for (j = 0; j < PIECE_SIZE; j++) {
            if (PIECES[nextPieceType][i][j]) {
                _XL_DRAW(previewX + j, previewY + i,
                        _TILE_1 + (nextPieceType % 4),
                        _XL_CYAN + (nextPieceType % 4));
            } else {
                _XL_DRAW(previewX + j, previewY + i,
                        _TILE_0, _XL_WHITE);
            }
        }
    }
}

void drawScore(void) {
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(BOARD_X_OFFSET + BOARD_WIDTH + 2, BOARD_Y_OFFSET + 8, 
              "SCORE");
    _XL_PRINTD(BOARD_X_OFFSET + BOARD_WIDTH + 2, BOARD_Y_OFFSET + 9, 
               1, score);
}

void drawGameOver(void) {
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(BOARD_X_OFFSET + 2, BOARD_Y_OFFSET + 8, "GAME OVER");
    _XL_PRINT(BOARD_X_OFFSET + 2, BOARD_Y_OFFSET + 10, 
              "PRESS FIRE");
}

void main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();
    
    while (1) {
        uint8_t input;
        uint8_t needRedraw = 1;
        uint8_t moveCounter = 0;
        
        score = 0;
        gameOver = 0;
        dropCounter = 0;
        
        initBoard();
        nextPieceType = (uint8_t)(_XL_RAND() % NUM_PIECES);
        spawnPiece();
        
        _XL_CLEAR_SCREEN();
        drawBoard();
        drawNextPiece();
        drawScore();
        
        while (!gameOver) {
            input = _XL_INPUT();
            
            if (_XL_LEFT(input)) {
                erasePiece();
                if (!checkCollision(pieceX - 1, pieceY)) {
                    pieceX--;
                }
                drawPiece();
            }
            
            if (_XL_RIGHT(input)) {
                erasePiece();
                if (!checkCollision(pieceX + 1, pieceY)) {
                    pieceX++;
                }
                drawPiece();
            }
            
            if (_XL_DOWN(input)) {
                erasePiece();
                if (!checkCollision(pieceX, pieceY + 1)) {
                    pieceY++;
                }
                drawPiece();
            }
            
            if (_XL_UP(input) || _XL_FIRE(input)) {
                erasePiece();
                rotatePiece();
                drawPiece();
                _XL_PING_SOUND();
            }
            
            moveCounter++;
            if (moveCounter >= 10) {
                moveCounter = 0;
                erasePiece();
                if (!checkCollision(pieceX, pieceY + 1)) {
                    pieceY++;
                } else {
                    drawPiece();
                    lockPiece();
                    clearLines();
                    drawBoard();
                    spawnPiece();
                    drawNextPiece();
                    drawScore();
                    if (!gameOver) {
                        drawPiece();
                    }
                }
                if (!gameOver) {
                    drawPiece();
                }
            }
            
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
        
        drawGameOver();
        _XL_EXPLOSION_SOUND();
        _XL_WAIT_FOR_INPUT();
    }
}
