
.export _ghosts;
_ghosts = $33A

.export _bombs;
_bombs = _ghosts + $28 

.export _player;
_player = _bombs + $14

.export _ghostSlowDown;
_ghostSlowDown = _player + $5

.export _points;
_points = _ghostSlowDown + $2

.export _highScore;
_highScore = _points + $2

.export _lives;
_lives = _highScore + $2

.export _level;
_level = _lives + $1

.export _ghostCount;
_ghostCount = _level + $1

.export _PLAYER_DOWN;
_PLAYER_DOWN = _ghostCount + $1

.export _PLAYER_UP;
_PLAYER_UP = _PLAYER_DOWN + $1

.export _PLAYER_LEFT;
_PLAYER_LEFT = _PLAYER_UP + $1

.export _PLAYER_RIGHT;
_PLAYER_RIGHT = _PLAYER_LEFT + $1

.export _GHOST_IMAGE;
_GHOST_IMAGE = _PLAYER_RIGHT + $1

.export _BOMB_IMAGE;
_BOMB_IMAGE = _GHOST_IMAGE + $1

.export _ind;
_ind = _BOMB_IMAGE + $1
				
.export _POWERUP_IMAGE;
_POWERUP_IMAGE = _ind + 1
	
.export _BULLET_IMAGE;
_BULLET_IMAGE = _POWERUP_IMAGE + $2

.export _GUN_IMAGE;
_GUN_IMAGE = _BULLET_IMAGE + $2

.export _SKULL_IMAGE;
_SKULL_IMAGE = _GUN_IMAGE + $2

.export _EXTRA_POINTS_IMAGE;
_EXTRA_POINTS_IMAGE = _SKULL_IMAGE + $2

.export _HORIZONTAL_BRICK_IMAGE;
_HORIZONTAL_BRICK_IMAGE = _EXTRA_POINTS_IMAGE + $2

.export _VERTICAL_BRICK_IMAGE;
_VERTICAL_BRICK_IMAGE = _HORIZONTAL_BRICK_IMAGE + $2

.export _LEFT_HORIZONTAL_MISSILE_IMAGE;
_LEFT_HORIZONTAL_MISSILE_IMAGE = _VERTICAL_BRICK_IMAGE + $2

.export _RIGHT_HORIZONTAL_MISSILE_IMAGE;
_RIGHT_HORIZONTAL_MISSILE_IMAGE = _LEFT_HORIZONTAL_MISSILE_IMAGE + $2

.export _ROCKET_IMAGE;
_ROCKET_IMAGE = _RIGHT_HORIZONTAL_MISSILE_IMAGE + $2

.export _FREEZE_IMAGE;
_FREEZE_IMAGE = _ROCKET_IMAGE + $2

.export _EXTRA_LIFE_IMAGE;
_EXTRA_LIFE_IMAGE = _FREEZE_IMAGE + $2

.export _INVINCIBILITY_IMAGE;
_INVINCIBILITY_IMAGE = _EXTRA_LIFE_IMAGE + $2

.export _CHASE_IMAGE;
_CHASE_IMAGE = _INVINCIBILITY_IMAGE + $2

.export _SUPER_IMAGE;
_SUPER_IMAGE = _CHASE_IMAGE + $4

.export _CONFUSE_IMAGE;
_CONFUSE_IMAGE = _SUPER_IMAGE + $2

.export _ZOMBIE_IMAGE;
_ZOMBIE_IMAGE = _CONFUSE_IMAGE + $2

		
		
		