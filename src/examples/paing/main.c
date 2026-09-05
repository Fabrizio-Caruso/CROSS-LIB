#include "cross_lib.h"

/* Screen state tracking to satisfy: "Do not use _XL_DRAW or _XL_DELETE on tiles that have not changed." */
uint8_t screen[XSize][YSize] = {0};
uint8_t color[XSize][YSize] = {0};

void set_tile(uint8_t x, uint8_t y, uint8_t tile, uint8_t col) {
    if (x >= XSize || y >= YSize) return;
    if (screen[x][y] != tile || color[x][y] != col) {
        screen[x][y] = tile;
        color[x][y] = col;
        _XL_DRAW(x, y, tile, col);
    }
}

void clear_tile(uint8_t x, uint8_t y) {
    if (x >= XSize || y >= YSize) return;
    if (screen[x][y] != 0 || color[x][y] != 0) {
        screen[x][y] = 0;
        color[x][y] = 0;
        _XL_DELETE(x, y);
    }
}

/* Game state variables (all uint8_t or uint16_t as requested) */
uint8_t player_y, computer_y;
uint8_t ball_x, ball_y, ball_dx, ball_dy; // dx: 0=left,1=right | dy: 0=up,1=down
uint8_t player_score = 0, computer_score = 0;
uint8_t last_player_score = 255, last_computer_score = 255;
uint8_t game_state = 0; /* 0: playing, 1: reset right, 2: reset left, 3: game over */
uint8_t prev_player_y, prev_computer_y;
uint8_t prev_ball_x, prev_ball_y;
uint8_t frame_counter = 0;

void init_game(void) {
    _XL_CLEAR_SCREEN();
    for (uint8_t x = 0; x < XSize; x++) {
        for (uint8_t y = 0; y < YSize; y++) {
            screen[x][y] = 0;
            color[x][y] = 0;
        }
    }

    player_y = YSize / 2 - 1;
    computer_y = YSize / 2 - 1;

    ball_x = XSize / 2;
    ball_y = YSize / 2;
    ball_dx = 1;
    ball_dy = _XL_RAND() % 2 == 0 ? 0 : 1;

    player_score = 0;
    computer_score = 0;
    game_state = 0;

    prev_player_y = player_y;
    prev_computer_y = computer_y;
    prev_ball_x = ball_x;
    prev_ball_y = ball_y;

    /* Draw initial paddles (height 3) and ball */
    for (uint8_t i = 0; i < 3; i++) {
        set_tile(1, player_y + i, _TILE_1, _XL_WHITE);
        set_tile(XSize - 2, computer_y + i, _TILE_1, _XL_WHITE);
    }
    set_tile(ball_x, ball_y, _TILE_3, _XL_WHITE);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINTD(2, 0, 2, player_score);
    _XL_PRINTD(XSize - 4, 0, 2, computer_score);
}

void update_game(void) {
    if (game_state == 3) return; /* Handled in main loop */

    frame_counter++;
    if (frame_counter >= 8) { /* Control game speed using Cross-Lib sleep macro */
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        frame_counter = 0;
    }

    if (game_state != 0) { /* Scoring delay / ball reset phase */
        game_state--;
        if (game_state == 0) {
            ball_x = XSize / 2;
            ball_y = YSize / 2;
            ball_dx = _XL_RAND() % 2 == 0 ? 1 : 0;
            ball_dy = _XL_RAND() % 2 == 0 ? 0 : 1;
        }
        return;
    }

    uint8_t new_player_y = player_y;
    uint8_t new_computer_y = computer_y;

    /* Player Input */
    uint8_t inp = _XL_INPUT();
    if (_XL_UP(inp)) {
        if (new_player_y > 0) new_player_y--;
    } else if (_XL_DOWN(inp)) {
        if (new_player_y < YSize - 3) new_player_y++;
    }

    /* Computer AI (simple tracking with margin) */
    uint8_t comp_center = computer_y + 1;
    if (comp_center < ball_y - 1) {
        new_computer_y++;
    } else if (comp_center > ball_y + 1) {
        new_computer_y--;
    }

    /* Ball Movement (uint8_t safe boundary handling) */
    uint8_t nx = ball_x;
    if (ball_dx) {
        nx++;
    } else {
        if (nx == 0) {
            nx = 1;
            ball_dx = 1; /* Bounce left wall */
        } else {
            nx--;
        }
    }

    uint8_t ny = ball_y;
    if (ball_dy) {
        ny++;
    } else {
        if (ny == 0) {
            ny = 1;
            ball_dy = 1; /* Bounce top wall */
        } else {
            ny--;
        }
    }

    /* Paddle Collision Detection */
    uint8_t hit_paddle = 0;
    if (nx == 1 && ny >= player_y && ny <= player_y + 2) {
        hit_paddle = 1;
        ball_dx = 1;
    } else if (nx == XSize - 2 && ny >= computer_y && ny <= computer_y + 2) {
        hit_paddle = 1;
        ball_dx = 0;
    }

    /* Scoring Logic */
    if (nx == 0 && !hit_paddle) {
        computer_score++;
        game_state = 1; /* Reset to right */
    } else if (nx == XSize - 1 && !hit_paddle) {
        player_score++;
        game_state = 2; /* Reset to left */
    } else {
        uint8_t next_ball_x = nx;
        uint8_t next_ball_y = ny;

        /* Only clear old positions if they moved */
        if (new_player_y != player_y || new_computer_y != computer_y) {
            for (uint8_t i = 0; i < 3; i++) clear_tile(1, prev_player_y + i);
            for (uint8_t i = 0; i < 3; i++) clear_tile(XSize - 2, prev_computer_y + i);
        }

        if (next_ball_x != ball_x || next_ball_y != ball_y) {
            clear_tile(prev_ball_x, prev_ball_y);
        }

        /* Commit new positions */
        player_y = new_player_y;
        computer_y = new_computer_y;
        ball_x = next_ball_x;
        ball_y = next_ball_y;

        prev_player_y = player_y;
        prev_computer_y = computer_y;
        prev_ball_x = ball_x;
        prev_ball_y = ball_y;

        /* Only draw new positions if they changed */
        for (uint8_t i = 0; i < 3; i++) set_tile(1, player_y + i, _TILE_1, _XL_WHITE);
        for (uint8_t i = 0; i < 3; i++) set_tile(XSize - 2, computer_y + i, _TILE_1, _XL_WHITE);
        set_tile(ball_x, ball_y, _TILE_3, _XL_WHITE);

        /* Update scores if changed */
        uint8_t ps_changed = (player_score != last_player_score);
        uint8_t cs_changed = (computer_score != last_computer_score);
        if (ps_changed) { _XL_PRINTD(2, 0, 2, player_score); last_player_score = player_score; }
        if (cs_changed) { _XL_PRINTD(XSize - 4, 0, 2, computer_score); last_computer_score = computer_score; }

        /* Win condition */
        if (player_score >= 7 || computer_score >= 7) {
            game_state = 3;
        }
    }
}

void main(void) {
    while (1) {
        _XL_INIT_GRAPHICS();
        _XL_INIT_INPUT();
        init_game();

        while (1) {
            update_game();
            if (game_state == 3) break; /* Game Over */
        }

        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        if (player_score >= 7) {
            _XL_PRINT(10, YSize / 2, "PLAYER WINS");
        } else {
            _XL_PRINT(10, YSize / 2, "CPU WINS");
        }
        _XL_SLEEP(2); /* Pause before restart */
    }
}
