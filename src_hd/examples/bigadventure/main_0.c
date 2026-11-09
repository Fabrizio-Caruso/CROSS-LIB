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

const char *garden_descs[] = {
    "FLOWERING MEADOW",
    "OLD OAK TREE",
    "GARDEN PATH",
    "BUTTERFLY PATCH",
    "ROSE BUSH",
    "VEGETABLE PLOT",
    "GARDEN GATE",
    "BIRD NEST",
    "WILDFLOWER FIELD",
    "SPRING GARDEN"
};

const char *castle_descs[] = {
    "STONE CORRIDOR",
    "KNIGHTS HALL",
    "CASTLE TOWER",
    "BARRICADED DOOR",
    "ARMORY ROOM",
    "KINGS THRONE",
    "CASTLE WALL",
    "GUARD ROOM",
    "CANDLE HALL",
    "IRON GATE"
};

const char *village_descs[] = {
    "BREAD SHOP",
    "BLACKSMITH FORGE",
    "VILLAGE SQUARE",
    "FARM HOUSE",
    "TANNOY STONE",
    "BUTCHER STALL",
    "VILLAGE PATH",
    "WATER WELL",
    "MARKET PLACE",
    "COTTAGE YARD"
};

const char *riverside_descs[] = {
    "RIVER BANK",
    "FISHING SPOT",
    "WILLOW TREE",
    "RIVER CROSSING",
    "ROCKY SHORE",
    "CANAL BRIDGE",
    "DRY RIVERBED",
    "WATERFALL EDGE",
    "MUD FLAT",
    "RIVER DOCK"
};

const char *region_names[] = {
    "GARDEN",
    "CASTLE",
    "VILLAGE",
    "RIVERSIDE"
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
    "AXE"
};

const char *monster_names[] = {
    "NONE",
    "GOBLIN",
    "SKELETON",
    "OGRE",
    "TROLL",
    "DRAGON"
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

void display_room(void) {
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

    _XL_PRINT(1, 0, "TEXT ADVENTURE");
    _XL_PRINT(1, 1, rname);
    _XL_PRINT(1, 2, desc);

    _XL_PRINT(1, 4, "EXITS:");
    if (rooms[player_room].exits & DIR_N) _XL_PRINT(10, 4, "N");
    if (rooms[player_room].exits & DIR_S) _XL_PRINT(14, 4, "S");
    if (rooms[player_room].exits & DIR_E) _XL_PRINT(18, 4, "E");
    if (rooms[player_room].exits & DIR_W) _XL_PRINT(22, 4, "W");

    if (rooms[player_room].item != ITEM_NONE) {
        _XL_PRINT(1, 5, "ITEM:");
        _XL_PRINT(8, 5, item_names[rooms[player_room].item]);
    }

    if (rooms[player_room].monster != MONSTER_NONE) {
        _XL_PRINT(1, 6, "MONSTER:");
        _XL_PRINT(12, 6, monster_names[rooms[player_room].monster]);
    }

    _XL_PRINT(1, 8, "HP:");
    _XL_PRINTD(4, 8, 3, player_hp);

    _XL_PRINT(1, 9, "WEAPON:");
    _XL_PRINT(10, 9, item_names[player_weapon]);

    if (player_shield > 0) {
        _XL_PRINT(1, 10, "SHIELD:");
        _XL_PRINT(10, 10, item_names[player_shield + 2]);
    }

    _XL_PRINT(1, 11, "POTIONS:");
    _XL_PRINTD(10, 11, 1, player_potions);

    _XL_PRINT(1, 12, "SCORE:");
    _XL_PRINTD(8, 12, 4, player_score);

    _XL_PRINT(1, 14, "PRESS DIRECTION TO MOVE");
    _XL_PRINT(1, 15, "PRESS F TO USE POTION");
}

void do_combat(uint8_t room_idx) {
    uint16_t mhp;
    uint16_t mdmg;
    uint8_t wdm;
    uint8_t sdr;
    uint8_t pdmg;
    uint8_t result;

    mhp = rooms[room_idx].monster_hp;
    mdmg = 0;
    sdr = 0;

    switch (rooms[room_idx].monster) {
        case MONSTER_GOBLIN: mdmg = MONSTER_DMG_GOBLIN; break;
        case MONSTER_SKELETON: mdmg = MONSTER_DMG_SKELETON; break;
        case MONSTER_OGRE: mdmg = MONSTER_DMG_OGRE; break;
        case MONSTER_TROLL: mdmg = MONSTER_DMG_TROLL; break;
        case MONSTER_DRAGON: mdmg = MONSTER_DMG_DRAGON; break;
        default: mdmg = 0; break;
    }

    switch (player_shield) {
        case 1: sdr = SHIELD_RED_LIGHT; break;
        case 2: sdr = SHIELD_RED_MED; break;
        case 3: sdr = SHIELD_RED_HEAVY; break;
        default: sdr = 0; break;
    }

    wdm = 0;
    switch (player_weapon) {
        case 1: wdm = WEAPON_DMG_DAGGER; break;
        case 2: wdm = WEAPON_DMG_IRON; break;
        case 3: wdm = WEAPON_DMG_AXE; break;
        case 4: wdm = WEAPON_DMG_MAGIC; break;
        default: wdm = WEAPON_DMG_DAGGER; break;
    }

    while (mhp > 0 && player_hp > 0) {
        pdmg = wdm;
        if (rooms[room_idx].monster == MONSTER_DRAGON && player_weapon != 4) {
            pdmg = 0;
        }
        if (mhp > pdmg) mhp = mhp - pdmg;
        else mhp = 0;

        result = mdmg - sdr;
        if (result > player_hp) result = player_hp;
        player_hp = player_hp - result;

        _XL_CLEAR_SCREEN();
        _XL_PRINT(1, 0, "COMBAT");
        _XL_PRINT(1, 1, monster_names[rooms[room_idx].monster]);
        _XL_PRINT(1, 2, "MONSTER HP:");
        _XL_PRINTD(12, 2, 3, mhp);
        _XL_PRINT(1, 3, "YOUR HP:");
        _XL_PRINTD(10, 3, 3, player_hp);

        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    if (mhp == 0) {
        if (rooms[room_idx].monster == MONSTER_DRAGON) {
            dragon_defeated = 1;
            player_score = player_score + 100;
            _XL_EXPLOSION_SOUND();
        } else {
            player_score = player_score + 10;
            _XL_PING_SOUND();
        }
        rooms[room_idx].monster = MONSTER_NONE;
        rooms[room_idx].monster_hp = 0;
    }
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
    _XL_PRINT(5, 4, "TEXT ADVENTURE");
    _XL_PRINT(3, 6, "FIND THE MAGIC SWORD");
    _XL_PRINT(3, 7, "KILL THE DRAGON");
    _XL_PRINT(2, 10, "PRESS ANY KEY TO START");
    _XL_WAIT_FOR_INPUT();
}

void show_game_over(void) {
    _XL_CLEAR_SCREEN();
    if (dragon_defeated) {
        _XL_PRINT(3, 5, "YOU DEFEATED THE DRAGON");
        _XL_PRINT(3, 7, "YOU WIN!");
        _XL_ZAP_SOUND();
    } else {
        _XL_PRINT(5, 5, "YOU DIED");
        _XL_PRINT(3, 7, "GAME OVER");
        _XL_EXPLOSION_SOUND();
    }
    _XL_PRINT(2, 10, "FINAL SCORE:");
    _XL_PRINTD(14, 10, 4, player_score);
    _XL_PRINT(2, 13, "PRESS ANY KEY TO RESTART");
    _XL_WAIT_FOR_INPUT();
}

void play_game(void) {
    uint8_t input;
    uint8_t pressed;

    show_title();

    while (!game_over) {
        display_room();
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

        input = _XL_INPUT();

        pressed = _XL_UP(input);
        if (pressed) {
            move_player(0);
            _XL_TICK_SOUND();
        }
        pressed = _XL_DOWN(input);
        if (pressed) {
            move_player(1);
            _XL_TICK_SOUND();
        }
        pressed = _XL_RIGHT(input);
        if (pressed) {
            move_player(2);
            _XL_TICK_SOUND();
        }
        pressed = _XL_LEFT(input);
        if (pressed) {
            move_player(3);
            _XL_TICK_SOUND();
        }
        pressed = _XL_FIRE(input);
        if (pressed) {
            use_potion();
        }

        if (dragon_defeated) {
            game_over = 1;
        }
        if (player_hp <= 0) {
            game_over = 1;
        }
    }

    show_game_over();
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