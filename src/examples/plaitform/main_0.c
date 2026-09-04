#include "cross_lib.h"

#define NUM_ENEMIES 3
#define NUM_ITEMS   5

/* Tile assignments – two vertical tiles per facing direction */
/* Player left : _TILE_0 (top), _TILE_1 (bottom)            */
/* Player right: _TILE_2 (top), _TILE_3 (bottom)            */
/* Enemy  left : _TILE_4 (top), _TILE_5 (bottom)            */
/* Enemy  right: _TILE_6 (top), _TILE_7 (bottom)            */
/* Collectible : _TILE_8                                    */
/* Platform    : _TILE_9                                    */

struct Enemy {
    uint8_t x;
    uint8_t y;      /* top tile row */
    uint8_t dir;    /* 0 = left, 1 = right */
};

struct ItemObj {
    uint8_t x;
    uint8_t y;
    uint8_t collected;
};

/* ------------------------------------------------------------------ */
/* Display-state tracking: 255 means "nothing drawn here"             */
/* ------------------------------------------------------------------ */
uint8_t disp[XSize * YSize];

static void init_disp(void) {
    for (uint16_t i = 0; i < XSize * YSize; i++) disp[i] = 255;
}

/* Draw only if the tracked value differs from what we want to show */
static void draw_tile(uint8_t x, uint8_t y, uint8_t tile_id, uint8_t color) {
    uint16_t idx = (uint16_t)(y * XSize + x);
    if (disp[idx] != tile_id) {
        disp[idx] = tile_id;
        _XL_DRAW(x, y, tile_id, color);
    }
}

/* Delete only if the position was previously drawn */
static void del_tile(uint8_t x, uint8_t y) {
    uint16_t idx = (uint16_t)(y * XSize + x);
    if (disp[idx] != 255) {
        disp[idx] = 255;
        _XL_DELETE(x, y);
    }
}

/* Wipe the whole screen and reset tracking */
static void clear_screen(void) {
    for (uint8_t yy = 0; yy < YSize; yy++)
        for (uint8_t xx = 0; xx < XSize; xx++)
            del_tile(xx, yy);
    _XL_CLEAR_SCREEN();
}

/* Draw a two-tall entity (top at y, bottom at y+1) */
static void draw_entity(uint8_t x, uint8_t y,
                        uint8_t top_tile, uint8_t bot_tile, uint8_t color) {
    draw_tile(x, y,     top_tile, color);
    draw_tile(x, y + 1, bot_tile, color);
}

/* Erase a two-tall entity that previously occupied (x,y) */
static void erase_entity(uint8_t x, uint8_t y) {
    del_tile(x, y);
    del_tile(x, y + 1);
}

/* ------------------------------------------------------------------ */
int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        /* ---- game state ---- */
        uint8_t px = XSize / 4;          /* player x            */
        uint8_t py = YSize - 3;           /* player top row      */
        uint8_t pdir = 1;                 /* facing right        */
        int8_t  vy = 0;                   /* vertical velocity   */
        uint8_t grounded = 1;

        Enemy enemies[NUM_ENEMIES];
        ItemObj items[NUM_ITEMS];
        uint8_t score = 0;
        uint8_t done = 0;
        uint8_t win = 0;

        init_disp();

        /* ---- static platforms (drawn once, never change) ---- */
        /* ground: full-width row at YSize-1 */
        for (uint8_t x = 0; x < XSize; x++)
            draw_tile(x, YSize - 1, _TILE_9, _XL_GREEN);

        /* platform A – left side, surface at row YSize-5 */
        uint8_t pa_x = XSize / 4 + 2;
        uint8_t pa_w = XSize / 4;
        for (uint8_t i = 0; i < pa_w; i++)
            draw_tile(pa_x + i, YSize - 5, _TILE_9, _XL_GREEN);

        /* platform B – right side, surface at row YSize-7 */
        uint8_t pb_x = XSize / 2 + 3;
        uint8_t pb_w = XSize / 4;
        for (uint8_t i = 0; i < pb_w; i++)
            draw_tile(pb_x + i, YSize - 7, _TILE_9, _XL_GREEN);

        /* ---- enemies ---- */
        enemies[0].x = pa_x + pa_w / 2;
        enemies[0].y = YSize - 6;   /* bottom at YSize-5 (on platform A) */
        enemies[0].dir = 1;

        enemies[1].x = pb_x + pb_w / 2;
        enemies[1].y = YSize - 8;   /* bottom at YSize-7 (on platform B) */
        enemies[1].dir = 0;

        enemies[2].x = XSize / 2;
        enemies[2].y = YSize - 3;   /* bottom at YSize-2 (on ground)     */
        enemies[2].dir = 1;

        /* ---- collectible items ---- */
        items[0].x = pa_x + 1;
        items[0].y = YSize - 6;
        items[0].collected = 0;

        items[1].x = pa_x + pa_w - 2;
        items[1].y = YSize - 6;
        items[1].collected = 0;

        items[2].x = pb_x + 1;
        items[2].y = YSize - 8;
        items[2].collected = 0;

        items[3].x = pb_x + pb_w - 2;
        items[3].y = YSize - 8;
        items[3].collected = 0;

        items[4].x = XSize / 2;
        items[4].y = YSize - 4;     /* just above ground */
        items[4].collected = 0;

        /* ---- HUD text ---- */
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(1, 1, "COLLECT ALL ITEMS");

        /* ==================== main game loop ==================== */
        while (!done) {
            uint8_t input = _XL_INPUT();

            /* --- player horizontal movement --- */
            if (_XL_LEFT(input)) {
                pdir = 0;
                if (px > 0) px--;
            }
            if (_XL_RIGHT(input)) {
                pdir = 1;
                if (px < XSize - 1) px++;
            }

            /* --- jump --- */
            if ((_XL_UP(input) || _XL_FIRE(input)) && grounded) {
                vy = 3;          /* upward (decreasing y) */
                grounded = 0;
            }

            /* --- vertical physics --- */
            if (!grounded) {
                if (vy > 0) {
                    /* moving up */
                    py--;
                    vy--;
                    if (vy == 0) {
                        /* apex – check if we landed on a platform top */
                        uint8_t bot = py + 1;
                        if (bot == YSize - 2) {          /* ground */
                            grounded = 1;
                        } else if (bot == YSize - 6) {   /* platform A level */
                            for (uint8_t i = 0; i < pa_w; i++)
                                if (px == pa_x + i) { grounded = 1; break; }
                        } else if (bot == YSize - 8) {   /* platform B level */
                            for (uint8_t i = 0; i < pb_w; i++)
                                if (px == pb_x + i) { grounded = 1; break; }
                        }
                    }
                } else if (vy < 0) {
                    /* falling */
                    py++;
                    uint8_t bot = py + 1;
                    /* check landing surfaces */
                    if (bot >= YSize - 2) {              /* ground */
                        py = YSize - 3;
                        grounded = 1;
                        vy = 0;
                    } else if (bot == YSize - 6) {       /* platform A */
                        for (uint8_t i = 0; i < pa_w; i++)
                            if (px == pa_x + i) { py = YSize - 7; grounded = 1; vy = 0; break; }
                    } else if (bot == YSize - 8) {       /* platform B */
                        for (uint8_t i = 0; i < pb_w; i++)
                            if (px == pb_x + i) { py = YSize - 9; grounded = 1; vy = 0; break; }
                    } else {
                        vy--;
                    }
                } else {
                    /* vy == 0 and not grounded: start falling */
                    vy = -1;
                }
            }

            /* --- enemy patrol movement --- */
            for (uint8_t i = 0; i < NUM_ENEMIES; i++) {
                if (enemies[i].dir) {          /* moving right */
                    enemies[i].x++;
                    if (i == 0 && enemies[i].x >= pa_x + pa_w - 1) enemies[i].dir = 0;
                    if (i == 1 && enemies[i].x >= pb_x + pb_w - 1) enemies[i].dir = 0;
                    if (i == 2 && enemies[i].x >= XSize - 1)       enemies[i].dir = 0;
                } else {                       /* moving left */
                    enemies[i].x--;
                    if (enemies[i].x <= pa_x) enemies[0].dir = 1;
                    if (i == 1 && enemies[i].x <= pb_x) enemies[1].dir = 1;
                    if (i == 2 && enemies[i].x <= 0)     enemies[2].dir = 1;
                }
            }

            /* --- collision: player vs enemy --- */
            for (uint8_t i = 0; i < NUM_ENEMIES; i++) {
                if (px == enemies[i].x &&
                    ((py == enemies[i].y) ||
                     (py + 1 == enemies[i].y) ||
                     (py == enemies[i].y + 1) ||
                     (py + 1 == enemies[i].y + 1))) {
                    done = 1;
                }
            }

            /* --- item collection --- */
            for (uint8_t i = 0; i < NUM_ITEMS; i++) {
                if (!items[i].collected && px == items[i].x &&
                    ((py == items[i].y) || (py + 1 == items[i].y))) {
                    items[i].collected = 1;
                    score++;
                    _XL_PING_SOUND();
                }
            }
            if (score >= NUM_ITEMS) win = 1, done = 1;

            /* --- render: erase old positions then draw new ones --- */
            /* Player */
            {
                uint8_t tt, bt;
                if (pdir == 0) { tt = _TILE_0; bt = _TILE_1; }
                else            { tt = _TILE_2; bt = _TILE_3; }
                /* erase previous frame position – we don't track it explicitly,
                   but draw_tile/del_tile are idempotent-safe via disp[] */
                draw_entity(px, py, tt, bt, _XL_CYAN);
            }

            /* Enemies */
            for (uint8_t i = 0; i < NUM_ENEMIES; i++) {
                uint8_t tt, bt;
                if (enemies[i].dir == 0) { tt = _TILE_4; bt = _TILE_5; }
                else                      { tt = _TILE_6; bt = _TILE_7; }
                draw_entity(enemies[i].x, enemies[i].y, tt, bt, _XL_RED);
            }

            /* Items (only if not collected) */
            for (uint8_t i = 0; i < NUM_ITEMS; i++) {
                if (!items[i].collected)
                    draw_tile(items[i].x, items[i].y, _TILE_8, _XL_YELLOW);
            }

            /* Score display */
            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINTD(1, YSize - 2, 1, score);

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 4);
        }

        /* ---- end-of-game message ---- */
        if (win) {
            _XL_SET_TEXT_COLOR(_XL_YELLOW);
            _XL_PRINT(5, YSize / 2, "YOU WIN!");
        } else {
            _XL_SET_TEXT_COLOR(_XL_MAGENTA);
            _XL_PRINT(5, YSize / 2, "GAME OVER");
            _XL_EXPLOSION_SOUND();
        }

        _XL_WAIT_FOR_INPUT();
        clear_screen();
    }
}
