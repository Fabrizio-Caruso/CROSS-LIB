struct PowerUpStruct
{
	// character coordinates
	unsigned char coolDown
	unsigned char initialCoolDown;
	
	// _status decides whether the character is active
	void (*behaviour)(void);
};

typedef struct PowerUpStruct PowerUp;

