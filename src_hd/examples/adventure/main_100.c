#include "cross_lib.h"

#define NO_EXIT 255
#define GRID_SIZE 10
#define NUM_ROOMS 100

#define ENEMY_NONE 0
#define ENEMY_GUARD 1
#define ENEMY_DRAGON 2
#define ENEMY_SKELETON 3
#define ENEMY_WRAITH 4
#define ENEMY_DARKLORD 5

#define ITEM_NONE 0
#define ITEM_POTION 1
#define ITEM_SWORD 2
#define ITEM_AMULET 3
#define ITEM_KEY 4

static uint8_t enemy_alive[NUM_ROOMS];
static uint8_t item_remaining[NUM_ROOMS];
static uint8_t score_remaining[NUM_ROOMS];
static uint8_t room_north[NUM_ROOMS];
static uint8_t room_south[NUM_ROOMS];
static uint8_t room_east[NUM_ROOMS];
static uint8_t room_west[NUM_ROOMS];
static uint8_t room_enemy[NUM_ROOMS];
static uint8_t room_item[NUM_ROOMS];
static uint8_t room_damage[NUM_ROOMS];
static uint8_t room_score[NUM_ROOMS];
static uint8_t enemy_max_hp[NUM_ROOMS];

static const char *room_names[GRID_SIZE][GRID_SIZE] = {
    {"THE ENTRANCE","THE VESTIBULE","HALL OF MIRRORS","CORRIDOR A","THE CLOISTER",
     "THE PATIO","THE ARCHIVE","OBSERVATORY","THE BELFRY","THE OUTPOST"},
    {"THE KITCHEN","DUNGEON CELL","THE ARMORY","THE CHAPEL","THE LABORATORY",
     "ALCHEMY CELLAR","TORMENT ROOM","WATCHTOWER","STOREROOM","GATEHOUSE"},
    {"THE GATE","PRISON WARREN","FORGE HALL","CATHEDRAL","ALCHEMY LAB",
     "MUSHROOM GROTTO","TORMENT CHAMBER","LOOKOUT","CAVE-IN","SENTINEL POST"},
    {"THE BRIDGE","OLD CELLARS","BLACKSMITHY","THE CRYPT","REACTOR CORE",
     "SPORE GARDEN","AGONY PIT","SPYGLASS TOWER","COLLAPSED WALL","BASTION"},
    {"THE CROSSING","UNDERCROFT","IRONWORKS","THE TOMB","MACHINERY HALL",
     "GLOWING FUNGUS","SCREAMING VOID","TELESCOPE ROOM","RUIN","FORTRESS"},
    {"THE FORGE","DEEP CELLARS","STEEL WORKS","THE VAULT","ENGINE ROOM",
     "MYCELIUM NEST","WHISPERING HALL","ASTROLABE ROOM","DEBRIS FIELD","KEEP"},
    {"THE SMELTERY","ABYSSAL PIT","COBBLER'S WORKS","HOLY SEPULCHRE","GEARWORKS",
     "BIOLUMINESCENT POOL","ECHO CHAMBER","CELESTIAL MAPS","SHATTERED STONE","DONJON"},
    {"BLAST FURNACE","VOID CHASM","IRON SMITHY","RELIC SHRINE","MECH HEART",
     "SPARKLING POND","RESONANT CAVERN","STARGAZER'S LOFT","CRUMBLING ARCH","CITADEL"},
    {"MOLTEN CHAMBER","ABYSS GATE","FORGE OF LEGENDS","MAUSOLEUM","CORE REACTOR",
     "CRYSTAL LAGOON","SOUND LABYRINTH","COSMIC CHARTS","FALLEN COLUMN","DONJON KEEP"},
    {"FURNACE MOUTH","PORTAL TO VOID","THE GODFORGE","THE PANTHEON","WORLD ENGINE",
     "PRISM POOL","HARMONIC MAZE","GALAXY MAP","DARK LORD THRONE","THE EXIT"}
};

static const char *room_desc1[GRID_SIZE][GRID_SIZE] = {
    {"COLD STONE MEETS FRESH AIR.","ECHOES BOUNCE OFF MARBLE FLOORS.",
     "MIRRORS REFLECT ENDLESS FEAR.","A LONG HALL, TORCHES BURN DIM.",
     "MONKS PRAY IN SILENT ROWS.","OPEN SKY AND SINGING BIRDS ABOVE.",
     "ANCIENT SCROLLS LINE THE WALLS.","STARS WHEEL THROUGH A SKY HOLE.",
     "BELLS TOLL WITHOUT A WIND.","SOLDIERS STAND AT EASE, WEARY."},
    {"SMELL OF ROT AND OLD FOOD HAUNTS.","A PRISONER'S MOAN ECHOES FAR.",
     "RUSTED BLADES HANG IN RACKS.","CANDLES GUTTER IN A COLD DRAFT.",
     "BUBBLING FLASKS CLUTTER EVERY SHELF.","FUNGAL SMELL, WEIRD AND WEAK.",
     "WHISPERS OF THE TORMENTED RISE.","A WATCHMAN'S LOG, ABANDONED MID-ENTRY.",
     "CRATES STACKED HIGH IN DARK CORNERS.","A HEAVY GATE, BARRED SHUT."},
    {"AN ANCIENT GATE, RUSTED BUT SOLID.","IRON BARS AND CHAINED DOORS.",
     "SPARKS FLY FROM A COLD ANVIL.","STAINED GLASS PAYS IN FADING COLOR.",
     "DISTILLED ESSENCES FILL THE AIR.","TINY MUSHROOMS GLOW A FAINT BLUE.",
     "SHACKLES HANG FROM CHAINS IN THE DARK.","A NARROW TOWER WITH A VIEW NORTH.",
     "ROCKS BLOCK THE PATH. A NARROW GAP.","ARMORED SENTRY POSITIONS, ABANDONED."},
    {"A RICKETY BRIDGE OVER A BLACK PIT.","DARK CELLARS, MOSS ON EVERY WALL.",
     "HAMMER BLAZES AND FORGE GLOW.","COFFINS LINE THE CATHEDRAL FLOOR.",
     "A PULSING REACTOR HUMS VIOLENTLY.","SPORES DRIFT IN SLOW, DANCING CIRCLES.",
     "THE FLOOR IS A PIT OF SPIKES.","A TOWER WITH LENSES TRACKING THE SKY.",
     "COLLAPSED STONE. YOU SQUEEZE THROUGH.","A BASTION WITH WATCHFIRE BOWS."},
    {"A STONE CROSSING OVER A DARK RIVER.","BENEATH THE BENEATH. CLOUTED DARKNESS.",
     "IRON BEAMS AND RIVETS EVERYWHERE.","TOMBS CARVED INTO THE LIVING ROCK.",
     "GEARS THE SIZE OF WHEELS TURN SLOW.","LARGE FUNGUS CAPS PULSE WITH LIGHT.",
     "THE WALLS SCREAM IF YOU LINGER.","A ROOM FULL OF BROKEN TELESCOPES.",
     "CRUMBLED RUINS. WEATHER EATS ALL.","A SMALL FORTRESS, WELL-DEFENDED."},
    {"A FORGE STILL SMOKING FROM USE.","THE DEPEST KNOWN CELLAR LEVEL.",
     "SHEETS OF FLAT IRON, HALF-SHAPED.","GOLD AND GEMS. A THRONE OF BONES.",
     "A GREAT ENGINE POUNDS LIKE A HEART.","MYCELIUM THREADS EVERY SURFACE.",
     "VOICES IN THE WALL. THEY KNOW YOU.","AN ASTROLABE TRACKS AN UNSEEN SUN.",
     "DEBRIS AND DUST. THE FLOOR GIVES WAY.","THE KEEP. THE HEART OF THE DUNGEON."},
    {"MOLTEN METAL DRIPS FROM THE CEILING.","THE PIT FALLETS INTO NOTHING.",
     "COBBLES INHALER, HALVED, CRACKED.","A HOLY GRAIL IN A CAGE OF THORNS.",
     "GEARS MESH IN ENDLESS COMPLEXITY.","BIOLUMINESCENT PLANTS FILL A SMALL POOL.",
     "SOUNDS BOUNCE AND BOUNCE. NO EXIT FEEL.","CELESTIAL MAPS IN FADING INK.",
     "STONE SLABS SHATTERED. A WIDE CRACK.","THE DONJON. THE INNERMOST TOWER."},
    {"A FURNACE BURNS WITH GREEN FLAME.","A CHASM SO WIDE IT HAS NO FLOOR.",
     "IRON SMELT INTO SOMETHING ALIVE.","RELICS OF OLD GODS IN A SHALLOW DEPRESSION.",
     "A MECHANICAL HEART PUMPS OIL.",
     "A POND THAT REFLECTS A DIFFERENT SKY.","EVERY SOUND ECHOES IN HARMONIC PATTERNS.",
     "A LOFT FOR STARGAZERS, NOW FORGOTTEN.","ARCHES CRUMBLE IN SLOW MOTION.",
     "THE CITADEL. THE OUTER WALLS OF POWER."},
    {"MOLTEN ROCK LAPS THE STONE FLOOR.","A GATE TO THE ABYSS, HUMS AND GLOWS.",
     "THE FORGE WHERE GODS SHAPED WEAPONS.","A MAUSOLEUM FOR THE FORGOTTEN KINGS.",
     "THE CORE PULSES. THE FLOOR VIBRATES.",
     "CRYSTAL FORMATIONS RISE FROM THE WATER.","SOUND WAVES INVISIBLE AND DEADLY.",
     "CHARTS OF NEIGHBORING GALAXIES, INKED.","A SINGLE COLUMN, FALLEN ACROSS THE WAY.",
     "THE KEEP OF THE DONJON. ALMOST THERE."},
    {"THE FURNACE BREATHES. THE HEAT IS LETHAL.","A PORTAL SWIRLS, SHOWING THE VOID.",
     "THE GODFORGE. WHERE LEGENDS ARE MADE.","THE PANTHEON. STATUES WATCH YOUR EVERY MOVE.",
     "THE WORLD ENGINE. IT POWERS ALL THINGS.",
     "PRISMS SPLIT LIGHT INTO RAINBOW STAIRS.","THE MAZE OF HARMONIES. SOUND KILLS.",
     "THE MAP SHOWS THE WAY. AND THE TRAP.","THE DARK LORD AWAITS ON HIS THRONE.",
     "A BRIGHT DOOR. SUNLIGHT. FREEDOM."}
};

static const char *room_desc2[GRID_SIZE][GRID_SIZE] = {
    {"A DOOR LEADS EAST. THE HALL STRETCHES FORTH.","TWO DOORS. EAST AND SOUTH. DUST SETTLES.",
     "YOUR REFLECTION WINKS. IT SHOULD NOT.","THE PASSAGE NARROWS. KEEP MOVING.",
     "PARCHMENT AND PRAYER. A QUIET PEACE.","WARM BREEZE. YOU CAN ALMOST TASTE FREEDOM.",
     "KNOWLEDGE IS POWER. OR A TRAP.","A CONSTELLATION MAPS THE WAY DOWN.",
     "THE BELL ROPE IS CUT. IT RANG ONCE.","THE OUTPOST GUARDS THE NORTHERN APPROACH."},
    {"THE SINK IS FULL OF BLACK WATER.","THE CELL DOOR IS ALREADY OPEN.","SOMETHING GLINTS AMONG THE RUST. A SWORD.",
     "A SILVER AMULET RESTS ON THE ALTAR.","THE LABORATORY SMOKES AND BUZZES.",
     "A BOTTLE OF HEALING LIQUID AWAITS.","THE FLOOR IS CHAINS. THE AIR IS SORROW.",
     "THE WATCHMAN IS GONE. HIS LANTERN STILL BURNS.","PICK A CRATE. SOMETHING CLINKS INSIDE.",
     "THE GATE CREAKS. BEYOND IT, THE DUNGEON."},
    {"BEYOND THE GATE, THE PATH DESCENDS.","THE PRISONERS ARE LONG GONE. THE CHAINS REMAIN.",
     "THE ANVIL IS COLD. THE FORGE IS DEAD.","THE CATHEDRAL IS EMPTY. THE GOD LEFT.",
     "MIX THE ESSENCES. CAREFULLY. OR NOT.","THE MUSHROOMS PROVIDE THE ONLY LIGHT.",
     "THE CHAINS RATTLE. SOMETHING DRAGS ITSELF.","THE TOWER LOOKOUT. THE SKY IS FUR BELOW.",
     "FIND THE GAP. CRAWL. DON'T LOOK DOWN.","THE SENTINEL HAS FALLEN. HIS BONES TELL."},
    {"THE BRIDGE SHAKES UNDER YOUR WEIGHT.","CELLARS UPON CELLARS. THE DARK IS THICK HERE.",
     "THE FORGE GLOWS ANGRY RED. A DRAGON NESTS IN THE HEAT.","OPEN COFFINS. THE DEAD ARE NOT AT REST.",
     "THE REACTOR IS CRITICAL. STEAM ERUPTS.","THE SPORES MAKE YOU DIZZY. AND SLEEPY.",
     "SPIKES. ALL SPIKES. AND THE SCREAMING.","THE LENSES FOCUS ON SOMETHING BEHIND YOU.",
     "THE WALL HAS FALLEN. THE WAY IS CLEAR, BARELY.","THE BASTION HOLDS. BARELY. HOLD YOUR GROUND."},
    {"THE RIVER BELOW RUSHES AND SCREAMS.","UNDER THE UNDER. NO ONE HAS BEEN HERE IN AGES.",
     "THE IRONWORKS HUM WITH RESIDUAL HEAT.","THE TOMB WHISPERS NAMES. YOURS IS NEXT.",
     "THE MACHINES TURN. YOU ARE A GEAR NOW.","THE FUNGUS LIGHTS THE WAY DEEPER IN.",
     "DO NOT LISTEN. DO NOT STAY. MOVE.","THE TELESCOPES POINT AT THE CEILING. WHAT FOR?",
     "THE RUINS SHIFT. THE FLOOR IS UNSTABLE.","THE FORTRESS DOORS ARE BARRICADED FROM WITHIN."},
    {"THE FORGE IS STILL WARM. SOMETHING USED IT RECENTLY.","THE CELLS GO DEEPER. THE AIR THICKENS.",
     "STEEL SLEEPS IN RACKS. WAITING FOR A HAND.","THE VAULT. THE RICHES. THE DANGER.",
     "THE ENGINE ROARS. THE WALLS SWEAT STEAM.","THE NEST IS ALIVE. IT MOUNTS TOWARD YOU.",
     "THE WHISPERS FORM WORDS. THEY KNOW YOUR NAME.","THE ASTROLABE SPINS WITHOUT CAUSE.",
     "THE DEBRIS HIDDEN A PASSAGE. YOU FIND IT.","THE KEEP. THE FINAL THRESHOLD. YOU MUST FIGHT."},
    {"THE METAL RUNS IN VEINS. THE FLOOR IS GLASS NOW.","THE PIT SWALLOWS SOUND. AND LIGHT.",
     "THE COBBLERS TOOLS ARE STILL WARM.","THE GRAIL PULSES. THE THORNS BITE THE CAGE.",
     "GEARS GRIND AGAINST GEARS. THE NOISE IS LOUD.","THE POOL REFLECTS STARS THAT ARE NOT ABOVE.",
     "THE ECHO CHAMBER AMPLIFIES YOUR BREATH TO A ROAR.","THE MAPS SHOW A WAY. IF YOU CAN READ THE FADING INK.",
     "THE STONE CRUMBLES IN HANDS. THE CRACK WIDENS.","THE DONJON. THE INNER SANCTUM. THE END NEARS."},
    {"THE FURNACE BURNS. THE HEAT PEELS SKIN.","THE CHASM HAS NO BOTTOM. OR MAYBE IT DOES.",
     "THE IRON IS ALIVE. IT MOVES. IT HUNTS.","THE RELICS WHISPER IN DEAD LANGUAGES.",
     "THE HEART PUMPS. THE OIL FLOWS. THE MACHINE LIVES.","THE POND IS COLD. THE REFLECTION IS WRONG.",
     "THE HARMONIES CONVERGE. THE SOUND CUTS DEEP.","THE STARGAZER'S LOFT. THE GALAXY IS CLOSER HERE.",
     "THE ARCHES FALL ONE BY ONE. THE WAY IS CHANGING.","THE CITADEL WALLS GLOW WITH Runic POWER."},
    {"THE MOLTEN ROCK BURNES AT THE ANKLES.","THE GATE HUMS. THE ABYSS STARES BACK.",
     "THE GODFORGE BURNS WITH WHITE FIRE. WEAPONS OF LEGEND.","THE MAUSOLEUM. THE KINGS WATCH. JUDGE. PASS.",
     "THE CORE IS EXPOSED. THE ENERGY IS KILLING YOU SLOWLY.","THE CRYSTALS SING. THE WATER IS SACRED.",
     "THE SOUND LABYRINTH. EVERY STEP IS A NOTE.","THE CHARTS GLOW. THE GALAXY IS MAPPED. YOU ARE NEXT.",
     "THE COLUMN FALLS. THE WAY IS SEALED. FIND ANOTHER.","THE KEEP. THE FINAL ROOM BEFORE THE THRONE."},
    {"THE HEAT IS A WALL. THE FURNACE EATS THE AIR.","THE PORTAL OPENS. THE VOID STARES. DO NOT LOOK AWAY.",
     "THE GODFORGE. THE WEAPON THAT SLAYS GODS AWAITS.","THE PANTHEON. THE STATUES TURN. THEY SEE YOU.",
     "THE WORLD ENGINE. STOP IT AND THE WORLD ENDS.","THE PRISMS FIRE BEAMS OF KILLING LIGHT IN PATTERNS.",
     "THE HARMONIC MAZE. THE WRONG NOTE IS DEATH.","THE GALAXY MAP. THE WAY HOME. IF YOU CAN REACH IT.",
     "THE DARK LORD RISES. THE THRONE IS HOT. THE END.","THE BRIGHT DOOR. THE SUN. GO. NOW. WHILE YOU CAN."}
};

static void init_dungeon(void)
{
    uint8_t r, c, i;
    uint8_t row, col;

    for (i = 0; i < NUM_ROOMS; i++) {
        row = i / GRID_SIZE;
        col = i % GRID_SIZE;

        /* Compute exits from grid position */
        if (row > 0) room_north[i] = i - GRID_SIZE;
        else room_north[i] = NO_EXIT;

        if (row < GRID_SIZE - 1) room_south[i] = i + GRID_SIZE;
        else room_south[i] = NO_EXIT;

        if (col < GRID_SIZE - 1) room_east[i] = i + 1;
        else room_east[i] = NO_EXIT;

        if (col > 0) room_west[i] = i - 1;
        else room_west[i] = NO_EXIT;
    }

    /* Enemy placements: row 0-1 none/weak, 2-4 mid, 5-7 strong, 8-9 boss */
    for (i = 0; i < NUM_ROOMS; i++) {
        room_enemy[i] = ENEMY_NONE;
        room_item[i] = ITEM_NONE;
        room_damage[i] = 0;
        room_score[i] = 0;
        enemy_max_hp[i] = 0;
    }

    /* Row 0-1: No enemies (safe start) */
    /* Room 12 (1,2): Guard */
    room_enemy[12] = ENEMY_GUARD;
    enemy_max_hp[12] = 15;

    /* Room 14 (1,4): Skeleton */
    room_enemy[14] = ENEMY_SKELETON;
    enemy_max_hp[14] = 18;

    /* Row 2: Guards and skeletons */
    room_enemy[21] = ENEMY_GUARD;
    enemy_max_hp[21] = 18;
    room_enemy[24] = ENEMY_SKELETON;
    enemy_max_hp[24] = 22;
    room_enemy[27] = ENEMY_GUARD;
    enemy_max_hp[27] = 20;

    /* Row 3: Skeletons and wraiths */
    room_enemy[31] = ENEMY_SKELETON;
    enemy_max_hp[31] = 25;
    room_enemy[33] = ENEMY_WRAITH;
    enemy_max_hp[33] = 30;
    room_enemy[36] = ENEMY_SKELETON;
    enemy_max_hp[36] = 28;
    room_enemy[39] = ENEMY_GUARD;
    enemy_max_hp[39] = 24;

    /* Row 4: Wraiths and dragons */
    room_enemy[41] = ENEMY_WRAITH;
    enemy_max_hp[41] = 32;
    room_enemy[43] = ENEMY_DRAGON;
    enemy_max_hp[43] = 45;
    room_enemy[46] = ENEMY_WRAITH;
    enemy_max_hp[46] = 35;
    room_enemy[49] = ENEMY_DRAGON;
    enemy_max_hp[49] = 40;

    /* Row 5: Dragons and wraiths */
    room_enemy[52] = ENEMY_DRAGON;
    enemy_max_hp[52] = 42;
    room_enemy[54] = ENEMY_WRAITH;
    enemy_max_hp[54] = 38;
    room_enemy[56] = ENEMY_DRAGON;
    enemy_max_hp[56] = 48;
    room_enemy[59] = ENEMY_DRAGON;
    enemy_max_hp[59] = 50;

    /* Row 6: Wraiths and dragons */
    room_enemy[61] = ENEMY_WRAITH;
    enemy_max_hp[61] = 40;
    room_enemy[63] = ENEMY_DRAGON;
    enemy_max_hp[63] = 52;
    room_enemy[66] = ENEMY_WRAITH;
    enemy_max_hp[66] = 42;
    room_enemy[69] = ENEMY_DRAGON;
    enemy_max_hp[69] = 55;

    /* Row 7: Dragons */
    room_enemy[71] = ENEMY_DRAGON;
    enemy_max_hp[71] = 55;
    room_enemy[73] = ENEMY_DRAGON;
    enemy_max_hp[73] = 58;
    room_enemy[76] = ENEMY_WRAITH;
    enemy_max_hp[76] = 48;
    room_enemy[79] = ENEMY_DRAGON;
    enemy_max_hp[79] = 60;

    /* Row 8: Dragons and wraiths */
    room_enemy[81] = ENEMY_DRAGON;
    enemy_max_hp[81] = 60;
    room_enemy[83] = ENEMY_DRAGON;
    enemy_max_hp[83] = 62;
    room_enemy[86] = ENEMY_WRAITH;
    enemy_max_hp[86] = 55;
    room_enemy[89] = ENEMY_DRAGON;
    enemy_max_hp[89] = 65;

    /* Row 9: Dark Lord at 98 */
    room_enemy[91] = ENEMY_DRAGON;
    enemy_max_hp[91] = 70;
    room_enemy[94] = ENEMY_DRAGON;
    enemy_max_hp[94] = 72;
    room_enemy[98] = ENEMY_DARKLORD;
    enemy_max_hp[98] = 100;

    /* Item placements */
    room_item[3] = ITEM_POTION;
    room_item[6] = ITEM_SWORD;
    room_item[12] = ITEM_POTION;
    room_item[15] = ITEM_AMULET;
    room_item[22] = ITEM_POTION;
    room_item[25] = ITEM_KEY;
    room_item[33] = ITEM_POTION;
    room_item[38] = ITEM_POTION;
    room_item[44] = ITEM_POTION;
    room_item[50] = ITEM_POTION;
    room_item[55] = ITEM_POTION;
    room_item[62] = ITEM_POTION;
    room_item[68] = ITEM_POTION;
    room_item[75] = ITEM_POTION;
    room_item[80] = ITEM_POTION;
    room_item[88] = ITEM_POTION;
    room_item[93] = ITEM_POTION;

    /* Trap damage */
    room_damage[5] = 8;
    room_damage[11] = 10;
    room_damage[16] = 12;
    room_damage[23] = 10;
    room_damage[28] = 14;
    room_damage[35] = 15;
    room_damage[42] = 18;
    room_damage[47] = 15;
    room_damage[53] = 20;
    room_damage[58] = 18;
    room_damage[64] = 22;
    room_damage[69] = 20;
    room_damage[74] = 25;
    room_damage[79] = 22;
    room_damage[85] = 25;
    room_damage[90] = 30;
    room_damage[95] = 28;

    /* Score bonuses */
    room_score[4] = 100;
    room_score[9] = 100;
    room_score[14] = 150;
    room_score[19] = 150;
    room_score[24] = 200;
    room_score[29] = 200;
    room_score[34] = 250;
    room_score[39] = 250;
    room_score[44] = 300;
    room_score[49] = 300;
    room_score[54] = 350;
    room_score[59] = 350;
    room_score[64] = 400;
    room_score[69] = 400;
    room_score[74] = 450;
    room_score[79] = 450;
    room_score[84] = 500;
    room_score[89] = 500;
    room_score[94] = 600;
    room_score[99] = 1000;
}

static void print_room_name(uint8_t room)
{
    uint8_t row = room / GRID_SIZE;
    uint8_t col = room % GRID_SIZE;
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(2, 0, room_names[row][col]);
}

static void print_room_desc(uint8_t room)
{
    uint8_t row = room / GRID_SIZE;
    uint8_t col = room % GRID_SIZE;
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(2, 2, room_desc1[row][col]);
    _XL_PRINT(2, 3, room_desc2[row][col]);
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

    (void)room;

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
    _XL_PRINT(3, 6, "YOU VANQUISHED THE DARK LORD");
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
    _XL_PRINT(2, 5, "A TEXT ADVENTURE OF 100 ROOMS");
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
    uint8_t i;

    room = 0;
    health = 50;
    score = 0;
    has_sword = 0;
    has_potion = 0;
    has_amulet = 0;
    has_key = 0;
    game_active = 1;
    trap_hit = 0;

    /* Reset mutable state */
    for (i = 0; i < NUM_ROOMS; i++) {
        enemy_alive[i]   = room_enemy[i];
        item_remaining[i] = room_item[i];
        score_remaining[i] = room_score[i];
    }

    /* Enter room loop */
    while (game_active) {
        _XL_CLEAR_SCREEN();

        /* Check for trap damage on entry */
        if (room_damage[room] > 0 && !trap_hit) {
            if (room_damage[room] >= health) {
                health = 0;
            } else {
                health -= room_damage[room];
            }
            trap_hit = 1;
            _XL_TOCK_SOUND();
            if (health > 0) {
                _XL_SET_TEXT_COLOR(_XL_RED);
                _XL_PRINT(2, 15, "THE TRAP HITS YOU FOR ");
                _XL_PRINTD(24, 15, 1, room_damage[room]);
                _XL_PRINT(28, 15, " HP!");
                _XL_SLEEP(1);
                _XL_CLEAR_SCREEN();
            }
        } else {
            trap_hit = 0;
        }

        if (health <= 0) break;

        /* Check mutable item state */
        if (item_remaining[room] != ITEM_NONE) {
            uint8_t item = item_remaining[room];
            item_remaining[room] = ITEM_NONE;
            _XL_TICK_SOUND();
            switch (item) {
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
        }

        /* Check mutable score bonus */
        if (score_remaining[room] > 0) {
            score += score_remaining[room];
            score_remaining[room] = 0;
            _XL_PING_SOUND();
        }

        /* Draw room */
        _XL_CLEAR_SCREEN();
        print_room_name(room);
        print_room_desc(room);
        print_exits(room);
        print_player_status(health, score, has_sword, has_potion,
                            has_amulet, has_key);
        print_instructions();

        /* Check for enemy */
        if (enemy_alive[room] != ENEMY_NONE) {
            enemy_max = get_enemy_hp(room);
            enemy_hp = enemy_max;
            print_enemy_info(enemy_alive[room], enemy_hp, enemy_max);

            /* Combat loop */
            while (enemy_hp > 0 && health > 0) {
                input = _XL_INPUT();
                if (_XL_FIRE(input)) {
                    do_fight(&health, &enemy_hp, room,
                             has_sword, has_amulet);
                    if (enemy_hp > 0 && health > 0) {
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

            if (health <= 0) break;
            if (enemy_hp <= 0) {
                enemy_alive[room] = ENEMY_NONE;
                score += 100;
                _XL_SET_TEXT_COLOR(_XL_YELLOW);
                _XL_PRINT(2, 15, "ENEMY SLAIN! +100 SCORE");
                _XL_SLEEP(1);
            }
        }

        /* Check for victory rooms */
        if (room == 99) {
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

    init_dungeon();

    while (1) {
        start_screen();
        play_game();
    }

    return 0;
}