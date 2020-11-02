#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define MAX_NAME_SIZE 12
#define NUM_OF_CHARACTERS 3

#define NUM_OF_STATIC_STATS 2
#define RACE 0
#define CLASS 1


#define NUM_OF_DYNAMIC_STATS 7
#define LIFE         (NUM_OF_STATIC_STATS+0)
#define STRENGTH     (NUM_OF_STATIC_STATS+1)
#define DEXTERITY    (NUM_OF_STATIC_STATS+2)
#define CHARISMA     (NUM_OF_STATIC_STATS+3)
#define STAMINA      (NUM_OF_STATIC_STATS+4)
#define INTELLIGENCE (NUM_OF_STATIC_STATS+5)
#define GOLD         (NUM_OF_STATIC_STATS+6)

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

#define increase_character_stat(_character_ptr, _stat_index)  ++(character_ptr)->stat[_stat_index]

#define decrease_character_stat(_character_ptr, _stat_index)  --(character_ptr)->stat[_stat_index]




char *race_names[NUM_OF_RACES] = {
    "human", "orc", "elf"
};

char *class_names[NUM_OF_CLASSES] = {
    "none", "warrior", "wizard", "bard", "assassin"
};



char *stats_names[NUM_OF_STATS] = {
    "race", "class", 
    "life",
    "strength", "dexterity", "charisma", "stamina", "intelligence", "gold"
};


struct CharacterStruct 
{
    uint8_t stat[NUM_OF_STATS];
    
    char name[MAX_NAME_SIZE];
};
typedef struct CharacterStruct Character;


Character characters[NUM_OF_CHARACTERS];


char *characters_names[NUM_OF_CHARACTERS] = 
{
    "Conan",
    "Ulrik", 
    "Tondor"
};


uint8_t characters_stats[NUM_OF_CHARACTERS][NUM_OF_STATS] =
{
    {
        HUMAN, NONE,     20,  5, 55, 10, 40,  0
    },
    { 
        ORC,   WARRIOR,  40,  8, 35,  5, 30, 20
    },
    {
        ELF,   ASSASSIN, 30,  3, 10,  2, 60, 80
    }
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
    if(get_stat(character_ptr,LIFE)>value)
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

void fight_round(Character* first_ptr, Character* second_ptr)
{
    
    attack_string(first_ptr, second_ptr);

    if(get_stat(second_ptr,LIFE))
    {
        attack_string(second_ptr, first_ptr);
    }
}


void test_fight(Character* first_ptr, Character* second_ptr)
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
        
        fight_round(first_ptr, second_ptr);
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

int main(void)
{

    Character* conan_ptr = &characters[0];
    Character* ulrik_ptr = &characters[1];
    initCharacters();
    
    printf("\n\nFight between \n%s and %s\n", conan_ptr->name, ulrik_ptr->name);
    getchar();
    test_fight(&characters[0], &characters[1]);

    return EXIT_SUCCESS;
}

