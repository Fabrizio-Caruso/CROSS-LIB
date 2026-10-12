#include "cross_lib.h"

#define GRID_W 8
#define GRID_H 8
#define GRID_X 4
#define GRID_Y 3
#define NUM_COLORS 4
#define STATE_PLAYING 0
#define STATE_CLEARING 1
#define STATE_GAME_OVER 2

uint8_t grid[GRID_H][GRID_W];
uint8_t clear_mask[GRID_H][GRID_W];
uint8_t displayed[GRID_H][GRID_W];
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

void init_grid(void)
{
    uint8_t r, c;
    for (r = 0; r < GRID_H; r++) {
        for (c = 0; c < GRID_W; c++) {
            grid[r][c] = 0;
            clear_mask[r][c] = 0;
            displayed[r][c] = 0;
        }
    }
}

void draw_borders(void)
{
    uint8_t y;
    uint8_t left_x;
    uint8_t right_x;

    left_x = (uint8_t)(GRID_X - 1);
    right_x = (uint8_t)(GRID_X + GRID_W);

    for (y = (uint8_t)(GRID_Y - 1); y <= (uint8_t)(GRID_Y + GRID_H); y++) {
        _XL_DRAW(left_x, y, _TILE_3, _XL_CYAN);
        _XL_DRAW(right_x, y, _TILE_3, _XL_CYAN);
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

void init_game(void)
{
    score = 0;
    last_score_shown = 0xFFFF;
    state = STATE_PLAYING;
    dropping = 0;
    clear_timer = 0;
    init_grid();
    _XL_CLEAR_SCREEN();
    draw_borders();
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 0, "SCORE");
    spawn_new_triplet();
}

uint8_t get_cell_color(uint8_t r, uint8_t c)
{
    if (state == STATE_PLAYING) {
        if (r == fall_row && c == fall_col) return fall_color0;
        if (r == (uint8_t)(fall_row + 1) && c == fall_col) return fall_color1;
        if (r == (uint8_t)(fall_row + 2) && c == fall_col) return fall_color2;
    }
    return grid[r][c];
}

void render(void)
{
    uint8_t r, c;
    uint8_t x, y, color;

    for (r = 0; r < GRID_H; r++) {
        for (c = 0; c < GRID_W; c++) {
            x = (uint8_t)(GRID_X + c);
            y = (uint8_t)(GRID_Y + r);
            color = get_cell_color(r, c);

            if (state == STATE_CLEARING && clear_mask[r][c]) {
                color = _XL_WHITE;
            }

            if (color == 0) {
                if (displayed[r][c] != 0) {
                    _XL_DELETE(x, y);
                    displayed[r][c] = 0;
                }
            } else {
                if (displayed[r][c] != color) {
                    _XL_DRAW(x, y, _TILE_1, color);
                    displayed[r][c] = color;
                }
            }
        }
    }
}

void update_score_display(void)
{
    if (score != last_score_shown) {
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD(9, 0, 5, score);
        last_score_shown = score;
    }
}

void check_matches(void)
{
    uint8_t r, c;
    uint8_t color;
    uint8_t found;
    uint8_t count;

    for (r = 0; r < GRID_H; r++)
        for (c = 0; c < GRID_W; c++)
            clear_mask[r][c] = 0;

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
        for (r = 0; r < GRID_H; r++)
            for (c = 0; c < GRID_W; c++)
                if (clear_mask[r][c]) count++;

        score += (uint16_t)(count * 10);
        if (count >= 4) score += 50;

        state = STATE_CLEARING;
        clear_timer = 3;
        _XL_PING_SOUND();
    }
}

void apply_gravity(void)
{
    uint8_t c;
    short r, r2;
    uint8_t temp;

    for (c = 0; c < GRID_W; c++) {
        r = (uint8_t)(GRID_H - 1);
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
    if ((uint8_t)(fall_row + 3) >= GRID_H) return 0;
    if (grid[fall_row + 3][fall_col] != 0) return 0;
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
    if (dropping) {
        while (can_fall()) {
            fall_row++;
        }
        place_triplet();
        dropping = 0;
    } else {
        if (can_fall()) {
            fall_row++;
        } else {
            place_triplet();
        }
    }
}

void update_clearing(void)
{
    uint8_t r, c;

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
    draw_borders();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(2, 12, "GAME OVER");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 14, "PRESS ANY KEY");
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

        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD(4, 4, 5, score);
        show_game_over();
        _XL_EXPLOSION_SOUND();
        _XL_WAIT_FOR_INPUT();
    }
}