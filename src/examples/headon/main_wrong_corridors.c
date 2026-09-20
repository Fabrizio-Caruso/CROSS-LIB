#include "cross_lib.h"

#define MAZE_W  ((uint16_t)(XSize - 4))
#define MAZE_H  ((uint16_t)(YSize - 4))
#define OX      ((uint8_t)((XSize - MAZE_W) / 2))
#define OY      (2)

/* New: all main horizontal/vertical corridors are this many tiles wide. */
#define PATH_WIDTH        (3u)
#define HALF_PATH         (1u)

/* Enemy randomly changes direction roughly once every 8 moves on average. */
#define ENEMY_RANDOM_CHANCE ((uint16_t)(4096))

static uint8_t maze[XSize][YSize];
static uint8_t dots[XSize][YSize];

static uint8_t px, py;
static uint8_t cx, cy;
static uint8_t pdir, cdir;

static uint16_t score;
static uint16_t dotsLeft;
static uint16_t shownScore;
static uint16_t shownDots;

static uint8_t gameOver;
static uint8_t win;

static short dx[4] = {0, 1, 0, -1};
static short dy[4] = {-1, 0, 1, 0};

/* ------------------------------------------------------------------ */
/* Small maze-drawing helpers                                          */
/* ------------------------------------------------------------------ */

static void markCell(uint8_t x, uint8_t y)
{
    if (x >= OX &&
        x < XSize &&
        (uint16_t)(x - OX) < MAZE_W &&
        y >= OY &&
        (uint16_t)(y - OY) < MAZE_H) {
        maze[x][y] = 1;
    }
}

static void drawThickHorizontal(uint8_t baseY, uint8_t xStart, uint8_t xEnd)
{
    uint8_t x;
    uint8_t i;

    if (xStart > xEnd) {
        return;
    }

    /* Draw a horizontal corridor that is PATH_WIDTH tiles tall. */
    for (i = 0; i < PATH_WIDTH; i++) {
        uint8_t y;

        y = baseY + ((uint16_t)i >= HALF_PATH ? (uint8_t)(i - HALF_PATH) : 0);
        if (y < HALF_PATH) {
            continue; /* Not possible, but keeps compiler quiet on C89. */
        }

        for (x = xStart; x <= xEnd; x++) {
            markCell(x, y);
        }
    }
}

static void drawThickVertical(uint8_t baseX, uint8_t yStart, uint8_t yEnd)
{
    uint8_t i;
    uint8_t y;

    if (yStart > yEnd) {
        return;
    }

    /* Draw a vertical corridor that is PATH_WIDTH tiles wide. */
    for (i = 0; i < PATH_WIDTH; i++) {
        uint8_t x;

        x = baseX + ((uint16_t)i >= HALF_PATH ? (uint8_t)(i - HALF_PATH) : 0);
        if (x < HALF_PATH) {
            continue;
        }

        for (y = yStart; y <= yEnd; y++) {
            markCell(x, y);
        }
    }
}

/* ------------------------------------------------------------------ */
/* Maze generation                                                     */
/* ------------------------------------------------------------------ */

void generateMaze(void)
{
    uint8_t x, y;
    uint8_t gv, gh;
    uint8_t v1x, v2x;
    uint8_t h1y, h2y;
    uint8_t rx;
    uint8_t ry;

    /* Clear all cells and dots first. */
    for (x = 0; x < XSize; x++) {
        for (y = 0; y < YSize; y++) {
            maze[x][y] = 0;
            dots[x][y] = 0;
        }
    }

    /*
       Gap between concentric path rectangles.
       Because the corridors are now 3 tiles wide, we prefer a larger gap
       when there is room, otherwise fall back to the old minimum.
    */
    if (MAZE_W / 7 >= 5) {
        gv = (uint8_t)(MAZE_W / 7);
    } else if (MAZE_W >= 10) {
        gv = 4;
    } else {
        gv = 2;
    }

    if (MAZE_H / 7 >= 5) {
        gh = (uint8_t)(MAZE_H / 7);
    } else if (MAZE_H >= 10) {
        gh = 4;
    } else {
        gh = 2;
    }

    /* Positions of the vertical path lines, in absolute screen x. */
    v1x = (uint8_t)(OX + MAZE_W / 3);
    v2x = (uint8_t)(OX + (2 * MAZE_W) / 3);

    /* Positions of the horizontal path lines, in absolute screen y. */
    h1y = (uint8_t)(OY + MAZE_H / 3);
    h2y = (uint8_t)(OY + (2 * MAZE_H) / 3);

    {
        uint8_t leftX;
        uint8_t rightX;
        uint8_t topY;
        uint8_t bottomY;

        /* Outer rectangle, 3 tiles thick. */
        if (MAZE_W > 0 && MAZE_H > 0) {
            leftX = OX;
            rightX = (uint8_t)(OX + MAZE_W - 1);
            topY = OY;
            bottomY = (uint8_t)(OY + MAZE_H - 1);

            // /* Top and bottom horizontal corridors. */
            // if (MAZE_W >= PATH_WIDTH) {
                // drawThickHorizontal(topY, leftX, rightX);
                // drawThickHorizontal(bottomY, leftX, rightX);
            // } else if (leftX <= rightX && topY <= bottomY) {
                // /* Very small map: mark whatever fits. */
                // uint8_t yy;

                // for (yy = topY; yy <= bottomY; yy++) {
                    // drawThickHorizontal(yy, leftX, rightX);
                // }
            // }

            // if (MAZE_H >= PATH_WIDTH) {
                // /* Left and right vertical corridors. */
                // drawThickVertical(leftX, topY, bottomY);
                // drawThickVertical(rightX, topY, bottomY);
            // } else if (leftX <= rightX && topY <= bottomY) {
                // uint8_t xx;

                // for (xx = leftX; xx <= rightX; xx++) {
                    // drawThickVertical(xx, topY, bottomY);
                // }
            // }
        }
    }

    /* Middle rectangle path. */
    if (MAZE_W >= gv * 2 + PATH_WIDTH && MAZE_H >= gh * 2 + PATH_WIDTH) {
        uint8_t mLeft;
        uint8_t mRight;
        uint8_t mTop;
        uint8_t mBottom;

        mLeft = (uint8_t)(OX + gv);
        mRight = (uint8_t)(OX + MAZE_W - 1 - gv);
        mTop = (uint8_t)(OY + gh);
        mBottom = (uint8_t)(OY + MAZE_H - 1 - gh);

        if (mLeft <= mRight && mTop <= mBottom) {
            drawThickHorizontal(mTop, mLeft, mRight);
            drawThickHorizontal(mBottom, mLeft, mRight);
            drawThickVertical(mLeft, mTop, mBottom);
            drawThickVertical(mRight, mTop, mBottom);
        }
    }

    /* Inner rectangle path. */
    if (MAZE_W >= gv * 4 + PATH_WIDTH && MAZE_H >= gh * 2 + PATH_WIDTH) {
        uint8_t iLeft;
        uint8_t iRight;
        uint8_t iTop;
        uint8_t iBottom;

        iLeft = (uint8_t)(OX + gv * 2);
        iRight = (uint8_t)(OX + MAZE_W - 1 - gv * 2);
        iTop = (uint8_t)(OY + gh * 2);
        iBottom = (uint8_t)(OY + MAZE_H - 1 - gh * 2);

        if (iLeft <= iRight && iTop <= iBottom) {
            drawThickHorizontal(iTop, iLeft, iRight);
            drawThickHorizontal(iBottom, iLeft, iRight);
            drawThickVertical(iLeft, iTop, iBottom);
            drawThickVertical(iRight, iTop, iBottom);
        }
    }


    /* Put dots on empty walkable cells only. */
    for (x = OX; x < XSize && (uint16_t)(x - OX) < MAZE_W; x++) {
        for (y = OY; y < YSize && (uint16_t)(y - OY) < MAZE_H; y++) {
            if (maze[x][y]) {
                dots[x][y] = 1;
            }
        }
    }

    /* Use Cross-API random numbers to create a little variation in dot placement. */
    {
        uint8_t i;

        for (i = 0; i < 24; i++) {
            uint16_t r = _XL_RAND();

            rx = (uint8_t)(OX + (r % MAZE_W));
            ry = (uint8_t)(OY + ((r / 137) % MAZE_H));

            if (rx < XSize && ry < YSize && maze[rx][ry]) {
                dots[rx][ry] = 0;
            }
        }
    }
}

void setStarts(void)
{
    /* Player: outer rectangle, top-left corner area, heading right. */
    px = OX;
    py = OY;
    pdir = 1;

    /* Enemy: bottom-right corner of the map, heading up along the right edge. */
    cx = (uint8_t)(OX + MAZE_W - 1);
    cy = (uint8_t)(OY + MAZE_H - 1);
    cdir = 0;

    /*
       Make sure both starts are passable even if the map is very small or
       some thick line did not fully reach that exact corner cell.
    */
    markCell(px, py);
    markCell(cx, cy);
}

uint8_t passable(uint8_t x, uint8_t y)
{
    if ((uint16_t)x < (uint16_t)OX ||
        (uint16_t)x >= (uint16_t)(OX + MAZE_W)) {
        return 0;
    }

    if (y < OY || (uint16_t)(y - OY) >= MAZE_H) {
        return 0;
    }

    return maze[x][y];
}

void updateHud(void)
{
    uint8_t hx;

    hx = (XSize > 5) ? 5 : 0;

    if (score != shownScore) {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(hx, 0, 1, score);
        shownScore = score;
    }

    if (dotsLeft != shownDots) {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(hx, 1, 1, dotsLeft);
        shownDots = dotsLeft;
    }
}

void drawBoard(void)
{
    uint8_t x, y;

    for (x = OX; x < XSize && (uint16_t)(x - OX) < MAZE_W; x++) {
        for (y = OY; y < YSize && (uint16_t)(y - OY) < MAZE_H; y++) {
            if (!maze[x][y]) {
                /* Background / wall area */
                _XL_DRAW(x, y, _TILE_0, _XL_CYAN);
            } else if (dots[x][y]) {
                /* Walkable path with a dot */
                _XL_DRAW(x, y, _TILE_1, _XL_YELLOW);
            }
        }
    }
}

void newGame(void)
{
    uint8_t x, y;
    uint8_t hx;

    generateMaze();
    setStarts();

    /* Clear the dots under both starting positions. */
    dots[px][py] = 0;
    dots[cx][cy] = 0;

    score = 0;
    shownScore = 0;
    dotsLeft = 0;

    for (x = OX; x < XSize && (uint16_t)(x - OX) < MAZE_W; x++) {
        for (y = OY; y < YSize && (uint16_t)(y - OY) < MAZE_H; y++) {
            if (dots[x][y]) {
                dotsLeft++;
            }
        }
    }

    shownDots = dotsLeft;
    gameOver = 0;
    win = 0;

    _XL_CLEAR_SCREEN();
    drawBoard();

    /* Draw the two cars. */
    _XL_DRAW(px, py, _TILE_2, _XL_GREEN);
    _XL_DRAW(cx, cy, _TILE_3, _XL_RED);

    hx = (XSize > 5) ? 5 : 0;

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "SCORE");
    _XL_PRINTD(hx, 0, 1, score);
    _XL_PRINT(0, 1, "DOTS");
    _XL_PRINTD(hx, 1, 1, dotsLeft);

    if (dotsLeft == 0) {
        gameOver = 1;
        win = 1;
    }
}

void moveCars(uint8_t input)
{
    uint8_t desired;
    uint8_t d, found;
    uint8_t oldPx, oldPy, oldCx, oldCy;
    uint8_t newPx, newPy, newCx, newCy;
    uint8_t crash, dotVisible;

    if (gameOver) {
        return;
    }

    oldPx = px;
    oldPy = py;
    oldCx = cx;
    oldCy = cy;

    /* --- Player: read desired direction from Cross input --- */
    if (_XL_UP(input)) {
        desired = 0;
    } else if (_XL_RIGHT(input)) {
        desired = 1;
    } else if (_XL_DOWN(input)) {
        desired = 2;
    } else if (_XL_LEFT(input)) {
        desired = 3;
    } else {
        desired = pdir;
    }

    /* Player can turn in any direction, including U-turns, if passable. */
    if (desired != pdir &&
        passable((uint8_t)(px + dx[desired]), (uint8_t)(py + dy[desired]))) {
        pdir = desired;
    }

    /* --- Player: move, turn clockwise on wall --- */
    newPx = (uint8_t)(px + dx[pdir]);
    newPy = (uint8_t)(py + dy[pdir]);

    if (!passable(newPx, newPy)) {
        found = 0;
        for (d = 1; d < 4; d++) {
            uint8_t tryDir = (uint8_t)((pdir + d) & 3);

            if (passable((uint8_t)(px + dx[tryDir]), (uint8_t)(py + dy[tryDir]))) {
                pdir = tryDir;
                found = 1;
                break;
            }
        }

        if (!found) {
            gameOver = 1;
            win = 0;
            _XL_EXPLOSION_SOUND();
            return;
        }

        newPx = (uint8_t)(px + dx[pdir]);
        newPy = (uint8_t)(py + dy[pdir]);
    }

    /* --- Enemy: random direction changes from time to time --- */
    {
        uint16_t r = _XL_RAND();

        if (r < ENEMY_RANDOM_CHANCE) {
            uint8_t newDir;
            uint8_t i;

            /*
               Pick a random direction and use it only if that cell is passable.
               This gives the enemy occasional non-deterministic behavior while
               still guaranteeing it does not drive into a wall immediately.
             */
            newDir = (uint8_t)(r & 3);

            found = 0;
            for (i = 0; i < 4; i++) {
                uint8_t candidate = (uint8_t)((newDir + i) & 3);

                if (passable((uint8_t)(cx + dx[candidate]),
                             (uint8_t)(cy + dy[candidate]))) {
                    cdir = candidate;
                    found = 1;
                    break;
                }
            }

            /* If no random choice worked, keep the old direction. */
        }
    }

    /* --- Enemy: move, turn counter-clockwise on wall if still blocked --- */
    if (!passable((uint8_t)(cx + dx[cdir]), (uint8_t)(cy + dy[cdir]))) {
        found = 0;
        for (d = 1; d < 4; d++) {
            uint8_t tryDir = (uint8_t)((cdir - d) & 3);

            if (passable((uint8_t)(cx + dx[tryDir]), (uint8_t)(cy + dy[tryDir]))) {
                cdir = tryDir;
                found = 1;
                break;
            }
        }

        if (!found) {
            gameOver = 1;
            win = 0;
            _XL_EXPLOSION_SOUND();
            return;
        }
    }

    newCx = (uint8_t)(cx + dx[cdir]);
    newCy = (uint8_t)(cy + dy[cdir]);

    /* --- Collision detection --- */
    crash = 0;

    if (newPx == newCx && newPy == newCy) {
        crash = 1;
    }

    if ((newPx == oldCx && newPy == oldCy) &&
        (newCx == oldPx && newCy == oldPy)) {
        crash = 1;
    }

    if (crash) {
        gameOver = 1;
        win = 0;
        _XL_EXPLOSION_SOUND();
        return;
    }

    /* --- Rendering: remove old tiles, restore dots, draw new positions --- */
    _XL_DELETE(oldPx, oldPy);
    _XL_DELETE(oldCx, oldCy);

    dotVisible = 1;
    if (dots[oldPx][oldPy]) {
        if ((newPx == oldPx && newPy == oldPy) ||
            (newCx == oldPx && newCy == oldPy)) {
            dotVisible = 0;
        }

        if (dotVisible) {
            _XL_DRAW(oldPx, oldPy, _TILE_1, _XL_YELLOW);
        }
    }

    dotVisible = 1;
    if (dots[oldCx][oldCy]) {
        if ((newPx == oldCx && newPy == oldCy) ||
            (newCx == oldCx && newCy == oldCy)) {
            dotVisible = 0;
        }

        if (dotVisible) {
            _XL_DRAW(oldCx, oldCy, _TILE_1, _XL_YELLOW);
        }
    }

    /* --- Collect dots at player's new position --- */
    if (dots[newPx][newPy]) {
        dotVisible = 1;

        if ((oldPx == newPx && oldPy == newPy) ||
            (oldCx == newPx && oldCy == newPy)) {
            dotVisible = 0;
        }

        if (dotVisible) {
            _XL_DELETE(newPx, newPy);
        }

        dots[newPx][newPy] = 0;
        score++;
        dotsLeft--;
        _XL_PING_SOUND();

        if (dotsLeft == 0) {
            win = 1;
            gameOver = 1;
        }
    }

    /* --- Collect dot at enemy's new position --- */
    if (dots[newCx][newCy]) {
        dotVisible = 1;

        if ((oldPx == newCx && oldPy == newCy) ||
            (oldCx == newCx && oldCy == newCy)) {
            dotVisible = 0;
        }

        if (dotVisible) {
            _XL_DELETE(newCx, newCy);
        }

        dots[newCx][newCy] = 0;
    }

    _XL_DRAW(newPx, newPy, _TILE_2, _XL_GREEN);
    _XL_DRAW(newCx, newCy, _TILE_3, _XL_RED);

    px = newPx;
    py = newPy;
    cx = newCx;
    cy = newCy;

    updateHud();

    if (win) {
        _XL_ZAP_SOUND();
    }
}

void startScreen(void)
{
    uint8_t tx, ty;

    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_WHITE);

    tx = (XSize >= 7) ? (uint8_t)(XSize / 2 - 3) : 0;
    ty = (YSize >= 3) ? (uint8_t)(YSize / 2 - 1) : 0;
    _XL_PRINT(tx, ty, "HEAD ON");

    tx = (XSize >= 10) ? (uint8_t)(XSize / 2 - 5) : 0;
    ty = (YSize >= 5) ? (uint8_t)(YSize / 2 + 2) : 0;
    _XL_PRINT(tx, ty, "PRESS FIRE");
}

void showGameOver(uint8_t won)
{
    uint8_t tx, ty;

    _XL_SET_TEXT_COLOR(won ? (uint8_t)_XL_GREEN : (uint8_t)_XL_RED);

    tx = (XSize >= 9) ? (uint8_t)(XSize / 2 - 4) : 0;
    ty = (YSize >= 3) ? (uint8_t)(YSize / 2 - 1) : 0;

    if (won) {
        _XL_PRINT(tx, ty, "YOU WIN");
    } else {
        _XL_PRINT(tx, ty, "GAME OVER");
    }

    _XL_SET_TEXT_COLOR(_XL_WHITE);

    tx = (XSize >= 10) ? (uint8_t)(XSize / 2 - 5) : 0;
    ty = (YSize >= 5) ? (uint8_t)(YSize / 2 + 2) : 0;
    _XL_PRINT(tx, ty, "PRESS FIRE");
}

int main(void)
{
    uint8_t input;
    uint8_t playing;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        startScreen();

        /* Wait for fire to begin. */
        do {
            input = _XL_INPUT();
            if (_XL_FIRE(input)) {
                break;
            }
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        } while (1);

        newGame();

        playing = 1;

        while (playing) {
            input = _XL_INPUT();
            moveCars(input);

            if (gameOver) {
                playing = 0;
            }

            /* Timing: keep the original frame pacing. */
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        showGameOver(win);

        do {
            input = _XL_INPUT();
            if (_XL_FIRE(input)) {
                break;
            }
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        } while (1);

        _XL_CLEAR_SCREEN();
    }

    return 0;
}
