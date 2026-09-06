#include "cross_lib.h"

#define NUM_PEGS 3
#define NUM_DISKS 4

static uint8_t pegs[3][4];
static uint8_t peg_heights[3];
static uint16_t moves;
static uint8_t phase;
static uint8_t sel_source;
static uint8_t sel_dest;
static uint8_t disk_colors[4];

static void draw_disk(uint8_t peg, uint8_t row, uint8_t size, uint8_t color);
static void delete_disk(uint8_t peg, uint8_t row, uint8_t size);
static void draw_marker(uint8_t peg, uint8_t color);
static void delete_marker(uint8_t peg);
static void init_game(void);

static void draw_disk(uint8_t peg, uint8_t row, uint8_t size, uint8_t color)
{
    uint8_t cx, w, xs, i, yt, yb;
    cx = (uint8_t)(5 + peg * 8);
    w = (uint8_t)(size * 2 + 2);
    xs = (uint8_t)(cx - w / 2);
    yt = (uint8_t)(12 - row * 2);
    yb = (uint8_t)(13 - row * 2);
    for (i = xs; i < xs + w; i++) {
        _XL_DRAW(i, yt, _TILE_1, color);
        _XL_DRAW(i, yb, _TILE_1, color);
    }
}

static void delete_disk(uint8_t peg, uint8_t row, uint8_t size)
{
    uint8_t cx, w, xs, i, yt, yb;
    cx = (uint8_t)(5 + peg * 8);
    w = (uint8_t)(size * 2 + 2);
    xs = (uint8_t)(cx - w / 2);
    yt = (uint8_t)(12 - row * 2);
    yb = (uint8_t)(13 - row * 2);
    for (i = xs; i < xs + w; i++) {
        _XL_DELETE(i, yt);
        _XL_DELETE(i, yb);
    }
}

static void draw_marker(uint8_t peg, uint8_t color)
{
    uint8_t cx;
    cx = (uint8_t)(5 + peg * 8);
    _XL_DRAW(cx, 5, _TILE_3, color);
}

static void delete_marker(uint8_t peg)
{
    uint8_t cx;
    cx = (uint8_t)(5 + peg * 8);
    _XL_DELETE(cx, 5);
}

static void init_game(void)
{
    uint8_t i, j, cx;

    peg_heights[0] = 4;
    peg_heights[1] = 0;
    peg_heights[2] = 0;

    pegs[0][0] = 4;
    pegs[0][1] = 3;
    pegs[0][2] = 2;
    pegs[0][3] = 1;

    moves = 0;
    phase = 0;
    sel_source = 0;
    sel_dest = 1;

    disk_colors[0] = _XL_RED;
    disk_colors[1] = _XL_GREEN;
    disk_colors[2] = _XL_CYAN;
    disk_colors[3] = _XL_YELLOW;

    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(5, 0, "TOWER OF HANOI");

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(2, 2, "LEFT RIGHT SELECT");
    _XL_PRINT(2, 3, "FIRE CONFIRM");

    for (i = 0; i < 3; i++) {
        cx = (uint8_t)(5 + i * 8);
        for (j = 6; j <= 14; j++) {
            _XL_DRAW(cx, j, _TILE_2, _XL_BLUE);
        }
    }

    for (i = 0; i < 32; i++) {
        _XL_DRAW(i, 14, _TILE_2, _XL_BLUE);
    }

    for (i = 0; i < 4; i++) {
        draw_disk(0, i, pegs[0][i], disk_colors[pegs[0][i] - 1]);
    }

    draw_marker(sel_source, _XL_GREEN);
    draw_marker(sel_dest, _XL_YELLOW);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 16, "MOVES");
    _XL_PRINTD(9, 16, 1, moves);

    _XL_WAIT_FOR_INPUT();
}

int main(void)
{
    uint8_t input;
    uint8_t i;
    uint8_t src, dst;
    uint8_t disk_id;
    uint8_t win;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    init_game();

    while (1) {
        win = 1;
        if (peg_heights[2] != 4) {
            win = 0;
        } else {
            for (i = 0; i < 4; i++) {
                if (pegs[2][i] != (uint8_t)(4 - i)) {
                    win = 0;
                }
            }
        }

        if (win) {
            _XL_SET_TEXT_COLOR(_XL_YELLOW);
            _XL_PRINT(5, 18, "YOU WIN!");
            _XL_PING_SOUND();
            _XL_WAIT_FOR_INPUT();
            init_game();
            continue;
        }

        input = _XL_INPUT();

        if (phase == 0) {
            if (_XL_LEFT(input)) {
                delete_marker(sel_source);
                sel_source = (uint8_t)((sel_source + 2) % 3);
                draw_marker(sel_source, _XL_GREEN);
                _XL_TICK_SOUND();
            }
            if (_XL_RIGHT(input)) {
                delete_marker(sel_source);
                sel_source = (uint8_t)((sel_source + 1) % 3);
                draw_marker(sel_source, _XL_GREEN);
                _XL_TICK_SOUND();
            }
            if (_XL_FIRE(input)) {
                if (peg_heights[sel_source] > 0) {
                    phase = 1;
                    _XL_PING_SOUND();
                }
            }
        } else {
            if (_XL_LEFT(input)) {
                delete_marker(sel_dest);
                sel_dest = (uint8_t)((sel_dest + 2) % 3);
                draw_marker(sel_dest, _XL_YELLOW);
                _XL_TICK_SOUND();
            }
            if (_XL_RIGHT(input)) {
                delete_marker(sel_dest);
                sel_dest = (uint8_t)((sel_dest + 1) % 3);
                draw_marker(sel_dest, _XL_YELLOW);
                _XL_TICK_SOUND();
            }
            if (_XL_FIRE(input)) {
                src = sel_source;
                dst = sel_dest;
                if (dst != src && peg_heights[src] > 0) {
                    disk_id = pegs[src][peg_heights[src] - 1];
                    if (peg_heights[dst] == 0 || pegs[dst][peg_heights[dst] - 1] > disk_id) {
                        delete_disk(src, peg_heights[src] - 1, disk_id);
                        peg_heights[src]--;
                        pegs[dst][peg_heights[dst]] = disk_id;
                        peg_heights[dst]++;
                        draw_disk(dst, peg_heights[dst] - 1, disk_id, disk_colors[disk_id - 1]);
                        moves++;
                        _XL_PRINTD(9, 16, 1, moves);
                        _XL_SHOOT_SOUND();
                        phase = 0;
                    } else {
                        _XL_TOCK_SOUND();
                    }
                }
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}
