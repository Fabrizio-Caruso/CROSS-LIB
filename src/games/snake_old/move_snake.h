#ifndef _MOVE_SNAKE_H
#define _MOVE_SNAKE_H


#define SNAKE_UP 0
#define SNAKE_DOWN 1
#define SNAKE_LEFT 2
#define SNAKE_RIGHT 3

#define HORIZONTAL(movement) ((movement)>SNAKE_DOWN)
#define VERTICAL(movement) ((movement)<SNAKE_LEFT)

#define hits_wall(x,y) \
    (map[x][y]>=WALL)

#define hits_deadly_item(x,y) \
    (map[x][y]==DEADLY)

uint8_t move_snake(uint8_t wished_direction);

#endif // _MOVE_SNAKE_H

