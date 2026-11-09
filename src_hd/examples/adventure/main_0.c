#include "cross_lib.h"

/* Room exit codes: 255 = no exit */
#define NO_EXIT 255
#define NUM_ROOMS 22

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

/* Room data */
static const uint8_t room_north[NUM_ROOMS] = {
    NO_EXIT, NO_EXIT, NO_EXIT, NO_EXIT, 1, NO_EXIT, 4, 6,
    NO_EXIT, NO_EXIT, 1, 6, 11, NO_EXIT, NO_EXIT, NO_EXIT,
    14, 16, NO_EXIT, NO_EXIT, NO_EXIT, NO_EXIT
};
static const uint8_t room_south[NUM_ROOMS] = {
    NO_EXIT, 10, NO_EXIT, NO_EXIT, NO_EXIT, NO_EXIT, 11, NO_EXIT,
    NO_EXIT, NO_EXIT, NO_EXIT, 12, NO_EXIT, NO_EXIT, 16, NO_EXIT,
    17, NO_EXIT, NO_EXIT, NO_EXIT, NO_EXIT, NO_EXIT
};
static const uint8_t room_east[NUM_ROOMS] = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9, 10, 12, NO_EXIT, 13, 14, 15, 17,
    18, 19, 20, 21, NO_EXIT
};
static const uint8_t room_west[NUM_ROOMS] = {
    NO_EXIT, 0, 1, 2, 3, 4, 5, 6,
    7, 8, 9, NO_EXIT, 12, 13, 14, NO_EXIT,
    15, 17, NO_EXIT, 18, 19, 20
};

static const uint8_t room_enemy[NUM_ROOMS] = {
    ENEMY_NONE, ENEMY_NONE, ENEMY_GUARD, ENEMY_NONE, ENEMY_NONE,
    ENEMY_NONE, ENEMY_NONE, ENEMY_DRAGON, ENEMY_SKELETON, ENEMY_NONE,
    ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE,
    ENEMY_NONE, ENEMY_WRAITH, ENEMY_NONE, ENEMY_DARKLORD, ENEMY_NONE,
    ENEMY_NONE, ENEMY_NONE
};

static const uint8_t room_item[NUM_ROOMS] = {
    ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_POTION, ITEM_NONE,
    ITEM_NONE, ITEM_SWORD, ITEM_NONE, ITEM_NONE, ITEM_AMULET,
    ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE,
    ITEM_KEY, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE,
    ITEM_NONE, ITEM_NONE
};

static const uint8_t room_damage[NUM_ROOMS] = {
    0, 0, 0, 0, 0, 10, 0, 0,
    0, 0, 0, 15, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};

static const uint8_t room_score[NUM_ROOMS] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 500, 0,
    0, 0, 0, 0, 0, 0
};

static const uint8_t enemy_max_hp[NUM_ROOMS] = {
    0, 0, 20, 0, 0, 0, 0, 40,
    25, 0, 0, 0, 0, 0, 0, 0,
    30, 0, 50, 0, 0, 0
};

/* Room names */
static void print_room_name(uint8_t room)
{
    switch (room) {
        case 0:  _XL_PRINT(2, 0, "THE ENTRANCE HALL"); break;
        case 1:  _XL_PRINT(2, 0, "A DARK CORRIDOR"); break;
        case 2:  _XL_PRINT(2, 0, "THE GUARD ROOM"); break;
        case 3:  _XL_PRINT(2, 0, "THE ANCIENT LIBRARY"); break;
        case 4:  _XL_PRINT(2, 0, "THE FOUNTAIN CHAMBER"); break;
        case 5:  _XL_PRINT(2, 0, "THE TRAP ROOM"); break;
        case 6:  _XL_PRINT(2, 0, "THE ARMORY"); break;
        case 7:  _XL_PRINT(2, 0, "THE DRAGON S LAIR"); break;
        case 8:  _XL_PRINT(2, 0, "THE OLD CELLAR"); break;
        case 9:  _XL_PRINT(2, 0, "THE CHAPEL"); break;
        case 10: _XL_PRINT(2, 0, "THE STONE BRIDGE"); break;
        case 11: _XL_PRINT(2, 0, "THE SPIKE PIT"); break;
        case 12: _XL_PRINT(2, 0, "THE THRONE ROOM"); break;
        case 13: _XL_PRINT(2, 0, "THE GLOWING GARDEN"); break;
        case 14: _XL_PRINT(2, 0, "THE ENDLESS MAZE"); break;
        case 15: _XL_PRINT(2, 0, "THE TREASURE VAULT"); break;
        case 16: _XL_PRINT(2, 0, "THE SHADOW CORRIDOR"); break;
        case 17: _XL_PRINT(2, 0, "THE UNDERGROUND RIVER"); break;
        case 18: _XL_PRINT(2, 0, "THE DARK LORD S THRONE"); break;
        case 19: _XL_PRINT(2, 0, "VICTORY HALL"); break;
        case 20: _XL_PRINT(2, 0, "THE SECRET ROOM"); break;
        case 21: _XL_PRINT(2, 0, "THE EXIT"); break;
        default: break;
    }
}

static void print_room_desc(uint8_t room)
{
    switch (room) {
        case 0:
            _XL_PRINT(2, 2, "YOU STAND IN A COLD STONE HALL.");
            _XL_PRINT(2, 3, "DOORS LEAD EAST AND SOUTH.");
            break;
        case 1:
            _XL_PRINT(2, 2, "A NARROW CORRIDOR STRETCHES EAST.");
            _XL_PRINT(2, 3, "A PASSAGE DROPS SOUTH.");
            break;
        case 2:
            _XL_PRINT(2, 2, "A HEAVY GUARD BLOCKS THE WAY.");
            _XL_PRINT(2, 3, "HE GRINS AND DRAWS HIS BLADE.");
            break;
        case 3:
            _XL_PRINT(2, 2, "ANCIENT TOMES LINES THE SHELVES.");
            _XL_PRINT(2, 3, "A GLOWING POTION SITS ON A TABLE.");
            break;
        case 4:
            _XL_PRINT(2, 2, "A CRYSTAL FOUNTAIN SHIMMERS HERE.");
            _XL_PRINT(2, 3, "WATER FLOWS OVER POLISHED STONE.");
            break;
        case 5:
            _XL_PRINT(2, 2, "THE FLOOR SHAKES! SPIKES RISE!");
            _XL_PRINT(2, 3, "YOU JUMP AND ESCAPE, BARELY.");
            break;
        case 6:
            _XL_PRINT(2, 2, "RUSTED WEAPONS HANG ON THE WALLS.");
            _XL_PRINT(2, 3, "A STEEL SWORD CATCHES YOUR EYE.");
            break;
        case 7:
            _XL_PRINT(2, 2, "A MIGHTY DRAGON COILS AROUND GOLD.");
            _XL_PRINT(2, 3, "ITS EYES BURN LIKE MOLTEN FIRE.");
            break;
        case 8:
            _XL_PRINT(2, 2, "BONES RATTLE IN THE DARK CELLS.");
            _XL_PRINT(2, 3, "A SKELETON RAISES ITS RUSTED SWORD.");
            break;
        case 9:
            _XL_PRINT(2, 2, "CANDLELIGHT FLECKS THE QUIET CHAPEL.");
            _XL_PRINT(2, 3, "A SILVER AMULET RESTS ON THE ALTAR.");
            break;
        case 10:
            _XL_PRINT(2, 2, "A RICKETY BRIDGE SPANS A DEEP PIT.");
            _XL_PRINT(2, 3, "WIND HOWLS FROM BELOW.");
            break;
        case 11:
            _XL_PRINT(2, 2, "THE FLOOR CRUMBLES! SPIKES BELOW!");
            _XL_PRINT(2, 3, "YOU GRAB A ROPE AND CLING ON!");
            break;
        case 12:
            _XL_PRINT(2, 2, "AN EMPTY THRONE SIT IN THE DARKNESS.");
            _XL_PRINT(2, 3, "DUST COVERS EVERY SURFACE.");
            break;
        case 13:
            _XL_PRINT(2, 2, "BIOLUMINESCENT PLANTS GLOW SOFT BLUE.");
            _XL_PRINT(2, 3, "THE AIR IS SWEET AND CALM.");
            break;
        case 14:
            _XL_PRINT(2, 2, "ENDLESS CORRIDORS TWIST AND TURN.");
            _XL_PRINT(2, 3, "ECHOES BOUNCE OFF COLD WALLS.");
            break;
        case 15:
            _XL_PRINT(2, 2, "GOLD AND GEMS PILE HIGH ON THE FLOOR!");
            _XL_PRINT(2, 3, "YOU POCKET A HANDFUL OF COINS.");
            break;
        case 16:
            _XL_PRINT(2, 2, "SHADOWS DANCE ON THE WALLS.");
            _XL_PRINT(2, 3, "A WRAITH MATERIALIZES BEFORE YOU!");
            break;
        case 17:
            _XL_PRINT(2, 2, "UNDERGROUND RIVERS FLOW THROUGH HERE.");
            _XL_PRINT(2, 3, "MOSS COVERS THE WET STONES.");
            break;
        case 18:
            _XL_PRINT(2, 2, "THE DARK LORD AWAKENS FROM HIS THRONE!");
            _XL_PRINT(2, 3, "DARKNESS COILS AROUND HIS FISTS!");
            break;
        case 19:
            _XL_PRINT(2, 2, "YOU STAND IN THE VICTORY HALL.");
            _XL_PRINT(2, 3, "THE DARKNESS HAS LIFTED. WELL DONE!");
            break;
        case 20:
            _XL_PRINT(2, 2, "A HIDDEN ROOM BEHIND THE WALL.");
            _XL_PRINT(2, 3, "A MYSTIC KEY SHINES ON A PEDASTAL.");
            break;
        case 21:
            _XL_PRINT(2, 2, "A BRIGHT DOOR LEADS TO FREEDOM.");
            _XL_PRINT(2, 3, "YOU STEP INTO THE SUNLIGHT. YOU ESCAPE!");
            break;
        default:
            break;
    }
}

static void print_exits(uint8_t room)
{
    uint8_t n = room_north[room];
    uint8_t s = room_south[room];
    uint8_t e = room_east[room];
    uint8_t w = room_west[room];

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(2, 5, "EXITS: ");
    if (n != NO_EXIT) _XL_PRINT(10, 5, "N ");
    if (s != NO_EXIT) _XL_PRINT(13, 5, "S ");
    if (e != NO_EXIT) _XL_PRINT(16, 5, "E ");
    if (w != NO_EXIT) _XL_PRINT(19, 5, "W ");
}

static void print_player_status(uint8_t health, uint16_t score,
                                uint8_t has_sword, uint8_t has_potion,
                                uint8_t has_amulet, uint8_t has_key)
{
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(2, 7, "HP: ");
    _XL_PRINTD(7, 7, 1, health);
    _XL_PRINT(12, 7, " SCORE: ");
    _XL_PRINTD(21, 7, 1, score);

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(2, 9, "ITEMS: ");
    if (has_sword)  _XL_PRINT(11, 9, "SWORD ");
    if (has_potion) _XL_PRINT(18, 9, "POTION ");
    if (has_amulet) _XL_PRINT(26, 9, "AMULET ");
    if (has_key)    _XL_PRINT(34, 9, "KEY ");
}

static void print_instructions(void)
{
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 11, "ARROWS:MOVE  FIRE:FIGHT/ATTACK");
}

static void print_enemy_info(uint8_t enemy_type, uint8_t enemy_hp,
                             uint8_t enemy_max)
{
    _XL_SET_TEXT_COLOR(_XL_RED);
    switch (enemy_type) {
        case ENEMY_GUARD:
            _XL_PRINT(2, 13, "ENEMY: GUARD ");
            break;
        case ENEMY_DRAGON:
            _XL_PRINT(2, 13, "ENEMY: DRAGON ");
            break;
        case ENEMY_SKELETON:
            _XL_PRINT(2, 13, "ENEMY: SKELETON ");
            break;
        case ENEMY_WRAITH:
            _XL_PRINT(2, 13, "ENEMY: WRAITH ");
            break;
        case ENEMY_DARKLORD:
            _XL_PRINT(2, 13, "ENEMY: DARK LORD ");
            break;
        default:
            break;
    }
    _XL_PRINT(16, 13, "HP: ");
    _XL_PRINTD(21, 13, 1, enemy_hp);
    _XL_PRINT(25, 13, "/");
    _XL_PRINTD(27, 13, 1, enemy_max);
}

static void print_message(uint8_t y, const char *msg, uint8_t color)
{
    _XL_SET_TEXT_COLOR(color);
    _XL_PRINT(2, y, msg);
}

static uint8_t get_enemy_hp(uint8_t room)
{
    return enemy_max_hp[room];
}

static void do_fight(uint8_t *health, uint8_t *enemy_hp, uint8_t room,
                     uint8_t has_sword, uint8_t has_amulet)
{
    uint8_t player_dmg;
    uint8_t enemy_dmg;
    uint8_t rand_val;

    /* Player attack */
    rand_val = (uint8_t)(_XL_RAND() % 6);
    player_dmg = 5 + rand_val;
    if (has_sword) player_dmg += 5;
    if (player_dmg > *enemy_hp) player_dmg = *enemy_hp;
    *enemy_hp -= player_dmg;

    _XL_SHOOT_SOUND();

    if (*enemy_hp <= 0) {
        _XL_EXPLOSION_SOUND();
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        _XL_PRINT(2, 15, "ENEMY DEFEATED! YOU ADVANCE.");
        _XL_SLEEP(1);
        return;
    }

    /* Enemy attack */
    rand_val = (uint8_t)(_XL_RAND() % 6);
    enemy_dmg = 3 + rand_val;
    if (has_amulet && enemy_dmg > 3) enemy_dmg -= 3;
    if (enemy_dmg > *health) enemy_dmg = *health;
    *health -= enemy_dmg;

    _XL_PING_SOUND();

    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(2, 15, "YOU HIT FOR ");
    _XL_PRINTD(14, 15, 1, player_dmg);
    _XL_PRINT(19, 15, " ENEMY HITS FOR ");
    _XL_PRINTD(33, 15, 1, enemy_dmg);
}

static void game_over_screen(void)
{
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(5, 5, "GAME OVER");
    _XL_PRINT(3, 8, "THE DARKNESS CLAIMS ANOTHER SOUL...");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(3, 12, "WAITING FOR RESTART...");
    _XL_ZAP_SOUND();
    _XL_SLEEP(2);
}

static void victory_screen(uint16_t score)
{
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(5, 3, "CONGRATULATIONS!");
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(3, 6, "YOU HAVE VANQUISHED THE DARK LORD");
    _XL_PRINT(3, 7, "AND ESCAPED THE DUNGEON!");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(3, 10, "FINAL SCORE: ");
    _XL_PRINTD(17, 10, 1, score);
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(3, 14, "WAITING FOR RESTART...");
    _XL_EXPLOSION_SOUND();
    _XL_SLEEP(2);
}

static void start_screen(void)
{
    _XL_CLEAR_SCREEN();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(3, 2, "THE CURSED DUNGEON");
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 5, "A TEXT ADVENTURE OF 22 ROOMS");
    _XL_PRINT(2, 7, "FIND THE DARK LORD AND ESCAPE!");
    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(2, 10, "ARROWS: MOVE N/S/E/W");
    _XL_PRINT(2, 11, "FIRE: ATTACK ENEMY");
    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(2, 14, "COLLECT ITEMS TO SURVIVE.");
    _XL_SET_TEXT_COLOR(_XL_RED);
    _XL_PRINT(2, 17, "PRESS ANY KEY TO BEGIN...");
    _XL_WAIT_FOR_INPUT();
}

static void play_game(void)
{
    uint8_t room;
    uint8_t health;
    uint16_t score;
    uint8_t has_sword;
    uint8_t has_potion;
    uint8_t has_amulet;
    uint8_t has_key;
    uint8_t enemy_hp;
    uint8_t enemy_max;
    uint8_t input;
    uint8_t game_active;
    uint8_t trap_hit;
    uint8_t item_found;

    room = 0;
    health = 50;
    score = 0;
    has_sword = 0;
    has_potion = 0;
    has_amulet = 0;
    has_key = 0;
    game_active = 1;
    trap_hit = 0;
    item_found = 0;

    /* Enter room loop */
    while (game_active) {
        _XL_CLEAR_SCREEN();

        /* Check for trap damage on entry */
        if (room_damage[room] > 0 && !trap_hit) {
            health -= room_damage[room];
            trap_hit = 1;
            _XL_TOCK_SOUND();
            if (health > 0) {
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(2, 15, "THE TRAP HITS YOU FOR ");
                _XL_PRINTD(24, 15, 1, room_damage[room]);
                _XL_PRINT(28, 15, " HP!");
                _XL_SLEEP(1);
                _XL_CLEAR_SCREEN();
            } else {
                health = 0;
            }
        } else {
            trap_hit = 0;
        }

        /* Check for item pickup */
        if (room_item[room] != ITEM_NONE && !item_found) {
            item_found = 1;
            _XL_TICK_SOUND();
            switch (room_item[room]) {
                case ITEM_POTION:
                    has_potion = 1;
                    if (health < 50) {
                        health += 20;
                        if (health > 50) health = 50;
                    }
                    _XL_SET_TEXT_COLOR(_XL_GREEN);
                    _XL_PRINT(2, 15, "YOU DRINK A POTION! +20 HP");
                    break;
                case ITEM_SWORD:
                    has_sword = 1;
                    _XL_SET_TEXT_COLOR(_XL_GREEN);
                    _XL_PRINT(2, 15, "YOU TAKE THE STEEL SWORD!");
                    break;
                case ITEM_AMULET:
                    has_amulet = 1;
                    _XL_SET_TEXT_COLOR(_XL_GREEN);
                    _XL_PRINT(2, 15, "YOU TAKE THE SILVER AMULET!");
                    break;
                case ITEM_KEY:
                    has_key = 1;
                    _XL_SET_TEXT_COLOR(_XL_GREEN);
                    _XL_PRINT(2, 15, "YOU TAKE THE MYSTIC KEY!");
                    break;
                default:
                    break;
            }
            _XL_SLEEP(1);
        } else {
            item_found = 0;
        }

        /* Check for score bonus */
        if (room_score[room] > 0) {
            score += room_score[room];
            _XL_PING_SOUND();
        }

        /* Draw room */
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_YELLOW);
        print_room_name(room);
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        print_room_desc(room);
        print_exits(room);
        print_player_status(health, score, has_sword, has_potion,
                            has_amulet, has_key);
        print_instructions();

        /* Check for enemy */
        if (room_enemy[room] != ENEMY_NONE) {
            enemy_max = get_enemy_hp(room);
            enemy_hp = enemy_max;
            print_enemy_info(room_enemy[room], enemy_hp, enemy_max);

            /* Combat loop */
            while (enemy_hp > 0 && health > 0) {
                input = _XL_INPUT();
                if (_XL_FIRE(input)) {
                    do_fight(&health, &enemy_hp, room,
                             has_sword, has_amulet);
                    if (enemy_hp > 0 && health > 0) {
                        /* Redraw HP bars */
                        _XL_SET_TEXT_COLOR(_XL_RED);
                        _XL_PRINT(2, 13, "ENEMY: ");
                        _XL_PRINT(9, 13, "HP: ");
                        _XL_PRINTD(14, 13, 1, enemy_hp);
                        _XL_PRINT(18, 13, "/");
                        _XL_PRINTD(20, 13, 1, enemy_max);
                        _XL_SET_TEXT_COLOR(_XL_GREEN);
                        _XL_PRINT(2, 7, "HP: ");
                        _XL_PRINTD(7, 7, 1, health);
                    }
                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                } else {
                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                }
            }

            if (health <= 0) {
                game_active = 0;
                break;
            }
            if (enemy_hp <= 0) {
                score += 100;
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_PRINT(2, 15, "ENEMY SLAIN! +100 SCORE");
                _XL_SLEEP(1);
            }
        }

        /* Check for victory rooms */
        if (room == 19 || room == 21) {
            game_active = 0;
            break;
        }

        /* Wait for movement input */
        input = _XL_INPUT();
        if (_XL_UP(input)) {
            if (room_north[room] != NO_EXIT) {
                room = room_north[room];
                _XL_TICK_SOUND();
            }
        } else if (_XL_DOWN(input)) {
            if (room_south[room] != NO_EXIT) {
                room = room_south[room];
                _XL_TICK_SOUND();
            }
        } else if (_XL_LEFT(input)) {
            if (room_west[room] != NO_EXIT) {
                room = room_west[room];
                _XL_TICK_SOUND();
            }
        } else if (_XL_RIGHT(input)) {
            if (room_east[room] != NO_EXIT) {
                room = room_east[room];
                _XL_TICK_SOUND();
            }
        } else {
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }
    }

    /* End of game */
    if (health <= 0) {
        game_over_screen();
    } else {
        victory_screen(score);
    }
}

int main(void)
{
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    while (1) {
        start_screen();
        play_game();
    }

    return 0;
}

