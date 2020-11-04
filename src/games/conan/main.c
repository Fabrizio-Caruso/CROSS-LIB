#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define MAX_NAME_SIZE 12
#define NUM_OF_CHARACTERS 3

#define NUM_OF_STATIC_STATS 2
#define RACE 0
#define CLASS 1


#define NUM_OF_DYNAMIC_STATS 8
#define LIFE         (NUM_OF_STATIC_STATS+0)
#define STRENGTH     (NUM_OF_STATIC_STATS+1)
#define DEXTERITY    (NUM_OF_STATIC_STATS+2)
#define CHARISMA     (NUM_OF_STATIC_STATS+3)
#define INTELLIGENCE (NUM_OF_STATIC_STATS+4)
#define STAMINA      (NUM_OF_STATIC_STATS+5)
#define MANA         (NUM_OF_STATIC_STATS+6)
#define GOLD         (NUM_OF_STATIC_STATS+7)

#define NUM_OF_STATS (NUM_OF_STATIC_STATS + NUM_OF_DYNAMIC_STATS)

#define NUM_OF_RACES 3
#define HUMAN 0
#define ORC 1
#define ELF 2

#define NUM_OF_CLASSES 5
#define NONE 0
#define WARRIOR 1
#define WIZARD 2
#define BARD 3
#define ASSASSIN 4

#define LEADER 0
#define MAX_PLAYER_PARTY_SIZE 6
#define MAX_ENEMY_PARTY_SIZE 12

#define set_stat(_character_ptr, _stat_index, _value)  (_character_ptr)->stat[_stat_index] =  _value

#define set_character_name(_character_ptr, _name)  strcpy((_character_ptr)->name,_name)

#define get_stat(_character_ptr, _stat_index) (_character_ptr->stat[_stat_index])

#define get_life(_character_ptr) get_stat(_character_ptr,LIFE)
#define get_strength(_character_ptr) get_stat(_character_ptr,STRENGTH)
#define get_dexterity(_character_ptr) get_stat(_character_ptr,DEXTERITY)
#define get_stamina(_character_ptr) get_stat(_character_ptr,STAMINA)


#define increase_stat(_character_ptr, _stat_index, _value)  ((_character_ptr)->stat[_stat_index])+=_value;

#define decrease_stat(_character_ptr, _stat_index, _value)  ((_character_ptr)->stat[_stat_index])-=_value;

#define increase_stamina(_character_ptr, _value) increase_stat(_character_ptr, STAMINA, _value)
#define decrease_stamina(_character_ptr, _value) decrease_stat(_character_ptr, STAMINA, _value)


#define CHARACTER_NAMES \
{ \
    "Conan", \
    "Ulrik", \
    "Sheewa" \
}

#define CONAN    0
#define ULRIK    1
#define SHEEWA   2

char *stats_names[NUM_OF_STATS] = {
//                    RACE     CLASS       LIFE     STRENGTH     DEXTERITY     CHARSIMA      INTELL          STAMINA      MANA     GOLD
                     "race",   "class",    "life",  "strength",  "dexterity",  "charisma",  "intelligence",  "stamina",   "mana",  "gold"};
#define CONAN_STATS  {HUMAN,    NONE,       50,      25,           55,           10,           40,               5,         10,       10}
#define ULRIK_STATS  {ORC,      WARRIOR,    30,      15,           10,            5,           30,              20,         20,        5}
#define SHEEWA_STATS {ELF,      ASSASSIN,   20,      10,           20,            2,           60,              40,         40,       30}

#define VERBOSE_OFF 0
#define VERBOSE_ON  1


char *race_names[NUM_OF_RACES] = {
    "human", "orc", "elf"
};

char *class_names[NUM_OF_CLASSES] = {
    "none", "warrior", "wizard", "bard", "assassin"
};






struct CharacterStruct 
{
    uint8_t stat[NUM_OF_STATS];
    
    char name[MAX_NAME_SIZE];
};
typedef struct CharacterStruct Character;


Character *player_party[MAX_PLAYER_PARTY_SIZE];


uint8_t player_party_size;

Character *enemy_party[MAX_ENEMY_PARTY_SIZE];

uint8_t enemy_party_size;


Character characters[NUM_OF_CHARACTERS];

Character* conan_ptr = &characters[CONAN];
Character* ulrik_ptr = &characters[ULRIK];
Character* sheewa_ptr = &characters[SHEEWA];


char *characters_names[NUM_OF_CHARACTERS] = CHARACTER_NAMES;



uint8_t characters_stats[NUM_OF_CHARACTERS][NUM_OF_STATS] =
{
    CONAN_STATS, ULRIK_STATS, SHEEWA_STATS
};



void initNames(void)
{
    uint8_t i;
    
    for(i=0;i<NUM_OF_CHARACTERS;++i)
    {
        memcpy(&characters[i].name,characters_names[i],MAX_NAME_SIZE);
    }
}


void initFeatures(void)
{
    uint8_t char_index;
    uint8_t stat_index;
    
    for(char_index=0;char_index<NUM_OF_CHARACTERS;++char_index)
    {
        for(stat_index=0;stat_index<NUM_OF_STATS;++stat_index)
        {
            set_stat(&characters[char_index], stat_index, characters_stats[char_index][stat_index]);
        }
    }
}


void initCharacters(void)
{
    initNames();
    initFeatures();
}


void showCharacter (const Character* character_ptr)
{
    uint8_t stat_index;
    
    printf("\n");
    printf("Name: %s\n", character_ptr->name);
    

    printf("%s: %s\n", stats_names[RACE], race_names[get_stat(character_ptr,RACE)]);
    printf("%s: %s\n", stats_names[CLASS], class_names[get_stat(character_ptr,CLASS)]);

    
    for(stat_index=NUM_OF_STATIC_STATS; stat_index<NUM_OF_STATS; ++stat_index)
    {
        printf("%s: %u\n", stats_names[stat_index], get_stat(character_ptr,stat_index));
    }
    printf("\n");
}


void showCharacters(void)
{
    uint8_t i;
    
    for(i=0;i<NUM_OF_CHARACTERS;++i)
    {
        showCharacter(&characters[i]);
    }
}

void blow(Character *character_ptr, uint8_t value)
{
    if(get_life(character_ptr)>value)
    {
        character_ptr->stat[LIFE]-=value;
    }
    else
    {
        set_stat(character_ptr,LIFE,0);
    }
}

#define STAMINA_RECHARGE 10
#define LOW_STAMINA_THRESHOLD 10
#define low_stamina(stamina) (stamina<LOW_STAMINA_THRESHOLD)

#define ATTACK_FACTOR_ADVANTAGE 2

uint8_t fight_stat(uint8_t stat_value, uint8_t stamina)
{
    return stat_value/(1+(rand()&3))/(1+low_stamina(stamina));
}

uint8_t attack(Character *attacker_ptr, Character* defender_ptr)
{
    uint8_t attacker_stamina = get_stat(attacker_ptr,STAMINA);
    uint8_t blow_hits;
    
    if(attacker_stamina)
    {
        if (ATTACK_FACTOR_ADVANTAGE*fight_stat(get_stat(attacker_ptr,DEXTERITY), attacker_stamina) >
            fight_stat(get_stat(defender_ptr,DEXTERITY), get_stat(defender_ptr,STAMINA)))
        {
            blow_hits = fight_stat(get_stat(attacker_ptr,STRENGTH), attacker_stamina);
            blow(defender_ptr, blow_hits);
        }
        else{
            blow_hits = 0;
        }
        decrease_stamina(attacker_ptr,1);
    }
    else
    {
        increase_stamina(attacker_ptr,STAMINA_RECHARGE);
    }

    return blow_hits;
}

void attack_string(Character *attacker, Character *defender)
{
    uint8_t attack_force = attack(attacker, defender);
    if(attack_force)
    {
        printf("\n%s attacks \nand hits %s \nwith force = %d\n", attacker->name, defender->name, attack_force);
    }
    else
    {
        printf("\n%s attacks \nbut %s \nfends off the attack\n", attacker->name, defender->name);
    }
    printf("\n");
}


void fight_round(Character* first_ptr, Character* second_ptr, uint8_t verbose)
{

    if(verbose)
    {
        attack_string(first_ptr, second_ptr);
    }
    else
    {
        attack(first_ptr, second_ptr);
    }
    if(verbose && get_life(second_ptr))
    {
        attack_string(second_ptr, first_ptr);
    }
    else
    {
        attack(second_ptr, first_ptr);
    }
}



void many_vs_one_fight(Character *group_ptr, uint8_t enemy_number, Character *single_ptr)
{
    uint8_t i;
    
    for(i=0;i<enemy_number;++i)
    {
        if(get_life(group_ptr) && get_life(single_ptr))
        {
            fight_round(single_ptr, group_ptr, VERBOSE_ON);
        }
        else
        {
            break;
        }
        group_ptr +=1;
    }
}


void party_fight(void)
{
    uint8_t min_size = MIN(player_party_size, enemy_party_size);
    uint8_t i;
    uint8_t round = 0;
    
    Character* player_ptr = player_party[LEADER];
    Character* enemy_ptr = enemy_party[LEADER];
    
    while (get_life(player_ptr) && get_life(enemy_ptr))
    {
        printf("\n\n\n------------------\n");
        printf("round %d\n", ++round);
        printf("------------------\n");
        getchar();
        showCharacter(player_ptr);
        getchar();
        showCharacter(enemy_ptr);
        getchar();
        
        printf("\nFight!\n");
        getchar();
        
        fight_round(player_party[LEADER],enemy_party[LEADER], VERBOSE_ON);
        
        if((get_life(player_ptr)) && (get_life(enemy_ptr)))
        {
            for(i=1;i<min_size;++i)
            {
                Character* player_party_member_ptr = player_party[i * get_life(player_party[i])];
                Character* enemy_party_member_ptr  = enemy_party[i * get_life(enemy_party[i])];
                uint8_t verbose = get_life(player_party[i]) * get_life(enemy_party[i]);
                
                fight_round(player_party_member_ptr, enemy_party_member_ptr, verbose);
            }
            if(player_party_size>min_size) // You have more party members
            {
                many_vs_one_fight(player_party[i], player_party_size-enemy_party_size, enemy_ptr);
            }
            else // The enemy party has more members
            {
                many_vs_one_fight(enemy_party[i], enemy_party_size - player_party_size, player_ptr);
            }
        }
    }
    
    showCharacters();
}


void initPlayerParty(void)
{
    player_party[LEADER] = conan_ptr;
    player_party_size = 1;
}

void initEnemyParty(void)
{
    enemy_party[LEADER] = ulrik_ptr;
    enemy_party[1] = sheewa_ptr;
    enemy_party_size = 2;
}

int main(void)
{

    Character *winner_ptr;
    
    initCharacters();
    
    initPlayerParty();
    initEnemyParty();
    
    getchar();
    
    party_fight();
    
    getchar();
    printf("\n\n");
    printf("-------------\n\n");
    if(get_life(player_party[LEADER]))
    {
        winner_ptr = player_party[LEADER];
    }
    else
    {
        winner_ptr = enemy_party[LEADER];
    }
    printf("\n%s wins!\n", winner_ptr->name);
    getchar();
    printf("\n\n");
    printf("-------------\n\n");
    showCharacters();

    return EXIT_SUCCESS;
}

