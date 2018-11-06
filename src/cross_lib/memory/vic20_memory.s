
.export _ghosts;
_ghosts = $33c

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

.export _skull;
_skull = _level + $1

.export _bullet;
_bullet = _skull + $5

.export _powerUp;
_powerUp = _bullet + $5

.export _powerUp2;
_powerUp2 = _powerUp + $A

.export _gun;
_gun = _powerUp2 + $A

.export _extraPoints;
_extraPoints = _gun + $A

.export _rockets;
_rockets = _extraPoints + $A

.export _freeze;
_freeze = _rockets + $14

.export _invincibility;
_invincibility = _freeze + $A

.export _extraLife;
_extraLife = _invincibility + $A

.export _super;
_super = _extraLife + $A

.export _confuse;
_confuse = _super + $A

.export _BROKEN_BRICK_IMAGE;
_BROKEN_BRICK_IMAGE = _confuse + $A

.export _zombie;
;_zombie = _confuse + $A
_zombie = $200


.export _chase;
_chase = _zombie + $A

.export _ghostCount;
_ghostCount = _chase + $A

.export _ghostLevel;
_ghostLevel = _ghostCount + $1

.export _leftHorizontalMissile;
_leftHorizontalMissile = _ghostLevel + $2;

.export _rightHorizontalMissile;
_rightHorizontalMissile = _leftHorizontalMissile + $5;

.export _chasingBullet;
_chasingBullet = _rightHorizontalMissile + $5;

.export _invincibility_count_down;
_invincibility_count_down = _chasingBullet + $5

.export _confuse_count_down;
_confuse_count_down = _invincibility_count_down + $2

.export _zombie_count_down;
_zombie_count_down = _confuse_count_down + $2

.export _strategyArray;
_strategyArray = _zombie_count_down + $2

.export _freeze_count_down;
_freeze_count_down = _strategyArray + $8

.export _skullSlowDown;
_skullSlowDown = _freeze_count_down + $2

.export _skullXCountDown;
_skullXCountDown = _skullSlowDown + $2

.export _skullYCountDown;
_skullYCountDown = _skullXCountDown + $1

.export _bulletDirection;
_bulletDirection = _skullYCountDown + $1

.export _chasedEnemyPtr;
_chasedEnemyPtr = _bulletDirection + $1

.export _chasedByGhosts;
_chasedByGhosts = _chasedEnemyPtr + $2

.export _rockets_x;
_rockets_x = _chasedByGhosts + $2

.export _extraLife_present_on_level;
_extraLife_present_on_level = _rockets_x + $4

.export _super_present_on_level;
_super_present_on_level = _extraLife_present_on_level + $1

.export _zombie_present_on_level;
_zombie_present_on_level = _super_present_on_level + $1

.export _dead_rockets;
_dead_rockets = _zombie_present_on_level + $1

.export _arrowRange;
_arrowRange = _dead_rockets + $1

.export _skullsKilled;
_skullsKilled = _arrowRange + $1

.export _missileBasesDestroyed;
_missileBasesDestroyed = _skullsKilled + $1

.export _horizontalWallsLength;
_horizontalWallsLength = _missileBasesDestroyed + $1

.export _invincibilityActive;
_invincibilityActive = _horizontalWallsLength + $1

.export _confuseActive;
_confuseActive = _invincibilityActive + $1

.export _zombieActive;
_zombieActive = _confuseActive + $1

.export _innerVerticalWallY;
_innerVerticalWallY = _zombieActive + $1

.export _innerVerticalWallLength;
_innerVerticalWallLength = _innerVerticalWallY + $1

.export _playerDirection;
_playerDirection = _innerVerticalWallLength + $1

.export _freezeActive;
_freezeActive = _playerDirection + $1

.export _extraLifeThroughPointsCounter;
_extraLifeThroughPointsCounter = _freezeActive + $1

.export _playerFire;
_playerFire = _extraLifeThroughPointsCounter + $1

.export _guns;
_guns = _playerFire + $1

.export _skullActive;
_skullActive = _guns + $1

.export _loop;
_loop = _skullActive + $1

.export _playerBlink;
_playerBlink = _loop + $2

.export _arrowYPosition;
_arrowYPosition = _playerBlink + $1

.export _PLAYER_DOWN;
_PLAYER_DOWN = _arrowYPosition + $1

.export _PLAYER_UP;
_PLAYER_UP = _PLAYER_DOWN + $2

.export _PLAYER_LEFT;
_PLAYER_LEFT = _PLAYER_UP + $2

.export _PLAYER_RIGHT;
_PLAYER_RIGHT = _PLAYER_LEFT + $2

.export _GHOST_IMAGE;
_GHOST_IMAGE = _PLAYER_RIGHT + $2

.export _BOMB_IMAGE;
_BOMB_IMAGE = _GHOST_IMAGE + $2

.export _ind;
_ind = _BOMB_IMAGE + $2

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
_SUPER_IMAGE = _CHASE_IMAGE + $2

.export _CONFUSE_IMAGE;
_CONFUSE_IMAGE = _SUPER_IMAGE + $2

.export _ZOMBIE_IMAGE;
_ZOMBIE_IMAGE = _CONFUSE_IMAGE + $2







		
		