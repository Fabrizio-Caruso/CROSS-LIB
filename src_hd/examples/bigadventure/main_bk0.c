#include "cross_lib.h"

#define NUM_ROOMS 400
#define GRID_W 20
#define GRID_H 20
#define DIR_N 1
#define DIR_S 2
#define DIR_E 4
#define DIR_W 8
#define NUM_EXTRA_EDGES 40

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


#define MAP_RADIUS_INITIAL 2
#define MAP_RADIUS_EXTENDED 5

#define NO_DEST 0xFFFF


typedef struct {
    uint8_t region;
    uint8_t exits;
    uint16_t dest_n;
    uint16_t dest_s;
    uint16_t dest_e;
    uint16_t dest_w;
    uint8_t item;
    uint8_t monster;
    uint16_t monster_hp;
} Room;

Room rooms[NUM_ROOMS];
uint16_t player_room;
uint16_t player_hp;
uint16_t player_max_hp;
uint8_t player_weapon;
uint8_t player_shield;
uint8_t player_potions;
uint16_t player_score;
uint8_t dragon_defeated;
uint8_t game_over;
uint8_t map_radius;


const char *garden_descs[100] = {
    "A LUSH MEADOW RICH WITH WILDFLOWERS AND HUMMING BEEES",
    "AN ANCIENT OAK TREE WITH TWISTED ROOTS AND DEEP SHADOW",
    "A WINDING GARDEN PATH LINED WITH ROSES AND MOSSY STONES",
    "A SUNLIT PATCH WHERE BUTTERFLIES DRIFT AMONG THE CLOVERS",
    "A DENSE ROSE BUSH FULL OF THORNS AND FRAGRANT BLOOMS",
    "A TILLED VEGETABLE PLOT WITH RICH DARK SOIL AND SPROUTS",
    "A WORN GARDEN GATE LEADING TO A QUIET HIDDEN GROVE",
    "A WOVEN BIRD NEST PERCHED HIGH IN A WILLOW CANOPY",
    "A BROAD FIELD OF WILDFLOWERS SWAYING IN THE BREEZE",
    "A BRIGHT SPRING GARDEN BURSTING WITH NEW GREEN GROWTH",
    "A QUIET FOUNTAIN WHERE WATER TRICKLES OVER SMOOTH STONES",
    "A TWISTED ELM TREE WITH BARK PEELING IN SILVER RIBBONS",
    "A BED OF DAISIES AND CLOVER BLOOMING UNDER MORNING DEW",
    "A HEDGE ROW OF PRIVET BUSH TRIMMED IN NEAT GREEN HILLS",
    "A STONE ARCH COVERED IN IVY AND SMALL WHITE FLOWERS",
    "A POND WHERE TADPOLES SWIM AND FROGS PERCH ON LILIES",
    "A TANGLE OF FERNS AND MOSS ALONG A SHADEWALL PATH",
    "A BIRDBATH OF OLD GRANITE WITH A THIN RIM OF ALGAE",
    "A ROW OF TULIPS IN PINK AND LILAC BOWING IN WIND",
    "A DRY STREAMBED WITH PEBBLES AND PATCHES OF GRASS",
    "A WILLOW TREE WITH HANGING BRANCHES AND GOLDEN LEAVES",
    "A GARDEN WALL TOPPED WITH CROOKED WIRE AND WILD ROSE",
    "A CIRCLE OF MUSHROOMS GROWING BENEATH AN OLD LOG",
    "A BUTTERFLY RESTING ON A WILD MARIGOLD BLOOM",
    "A SPOOKY TOADSTOOL HAT ON A STUMP NEAR THE ROOTS",
    "A CLOVER FIELD BOUND BY A TANGLE OF THORN BUSH",
    "A RAINBOW OF FERNS UNFURLING AFTER THE MORNING RAIN",
    "A SNAIL CRAWLING SLOWLY ACROSS A MOSSY PATH STONE",
    "A LADYBUG PAUSED ON A CAMELLIA LEAF BY THE FENCE",
    "A GRASSHOPPER JUMPING FROM STEM TO STEM IN THE DRY",
    "A HEMLOCK TREE CASTING LONG SHADOWS ACROSS THE LAWN",
    "A WILD GRAPE VINE CLINGING TO A RUSTED GATE POST",
    "A BED OF LILIES OPENING THEIR WHITE PETS IN SUN",
    "A MUDY POTHOLE FILLED WITH RAINWATER AND LEAVES",
    "A ROW OF BIRCHES WITH PAPER-WHITE BARK AND SWAYING LIMBS",
    "A SMALL ARBUTUS BUSH BOWING UNDER ITS RED BERRIES",
    "A PATCH OF MOSS GLOWING GREEN ON THE NORTH WALL",
    "A WILDED HEDGEHOG DOZING IN THE DRY GRASS BY THE DOOR",
    "A CROOKED GARDEN STEM BENT BY WIND AND SUN",
    "A FROG CALLING LOUDLY FROM THE BASE OF THE FENCE",
    "A WORM WIGGLING THROUGH LOOSE SOIL NEAR THE ROOTS",
    "A BED OF MARGARIDES BOWING HEADS IN THE EVENING",
    "A STONE WALL BOUND WITH MOSS AND SMALL WHITE FLOWERS",
    "A DRY DITCH LINED WITH GRASS AND TINY WHITE FLOWERS",
    "A WILDED BUSH FULL OF BERRIES AND HIDDEN BUD",
    "A PATCH OF FOG CRAWLING LOW OVER THE TALL GRASS",
    "A CROOKED BRANCH BENT LOW WHERE A BIRD SLEPT",
    "A SMALL FIELD MOUSE DARTING THROUGH THE TALL GRASS",
    "A ROW OF CROOKED STAKES HOLDING UP YOUNG VINES",
    "A BED OF HIBISCUS FLAMING RED AGAINST THE DARK LEAF",
    "A WILDED PANSY BLOWING IN THE GATEWAY BREEZE",
    "A STONE TRELLIS COVERED IN WILDED CLIMBING ROSE",
    "A DRY POND EDGE WHERE FROGS AND TADPOLES MEET",
    "A BUSH OF WILD BLACKTHORN FULL OF SMALL WHITE FLOWERS",
    "A PATCH OF CLOVER GLOWING BRIGHT IN THE MORNING SUN",
    "A WILDED FENCE POST WITH PEELING BARK AND A SMALL HOLE",
    "A ROW OF YOUNG PEAR TREES BOWING WITH WHITE BLOOM",
    "A BED OF CROOKED FERNS UNFURLING IN THE SHADE",
    "A STONE PATH WORN SMOOTH BY YEARS OF QUIET STEPS",
    "A WILDED DOGROSE BUSH FULL OF THORNS AND FRAGILE BLOOM",
    "A PATCH OF WILD THYME BLOOMING PURPLE BY THE WALL",
    "A CROOKED GARDEN GATE HINGED ON RUSTED IRON PINS",
    "A BED OF WILD PEAS TRAILING UP A RUSTED WIRE",
    "A SMALL PUDDLE WHERE A DRAGONFLY LANDS AND FLIES",
    "A WILDED HEDGEHOG NEST BUILT OF DRY GRASS AND MOSS",
    "A ROW OF OLD MAPLE TREES SHEDDING GOLDEN LEAVES",
    "A BED OF WILD VIOLETS BLOOMING PURPLE IN THE DAWN",
    "A PATCH OF WILD GARLIC BLOOMING SILVER IN THE FIELD",
    "A BED OF WILD LAVENDER BLOOMING PURPLE IN THE SUN",
    "A CROOKED WILLOW BRANCH DRAGGING IN THE MUD",
    "A BED OF WILD PARSLEY RUFFLED AND BRIGHT BY THE GATE",
    "A SMALL FIELD SNAIL CRAWLING SLOWLY ON THE PATH",
    "A WILDED BUSH OF WILD ROSE FULL OF THORNS AND BLOOM",
    "A BED OF WILD FENNEL BOWING HEADS IN THE WIND",
    "A BED OF WILD TANSY BLOOMING YELLOW IN THE SUN",
    "A STONE WALL TOPPED WITH WILDED ROSE AND CROOKED WIRE",
    "A BED OF WILD MINT RUFFLED AND BRIGHT BY THE FENCE",
    "A BED OF WILD SAGE BLOOMING SILVER IN THE DAWN",
    "A BED OF WILD ROSE FULL OF THORNS AND FRAGILE BLOOM",
    "A BED OF WILD CLOVER GLOWING BRIGHT IN THE MORNING SUN",
    "A BED OF WILD DAISIES BLOOMING WHITE IN THE FIELD",
    "A BED OF WILD LILIES OPENING WHITE PETS IN SUN",
    "A BED OF WILD TULIPS IN PINK AND LILAC BOWING",
    "A BED OF WILD MARIGOLDS BOWING HEADS IN THE EVENING",
    "A BED OF WILD PEAS TRAILING UP A RUSTED WIRE",
    "A BED OF WILD ROSE FULL OF THORNS AND FRAGILE BLOOM",
    "A BED OF WILD CLOVER GLOWING BRIGHT IN THE MORNING SUN",
    "A BED OF WILD DAISIES BLOOMING WHITE IN THE FIELD",
    "A BED OF WILD LILIES OPENING WHITE PETS IN SUN",
    "A BED OF WILD TULIPS IN PINK AND LILAC BOWING",
    "A BED OF WILD MARIGOLDS BOWING HEADS IN THE EVENING",
    "A BED OF WILD PEAS TRAILING UP A RUSTED WIRE",
    "A BED OF WILD ROSE FULL OF THORNS AND FRAGILE BLOOM",
    "A BED OF WILD CLOVER GLOWING BRIGHT IN THE MORNING SUN",
    "A BED OF WILD DAISIES BLOOMING WHITE IN THE FIELD",
    "A BED OF WILD LILIES OPENING WHITE PETS IN SUN",
    "A BED OF WILD TULIPS IN PINK AND LILAC BOWING",
    "A BED OF WILD MARIGOLDS BOWING HEADS IN THE EVENING",
    "A BED OF WILD PEAS TRAILING UP A RUSTED WIRE",
    "A BED OF WILD ROSE FULL OF THORNS AND FRAGILE BLOOM",
};

const char *castle_descs[100] = {
    "A COLD STONE CORRIDOR WITH MOIST WALLS AND FAINT ECHOES",
    "A GRAND HALL WHERE KNIGHTS ONCE DRANK AND SWORE OATHS",
    "A TALL TOWER ROOM WITH A NARROW WINDOW AND COLD AIR",
    "A BARRICADED DOOR BEHIND WHICH LIES A DARK PASSAGE",
    "A CLANKING ARMORY ROOM FILLED WITH RUSTED IRON BLADES",
    "A HIGH THRONE HALL WHERE A KINGCE SAT IN GOLD AND SHINE",
    "A THICK CASTLE WALL OF GRAY STONE AND OLD MORTAR",
    "A DIM GUARD ROOM WITH A BURNED OUT HEARTH AND CHAINS",
    "A CANDLELIT HALL WHERE WAX DROPS ON TOILE AND DUST",
    "A HEAVY IRON GATE SET IN A WALL OF CARVED STONE",
    "A NARROW STAIRCASE OF WORN STEPS LEADING TO THE MOAT",
    "A DUSTY SCRIPTORIUM WHERE MONKS ONCE COPIED OLD TEXTS",
    "A TALL MURDER HOLE WITH A DROPPING STONE STILL SET",
    "A COLD DUNGEON CELL WITH MOSSY STONE AND THIN AIR",
    "A BANNED ARCHIVE ROOM FULL OF MOLD-STAINED PARCHMENT",
    "A NARROW WATCHTOWER ROOM WITH A SINGLE SLIT OF LIGHT",
    "A GRAND KITCHEN HALL WITH BLACKENED POTS AND OLD ASH",
    "A RUSTED CHAIN HANGING FROM A BEAM IN THE DARK",
    "A STONE FLOOR WET WITH DROPPING WATER AND OLD MOSS",
    "A TALL GATEHOUSE ARCH WITH CARVED STONE AND COLD DRAFT",
    "A BARRACKS ROOM WITH BENT STRAWS AND OLD CAMP BEDS",
    "A COLD CHAPEL WHERE THE ALTAR HOLDS A CRACKED CANDLE",
    "A MURDER HOLE CORRIDOR WITH A PORTCULLIS STILL RAISED",
    "A STONE CELL WHERE A PRISONER ONCE SCRATCHED HIS NAME",
    "A RUSTED HINGE GATE WITH A SINGLE BOLT AND OLD KEY",
    "A TALL FLAGPOLE MOUNTED ON THE BATTLEMENTS IN WIND",
    "A DUSTY GRANARY ROOM WITH OLD SACKS AND DRY GRAIN",
    "A COLD STAIRWELL WHERE THE ECHO DROPS LIKE A STONE",
    "A BANNED TOWER ROOM WITH A NARROW BED AND OLD QUILT",
    "A STONE FLOOR WHERE A BONE STILL LIES HALF BURIED",
    "A RUSTED BARRICADED DOOR WITH A SINGLE IRON BOLT",
    "A COLD WATCH ROOM WITH A BURNED OUT CANDLE AND DUST",
    "A TALL ARROW SLIT LETTING IN A THIN LINE OF DAY",
    "A DUSTY STOREROOM WITH OLD ROPE AND TANGLED COILS",
    "A STONE WALL WHERE A BANNER HANGS FADED AND THIN",
    "A RUSTED MACE LEANING AGAINST A COLD STONE WALL",
    "A COLD INFIRMARY ROOM WITH OLD BEDS AND BURNED LINEN",
    "A TALL CRENELLATION WALL WITH A SINGLE HOLED STONE",
    "A DUSTY LIBRARY ROOM WITH SHELVES OF CRACKED SPINES",
    "A STONE FLOOR WHERE OLD BLOOD STILL STAINS THE GRAY",
    "A RUSTED HALBERD STANDING IN A CORNER OF THE HALL",
    "A COLD BATH ROOM WITH A STONE BASIN AND OLD LIME",
    "A TALL BARRICADED DOOR WITH A SINGLE IRON BOLT",
    "A DUSTY KITCHEN WITH OLD POTS AND BLACKENED ASH",
    "A STONE WALL WHERE A HERALDIC SHIELD HANGS FADED",
    "A RUSTED SWORD LEANING AGAINST A COLD STONE PILLAR",
    "A COLD SCULLERY ROOM WITH OLD BOWLS AND DRY LIME",
    "A TALL DRAWBRIDGE MECHANISM WITH RUSTED CHAIN AND WIRE",
    "A DUSTY ARMORY ROOM WITH RUSTED SPEARS AND OLD LAMBS",
    "A STONE FLOOR WHERE A BONE STILL LIES HALF BURIED",
    "A RUSTED SHIELD LEANING AGAINST A COLD STONE WALL",
    "A COLD GUARD ROOM WITH A BURNED OUT HEARTH AND DUST",
    "A TALL MACHICOLATION HOLE WITH A DROPPING STONE",
    "A DUSTY SCRIPTORIUM WHERE MONKS ONCE COPIED OLD TEXTS",
    "A STONE FLOOR WET WITH DROPPING WATER AND OLD MOSS",
    "A RUSTED CHAIN HANGING FROM A BEAM IN THE DARK",
    "A COLD DUNGEON CELL WITH MOSSY STONE AND THIN AIR",
    "A TALL GATEHOUSE ARCH WITH CARVED STONE AND COLD DRAFT",
    "A DUSTY BARRACKS ROOM WITH BENT STRAWS AND OLD BEDS",
    "A STONE FLOOR WHERE A BONE STILL LIES HALF BURIED",
    "A RUSTED BOLT GATE WITH A SINGLE IRON PIN AND DUST",
    "A COLD CHAPEL WHERE THE ALTAR HOLDS A CRACKED CANDLE",
    "A TALL WATCHTOWER ROOM WITH A SINGLE SLIT OF LIGHT",
    "A DUSTY GRANARY ROOM WITH OLD SACKS AND DRY GRAIN",
    "A STONE FLOOR WET WITH DROPPING WATER AND OLD MOSS",
    "A RUSTED HINGE GATE WITH A SINGLE BOLT AND OLD KEY",
    "A COLD WATCH ROOM WITH A BURNED OUT CANDLE AND DUST",
    "A TALL ARROW SLIT LETTING IN A THIN LINE OF DAY",
    "A DUSTY STOREROOM WITH OLD ROPE AND TANGLED COILS",
    "A STONE WALL WHERE A BANNER HANGS FADED AND THIN",
    "A RUSTED MACE LEANING AGAINST A COLD STONE WALL",
    "A COLD INFIRMARY ROOM WITH OLD BEDS AND BURNED LINEN",
    "A TALL CRENELLATION WALL WITH A SINGLE HOLED STONE",
    "A DUSTY LIBRARY ROOM WITH SHELVES OF CRACKED SPINES",
    "A STONE FLOOR WHERE OLD BLOOD STILL STAINS THE GRAY",
    "A RUSTED HALBERD STANDING IN A CORNER OF THE HALL",
    "A COLD BATH ROOM WITH A STONE BASIN AND OLD LIME",
    "A TALL BARRICADED DOOR WITH A SINGLE IRON BOLT",
    "A DUSTY KITCHEN WITH OLD POTS AND BLACKENED ASH",
    "A STONE WALL WHERE A HERALDIC SHIELD HANGS FADED",
    "A RUSTED SWORD LEANING AGAINST A COLD STONE PILLAR",
    "A COLD SCULLERY ROOM WITH OLD BOWLS AND DRY LIME",
    "A TALL DRAWBRIDGE MECHANISM WITH RUSTED CHAIN AND WIRE",
    "A DUSTY ARMORY ROOM WITH RUSTED SPEARS AND OLD LAMBS",
    "A STONE FLOOR WHERE A BONE STILL LIES HALF BURIED",
    "A RUSTED SHIELD LEANING AGAINST A COLD STONE WALL",
    "A COLD GUARD ROOM WITH A BURNED OUT HEARTH AND DUST",
    "A TALL MACHICOLATION HOLE WITH A DROPPING STONE",
    "A DUSTY SCRIPTORIUM WHERE MONKS ONCE COPIED OLD TEXTS",
    "A STONE FLOOR WET WITH DROPPING WATER AND OLD MOSS",
    "A RUSTED CHAIN HANGING FROM A BEAM IN THE DARK",
    "A COLD DUNGEON CELL WITH MOSSY STONE AND THIN AIR",
    "A TALL GATEHOUSE ARCH WITH CARVED STONE AND COLD DRAFT",
    "A DUSTY BARRACKS ROOM WITH BENT STRAWS AND OLD BEDS",
    "A STONE FLOOR WHERE A BONE STILL LIES HALF BURIED",
    "A RUSTED BOLT GATE WITH A SINGLE IRON PIN AND DUST",
    "A COLD CHAPEL WHERE THE ALTAR HOLDS A CRACKED CANDLE",
    "A TALL WATCHTOWER ROOM WITH A SINGLE SLIT OF LIGHT",
    "A DUSTY GRANARY ROOM WITH OLD SACKS AND DRY GRAIN",
    "A STONE FLOOR WET WITH DROPPING WATER AND OLD MOSS"
};

const char *village_descs[100] = {
    "A SMOKEY BREAD SHOP WITH WARM LOAVES ON A WOODEN SHELF",
    "A HOT BLACKSMITH FORGE WHERE HAMMERING ECHOES ALL DAY",
    "A STONY VILLAGE SQUARE WHERE CARRIERS AND CHILDREN MIX",
    "A TILTED FARM HOUSE WITH A LEAKY ROOF AND A DOG",
    "A WORN TANNERY STONE WHERE LEATHER IS SOWN AND DRIED",
    "A BUTCHERS STALL WITH BLOODED KNIVES AND COLD MEAT",
    "A DUSTY VILLAGE PATH WHERE MULES AND CARRIERS PASS",
    "A COLD WATER WELL WHERE VILLAGERS DRAW AT DAWN",
    "A Lively MARKET PLACE WHERE MERCHANTS SHOUT AND HAGGLE",
    "A QUIET COTTAGE YARD WITH A CAT AND A SLEEPING GOAT",
    "A WARM CHEESE SHOP WITH WHEELS OF HARD GOAT CHEESE",
    "A NOISY CARPENTERS SHOP WITH SAWDUST AND OLD PLANE",
    "A DRY TANNER YARD WHERE HIDES HANG ON WOODEN PEG",
    "A BAKERS OVEN ROOM WITH SOOT AND OLD BREAD CRUST",
    "A COLD COOPER SHOP WITH OLD BARRELS AND WET STAVE",
    "A WORN CORDWAINER BENCH WITH OLD LEATHER AND AWL",
    "A STONY FISH STALL WHERE SCALES STILL GLINT IN THE DRY",
    "A HOT WEAVER LOOM WHERE THREADS CROSS AND REPEAT",
    "A DUSTY POTTERS YARD WITH WET CLAY AND OLD WHEELS",
    "A COLD MILL ROOM WHERE GRAIN STILL GRINDS IN THE DARK",
    "A WARM DYE WORKS WHERE VATS BUBBLE IN RED AND BLUE",
    "A NOISY FALCONER YARD WHERE HAWKS PERCH ON WOODEN PERCH",
    "A DRY GLOVEMAKER BENCH WITH OLD HIDE AND STITCHED THREAD",
    "A COLD CHANDLER ROOM WITH OLD TALLOW AND WICKED WAX",
    "A WORN SADDLER SHOP WITH OLD LEATHER AND BRONZE BUCKLE",
    "A HOT TINSMITH FORGE WHERE TIN STILL RINGS IN THE AIR",
    "A DUSTY WHEELWRIGHT YARD WITH OLD HUBS AND WET RIM",
    "A COLD GREENGROCER STALL WHERE APPLES AND CARROTS MIX",
    "A WARM FULLER YARD WHERE WOOL STILL BUBBLES IN THE TUB",
    "A NOISY SCABORD BENCH WITH OLD BUCKLE AND RUSTED PIN",
    "A DRY POTION SELLER STALL WHERE VIALS CLINK IN THE DRY",
    "A COLD BUTCHERS BACK ROOM WHERE BONE STILL STAINS THE HOOK",
    "A WORN APOTHECARY SHELF WHERE VIALS AND POWDERS DUST",
    "A HOT MILL STONE ROOM WHERE FLOUR STILL CLOUDS THE AIR",
    "A DUSTY SCABORD BENCH WITH OLD BUCKLE AND RUSTED PIN",
    "A COLD FALCONER PERCH WHERE A HAWK STILL RINGS THE YARD",
    "A WARM BAKERS BACK ROOM WHERE DOUGH STILL RISES IN THE TUB",
    "A NOISY COOPER BENCH WHERE STAVES STILL BEND AND CURVE",
    "A DRY CARPENTER YARD WHERE SAWDUST STILL CLOUDS THE FLOOR",
    "A COLD TANNER BACK ROOM WHERE LIME STILL STAINS THE HIDE",
    "A WORN MILL BACK ROOM WHERE GRAIN STILL GRINDS IN THE DARK",
    "A HOT DYE WORKS BACK ROOM WHERE VATS STILL BUBBLE IN RED",
    "A DUSTY FALCONER YARD WHERE FEATHERS STILL DUST THE PERCH",
    "A COLD GLOVEMAKER BENCH WHERE HIDE STILL DRY AND CRACKS",
    "A WARM CHANDLER ROOM WHERE WICK STILL BURNS IN THE DARK",
    "A NOISY SADDLER SHOP WHERE LEATHER STILL CREAKS ON THE BENCH",
    "A DRY TINSMITH FORGE WHERE TIN STILL RINGS IN THE AIR",
    "A COLD WHEELWRIGHT YARD WHERE HUBS STILL WET AND WORN",
    "A WORN GREENGROCER STALL WHERE APPLES STILL ROT IN THE DRY",
    "A HOT FULLER YARD WHERE WOOL STILL BUBBLES IN THE TUB",
    "A DUSTY SCABORD BENCH WHERE BUCKLE STILL RUSTS ON THE PIN",
    "A COLD POTION SELLER STALL WHERE VIALS STILL CLINK IN THE DRY",
    "A WORN BUTCHERS BACK ROOM WHERE BONE STILL STAINS THE HOOK",
    "A NOISY APOTHECARY SHELF WHERE POWDER STILL DUSTS THE VIAL",
    "A DRY MILL STONE ROOM WHERE FLOUR STILL CLOUDS THE AIR",
    "A COLD SCABORD BENCH WHERE BUCKLE STILL RUSTS ON THE PIN",
    "A WARM FALCONER PERCH WHERE A HAWK STILL RINGS THE YARD",
    "A NOISY BAKERS BACK ROOM WHERE DOUGH STILL RISES IN THE TUB",
    "A DRY COOPER BENCH WHERE STAVES STILL BEND AND CURVE",
    "A COLD CARPENTER YARD WHERE SAWDUST STILL CLOUDS THE FLOOR",
    "A WORN TANNER BACK ROOM WHERE LIME STILL STAINS THE HIDE",
    "A HOT MILL BACK ROOM WHERE GRAIN STILL GRINDS IN THE DARK",
    "A DUSTY DYE WORKS BACK ROOM WHERE VATS STILL BUBBLE IN RED",
    "A COLD FALCONER YARD WHERE FEATHERS STILL DUST THE PERCH",
    "A WORN GLOVEMAKER BENCH WHERE HIDE STILL DRY AND CRACKS",
    "A NOISY CHANDLER ROOM WHERE WICK STILL BURNS IN THE DARK",
    "A DRY SADDLER SHOP WHERE LEATHER STILL CREAKS ON THE BENCH",
    "A COLD TINSMITH FORGE WHERE TIN STILL RINGS IN THE AIR",
    "A WORN WHEELWRIGHT YARD WHERE HUBS STILL WET AND WORN",
    "A HOT GREENGROCER STALL WHERE APPLES STILL ROT IN THE DRY",
    "A DUSTY FULLER YARD WHERE WOOL STILL BUBBLES IN THE TUB",
    "A COLD SCABORD BENCH WHERE BUCKLE STILL RUSTS ON THE PIN",
    "A WORN POTION SELLER STALL WHERE VIALS STILL CLINK IN THE DRY",
    "A NOISY BUTCHERS BACK ROOM WHERE BONE STILL STAINS THE HOOK",
    "A DRY APOTHECARY SHELF WHERE POWDER STILL DUSTS THE VIAL",
    "A COLD MILL STONE ROOM WHERE FLOUR STILL CLOUDS THE AIR",
    "A WORN SCABORD BENCH WHERE BUCKLE STILL RUSTS ON THE PIN",
    "A NOISY FALCONER PERCH WHERE A HAWK STILL RINGS THE YARD",
    "A DRY BAKERS BACK ROOM WHERE DOUGH STILL RISES IN THE TUB",
    "A COLD COOPER BENCH WHERE STAVES STILL BEND AND CURVE",
    "A WORN CARPENTER YARD WHERE SAWDUST STILL CLOUDS THE FLOOR",
    "A HOT TANNER BACK ROOM WHERE LIME STILL STAINS THE HIDE",
    "A DUSTY MILL BACK ROOM WHERE GRAIN STILL GRINDS IN THE DARK",
    "A COLD DYE WORKS BACK ROOM WHERE VATS STILL BUBBLE IN RED",
    "A WORN FALCONER YARD WHERE FEATHERS STILL DUST THE PERCH",
    "A NOISY GLOVEMAKER BENCH WHERE HIDE STILL DRY AND CRACKS",
    "A DRY CHANDLER ROOM WHERE WICK STILL BURNS IN THE DARK",
    "A COLD SADDLER SHOP WHERE LEATHER STILL CREAKS ON THE BENCH",
    "A WORN TINSMITH FORGE WHERE TIN STILL RINGS IN THE AIR",
    "A NOISY WHEELWRIGHT YARD WHERE HUBS STILL WET AND WORN",
    "A DRY GREENGROCER STALL WHERE APPLES STILL ROT IN THE DRY",
    "A COLD FULLER YARD WHERE WOOL STILL BUBBLES IN THE TUB",
    "A WORN FALCONER YARD WHERE FEATHERS STILL DUST THE PERCH",
    "A NOISY GLOVEMAKER BENCH WHERE HIDE STILL DRY AND CRACKS",
    "A DRY CHANDLER ROOM WHERE WICK STILL BURNS IN THE DARK",
    "A COLD SADDLER SHOP WHERE LEATHER STILL CREAKS ON THE BENCH",
    "A WORN TINSMITH FORGE WHERE TIN STILL RINGS IN THE AIR",
    "A NOISY WHEELWRIGHT YARD WHERE HUBS STILL WET AND WORN",
    "A DRY GREENGROCER STALL WHERE APPLES STILL ROT IN THE DRY",
    "A COLD FULLER YARD WHERE WOOL STILL BUBBLES IN THE TUB"
};

const char *riverside_descs[100] = {
    "A MUDY RIVER BANK WHERE FISH JUMP AND REEDS SWAY",
    "A QUIET FISHING SPOT WITH A LINE CAST INTO THE FLOW",
    "A BENDING WILLOW TREE DRIPPING INTO THE COOL WATER",
    "A ROCKY RIVER CROSSING WHERE STONES ARE SLIPPERY AND WET",
    "A GRITTY ROCKY SHORE WHERE TIDES LICK AND PULL",
    "A NARROW CANAL BRIDGE WHERE WATER SLOSHES BELOW",
    "A DRY RIVERBED CRACKED AND BAKED UNDER THE SUN",
    "A ROARING WATERFALL EDGE WHERE MIST AND LIGHT MIX",
    "A STICKY MUD FLAT WHERE HERONS STAND AND WAIT",
    "A WOODEN RIVER DOCK WHERE BOATS CREAK AND SWAY",
    "A MUDY SHALLOWS WHERE TADPOLES DART AND FROGS CALL",
    "A ROCKY EDDY WHERE WATER SPINS AND STONES ROLL",
    "A WILDED REED BANK WHERE HERONS FISH AND WAIT",
    "A NARROW SAND BAR WHERE CRABS SNAP AND HIDE",
    "A DRY MUD FLAT WHERE BARNACLES CLING AND PEEL",
    "A ROCKY WEIR WHERE FISH BOW AND JUMP IN THE COLD",
    "A MUDY PIER WHERE MOORING ROPES CREAK AND SAG",
    "A WILDED WILLOW BANK WHERE BRANCHES DRAG IN THE MUD",
    "A ROCKY SLUICE GATE WHERE WATER FORCES AND FOAM",
    "A DRY EMBANKMENT WHERE SILT STILL BAKES IN THE SUN",
    "A MUDY LOCK WHERE GATES CREAK AND WATER DRAINS",
    "A ROCKY QUAY WHERE BOATS CREAK AND PILINGS RUST",
    "A WILDED HERON PERCH WHERE BIRDS WAIT AND FISH",
    "A NARROW FOG BANK WHERE MIST CRAWLS OVER THE FLOW",
    "A DRY TIDE LINE WHERE ALGAE DRY AND CRACK",
    "A ROCKY MURCHON POST WHERE SEAPLANT CLING AND BEND",
    "A MUDY BARNACLE FLAT WHERE SHELLS STICK AND DRY",
    "A WILDED KELP BED WHERE KELP STILL SWAYS IN THE DRY",
    "A ROCKY MUSSEL ROCK WHERE MUSSELS STILL CLING AND DRY",
    "A DRY OYSTER FLAT WHERE OYSTERS STILL DRY AND CRACK",
    "A MUDY CRAB HOLE WHERE CRABS STILL SNAP AND HIDE",
    "A ROCKY LOBSTER POT WHERE ROPE STILL TANGLES AND RUSTS",
    "A WILDED EEL BED WHERE EELS STILL SLIDE AND SLIP",
    "A NARROW PIKE HOLE WHERE PIKE STILL LURK AND STRIKE",
    "A DRY PERCH ROCK WHERE PERCH STILL JUMP AND GLINT",
    "A ROCKY CARP POND WHERE CARP STILL BOW AND BUBBLE",
    "A MUDY TROUT BED WHERE TROUT STILL SLIDE AND DASH",
    "A WILDED SALMON RUN WHERE SALMON STILL BOW AND JUMP",
    "A ROCKY STURGEON HOLE WHERE STURGEON STILL LURK AND WAIT",
    "A DRY BARNACLE ROCK WHERE BARNACLES STILL CLING AND DRY",
    "A MUDY MUSSEL BED WHERE MUSSELS STILL CLING AND DRY",
    "A WILDED OYSTER FLAT WHERE OYSTERS STILL DRY AND CRACK",
    "A ROCKY CRAB HOLE WHERE CRABS STILL SNAP AND HIDE",
    "A DRY LOBSTER POT WHERE ROPE STILL TANGLES AND RUSTS",
    "A MUDY EEL BED WHERE EELS STILL SLIDE AND SLIP",
    "A WILDED PIKE HOLE WHERE PIKE STILL LURK AND STRIKE",
    "A ROCKY PERCH ROCK WHERE PERCH STILL JUMP AND GLINT",
    "A DRY CARP POND WHERE CARP STILL BOW AND BUBBLE",
    "A MUDY TROUT BED WHERE TROUT STILL SLIDE AND DASH",
    "A WILDED SALMON RUN WHERE SALMON STILL BOW AND JUMP",
    "A ROCKY STURGEON HOLE WHERE STURGEON STILL LURK AND WAIT",
    "A DRY BARNACLE ROCK WHERE BARNACLES STILL CLING AND DRY",
    "A MUDY MUSSEL BED WHERE MUSSELS STILL CLING AND DRY",
    "A WILDED OYSTER FLAT WHERE OYSTERS STILL DRY AND CRACK",
    "A ROCKY CRAB HOLE WHERE CRABS STILL SNAP AND HIDE",
    "A DRY LOBSTER POT WHERE ROPE STILL TANGLES AND RUSTS",
    "A MUDY EEL BED WHERE EELS STILL SLIDE AND SLIP",
    "A WILDED PIKE HOLE WHERE PIKE STILL LURK AND STRIKE",
    "A ROCKY PERCH ROCK WHERE PERCH STILL JUMP AND GLINT",
    "A DRY CARP POND WHERE CARP STILL BOW AND BUBBLE",
    "A MUDY TROUT BED WHERE TROUT STILL SLIDE AND DASH",
    "A WILDED SALMON RUN WHERE SALMON STILL BOW AND JUMP",
    "A ROCKY STURGEON HOLE WHERE STURGEON STILL LURK AND WAIT",
    "A DRY BARNACLE ROCK WHERE BARNACLES STILL CLING AND DRY",
    "A MUDY MUSSEL BED WHERE MUSSELS STILL CLING AND DRY",
    "A WILDED OYSTER FLAT WHERE OYSTERS STILL DRY AND CRACK",
    "A ROCKY CRAB HOLE WHERE CRABS STILL SNAP AND HIDE",
    "A DRY LOBSTER POT WHERE ROPE STILL TANGLES AND RUSTS",
    "A MUDY EEL BED WHERE EELS STILL SLIDE AND SLIP",
    "A WILDED PIKE HOLE WHERE PIKE STILL LURK AND STRIKE",
    "A ROCKY PERCH ROCK WHERE PERCH STILL JUMP AND GLINT",
    "A DRY CARP POND WHERE CARP STILL BOW AND BUBBLE",
    "A MUDY TROUT BED WHERE TROUT STILL SLIDE AND DASH",
    "A WILDED SALMON RUN WHERE SALMON STILL BOW AND JUMP",
    "A ROCKY STURGEON HOLE WHERE STURGEON STILL LURK AND WAIT",
    "A DRY BARNACLE ROCK WHERE BARNACLES STILL CLING AND DRY",
    "A MUDY MUSSEL BED WHERE MUSSELS STILL CLING AND DRY",
    "A WILDED OYSTER FLAT WHERE OYSTERS STILL DRY AND CRACK",
    "A ROCKY CRAB HOLE WHERE CRABS STILL SNAP AND HIDE",
    "A DRY LOBSTER POT WHERE ROPE STILL TANGLES AND RUSTS",
    "A MUDY EEL BED WHERE EELS STILL SLIDE AND SLIP",
    "A WILDED PIKE HOLE WHERE PIKE STILL LURK AND STRIKE",
    "A ROCKY PERCH ROCK WHERE PERCH STILL JUMP AND GLINT",
    "A DRY CARP POND WHERE CARP STILL BOW AND BUBBLE",
    "A MUDY TROUT BED WHERE TROUT STILL SLIDE AND DASH",
    "A WILDED SALMON RUN WHERE SALMON STILL BOW AND JUMP",
    "A ROCKY STURGEON HOLE WHERE STURGEON STILL LURK AND WAIT",
    "A DRY BARNACLE ROCK WHERE BARNACLES STILL CLING AND DRY",
    "A MUDY MUSSEL BED WHERE MUSSELS STILL CLING AND DRY",
    "A WILDED OYSTER FLAT WHERE OYSTERS STILL DRY AND CRACK",
    "A ROCKY CRAB HOLE WHERE CRABS STILL SNAP AND HIDE",
    "A DRY LOBSTER POT WHERE ROPE STILL TANGLES AND RUSTS",
    "A MUDY EEL BED WHERE EELS STILL SLIDE AND SLIP",
    "A WILDED PIKE HOLE WHERE PIKE STILL LURK AND STRIKE",
    "A ROCKY PERCH ROCK WHERE PERCH STILL JUMP AND GLINT",
    "A DRY CARP POND WHERE CARP STILL BOW AND BUBBLE",
    "A MUDY TROUT BED WHERE TROUT STILL SLIDE AND DASH",
    "A WILDED SALMON RUN WHERE SALMON STILL BOW AND JUMP",
    "A ROCKY STURGEON HOLE WHERE STURGEON STILL LURK AND WAIT",
    "A DRY BARNACLE ROCK WHERE BARNACLES STILL CLING AND DRY"
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

void split_description(const char *src, char *line1, char *line2) {
    int len = 0;
    while (src[len]) len++;
    int mid = len / 2;
    int i = mid;

    /* Find nearest space at or before mid */
    while (i > 0 && src[i] != ' ') i--;

    if (i == 0) {
        /* No space found left of mid, try right */
        i = mid;
        while (i < len && src[i] != ' ') i++;
        if (i >= len) i = mid;
    }

    /* Copy line1 */
    int j = 0;
    while (j < i) { line1[j] = src[j]; j++; }
    line1[j] = '\0';

    /* Skip spaces, copy line2 */
    while (src[i] == ' ') i++;
    j = 0;
    while (src[i]) { line2[j] = src[i]; i++; j++; }
    line2[j] = '\0';
}

uint8_t get_region(uint16_t room_idx) {
    uint8_t row = room_idx / GRID_W;
    uint8_t col = room_idx % GRID_W;
    if (row < 10 && col < 10) return REGION_GARDEN;
    if (row < 10 && col >= 10) return REGION_VILLAGE;
    if (row >= 10 && col < 10) return REGION_RIVERSIDE;
    return REGION_CASTLE;
}

void generate_spanning_tree(void) {
    uint8_t in_tree[NUM_ROOMS];
    uint16_t frontier[NUM_ROOMS];
    uint16_t frontier_count;
    uint16_t i;
    uint16_t idx;
    uint16_t current;
    uint8_t row;
    uint8_t col;

    for (i = 0; i < NUM_ROOMS; i++) {
        in_tree[i] = 0;
        rooms[i].exits = 0;
        rooms[i].dest_n = NO_DEST;
        rooms[i].dest_s = NO_DEST;
        rooms[i].dest_e = NO_DEST;
        rooms[i].dest_w = NO_DEST;
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
    uint16_t i;
    uint16_t a;
    uint16_t b;
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

        if (adj == DIR_N && rooms[a].dest_n == NO_DEST) {
            rooms[a].dest_n = b;
            rooms[a].exits |= DIR_N;
            rooms[b].dest_s = a;
            rooms[b].exits |= DIR_S;
        } else if (adj == DIR_S && rooms[a].dest_s == NO_DEST) {
            rooms[a].dest_s = b;
            rooms[a].exits |= DIR_S;
            rooms[b].dest_n = a;
            rooms[b].exits |= DIR_N;
        } else if (adj == DIR_E && rooms[a].dest_e == NO_DEST) {
            rooms[a].dest_e = b;
            rooms[a].exits |= DIR_E;
            rooms[b].dest_w = a;
            rooms[b].exits |= DIR_W;
        } else if (adj == DIR_W && rooms[a].dest_w == NO_DEST) {
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
    uint16_t r;
    uint8_t region;

    farthest = bfs_farthest();

    rooms[farthest].monster = MONSTER_DRAGON;
    rooms[farthest].monster_hp = MONSTER_HP_DRAGON;

    rooms[0].item = ITEM_DAGGER;

    r = _XL_RAND() % NUM_ROOMS;
    if (r == 0) r = 1;
    region = rooms[r].region;
    if (region != REGION_CASTLE) {
        r = 200 + _XL_RAND() % 200;
    }
    if (r == farthest) r = 201 + _XL_RAND() % 199;
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

    /* Place torch in the castle quadrant (bottom-right) */
    r = 300 + _XL_RAND() % 100;
    if (r == farthest) r = 300 + (_XL_RAND() % 99) + 1;
    if (rooms[r].item != ITEM_NONE) {
        r = 301 + _XL_RAND() % 99;
        if (r == farthest) r = 301;
    }
    rooms[r].item = ITEM_TORCH;

    i = 1;
    while (i < 8) {
        r = 1 + _XL_RAND() % 100;
        if (rooms[r].monster == MONSTER_NONE && r != farthest) {
            rooms[r].monster = MONSTER_GOBLIN;
            rooms[r].monster_hp = MONSTER_HP_GOBLIN;
            i++;
        }
    }

    i = 1;
    while (i < 6) {
        r = 101 + _XL_RAND() % 100;
        if (rooms[r].monster == MONSTER_NONE && r != farthest) {
            rooms[r].monster = MONSTER_SKELETON;
            rooms[r].monster_hp = MONSTER_HP_SKELETON;
            i++;
        }
    }

    i = 1;
    while (i < 4) {
        r = 201 + _XL_RAND() % 199;
        if (rooms[r].monster == MONSTER_NONE && r != farthest) {
            rooms[r].monster = MONSTER_OGRE;
            rooms[r].monster_hp = MONSTER_HP_OGRE;
            i++;
        }
    }

    i = 1;
    while (i < 3) {
        r = 201 + _XL_RAND() % 199;
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


void display_map(void) {
    uint8_t pr, pc;
    int16_t dx, dy;
    uint16_t screen_x, screen_y;
    int16_t grid_r, grid_c;
    int16_t dist_sq;
    uint8_t radius;
    uint16_t tile;
    uint16_t room_idx;
    uint8_t connected;

    pr = player_room / GRID_W;
    pc = player_room % GRID_W;

    /* Fixed screen position for the player */
    screen_x = XSize -7;
    screen_y = YSize - 7;

    /* Visibility radius: 2 without torch, 5 with torcƒh */
    radius = (map_radius == MAP_RADIUS_EXTENDED) ? MAP_RADIUS_EXTENDED : MAP_RADIUS_INITIAL;

    for (dy = -radius; dy <= radius; dy++) {
        for (dx = -radius; dx <= radius; dx++) {
            dist_sq = dx * dx + dy * dy;
            if (dist_sq > radius * radius) continue;

            /* Map screen offset to grid position */
            /* dy < 0 is north (row decreases), dy > 0 is south (row increases) */
            /* dx > 0 is east (col increases),  dx < 0 is west (col decreases) */
            grid_r = (int16_t)pr + dy;
            grid_c = (int16_t)pc + dx;

            if (grid_r < 0 || grid_r >= GRID_H || grid_c < 0 || grid_c >= GRID_W) continue;

            room_idx = (uint16_t)(grid_r * GRID_W + grid_c);

            if (dx == 0 && dy == 0) {
                /* Player tile */
                tile = _TILE_0;
            } else if ((dx == 0 && (dy == -1 || dy == 1)) || (dy == 0 && (dx == -1 || dx == 1))) {
                /* Directly adjacent tile: show exit or wall */
                connected = 0;
                if (dy < 0 && (rooms[player_room].exits & DIR_N)) connected = 1;
                if (dy > 0 && (rooms[player_room].exits & DIR_S)) connected = 1;
                if (dx > 0 && (rooms[player_room].exits & DIR_E)) connected = 1;
                if (dx < 0 && (rooms[player_room].exits & DIR_W)) connected = 1;
                tile = connected ? _TILE_1 : _TILE_2;
            } else {
                /* Further visible rooms: walkable path */
                tile = _TILE_1;
            }

            _XL_DRAW(screen_x + dx, screen_y + dy, tile, _XL_WHITE);
        }
    }
}

void display_room_info(void) {
    const char *desc;
    const char *rname;
    uint8_t region;
    uint8_t desc_idx;
    char line1[64];
    char line2[64];

    _XL_CLEAR_SCREEN();

    region = rooms[player_room].region;
    rname = region_names[region];
    desc_idx = player_room;


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

    /* Split description into two lines at a word boundary */
    split_description(desc, line1, line2);

    _XL_SET_TEXT_COLOR(_XL_MAGENTA);
    _XL_PRINT(1, 0, "TEXT ADVENTURE");

    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(1, 1, rname);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(1, 2, line1);
    _XL_PRINT(1, 3, line2);

    _XL_SET_TEXT_COLOR(_XL_BLUE);
    _XL_PRINT(1, 5, "EXITS:");
    if (rooms[player_room].exits & DIR_N) _XL_PRINT(10, 5, "N");
    if (rooms[player_room].exits & DIR_S) _XL_PRINT(14, 5, "S");
    if (rooms[player_room].exits & DIR_E) _XL_PRINT(18, 5, "E");
    if (rooms[player_room].exits & DIR_W) _XL_PRINT(22, 5, "W");

    if (rooms[player_room].item != ITEM_NONE) {
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(1, 6, "ITEM:");
        _XL_PRINT(8, 6, item_names[rooms[player_room].item]);
    }

    if (rooms[player_room].monster != MONSTER_NONE) {
        _XL_SET_TEXT_COLOR(_XL_RED);
        _XL_PRINT(1, 7, "MONSTER:");
        _XL_PRINT(12, 7, monster_names[rooms[player_room].monster]);
    }

    _XL_SET_TEXT_COLOR(_XL_GREEN);
    _XL_PRINT(1, 9, "HP:");
    _XL_PRINTD(4, 9, 3, player_hp);

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(1, 10, "WEAPON:");
    _XL_PRINT(10, 10, item_names[player_weapon]);

    if (player_shield > 0) {
        _XL_SET_TEXT_COLOR(_XL_CYAN);
        _XL_PRINT(1, 11, "SHIELD:");
        _XL_PRINT(10, 11, item_names[player_shield + 2]);
    }

    _XL_SET_TEXT_COLOR(_XL_CYAN);
    _XL_PRINT(1, 12, "POTIONS:");
    _XL_PRINTD(10, 12, 1, player_potions);

    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _XL_PRINT(1, 13, "SCORE:");
    _XL_PRINTD(8, 13, 4, player_score);

    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(1, 15, "PRESS DIRECTION TO MOVE");
    _XL_PRINT(1, 16, "PRESS F TO USE POTION");

    display_map();
}

void display_combat_screen(uint16_t room_idx) {
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

void do_combat(uint16_t room_idx) {
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

    while (rooms[room_idx].monster != MONSTER_NONE && player_hp > 0 && !fled) {
        display_combat_screen(room_idx);
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);

        any_press = 0;
        while (!any_press) {
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

void collect_item(uint16_t room_idx) {
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
    uint16_t dest;

    dest = NO_DEST;
    if (dir == 0 && (rooms[player_room].exits & DIR_N)) {
        dest = rooms[player_room].dest_n;
    } else if (dir == 1 && (rooms[player_room].exits & DIR_S)) {
        dest = rooms[player_room].dest_s;
    } else if (dir == 2 && (rooms[player_room].exits & DIR_E)) {
        dest = rooms[player_room].dest_e;
    } else if (dir == 3 && (rooms[player_room].exits & DIR_W)) {
        dest = rooms[player_room].dest_w;
    }

    if (dest != NO_DEST) {
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
        while (!any_press) {
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


/* --- New globals and lookup table --- */

uint8_t visited[NUM_ROOMS];

/*
 * room_tile[] is indexed by the exits bitmask of a room.
 *   DIR_N=1, DIR_S=2, DIR_E=4, DIR_W=8  ->  exits value 0..15
 * Each entry maps to a tile that shows which directions are open.
 */
const uint16_t room_tile[16] = {
    _TILE_15,   /* 0x0: no exits                    */
    _TILE_14,   /* 0x1: N                           */
    _TILE_13,   /* 0x2: S                           */
    _TILE_12,   /* 0x3: N+S                         */
    _TILE_11,   /* 0x4: E                           */
    _TILE_10,   /* 0x5: N+E                         */
    _TILE_9,    /* 0x6: S+E                         */
    _TILE_8,    /* 0x7: N+S+E                       */
    _TILE_7,    /* 0x8: W                           */
    _TILE_6,    /* 0x9: N+W                         */
    _TILE_5,    /* 0xA: S+W                         */
    _TILE_4,    /* 0xB: N+S+W                       */
    _TILE_3,    /* 0xC: E+W                         */
    _TILE_2,    /* 0xD: N+E+W                       */
    _TILE_1,    /* 0xE: S+E+W  (missing N)          */
    _TILE_0     /* 0xF: N+S+E+W (all directions)    */
};

/* --- Modified display_map --- */

void display_map(void) {
    uint8_t pr, pc;
    int16_t dx, dy;
    uint16_t screen_x, screen_y;
    int16_t grid_r, grid_c;
    int16_t dist_sq;
    uint8_t radius;
    uint16_t room_idx;
    uint16_t tile;

    pr = player_room / GRID_W;
    pc = player_room % GRID_W;

    /* Fixed screen anchor for the player */
    screen_x = XSize / 4;
    screen_y = YSize - 8;

    /* Visibility radius: 2 without torch, 5 with torch */
    radius = (map_radius == MAP_RADIUS_EXTENDED) ? MAP_RADIUS_EXTENDED : MAP_RADIUS_INITIAL;

    for (dy = -radius; dy <= radius; dy++) {
        for (dx = -radius; dx <= radius; dx++) {
            dist_sq = dx * dx + dy * dy;
            if (dist_sq > radius * radius) continue;

            /* Map screen offset to grid position */
            grid_r = (int16_t)pr + dy;
            grid_c = (int16_t)pc + dx;

            if (grid_r < 0 || grid_r >= GRID_H || grid_c < 0 || grid_c >= GRID_W) continue;

            room_idx = (uint16_t)(grid_r * GRID_W + grid_c);

            if (dx == 0 && dy == 0) {
                /* Player position */
                tile = _TILE_26;
            } else if (visited[room_idx]) {
                /* Visited room: tile determined by exits */
                tile = room_tile[rooms[room_idx].exits];
            } else {
                /* Unvisited room within view */
                tile = _TILE_27;
            }

            _XL_DRAW(screen_x + dx, screen_y + dy, tile, _XL_WHITE);
        }
    }
}

/* --- Mark rooms as visited when the player moves --- */

void move_player(uint8_t dir) {
    uint16_t dest;

    dest = NO_DEST;
    if (dir == 0 && (rooms[player_room].exits & DIR_N)) {
        dest = rooms[player_room].dest_n;
    } else if (dir == 1 && (rooms[player_room].exits & DIR_S)) {
        dest = rooms[player_room].dest_s;
    } else if (dir == 2 && (rooms[player_room].exits & DIR_E)) {
        dest = rooms[player_room].dest_e;
    } else if (dir == 3 && (rooms[player_room].exits & DIR_W)) {
        dest = rooms[player_room].dest_w;
    }

    if (dest != NO_DEST) {
        player_room = dest;
        visited[player_room] = 1;          /* mark new room as visited */
        if (rooms[player_room].item != ITEM_NONE) {
            collect_item(player_room);
        }
        if (rooms[player_room].monster != MONSTER_NONE) {
            do_combat(player_room);
        }
    }
}

/* --- In init_game, mark starting room as visited --- */

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
        visited[i] = 0;
    }

    visited[0] = 1;                        /* starting room is visited */

    generate_spanning_tree();
    add_extra_edges();
    place_items_and_monsters();
}

int main(void) {
    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    for (;;) {
        init_game();
        play_game();
        show_game_over();
        _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
    }

    return 0;
}
