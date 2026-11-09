#include "cross_lib.h"

#define GRID_W 16
#define GRID_H 10
#define NUM_COLORS 7
#define FALL_INTERVAL 10
#define TIMELINE_INTERVAL 20
#define DISPLAY_EMPTY 0
#define DISPLAY_TIMELINE 8

static uint8_t grid[GRID_H][GRID_W];
static uint8_t display_buf[GRID_H][GRID_W];
static uint8_t piece_x;
static uint8_t piece_y;
static uint8_t piece_color;
static uint8_t piece_mask;
static uint8_t piece_active;
static uint8_t time_line_col;
static uint16_t score;
static uint8_t game_over;
static uint8_t frame_count;
static uint8_t timeline_frame;
static uint16_t last_score;

static uint8_t is_blocked(uint8_t x, uint8_t y)
{
    if (y >= GRID_H) return 1;
    if (x >= GRID_W) return 1;
    return grid[y][x] != 0;
}

static void place_cell(uint8_t x, uint8_t y, uint8_t color)
{
    if (y < GRID_H && x < GRID_W)
    {
        grid[y][x] = color;
    }
}

static uint8_t get_color_id(uint8_t val)
{
    switch (val)
    {
        case 1: return _XL_RED;
        case 2: return _XL_CYAN;
        case 3: return _XL_GREEN;
        case 4: return _XL_YELLOW;
        case 5: return _XL_BLUE;
        case 6: return _XL_MAGENTA;
        case 7: return _XL_WHITE;
        default: return _XL_WHITE;
    }
}

static void spawn_piece(void)
{
    piece_x = (uint8_t)(_XL_RAND() % (GRID_W - 1));
    piece_y = 0;
    piece_color = (uint8_t)(_XL_RAND() % NUM_COLORS) + 1;
    piece_mask = 0x0F;
    piece_active = 1;
}

static void check_game_over(void)
{
    uint8_t c;
    for (c = 0; c < GRID_W; c++)
    {
        if (grid[0][c] != 0)
        {
            game_over = 1;
            return;
        }
    }
}

static void move_piece_down(void)
{
    uint8_t tl_can, tr_can, bl_can, br_can;
    uint8_t c;

    if (!piece_active) return;

    tl_can = 1;
    tr_can = 1;
    bl_can = 1;
    br_can = 1;

    if (piece_mask & 0x01)
    {
        if (!(piece_mask & 0x04))
        {
            if (is_blocked(piece_x, piece_y + 1)) tl_can = 0;
        }
    }
    if (piece_mask & 0x02)
    {
        if (!(piece_mask & 0x08))
        {
            if (is_blocked(piece_x + 1, piece_y + 1)) tr_can = 0;
        }
    }
    if (piece_mask & 0x04)
    {
        if (is_blocked(piece_x, piece_y + 2)) bl_can = 0;
    }
    if (piece_mask & 0x08)
    {
        if (is_blocked(piece_x + 1, piece_y + 2)) br_can = 0;
    }

    if (piece_mask & 0x01)
    {
        if (!tl_can)
        {
            place_cell(piece_x, piece_y, piece_color);
            piece_mask &= 0xFE;
        }
    }
    if (piece_mask & 0x02)
    {
        if (!tr_can)
        {
            place_cell(piece_x + 1, piece_y, piece_color);
            piece_mask &= 0xFD;
        }
    }
    if (piece_mask & 0x04)
    {
        if (!bl_can)
        {
            place_cell(piece_x, piece_y + 1, piece_color);
            piece_mask &= 0xFB;
        }
    }
    if (piece_mask & 0x08)
    {
        if (!br_can)
        {
            place_cell(piece_x + 1, piece_y + 1, piece_color);
            piece_mask &= 0xF7;
        }
    }

    if (piece_mask == 0)
    {
        piece_active = 0;
        check_game_over();
    }
    else
    {
        if (piece_y + 1 >= GRID_H)
        {
            if (piece_mask & 0x01) place_cell(piece_x, piece_y, piece_color);
            if (piece_mask & 0x02) place_cell(piece_x + 1, piece_y, piece_color);
            if (piece_mask & 0x04) place_cell(piece_x, piece_y + 1, piece_color);
            if (piece_mask & 0x08) place_cell(piece_x + 1, piece_y + 1, piece_color);
            piece_active = 0;
            check_game_over();
        }
        else
        {
            piece_y++;
        }
    }
}

static void check_clears(void)
{
    uint8_t r, c;
    uint8_t cleared;

    cleared = 0;
    for (r = 0; r < GRID_H - 1; r++)
    {
        for (c = 0; c < GRID_W - 1; c++)
        {
            if (c != time_line_col && c + 1 != time_line_col) continue;
            if (grid[r][c] != 0 &&
                grid[r][c] == grid[r][c + 1] &&
                grid[r][c] == grid[r + 1][c] &&
                grid[r][c] == grid[r + 1][c + 1])
            {
                grid[r][c] = 0;
                grid[r][c + 1] = 0;
                grid[r + 1][c] = 0;
                grid[r + 1][c + 1] = 0;
                cleared = 1;
            }
        }
    }

    if (cleared)
    {
        score += 100;
        _XL_PING_SOUND();
    }
}

static uint8_t can_move_left(void)
{
    if (piece_x == 0) return 0;
    if ((piece_mask & 0x01) && piece_y < GRID_H)
    {
        if (grid[piece_y][piece_x - 1] != 0) return 0;
    }
    if ((piece_mask & 0x04) && piece_y + 1 < GRID_H)
    {
        if (grid[piece_y + 1][piece_x - 1] != 0) return 0;
    }
    return 1;
}

static uint8_t can_move_right(void)
{
    if (piece_x + 2 >= GRID_W) return 0;
    if ((piece_mask & 0x02) && piece_y < GRID_H)
    {
        if (grid[piece_y][piece_x + 2] != 0) return 0;
    }
    if ((piece_mask & 0x08) && piece_y + 1 < GRID_H)
    {
        if (grid[piece_y + 1][piece_x + 2] != 0) return 0;
    }
    return 1;
}

static void render(void)
{
    uint8_t r, c;

    for (r = 0; r < GRID_H; r++)
    {
        for (c = 0; c < GRID_W; c++)
        {
            uint8_t new_val;
            uint8_t bit;

            new_val = grid[r][c];

            if (piece_active)
            {
                bit = 0;
                if (r == piece_y && c == piece_x) bit = 0x01;
                else if (r == piece_y && c == piece_x + 1) bit = 0x02;
                else if (r == piece_y + 1 && c == piece_x) bit = 0x04;
                else if (r == piece_y + 1 && c == piece_x + 1) bit = 0x08;
                if (bit != 0 && (piece_mask & bit))
                {
                    new_val = piece_color;
                }
            }

            if (c == time_line_col)
            {
                new_val = DISPLAY_TIMELINE;
            }

            if (display_buf[r][c] != new_val)
            {
                if (display_buf[r][c] != DISPLAY_EMPTY)
                {
                    _XL_DELETE(c, r);
                }
                if (new_val != DISPLAY_EMPTY)
                {
                    uint8_t color;
                    uint8_t tile;
                    if (new_val == DISPLAY_TIMELINE)
                    {
                        tile = _TILE_0;
                        color = _XL_WHITE;
                    }
                    else
                    {
                        tile = _TILE_1;
                        color = get_color_id(new_val);
                    }
                    _XL_DRAW(c, r, tile, color);
                }
                display_buf[r][c] = new_val;
            }
        }
    }

    if (score != last_score)
    {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0, GRID_H + 1, 6, score);
        last_score = score;
    }
}

static void reset_game(void)
{
    uint8_t r, c;
    for (r = 0; r < GRID_H; r++)
    {
        for (c = 0; c < GRID_W; c++)
        {
            grid[r][c] = 0;
            display_buf[r][c] = DISPLAY_EMPTY;
        }
    }
    piece_active = 0;
    time_line_col = 0;
    score = 0;
    last_score = 0;
    game_over = 0;
    frame_count = 0;
    timeline_frame = 0;
    _XL_CLEAR_SCREEN();
    spawn_piece();
}

int main(void)
{
    uint8_t input;
    uint8_t r, c;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1)
    {
        reset_game();

        while (!game_over)
        {
            input = _XL_INPUT();
            if (piece_active)
            {
                if (_XL_LEFT(input) && can_move_left())
                {
                    piece_x--;
                }
                if (_XL_RIGHT(input) && can_move_right())
                {
                    piece_x++;
                }
            }

            frame_count++;
            if (frame_count >= FALL_INTERVAL)
            {
                frame_count = 0;
                if (piece_active)
                {
                    move_piece_down();
                }
                else
                {
                    spawn_piece();
                }
            }

            timeline_frame++;
            if (timeline_frame >= TIMELINE_INTERVAL)
            {
                timeline_frame = 0;
                time_line_col++;
                if (time_line_col >= GRID_W)
                {
                    time_line_col = 0;
                }
                else
                {
                    check_clears();
                }
            }

            render();

            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        _XL_CLEAR_SCREEN();
        for (r = 0; r < GRID_H; r++)
        {
            for (c = 0; c < GRID_W; c++)
            {
                display_buf[r][c] = DISPLAY_EMPTY;
            }
        }
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 0, "GAME OVER");
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD(0, 2, 6, score);
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(0, 4, "PRESS ANY KEY TO RESTART");
        _XL_WAIT_FOR_INPUT();
    }

    return 0;
}