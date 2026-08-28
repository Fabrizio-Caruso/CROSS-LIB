#include "cross_lib.h"

/* ──────────── Constants ──────────── */
#define MAX_DROPS   8
#define MAX_LIVES   3
#define SPAWN_EVERY 12      /* base frames between spawns */
#define SPEED_RAMP  40      /* frames per difficulty level */

/* ──────────── Types ──────────────── */

typedef struct {
    uint8_t active;   /* 1 = in play, 0 = empty slot */
    uint8_t x;        /* column (0..XSize) */
    uint8_t y;        /* row    (0..YSize) */
    uint8_t speed;    /* rows per frame: 1 or 2 */
} Drop;

/* ──────────── Pseudo-random ──────── */

static uint16_t rng_state;

uint8_t rand_below(uint8_t n) {
    rng_state = rng_state * 7 + 13;          /* wraps in uint16_t */
    return (uint8_t)(rng_state % n);
}

/* ──────────── Game state ───────────── */

enum GameState { STATE_TITLE, STATE_PLAYING, STATE_GAMEOVER };

static Drop   drops[MAX_DROPS];
static uint8_t player_x;
static uint8_t player_y;
static uint16_t score;          /* frames survived */
static uint8_t lives;
static uint8_t difficulty;     /* ramps up over time */
static uint8_t spawn_timer;

/* ──────────── Helpers ─────────────── */

void reset_drops(void) {
    uint8_t i;
    for (i = 0; i < MAX_DROPS; i++) {
        drops[i].active = 0;
    }
}

uint8_t find_empty_slot(void) {
    uint8_t i;
    for (i = 0; i < MAX_DROPS; i++) {
        if (!drops[i].active) return i;
    }
    return MAX_DROPS;   /* all full */
}

void spawn_drop(void) {
    uint8_t slot;
    uint8_t speed;

    slot = find_empty_slot();
    if (slot == MAX_DROPS) return;  /* no room */

    drops[slot].active = 1;
    drops[slot].x      = rand_below((uint8_t)(XSize + 1));
    drops[slot].y      = 0;
    speed              = 1;
    if (difficulty > 2) speed = 2;   /* faster at higher difficulty */
    drops[slot].speed  = speed;
}

void update_drops(void) {
    uint8_t i;

    for (i = 0; i < MAX_DROPS; i++) {
        if (!drops[i].active) continue;

        /* Move drop down */
        drops[i].y += drops[i].speed;

        /* Remove if it fell off the bottom */
        if (drops[i].y > YSize) {
            drops[i].active = 0;
        }
    }
}

uint8_t check_collision(void) {
    uint8_t i;
    for (i = 0; i < MAX_DROPS; i++) {
        if (!drops[i].active) continue;
        /* Simple: same tile position */
        if (drops[i].x == player_x && drops[i].y == player_y) {
            return 1;
        }
    }
    return 0;
}

/* ──────────── Rendering ────────────── */

void draw_lives(void) {
    uint8_t i;
    for (i = 0; i < MAX_LIVES; i++) {
        if (i < lives) {
            _XL_DRAW((uint8_t)(i + 1), 0, 3, _XL_RED);      /* heart tile */
        } else {
            _XL_DELETE((uint8_t)(i + 1), 0);                /* empty slot */
        }
    }
}

void draw_score(void) {
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(0, (uint8_t)(YSize - 1), 8, score);
}

/* ──────────── Title screen ─────────── */

void show_title(void) {
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(2, 3, "DODGE THE RAIN");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 6, "Move with arrows.");
    _XL_PRINT(2, 7, "Avoid the red drops!");
    _XL_PRINT(2, 8, "3 lives. Good luck.");
    _XL_WAIT_FOR_INPUT();
}

/* ──────────── Game over screen ─────── */

void show_gameover(void) {
    uint8_t i;
    _XL_CLEAR_SCREEN();
    for (i = 0; i < MAX_DROPS; i++) drops[i].active = 0;

    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(4, 3, "GAME OVER");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(4, 6, 8, score);
    _XL_PRINT(12, 6, " frames");
    _XL_WAIT_FOR_INPUT();
}

/* ──────────── Main loop ────────────── */

int main(void) {
    uint8_t input;
    uint8_t hit;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    rng_state = 42;

    /* ── Title ── */
    show_title();

    /* ── Reset game state ── */
    reset_drops();
    player_x  = (uint8_t)(XSize / 2);
    player_y  = (uint8_t)(YSize - 4);   /* near bottom */
    score     = 0;
    lives     = MAX_LIVES;
    difficulty = 1;
    spawn_timer = 0;

    /* ── Playing loop ── */
    while (lives > 0) {
        _XL_CLEAR_SCREEN();

        /* Draw HUD */
        draw_lives();
        draw_score();

        /* Difficulty ramp: every SPEED_RAMP frames, level up */
        if ((score % SPEED_RAMP) == 0 && score != 0) {
            difficulty++;
        }

        /* Spawn logic: interval shrinks as difficulty rises */
        spawn_timer++;
        {
            uint8_t interval = (uint8_t)(SPAWN_EVERY - (difficulty > 6 ? 6 : difficulty));
            if (interval < 3) interval = 3;   /* don't go below 3 */
            if (spawn_timer >= interval) {
                spawn_drop();
                spawn_timer = 0;
            }
        }

        /* Update drops */
        update_drops();

        /* Draw drops */
        {
            uint8_t i;
            for (i = 0; i < MAX_DROPS; i++) {
                if (!drops[i].active) continue;
                _XL_DRAW(drops[i].x, drops[i].y, 4, _XL_RED);
            }
        }

        /* Draw player */
        _XL_DRAW(player_x, player_y, 5, _XL_CYAN);

        /* Timing */
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

        /* Poll input and move player */
        input = _XL_INPUT();
        if (_XL_LEFT(input))  { player_x--; }
        if (_XL_RIGHT(input)) { player_x++; }
        if (_XL_UP(input))    { player_y--; }
        if (_XL_DOWN(input))  { player_y++; }

        /* Clamp */
        if (player_x > XSize) player_x = XSize;
        if (player_y > YSize) player_y = YSize;

        /* Collision check */
        hit = check_collision();
        if (hit) {
            lives--;
            /* Brief pause so the hit is visible */
            _XL_SLEEP(1);
            /* Clear all drops on hit for fairness */
            reset_drops();
        }

        score++;   /* survived this frame */
    }

    /* ── Game over ── */
    show_gameover();

    return 0;
}
