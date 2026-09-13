
#include "cross_lib.h"

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

uint8_t passable(uint8_t x, uint8_t y)
{
    if (x >= XSize || y >= YSize) {
        return 0;
    }
    return maze[x][y];
}

void generateMaze(void)
{
    uint8_t x, y;
    uint8_t v1, v2, v3, v4;
    uint8_t h1, h2, h3, h4;
    uint8_t cell;

    v1 = 0;
    v2 = (uint8_t)(XSize / 3);
    v3 = (uint8_t)((2 * XSize) / 3);
    v4 = (XSize > 0) ? (uint8_t)(XSize - 1) : 0;

    h1 = 0;
    h2 = (uint8_t)(YSize / 3);
    h3 = (uint8_t)((2 * YSize) / 3);
    h4 = (YSize > 0) ? (uint8_t)(YSize - 1) : 0;

    for (x = 0; x < XSize; x++) {
        for (y = 0; y < YSize; y++) {
            cell = 0;

            if (x == v1 || x == v2 || x == v3 || x == v4) {
                cell = 1;
            }
            if (y == h1 || y == h2 || y == h3 || y == h4) {
                cell = 1;
            }

            maze[x][y] = cell;
            dots[x][y] = cell;
        }
    }
}

void setStarts(void)
{
    uint8_t x, y;
    uint8_t found;

    px = 0;
    py = 0;
    found = 0;

    for (x = 0; x < XSize; x++) {
        for (y = 0; y < YSize; y++) {
            if (passable(x, y)) {
                px = x;
                py = y;
                found = 1;
                break;
            }
        }
        if (found) {
            break;
        }
    }

    cx = 0;
    cy = 0;
    found = 0;

    for (x = 0; x < XSize; x++) {
        for (y = 0; y < YSize; y++) {
            if (passable(x, y) && !(x == px && y == py)) {
                cx = x;
                cy = y;
                found = 1;
            }
        }
    }

    if (!found) {
        cx = px;
        cy = py;
    }
}

void updateHud(void)
{
    uint8_t hx, hy;

    hx = (XSize > 5) ? 5 : 0;
    hy = (YSize > 1) ? 1 : 0;

    if (score != shownScore) {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(hx, 0, 1, score);
        shownScore = score;
    }

    if (dotsLeft != shownDots) {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(hx, hy, 1, dotsLeft);
        shownDots = dotsLeft;
    }
}

void drawBoard(void)
{
    uint8_t x, y;

    for (x = 0; x < XSize; x++) {
        for (y = 0; y < YSize; y++) {
            if (!maze[x][y]) {
                _XL_DRAW(x, y, _TILE_0, _XL_CYAN);
            } else if (dots[x][y]) {
                _XL_DRAW(x, y, _TILE_1, _XL_YELLOW);
            }
        }
    }
}

void newGame(void)
{
    uint8_t x, y;
    uint8_t hx, hy;

    generateMaze();
    setStarts();

    pdir = 1;
    cdir = 3;

    dots[px][py] = 0;
    dots[cx][cy] = 0;

    score = 0;
    shownScore = 0;
    dotsLeft = 0;

    for (x = 0; x < XSize; x++) {
        for (y = 0; y < YSize; y++) {
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

    _XL_DRAW(px, py, _TILE_2, _XL_GREEN);
    _XL_DRAW(cx, cy, _TILE_3, _XL_RED);

    hx = (XSize > 5) ? 5 : 0;
    hy = (YSize > 1) ? 1 : 0;

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "SCORE");
    _XL_PRINTD(hx, 0, 1, score);
    _XL_PRINT(0, hy, "DOTS");
    _XL_PRINTD(hx, hy, 1, dotsLeft);

    if (dotsLeft == 0) {
        gameOver = 1;
        win = 1;
    }
}

void moveCars(uint8_t input)
{
    uint8_t desired, reverse, opp, reverseC;
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

    reverse = (uint8_t)((pdir + 2) & 3);

    if (desired != pdir &&
        desired != reverse &&
        passable((uint8_t)(px + dx[desired]), (uint8_t)(py + dy[desired]))) {
        pdir = desired;
    }

    newPx = (uint8_t)(px + dx[pdir]);
    newPy = (uint8_t)(py + dy[pdir]);

    if (!passable(newPx, newPy)) {
        if (desired != reverse &&
            passable((uint8_t)(px + dx[desired]), (uint8_t)(py + dy[desired]))) {
            pdir = desired;
            newPx = (uint8_t)(px + dx[pdir]);
            newPy = (uint8_t)(py + dy[pdir]);
        } else {
            gameOver = 1;
            win = 0;
            _XL_EXPLOSION_SOUND();
            return;
        }
    }

    opp = (uint8_t)((pdir + 2) & 3);
    reverseC = (uint8_t)((cdir + 2) & 3);

    if (passable((uint8_t)(cx + dx[cdir]), (uint8_t)(cy + dy[cdir]))) {
        if (opp != cdir &&
            opp != reverseC &&
            passable((uint8_t)(cx + dx[opp]), (uint8_t)(cy + dy[opp]))) {
            cdir = opp;
        }
    } else {
        found = 0;

        if (opp != reverseC &&
            passable((uint8_t)(cx + dx[opp]), (uint8_t)(cy + dy[opp]))) {
            cdir = opp;
            found = 1;
        } else {
            for (d = 0; d < 4; d++) {
                if (d != reverseC &&
                    passable((uint8_t)(cx + dx[d]), (uint8_t)(cy + dy[d]))) {
                    cdir = d;
                    found = 1;
                    break;
                }
            }
        }

        if (!found) {
            for (d = 0; d < 4; d++) {
                if (passable((uint8_t)(cx + dx[d]), (uint8_t)(cy + dy[d]))) {
                    cdir = d;
                    found = 1;
                    break;
                }
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

    if (!passable(newCx, newCy)) {
        gameOver = 1;
        win = 0;
        _XL_EXPLOSION_SOUND();
        return;
    }

    crash = 0;

    if (newPx == newCx && newPy == newCy) {
        crash = 1;
    }

    if (newPx == oldCx && newPy == oldCy &&
        newCx == oldPx && newCy == oldPy) {
        crash = 1;
    }

    if (crash) {
        gameOver = 1;
        win = 0;
        _XL_EXPLOSION_SOUND();
        return;
    }

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

    if (dots[newCx][newCy]) {
        dotVisible = 1;
        if ((oldPx == newCx && oldPy == newCy) ||
            (oldCx == newCx && oldCy == newCy)) {
            dotVisible = 0;
        }

        if (dotVisible) {
            _XL_DELETE(newCx, newCy);
        }
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
        _XL_WAIT_FOR_INPUT();

        newGame();

        playing = 1;

        while (playing) {
            input = _XL_INPUT();
            moveCars(input);

            if (gameOver) {
                playing = 0;
            }

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        showGameOver(win);
        _XL_WAIT_FOR_INPUT();
        _XL_CLEAR_SCREEN();
    }

    return 0;
}
