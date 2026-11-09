
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

