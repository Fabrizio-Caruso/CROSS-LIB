
.export _ghosts;
_ghosts = $100

.export _bombs;
_bombs = _ghosts + $28 

.export _player;
_player = _bombs + $14

.export _points;
_points = _player + $5

.export _highScore;
_highScore = _points + $2

.export _lives;
_lives = _highScore + $2

.export _level;
_level = _lives + $1

.export _ghostCount;
_ghostCount = _level + $1

.export _PLAYER_DOWN_IMAGE;
_PLAYER_DOWN_IMAGE = _ghostCount + $1

.export _PLAYER_UP_IMAGE;
_PLAYER_UP_IMAGE = _PLAYER_DOWN_IMAGE + $1

.export _PLAYER_LEFT_IMAGE;
_PLAYER_LEFT_IMAGE = _PLAYER_UP_IMAGE + $1

.export _PLAYER_RIGHT_IMAGE;
_PLAYER_RIGHT_IMAGE = _PLAYER_LEFT_IMAGE + $1

.export _GHOST_IMAGE;
_GHOST_IMAGE = _PLAYER_RIGHT_IMAGE + $1

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

.export _freeze_count_down;
_freeze_count_down = _ZOMBIE_IMAGE + $8

.export _loop;
_loop = $100

.export _bulletDirection;
_bulletDirection = _loop + $2

.export _skullXCountDown;
_skullXCountDown = _bulletDirection + $1

.export _skullYCountDown;
_skullYCountDown = _skullXCountDown + $1

.export _ghostSlowDown;
_ghostSlowDown = _skullYCountDown + $1

.export _skullActive;
_skullActive = _ghostSlowDown + $2

.export _skull;
_skull = _skullActive + $1

.export _bullet;
_bullet = _skull + $5

.export _powerUp;
_powerUp = _bullet + $5

.export _gun;
_gun = _powerUp + $A

.export _extraPoints;
_extraPoints = _gun + $A

.export _playerDirection;
_playerDirection = _extraPoints + $A

.export _freezeActive;
_freezeActive = _playerDirection + $1

.export _extraLifeThroughPointsCounter;
_extraLifeThroughPointsCounter = _freezeActive + $1

.export _playerFire;
_playerFire = _extraLifeThroughPointsCounter + $1

.export _guns;
_guns = _playerFire + $1

.export _skullSlowDown;
_skullSlowDown = _guns + $2
