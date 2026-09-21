#include "cross_lib.h"

#define NUM_ROOMS 100
#define GRID_W 10
#define GRID_H 10
#define DIR_N 1
#define DIR_S 2
#define DIR_E 4
#define DIR_W 8
#define NUM_EXTRA_EDGES 15

#define REGION_GARDEN 0
#define REGION_CASTLE 1
#define REGION_VILLAGE 2
#define REGION_RIVERSIDE 3

#define ITEM_NONE 0
#define ITEM_MAGIC_SWORD 1
#define ITEM_HEAL_POTION 2
#define ITEM_LIGHT_SHIELD 3
#define ITEM_MED_SHIELD 4
#define ITEM_HEAVY_SHIELD 5
#define ITEM_DAGGER 6
#define ITEM_IRON_SWORD 7
#define ITEM_AXE 8
#define ITEM_TORCH 9

#define MONSTER_NONE 0
#define MONSTER_GOBLIN 1
#define MONSTER_SKELETON 2
#define MONSTER_OGRE 3
#define MONSTER_TROLL 4
#define MONSTER_DRAGON 5

#define MONSTER_HP_GOBLIN 20
#define MONSTER_HP_SKELETON 30
#define MONSTER_HP_OGRE 50
#define MONSTER_HP_TROLL 70
#define MONSTER_HP_DRAGON 100

#define MONSTER_DMG_GOBLIN 5
#define MONSTER_DMG_SKELETON 8
#define MONSTER_DMG_OGRE 15
#define MONSTER_DMG_TROLL 20
#define MONSTER_DMG_DRAGON 30

#define WEAPON_DMG_DAGGER 5
#define WEAPON_DMG_IRON 10
#define WEAPON_DMG_AXE 15
#define WEAPON_DMG_MAGIC 25

#define SHIELD_RED_LIGHT 2
#define SHIELD_RED_MED 5
#define SHIELD_RED_HEAVY 10

#define POTION_HEAL 30

#define MAP_RADIUS_INITIAL 3
#define MAP_RADIUS_EXTENDED 5

typedef struct {
    uint8_t region;
    uint8_t exits;
    uint8_t dest_n;
    uint8_t dest_s;
    uint8_t dest_e;
    uint8_t dest_w;
    uint8_t item;
    uint8_t monster;
    uint16_t monster_hp;
} Room;

Room rooms[NUM_ROOMS];
uint8_t player_room;
uint16_t player_hp;
uint16_t player_max_hp;
uint8_t player_weapon;
uint8_t player_shield;
uint8_t player_potions;
uint16_t player_score;
uint8_t dragon_defeated;
uint8_t game_over;
uint8_t map_radius;

const char *garden_descs[] = {
    "A LUSH MEADOW RICH WITH WILDFLOWERS AND HUMMING BEEES",
    "AN ANCIENT OAK TREE WITH TWISTED ROOTS AND DEEP SHADOW",
    "A WINDING GARDEN PATH LINED WITH ROSES AND MOSSY STONES",
    "A SUNLIT PATCH WHERE BUTTERFLIES DRIFT AMONG THE CLOVERS",
    "A DENSE ROSE BUSH FULL OF THORNS AND FRAGRANT BLOOMS",
    "A TILLED VEGETABLE PLOT WITH RICH DARK SOIL AND SPROUTS",
    "A WORN GARDEN GATE LEADING TO A QUIET HIDDEN GROVE",
    "A WOVEN BIRD NEST PERCHED HIGH IN A WILLOW CANOPY",
    "A BROAD FIELD OF WILDFLOWERS SWAYING IN THE BREEZE",
    "A BRIGHT SPRING GARDEN BURSTING WITH NEW GREEN GROWTH"
};

const char *castle_descs[] = {
    "A COLD STONE CORRIDOR WITH MOIST WALLS AND FAINT ECHOES",
    "A GRAND HALL WHERE KNIGHTS ONCE DRANK AND SWORE OATHS",
    "A TALL TOWER ROOM WITH A NARROW WINDOW AND COLD AIR",
    "A BARRICADED DOOR BEHIND WHICH LIES A DARK PASSAGE",
    "A CLANKING ARMORY ROOM FILLED WITH RUSTED IRON BLADES",
    "A HIGH THRONE HALL WHERE A KINGCE SAT IN GOLD AND SHINE",
    "A THICK CASTLE WALL OF GRAY STONE AND OLD MORTAR",
    "A DIM GUARD ROOM WITH A BURNED OUT HEARTH AND CHAINS",
    "A CANDLELIT HALL WHERE WAX DROPS ON TOILE AND DUST",
    "A HEAVY IRON GATE SET IN A WALL OF CARVED STONE"
};

const char *village_descs[] = {
    "A SMOKEY BREAD SHOP WITH WARM LOAVES ON A WOODEN SHELF",
    "A HOT BLACKSMITH FORGE WHERE HAMMERING ECHOES ALL DAY",
    "A STONY VILLAGE SQUARE WHERE CARRIERS AND CHILDREN MIX",
    "A TILTED FARM HOUSE WITH A LEAKY ROOF AND A DOG",
    "A WORN TANNERY STONE WHERE LEATHER IS SOWN AND DRIED",
    "A BUTCHERS STALL WITH BLOODED KNIVES AND COLD MEAT",
    "A DUSTY VILLAGE PATH WHERE MULES AND CARRIERS PASS",
    "A COLD WATER WELL WHERE VILLAGERS DRAW AT DAWN",
    "A Lively MARKET PLACE WHERE MERCHANTS SHOUT AND HAGGLE",
    "A QUIET COTTAGE YARD WITH A CAT AND A SLEEPING GOAT"
};

const char *riverside_descs[] = {
    "A MUDY RIVER BANK WHERE FISH JUMP AND REEDS SWAY",
    "A QUIET FISHING SPOT WITH A LINE CAST INTO THE FLOW",
    "A BENDING WILLOW TREE DRIPPING INTO THE COOL WATER",
    "A ROCKY RIVER CROSSING WHERE STONES ARE SLIPPERY AND WET",
    "A GRITTY ROCKY SHORE WHERE TIDES LICK AND PULL",
    "A NARROW CANAL BRIDGE WHERE WATER SLOSHES BELOW",
    "A DRY RIVERBED CRACKED AND BAKED UNDER THE SUN",
    "A ROARING WATERFALL EDGE WHERE MIST AND LIGHT MIX",
    "A STICKY MUD FLAT WHERE HERONS STAND AND WAIT",
    "A WOODEN RIVER DOCK WHERE BOATS CREAK AND SWAY"
};

const char *region_names[] = {
    "THE GARDEN",
    "THE CASTLE",
    "THE VILLAGE",
    "THE RIVERSIDE"
};

const char *item_names[] = {
    "NONE",
    "MAGIC SWORD",
    "HEAL POTION",
    "LIGHT SHIELD",
    "MED SHIELD",
    "HEAVY SHIELD",
    "DAGGER",
    "IRON SWORD",
    "AXE",
    "TORCH"
};

const char *monster_names[] = {
    "NONE",
    "GOBLIN",
    "SKELETON",
    "OGRE",
    "TROLL",
    "DRAGON"
};

const char *monster_descs[] = {
    "",
    "A SMALL GREEN GOBLIN WITH SHARP TEETH AND RED EYES",
    "A RATTLY SKELETON WITH A BOW AND HOLLOW SOCKETS",
    "A MASSIVE OGRE WITH BRUISED KNuckles AND A GRIN",
    "A TALL TROLL WITH GREY SKIN AND A CLUB OF STONE",
    "AN ANCIENT DRAGON WITH SCALES OF FIRE AND SMOKE"
};

uint8_t get_region(uint8_t room_idx) {
    uint8_t row = room_idx / GRID_W;
    uint8_t col = room_idx % GRID_W;
    if (row < 5 && col < 5) return REGION_GARDEN;
    if (row < 5 && col >= 5) return REGION_VILLAGE;
    if (row >= 5 && col < 5) return REGION_RIVERSIDE;
    return REGION_CASTLE;
}

void generate_spanning_tree(void) {
    uint8_t in_tree[NUM_ROOMS];
    uint8_t frontier[NUM_ROOMS];
    uint8_t frontier_count;
    uint8_t i;
    uint8_t idx;
    uint8_t current;
    uint8_t row;
    uint8_t col;

    for (i = 0; i < NUM_ROOMS; i++) {
        in_tree[i] = 0;
        rooms[i].exits = 0;
        rooms[i].dest_n = 255;
        rooms[i].dest_s = 255;
        rooms[i].dest_e = 255;
        rooms[i].dest_w = 255;
    }

    in_tree[0] = 1;
    frontier_count = 0;
    frontier[frontier_count++] = 0;

    while (frontier_count > 0) {
        idx = _XL_RAND() % frontier_count;
        current = frontier[idx];
        frontier[idx] = frontier[frontier_count - 1];
        frontier_count--;

        row = current / GRID_W;
        col = current % GRID_W;

        if (row > 0 && !in_tree[current - GRID_W]) {
            in_tree[current - GRID_W] = 1;
            rooms[current].dest_n = current - GRID_W;
            rooms[current].exits |= DIR_N;
            rooms[current - GRID_W].dest_s = current;
            rooms[current - GRID_W].exits |= DIR_S;
            frontier[frontier_count++] = current - GRID_W;
        }
        if (row < GRID_H - 1 && !in_tree[current + GRID_W]) {
            in_tree[current + GRID_W] = 1;
            rooms[current].dest_s = current + GRID_W;
            rooms[current].exits |= DIR_S;
            rooms[current + GRID_W].dest_n = current;
            rooms[current + GRID_W].exits |= DIR_N;
            frontier[frontier_count++] = current + GRID_W;
        }
        if (col < GRID_W - 1 && !in_tree[current + 1]) {
            in_tree[current + 1] = 1;
            rooms[current].dest_e = current + 1;
            rooms[current].exits |= DIR_E;
            rooms[current + 1].dest_w = current;
            rooms[current + 1].exits |= DIR_W;
            frontier[frontier_count++] = current + 1;
        }
        if (col > 0 && !in_tree[current - 1]) {
            in_tree[current - 1] = 1;
            rooms[current].dest_w = current - 1;
            rooms[current].exits |= DIR_W;
            rooms[current - 1].dest_e = current;
            rooms[current - 1].exits |= DIR_E;
            frontier[frontier_count++] = current - 1;
        }
    }
}

void add_extra_edges(void) {
    uint8_t i;
    uint8_t a;
    uint8_t b;
    uint8_t row;
    uint8_t col;
    uint8_t adj;

    for (i = 0; i < NUM_EXTRA_EDGES; i++) {
        a = _XL_RAND() % NUM_ROOMS;
        b = _XL_RAND() % NUM_ROOMS;
        row = a / GRID_W;
        col = a % GRID_W;

        if (row > 0) {
            b = a - GRID_W;
            adj = DIR_N;
        } else if (row < GRID_H - 1) {
            b = a + GRID_W;
            adj = DIR_S;
        } else if (col < GRID_W - 1) {
            b = a + 1;
            adj = DIR_E;
        } else if (col > 0) {
            b = a - 1;
            adj = DIR_W;
        } else {
            continue;
        }

        if (adj == DIR_N && rooms[a].dest_n == 255) {
            rooms[a].dest_n = b;
            rooms[a].exits |= DIR_N;
            rooms[b].dest_s = a;
            rooms[b].exits |= DIR_S;
        } else if (adj == DIR_S && rooms[a].dest_s == 255) {
            rooms[a].dest_s = b;
            rooms[a].exits |= DIR_S;
            rooms[b].dest_n = a;
            rooms[b].exits |= DIR_N;
        } else if (adj == DIR_E && rooms[a].dest_e == 255) {
            rooms[a].dest_e = b;
            rooms[a].exits |= DIR_E;
            rooms[b].dest_w = a;
            rooms[b].exits |= DIR_W;
        } else if (adj == DIR_W && rooms[a].dest_w == 255) {
            rooms[a].dest_w = b;
            rooms[a].exits |= DIR_W;
            rooms[b].dest_e = a;
            rooms[b].exits |= DIR_E;
        }
    }
}

uint16_t bfs_farthest(void) {
    uint16_t dist[NUM_ROOMS];
    uint8_t visited[NUM_ROOMS];
    uint16_t queue[NUM_ROOMS];
    uint16_t q_head;
    uint16_t q_tail;
    uint16_t i;
    uint16_t current;
    uint16_t max_dist;
    uint16_t farthest_room;

    for (i = 0; i < NUM_ROOMS; i++) {
        dist[i] = 0;
        visited[i] = 0;
    }

    visited[0] = 1;
    q_head = 0;
    q_tail = 0;
    queue[q_tail++] = 0;

    while (q_head < q_tail) {
        current = queue[q_head++];
        if (rooms[current].exits & DIR_N && !visited[rooms[current].dest_n]) {
            visited[rooms[current].dest_n] = 1;
            dist[rooms[current].dest_n] = dist[current] + 1;
            queue[q_tail++] = rooms[current].dest_n;
        }
        if (rooms[current].exits & DIR_S && !visited[rooms[current].dest_s]) {
            visited[rooms[current].dest_s] = 1;
            dist[rooms[current].dest_s] = dist[current] + 1;
            queue[q_tail++] = rooms[current].dest_s;
        }
        if (rooms[current].exits & DIR_E && !visited[rooms[current].dest_e]) {
            visited[rooms[current].dest_e] = 1;
            dist[rooms[current].dest_e] = dist[current] + 1;
            queue[q_tail++] = rooms[current].dest_e;
        }
        if (rooms[current].exits & DIR_W && !visited[rooms[current].dest_w]) {
            visited[rooms[current].dest_w] = 1;
            dist[rooms[current].dest_w] = dist[current] + 1;
            queue[q_tail++] = rooms[current].dest_w;
        }
    }

    max_dist = 0;
    farthest_room = 0;
    for (i = 1; i < NUM_ROOMS; i++) {
        if (dist[i] > max_dist) {
            max_dist = dist[i];
            farthest_room = i;
        }
    }

    return farthest_room;
}

void place_items_and_monsters(void) {
    uint16_t i;
    uint16_t farthest;
    uint8_t r;
    uint8_t region;

    farthest = bfs_farthest();

    rooms[farthest].monster = MONSTER_DRAGON;
    rooms[farthest].monster_hp = MONSTER_HP_DRAGON;

    rooms[0].item = ITEM_DAGGER;

    r = _XL_RAND() % NUM_ROOMS;
    if (r == 0) r = 1;
    region = rooms[r].region;
    if (region != REGION_CASTLE) {
        r = 50 + _XL_RAND() % 50;
    }
    if (r == farthest) r = 51 + _XL_RAND() % 49;
    rooms[r].item = ITEM_MAGIC_SWORD;

    i = 1;
    while (i < 5) {
        r = _XL_RAND() % NUM_ROOMS;
        if (r != 0 && rooms[r].item == ITEM_NONE && r != farthest) {
            rooms[r].item = ITEM_HEAL_POTION;
            i++;
        }
    }

    i = 1;
    while (i < 3) {
        r = _XL_RAND() % NUM_ROOMS;
        if (r != 0 && rooms[r].item == ITEM_NONE && r != farthest) {
            rooms[r].item = ITEM_LIGHT_SHIELD;
            i++;
        }
    }

    i = 1;
    while (i < 3) {
        r = _XL_RAND() % NUM_ROOMS;
        if (r != 0 && rooms[r].item == ITEM_NONE && r != farthest) {
            rooms[r].item = ITEM_MED_SHIELD;
            i++;
        }
    }

    i = 1;
    while (i < 2) {
        r = _XL_RAND() % NUM_ROOMS;
        if (r != 0 && rooms[r].item == ITEM_NONE && r != farthest) {
            rooms[r].item = ITEM_HEAVY_SHIELD;
            i++;
        }
    }

    i = 1;
    while (i < 4) {
        r = _XL_RAND() % NUM_ROOMS;
        if (r != 0 && rooms[r].item == ITEM_NONE && r != farthest) {
            rooms[r].item = ITEM_IRON_SWORD;
            i++;
        }
    }

    i = 1;
    while (i < 3) {
        r = _XL_RAND() % NUM_ROOMS;
        if (r != 0 && rooms[r].item == ITEM_NONE && r != farthest) {
            rooms[r].item = ITEM_AXE;
            i++;
        }
    }

    /* Place torch far from the start (bottom-right quadrant, castle region) */
    r = 75 + _XL_RAND() % 25;
    if (r == farthest) r = 75 + (_XL_RAND() % 24) + 1;
    if (rooms[r].item != ITEM_NONE) {
        r = 76 + _XL_RAND() % 24;
        if (r == farthest) r = 77;
    }
    rooms[r].item = ITEM_TORCH;

    i = 1;
    while (i < 8) {
        r = 1 + _XL_RAND() % 25;
        if (rooms[r].monster == MONSTER_NONE && r != farthest) {
            rooms[r].monster = MONSTER_GOBLIN;
            rooms[r].monster_hp = MONSTER_HP_GOBLIN;
            i++;
        }
    }

    i = 1;
    while (i < 6) {
        r = 26 + _XL_RAND() % 25;
        if (rooms[r].monster == MONSTER_NONE && r != farthest) {
            rooms[r].monster = MONSTER_SKELETON;
            rooms[r].monster_hp = MONSTER_HP_SKELETON;
            i++;
        }
    }

    i = 1;
    while (i < 4) {
        r = 51 + _XL_RAND() % 49;
        if (rooms[r].monster == MONSTER_NONE && r != farthest) {
            rooms[r].monster = MONSTER_OGRE;
            rooms[r].monster_hp = MONSTER_HP_OGRE;
            i++;
        }
    }

    i = 1;
    while (i < 3) {
        r = 51 + _XL_RAND() % 49;
        if (rooms[r].monster == MONSTER_NONE && r != farthest) {
            rooms[r].monster = MONSTER_TROLL;
            rooms[r].monster_hp = MONSTER_HP_TROLL;
            i++;
        }
    }
}

void init_game(void) {
    uint16_t i;

    player_room = 0;
    player_hp = 100;
    player_max_hp = 100;
    player_weapon = ITEM_DAGGER;
    player_shield = 0;
    player_potions = 0;
    player_score = 0;
    dragon_defeated = 0;
    game_over = 0;
    map_radius = MAP_RADIUS_INITIAL;

    for (i = 0; i < NUM_ROOMS; i++) {
        rooms[i].region = get_region(i);
        rooms[i].item = ITEM_NONE;
        rooms[i].monster = MONSTER_NONE;
        rooms[i].monster_hp = 0;
    }

    generate_spanning_tree();
    add_extra_edges();
    place_items_and_monsters();
}

// void display_map(void) {
    // uint8_t pr, pc;
    // uint8_t r, c;
    // uint8_t dr, dc;
    // uint16_t dr2, dc2;
    // uint8_t tile;

    // pr = player_room / GRID_W;
    // pc = player_room % GRID_W;

    // for (r = 0; r < GRID_H; r++) {
        // for (c = 0; c < GRID_W; c++) {
            // if (r >= pr) dr = r - pr; else dr = pr - r;
            // if (c >= pc) dc = c - pc; else dc = pc - c;

            // dr2 = dr * dr;
            // dc2 = dc * dc;

            // if (dr2 + dc2 <= map_radius * map_radius) {
                // if (r == pr && c == pc) {
                    // /* Player position */
                    // tile = _TILE_0;
                // } else if (r == pr + 1 && c == pc) {
                    // /* Directly south: wall if no exit south */
                    // tile = (rooms[player_room].exits & DIR_S) ? _TILE_1 : _TILE_2;
                // } else if (r == pr - 1 && c == pc) {
                    // /* Directly north: wall if no exit north */
                    // tile = (rooms[player_room].exits & DIR_N) ? _TILE_1 : _TILE_2;
                // } else if (r == pr && c == pc + 1) {
                    // /* Directly east: wall if no exit east */
                    // tile = (rooms[player_room].exits & DIR_E) ? _TILE_1 : _TILE_2;
                // } else if (r == pr && c == pc - 1) {
                    // /* Directly west: wall if no exit west */
                    // tile = (rooms[player_room].exits & DIR_W) ? _TILE_1 : _TILE_2;
                // } else {
                    // /* Other visible tiles: possible room */
                    // tile = _TILE_1;
                // }
            // } else {
                // /* Outside radius: wall */
                // tile = _TILE_2;
            // }

            // _XL_DRAW(c, r, tile, _XL_WHITE);
        // }
    // }
// }

void display_room_info(void) {
    const char *desc;
    const char *rname;
    uint8_t region;
    uint8_t desc_idx;

    _XL_CLEAR_SCREEN();

    region = rooms[player_room].region;
    rname = region_names[region];
    desc_idx = player_room % 10;

    switch (region) {
        case REGION_GARDEN:
            desc = garden_descs[desc_idx];
            break;
        case REGION_CASTLE:
            desc = castle_descs[desc_idx];
            break;
        case REGION_VILLAGE:
            desc = village_descs[desc_idx];
            break;
        case REGION_RIVERSIDE:
            desc = riverside_descs[desc_idx];
            break;
        default:
            desc = garden_descs[0];
            break;
    }

    _XL_SET_TEXT_COLOR(_XL_MAGENTA);
    _XL_PRINT(1, 0, "TEXT ADVENTURE");

    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(1, 1, rname);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(1, 2, desc);

    _XL_SET_TEXT_COLOR(_XL_BLUE);
    _XL_PRINT(1, 4, "EXITS:");
    if (rooms[player_room].exits & DIR_N) _XL_PRINT(10, 4, "N");
    if (rooms[player_room].exits & DIR_S) _XL_PRINT(14, 4, "S");
    if (rooms[player_room].exits & DIR_E) _XL_PRINT(18, 4, "E");
    if (rooms[player_room].exits & DIR_W) _XL_PRINT(22, 4, "W");

    if (rooms[player_room].item != ITEM_NONE) {
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(1, 5, "ITEM:");
        _XL_PRINT(8, 5, item_names[rooms[player_room].item]);
    }

    if (rooms[player_room].monster != MONSTER_NONE) {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(1, 6, "MONSTER:");
        _XL_PRINT(12, 6, monster_names[rooms[player_room].monster]);
    }

    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(1, 8, "HP:");
    _XL_PRINTD(4, 8, 3, player_hp);

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(1, 9, "WEAPON:");
    _XL_PRINT(10, 9, item_names[player_weapon]);

    if (player_shield > 0) {
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(1, 10, "SHIELD:");
        _XL_PRINT(10, 10, item_names[player_shield + 2]);
    }

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(1, 11, "POTIONS:");
    _XL_PRINTD(10, 11, 1, player_potions);

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(1, 12, "SCORE:");
    _XL_PRINTD(8, 12, 4, player_score);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(1, 14, "PRESS DIRECTION TO MOVE");
    _XL_PRINT(1, 15, "PRESS F TO USE POTION");

    // display_map();
}

void display_combat_screen(uint8_t room_idx) {
    _XL_CLEAR_SCREEN();

    _XL_SET_TEXT_COLOR(_XL_MAGENTA);
    _XL_PRINT(1, 0, "COMBAT");

    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(1, 1, monster_names[rooms[room_idx].monster]);
    _XL_PRINT(1, 2, monster_descs[rooms[room_idx].monster]);

    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(1, 4, "ENEMY HP:");
    _XL_PRINTD(11, 4, 3, rooms[room_idx].monster_hp);

    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(1, 5, "YOUR HP:");
    _XL_PRINTD(10, 5, 3, player_hp);

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(1, 7, "WEAPON:");
    _XL_PRINT(10, 7, item_names[player_weapon]);

    if (player_shield > 0) {
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(1, 8, "SHIELD:");
        _XL_PRINT(10, 8, item_names[player_shield + 2]);
    }

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(1, 10, "PRESS F TO ATTACK");
    _XL_PRINT(1, 11, "PRESS DIRECTION TO FLEE");

    if (rooms[player_room].exits & DIR_N) {
        _XL_SET_TEXT_COLOR(_XL_BLUE);
        _XL_PRINT(1, 13, "FLEE NORTH:");
    }
    if (rooms[player_room].exits & DIR_S) {
        _XL_SET_TEXT_COLOR(_XL_BLUE);
        _XL_PRINT(1, 14, "FLEE SOUTH:");
    }
    if (rooms[player_room].exits & DIR_E) {
        _XL_SET_TEXT_COLOR(_XL_BLUE);
        _XL_PRINT(1, 15, "FLEE EAST:");
    }
    if (rooms[player_room].exits & DIR_W) {
        _XL_SET_TEXT_COLOR(_XL_BLUE);
        _XL_PRINT(1, 16, "FLEE WEST:");
    }
}

void do_combat(uint8_t room_idx) {
    uint8_t input;
    uint8_t pressed;
    uint8_t dir;
    uint8_t wdm;
    uint8_t sdr;
    uint8_t pdmg;
    uint8_t result;
    uint8_t fled;
	uint8_t any_press;

    wdm = 0;
    switch (player_weapon) {
        case 1: wdm = WEAPON_DMG_DAGGER; break;
        case 2: wdm = WEAPON_DMG_IRON; break;
        case 3: wdm = WEAPON_DMG_AXE; break;
        case 4: wdm = WEAPON_DMG_MAGIC; break;
        default: wdm = WEAPON_DMG_DAGGER; break;
    }

    sdr = 0;
    switch (player_shield) {
        case 1: sdr = SHIELD_RED_LIGHT; break;
        case 2: sdr = SHIELD_RED_MED; break;
        case 3: sdr = SHIELD_RED_HEAVY; break;
        default: sdr = 0; break;
    }

    fled = 0;

    while (rooms[room_idx].monster != MONSTER_NONE && player_hp > 0 && !fled) 
	{
        display_combat_screen(room_idx);
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

		any_press = 0;
		while(!any_press)
		{
			input = _XL_INPUT();

			pressed = _XL_FIRE(input);
			if (pressed) {
				pdmg = wdm;
				if (rooms[room_idx].monster == MONSTER_DRAGON && player_weapon != 4) {
					pdmg = 0;
				}
				if (rooms[room_idx].monster_hp > pdmg) {
					rooms[room_idx].monster_hp = rooms[room_idx].monster_hp - pdmg;
				} else {
					rooms[room_idx].monster_hp = 0;
					rooms[room_idx].monster = MONSTER_NONE;
				}

				result = 0;
				switch (rooms[room_idx].monster) {
					case MONSTER_GOBLIN: result = MONSTER_DMG_GOBLIN; break;
					case MONSTER_SKELETON: result = MONSTER_DMG_SKELETON; break;
					case MONSTER_OGRE: result = MONSTER_DMG_OGRE; break;
					case MONSTER_TROLL: result = MONSTER_DMG_TROLL; break;
					case MONSTER_DRAGON: result = MONSTER_DMG_DRAGON; break;
					default: result = 0; break;
				}
				if (result > sdr) result = result - sdr;
				else result = 0;

				if (result > player_hp) result = player_hp;
				player_hp = player_hp - result;

				if (pdmg > 0) _XL_SHOOT_SOUND();
				_XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
				any_press = 1;
			}

			pressed = _XL_UP(input);
			if (pressed && (rooms[player_room].exits & DIR_N)) {
				player_room = rooms[player_room].dest_n;
				fled = 1;
				_XL_TICK_SOUND();
				any_press = 1;
				break;
			}
			pressed = _XL_DOWN(input);
			if (pressed && (rooms[player_room].exits & DIR_S)) {
				player_room = rooms[player_room].dest_s;
				fled = 1;
				_XL_TICK_SOUND();
				any_press = 1;
				break;
			}
			pressed = _XL_RIGHT(input);
			if (pressed && (rooms[player_room].exits & DIR_E)) {
				player_room = rooms[player_room].dest_e;
				fled = 1;
				_XL_TICK_SOUND();
				break;
			}
			pressed = _XL_LEFT(input);
			if (pressed && (rooms[player_room].exits & DIR_W)) {
				player_room = rooms[player_room].dest_w;
				fled = 1;
				_XL_TICK_SOUND();
				any_press = 1;
				break;
			}

			if (player_hp <= 0) {
				_XL_SLEEP(1);
				game_over = 1;
				break;
			}
		}
    }

    if (rooms[room_idx].monster == MONSTER_NONE) {
        if (room_idx == player_room) {
            if (rooms[room_idx].monster == MONSTER_NONE) {
                if (dragon_defeated) {
                    player_score = player_score + 100;
                    _XL_EXPLOSION_SOUND();
                } else {
                    player_score = player_score + 10;
                    _XL_PING_SOUND();
                }
            }
        }
    }

    (void)dir;
}

void collect_item(uint8_t room_idx) {
    uint8_t item;

    item = rooms[room_idx].item;

    switch (item) {
        case ITEM_MAGIC_SWORD:
            player_weapon = 4;
            _XL_SHOOT_SOUND();
            break;
        case ITEM_HEAL_POTION:
            player_potions = player_potions + 1;
            _XL_TICK_SOUND();
            break;
        case ITEM_LIGHT_SHIELD:
            player_shield = 1;
            _XL_TICK_SOUND();
            break;
        case ITEM_MED_SHIELD:
            player_shield = 2;
            _XL_TICK_SOUND();
            break;
        case ITEM_HEAVY_SHIELD:
            player_shield = 3;
            _XL_TICK_SOUND();
            break;
        case ITEM_DAGGER:
            if (player_weapon < 1) player_weapon = 1;
            _XL_TICK_SOUND();
            break;
        case ITEM_IRON_SWORD:
            if (player_weapon < 2) player_weapon = 2;
            _XL_TICK_SOUND();
            break;
        case ITEM_AXE:
            if (player_weapon < 3) player_weapon = 3;
            _XL_TICK_SOUND();
            break;
        case ITEM_TORCH:
            map_radius = MAP_RADIUS_EXTENDED;
            _XL_TOCK_SOUND();
            break;
        default:
            break;
    }

    rooms[room_idx].item = ITEM_NONE;
}

uint8_t use_potion(void) {
    if (player_potions > 0) {
        player_potions = player_potions - 1;
        player_hp = player_hp + POTION_HEAL;
        if (player_hp > player_max_hp) player_hp = player_max_hp;
        _XL_TOCK_SOUND();
        return 1;
    }
    return 0;
}

void move_player(uint8_t dir) {
    uint8_t dest;

    dest = 255;
    if (dir == 0 && (rooms[player_room].exits & DIR_N)) {
        dest = rooms[player_room].dest_n;
    } else if (dir == 1 && (rooms[player_room].exits & DIR_S)) {
        dest = rooms[player_room].dest_s;
    } else if (dir == 2 && (rooms[player_room].exits & DIR_E)) {
        dest = rooms[player_room].dest_e;
    } else if (dir == 3 && (rooms[player_room].exits & DIR_W)) {
        dest = rooms[player_room].dest_w;
    }

    if (dest != 255) {
        player_room = dest;
        if (rooms[player_room].item != ITEM_NONE) {
            collect_item(player_room);
        }
        if (rooms[player_room].monster != MONSTER_NONE) {
            do_combat(player_room);
        }
    }
}

void show_title(void) {
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_MAGENTA);
    _XL_PRINT(5, 3, "TEXT ADVENTURE");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(3, 5, "FIND THE MAGIC SWORD IN THE CASTLE");
    _XL_PRINT(3, 6, "TRAVEL FAR AND KILL THE DRAGON");
    _XL_PRINT(3, 7, "COLLECT WEAPONS AND SHIELDS ALONG THE WAY");
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(3, 9, "ITEMS ARE CYAN AND ENEMIES ARE RED");
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(2, 11, "PRESS ANY KEY TO BEGIN");
    _XL_WAIT_FOR_INPUT();
}

void show_game_over(void) {
    _XL_CLEAR_SCREEN();
    if (dragon_defeated) {
        _XL_SET_TEXT_COLOR(_XL_GREEN);
        _XL_PRINT(2, 4, "YOU DEFEATED THE ANCIENT DRAGON");
        _XL_PRINT(2, 5, "THE REALM IS SAFE ONCE MORE");
        _XL_PRINT(2, 7, "YOU WIN!");
        _XL_ZAP_SOUND();
    } else {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(4, 4, "YOUR BLOOD SOAKS THE COLD GROUND");
        _XL_PRINT(4, 5, "YOUR QUEST ENDS IN SHADOW AND DUST");
        _XL_PRINT(3, 7, "GAME OVER");
        _XL_EXPLOSION_SOUND();
    }
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(2, 10, "FINAL SCORE:");
    _XL_PRINTD(14, 10, 4, player_score);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 13, "PRESS ANY KEY TO RESTART");
    _XL_WAIT_FOR_INPUT();
}

void play_game(void) {
    uint8_t input;
    uint8_t pressed;
	uint8_t any_press;

    show_title();

    while (!game_over) {
        display_room_info();
        // _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
		any_press = 0;
		
		while(!any_press)
		{
			
			input = _XL_INPUT();

			pressed = _XL_UP(input);
			if (pressed) {
				move_player(0);
				any_press = 1;
				if (!game_over) _XL_TICK_SOUND();
			}
			pressed = _XL_DOWN(input);
			if (pressed && !game_over) {
				move_player(1);
				any_press = 1;
				if (!game_over) _XL_TICK_SOUND();
			}
			pressed = _XL_RIGHT(input);
			if (pressed && !game_over) {
				move_player(2);
				any_press = 1;
				if (!game_over) _XL_TICK_SOUND();
			}
			pressed = _XL_LEFT(input);
			if (pressed && !game_over) {
				move_player(3);
				any_press = 1;
				if (!game_over) _XL_TICK_SOUND();
			}
			pressed = _XL_FIRE(input);
			if (pressed && !game_over) {
				use_potion();
				any_press = 1;
			}

			if (dragon_defeated) {
				game_over = 1;
			}
			if (player_hp <= 0) {
				game_over = 1;
			}
		}
    }
}

int main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    for (;;) {
        init_game();
        play_game();
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}