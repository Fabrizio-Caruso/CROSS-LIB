#include "cross_lib.h"

#define GRID_W 10
#define GRID_H 10
#define ROOMS (GRID_W * GRID_H)
#define MAX_EDGES 180

#define ENEMY_NONE   0
#define ENEMY_SLIME  1
#define ENEMY_GOBLIN 2
#define ENEMY_ORC    3
#define ENEMY_DRAGON 4

#define ITEM_NONE   0
#define ITEM_POTION 1
#define ITEM_WEAPON 2
#define ITEM_SHIELD 3

/* Direction bits */
#define DIR_N 1
#define DIR_S 2
#define DIR_W 4
#define DIR_E 8

/* Globals */
uint8_t room_conn[ROOMS];
uint8_t room_enemy[ROOMS];
uint8_t room_enemy_hp[ROOMS];
uint8_t room_item[ROOMS];

uint8_t uf_parent[ROOMS];
uint8_t uf_rank[ROOMS];

uint8_t edge_u[MAX_EDGES];
uint8_t edge_v[MAX_EDGES];
uint8_t edge_count;

uint8_t player_x, player_y;
uint8_t player_energy, player_max_energy;
uint8_t player_attack, player_defense;
uint8_t player_has_key;
uint16_t player_score;
uint8_t game_over;
uint8_t game_won;
const char *msg;

/* Room descriptions indexed by y * GRID_W + x */
const char *room_desc[ROOMS] = {
    /* y=0 */
    /* (0,0) */ "GARDEN GATE AND PATH BEGINS",
    /* (1,0) */ "GRAVEL BY THE ROSE BED",
    /* (2,0) */ "ROSE ARCH AND THORNS CATCH LIGHT",
    /* (3,0) */ "OLD OAK AND ROOTS CRACK STONE",
    /* (4,0) */ "COURTYARD AND PIGEONS IN RAFTERS",
    /* (5,0) */ "ARMORY AND SWORDS IN RACKS",
    /* (6,0) */ "GREAT HALL AND BANNERS HEAVY",
    /* (7,0) */ "BANQUET HALL AND GOBLETS EMPTY",
    /* (8,0) */ "RAMPART WALK AND WIND AND STONE",
    /* (9,0) */ "DEEP WELL AND WATER BLACK",
    /* y=1 */
    /* (0,1) */ "SUNLIT HERB GARDEN",
    /* (1,1) */ "TRICKLING FOUNTAIN AND WATER COOL",
    /* (2,1) */ "STONE BENCH UNDER WILLOW",
    /* (3,1) */ "SERVANT PASSAGE AND TORCHES FLICKER",
    /* (4,1) */ "THRONE ROOM AND GOLD PEEING",
    /* (5,1) */ "SOLAR AND SUN THROUGH GLASS",
    /* (6,1) */ "SCRIBE'S DEN AND INK-STAINED",
    /* (7,1) */ "WATCHTOWER AND EYES IN DARK",
    /* (8,1) */ "TORTURE FLOOR AND IRON AND RUST",
    /* (9,1) */ "BLOOD DRAIN AND RED STREAKS DOWN",
    /* y=2 */
    /* (0,2) */ "CLOVER FIELD AND DEW ON BLADES",
    /* (1,2) */ "WISTERIA TRELLIS AND PURPLE BLOOM",
    /* (2,2) */ "TAPESTRY HALL AND SILK FADED",
    /* (3,2) */ "LIBRARY AND SPINES CRACKED",
    /* (4,2) */ "GUARD ROOM AND CHAINMAIL ON WALL",
    /* (5,2) */ "VAULT DOOR AND IRON AND RIVETS",
    /* (6,2) */ "POWDER ROOM AND OLD SULFUR SMELL",
    /* (7,2) */ "BONE HEAP AND RIBS STACKED",
    /* (8,2) */ "IRON GATE AND COLD TO TOUCH",
    /* (9,2) */ "SPINE ROAD AND VERTEBRAE AS STEPS",
    /* y=3 */
    /* (0,3) */ "GARDEN WALL AND IVY THICK",
    /* (1,3) */ "KITCHEN HEARTH AND BREAD WARM",
    /* (2,3) */ "CHAPEL AND CANDLES GUTTER LOW",
    /* (3,3) */ "MUSIC ROOM AND HARP RUSTED",
    /* (4,3) */ "PRAYER NICHE AND CARVINGS WORN",
    /* (5,3) */ "SECRET PASSAGE AND BRICK AND MORTAR",
    /* (6,3) */ "TUNNEL AND DRIPPING AND NARROW",
    /* (7,3) */ "SCREAMING WALL AND VOICES TRAPPED",
    /* (8,3) */ "DOOM STONE AND OLD TONGUE MARKS",
    /* (9,3) */ "THROAT AND SWALLOWING THE LIGHT",
    /* y=4 */
    /* (0,4) */ "STAIR LANDING AND RAIL COLD",
    /* (1,4) */ "DINING HALL AND CHAIRS PUSHED BACK",
    /* (2,4) */ "BEDCHAMBER AND SHEETS STILL CRUMPLED",
    /* (3,4) */ "DUNGEON STAIR AND STEPS THIN",
    /* (4,4) */ "CROSSROADS AND FOUR DARK CORRIDORS",
    /* (5,4) */ "CHASM AND NOTHING BELOW",
    /* (6,4) */ "LAVA CRACK AND HEAT LICKS UP",
    /* (7,4) */ "FIST OF ROCK AND KNUCKLES BARED",
    /* (8,4) */ "GORGE AND WIND FROM BELOW",
    /* (9,4) */ "MAW AND TEETH OF STONE",
    /* y=5 */
    /* (0,5) */ "TOWER STAIR AND WIND HOWLS",
    /* (1,5) */ "CLOISTER AND FOOTSTEPS ECHO",
    /* (2,5) */ "CELL BLOCK AND BARS VERDIGRISSED",
    /* (3,5) */ "ANTECHAMBER AND CHAINS HANGING",
    /* (4,5) */ "MUSHROOM HOLLOW AND PALE GLOW",
    /* (5,5) */ "CENTER OF DARK AND BLIND",
    /* (6,5) */ "GLOOM AND NO ECHO RETURNS",
    /* (7,5) */ "IRON CELL AND DOOR AJAR",
    /* (8,5) */ "FURNACE AND WALLS GLOWING",
    /* (9,5) */ "DRAGON'S APPROACH AND HEAT RISES",
    /* y=6 */
    /* (0,6) */ "GATEHOUSE AND PORTCULLIS HALF-RAISED",
    /* (1,6) */ "UNDERCROFT AND DAMP AND COLD",
    /* (2,6) */ "CELL AND PRAYERS SCRATCHED ON WALL",
    /* (3,6) */ "ROOT MAZE AND TWISTING DARK",
    /* (4,6) */ "WORM NEST AND WRITHING WET",
    /* (5,6) */ "PULP WALL AND SOFT AND YIELDING",
    /* (6,6) */ "NEXUS AND ALL PATHS CONVERGE",
    /* (7,6) */ "SQUEEZE AND SHOULDERS SCRAPE ROCK",
    /* (8,6) */ "MOLTEN CRACK AND STONE SWEATS",
    /* (9,6) */ "ANTECHAMBER AND SCALES ON WALL",
    /* y=7 */
    /* (0,7) */ "CRYPT ENTRANCE AND AIR STILL",
    /* (1,7) */ "PIT STAIR AND ROPE FRAYING",
    /* (2,7) */ "STONE THROAT AND BREATH FOGS",
    /* (3,7) */ "BLACK STONE AND NO LIGHT BOUNCES",
    /* (4,7) */ "GRAVE ROW AND SLABS SHIFTED",
    /* (5,7) */ "SUMP AND WATER TO KNEES",
    /* (6,7) */ "ASH PIT AND EMBERS BREATHING",
    /* (7,7) */ "LAST STAIR AND RUNGS RED-HOT",
    /* (8,7) */ "GLOW AND ORANGE LIGHT NO FLAME",
    /* (9,7) */ "THRESHOLD AND GOLD COINS AND BONES",
    /* y=8 */
    /* (0,8) */ "GATE TOWER AND SHADOW DEEP",
    /* (1,8) */ "FOG PIT AND COLD SEEPS UP",
    /* (2,8) */ "MOSS FLOOR AND SOFT AND WRONG",
    /* (3,8) */ "ASH BED AND CINDERS WARM",
    /* (4,8) */ "RIDGE AND STONE LIKE A SPINE",
    /* (5,8) */ "WRAITH LANE AND SHAPES THAT MOVE",
    /* (6,8) */ "EMBER FIELD AND GROUND STILL WARM",
    /* (7,8) */ "FINAL SQUEEZE AND EARTH'S RIBS",
    /* (8,8) */ "HEARTH AND FIRE ROARS AND WALLS HOT",
    /* (9,8) */ "FINAL GATE AND KEYHOLE WAITING",
    /* y=9 */
    /* (0,9) */ "GARDEN'S SHADOW AND LAST GREEN",
    /* (1,9) */ "PIT EDGE AND OVERHANG CRUMBLING",
    /* (2,9) */ "BRINK AND ONE STEP FROM FALL",
    /* (3,9) */ "LAST GARDEN AND DEAD FLOWERS IN FROST",
    /* (4,9) */ "EDGE AND OVERHANG TO THE VOID",
    /* (5,9) */ "LAIR'S BRINK AND BREATH RAGGED",
    /* (6,9) */ "LAST STEP AND DRAGON'S SHADOW LONG",
    /* (7,9) */ "DOOR AND IRON AND DRAGON'S BREATH THROUGH",
    /* (8,9) */ "LAST WALL AND DRAGON'S SNORE LIKE THUNDER",
    /* (9,9) */ "DRAGON'S LAIR AND THE EXIT BEYOND",
};

/* ---------- Union-Find ---------- */

void uf_init(void)
{
    uint8_t i;
    for (i = 0; i < ROOMS; i++) {
        uf_parent[i] = i;
        uf_rank[i] = 0;
    }
}

uint8_t uf_find(uint8_t x)
{
    while (uf_parent[x] != x) {
        uf_parent[x] = uf_parent[uf_parent[x]];
        x = uf_parent[x];
    }
    return x;
}

void uf_union(uint8_t a, uint8_t b)
{
    uint8_t ra, rb;
    ra = uf_find(a);
    rb = uf_find(b);
    if (ra != rb) {
        if (uf_rank[ra] < uf_rank[rb]) {
            uf_parent[ra] = rb;
        } else {
            uf_parent[rb] = ra;
            if (uf_rank[ra] == uf_rank[rb]) {
                uf_rank[ra]++;
            }
        }
    }
}

/* ---------- Maze Generation (Randomized Kruskal's MST) ---------- */

void gen_edges(void)
{
    uint8_t x, y;
    edge_count = 0;
    /* Horizontal edges: (x,y) -- (x+1,y) */
    for (y = 0; y < GRID_H; y++) {
        for (x = 0; x < GRID_W - 1; x++) {
            edge_u[edge_count] = y * GRID_W + x;
            edge_v[edge_count] = y * GRID_W + (x + 1);
            edge_count++;
        }
    }
    /* Vertical edges: (x,y) -- (x,y+1) */
    for (x = 0; x < GRID_W; x++) {
        for (y = 0; y < GRID_H - 1; y++) {
            edge_u[edge_count] = y * GRID_W + x;
            edge_v[edge_count] = (y + 1) * GRID_W + x;
            edge_count++;
        }
    }
}

void shuffle_edges(void)
{
    uint8_t i, j, tmp;
    for (i = edge_count - 1; i > 0; i--) {
        j = (uint8_t)(_XL_RAND() % (i + 1));
        tmp = edge_u[i]; edge_u[i] = edge_u[j]; edge_u[j] = tmp;
        tmp = edge_v[i]; edge_v[i] = edge_v[j]; edge_v[j] = tmp;
    }
}

void gen_maze(void)
{
    uint8_t i, added, u, v, ux, uy, vx, vy;
    uf_init();
    gen_edges();
    shuffle_edges();
    for (i = 0; i < ROOMS; i++) {
        room_conn[i] = 0;
    }
    added = 0;
    for (i = 0; i < edge_count && added < ROOMS - 1; i++) {
        u = edge_u[i];
        v = edge_v[i];
        if (uf_find(u) != uf_find(v)) {
            uf_union(u, v);
            ux = u % GRID_W; uy = u / GRID_W;
            vx = v % GRID_W; vy = v / GRID_W;
            if (ux == vx) {
                /* Vertical edge */
                if (uy < vy) {
                    room_conn[u] |= DIR_S;
                    room_conn[v] |= DIR_N;
                } else {
                    room_conn[u] |= DIR_N;
                    room_conn[v] |= DIR_S;
                }
            } else {
                /* Horizontal edge */
                if (ux < vx) {
                    room_conn[u] |= DIR_E;
                    room_conn[v] |= DIR_W;
                } else {
                    room_conn[u] |= DIR_W;
                    room_conn[v] |= DIR_E;
                }
            }
            added++;
        }
    }
}

/* ---------- Entity Placement ---------- */

void place_entities(void)
{
    uint8_t rooms[98];
    uint8_t num_rooms, i, j, tmp, idx, k;

    for (i = 0; i < ROOMS; i++) {
        room_enemy[i] = ENEMY_NONE;
        room_enemy_hp[i] = 0;
        room_item[i] = ITEM_NONE;
    }

    /* Dragon at exit room (9,9) = index 99 */
    room_enemy[ROOMS - 1] = ENEMY_DRAGON;
    room_enemy_hp[ROOMS - 1] = 60;

    /* Build list of available rooms (exclude start=0 and exit=99) */
    num_rooms = 0;
    for (i = 0; i < ROOMS; i++) {
        if (i != 0 && i != ROOMS - 1) {
            rooms[num_rooms++] = i;
        }
    }

    /* Shuffle */
    for (i = num_rooms - 1; i > 0; i--) {
        j = (uint8_t)(_XL_RAND() % (i + 1));
        tmp = rooms[i]; rooms[i] = rooms[j]; rooms[j] = tmp;
    }

    /* Place 10 slimes */
    idx = 0;
    for (k = 0; k < 10; k++) {
        room_enemy[rooms[idx]] = ENEMY_SLIME;
        room_enemy_hp[rooms[idx]] = 10;
        idx++;
    }
    /* Place 10 goblins */
    for (k = 0; k < 10; k++) {
        room_enemy[rooms[idx]] = ENEMY_GOBLIN;
        room_enemy_hp[rooms[idx]] = 20;
        idx++;
    }
    /* Place 10 orcs */
    for (k = 0; k < 10; k++) {
        room_enemy[rooms[idx]] = ENEMY_ORC;
        room_enemy_hp[rooms[idx]] = 35;
        idx++;
    }
    /* Place 7 potions */
    for (k = 0; k < 7; k++) {
        room_item[rooms[idx]] = ITEM_POTION;
        idx++;
    }
    /* Place 7 weapons */
    for (k = 0; k < 7; k++) {
        room_item[rooms[idx]] = ITEM_WEAPON;
        idx++;
    }
    /* Place 6 shields */
    for (k = 0; k < 6; k++) {
        room_item[rooms[idx]] = ITEM_SHIELD;
        idx++;
    }
}

/* ---------- Enemy Stats Helpers ---------- */

uint8_t get_enemy_atk(uint8_t type)
{
    switch (type) {
        case ENEMY_SLIME:  return 3;
        case ENEMY_GOBLIN: return 5;
        case ENEMY_ORC:    return 8;
        case ENEMY_DRAGON: return 12;
        default:           return 0;
    }
}

uint8_t get_enemy_def(uint8_t type)
{
    switch (type) {
        case ENEMY_SLIME:  return 0;
        case ENEMY_GOBLIN: return 1;
        case ENEMY_ORC:    return 3;
        case ENEMY_DRAGON: return 5;
        default:           return 0;
    }
}

const char *enemy_name(uint8_t type)
{
    switch (type) {
        case ENEMY_SLIME:  return "SLIME";
        case ENEMY_GOBLIN: return "GOBLIN";
        case ENEMY_ORC:    return "ORC";
        case ENEMY_DRAGON: return "DRAGON";
        default:           return "NONE";
    }
}

/* ---------- Combat ---------- */

void do_fight(uint8_t room)
{
    uint8_t e_atk, e_def, dmg, e_atk_dmg;

    if (room_enemy[room] == ENEMY_NONE) {
        msg = "NO ENEMY TO FIGHT.";
        return;
    }

    e_def = get_enemy_def(room_enemy[room]);
    e_atk = get_enemy_atk(room_enemy[room]);

    /* Player attacks */
    dmg = player_attack - e_def;
    if (dmg < 1) dmg = 1;
    room_enemy_hp[room] -= dmg;

    if (room_enemy_hp[room] > 0) {
        /* Enemy counter-attacks */
        e_atk_dmg = e_atk - player_defense;
        if (e_atk_dmg < 1) e_atk_dmg = 1;
        player_energy -= e_atk_dmg;
        if (player_energy > player_max_energy) player_energy = player_max_energy;
        if (player_energy == 0) {
            msg = "YOU WERE DEFEATED.";
            game_over = 1;
            _XL_EXPLOSION_SOUND();
            return;
        }
        msg = "YOU HIT FOR ";
        _XL_TOCK_SOUND();
    } else {
        /* Enemy defeated */
        switch (room_enemy[room]) {
            case ENEMY_SLIME:
                player_score += 10;
                msg = "SLIME DEFEATED 10 PTS";
                break;
            case ENEMY_GOBLIN:
                player_score += 20;
                msg = "GOBLIN DEFEATED 20 PTS";
                break;
            case ENEMY_ORC:
                player_score += 35;
                msg = "ORC DEFEATED 35 PTS";
                break;
            case ENEMY_DRAGON:
                player_score += 100;
                player_has_key = 1;
                msg = "DRAGON SLAIN";
                _XL_EXPLOSION_SOUND();
                break;
        }
        room_enemy[room] = ENEMY_NONE;
        room_enemy_hp[room] = 0;
    }
}

/* ---------- Item Pickup ---------- */

void do_pickup(uint8_t room)
{
    switch (room_item[room]) {
        case ITEM_POTION:
            player_energy += 20;
            if (player_energy > player_max_energy) player_energy = player_max_energy;
            player_score += 5;
            msg = "ENERGY RESTORED WITH THE POTION";
            room_item[room] = ITEM_NONE;
            _XL_PING_SOUND();
            break;
        case ITEM_WEAPON:
            player_attack += 3;
            player_score += 5;
            msg = "PICKED UP WEAPON GIVE ATK UP 3";
            room_item[room] = ITEM_NONE;
            _XL_PING_SOUND();
            break;
        case ITEM_SHIELD:
            player_defense += 2;
            player_score += 5;
            msg = "PICKED UP SHIELD GIVES DEF UP 2";
            room_item[room] = ITEM_NONE;
            _XL_PING_SOUND();
            break;
        default:
            msg = "NOTHING TO PICK UP";
            break;
    }
}

/* ---------- Display ---------- */

void display(void)
{
    uint8_t room;

    room = player_y * GRID_W + player_x;

    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 0, "KILL THE DRAGON");

    /* Stats */
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(0, 2, "ENERGY ");
    _XL_PRINTD(8, 2, 1, player_energy);
    _XL_PRINT(14, 2, "/");
    _XL_PRINTD(15, 2, 1, player_max_energy);

    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(23, 2, "ATK ");
    _XL_PRINTD(28, 2, 1, player_attack);

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(35, 2, "DEF ");
    _XL_PRINTD(40, 2, 1, player_defense);

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(47, 2, "KEY ");
    if (player_has_key) {
        _XL_PRINT(52, 2, "YES");
    } else {
        _XL_PRINT(52, 2, "NO");
    }

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(60, 2, "SCORE ");
    _XL_PRINTD(67, 2, 1, player_score);

    /* Room position and description */
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(0, 4, "ROOM ");
    _XL_PRINTD(6, 4, 1, player_x);
    _XL_PRINT(8, 4, ",");
    _XL_PRINTD(10, 4, 1, player_y);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(25, 4, room_desc[room]);

    /* Connections */
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    if (room_conn[room] & DIR_N) {
        _XL_PRINT(0, 5, "NORTH OPEN");
    } else {
        _XL_PRINT(0, 5, "NORTH WALL");
    }
    if (room_conn[room] & DIR_S) {
        _XL_PRINT(15, 5, "SOUTH OPEN");
    } else {
        _XL_PRINT(15, 5, "SOUTH WALL");
    }
    if (room_conn[room] & DIR_E) {
        _XL_PRINT(30, 5, "EAST OPEN");
    } else {
        _XL_PRINT(30, 5, "EAST WALL");
    }
    if (room_conn[room] & DIR_W) {
        _XL_PRINT(45, 5, "WEST OPEN");
    } else {
        _XL_PRINT(45, 5, "WEST WALL");
    }

    /* Enemy info */
    _XL_SET_TEXT_COLOR(_XL_RED);
    if (room_enemy[room] != ENEMY_NONE) {
        _XL_PRINT(0, 7, "ENEMY ");
        _XL_PRINT(7, 7, enemy_name(room_enemy[room]));
        _XL_PRINT(16, 7, "HP ");
        _XL_PRINTD(20, 7, 1, room_enemy_hp[room]);
    } else if (room == ROOMS - 1) {
        if (player_has_key) {
            _XL_SET_TEXT_COLOR(_XL_GREEN);
            _XL_PRINT(0, 7, "EXIT OPEN");
        } else {
            _XL_PRINT(0, 7, "EXIT CLOSED");
        }
    } else {
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINT(0, 7, "NO ENEMY HERE");
    }

    /* Item info */
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    if (room_item[room] != ITEM_NONE) {
        _XL_PRINT(0, 9, "ITEM ");
        switch (room_item[room]) {
            case ITEM_POTION:
                _XL_PRINT(6, 9, "HEALTH POTION 20 ENERGY");
                break;
            case ITEM_WEAPON:
                _XL_PRINT(6, 9, "WEAPON 3 ATTACK");
                break;
            case ITEM_SHIELD:
                _XL_PRINT(6, 9, "SHIELD 2 DEFENSE");
                break;
        }
    } else {
        _XL_PRINT(0, 9, "NO ITEM HERE");
    }

    /* Controls */
    _XL_SET_TEXT_COLOR(_XL_MAGENTA);
    _XL_PRINT(0, 11, "UP NORTH  DOWN SOUTH  LEFT WEST  RIGHT EAST");
    _XL_PRINT(0, 12, "FIRE FIGHT");

    /* Message */
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(0, 14, msg);

    /* Map hint */
    _XL_SET_TEXT_COLOR(_XL_BLUE);
    // _XL_PRINT(0, 16, "MAP 10X10  START 0,0  EXIT 9,9");
    _XL_PRINT(0, 17, "FIND THE DRAGON KILL IT GET THE KEY");
}

/* ---------- Game Init ---------- */

void init_game(void)
{
    player_x = 0;
    player_y = 0;
    player_energy = 50;
    player_max_energy = 50;
    player_attack = 5;
    player_defense = 0;
    player_has_key = 0;
    player_score = 0;
    game_over = 0;
    game_won = 0;
    msg = "FIND THE DRAGON AND ESCAPE!";

    gen_maze();
    place_entities();
}

/* ---------- Game Over / Win Screens ---------- */

void show_game_over(void)
{
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(5, 5, "GAME OVER");
    _XL_PRINT(5, 7, "YOU WERE DEFEATED IN THE MAZE");
    _XL_PRINT(5, 9, "FINAL SCORE ");
    _XL_PRINTD(18, 9, 1, player_score);
    _XL_PRINT(5, 11, "PRESS ANY KEY TO RESTART");
}

void show_game_won(void)
{
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(5, 5, "YOU ESCAPED THE MAZE");
    _XL_PRINT(5, 7, "THE DRAGON IS SLAIN");
    _XL_PRINT(5, 9, "FINAL SCORE ");
    _XL_PRINTD(18, 9, 1, player_score);
    _XL_PRINT(5, 11, "PRESS ANY KEY TO PLAY AGAIN");
    _XL_PING_SOUND();
}

/* ---------- Main ---------- */

int main(void)
{
    uint8_t input;
    uint8_t room;
    uint8_t new_x, new_y;
    uint8_t dir;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        init_game();

        display();

        while (1) {
            room = player_y * GRID_W + player_x;

            input = _XL_INPUT();

            if (game_over || game_won)
                break;

            /* Movement */
            dir = 0;
            if (_XL_UP(input)) {
                dir = DIR_N;
                new_y = player_y - 1;
                new_x = player_x;
            } else if (_XL_DOWN(input)) {
                dir = DIR_S;
                new_y = player_y + 1;
                new_x = player_x;
            } else if (_XL_LEFT(input)) {
                dir = DIR_W;
                new_x = player_x - 1;
                new_y = player_y;
            } else if (_XL_RIGHT(input)) {
                dir = DIR_E;
                new_x = player_x + 1;
                new_y = player_y;
            }

            if (dir != 0) {
                if (room_conn[room] & dir) {
                    if (new_x < GRID_W && new_y < GRID_H) {
                        player_x = new_x;
                        player_y = new_y;
                        msg = "YOU MOVED";
                        _XL_TICK_SOUND();
                    }
                } else {
                    msg = "CANNOT GO THAT WAY";
                    _XL_TOCK_SOUND();
                }
                display();
                continue;
            }

            /* Fire / Interact */
            if (_XL_FIRE(input)) {
                if (room == ROOMS - 1 && player_has_key) {
                    game_won = 1;
                    player_score += 200;
                    break;
                }
                if (room_enemy[room] != ENEMY_NONE) {
                    do_fight(room);
                    if (game_over)
                        break;
                } else if (room_item[room] != ITEM_NONE) {
                    do_pickup(room);
                } else if (room == ROOMS - 1) {
                    msg = "KILL THE DRAGON FIRST.";
                    _XL_TOCK_SOUND();
                } else {
                    msg = "NOTHING TO INTERACT WITH";
                }
                display();
                continue;
            }
        }

        /* End of game */
        if (game_over) {
            show_game_over();
        } else {
            show_game_won();
        }

        _XL_WAIT_FOR_INPUT();
    }

    return 0;
}
