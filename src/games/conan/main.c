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

#define MAX_PLAYER_PARTY_SIZE 6
#define MAX_ENEMY_PARTY_SIZE 12

#define set_stat(_character_ptr, _stat_index, _value)  (_character_ptr)->stat[_stat_index] =  _value

#define set_character_name(_character_ptr, _name)  strcpy((_character_ptr)->name,_name)

#define get_stat(_character_ptr, _stat_index) (_character_ptr->stat[_stat_index])

#define get_life(_character_ptr) get_stat(_character_ptr,LIFE)
#define get_strength(_character_ptr) get_stat(_character_ptr,STRENGTH)
#define get_dexterity(_character_ptr) get_stat(_character_ptr,DEXTERITY)
#define get_stamina(_character_ptr) get_stat(_character_ptr,STAMINA)


#define increase_character_stat(_character_ptr, _stat_index)  ++(character_ptr)->stat[_stat_index]

#define decrease_character_stat(_character_ptr, _stat_index)  --(character_ptr)->stat[_stat_index]

#define CHARACTER_NAMES \
{ \
    "Conan", \
    "Ulrik", \
    "Sheewa" \
}

#define CONAN    0
#define ULRIK    1
#define SHEEWA   2

//                    RACE     CLASS       LIFE    STRENGTH   DEXTERITY   CHARSIMA   INTELL     STAMINA       GOLD
#define CONAN_STATS  {HUMAN,    NONE,       50,      8,         55,         10,        40,          0,         10}
#define ULRIK_STATS  {ORC,      WARRIOR,    40,      9,         10,          5,        30,         20,         20}
#define SHEEWA_STATS {ELF,      ASSASSIN,   20,      3,         20,          2,        60,         80,         40}

#define VERBOSE_OFF 0
#define VERBOSE_ON  1


char *race_names[NUM_OF_RACES] = {
    "human", "orc", "elf"
};

char *class_names[NUM_OF_CLASSES] = {
    "none", "warrior", "wizard", "bard", "assassin"
};



char *stats_names[NUM_OF_STATS] = {
    "race", 
    "class", 
    "life",
    "strength", 
    "dexterity", 
    "charisma", 
    "intelligence", 
    "stamina",
    "mana",
    "gold"
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

#define low_stamina(stamina) (stamina<10)

uint8_t fight_stat(uint8_t stat_value, uint8_t stamina)
{
    return stat_value/(1+(rand()&3))/(1+low_stamina(stamina));
}

uint8_t attack(Character *attacker_ptr, Character* defender_ptr)
{
    uint8_t attacker_stamina = get_stat(attacker_ptr,STAMINA);
    uint8_t blow_hits;
    
    if (2*fight_stat(get_stat(attacker_ptr,DEXTERITY), attacker_stamina) >
        fight_stat(get_stat(defender_ptr,DEXTERITY), get_stat(defender_ptr,STAMINA)))
    {
        blow_hits = fight_stat(get_stat(attacker_ptr,STRENGTH), attacker_stamina);
        blow(defender_ptr, blow_hits);
    }
    else{
        blow_hits = 0;
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
    
    Character* player_ptr = player_party[0];
    Character* foe_ptr = enemy_party[0];
    
    while (get_life(player_ptr) && get_life(foe_ptr))
    {
        printf("\n\n\n------------------\n");
        printf("round %d\n", ++round);
        printf("------------------\n");
        getchar();
        showCharacter(player_ptr);
        getchar();
        showCharacter(foe_ptr);
        getchar();
        
        // DEBUG
        showCharacter(enemy_party[1]);
        getchar();
        
        printf("\nFight!\n");
        getchar();
        
        fight_round(player_party[0],enemy_party[0], VERBOSE_ON);
        
        if((get_life(player_ptr)) && (get_life(foe_ptr)))
        {
            for(i=1;i<min_size;++i)
            {
                fight_round(player_party[i],enemy_party[i], VERBOSE_OFF);
            }
            if(player_party_size>min_size) // You have more party members
            {
                many_vs_one_fight(player_party[i],player_party_size-enemy_party_size,foe_ptr);
                // for(;i<player_party_size;++i)
                // {
                    // if(get_life(player_party[i]) && get_life(enemy_party[0]))
                    // {
                        // fight_round(player_party[i],enemy_party[0], VERBOSE_ON);
                    // }
                    // else
                    // {
                        // break;
                    // }
                // }
            }
            else // The enemy party has more members
            {
                many_vs_one_fight(enemy_party[i], enemy_party_size - player_party_size, player_ptr);
                // for(;i<enemy_party_size;++i)
                // {
                    // if(get_life(player_ptr) && get_life(enemy_party[i]))
                    // {
                        // fight_round(player_party[0],enemy_party[i], VERBOSE_ON);
                    // }
                    // else
                    // {
                        // break;
                    // }
                // }
            }
        }
    }
    
    showCharacters();
}


/*
void full_fight(Character* first_ptr, Character* second_ptr)
{
    uint8_t round = 1;
    Character *winner_ptr;
    Character *loser_ptr;
    
    while((get_stat(first_ptr,LIFE))&&(get_stat(second_ptr,LIFE)))
    {
        printf("\n\n\n------------------\n");
        printf("round %d\n", round++);
        printf("------------------\n");
        getchar();
        showCharacter(first_ptr);
        getchar();
        showCharacter(second_ptr);
        printf("------------------\n");
        
        getchar();
        
        fight_round(first_ptr, second_ptr, VERBOSE_ON);
        getchar();
    }
    if(first_ptr->stat[LIFE])
    {   
        winner_ptr = first_ptr;
        loser_ptr = second_ptr;
    }
    else
    {
        winner_ptr = second_ptr;
        loser_ptr = first_ptr;
    }
    printf("\n%s dies!\n", loser_ptr->name);
    printf("%s wins\n", winner_ptr->name);
}
*/


void initPlayerParty(void)
{
    player_party[0] = conan_ptr;
    player_party_size = 1;
}

void initEnemyParty(void)
{
    enemy_party[0] = ulrik_ptr;
    enemy_party[1] = sheewa_ptr;
    enemy_party_size = 2;
}

int main(void)
{


    initCharacters();
    
    initPlayerParty();
    initEnemyParty();
    
    getchar();
    
    party_fight();

    return EXIT_SUCCESS;
}

