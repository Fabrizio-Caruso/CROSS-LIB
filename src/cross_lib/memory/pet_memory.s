
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
		
		
		
		
		
		