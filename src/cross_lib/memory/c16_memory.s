
.export _ghosts;
_ghosts = $333

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

.export _zombie;
_zombie = _confuse + $A

.export _chase;
_chase = _zombie + $A

.export _ghostCount;
_ghostCount = _chase + $A

.export _ghostLevel;
_ghostLevel = _ghostCount + $1

.export _leftHorizontalMissile;
_leftHorizontalMissile = _ghostLevel + $2

.export _rightHorizontalMissile;
_rightHorizontalMissile = _leftHorizontalMissile + $5

.export _chasingBullet;
_chasingBullet = _rightHorizontalMissile + $5

.export _invincibility_count_down;
_invincibility_count_down = _chasingBullet + $5

.export _confuse_count_down;
_confuse_count_down = _invincibility_count_down + $2

.export _zombie_count_down;
_zombie_count_down = _confuse_count_down + $2
	
.export _strategyArray;
_strategyArray = _zombie_count_down + $2


; 241

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

.export _innerVerticalWallX;
_innerVerticalWallX = _innerVerticalWallY + $1

.export _innerVerticalWallLength;
_innerVerticalWallLength = _innerVerticalWallX + $1

.export _playerDirection;
_playerDirection = _innerVerticalWallLength + $1

.export _freezeActive;
_freezeActive = _playerDirection + $1

.export _extraLifeThroughPointsCounter;
_extraLifeThroughPointsCounter = _freezeActive + $1



	
	
	
		
		
		
		
		
		
		