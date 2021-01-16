PUBLIC _ghosts, _bombs, _player, _ghostSlowDown, _points, _highScore, _lives, _level, _ghostCount, _GHOST_IMAGE, _BOMB_IMAGE, _ind

defc _ghosts = 0x2A00

defc _bombs = _ghosts + $28 

defc _player = _bombs + $14

defc _ghostSlowDown = _player + $5

defc _points = _ghostSlowDown + $2

defc _highScore = _points + $2

defc _lives = _highScore + $2

defc _level = _lives + $1

defc _ghostCount = _level + $1

defc _GHOST_IMAGE = _ghostCount + $1

defc _BOMB_IMAGE = _GHOST_IMAGE + $1

defc _ind = _BOMB_IMAGE + $1

