#include "cross_lib.h"

/* ===== Card Constants ===== */
#define CARD_W      4
#define CARD_H      6
#define NUM_TAB     7
#define NUM_FND     4
#define MAX_PILE    26
#define TAB_Y       8
#define TAB_SPACING 5
#define STOCK_X     0
#define WASTE_X     5
#define FND_X0      10
#define FND_SPACING 5
#define TOP_Y       0

/* Suit indices */
#define HEARTS     0
#define DIAMONDS   1
#define CLUBS      2
#define SPADES     3

/* Card helpers */
#define C_SUIT(c)  ((c) % 4)
#define C_RANK(c)  ((c) / 4)
#define C_RED(c)   ((C_SUIT(c) == HEARTS) || (C_SUIT(c) == DIAMONDS))

/* Game states */
#define ST_PLAY    0
#define ST_WON     1

/* Background color for erasing */
#define BG_COLOR   _XL_BLACK

/* ===== Global State ===== */
static uint8_t g_stock[MAX_PILE];
static uint8_t g_stock_n;
static uint8_t g_waste[MAX_PILE];
static uint8_t g_waste_n;
static uint8_t g_fnd[NUM_FND][MAX_PILE];
static uint8_t g_fnd_n[NUM_FND];
static uint8_t g_tab[NUM_TAB][MAX_PILE];
static uint8_t g_tab_n[NUM_TAB];
static uint8_t g_tab_fd[NUM_TAB];
static uint8_t g_state;
static uint8_t g_moves;
static uint8_t g_has_sel;
static uint8_t g_sel_pile;
static uint8_t g_sel_row;
static uint8_t g_sel_n;
static uint8_t g_cur_col;
static uint8_t g_cur_row;
static uint8_t g_cur_top;
static uint8_t g_top_col;
static uint16_t g_deck[52];

/* Previous cursor position for erase */
static uint8_t g_prev_cx;
static uint8_t g_prev_cy;
static uint8_t g_prev_cur_top;
static uint8_t g_prev_top_col;

/* Previous selection highlight positions */
static uint8_t g_prev_sel_pile;
static uint8_t g_prev_sel_row;
static uint8_t g_prev_sel_n;

/* ===== Prototypes ===== */
static void new_game(void);
static void deal_initial(void);
static void shuffle_deck(void);
static void draw_card_face(uint8_t x, uint8_t y, uint8_t card);
static void draw_card_back(uint8_t x, uint8_t y);
static void draw_empty_box(uint8_t x, uint8_t y);
static void erase_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
static void draw_stock_area(void);
static void draw_waste_area(void);
static void draw_fnd_area(uint8_t idx);
static void draw_tab_area(uint8_t idx);
static void draw_moves_counter(void);
static void erase_cursor(void);
static void draw_cursor(void);
static void erase_sel_highlight(void);
static void draw_sel_highlight(void);
static uint8_t suit_color(uint8_t suit);
static char rank_char(uint8_t rank);
static uint8_t can_stack_tab(uint8_t below, uint8_t above);
static uint8_t can_place_fnd(uint8_t fnd_idx, uint8_t card);
static void try_pickup(void);
static void try_drop(void);
static void draw_stock(void);
static void check_win(void);
static void handle_input(void);
static void move_tab_to_tab(uint8_t from, uint8_t row, uint8_t to, uint8_t n);
static void move_waste_to(uint8_t dest_type, uint8_t dest_idx);
static void move_tab_to_fnd(uint8_t tab_idx, uint8_t fnd_idx);
static void move_waste_to_fnd(uint8_t fnd_idx);
static void move_waste_to_tab(uint8_t tab_idx);
static void move_tab_to_tab_seq(uint8_t from, uint8_t row, uint8_t to);
static void draw_initial_screen(void);

/* ===== Card Display ===== */
static uint8_t suit_color(uint8_t suit)
{
    switch (suit) {
        case HEARTS:   return _XL_RED;
        case DIAMONDS: return _XL_RED;
        case CLUBS:    return _XL_WHITE;
        case SPADES:   return _XL_WHITE;
    }
    return _XL_WHITE;
}

static char rank_char(uint8_t rank)
{
    if (rank < 9) return (char)('2' + rank);
    switch (rank) {
        case 9:  return 'T';
        case 10: return 'J';
        case 11: return 'Q';
        case 12: return 'K';
    }
    return 'A';
}

static void draw_card_face(uint8_t x, uint8_t y, uint8_t card)
{
    uint8_t suit = C_SUIT(card);
    uint8_t rank = C_RANK(card);
    uint8_t color = suit_color(suit);
    uint8_t i;
    char rc;
    char sc;

    for (i = 0; i < CARD_H; i++) {
        _XL_DRAW(x, y + i, _TILE_0, color);
        _XL_DRAW(x + 1, y + i, _TILE_0, color);
        _XL_DRAW(x + 2, y + i, _TILE_0, color);
        _XL_DRAW(x + 3, y + i, _TILE_0, color);
    }

    rc = rank_char(rank);
    sc = (char)('H' + suit);
    _XL_SET_TEXT_COLOR(color);
    _XL_CHAR(x, y + 1, rc);
    _XL_CHAR(x, y + 2, sc);
}

static void draw_card_back(uint8_t x, uint8_t y)
{
    uint8_t i;
    for (i = 0; i < CARD_H; i++) {
        _XL_DRAW(x, y + i, _TILE_1, _XL_BLUE);
        _XL_DRAW(x + 1, y + i, _TILE_1, _XL_BLUE);
        _XL_DRAW(x + 2, y + i, _TILE_1, _XL_BLUE);
        _XL_DRAW(x + 3, y + i, _TILE_1, _XL_BLUE);
    }
}

static void draw_empty_box(uint8_t x, uint8_t y)
{
    _XL_DRAW(x, y, _TILE_2, _XL_GREEN);
    _XL_DRAW(x + 1, y, _TILE_2, _XL_GREEN);
    _XL_DRAW(x + 2, y, _TILE_2, _XL_GREEN);
    _XL_DRAW(x + 3, y, _TILE_2, _XL_GREEN);
    _XL_DRAW(x, y + 5, _TILE_2, _XL_GREEN);
    _XL_DRAW(x + 1, y + 5, _TILE_2, _XL_GREEN);
    _XL_DRAW(x + 2, y + 5, _TILE_2, _XL_GREEN);
    _XL_DRAW(x + 3, y + 5, _TILE_2, _XL_GREEN);
}

/* ===== Erase / Targeted Draw ===== */
static void erase_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    uint8_t i, j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            _XL_DRAW(x + j, y + i, _TILE_0, BG_COLOR);
        }
    }
}

static void draw_stock_area(void)
{
    uint8_t x = STOCK_X;
    uint8_t y = TOP_Y;

    erase_rect(x, y, CARD_W, CARD_H + 1);

    if (g_stock_n > 0) {
        draw_card_back(x, y);
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINTD(x, y + CARD_H, 2, g_stock_n);
    } else {
        draw_empty_box(x, y);
    }
}

static void draw_waste_area(void)
{
    uint8_t x = WASTE_X;
    uint8_t y = TOP_Y;

    erase_rect(x, y, CARD_W, CARD_H);

    if (g_waste_n > 0) {
        draw_card_face(x, y, g_waste[g_waste_n - 1]);
    } else {
        draw_empty_box(x, y);
    }
}

static void draw_fnd_area(uint8_t idx)
{
    uint8_t fx = FND_X0 + idx * FND_SPACING;
    uint8_t y = TOP_Y;

    erase_rect(fx, y, CARD_W, CARD_H);

    if (g_fnd_n[idx] > 0) {
        draw_card_face(fx, y, g_fnd[idx][g_fnd_n[idx] - 1]);
    } else {
        draw_empty_box(fx, y);
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_CHAR(fx + 1, y + 2, (char)('H' + idx));
    }
}

static void draw_tab_area(uint8_t idx)
{
    uint8_t tx = idx * TAB_SPACING;
    uint8_t fd = g_tab_fd[idx];
    uint8_t n = g_tab_n[idx];
    uint8_t r;
    uint8_t ty;

    /* Erase the entire column area */
    erase_rect(tx, TAB_Y, CARD_W, (uint8_t)(YSize - TAB_Y));

    if (n == 0) {
        draw_empty_box(tx, TAB_Y);
        return;
    }

    /* Face-down cards: 1 tile row each */
    for (r = 0; r < fd; r++) {
        ty = TAB_Y + r;
        _XL_DRAW(tx, ty, _TILE_1, _XL_BLUE);
        _XL_DRAW(tx + 1, ty, _TILE_1, _XL_BLUE);
        _XL_DRAW(tx + 2, ty, _TILE_1, _XL_BLUE);
        _XL_DRAW(tx + 3, ty, _TILE_1, _XL_BLUE);
    }

    /* Face-up cards */
    for (r = fd; r < n; r++) {
        uint8_t is_last = (r == n - 1);
        ty = TAB_Y + r;
        if (is_last) {
            draw_card_face(tx, ty, g_tab[idx][r]);
        } else {
            uint8_t sc = suit_color(C_SUIT(g_tab[idx][r]));
            _XL_DRAW(tx, ty, _TILE_0, sc);
            _XL_DRAW(tx + 1, ty, _TILE_0, sc);
            _XL_DRAW(tx + 2, ty, _TILE_0, sc);
            _XL_DRAW(tx + 3, ty, _TILE_0, sc);
            _XL_SET_TEXT_COLOR(sc);
            _XL_CHAR(tx, ty, rank_char(C_RANK(g_tab[idx][r])));
        }
    }
}

static void draw_moves_counter(void)
{
    uint8_t y = (uint8_t)(YSize - 1);
    erase_rect(0, y, 6, 1);
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINTD(0, y, 4, g_moves);
}

static void draw_initial_screen(void)
{
    draw_stock_area();
    draw_waste_area();
    {
        uint8_t i;
        for (i = 0; i < NUM_FND; i++) draw_fnd_area(i);
        for (i = 0; i < NUM_TAB; i++) draw_tab_area(i);
    }
    draw_moves_counter();
    g_prev_cx = 0;
    g_prev_cy = 0;
    g_prev_cur_top = 0;
    g_prev_top_col = 0;
    g_prev_sel_pile = 0;
    g_prev_sel_row = 0;
    g_prev_sel_n = 0;
    draw_cursor();
}

/* ===== Cursor (erase old, draw new) ===== */
static void erase_cursor(void)
{
    uint8_t x, y;
    if (g_prev_cur_top) {
        switch (g_prev_top_col) {
            case 0:  x = STOCK_X; break;
            case 1:  x = WASTE_X; break;
            default: x = FND_X0 + (g_prev_top_col - 2) * FND_SPACING; break;
        }
        y = TOP_Y;
        _XL_DRAW(x, y, _TILE_0, BG_COLOR);
    } else {
        x = g_prev_cx * TAB_SPACING;
        y = TAB_Y + g_prev_cy;
        if (y < YSize - 1) {
            _XL_DRAW(x, y, _TILE_0, BG_COLOR);
        }
    }
}

static void draw_cursor(void)
{
    uint8_t cx, cy;

    erase_cursor();

    if (g_cur_top) {
        switch (g_top_col) {
            case 0: cx = STOCK_X; break;
            case 1: cx = WASTE_X; break;
            default: cx = FND_X0 + (g_top_col - 2) * FND_SPACING; break;
        }
        cy = TOP_Y;
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_CHAR(cx, cy, '^');
    } else {
        cx = g_cur_col;
        cy = g_cur_row;
        {
            uint8_t tx = cx * TAB_SPACING;
            uint8_t ty = TAB_Y + cy;
            if (ty < YSize - 1) {
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_CHAR(tx, ty, '>');
            }
        }
    }

    g_prev_cx = g_cur_col;
    g_prev_cy = g_cur_row;
    g_prev_cur_top = g_cur_top;
    g_prev_top_col = g_top_col;
}

/* ===== Selection Highlight ===== */
static void erase_sel_highlight(void)
{
    if (g_prev_sel_n == 0) return;
    if (g_prev_sel_pile < NUM_TAB) {
        uint8_t i;
        uint8_t tx = g_prev_sel_pile * TAB_SPACING;
        for (i = g_prev_sel_row; i < g_prev_sel_row + g_prev_sel_n; i++) {
            uint8_t ty = TAB_Y + i;
            if (ty < YSize - 1) {
                _XL_DRAW(tx + 3, ty, _TILE_0, BG_COLOR);
            }
        }
    }
    g_prev_sel_n = 0;
}

static void draw_sel_highlight(void)
{
    if (!g_has_sel || g_sel_n == 0) return;
    if (g_sel_pile < NUM_TAB) {
        uint8_t i;
        uint8_t tx = g_sel_pile * TAB_SPACING;
        for (i = g_sel_row; i < g_sel_row + g_sel_n; i++) {
            uint8_t ty = TAB_Y + i;
            if (ty < YSize - 1) {
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_CHAR(tx + 3, ty, '*');
            }
        }
    }
    g_prev_sel_pile = g_sel_pile;
    g_prev_sel_row = g_sel_row;
    g_prev_sel_n = g_sel_n;
}

/* ===== Game Logic ===== */
static void shuffle_deck(void)
{
    uint8_t i;
    uint16_t j;
    uint16_t tmp;
    for (i = 51; i > 0; i--) {
        j = (uint16_t)(_XL_RAND() % (i + 1));
        tmp = g_deck[i];
        g_deck[i] = g_deck[j];
        g_deck[j] = tmp;
    }
}

static void deal_initial(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t idx;

    for (i = 0; i < 52; i++) {
        g_deck[i] = i;
    }

    shuffle_deck();

    idx = 0;
    for (i = 0; i < NUM_TAB; i++) {
        g_tab_n[i] = 0;
        g_tab_fd[i] = 0;
        for (j = 0; j <= i; j++) {
            g_tab[i][g_tab_n[i]] = g_deck[idx++];
            g_tab_n[i]++;
        }
        g_tab_fd[i] = g_tab_n[i] - 1;
    }

    g_stock_n = 0;
    for (i = 0; i < 52 - idx; i++) {
        g_stock[g_stock_n] = g_deck[idx + i];
        g_stock_n++;
    }

    g_waste_n = 0;
    for (i = 0; i < NUM_FND; i++) {
        g_fnd_n[i] = 0;
    }
}

static void new_game(void)
{
    uint8_t i;
    g_state = ST_PLAY;
    g_moves = 0;
    g_has_sel = 0;
    g_cur_col = 0;
    g_cur_row = 0;
    g_cur_top = 0;
    g_top_col = 0;
    for (i = 0; i < NUM_FND; i++) g_fnd_n[i] = 0;
    deal_initial();
}

static uint8_t can_stack_tab(uint8_t below, uint8_t above)
{
    if (C_RANK(above) == C_RANK(below) - 1) {
        if (C_RED(above) != C_RED(below)) return 1;
    }
    return 0;
}

static uint8_t can_place_fnd(uint8_t fnd_idx, uint8_t card)
{
    uint8_t suit = C_SUIT(card);
    uint8_t rank = C_RANK(card);
    if (fnd_idx != suit) return 0;
    if (g_fnd_n[fnd_idx] == 0) {
        return (rank == 0);
    }
    {
        uint8_t top = g_fnd[fnd_idx][g_fnd_n[fnd_idx] - 1];
        return (rank == C_RANK(top) + 1);
    }
}

static void draw_stock(void)
{
    if (g_stock_n > 0) {
        g_waste[g_waste_n] = g_stock[g_stock_n - 1];
        g_waste_n++;
        g_stock_n--;
        _XL_TICK_SOUND();
    } else if (g_waste_n > 0) {
        uint8_t i;
        uint8_t n = g_waste_n;
        for (i = 0; i < n; i++) {
            g_stock[g_stock_n] = g_waste[n - 1 - i];
            g_stock_n++;
        }
        g_waste_n = 0;
        _XL_TOCK_SOUND();
    }
}

static void check_win(void)
{
    uint8_t i;
    uint8_t total = 0;
    for (i = 0; i < NUM_FND; i++) {
        total += g_fnd_n[i];
    }
    if (total == 52) {
        g_state = ST_WON;
        _XL_EXPLOSION_SOUND();
    }
}

/* ===== Move Operations ===== */
static void move_tab_to_tab(uint8_t from, uint8_t row, uint8_t to, uint8_t n)
{
    uint8_t i;

    for (i = 0; i < n; i++) {
        g_tab[to][g_tab_n[to]] = g_tab[from][row + i];
        g_tab_n[to]++;
    }
    g_tab_n[from] = row;
    if (g_tab_fd[from] > g_tab_n[from]) {
        g_tab_fd[from] = g_tab_n[from];
    }
}

static void move_tab_to_tab_seq(uint8_t from, uint8_t row, uint8_t to)
{
    uint8_t n;
    n = g_tab_n[from] - row;
    move_tab_to_tab(from, row, to, n);
}

static void move_waste_to(uint8_t dest_type, uint8_t dest_idx)
{
    uint8_t card;
    if (g_waste_n == 0) return;
    card = g_waste[g_waste_n - 1];
    g_waste_n--;

    if (dest_type == 0) {
        g_fnd[dest_idx][g_fnd_n[dest_idx]] = card;
        g_fnd_n[dest_idx]++;
    } else {
        g_tab[dest_idx][g_tab_n[dest_idx]] = card;
        g_tab_n[dest_idx]++;
    }
}

static void move_tab_to_fnd(uint8_t tab_idx, uint8_t fnd_idx)
{
    uint8_t card;
    if (g_tab_n[tab_idx] == 0) return;
    card = g_tab[tab_idx][g_tab_n[tab_idx] - 1];
    g_tab_n[tab_idx]--;
    if (g_tab_fd[tab_idx] > g_tab_n[tab_idx]) {
        g_tab_fd[tab_idx] = g_tab_n[tab_idx];
    }
    g_fnd[fnd_idx][g_fnd_n[fnd_idx]] = card;
    g_fnd_n[fnd_idx]++;
}

static void move_waste_to_fnd(uint8_t fnd_idx)
{
    move_waste_to(0, fnd_idx);
}

static void move_waste_to_tab(uint8_t tab_idx)
{
    move_waste_to(1, tab_idx);
}

/* ===== Pickup / Drop ===== */
static void try_pickup(void)
{
    if (g_has_sel) {
        g_has_sel = 0;
        g_sel_n = 0;
        _XL_TICK_SOUND();
        return;
    }

    if (g_cur_top) {
        if (g_top_col == 1 && g_waste_n > 0) {
            g_has_sel = 1;
            g_sel_pile = 8;
            g_sel_row = 0;
            g_sel_n = 1;
            _XL_PING_SOUND();
        }
        return;
    }

    {
        uint8_t col = g_cur_col;
        uint8_t row = g_cur_row;
        uint8_t n = g_tab_n[col];

        if (n == 0) return;
        if (row >= n) row = n - 1;
        if (row < g_tab_fd[col]) return;

        {
            uint8_t i;
            uint8_t valid = 1;
            for (i = row; i + 1 < n; i++) {
                if (C_RANK(g_tab[col][i]) != C_RANK(g_tab[col][i + 1]) + 1) {
                    valid = 0;
                    break;
                }
                if (C_RED(g_tab[col][i]) == C_RED(g_tab[col][i + 1])) {
                    valid = 0;
                    break;
                }
            }
            if (!valid) return;
        }

        g_has_sel = 1;
        g_sel_pile = col;
        g_sel_row = row;
        g_sel_n = n - row;
        _XL_PING_SOUND();
    }
}

static void try_drop(void)
{
    uint8_t i;
    uint8_t top_card;
    uint8_t target;
    uint8_t valid;
    uint8_t src_tab = 255;
    uint8_t dst_tab = 255;
    uint8_t moved_fnd = 255;
    uint8_t drew_stock = 0;

    if (!g_has_sel) {
        if (!g_cur_top && g_tab_n[g_cur_col] > 0) {
            uint8_t tc = g_tab_n[g_cur_col] - 1;
            if (tc >= g_tab_fd[g_cur_col]) {
                top_card = g_tab[g_cur_col][tc];
                for (i = 0; i < NUM_FND; i++) {
                    if (can_place_fnd(i, top_card)) {
                        move_tab_to_fnd(g_cur_col, i);
                        g_moves++;
                        _XL_SHOOT_SOUND();
                        src_tab = g_cur_col;
                        moved_fnd = i;
                        check_win();
                        break;
                    }
                }
            }
        }
        /* Update visuals */
        if (src_tab != 255) {
            draw_tab_area(src_tab);
            draw_fnd_area(moved_fnd);
            draw_moves_counter();
        }
        return;
    }

    if (g_sel_pile == 8) {
        top_card = g_waste[g_waste_n - 1];
    } else {
        top_card = g_tab[g_sel_pile][g_sel_row];
    }

    if (g_cur_top) {
        if (g_top_col >= 2) {
            target = g_top_col - 2;
            if (g_sel_n == 1 && can_place_fnd(target, top_card)) {
                if (g_sel_pile == 8) {
                    move_waste_to_fnd(target);
                    moved_fnd = target;
                } else {
                    src_tab = g_sel_pile;
                    move_tab_to_fnd(g_sel_pile, target);
                    moved_fnd = target;
                }
                g_moves++;
                g_has_sel = 0;
                g_sel_n = 0;
                _XL_SHOOT_SOUND();
                check_win();

                if (g_sel_pile == 8) {
                    draw_waste_area();
                } else {
                    draw_tab_area(src_tab);
                }
                draw_fnd_area(moved_fnd);
                draw_moves_counter();
                return;
            }
        }
        if (g_top_col == 0) {
            draw_stock();
            g_moves++;
            g_has_sel = 0;
            g_sel_n = 0;
            drew_stock = 1;
            return;
        }
        g_has_sel = 0;
        g_sel_n = 0;
        _XL_TICK_SOUND();
        return;
    }

    target = g_cur_col;

    if (g_sel_pile < NUM_TAB && g_sel_pile == target) {
        g_has_sel = 0;
        g_sel_n = 0;
        return;
    }

    valid = 0;
    if (g_tab_n[target] == 0) {
        if (g_sel_n == 1 && C_RANK(top_card) == 12) {
            valid = 1;
        }
    } else {
        {
            uint8_t tgt_top = g_tab[target][g_tab_n[target] - 1];
            if (can_stack_tab(tgt_top, top_card)) {
                valid = 1;
            }
        }
    }

    if (valid) {
        dst_tab = target;
        if (g_sel_pile == 8) {
            move_waste_to_tab(target);
        } else {
            src_tab = g_sel_pile;
            move_tab_to_tab_seq(g_sel_pile, g_sel_row, target);
        }
        g_moves++;
        g_has_sel = 0;
        g_sel_n = 0;
        _XL_TOCK_SOUND();
        check_win();

        if (g_sel_pile == 8) {
            draw_waste_area();
        } else {
            draw_tab_area(src_tab);
        }
        draw_tab_area(dst_tab);
        draw_moves_counter();
        return;
    }

    g_has_sel = 0;
    g_sel_n = 0;
    _XL_TICK_SOUND();
}

/* ===== Input Handling ===== */
static void handle_input(void)
{
    uint8_t input;
    uint8_t changed_stock = 0;
    uint8_t changed_waste = 0;
    uint8_t changed_fnd[NUM_FND];
    uint8_t changed_tab[NUM_TAB];
    uint8_t i;
    uint8_t cursor_moved = 0;

    for (i = 0; i < NUM_FND; i++) changed_fnd[i] = 0;
    for (i = 0; i < NUM_TAB; i++) changed_tab[i] = 0;

    input = _XL_INPUT();

    if (_XL_UP(input)) {
        if (g_cur_top) {
            /* stay */
        } else {
            if (g_cur_row > 0) {
                g_cur_row--;
            } else {
                g_cur_top = 1;
                g_top_col = g_cur_col;
                if (g_top_col > 5) g_top_col = 5;
            }
        }
        cursor_moved = 1;
    } else if (_XL_DOWN(input)) {
        if (g_cur_top) {
            g_cur_top = 0;
            g_cur_col = g_top_col;
            if (g_cur_col > 6) g_cur_col = 6;
            g_cur_row = g_tab_n[g_cur_col];
            if (g_cur_row > 0) g_cur_row--;
            if (g_cur_row < g_tab_fd[g_cur_col]) {
                g_cur_row = g_tab_fd[g_cur_col];
            }
        } else {
            if (g_cur_row + 1 < g_tab_n[g_cur_col]) {
                g_cur_row++;
            }
        }
        cursor_moved = 1;
    } else if (_XL_LEFT(input)) {
        if (g_cur_top) {
            if (g_top_col > 0) g_top_col--;
        } else {
            if (g_cur_col > 0) {
                g_cur_col--;
                if (g_cur_row >= g_tab_n[g_cur_col]) {
                    g_cur_row = g_tab_n[g_cur_col];
                    if (g_cur_row > 0) g_cur_row--;
                }
            }
        }
        cursor_moved = 1;
    } else if (_XL_RIGHT(input)) {
        if (g_cur_top) {
            if (g_top_col < 5) g_top_col++;
        } else {
            if (g_cur_col < NUM_TAB - 1) {
                g_cur_col++;
                if (g_cur_row >= g_tab_n[g_cur_col]) {
                    g_cur_row = g_tab_n[g_cur_col];
                    if (g_cur_row > 0) g_cur_row--;
                }
            }
        }
        cursor_moved = 1;
    } else if (_XL_FIRE(input)) {
        if (g_cur_top) {
            if (g_top_col == 0) {
                if (!g_has_sel) {
                    draw_stock();
                    g_moves++;
                    changed_stock = 1;
                    changed_waste = 1;
                } else {
                    try_drop();
                    return; /* try_drop handles its own draws */
                }
            } else if (g_top_col == 1) {
                if (!g_has_sel) {
                    try_pickup();
                } else {
                    try_drop();
                    return;
                }
            } else {
                try_drop();
                return;
            }
        } else {
            if (!g_has_sel) {
                try_pickup();
            } else {
                try_drop();
                return;
            }
        }
    }

    /* Draw only what changed */
    if (changed_stock) draw_stock_area();
    if (changed_waste) draw_waste_area();
    for (i = 0; i < NUM_FND; i++) {
        if (changed_fnd[i]) draw_fnd_area(i);
    }
    for (i = 0; i < NUM_TAB; i++) {
        if (changed_tab[i]) draw_tab_area(i);
    }
    if (g_moves) draw_moves_counter();

    /* Update selection highlight */
    erase_sel_highlight();
    draw_sel_highlight();

    /* Update cursor */
    if (cursor_moved) {
        draw_cursor();
    }
}

/* ===== Win Screen (one-time clear is acceptable for state transition) ===== */
static void show_win_screen(void)
{
    uint8_t input;
    uint8_t restart = 0;

    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(5, 5, "YOU WIN!");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(4, 8, "MOVES:");
    _XL_PRINTD(12, 8, 4, g_moves);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(3, 12, "PRESS FIRE TO RESTART");
    _XL_SLEEP(2);

    while (!restart) {
        input = _XL_INPUT();
        if (_XL_FIRE(input)) {
            restart = 1;
        }
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }
}

/* ===== Main ===== */
int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    new_game();
    draw_initial_screen();

    while (1) {
        if (g_state == ST_PLAY) {
            handle_input();
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        } else {
            show_win_screen();
            new_game();
            draw_initial_screen();
        }
    }

    return 0;
}