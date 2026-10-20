#include "cross_lib.h"

/* Game constants */
#define SNAKE_MAX_LENGTH (((XSize)-2)*((YSize)-2)/2)
#define INITIAL_SNAKE_LENGTH 3
#define TILE_SNAKE_BODY _TILE_2
#define TILE_FOOD       _TILE_3
#define TILE_BORDER     _TILE_26
#define TILE_HEAD_UP    _TILE_1
#define TILE_HEAD_RIGHT _TILE_4
#define TILE_HEAD_DOWN  _TILE_5
#define TILE_HEAD_LEFT  _TILE_6
#define TILE_TAIL_UP    _TILE_7
#define TILE_TAIL_RIGHT _TILE_8
#define TILE_TAIL_DOWN  _TILE_9
#define TILE_TAIL_LEFT  _TILE_10
#define TILE_SPECIAL    _TILE_25

#define SCORE_X 6
#define HISCORE_X (XSize-1-4)

/* Mode constants */
#define MODE_CLASSIC          0
#define MODE_FRENZY           1
#define MODE_INSANITY         2
#define MODE_CLASSIC_WRAP     3
#define MODE_FRENZY_WRAP      4
#define MODE_INSANITY_WRAP    5
#define MODE_DELUXE_EASY      6
#define MODE_DELUXE_MEDIUM    7
#define MODE_DELUXE_HARD      8
#define MODE_COUNT            9

/* Frenzy / insanity maximum simultaneous apples. */
#define FRENZY_MAX_APPLES (XSize/2)

/* Apple arrays must be large enough for the maximum mode. */
#define MAX_APPLES FRENZY_MAX_APPLES

/* Special item points and count */
#define SPECIAL_ITEM_POINTS 5
#define MAX_SPECIAL_COUNT 20

/*
 * Non-classic modes start with one apple.
 * A new apple is unlocked every APPLE_SPAWN_INTERVAL ticks.
 */
#define APPLE_SPAWN_INTERVAL 100

/* Deluxe special item: steps allowed before it vanishes. */
#define DELUXE_STEP_LIMIT ((XSize) + (YSize))

/* Deluxe special item: ticks until it reappears after vanishing. */
#define DELUXE_RESPAWN_INTERVAL 200

#define INSANITY_WALLS ((XSize/3) + (YSize/3))

/* Deluxe wall counts per difficulty. */
#define DELUXE_WALLS_EASY  ((XSize) / 5)
#define DELUXE_WALLS_MED   (((XSize) / 4) + (YSize/5))
#define DELUXE_WALLS_HARD  ((XSize/4) + (YSize/3))

/*
 * Insanity wall count: (XSize + YSize) / 4.
 * Keep at least 1 so the array is not zero-sized.
 */
#define MAX_WALL_COUNT INSANITY_WALLS


#define WALL_COLOR _XL_CYAN
#define SPECIAL_COLOR _XL_YELLOW
#define SELECTION_ARROW_COLOR _XL_YELLOW

/* Wall tile size (2x2 block) and minimum Chebyshev distance between walls. */
#define WALL_SIZE 2
#define WALL_MIN_DIST 4

// BETA
#define BETA 1

/* Game state variables */
uint8_t snake_x[SNAKE_MAX_LENGTH];
uint8_t snake_y[SNAKE_MAX_LENGTH];
uint8_t snake_length;
uint8_t direction;
uint16_t score;
uint8_t game_over;
uint16_t apples_spawned;
uint16_t slowdown;

/* One high-score record per mode */
uint16_t hiscore[MODE_COUNT];

/* Mode / multi-apple / wall state */
uint8_t game_mode;

uint8_t apple_x[MAX_APPLES];
uint8_t apple_y[MAX_APPLES];
uint8_t apple_active[MAX_APPLES];
uint8_t active_apple_count;

uint8_t unlocked_apples;
uint8_t apple_timer;

// uint8_t ate_apple;
uint8_t eaten_apple_x;
uint8_t eaten_apple_y;

uint16_t wall_count;
uint8_t wall_x[MAX_WALL_COUNT];
uint8_t wall_y[MAX_WALL_COUNT];

/* Deluxe special item state */
uint8_t special_x;
uint8_t special_y;
uint8_t special_active;
uint16_t special_timer;
uint16_t special_respawn_timer;

/* deluxe count */
uint8_t special_count;

/* Function declarations */
void init_game(void);
void clear_apples(void);
void init_apples(void);
void maintain_apples_to_target(uint8_t target);
void update_apple_timer(void);
void update_special_item(void);
void spawn_walls(void);
void move_snake(void);
uint8_t check_collision(void);
void draw_border(void);
void draw_walls(void);
void draw_special_item(void);
void draw_snake_initial(void);
void draw_apples(void);
void handle_input(uint8_t input);
uint8_t show_title_screen(void);

static uint8_t is_on_snake(uint8_t x, uint8_t y);
static uint8_t is_wall(uint8_t x, uint8_t y);
static uint8_t is_apple(uint8_t x, uint8_t y);
static uint8_t is_borderless_mode(void);
static uint8_t is_deluxe_mode(void);
static uint8_t find_empty_apple_slot(void);
static uint8_t max_apple_count(void);
static uint8_t spawn_apple_at_slot(uint8_t slot);
static uint8_t spawn_special_item(void);
static uint8_t wall_position_ok(uint8_t x, uint8_t y);
static uint8_t get_head_tile(uint8_t dir);
static uint8_t get_tail_tile_from_delta(signed char dx, signed char dy);


static void display_score(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(SCORE_X, 0, 4, score);
}


static uint8_t is_on_snake(uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < snake_length; i++)
    {
        if (x == snake_x[i] && y == snake_y[i])
            return 1;
    }

    return 0;
}

static uint8_t is_wall(uint8_t x, uint8_t y)
{
    uint16_t i;
    uint8_t k;
    uint8_t cx, cy;

    for (i = 0; i < wall_count; i++)
    {
        for (k = 0; k < (WALL_SIZE * WALL_SIZE); k++)
        {
            cx = wall_x[i] + (uint8_t)(k % WALL_SIZE);
            cy = wall_y[i] + (uint8_t)(k / WALL_SIZE);
            if (x == cx && y == cy)
                return 1;
        }
    }

    return 0;
}

static uint8_t is_apple(uint8_t x, uint8_t y)
{
    uint8_t i;

    for (i = 0; i < MAX_APPLES; i++)
    {
        if (apple_active[i] && x == apple_x[i] && y == apple_y[i])
            return 1;
    }

    return 0;
}

static uint8_t is_borderless_mode(void)
{
    return (game_mode >= MODE_CLASSIC_WRAP);
}

static uint8_t is_deluxe_mode(void)
{
    return (game_mode >= MODE_DELUXE_EASY && game_mode <= MODE_DELUXE_HARD);
}

static uint8_t find_empty_apple_slot(void)
{
    uint8_t i;

    for (i = 0; i < MAX_APPLES; i++)
    {
        if (!apple_active[i])
            return i;
    }

    return 0xFF;
}

static uint8_t max_apple_count(void)
{
    if (game_mode == MODE_CLASSIC || game_mode == MODE_CLASSIC_WRAP)
        return 1;

    if (FRENZY_MAX_APPLES < 1)
        return 1;

    if (FRENZY_MAX_APPLES > MAX_APPLES)
        return (uint8_t)MAX_APPLES;

    return (uint8_t)FRENZY_MAX_APPLES;
}

static uint8_t spawn_apple_at_slot(uint8_t slot)
{
    uint8_t x, y;
    uint8_t valid;
    uint8_t relaxed;
    uint16_t tries;
    uint16_t inner_cells;
    uint16_t corner_threshold;

    if (apple_active[slot])
        return 0;

    inner_cells = (uint16_t)(((XSize) - 2) * ((YSize) - 2));
    corner_threshold = (inner_cells > 10) ? (inner_cells - 10) : 0;

    for (relaxed = 0; relaxed < 2; relaxed++)
    {
        for (tries = 0; tries < 500; tries++)
        {
            valid = 1;

            x = (uint8_t)(_XL_RAND() % (XSize - 2) + 1);
            y = (uint8_t)(_XL_RAND() % (YSize - 2) + 1);

            if (is_on_snake(x, y))
                valid = 0;

            if (is_wall(x, y))
                valid = 0;

            if (is_apple(x, y))
                valid = 0;

            if (is_deluxe_mode() && special_active && x == special_x && y == special_y)
                valid = 0;

            if (relaxed == 0)
            {
                if (apples_spawned < ((XSize) * (YSize)) / 2)
                {
                    if (x == 1 ||
                        x == (uint8_t)(XSize - 2) ||
                        y == 1 ||
                        y == (uint8_t)(YSize - 2))
                    {
                        valid = 0;
                    }
                }

                if (apples_spawned < corner_threshold)
                {
                    if ((x == 1 && y == 1) ||
                        (x == (uint8_t)(XSize - 2) && y == 1) ||
                        (x == 1 && y == (uint8_t)(YSize - 2)) ||
                        (x == (uint8_t)(XSize - 2) && y == (uint8_t)(YSize - 2)))
                    {
                        valid = 0;
                    }
                }
            }

            if (valid)
            {
                apple_x[slot] = x;
                apple_y[slot] = y;
                apple_active[slot] = 1;
                draw_apples();
                active_apple_count++;
                apples_spawned++;
                return 1;
            }
        }
    }

    return 0;
}

static uint8_t spawn_special_item(void)
{
    uint8_t x, y;
    uint8_t valid;
    uint16_t tries;

    if (special_active)
        return 0;

    for (tries = 0; tries < 500; tries++)
    {
        valid = 1;

        x = (uint8_t)(_XL_RAND() % (XSize - 2) + 1);
        y = (uint8_t)(_XL_RAND() % (YSize - 2) + 1);

        if (is_on_snake(x, y))
            valid = 0;
        if (is_wall(x, y))
            valid = 0;
        if (is_apple(x, y))
            valid = 0;

        if (valid)
        {
            special_x = x;
            special_y = y;
            special_active = 1;
            special_timer = (uint16_t)DELUXE_STEP_LIMIT;
            draw_special_item();
            return 1;
        }
    }

    return 0;
}

static uint8_t wall_position_ok(uint8_t x, uint8_t y)
{
    uint16_t i;
    signed char dx, dy;
    uint8_t k;
    uint8_t cx, cy;

    if (x < 1 ||
        x + WALL_SIZE >= (uint8_t)(XSize - 1) ||
        y < 1 ||
        y + WALL_SIZE >= (uint8_t)(YSize - 1))
    {
        return 0;
    }

    for (k = 0; k < (WALL_SIZE * WALL_SIZE); k++)
    {
        cx = x + (uint8_t)(k % WALL_SIZE);
        cy = y + (uint8_t)(k / WALL_SIZE);
        if (is_on_snake(cx, cy))
            return 0;
    }

    for (i = 0; i < wall_count; i++)
    {
        if (x > wall_x[i] + (WALL_SIZE - 1))
            dx = (signed char)(x - (wall_x[i] + WALL_SIZE - 1));
        else if (wall_x[i] > x + (WALL_SIZE - 1))
            dx = (signed char)(wall_x[i] - (x + (WALL_SIZE - 1)));
        else
            dx = 0;

        if (y > wall_y[i] + (WALL_SIZE - 1))
            dy = (signed char)(y - (wall_y[i] + WALL_SIZE - 1));
        else if (wall_y[i] > y + (WALL_SIZE - 1))
            dy = (signed char)(wall_y[i] - (y + (WALL_SIZE - 1)));
        else
            dy = 0;

        if (dx < WALL_MIN_DIST && dy < WALL_MIN_DIST)
            return 0;
    }

    return 1;
}

void clear_apples(void)
{
    uint8_t i;

    for (i = 0; i < MAX_APPLES; i++)
        apple_active[i] = 0;

    active_apple_count = 0;
}

void init_apples(void)
{
    clear_apples();
    apple_timer = 0;
    unlocked_apples = 1;
    maintain_apples_to_target(1);
}

void maintain_apples_to_target(uint8_t target)
{
    uint8_t slot;

    while (active_apple_count < target)
    {
        slot = find_empty_apple_slot();

        if (slot == 0xFF)
            return;

        if (!spawn_apple_at_slot(slot))
            return;
    }
}

void update_apple_timer(void)
{
    uint8_t max_apples;

    if (game_mode == MODE_CLASSIC || game_mode == MODE_CLASSIC_WRAP)
    {
        maintain_apples_to_target(1);
        return;
    }

    max_apples = max_apple_count();

    if (active_apple_count < unlocked_apples)
    {
        maintain_apples_to_target(unlocked_apples);
        return;
    }

    apple_timer++;

    if (apple_timer >= APPLE_SPAWN_INTERVAL)
    {
        apple_timer = 0;

        if (unlocked_apples < max_apples)
            unlocked_apples++;

        maintain_apples_to_target(unlocked_apples);
    }
}

void update_special_item(void)
{
    if (!is_deluxe_mode())
        return;

    if (special_active)
    {
        if (special_timer > 0)
        {
            special_timer--;
        }

        if (special_timer == 0)
        {
            special_active = 0;
            _XL_DELETE(special_x, special_y);
            special_respawn_timer = DELUXE_RESPAWN_INTERVAL;
        }
    }
    else
    {
        if (special_respawn_timer > 0)
            special_respawn_timer--;

        if (special_respawn_timer == 0)
            spawn_special_item();
    }
}

void spawn_walls(void)
{
    uint16_t desired;
    uint16_t i;
    uint8_t placed;
    uint8_t x;
    uint8_t y;
    uint16_t tries;
    uint8_t fx;
    uint8_t fy;

    wall_count = 0;

    if (game_mode == MODE_INSANITY || game_mode == MODE_INSANITY_WRAP)
    {
        desired = (uint16_t) INSANITY_WALLS;
    }
    else if (game_mode == MODE_DELUXE_EASY)
    {
        desired = (uint16_t)DELUXE_WALLS_EASY;
    }
    else if (game_mode == MODE_DELUXE_MEDIUM)
    {
        desired = (uint16_t)DELUXE_WALLS_MED;
    }
    else if (game_mode == MODE_DELUXE_HARD)
    {
        desired = (uint16_t)DELUXE_WALLS_HARD;
    }
    else
    {
        return;
    }

    // if (desired > (uint16_t)MAX_WALL_COUNT)
        // desired = (uint16_t)MAX_WALL_COUNT;

    // if (desired == 0)
        // return;

    /* Random pass */
    for (i = 0; i < desired; i++)
    {
        placed = 0;
        tries = 0;

        while (!placed && tries < 1000)
        {
            x = (uint8_t)(_XL_RAND() % (XSize - WALL_SIZE - 1) + 1);
            y = (uint8_t)(_XL_RAND() % (YSize - WALL_SIZE - 1) + 1);

            if (wall_position_ok(x, y))
            {
                wall_x[wall_count] = x;
                wall_y[wall_count] = y;
                wall_count++;
                placed = 1;
            }

            tries++;
        }

        if (!placed)
            break;
    }

    /* Fallback deterministic scan */
    if (wall_count < desired)
    {
        fx = 1;

        while (fx + WALL_SIZE - 1 < (uint8_t)(XSize - 1) && wall_count < desired)
        {
            fy = 1;

            while (fy + WALL_SIZE - 1 < (uint8_t)(YSize - 1) && wall_count < desired)
            {
                if (wall_position_ok(fx, fy))
                {
                    wall_x[wall_count] = fx;
                    wall_y[wall_count] = fy;
                    wall_count++;
                }

                fy++;
            }

            fx++;
        }
    }
}

void init_game(void)
{
    uint8_t i;

    snake_length = INITIAL_SNAKE_LENGTH;
    direction = 1;
    score = 0;
    game_over = 0;
    slowdown = 6 * _XL_SLOW_DOWN_FACTOR;
    apples_spawned = 0;
    // ate_apple = 0;

    special_active = 0;
    special_x = 0;
    special_y = 0;
    special_timer = 0;
    special_respawn_timer = 0;
    
    special_count = 0;

    for (i = 0; i < snake_length; i++)
    {
        snake_x[i] = (uint8_t)(XSize / 2 - i);
        snake_y[i] = (uint8_t)(YSize / 2);
    }

    spawn_walls();
    init_apples();

    if (is_deluxe_mode())
        spawn_special_item();
}

void move_snake(void)
{
    uint8_t i;
    uint8_t new_head_x, new_head_y;
    uint8_t apple_slot;

    new_head_x = snake_x[0];
    new_head_y = snake_y[0];

    switch (direction)
    {
        case 0: new_head_y--; break;
        case 1: new_head_x++; break;
        case 2: new_head_y++; break;
        case 3: new_head_x--; break;
    }

    /* Wrap-around for borderless modes */
    if (is_borderless_mode())
    {
        if (new_head_x == 0)
            new_head_x = (uint8_t)(XSize - 2);
        else if (new_head_x >= (uint8_t)(XSize - 1))
            new_head_x = 1;

        if (new_head_y == 0)
            new_head_y = (uint8_t)(YSize - 2);
        else if (new_head_y >= (uint8_t)(YSize - 1))
            new_head_y = 1;
    }

    apple_slot = 0xFF;

    for (i = 0; i < MAX_APPLES; i++)
    {
        if (apple_active[i] &&
            apple_x[i] == new_head_x &&
            apple_y[i] == new_head_y)
        {
            apple_slot = i;
            break;
        }
    }

    // ate_apple = 0;

    if (apple_slot != 0xFF)
    {
        // ate_apple = 1;
        eaten_apple_x = apple_x[apple_slot];
        eaten_apple_y = apple_y[apple_slot];

        apple_active[apple_slot] = 0;
        active_apple_count--;

        if (snake_length < (uint8_t)SNAKE_MAX_LENGTH)
            snake_length++;

        score++;
        display_score();
        _XL_PING_SOUND();

        if (score == 10)
            slowdown = 5 * _XL_SLOW_DOWN_FACTOR;
        else if (score == 50)
            slowdown = 4 * _XL_SLOW_DOWN_FACTOR;
        else if (score == 100)
            slowdown = 3 * _XL_SLOW_DOWN_FACTOR;
    }

    /* Check special item pickup */
    if (is_deluxe_mode() && special_active &&
        new_head_x == special_x && new_head_y == special_y)
    {
        special_active = 0;
        _XL_DELETE(special_x, special_y);
        score += SPECIAL_ITEM_POINTS + special_count;
        if(special_count<MAX_SPECIAL_COUNT)
        {
            ++special_count;
        }
        display_score();
        _XL_PING_SOUND();
        special_respawn_timer = DELUXE_RESPAWN_INTERVAL;
    }

    for (i = snake_length - 1; i > 0; i--)
    {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    snake_x[0] = new_head_x;
    snake_y[0] = new_head_y;
}

uint8_t check_collision(void)
{
    uint8_t i;

    if (!is_borderless_mode())
    {
        if (snake_x[0] == 0 ||
            snake_x[0] >= (uint8_t)(XSize - 1) ||
            snake_y[0] == 0 ||
            snake_y[0] >= (uint8_t)(YSize - 1))
        {
            return 1;
        }
    }

    if (is_wall(snake_x[0], snake_y[0]))
        return 1;

    for (i = 1; i < snake_length; i++)
    {
        if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i])
            return 1;
    }

    return 0;
}

void draw_border(void)
{
    uint8_t i;

    if (is_borderless_mode())
        return;

    for (i = 0; i < XSize; i++)
    {
        _XL_DRAW(i, 0, TILE_BORDER, _XL_YELLOW);
        _XL_DRAW(i, YSize - 1, TILE_BORDER, _XL_YELLOW);
    }

    for (i = 1; i < YSize - 1; i++)
    {
        _XL_DRAW(0, i, TILE_BORDER, _XL_YELLOW);
        _XL_DRAW(XSize - 1, i, TILE_BORDER, _XL_YELLOW);
    }
}

void draw_walls(void)
{
    uint16_t i;
    uint8_t k;
    uint8_t cx, cy;

    for (i = 0; i < wall_count; i++)
    {
        for (k = 0; k < (WALL_SIZE * WALL_SIZE); k++)
        {
            cx = wall_x[i] + (uint8_t)(k % WALL_SIZE);
            cy = wall_y[i] + (uint8_t)(k / WALL_SIZE);
            _XL_DRAW(cx, cy, TILE_BORDER, WALL_COLOR);
        }
    }
}

void draw_special_item(void)
{
    if (is_deluxe_mode() && special_active)
        _XL_DRAW(special_x, special_y, TILE_SPECIAL, SPECIAL_COLOR);
}

static uint8_t get_head_tile(uint8_t dir)
{
    if (dir == 0) return TILE_HEAD_UP;
    if (dir == 1) return TILE_HEAD_RIGHT;
    if (dir == 2) return TILE_HEAD_DOWN;
    return TILE_HEAD_LEFT;
}

static uint8_t get_tail_tile_from_delta(signed char dx, signed char dy)
{
    if (dx == 1 && dy == 0) return TILE_TAIL_RIGHT;
    if (dx == -1 && dy == 0) return TILE_TAIL_LEFT;
    if (dx == 0 && dy == 1) return TILE_TAIL_DOWN;
    if (dx == 0 && dy == -1) return TILE_TAIL_UP;
    return TILE_TAIL_RIGHT;
}

void draw_snake_initial(void)
{
    uint8_t i;
    uint8_t head_tile;
    uint8_t tail_tile;
    signed char dx, dy;

    head_tile = get_head_tile(direction);

    if (snake_length > 1)
    {
        dx = (signed char)snake_x[snake_length - 1] - (signed char)snake_x[snake_length - 2];
        dy = (signed char)snake_y[snake_length - 1] - (signed char)snake_y[snake_length - 2];
        tail_tile = get_tail_tile_from_delta(dx, dy);
    }
    else
    {
        tail_tile = get_head_tile(direction);
    }

    for (i = 0; i < snake_length; i++)
    {
        if (i == 0)
            _XL_DRAW(snake_x[i], snake_y[i], head_tile, _XL_GREEN);
        else if (i == snake_length - 1)
            _XL_DRAW(snake_x[i], snake_y[i], tail_tile, _XL_GREEN);
        else
            _XL_DRAW(snake_x[i], snake_y[i], TILE_SNAKE_BODY, _XL_GREEN);
    }
}

void draw_apples(void)
{
    uint8_t i;

    for (i = 0; i < MAX_APPLES; i++)
    {
        if (apple_active[i])
            _XL_DRAW(apple_x[i], apple_y[i], TILE_FOOD, _XL_RED);
    }
}

void handle_input(uint8_t input)
{
    if (_XL_UP(input) && direction != 2)
        direction = 0;
    else if (_XL_RIGHT(input) && direction != 3)
        direction = 1;
    else if (_XL_DOWN(input) && direction != 0)
        direction = 2;
    else if (_XL_LEFT(input) && direction != 1)
        direction = 3;
}

#if XSize<=20
    #define SELECT_X 1
#else
    #define SELECT_X (XSize/2-12)
#endif

uint8_t show_title_screen(void)
{
    uint8_t selected;
    uint8_t input;
    uint8_t old_arrow_y;
    uint8_t new_arrow_y;
    uint8_t y[MODE_COUNT];
    uint8_t i;
    
    selected = MODE_CLASSIC;

    y[0] = 5;   /* 1 CLASSIC    */
    y[1] = 6;   /* 2 FRENZY     */
    y[2] = 7;   /* 3 INSANITY   */
    y[3] = 8;   /* 4 C.WRAP     */
    y[4] = 9;   /* 5 F.WRAP     */
    y[5] = 10;  /* 6 I.WRAP     */
    y[6] = 11;  /* 7 D.EASY     */
    y[7] = 12;  /* 8 D.MED      */
    y[8] = 13;  /* 9 D.HARD     */

    _XL_CLEAR_SCREEN();
    draw_border();

    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(XSize/2-5, 1, "MINISNAKE");

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(SELECT_X, 3, "SELECT MODE");

    _XL_SET_TEXT_COLOR(_XL_GREEN);
    
    #if XSize<=20
    _XL_PRINT(SELECT_X+3, y[0], "CLASSIC");
    _XL_PRINT(SELECT_X+3, y[1], "FRENZY");
    _XL_PRINT(SELECT_X+3, y[2], "INSANE");
    _XL_PRINT(SELECT_X+3, y[3], "CLASSIC 2");
    _XL_PRINT(SELECT_X+3, y[4], "FRENZY  2");
    _XL_PRINT(SELECT_X+3, y[5], "INSANE  2");
    _XL_PRINT(SELECT_X+3, y[6], "DELUXE  1");
    _XL_PRINT(SELECT_X+3, y[7], "DELUXE  2");
    _XL_PRINT(SELECT_X+3, y[8], "DELUXE  3");
    for(i=0;i<9;++i)
    {
        
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(SELECT_X+2, y[i], 1, i+1);
        _XL_PRINTD(SELECT_X+4+9, y[i], 4, hiscore[i]);
    }
    #else
    _XL_PRINT(SELECT_X+4, y[0], "CLASSIC");
    _XL_PRINT(SELECT_X+4, y[1], "FRENZY");
    _XL_PRINT(SELECT_X+4, y[2], "INSANE");
    _XL_PRINT(SELECT_X+4, y[3], "CLASSIC WRAP");
    _XL_PRINT(SELECT_X+4, y[4], "FRENZY  WRAP");
    _XL_PRINT(SELECT_X+4, y[5], "INSANE  WRAP");
    _XL_PRINT(SELECT_X+4, y[6], "DELUXE EASY");
    _XL_PRINT(SELECT_X+4, y[7], "DELUXE MEDIUM");
    _XL_PRINT(SELECT_X+4, y[8], "DELUXE HARD");
    for(i=0;i<9;++i)
    {
        
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(SELECT_X+2, y[i], 1, i+1);
        _XL_PRINTD(SELECT_X+4+14, y[i], 4, hiscore[i]);
    }
    #endif



    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(SELECT_X, YSize - 2, "FIRE TO START");
    
    #if defined(BETA)
    _XL_PRINT(XSize-1-4,YSize-1, "BETA");
    #endif

    old_arrow_y = y[selected];
    _XL_SET_TEXT_COLOR(SELECTION_ARROW_COLOR);
    _XL_DRAW(SELECT_X, old_arrow_y, TILE_HEAD_DOWN, _XL_GREEN);

    while (1)
    {
        input = _XL_INPUT();

        if (input != 0)
        {
            if (_XL_FIRE(input))
                return selected;

            if (_XL_UP(input))
            {
                if (selected == MODE_CLASSIC)
                    selected = (uint8_t)(MODE_COUNT - 1);
                else
                    selected--;

                _XL_SET_TEXT_COLOR(_XL_BLACK);
                _XL_PRINT(SELECT_X, old_arrow_y, "  ");

                new_arrow_y = y[selected];
                _XL_DRAW(SELECT_X, new_arrow_y, TILE_HEAD_DOWN, _XL_GREEN);
                old_arrow_y = new_arrow_y;
            }
            else if (_XL_DOWN(input))
            {
                if (selected == (uint8_t)(MODE_COUNT - 1))
                    selected = MODE_CLASSIC;
                else
                    selected++;

                _XL_SET_TEXT_COLOR(_XL_BLACK);
                _XL_PRINT(SELECT_X, old_arrow_y, "  ");

                new_arrow_y = y[selected];
                _XL_DRAW(SELECT_X, new_arrow_y, TILE_HEAD_DOWN, _XL_GREEN);
                old_arrow_y = new_arrow_y;
            }
        }

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 4);
    }
}


int main(void)
{
    uint8_t input;
    uint8_t old_head_x, old_head_y;
    uint8_t old_tail_x, old_tail_y;
    uint8_t old_length;
    uint8_t head_tile, tail_tile;
    signed char dx, dy;
    uint8_t mode;
    uint8_t i;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    for (i = 0; i < MODE_COUNT; i++)
        hiscore[i] = 0;

    while (1)
    {
        mode = show_title_screen();
        game_mode = mode;

        init_game();

        _XL_CLEAR_SCREEN();
        draw_border();

        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(HISCORE_X - 2, 0, "HI");

        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(SCORE_X - 5, 0, "SCORE");

        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(SCORE_X, 0, 4, score);
        _XL_PRINTD(HISCORE_X, 0, 4, hiscore[game_mode]);

        draw_walls();
        draw_snake_initial();
        draw_apples();
        draw_special_item();

        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();

        while (!game_over)
        {
            input = _XL_INPUT();

            if (input != 0)
                handle_input(input);

            old_head_x = snake_x[0];
            old_head_y = snake_y[0];
            old_tail_x = snake_x[snake_length - 1];
            old_tail_y = snake_y[snake_length - 1];
            old_length = snake_length;

            move_snake();

            if (check_collision())
            {
                game_over = 1;
                _XL_EXPLOSION_SOUND();
                _XL_SLEEP(1);

                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(XSize / 2 - 4, YSize / 2 - 1, "GAME OVER");

                _XL_SLEEP(1);

                if (score > hiscore[game_mode])
                {
                    hiscore[game_mode] = score;
                    _XL_SET_TEXT_COLOR(_XL_YELLOW);
                    _XL_PRINT(XSize / 2 - 6, YSize / 2 + 1, "NEW HIGH SCORE");
                    _XL_SLEEP(1);
                }

                _XL_WAIT_FOR_INPUT();
            }
            else
            {
                head_tile = get_head_tile(direction);

                if (snake_length > 1)
                {
                    dx = (signed char)snake_x[snake_length - 1] - (signed char)snake_x[snake_length - 2];
                    dy = (signed char)snake_y[snake_length - 1] - (signed char)snake_y[snake_length - 2];
                    tail_tile = get_tail_tile_from_delta(dx, dy);
                }
                else
                {
                    tail_tile = head_tile;
                }

                // if (ate_apple)
                    // _XL_DELETE(eaten_apple_x, eaten_apple_y);

                if (snake_length > old_length)
                {
                    _XL_DRAW(old_head_x, old_head_y, TILE_SNAKE_BODY, _XL_GREEN);
                    _XL_DRAW(snake_x[0], snake_y[0], head_tile, _XL_GREEN);
                    _XL_DRAW(snake_x[snake_length - 1], snake_y[snake_length - 1], tail_tile, _XL_GREEN);
                }
                else
                {
                    _XL_DELETE(old_tail_x, old_tail_y);
                    _XL_DRAW(old_head_x, old_head_y, TILE_SNAKE_BODY, _XL_GREEN);
                    _XL_DRAW(snake_x[0], snake_y[0], head_tile, _XL_GREEN);
                    _XL_DRAW(snake_x[snake_length - 1], snake_y[snake_length - 1], tail_tile, _XL_GREEN);
                }

                update_apple_timer();
                update_special_item();

                _XL_SLOW_DOWN(slowdown);
            }
        }
    }
}

