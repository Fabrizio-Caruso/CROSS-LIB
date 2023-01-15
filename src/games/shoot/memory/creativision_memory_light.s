
.export _ghosts;
_ghosts = $100

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

.export _GHOST_IMAGE;
_GHOST_IMAGE = _ghostCount + $1

.export _BOMB_IMAGE;
_BOMB_IMAGE = _GHOST_IMAGE + $2

.export _PLAYER_UP_IMAGE;
_PLAYER_UP_IMAGE = _BOMB_IMAGE + $2

.export _PLAYER_DOWN_IMAGE;
_PLAYER_DOWN_IMAGE = _PLAYER_UP_IMAGE + $2

.export _PLAYER_LEFT_IMAGE;
_PLAYER_LEFT_IMAGE = _PLAYER_DOWN_IMAGE + $2

.export _PLAYER_RIGHT_IMAGE;
_PLAYER_RIGHT_IMAGE = _PLAYER_LEFT_IMAGE + $2

.export _ind;
_ind = _PLAYER_RIGHT_IMAGE + $2

; LIGHT

.export _skullSlowDown;
_skullSlowDown = _ind + $1

.export _skullXCountDown;
_skullXCountDown = _skullSlowDown + $2

.export _skullYCountDown;
_skullYCountDown = _skullXCountDown + $1

.export _bulletDirection;
_bulletDirection = _skullYCountDown + $1

.export _freeze_count_down;
_freeze_count_down = _bulletDirection + $4

.export _playerDirection;
_playerDirection = _freeze_count_down + $1

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