#include "cross_lib.h"
#include <stdint.h>

/* Dungeon size */
#define GRID_SIZE 10
#define NUM_ROOMS 100

/* Exit codes */
#define NO_EXIT 255

/* Enemy types */
#define ENEMY_NONE 0
#define ENEMY_GUARD 1
#define ENEMY_DRAGON 2
#define ENEMY_SKELETON 3
#define ENEMY_WRAITH 4
#define ENEMY_DARKLORD 5

/* Item types */
#define ITEM_NONE 0
#define ITEM_POTION 1
#define ITEM_SWORD 2
#define ITEM_AMULET 3
#define ITEM_KEY 4

/*
 * Room data.
 *
 * These are generated at runtime by generate_random_dungeon().
 * If your previous code declared these as `const`, remove `const`.
 */
static uint8_t room_north[NUM_ROOMS];
static uint8_t room_south[NUM_ROOMS];
static uint8_t room_east[NUM_ROOMS];
static uint8_t room_west[NUM_ROOMS];

static uint8_t room_enemy[NUM_ROOMS];
static uint8_t room_item[NUM_ROOMS];
static uint8_t room_damage[NUM_ROOMS];

/*
 * Score can now be > 255, so use uint16_t.
 * If you keep everything under 255, uint8_t is also okay.
 */
static uint16_t room_score[NUM_ROOMS];

static uint8_t enemy_max_hp[NUM_ROOMS];

/* Mutable per-run state */
static uint8_t enemy_alive[NUM_ROOMS];
static uint8_t item_remaining[NUM_ROOMS];
static uint16_t score_remaining[NUM_ROOMS];

/*
 * Tiny deterministic RNG.
 * This avoids depending on libc rand() in your target environment.
 */
static uint32_t rng_state;

static void rng_seed(uint32_t seed)
{
    if (seed == 0u)
        seed = 1u;

    rng_state = seed;
}

static uint32_t rng_next(void)
{
    uint32_t x = rng_state;

    x ^= (x << 13);
    x ^= (x >> 17);
    x ^= (x << 5);

    rng_state = x;
    return x;
}

static uint8_t rng_range(uint8_t max)
{
    if (max <= 1u)
        return 0;

    return (uint8_t)(rng_next() % (uint32_t)max);
}

/*
 * Open a two-way wall between adjacent rooms a and b.
 */
static void open_wall(uint8_t a, uint8_t b)
{
    int ar, ac, br, bc;

    ar = (int)a / GRID_SIZE;
    ac = (int)a % GRID_SIZE;

    br = (int)b / GRID_SIZE;
    bc = (int)b % GRID_SIZE;

    if (br == ar - 1) {
        /* b is north of a */
        room_south[a] = b;
        room_north[b] = a;
    } else if (br == ar + 1) {
        /* b is south of a */
        room_north[a] = b;
        room_south[b] = a;
    } else if (bc == ac + 1) {
        /* b is east of a */
        room_east[a] = b;
        room_west[b] = a;
    } else if (bc == ac - 1) {
        /* b is west of a */
        room_west[a] = b;
        room_east[b] = a;
    }
}

static void clear_dungeon(void)
{
    int i;

    for (i = 0; i < NUM_ROOMS; i++) {
        room_north[i] = NO_EXIT;
        room_south[i] = NO_EXIT;
        room_east[i] = NO_EXIT;
        room_west[i] = NO_EXIT;

        room_enemy[i] = ENEMY_NONE;
        room_item[i] = ITEM_NONE;
        room_damage[i] = 0;
        room_score[i] = 0;
        enemy_max_hp[i] = 0;

        enemy_alive[i] = ENEMY_NONE;
        item_remaining[i] = ITEM_NONE;
        score_remaining[i] = 0;
    }
}

/*
 * Pick a random room with:
 *   - not start room 0
 *   - not final room 99
 *   - Manhattan distance from start between min_dist and max_dist
 *   - optionally avoid rooms that already have an item
 *   - optionally avoid rooms that already have an enemy
 */
static int random_room_excluding(int min_dist,
                                 int max_dist,
                                 int avoid_item,
                                 int avoid_enemy)
{
    int tries;
    int r;
    int d;

    /* Random try first */
    for (tries = 0; tries < 2000; tries++) {
        r = (int)rng_range(NUM_ROOMS);

        if (r == 0 || r == 99)
            continue;

        d = (r / GRID_SIZE) + (r % GRID_SIZE);

        if (d < min_dist || d > max_dist)
            continue;

        if (avoid_item && room_item[r] != ITEM_NONE)
            continue;

        if (avoid_enemy && room_enemy[r] != ENEMY_NONE)
            continue;

        return r;
    }

    /* Fallback: scan deterministically */
    for (r = 1; r < 99; r++) {
        d = (r / GRID_SIZE) + (r % GRID_SIZE);

        if (d < min_dist || d > max_dist)
            continue;

        if (avoid_item && room_item[r] != ITEM_NONE)
            continue;

        if (avoid_enemy && room_enemy[r] != ENEMY_NONE)
            continue;

        return r;
    }

    return -1;
}

static uint8_t make_enemy_hp(uint8_t enemy_type, uint8_t dist)
{
    uint8_t hp;

    switch (enemy_type) {
        case ENEMY_GUARD:
            hp = (uint8_t)(12 + dist * 2);
            break;

        case ENEMY_SKELETON:
            hp = (uint8_t)(16 + dist * 2);
            break;

        case ENEMY_WRAITH:
            hp = (uint8_t)(20 + dist * 3);
            break;

        case ENEMY_DRAGON:
            hp = (uint8_t)(30 + dist * 3);
            break;

        case ENEMY_DARKLORD:
            return 100;

        default:
            hp = 0;
            break;
    }

    if (hp > 90)
        hp = 90;

    return hp;
}

static void place_enemy(uint8_t room, uint8_t enemy_type)
{
    uint8_t dist;

    room_enemy[room] = enemy_type;

    dist = (uint8_t)((room / GRID_SIZE) + (room % GRID_SIZE));
    enemy_max_hp[room] = make_enemy_hp(enemy_type, dist);
}

/*
 * Generate a new random dungeon.
 *
 * Start room: 0
 * Final room: 99
 *
 * Guarantees:
 *   - every room is connected to start
 *   - therefore every enemy/trap/boss/item is reachable
 */
void generate_random_dungeon(uint32_t seed)
{
    uint8_t visited[NUM_ROOMS];
    uint8_t stack[NUM_ROOMS];

    int sp = 0;
    int i, r, c;
    int top, next;
    int optc, n;

    int a, dir, nr, nc, b2;
    int b;
    int item_room;
    int trap_room;
    int d;
    int chance;
    int roll;
    int placed;
    int tries;

    uint8_t enemy_type;

    int options[4];

    rng_seed(seed);
    clear_dungeon();

    /*
     * Generate a random DFS spanning tree maze.
     *
     * This creates exactly enough walls to make all 100 rooms reachable
     * from room 0.
     */
    for (i = 0; i < NUM_ROOMS; i++)
        visited[i] = 0;

    visited[0] = 1;
    stack[sp++] = 0;

    while (sp > 0) {
        top = stack[sp - 1];
        r = top / GRID_SIZE;
        c = top % GRID_SIZE;

        optc = 0;

        if (r > 0) {
            n = top - GRID_SIZE;
            if (!visited[n])
                options[optc++] = n;
        }

        if (r + 1 < GRID_SIZE) {
            n = top + GRID_SIZE;
            if (!visited[n])
                options[optc++] = n;
        }

        if (c + 1 < GRID_SIZE) {
            n = top + 1;
            if (!visited[n])
                options[optc++] = n;
        }

        if (c > 0) {
            n = top - 1;
            if (!visited[n])
                options[optc++] = n;
        }

        if (optc == 0) {
            sp--;
            continue;
        }

        next = options[(int)rng_range((uint8_t)optc)];

        open_wall((uint8_t)top, (uint8_t)next);

        visited[next] = 1;
        stack[sp++] = (uint8_t)next;
    }

    /*
     * Optional: add a few extra passages so the dungeon feels less like
     * a pure perfect maze.
     *
     * Removing more walls never breaks reachability.
     */
    for (i = 0; i < 10; i++) {
        a = (int)rng_range(NUM_ROOMS);
        r = a / GRID_SIZE;
        c = a % GRID_SIZE;

        dir = (int)rng_range(4);

        nr = r;
        nc = c;

        if (dir == 0)
            nr--;
        else if (dir == 1)
            nr++;
        else if (dir == 2)
            nc++;
        else
            nc--;

        if (nr >= 0 && nc >= 0 && nr < GRID_SIZE && nc < GRID_SIZE) {
            b2 = nr * GRID_SIZE + nc;
            open_wall((uint8_t)a, (uint8_t)b2);
        }
    }

    /*
     * Base score for rooms.
     * Farther rooms give more score.
     */
    for (i = 0; i < NUM_ROOMS; i++) {
        r = i / GRID_SIZE;
        c = i % GRID_SIZE;

        if (i == 0) {
            room_score[i] = 0;
        } else if (i == 99) {
            room_score[i] = 1000;
        } else {
            room_score[i] = (uint16_t)(5 + (r + c) * 5);
        }
    }

    /*
     * Final boss in bottom-right room.
     */
    room_enemy[99] = ENEMY_DARKLORD;
    enemy_max_hp[99] = 100;
    room_score[99] = 1000;

    /*
     * A few mini-bosses deeper into the dungeon.
     */
    b = random_room_excluding(7, 10, 1, 1);
    if (b >= 0) {
        place_enemy((uint8_t)b, ENEMY_DRAGON);
        room_score[b] = 200;
    }

    b = random_room_excluding(10, 14, 1, 1);
    if (b >= 0) {
        place_enemy((uint8_t)b, ENEMY_WRAITH);
        room_score[b] = 250;
    }

    b = random_room_excluding(13, 17, 1, 1);
    if (b >= 0) {
        place_enemy((uint8_t)b, ENEMY_DRAGON);
        room_score[b] = 300;
    }

    /*
     * Random enemies, scaled by distance from start.
     */
    for (r = 1; r < 99; r++) {
        if (room_enemy[r] != ENEMY_NONE)
            continue;

        d = (r / GRID_SIZE) + (r % GRID_SIZE);

        /* Keep the very start mostly safe */
        if (d <= 2)
            continue;

        chance = 25 + d * 3;
        if (chance > 85)
            chance = 85;

        if ((int)rng_range(100) < chance) {
            roll = (int)rng_range(100);

            if (d < 6) {
                if (roll < 70)
                    enemy_type = ENEMY_GUARD;
                else
                    enemy_type = ENEMY_SKELETON;
            } else if (d < 10) {
                if (roll < 60)
                    enemy_type = ENEMY_SKELETON;
                else
                    enemy_type = ENEMY_WRAITH;
            } else if (d < 14) {
                if (roll < 55)
                    enemy_type = ENEMY_WRAITH;
                else
                    enemy_type = ENEMY_DRAGON;
            } else {
                if (roll < 45)
                    enemy_type = ENEMY_DRAGON;
                else
                    enemy_type = ENEMY_WRAITH;
            }

            place_enemy((uint8_t)r, enemy_type);
        }
    }

    /*
     * Items.
     *
     * These are all placed in normal rooms, not the start or final room.
     */
    item_room = random_room_excluding(2, 6, 1, 0);
    if (item_room >= 0)
        room_item[item_room] = ITEM_SWORD;

    item_room = random_room_excluding(4, 12, 1, 0);
    if (item_room >= 0)
        room_item[item_room] = ITEM_AMULET;

    item_room = random_room_excluding(6, 16, 1, 0);
    if (item_room >= 0)
        room_item[item_room] = ITEM_KEY;

    for (i = 0; i < 16; i++) {
        item_room = random_room_excluding(1, 18, 1, 0);
        if (item_room >= 0)
            room_item[item_room] = ITEM_POTION;
    }

    /*
     * Traps.
     *
     * Damage increases with distance from start.
     */
    for (i = 0; i < 25; i++) {
        placed = 0;

        for (tries = 0; tries < 1000 && !placed; tries++) {
            trap_room = (int)rng_range(NUM_ROOMS);

            if (trap_room == 0 || trap_room == 99)
                continue;

            if (room_damage[trap_room] != 0)
                continue;

            d = (trap_room / GRID_SIZE) + (trap_room % GRID_SIZE);

            room_damage[trap_room] =
                (uint8_t)(3 + (d * 3) / 2 + (int)rng_range(3));

            if (room_damage[trap_room] > 30)
                room_damage[trap_room] = 30;

            placed = 1;
        }
    }
}

#include <stdio.h>

void draw_dungeon_map(void)
{
    char map[2 * GRID_SIZE + 1][2 * GRID_SIZE + 1];
    int i, j, room, r, c;

    /* Fill with walls */
    for (i = 0; i < 2 * GRID_SIZE + 1; i++) {
        for (j = 0; j < 2 * GRID_SIZE + 1; j++) {
            map[i][j] = '#';
        }
    }

    /* Draw room centers */
    for (room = 0; room < NUM_ROOMS; room++) {
        r = room / GRID_SIZE;
        c = room % GRID_SIZE;

        if (room == 0)
            map[2 * r + 1][2 * c + 1] = 'S';
        else if (room == 99)
            map[2 * r + 1][2 * c + 1] = 'D';
        else
            map[2 * r + 1][2 * c + 1] = '.';
    }

    /* Open walls between connected rooms */
    for (room = 0; room < NUM_ROOMS; room++) {
        r = room / GRID_SIZE;
        c = room % GRID_SIZE;

        if (r > 0 && room_north[room] != NO_EXIT)
            map[2 * r][2 * c + 1] = ' ';

        if (r + 1 < GRID_SIZE && room_south[room] != NO_EXIT)
            map[2 * r + 2][2 * c + 1] = ' ';

        if (c > 0 && room_west[room] != NO_EXIT)
            map[2 * r + 1][2 * c] = ' ';

        if (c + 1 < GRID_SIZE && room_east[room] != NO_EXIT)
            map[2 * r + 1][2 * c + 2] = ' ';
    }

    /* Print map */
    for (i = 0; i < 2 * GRID_SIZE + 1; i++) {
        for (j = 0; j < 2 * GRID_SIZE + 1; j++)
            putchar(map[i][j]);
        putchar('\n');
    }
}

/*
 * Optional validation helper.
 *
 * Returns 1 if every room is reachable from room 0.
 * The generator above should always pass this.
 */
int dungeon_is_reachable_from_start(void)
{
    uint8_t seen[NUM_ROOMS];
    uint8_t stack[NUM_ROOMS];

    int sp = 0;
    int i, top;
    uint8_t n, s, e, w;

    for (i = 0; i < NUM_ROOMS; i++)
        seen[i] = 0;

    seen[0] = 1;
    stack[sp++] = 0;

    while (sp > 0) {
        top = stack[sp - 1];
        sp--;

        n = room_north[top];
        s = room_south[top];
        e = room_east[top];
        w = room_west[top];

        if (n != NO_EXIT && !seen[n]) {
            seen[n] = 1;
            stack[sp++] = n;
        }

        if (s != NO_EXIT && !seen[s]) {
            seen[s] = 1;
            stack[sp++] = s;
        }

        if (e != NO_EXIT && !seen[e]) {
            seen[e] = 1;
            stack[sp++] = e;
        }

        if (w != NO_EXIT && !seen[w]) {
            seen[w] = 1;
            stack[sp++] = w;
        }
    }

    for (i = 0; i < NUM_ROOMS; i++) {
        if (!seen[i])
            return 0;
    }

    return 1;
}

// void draw_dungeon_map(void)
// {
    // char map[2*GRID_SIZE+1][2*GRID_SIZE+1];
    // int i, j, room, r, c;

    // for (i=0; i<2*GRID_SIZE+1; i++)
        // for (j=0; j<2*GRID_SIZE+1; j++)
            // map[i][j] = '#';

    // for (room=0; room<NUM_ROOMS; room++) {
        // r = room / GRID_SIZE;
        // c = room % GRID_SIZE;
        // if (room == 0) map[2*r+1][2*c+1] = 'S';
        // else if (room == 99) map[2*r+1][2*c+1] = 'D';
        // else map[2*r+1][2*c+1] = '.';
    // }

    // for (room=0; room<NUM_ROOMS; room++) {
        // r = room / GRID_SIZE;
        // c = room % GRID_SIZE;

        // if (room_north[room] != NO_EXIT)
            // map[2*r][2*c+1] = ' ';
        // if (room_south[room] != NO_EXIT)
            // map[2*r+2][2*c+1] = ' ';
        // if (room_west[room] != NO_EXIT)
            // map[2*r+1][2*c] = ' ';
        // if (room_east[room] != NO_EXIT)
            // map[2*r+1][2*c+2] = ' ';
    // }

    // for (i=0; i<2*GRID_SIZE+1; i++) {
        // for (j=0; j<2*GRID_SIZE+1; j++)
            // putchar(map[i][j]);
        // putchar('\n');
    // }
// }

int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    init_dungeon();

    while (1) {
        start_screen();
        play_game();
    }

    return 0;
}