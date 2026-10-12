#include "cross_lib.h"

#define GRID_W 8
#define GRID_H 8
#define NUM_COLORS 4
#define STATE_PLAYING 0
#define STATE_CLEARING 1
#define STATE_GAME_OVER 2

#define TILE_WALL _TILE_16


uint8_t tile[4][4][4] = 
{
    {{_TILE_0, _TILE_1},  {_TILE_2, _TILE_3}},
    {{_TILE_4, _TILE_5},  {_TILE_6, _TILE_7}},
    {{_TILE_8, _TILE_9},  {_TILE_10,_TILE_11}},
    {{_TILE_12,_TILE_13}, {_TILE_14,_TILE_15}}
};


static uint8_t GAME_COLORS[NUM_COLORS + 1] = {
    0,
    _XL_RED,
    _XL_YELLOW,
    _XL_GREEN,
    _XL_CYAN
};

uint8_t grid[GRID_H][GRID_W];
uint8_t clear_mask[GRID_H][GRID_W];
uint8_t displayed[GRID_H][GRID_W][2][2];

uint8_t fall_color0;
uint8_t fall_color1;
uint8_t fall_color2;
uint8_t fall_row;
uint8_t fall_col;

uint16_t score;
uint16_t last_score_shown;

uint8_t state;
uint8_t clear_timer;
uint8_t dropping;

uint8_t origin_x;
uint8_t origin_y;

void init_grid(void)
{
    uint8_t r;
    uint8_t c;
    uint8_t sx;
    uint8_t sy;

    for (r = 0; r < GRID_H; r++) {
        for (c = 0; c < GRID_W; c++) {
            grid[r][c] = 0;
            clear_mask[r][c] = 0;

            for (sy = 0; sy < 2; sy++) {
                for (sx = 0; sx < 2; sx++) {
                    displayed[r][c][sy][sx] = 0;
                }
            }
        }
    }
}

void calc_origin(void)
{
    uint16_t w;
    uint16_t h;
    uint16_t min_x;
    uint16_t max_x;
    uint16_t min_y;
    uint16_t max_y;

    w = (uint16_t)GRID_W * 2;
    h = (uint16_t)GRID_H * 2;

    if ((uint16_t)XSize >= w + 4) {
        min_x = 2;
        max_x = (uint16_t)XSize - w - 2;

        if (max_x < min_x) {
            max_x = min_x;
        }

        origin_x = (uint8_t)(min_x + ((max_x - min_x) / 2));
    } else if ((uint16_t)XSize >= w) {
        origin_x = (uint8_t)(((uint16_t)XSize - w) / 2);
    } else {
        origin_x = 0;
    }

    if ((uint16_t)YSize >= h + 5) {
        min_y = 3;
        max_y = (uint16_t)YSize - h - 2;

        if (max_y < min_y) {
            max_y = min_y;
        }

        origin_y = (uint8_t)(min_y + ((max_y - min_y) / 2));
    } else if ((uint16_t)YSize >= h) {
        origin_y = (uint8_t)(((uint16_t)YSize - h) / 2);
    } else {
        origin_y = 0;
    }
}

void draw_borders(void)
{
    uint8_t x;
    uint8_t y;
    uint8_t left_x;
    uint8_t right_x;
    uint8_t top_y;
    uint8_t bottom_y;
    uint16_t w;
    uint16_t h;

    w = (uint16_t)GRID_W * 2;
    h = (uint16_t)GRID_H * 2;

    if (origin_x < 2 || origin_y < 3) {
        return;
    }

    if ((uint16_t)(origin_x + w + 2) > (uint16_t)XSize) {
        return;
    }

    if ((uint16_t)(origin_y + h + 2) > (uint16_t)YSize) {
        return;
    }

    left_x = (uint8_t)(origin_x - 2);
    right_x = (uint8_t)(origin_x + w + 1);
    top_y = (uint8_t)(origin_y - 2);
    bottom_y = (uint8_t)(origin_y + h + 1);

    for (y = top_y; y <= bottom_y; y++) {
        for (x = left_x; x <= right_x; x++) {
            if ((uint16_t)x >= (uint16_t)XSize || (uint16_t)y >= (uint16_t)YSize) {
                continue;
            }

            if ((uint16_t)y < (uint16_t)top_y + 2 ||
                (uint16_t)y > (uint16_t)bottom_y - 2 ||
                (uint16_t)x < (uint16_t)left_x + 2 ||
                (uint16_t)x > (uint16_t)right_x - 2) {
                _XL_DRAW(x, y, TILE_WALL, _XL_CYAN);
            }
        }
    }
}

void spawn_new_triplet(void)
{
    uint8_t c;
    uint8_t can_place;

    fall_color0 = (uint8_t)((uint16_t)_XL_RAND() % NUM_COLORS) + 1;
    fall_color1 = (uint8_t)((uint16_t)_XL_RAND() % NUM_COLORS) + 1;
    fall_color2 = (uint8_t)((uint16_t)_XL_RAND() % NUM_COLORS) + 1;
    fall_row = 0;
    dropping = 0;

    can_place = 0;
    for (c = 0; c < GRID_W; c++) {
        if (grid[0][c] == 0 && grid[1][c] == 0 && grid[2][c] == 0) {
            can_place = 1;
            break;
        }
    }

    if (!can_place) {
        state = STATE_GAME_OVER;
        return;
    }

    fall_col = (uint8_t)((uint16_t)_XL_RAND() % GRID_W);

    while (grid[0][fall_col] != 0 || grid[1][fall_col] != 0 || grid[2][fall_col] != 0) {
        fall_col = (uint8_t)((fall_col + 1) % GRID_W);
    }
}

uint8_t get_cell_color(uint8_t r, uint8_t c)
{
    if (state == STATE_PLAYING) {
        if (r == fall_row && c == fall_col) {
            return fall_color0;
        }

        if (r == (uint8_t)(fall_row + 1) && c == fall_col) {
            return fall_color1;
        }

        if (r == (uint8_t)(fall_row + 2) && c == fall_col) {
            return fall_color2;
        }
    }

    return grid[r][c];
}


void render(void)
{
    uint8_t r;
    uint8_t c;
    uint8_t sx;
    uint8_t sy;
    uint8_t x;
    uint8_t y;
    uint8_t color_idx;
    uint8_t color;

    for (r = 0; r < GRID_H; r++) {
        for (c = 0; c < GRID_W; c++) {
            color_idx = get_cell_color(r, c);

            if (color_idx == 0) {
                color = 0;
            } else {
                color = GAME_COLORS[color_idx];
            }

            if (state == STATE_CLEARING && clear_mask[r][c]) {
                color = _XL_WHITE;
            }

            x = (uint8_t)(origin_x + (c << 1));
            y = (uint8_t)(origin_y + (r << 1));

            for (sy = 0; sy < 2; sy++) {
                if ((uint16_t)(y + sy) >= (uint16_t)YSize) {
                    displayed[r][c][sy][0] = 0;
                    displayed[r][c][sy][1] = 0;
                    continue;
                }

                for (sx = 0; sx < 2; sx++) {
                    if ((uint16_t)(x + sx) >= (uint16_t)XSize) {
                        displayed[r][c][sy][sx] = 0;
                        continue;
                    }

                    if (color == 0) {
                        if (displayed[r][c][sy][sx] != 0) {
                            _XL_DELETE((uint8_t)(x + sx), (uint8_t)(y + sy));
                            displayed[r][c][sy][sx] = 0;
                        }
                    } else {
                        if (displayed[r][c][sy][sx] != color) {
                        _XL_DRAW((uint8_t)(x + sx), (uint8_t)(y + sy), tile[color_idx-1][sy][sx], color);
                            displayed[r][c][sy][sx] = color;
                        }
                    }
                }
            }
        }
    }
}

void update_score_display(void)
{
    if (score != last_score_shown) {
        _XL_SET_TEXT_COLOR(_XL_YELLOW);

        if ((uint16_t)XSize >= 11) {
            _XL_PRINTD(6, 0, 5, score);
        } else if ((uint16_t)XSize >= 5) {
            _XL_PRINTD(0, 0, 5, score);
        }

        last_score_shown = score;
    }
}

void init_game(void)
{
    score = 0;
    last_score_shown = 0xFFFF;
    state = STATE_PLAYING;
    dropping = 0;
    clear_timer = 0;

    calc_origin();
    init_grid();

    _XL_CLEAR_SCREEN();
    draw_borders();

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    if ((uint16_t)XSize >= 11) {
        _XL_PRINT(0, 0, "SCORE");
    }

    update_score_display();
    spawn_new_triplet();
}

void check_matches(void)
{
    uint8_t r;
    uint8_t c;
    uint8_t color;
    uint8_t found;
    uint8_t count;

    for (r = 0; r < GRID_H; r++) {
        for (c = 0; c < GRID_W; c++) {
            clear_mask[r][c] = 0;
        }
    }

    found = 0;

    for (r = 0; r < GRID_H; r++) {
        for (c = 0; (uint8_t)(c + 2) < GRID_W; c++) {
            color = grid[r][c];
            if (color != 0 && color == grid[r][c + 1] && color == grid[r][c + 2]) {
                clear_mask[r][c] = 1;
                clear_mask[r][c + 1] = 1;
                clear_mask[r][c + 2] = 1;
                found = 1;
            }
        }
    }

    for (c = 0; c < GRID_W; c++) {
        for (r = 0; (uint8_t)(r + 2) < GRID_H; r++) {
            color = grid[r][c];
            if (color != 0 && color == grid[r + 1][c] && color == grid[r + 2][c]) {
                clear_mask[r][c] = 1;
                clear_mask[r + 1][c] = 1;
                clear_mask[r + 2][c] = 1;
                found = 1;
            }
        }
    }

    for (r = 0; (uint8_t)(r + 2) < GRID_H; r++) {
        for (c = 0; (uint8_t)(c + 2) < GRID_W; c++) {
            color = grid[r][c];
            if (color != 0 && color == grid[r + 1][c + 1] && color == grid[r + 2][c + 2]) {
                clear_mask[r][c] = 1;
                clear_mask[r + 1][c + 1] = 1;
                clear_mask[r + 2][c + 2] = 1;
                found = 1;
            }
        }
    }

    for (r = 0; (uint8_t)(r + 2) < GRID_H; r++) {
        for (c = 2; c < GRID_W; c++) {
            color = grid[r][c];
            if (color != 0 && color == grid[r + 1][c - 1] && color == grid[r + 2][c - 2]) {
                clear_mask[r][c] = 1;
                clear_mask[r + 1][c - 1] = 1;
                clear_mask[r + 2][c - 2] = 1;
                found = 1;
            }
        }
    }

    if (found) {
        count = 0;

        for (r = 0; r < GRID_H; r++) {
            for (c = 0; c < GRID_W; c++) {
                if (clear_mask[r][c]) {
                    count++;
                }
            }
        }

        score += (uint16_t)(count * 10);

        if (count >= 4) {
            score += 50;
        }

        state = STATE_CLEARING;
        clear_timer = 3;
        _XL_PING_SOUND();
    }
}

void apply_gravity(void)
{
    uint8_t c;
    short r;
    short r2;
    uint8_t temp;

    for (c = 0; c < GRID_W; c++) {
        r = (short)(GRID_H - 1);

        for (r2 = (short)(GRID_H - 1); r2 >= 0; r2--) {
            if (grid[r2][c] != 0) {
                temp = grid[r2][c];
                grid[r2][c] = 0;
                grid[r][c] = temp;
                r--;
            }
        }
    }
}

void place_triplet(void)
{
    grid[fall_row][fall_col] = fall_color0;
    grid[fall_row + 1][fall_col] = fall_color1;
    grid[fall_row + 2][fall_col] = fall_color2;

    _XL_TOCK_SOUND();

    check_matches();

    if (state == STATE_PLAYING) {
        spawn_new_triplet();
    }
}

uint8_t can_fall(void)
{
    if ((uint8_t)(fall_row + 3) >= GRID_H) {
        return 0;
    }

    if (grid[fall_row + 3][fall_col] != 0) {
        return 0;
    }

    return 1;
}

void handle_input(uint8_t input)
{
    uint8_t temp;

    if (_XL_LEFT(input)) {
        if (fall_col > 0) {
            if (grid[fall_row][fall_col - 1] == 0 &&
                grid[fall_row + 1][fall_col - 1] == 0 &&
                grid[fall_row + 2][fall_col - 1] == 0) {
                fall_col--;
                _XL_TICK_SOUND();
            }
        }
    }

    if (_XL_RIGHT(input)) {
        if (fall_col < GRID_W - 1) {
            if (grid[fall_row][fall_col + 1] == 0 &&
                grid[fall_row + 1][fall_col + 1] == 0 &&
                grid[fall_row + 2][fall_col + 1] == 0) {
                fall_col++;
                _XL_TICK_SOUND();
            }
        }
    }

    if (_XL_FIRE(input)) {
        temp = fall_color0;
        fall_color0 = fall_color1;
        fall_color1 = fall_color2;
        fall_color2 = temp;
        _XL_TICK_SOUND();
    }

    if (_XL_DOWN(input)) {
        dropping = 1;
        _XL_SHOOT_SOUND();
    }
}


void update_playing(void)
{
    uint8_t steps = (dropping ? 2 : 1);

    while (steps--) {
        if (can_fall()) {
            fall_row++;
        } else {
            break;
        }
    }

    if (!can_fall()) {
        place_triplet();
    }

    dropping = 0;
}

// void update_playing(void)
// {
    // if (dropping) {
        // while (can_fall()) {
            // fall_row++;
        // }

        // place_triplet();
        // dropping = 0;
    // } else {
        // if (can_fall()) {
            // fall_row++;
        // } else {
            // place_triplet();
        // }
    // }
// }

void update_clearing(void)
{
    uint8_t r;
    uint8_t c;

    if (clear_timer > 0) {
        clear_timer--;
    } else {
        for (r = 0; r < GRID_H; r++) {
            for (c = 0; c < GRID_W; c++) {
                if (clear_mask[r][c]) {
                    grid[r][c] = 0;
                    clear_mask[r][c] = 0;
                }
            }
        }

        apply_gravity();
        state = STATE_PLAYING;
        spawn_new_triplet();
    }
}

void show_game_over(void)
{
    uint8_t x;

    draw_borders();

    _XL_SET_TEXT_COLOR(_XL_RED);

    if ((uint16_t)XSize >= 9 && (uint16_t)YSize >= 5) {
        x = (uint8_t)(((uint16_t)XSize - 9) / 2);
        _XL_PRINT(x, 4, "GAME OVER");
    } else if ((uint16_t)XSize >= 4 && (uint16_t)YSize >= 5) {
        _XL_PRINT(0, 4, "GAME");
    }

    _XL_SET_TEXT_COLOR(_XL_WHITE);

    if ((uint16_t)XSize >= 13 && (uint16_t)YSize >= 7) {
        x = (uint8_t)(((uint16_t)XSize - 13) / 2);
        _XL_PRINT(x, 6, "PRESS ANY KEY");
    } else if ((uint16_t)XSize >= 9 && (uint16_t)YSize >= 7) {
        x = (uint8_t)(((uint16_t)XSize - 9) / 2);
        _XL_PRINT(x, 6, "PRESS KEY");
    } else if ((uint16_t)XSize >= 3 && (uint16_t)YSize >= 7) {
        _XL_PRINT(0, 6, "KEY");
    }
}

int main(void)
{
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        init_game();

        while (state != STATE_GAME_OVER) {
            input = _XL_INPUT();

            if (state == STATE_PLAYING) {
                handle_input(input);
                update_playing();
            } else if (state == STATE_CLEARING) {
                update_clearing();
            }

            render();
            update_score_display();
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();

        _XL_CLEAR_SCREEN();

        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        if ((uint16_t)XSize >= 5) {
            _XL_PRINTD((uint8_t)(((uint16_t)XSize - 5) / 2), 0, 5, score);
        }

        show_game_over();

        _XL_EXPLOSION_SOUND();
        _XL_WAIT_FOR_INPUT();
    }
}


