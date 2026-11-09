#include "cross_lib.h"

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t ox;
    uint8_t oy;
    short dx; /* Horizontal velocity */
    short dy; /* Vertical velocity */
    short active;
} Person;

void main(void) {
    /* Variable declarations at the beginning (ANSI C89) */
    uint8_t i;
    uint8_t input;
    uint8_t net_x;
    uint8_t net_ox;
    uint8_t net_y;
    uint16_t score;
    uint16_t misses;
    uint16_t frame_count;
    uint16_t spawn_rate;
    uint16_t difficulty_timer;
    short base_dy;
    Person people[5];

    /* Initialization */
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();
    _XL_CLEAR_SCREEN();

    score = 0;
    misses = 0;
    frame_count = 0;
    difficulty_timer = 0;
    spawn_rate = 50;
    base_dy = 1;
    net_y = YSize - 3; /* Aligned with bottom of ambulance */
    net_x = XSize / 2;
    net_ox = net_x;

    for (i = 0; i < 5; i++) {
        people[i].active = 0;
        people[i].ox = 0;
        people[i].oy = 0;
    }

    /* Draw Static Environment */
    /* Building */
    for (i = 0; i < 6; i++) {
        _XL_DRAW(i, 2, _TILE_1, _XL_RED);
        _XL_DRAW(i, 3, _TILE_1, _XL_RED);
    }

    /* Ambulance */
    _XL_DRAW(XSize - 4, YSize - 3, _TILE_2, _XL_WHITE);
    _XL_DRAW(XSize - 3, YSize - 3, _TILE_2, _XL_WHITE);
    _XL_DRAW(XSize - 4, YSize - 2, _TILE_2, _XL_WHITE);
    _XL_DRAW(XSize - 3, YSize - 2, _TILE_2, _XL_WHITE);

    /* UI */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "SCORE");
    _XL_PRINT(XSize - 10, 0, "MISS");

    while (1) {
        input = _XL_INPUT();
        net_ox = net_x;

        /* 1. Input Handling */
        if (_XL_LEFT(input)) {
            if (net_x > 2) net_x--;
        }
        if (_XL_RIGHT(input)) {
            if (net_x < XSize - 8) net_x++;
        }

        /* 2. Spawning Logic */
        frame_count++;
        if (frame_count % spawn_rate == 0) {
            for (i = 0; i < 5; i++) {
                if (people[i].active == 0) {
                    people[i].active = 1;
                    people[i].x = 3;
                    people[i].y = 2;
                    people[i].ox = 3;
                    people[i].oy = 2;
                    people[i].dx = 0;
                    /* Different speeds based on random seed */
                    people[i].dy = (short)(base_dy + (_XL_RAND() % 2));
                    break;
                }
            }
        }

        /* Difficulty Scaling: Increase falling speed over time */
        if (frame_count % 500 == 0) {
            if (base_dy < 3) base_dy++;
            if (spawn_rate > 15) spawn_rate--;
        }

        /* 3. Update Net Drawing */
        if (net_x != net_ox) {
            for (i = 0; i < 6; i++) {
                _XL_DELETE(net_ox + i, net_y);
            }
            for (i = 0; i < 6; i++) {
                _XL_DRAW(net_x + i, net_y, _TILE_4, _XL_CYAN);
            }
        }

        /* 4. Update People */
        for (i = 0; i < 5; i++) {
            if (people[i].active) {
                /* Apply Movement */
                people[i].x += (uint8_t)people[i].dx;
                people[i].y += (uint8_t)people[i].dy;

                /* Collision: Net Catch */
                if (people[i].y >= net_y && people[i].y <= net_y + 1) {
                    if (people[i].x >= net_x && people[i].x <= net_x + 5) {
                        score++;
                        _XL_PING_SOUND();

                        /* Bounce: Invert vertical and ensure horizontal movement */
                        people[i].dy = -people[i].dy;
                        if (people[i].dx == 0) people[i].dx = 1;
                        else people[i].dx += 1;
                    }
                }

                /* Collision: Top of Screen (Bounce back down) */
                if (people[i].y < 2) {
                    people[i].y = 2;
                    people[i].dy = (short)(base_dy + (_XL_RAND() % 2));
                }

                /* Collision: Ambulance (Success) */
                if (people[i].x >= XSize - 5 && people[i].y >= YSize - 4) {
                    people[i].active = 0;
                    _XL_TICK_SOUND();
                }
                /* Collision: Floor (Miss) */
                else if (people[i].y >= YSize - 1) {
                    people[i].active = 0;
                    misses++;
                    _XL_TOCK_SOUND();
                }
                /* Collision: Walls */
                else if (people[i].x >= XSize || people[i].x < 0) {
                    people[i].active = 0;
                    misses++;
                    _XL_TOCK_SOUND();
                }

                /* Draw logic: Only if active and has moved */
                if (people[i].active) {
                    if (people[i].x != people[i].ox || people[i].y != people[i].oy) {
                        _XL_DELETE(people[i].ox, people[i].oy);
                        people[i].ox = people[i].x;
                        people[i].oy = people[i].y;
                        _XL_DRAW(people[i].x, people[i].y, _TILE_3, _XL_YELLOW);
                    }
                } else {
                    _XL_DELETE(people[i].ox, people[i].oy);
                }
            }
        }

        /* 5. Display Scores */
        _XL_PRINTD(0, 1, 3, score);
        _XL_PRINTD(XSize - 10, 1, 3, misses);

        /* 6. Timing Control */
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

