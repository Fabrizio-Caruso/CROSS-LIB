#include "cross_lib.h"

/* Game constants */
#define SNAKE_MAX_LENGTH 100
#define INITIAL_SNAKE_LENGTH 3
#define TILE_SNAKE_HEAD _TILE_1
#define TILE_SNAKE_BODY _TILE_2
#define TILE_FOOD _TILE_3
#define TILE_EMPTY _TILE_0

/* Game state variables */
uint8_t snake_x[SNAKE_MAX_LENGTH];
uint8_t snake_y[SNAKE_MAX_LENGTH];
uint8_t snake_length;
uint8_t direction; /* 0=up, 1=right, 2=down, 3=left */
uint8_t food_x, food_y;
uint16_t score;
uint8_t game_over;

/* Function declarations */
void init_game(void);
void spawn_food(void);
void move_snake(void);
uint8_t check_collision(void);
void draw_snake(void);
void draw_food(void);
void handle_input(uint8_t input);
void update_display(void);

// void _XL_INIT_GRAPHICS(void);
// void _XL_INIT_INPUT(void);
// void _XL_INIT_SOUND(void);

void init_game(void)
{
    uint8_t i;

    /* Initialize snake */
    snake_length = INITIAL_SNAKE_LENGTH;
    direction = 1; /* Start moving right */
    score = 0;
    game_over = 0;

    /* Position snake in the middle of the screen */
    for (i = 0; i < snake_length; i++)
    {
        snake_x[i] = XSize / 2 - i;
        snake_y[i] = YSize / 2;
    }

    /* Spawn first food */
    spawn_food();
}

void spawn_food(void)
{
    uint8_t valid_position;
    uint8_t i;

    do
    {
        valid_position = 1;
        food_x = _XL_RAND() % (XSize - 2) + 1;
        food_y = _XL_RAND() % (YSize - 2) + 1;

        /* Check if food is not on snake */
        for (i = 0; i < snake_length; i++)
        {
            if (food_x == snake_x[i] && food_y == snake_y[i])
            {
                valid_position = 0;
                break;
            }
        }
    } while (!valid_position);
}

void move_snake(void)
{
    uint8_t i;
    uint8_t new_head_x, new_head_y;

    /* Calculate new head position */
    new_head_x = snake_x[0];
    new_head_y = snake_y[0];

    switch (direction)
    {
        case 0: /* Up */
            new_head_y--;
            break;
        case 1: /* Right */
            new_head_x++;
            break;
        case 2: /* Down */
            new_head_y++;
            break;
        case 3: /* Left */
            new_head_x--;
            break;
    }

    /* Check if food is eaten */
    if (new_head_x == food_x && new_head_y == food_y)
    {
        snake_length++;
        score += 10;
        _XL_PING_SOUND();
        spawn_food();
    }

    /* Move snake body */
    for (i = snake_length - 1; i > 0; i--)
    {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    /* Move head */
    snake_x[0] = new_head_x;
    snake_y[0] = new_head_y;
}

uint8_t check_collision(void)
{
    uint8_t i;

    /* Check wall collision */
    if (snake_x[0] == 0 || snake_x[0] >= XSize - 1 ||
        snake_y[0] == 0 || snake_y[0] >= YSize - 1)
    {
        return 1;
    }

    /* Check self collision */
    for (i = 1; i < snake_length; i++)
    {
        if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i])
        {
            return 1;
        }
    }

    return 0;
}

void draw_snake(void)
{
    uint8_t i;

    /* Draw head */
    _XL_DRAW(snake_x[0], snake_y[0], TILE_SNAKE_HEAD, _XL_GREEN);

    /* Draw body */
    for (i = 1; i < snake_length; i++)
    {
        _XL_DRAW(snake_x[i], snake_y[i], TILE_SNAKE_BODY, _XL_GREEN);
    }
}

void draw_food(void)
{
    _XL_DRAW(food_x, food_y, TILE_FOOD, _XL_RED);
}

void handle_input(uint8_t input)
{
    if (_XL_UP(input) && direction != 2)
    {
        direction = 0;
    }
    else if (_XL_RIGHT(input) && direction != 3)
    {
        direction = 1;
    }
    else if (_XL_DOWN(input) && direction != 0)
    {
        direction = 2;
    }
    else if (_XL_LEFT(input) && direction != 1)
    {
        direction = 3;
    }
}

void update_display(void)
{
    _XL_CLEAR_SCREEN();

    /* Draw score */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 0, "SCORE");
    _XL_PRINTD(6, 0, 1, score);

    draw_snake();
    draw_food();
}

void main(void)
{
    uint8_t input;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1)
    {
        init_game();
        update_display();

        while (!game_over)
        {
            input = _XL_INPUT();

            if (input != 0)
            {
                handle_input(input);
            }

            move_snake();

            if (check_collision())
            {
                game_over = 1;
                _XL_EXPLOSION_SOUND();

                /* Display game over */
                _XL_CLEAR_SCREEN();
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(XSize / 2 - 4, YSize / 2 - 1, "GAME OVER");
                _XL_SET_TEXT_COLOR(_XL_WHITE);
                _XL_PRINT(XSize / 2 - 5, YSize / 2 + 1, "SCORE");
                _XL_PRINTD(XSize / 2 + 1, YSize / 2 + 1, 1, score);

                _XL_WAIT_FOR_INPUT();
            }
            else
            {
                update_display();
                _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR * 5);
            }
        }
    }
}