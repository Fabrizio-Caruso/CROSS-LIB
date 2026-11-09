/*
 * Asteroids - Cross-Lib Tile Grid Edition
 *
 * Controls:
 *   LEFT/RIGHT/UP/DOWN  - Move ship
 *   FIRE                - Shoot bullet upward
 *
 * Large asteroids (2x2) split into two small ones when hit.
 * Small asteroids disappear on impact.
 * You have 3 lives. Score: 10 per small, 20 per large destroyed.
 */

#include "cross_lib.h"

/* ---- Constants ---- */
#define MAX_ASTEROIDS   16
#define MAX_BULLETS     4
#define NUM_LIVES       3
#define SPAWN_INTERVAL  9      /* frames between spawn waves */
#define WAVE_SIZE_MIN   2
#define WAVE_SIZE_MAX   4

/* Asteroid sizes */
#define AST_LARGE       2      /* occupies 2x2 tiles */
#define AST_SMALL       1      /* occupies 1 tile  */

/* ---- Types (fixed-size, no heap) ---- */
typedef struct {
    uint8_t x, y;              /* top-left corner */
    int8_t  vx, vy;
    uint8_t size;              /* AST_LARGE or AST_SMALL */
    uint8_t active;
} Asteroid;

typedef struct {
    uint8_t x, y;
    int8_t  vx, vy;
    uint8_t active;
} Bullet;

/* ---- Static state (no heap) ---- */
static Asteroid asteroids[MAX_ASTEROIDS];
static Bullet   bullets[MAX_BULLETS];
static uint8_t  px, py;
static uint8_t  lives;
static uint16_t score;
static uint8_t  game_over;
static uint16_t frame_count;

/* ---- Drawing helpers ---- */

static void draw_player(void)
{
    _XL_DRAW(px, py, _TILE_0, _XL_CYAN);
}

static void erase_player(void)
{
    _XL_DELETE(px, py);
}

static void draw_score(void)
{
    _XL_PRINTD(0, 0, 1, score);
}

static void draw_lives(void)
{
    uint8_t i;
    for (i = 0; i < NUM_LIVES; i++) {
        if ((int)i < (int)lives) {
            _XL_DRAW(i, YSize - 1, _TILE_3, _XL_GREEN);
        } else {
            _XL_DELETE(i, YSize - 1);
        }
    }
}

/* Draw an asteroid at its stored position based on size */
static void draw_asteroid(uint8_t idx)
{
    Asteroid *a = &asteroids[idx];
    uint8_t dx, dy;

    if (a->size == AST_LARGE) {
        for (dy = 0; dy < 2; dy++) {
            for (dx = 0; dx < 2; dx++) {
                uint8_t tx = a->x + dx;
                uint8_t ty = a->y + dy;
                if ((uint8_t)tx < XSize && (uint8_t)ty < YSize) {
                    _XL_DRAW(tx, ty, _TILE_1, _XL_RED);
                }
            }
        }
    } else {
        _XL_DRAW(a->x, a->y, _TILE_4, _XL_YELLOW);
    }
}

static void erase_asteroid(uint8_t idx)
{
    Asteroid *a = &asteroids[idx];
    uint8_t dx, dy;

    if (a->size == AST_LARGE) {
        for (dy = 0; dy < 2; dy++) {
            for (dx = 0; dx < 2; dx++) {
                uint8_t tx = a->x + dx;
                uint8_t ty = a->y + dy;
                if ((uint8_t)tx < XSize && (uint8_t)ty < YSize) {
                    _XL_DELETE(tx, ty);
                }
            }
        }
    } else {
        _XL_DELETE(a->x, a->y);
    }
}

/* ---- Game logic ---- */

static uint8_t find_free_asteroid(void)
{
    uint8_t i;
    for (i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) return i;
    }
    return MAX_ASTEROIDS; /* none free */
}

static void spawn_asteroid(uint8_t size)
{
    uint8_t idx;
    uint8_t r;

    idx = find_free_asteroid();
    if (idx == MAX_ASTEROIDS) return;

    /* Spawn at top, leave room for large asteroids */
    {
        uint8_t max_x = XSize - size;
        asteroids[idx].x = (uint8_t)(_XL_RAND() % max_x);
    }
    asteroids[idx].y = 0;

    /* Random horizontal drift: -1, 0, or +1 */
    r = _XL_RAND() % 3;
    if (r == 0)       asteroids[idx].vx = -1;
    else if (r == 2)  asteroids[idx].vx = 1;
    else               asteroids[idx].vx = 0;

    /* Downward speed: large slower, small faster */
    if (size == AST_LARGE) {
        asteroids[idx].vy = 1;
    } else {
        asteroids[idx].vy = (int8_t)(1 + (_XL_RAND() % 2));
    }

    asteroids[idx].size = size;
    asteroids[idx].active = 1;
    draw_asteroid(idx);
}

/* Spawn a wave of multiple asteroids */
static void spawn_wave(void)
{
    uint8_t count, i;
    uint8_t r;

    /* Random wave size between WAVE_SIZE_MIN and WAVE_SIZE_MAX */
    count = WAVE_SIZE_MIN + (uint8_t)(_XL_RAND() % (WAVE_SIZE_MAX - WAVE_SIZE_MIN + 1));

    for (i = 0; i < count; i++) {
        /* Mix of large and small: 60% large, 40% small */
        r = _XL_RAND() % 10;
        if (r < 7) {
            spawn_asteroid(AST_LARGE);
        } else {
            spawn_asteroid(AST_SMALL);
        }
    }
}

/* Split a large asteroid into two small ones */
static void split_asteroid(uint8_t idx)
{
    Asteroid *a = &asteroids[idx];
    uint8_t new_idx;
    int8_t base_vx, base_vy;

    base_vx = a->vx;
    base_vy = a->vy;

    /* First small fragment: drift left */
    new_idx = find_free_asteroid();
    if (new_idx < MAX_ASTEROIDS) {
        asteroids[new_idx].x = a->x;
        asteroids[new_idx].y = a->y;
        asteroids[new_idx].vx = base_vx - 1;
        asteroids[new_idx].vy = base_vy + 1;
        if (asteroids[new_idx].vy > 2) asteroids[new_idx].vy = 2;
        asteroids[new_idx].size = AST_SMALL;
        asteroids[new_idx].active = 1;
        draw_asteroid(new_idx);
    }

    /* Second small fragment: drift right */
    new_idx = find_free_asteroid();
    if (new_idx < MAX_ASTEROIDS) {
        asteroids[new_idx].x = a->x + 1;
        asteroids[new_idx].y = a->y + 1;
        asteroids[new_idx].vx = base_vx + 1;
        asteroids[new_idx].vy = base_vy + 1;
        if (asteroids[new_idx].vy > 2) asteroids[new_idx].vy = 2;
        asteroids[new_idx].size = AST_SMALL;
        asteroids[new_idx].active = 1;
        draw_asteroid(new_idx);
    }
}

static void fire_bullet(void)
{
    uint8_t i;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = px;
            if (py > 1)
                bullets[i].y = py - 1;
            else
                bullets[i].y = 0;

            bullets[i].vx = 0;
            bullets[i].vy = -2;
            bullets[i].active = 1;

            _XL_DRAW(bullets[i].x, bullets[i].y, _TILE_2, _XL_WHITE);
            _XL_SHOOT_SOUND();
            return;
        }
    }
}

static void update_bullets(void)
{
    uint8_t i;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;

        _XL_DELETE(bullets[i].x, bullets[i].y);

        {
            int16_t nx = (int16_t)bullets[i].x + bullets[i].vx;
            int16_t ny = (int16_t)bullets[i].y + bullets[i].vy;

            if ((uint8_t)nx >= XSize || (uint8_t)ny >= YSize || ny < 0) {
                bullets[i].active = 0;
                continue;
            }

            bullets[i].x = (uint8_t)nx;
            bullets[i].y = (uint8_t)ny;
        }
        _XL_DRAW(bullets[i].x, bullets[i].y, _TILE_2, _XL_WHITE);
    }
}

static void update_asteroids(void)
{
    uint8_t i;
    for (i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) continue;

        erase_asteroid(i);

        {
            int16_t nx = (int16_t)asteroids[i].x + asteroids[i].vx;
            int16_t ny = (int16_t)asteroids[i].y + asteroids[i].vy;
            uint8_t extent = asteroids[i].size;

            if ((uint8_t)nx >= XSize || (uint8_t)(ny + extent - 1) >= YSize || nx < 0 || ny < 0) {
                asteroids[i].active = 0;
                continue;
            }

            asteroids[i].x = (uint8_t)nx;
            asteroids[i].y = (uint8_t)ny;
        }
        draw_asteroid(i);
    }
}

/* Check if point (bx,by) is inside asteroid at index idx */
static uint8_t asteroid_contains(uint8_t idx, uint8_t bx, uint8_t by)
{
    Asteroid *a = &asteroids[idx];
    uint8_t extent = a->size;

    if ((int)bx < (int)a->x || (int)by < (int)a->y) return 0;
    if ((uint8_t)(bx - a->x) >= extent || (uint8_t)(by - a->y) >= extent) return 0;
    return 1;
}

static void check_collisions(void)
{
    uint8_t i, j;

    /* Bullet vs Asteroid */
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        for (j = 0; j < MAX_ASTEROIDS; j++) {
            if (!asteroids[j].active) continue;

            if (asteroid_contains(j, bullets[i].x, bullets[i].y)) {

                _XL_DELETE(bullets[i].x, bullets[i].y);
                bullets[i].active = 0;

                erase_asteroid(j);

                if (asteroids[j].size == AST_LARGE) {
                    /* Split into two small fragments */
                    asteroids[j].active = 0;
                    split_asteroid(j);
                    score += 20;
                } else {
                    asteroids[j].active = 0;
                    score += 10;
                }

                draw_score();
                _XL_EXPLOSION_SOUND();
                break;
            }
        }
    }

    /* Player vs Asteroid */
    for (j = 0; j < MAX_ASTEROIDS; j++) {
        if (!asteroids[j].active) continue;
        if (asteroid_contains(j, px, py)) {

            erase_asteroid(j);
            asteroids[j].active = 0;

            lives--;
            draw_lives();
            _XL_ZAP_SOUND();

            if (lives == 0) {
                game_over = 1;
            } else {
                erase_player();
                px = XSize / 2;
                py = YSize - 3;
                draw_player();
            }
        }
    }
}

/* ---- Main ---- */

int main(void)
{
    uint8_t i;
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    /* Initialize state */
    px = XSize / 2;
    py = YSize - 3;
    lives = NUM_LIVES;
    score = 0;
    game_over = 0;
    frame_count = 0;

    for (i = 0; i < MAX_ASTEROIDS; i++) asteroids[i].active = 0;
    for (i = 0; i < MAX_BULLETS; i++)   bullets[i].active = 0;

    _XL_CLEAR_SCREEN();
    draw_player();
    draw_score();
    draw_lives();

    /* Spawn initial wave */
    spawn_wave();

    /* ---- Game loop ---- */
    while (!game_over) {
        input = _XL_INPUT();

        /* Player movement */
        if (_XL_LEFT(input)) {
            erase_player();
            if (px > 0) px--;
            draw_player();
        }
        if (_XL_RIGHT(input)) {
            erase_player();
            if ((uint8_t)(px + 1) < XSize) px++;
            draw_player();
        }
        if (_XL_UP(input)) {
            erase_player();
            if (py > 0) py--;
            draw_player();
        }
        if (_XL_DOWN(input)) {
            erase_player();
            if ((uint8_t)(py + 1) < YSize) py++;
            draw_player();
        }

        /* Firing */
        if (_XL_FIRE(input)) {
            fire_bullet();
        }

        /* Update entities */
        update_bullets();
        update_asteroids();
        check_collisions();

        /* Spawn new waves on a timer */
        frame_count++;
        if (frame_count >= SPAWN_INTERVAL) {
            frame_count = 0;
            spawn_wave();
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    /* ---- Game Over screen ---- */
    _XL_CLEAR_SCREEN();
    _XL_PRINT(XSize / 4, YSize / 2 - 1, "GAME OVER");
    _XL_PRINTD(XSize / 4, (uint8_t)(YSize / 2 + 1), 1, score);
    _XL_EXPLOSION_SOUND();

    return 0;
}
