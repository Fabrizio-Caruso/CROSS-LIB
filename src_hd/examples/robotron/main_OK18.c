#include "cross_lib.h"

#define MAX_ENEMIES 18
#define MIN_BULLETS  4
#define MAX_BULLETS 12
#define MAX_ITEMS   4
#define ENEMY_MOVE_CHANCE 32
#define UI_ROW 0
#define PLAY_TOP 2

uint8_t player_x, player_y, player_dir, last_move_dir;
uint8_t player_prev_x, player_prev_y, player_prev_dir;
short lives;
uint16_t score, score_last;
uint8_t enemy_x[MAX_ENEMIES], enemy_y[MAX_ENEMIES];
uint8_t enemy_active[MAX_ENEMIES];
uint8_t enemy_dir[MAX_ENEMIES];
uint8_t enemy_prev_x[MAX_ENEMIES], enemy_prev_y[MAX_ENEMIES];
uint8_t enemy_prev_active[MAX_ENEMIES];
uint8_t bullet_x[MAX_BULLETS], bullet_y[MAX_BULLETS];
short bullet_dx[MAX_BULLETS], bullet_dy[MAX_BULLETS];
uint8_t bullet_active[MAX_BULLETS];
uint8_t item_x[MAX_ITEMS], item_y[MAX_ITEMS];
uint8_t item_type[MAX_ITEMS];
uint8_t item_active[MAX_ITEMS];

uint8_t fire_mode;
uint8_t fire_upgrade_cnt;
uint16_t max_bullets_allowed;
uint8_t invincible_timer;
uint8_t fire_tick;

static void draw_player(uint8_t x, uint8_t y, uint8_t dir)
{
    uint8_t col,t0,t1;
    col = invincible_timer ? _XL_YELLOW : _XL_CYAN;
    if (dir==0){t0=_TILE_10;t1=_TILE_11;} else {t0=_TILE_12;t1=_TILE_13;}
    _XL_DRAW(x,y,t0,col); _XL_DRAW(x,y+1,t1,col);
}
static void del_player(uint8_t x,uint8_t y){_XL_DELETE(x,y);_XL_DELETE(x,y+1);}
static void draw_enemy(uint8_t x,uint8_t y,uint8_t dir)
{
    uint8_t t0,t1;
    if (dir==0){t0=_TILE_20;t1=_TILE_21;} else {t0=_TILE_23;t1=_TILE_24;}
    _XL_DRAW(x,y,t0,_XL_RED); _XL_DRAW(x,y+1,t1,_XL_RED);
}
static void del_enemy(uint8_t x,uint8_t y){_XL_DELETE(x,y);_XL_DELETE(x,y+1);}
static void draw_item(uint8_t x,uint8_t y,uint8_t t)
{
    uint8_t col;
    if (t==0) col = _XL_CYAN; else if (t==1) col = _XL_YELLOW; else col = _XL_GREEN;
    _XL_DRAW(x,y,_TILE_22,col);
}
static uint8_t enemy_occupies(uint8_t x, uint8_t y)
{
    uint8_t i;
    for (i=0;i<MAX_ENEMIES;i++) if (enemy_active[i]) {
        if ((enemy_x[i]==x && enemy_y[i]==y) || (enemy_x[i]==x && enemy_y[i]+1==y)) return 1;
    }
    return 0;
}
static uint8_t bullet_hits_enemy(uint8_t bx, uint8_t by, uint8_t ex, uint8_t ey)
{
    uint8_t dx;
    dx = bx > ex ? bx-ex : ex-bx;
    if (dx>1) return 0;
    if (by==ey || by==ey+1) return 1;
    return 0;
}
static void check_player_enemy_collision(void)
{
    uint8_t i;
    for (i=0;i<MAX_ENEMIES;i++){
        if (!enemy_active[i]) continue;
        if (enemy_x[i]==player_x && (enemy_y[i]==player_y || enemy_y[i]+1==player_y)){
            if (invincible_timer){
                del_enemy(enemy_x[i],enemy_y[i]); enemy_active[i]=0; score+=20;
            }else{
                lives--; del_enemy(enemy_x[i],enemy_y[i]); enemy_active[i]=0; _XL_EXPLOSION_SOUND();
            }
        }
    }
}
void init_game(void)
{
    uint8_t i,side;
    _XL_CLEAR_SCREEN();
    player_x = XSize/2; player_y = PLAY_TOP + (YSize-PLAY_TOP-2)/2;
    player_dir = 1; last_move_dir = 3; lives = 3; score = 0; score_last = 0xFFFF;
    fire_mode = 0; fire_upgrade_cnt = 0; max_bullets_allowed = MIN_BULLETS;
    invincible_timer = 0; fire_tick = 0;
    player_prev_x = player_x; player_prev_y = player_y; player_prev_dir = player_dir;
    for (i=0;i<MAX_ENEMIES;i++){enemy_active[i]=0;enemy_prev_active[i]=0;enemy_dir[i]=1;}
    for (i=0;i<MAX_BULLETS;i++) bullet_active[i]=0;
    for (i=0;i<MAX_ITEMS;i++) item_active[i]=0;
    for (i=0;i<MAX_ENEMIES;i++){
        enemy_active[i]=1;
        side=_XL_RAND()%4;
        if (side==0){enemy_x[i]=0; enemy_y[i]=PLAY_TOP+(uint8_t)(_XL_RAND()%(YSize-PLAY_TOP-1));}
        else if(side==1){enemy_x[i]=XSize-1; enemy_y[i]=PLAY_TOP+(uint8_t)(_XL_RAND()%(YSize-PLAY_TOP-1));}
        else if(side==2){enemy_x[i]=(uint8_t)(_XL_RAND()%XSize); enemy_y[i]=PLAY_TOP;}
        else {enemy_x[i]=(uint8_t)(_XL_RAND()%XSize); enemy_y[i]=YSize-2;}
        enemy_prev_x[i]=enemy_x[i]; enemy_prev_y[i]=enemy_y[i]; enemy_prev_active[i]=1;
        enemy_dir[i]=1;
        draw_enemy(enemy_x[i],enemy_y[i],enemy_dir[i]);
    }
    draw_player(player_x,player_y,player_dir);
    _XL_SET_TEXT_COLOR(_XL_WHITE);
    _XL_PRINT(UI_ROW,0,"SCORE"); _XL_PRINTD(6,UI_ROW,4,score);
}
static uint8_t count_active_bullets(void)
{
    uint8_t i,c;
    c=0;
    for(i=0;i<MAX_BULLETS;i++) if(bullet_active[i]) c++;
    return c;
}


void update_game(void)
{
    uint8_t inp,i,j,k;
    short dx,dy;
    short bdx[3],bdy[3];
    uint8_t changed;
    uint8_t bx,by;
    uint8_t active_bullets;
    uint8_t nx,ny;
    uint8_t occupied;
    uint8_t ox,oy;
    uint8_t t_item;
    uint8_t rad;
    uint8_t dxx,dyy;
    uint8_t spawn_thresh;
    uint8_t hit_found;

    if (score!=score_last){_XL_SET_TEXT_COLOR(_XL_WHITE);_XL_PRINTD(6,UI_ROW,4,score);score_last=score;}
    if (invincible_timer) invincible_timer--;

    check_player_enemy_collision();

    inp=_XL_INPUT();
    if (_XL_LEFT(inp)){if(player_x>0)player_x--;player_dir=0;last_move_dir=2;}
    if (_XL_RIGHT(inp)){if(player_x<XSize-1)player_x++;player_dir=1;last_move_dir=3;}
    if (_XL_UP(inp)){if(player_y>PLAY_TOP)player_y--;last_move_dir=0;}
    if (_XL_DOWN(inp)){if(player_y<YSize-2)player_y++;last_move_dir=1;}

    changed = (player_x!=player_prev_x||player_y!=player_prev_y||player_dir!=player_prev_dir);
    if (changed){del_player(player_prev_x,player_prev_y);draw_player(player_x,player_y,player_dir);
                 player_prev_x=player_x;player_prev_y=player_y;player_prev_dir=player_dir;}

    check_player_enemy_collision();

    active_bullets = count_active_bullets();
    if (_XL_FIRE(inp) && (fire_tick%4==0) && active_bullets < max_bullets_allowed){
        if (fire_mode){
            if (last_move_dir==2){bdx[0]=-1;bdy[0]=0;bdx[1]=-1;bdy[1]=-1;bdx[2]=-1;bdy[2]=1;}
            else if(last_move_dir==3){bdx[0]=1;bdy[0]=0;bdx[1]=1;bdy[1]=-1;bdx[2]=1;bdy[2]=1;}
            else if(last_move_dir==0){bdx[0]=0;bdy[0]=-1;bdx[1]=-1;bdy[1]=-1;bdx[2]=1;bdy[2]=-1;}
            else {bdx[0]=0;bdy[0]=1;bdx[1]=-1;bdy[1]=1;bdx[2]=1;bdy[2]=1;}
        }else{
            bdx[0]=(last_move_dir==2)?-1:(last_move_dir==3)?1:0;
            bdy[0]=(last_move_dir==0)?-1:(last_move_dir==1)?1:0;
        }
        for (k=0;k<(fire_mode?3:1);k++){
            bx = player_x + bdx[k];
            by = player_y + bdy[k];
            if (bdy[k] > 0) by += 1;          /* push down shots below the bottom tile */
            /* horizontal shots keep by == player_y -> next to the top tile */

            if (bx>=XSize||by>=YSize||by==0) continue;

            hit_found = 0;
            for (j=0;j<MAX_ENEMIES;j++){
                if (enemy_active[j] && bullet_hits_enemy(bx,by,enemy_x[j],enemy_y[j])){
                    del_enemy(enemy_x[j],enemy_y[j]); enemy_active[j]=0; score+=10; _XL_TOCK_SOUND();
                    t_item=_XL_RAND()%3;
                    for (i=0;i<MAX_ITEMS;i++) if (!item_active[i]){
                        item_active[i]=1; item_type[i]=t_item; item_x[i]=enemy_x[j]; item_y[i]=enemy_y[j];
                        if (!enemy_occupies(item_x[i],item_y[i])) draw_item(item_x[i],item_y[i],t_item);
                        break;
                    }
                    hit_found = 1;
                    break;
                }
            }
            if (hit_found) continue;

            for (i=0;i<MAX_BULLETS;i++) if (!bullet_active[i]){
                bullet_active[i]=1; bullet_x[i]=bx; bullet_y[i]=by;
                bullet_dx[i]=bdx[k]; bullet_dy[i]=bdy[k];
                _XL_DRAW(bullet_x[i],bullet_y[i],_TILE_0,_XL_YELLOW); _XL_SHOOT_SOUND(); break;
            }
        }
    }
    fire_tick++;

    for (i=0;i<MAX_BULLETS;i++){
        if (!bullet_active[i]) continue;
        ox=bullet_x[i]; oy=bullet_y[i];
        bullet_x[i]+=bullet_dx[i]*2; bullet_y[i]+=bullet_dy[i]*2;
        if (!(ox==player_x && (oy==player_y || oy==player_y+1))) _XL_DELETE(ox,oy);
        if (bullet_x[i]<XSize && bullet_y[i]>0 && bullet_y[i]<YSize){
            if (!(bullet_x[i]==player_x && (bullet_y[i]==player_y || bullet_y[i]==player_y+1)))
                _XL_DRAW(bullet_x[i],bullet_y[i],_TILE_0,_XL_YELLOW);
        }else{bullet_active[i]=0;}
        for (j=0;j<MAX_ENEMIES;j++){
            if (enemy_active[j] && bullet_hits_enemy(bullet_x[i],bullet_y[i],enemy_x[j],enemy_y[j])){
                del_enemy(enemy_x[j],enemy_y[j]); enemy_active[j]=0; bullet_active[i]=0;
                if (!(bullet_x[i]==player_x && (bullet_y[i]==player_y || bullet_y[i]==player_y+1)))
                    _XL_DELETE(bullet_x[i],bullet_y[i]);
                score+=10; _XL_TOCK_SOUND();
                if (_XL_RAND()%100<60){
                    t_item=_XL_RAND()%3;
                    for (k=0;k<MAX_ITEMS;k++) if (!item_active[k]){
                        item_active[k]=1; item_type[k]=t_item; item_x[k]=enemy_x[j]; item_y[k]=enemy_y[j];
                        if (!enemy_occupies(item_x[k],item_y[k])) draw_item(item_x[k],item_y[k],t_item);
                        break;
                    }
                }
            }
        }
    }

    for (i=0;i<MAX_ENEMIES;i++){
        if (!enemy_active[i]) continue;
        if (_XL_RAND()%ENEMY_MOVE_CHANCE!=0) continue;
        enemy_active[i]=0;
        nx = enemy_x[i]; ny = enemy_y[i];
        dx = (player_x>enemy_x[i])?1:(player_x<enemy_x[i])?-1:0;
        dy = (player_y>enemy_y[i])?1:(player_y<enemy_y[i])?-1:0;
        if (dx && enemy_x[i]+dx<XSize && enemy_x[i]+dx>0 && !enemy_occupies(enemy_x[i]+dx,enemy_y[i]))
            nx=enemy_x[i]+dx; else dx=0;
        if (dy && enemy_y[i]+dy<YSize-1 && enemy_y[i]+dy>=PLAY_TOP && !enemy_occupies(enemy_x[i],enemy_y[i]+dy))
            ny=enemy_y[i]+dy; else dy=0;
        enemy_active[i]=1;
        occupied = 0;
        for (j=0;j<MAX_ENEMIES;j++) if (j!=i && enemy_active[j]){
            if ((enemy_x[j]==nx && enemy_y[j]==ny) || (enemy_x[j]==nx && enemy_y[j]+1==ny)){occupied=1;break;}
        }
        if (!occupied){if (dx<0) enemy_dir[i]=0; else if (dx>0) enemy_dir[i]=1; enemy_x[i]=nx; enemy_y[i]=ny;}
    }

    for (i=0;i<MAX_ENEMIES;i++){
        if (enemy_active[i]!=enemy_prev_active[i]){
            if (enemy_prev_active[i]) del_enemy(enemy_prev_x[i],enemy_prev_y[i]);
            if (enemy_active[i]) draw_enemy(enemy_x[i],enemy_y[i],enemy_dir[i]);
            enemy_prev_active[i]=enemy_active[i]; enemy_prev_x[i]=enemy_x[i]; enemy_prev_y[i]=enemy_y[i];
        }else if (enemy_active[i]){
            if (enemy_x[i]!=enemy_prev_x[i]||enemy_y[i]!=enemy_prev_y[i]){
                del_enemy(enemy_prev_x[i],enemy_prev_y[i]);
                draw_enemy(enemy_x[i],enemy_y[i],enemy_dir[i]);
                enemy_prev_x[i]=enemy_x[i]; enemy_prev_y[i]=enemy_y[i];
            }
        }
    }

    for (i=0;i<MAX_ITEMS;i++) if (item_active[i]){
        if (!enemy_occupies(item_x[i],item_y[i])) draw_item(item_x[i],item_y[i],item_type[i]);
    }

    for (i=0;i<MAX_ITEMS;i++){
        if (!item_active[i]) continue;
        if (player_x==item_x[i] && (player_y==item_y[i] || player_y+1==item_y[i] || player_y==item_y[i]+1)){
            _XL_DELETE(item_x[i],item_y[i]);
            if (item_type[i]==0) invincible_timer=120;
            else if (item_type[i]==1){
                rad=XSize/4;
                for (j=0;j<MAX_ENEMIES;j++) if (enemy_active[j]){
                    dxx = enemy_x[j] > item_x[i] ? enemy_x[j]-item_x[i] : item_x[i]-enemy_x[j];
                    dyy = enemy_y[j] > item_y[i] ? enemy_y[j]-item_y[i] : item_y[i]-enemy_y[j];
                    if (dxx<=rad && dyy<=rad){del_enemy(enemy_x[j],enemy_y[j]); enemy_active[j]=0;}
                }
            }else{
                fire_upgrade_cnt++;
                if (fire_upgrade_cnt==1) max_bullets_allowed=8;
                else if (fire_upgrade_cnt==2) max_bullets_allowed=16;
                else if (fire_upgrade_cnt==3){max_bullets_allowed=32; fire_mode=1;}
                else score+=500;
            }
            item_active[i]=0;
        }
    }

    spawn_thresh=3+(score/40); if (spawn_thresh>30) spawn_thresh=30;
    if (_XL_RAND()%100 < spawn_thresh){
        for (i=0;i<MAX_ENEMIES;i++) if (!enemy_active[i]){
            uint8_t side;
            side=_XL_RAND()%4;
            if (side==0){enemy_x[i]=0; enemy_y[i]=PLAY_TOP+(uint8_t)(_XL_RAND()%(YSize-PLAY_TOP-1));}
            else if(side==1){enemy_x[i]=XSize-1; enemy_y[i]=PLAY_TOP+(uint8_t)(_XL_RAND()%(YSize-PLAY_TOP-1));}
            else if(side==2){enemy_x[i]=(uint8_t)(_XL_RAND()%XSize); enemy_y[i]=PLAY_TOP;}
            else {enemy_x[i]=(uint8_t)(_XL_RAND()%XSize); enemy_y[i]=YSize-2;}
            enemy_active[i]=1; enemy_dir[i]=1;
            draw_enemy(enemy_x[i],enemy_y[i],enemy_dir[i]);
            enemy_prev_x[i]=enemy_x[i]; enemy_prev_y[i]=enemy_y[i]; enemy_prev_active[i]=1; break;
        }
    }
}


// void update_game(void)
// {
    // uint8_t inp,i,j,k;
    // short dx,dy;
    // short bdx[3],bdy[3];
    // uint8_t changed;
    // uint8_t bx,by;
    // uint8_t active_bullets;
    // uint8_t nx,ny;
    // uint8_t occupied;
    // uint8_t ox,oy;
    // uint8_t t_item;
    // uint8_t rad;
    // uint8_t dxx,dyy;
    // uint8_t spawn_thresh;
    // uint8_t hit_found;

    // if (score!=score_last){_XL_SET_TEXT_COLOR(_XL_WHITE);_XL_PRINTD(6,UI_ROW,4,score);score_last=score;}
    // if (invincible_timer) invincible_timer--;

    // check_player_enemy_collision();

    // inp=_XL_INPUT();
    // if (_XL_LEFT(inp)){if(player_x>0)player_x--;player_dir=0;last_move_dir=2;}
    // if (_XL_RIGHT(inp)){if(player_x<XSize-1)player_x++;player_dir=1;last_move_dir=3;}
    // if (_XL_UP(inp)){if(player_y>PLAY_TOP)player_y--;last_move_dir=0;}
    // if (_XL_DOWN(inp)){if(player_y<YSize-2)player_y++;last_move_dir=1;}

    // changed = (player_x!=player_prev_x||player_y!=player_prev_y||player_dir!=player_prev_dir);
    // if (changed){del_player(player_prev_x,player_prev_y);draw_player(player_x,player_y,player_dir);
                 // player_prev_x=player_x;player_prev_y=player_y;player_prev_dir=player_dir;}

    // check_player_enemy_collision();

    // active_bullets = count_active_bullets();
    // if (_XL_FIRE(inp) && (fire_tick%4==0) && active_bullets < max_bullets_allowed){
        // if (fire_mode){
            // if (last_move_dir==2){bdx[0]=-1;bdy[0]=0;bdx[1]=-1;bdy[1]=-1;bdx[2]=-1;bdy[2]=1;}
            // else if(last_move_dir==3){bdx[0]=1;bdy[0]=0;bdx[1]=1;bdy[1]=-1;bdx[2]=1;bdy[2]=1;}
            // else if(last_move_dir==0){bdx[0]=0;bdy[0]=-1;bdx[1]=-1;bdy[1]=-1;bdx[2]=1;bdy[2]=-1;}
            // else {bdx[0]=0;bdy[0]=1;bdx[1]=-1;bdy[1]=1;bdx[2]=1;bdy[2]=1;}
        // }else{
            // bdx[0]=(last_move_dir==2)?-1:(last_move_dir==3)?1:0;
            // bdy[0]=(last_move_dir==0)?-1:(last_move_dir==1)?1:0;
        // }
        // for (k=0;k<(fire_mode?3:1);k++){
            // bx=player_x+bdx[k]; by=player_y+bdy[k];
            // if (bx>=XSize||by>=YSize||by==0) continue;

            // /* immediate hit test – do not draw a bullet over an enemy */
            // hit_found = 0;
            // for (j=0;j<MAX_ENEMIES;j++){
                // if (enemy_active[j] && bullet_hits_enemy(bx,by,enemy_x[j],enemy_y[j])){
                    // del_enemy(enemy_x[j],enemy_y[j]); enemy_active[j]=0; score+=10; _XL_TOCK_SOUND();
                    // t_item=_XL_RAND()%3;
                    // for (i=0;i<MAX_ITEMS;i++) if (!item_active[i]){
                        // item_active[i]=1; item_type[i]=t_item; item_x[i]=enemy_x[j]; item_y[i]=enemy_y[j];
                        // if (!enemy_occupies(item_x[i],item_y[i])) draw_item(item_x[i],item_y[i],t_item);
                        // break;
                    // }
                    // hit_found = 1;
                    // break;
                // }
            // }
            // if (hit_found) continue;

            // for (i=0;i<MAX_BULLETS;i++) if (!bullet_active[i]){
                // bullet_active[i]=1; bullet_x[i]=bx; bullet_y[i]=by;
                // bullet_dx[i]=bdx[k]; bullet_dy[i]=bdy[k];
                // _XL_DRAW(bullet_x[i],bullet_y[i],_TILE_0,_XL_YELLOW); _XL_SHOOT_SOUND(); break;
            // }
        // }
    // }
    // fire_tick++;

    // for (i=0;i<MAX_BULLETS;i++){
        // if (!bullet_active[i]) continue;
        // ox=bullet_x[i]; oy=bullet_y[i];
        // bullet_x[i]+=bullet_dx[i]*2; bullet_y[i]+=bullet_dy[i]*2;

        // if (!(ox==player_x && (oy==player_y || oy==player_y+1))) _XL_DELETE(ox,oy);

        // if (bullet_x[i]<XSize && bullet_y[i]>0 && bullet_y[i]<YSize){
            // if (!(bullet_x[i]==player_x && (bullet_y[i]==player_y || bullet_y[i]==player_y+1)))
                // _XL_DRAW(bullet_x[i],bullet_y[i],_TILE_0,_XL_YELLOW);
        // }else{
            // bullet_active[i]=0;
        // }
        // for (j=0;j<MAX_ENEMIES;j++){
            // if (enemy_active[j] && bullet_hits_enemy(bullet_x[i],bullet_y[i],enemy_x[j],enemy_y[j])){
                // del_enemy(enemy_x[j],enemy_y[j]); enemy_active[j]=0; bullet_active[i]=0;
                // if (!(bullet_x[i]==player_x && (bullet_y[i]==player_y || bullet_y[i]==player_y+1)))
                    // _XL_DELETE(bullet_x[i],bullet_y[i]);
                // score+=10; _XL_TOCK_SOUND();
                // if (_XL_RAND()%100<60){
                    // t_item=_XL_RAND()%3;
                    // for (k=0;k<MAX_ITEMS;k++) if (!item_active[k]){
                        // item_active[k]=1; item_type[k]=t_item; item_x[k]=enemy_x[j]; item_y[k]=enemy_y[j];
                        // if (!enemy_occupies(item_x[k],item_y[k])) draw_item(item_x[k],item_y[k],t_item);
                        // break;
                    // }
                // }
            // }
        // }
    // }

    // for (i=0;i<MAX_ENEMIES;i++){
        // if (!enemy_active[i]) continue;
        // if (_XL_RAND()%ENEMY_MOVE_CHANCE!=0) continue;

        // enemy_active[i]=0;               /* hide self for occupancy test */
        // nx = enemy_x[i]; ny = enemy_y[i];
        // dx = (player_x>enemy_x[i])?1:(player_x<enemy_x[i])?-1:0;
        // dy = (player_y>enemy_y[i])?1:(player_y<enemy_y[i])?-1:0;

        // if (dx && enemy_x[i]+dx<XSize && enemy_x[i]+dx>0 && !enemy_occupies(enemy_x[i]+dx,enemy_y[i]))
            // nx=enemy_x[i]+dx; else dx=0;
        // if (dy && enemy_y[i]+dy<YSize-1 && enemy_y[i]+dy>=PLAY_TOP && !enemy_occupies(enemy_x[i],enemy_y[i]+dy))
            // ny=enemy_y[i]+dy; else dy=0;

        // enemy_active[i]=1;               /* restore */

        // occupied = 0;
        // for (j=0;j<MAX_ENEMIES;j++) if (j!=i && enemy_active[j]){
            // if ((enemy_x[j]==nx && enemy_y[j]==ny) || (enemy_x[j]==nx && enemy_y[j]+1==ny)){occupied=1;break;}
        // }
        // if (!occupied){if (dx<0) enemy_dir[i]=0; else if (dx>0) enemy_dir[i]=1; enemy_x[i]=nx; enemy_y[i]=ny;}
    // }

    // for (i=0;i<MAX_ENEMIES;i++){
        // if (enemy_active[i]!=enemy_prev_active[i]){
            // if (enemy_prev_active[i]) del_enemy(enemy_prev_x[i],enemy_prev_y[i]);
            // if (enemy_active[i]) draw_enemy(enemy_x[i],enemy_y[i],enemy_dir[i]);
            // enemy_prev_active[i]=enemy_active[i]; enemy_prev_x[i]=enemy_x[i]; enemy_prev_y[i]=enemy_y[i];
        // }else if (enemy_active[i]){
            // if (enemy_x[i]!=enemy_prev_x[i]||enemy_y[i]!=enemy_prev_y[i]){
                // del_enemy(enemy_prev_x[i],enemy_prev_y[i]);
                // draw_enemy(enemy_x[i],enemy_y[i],enemy_dir[i]);
                // enemy_prev_x[i]=enemy_x[i]; enemy_prev_y[i]=enemy_y[i];
            // }
        // }
    // }

    // for (i=0;i<MAX_ITEMS;i++) if (item_active[i]){
        // if (!enemy_occupies(item_x[i],item_y[i])) draw_item(item_x[i],item_y[i],item_type[i]);
    // }

    // for (i=0;i<MAX_ITEMS;i++){
        // if (!item_active[i]) continue;
        // if (player_x==item_x[i] && (player_y==item_y[i] || player_y+1==item_y[i] || player_y==item_y[i]+1)){
            // _XL_DELETE(item_x[i],item_y[i]);
            // if (item_type[i]==0) invincible_timer=120;
            // else if (item_type[i]==1){
                // rad=XSize/4;
                // for (j=0;j<MAX_ENEMIES;j++) if (enemy_active[j]){
                    // dxx = enemy_x[j] > item_x[i] ? enemy_x[j]-item_x[i] : item_x[i]-enemy_x[j];
                    // dyy = enemy_y[j] > item_y[i] ? enemy_y[j]-item_y[i] : item_y[i]-enemy_y[j];
                    // if (dxx<=rad && dyy<=rad){del_enemy(enemy_x[j],enemy_y[j]); enemy_active[j]=0;}
                // }
            // }else{
                // fire_upgrade_cnt++;
                // if (fire_upgrade_cnt==1) max_bullets_allowed=8;
                // else if (fire_upgrade_cnt==2) max_bullets_allowed=16;
                // else if (fire_upgrade_cnt==3){max_bullets_allowed=32; fire_mode=1;}
                // else score+=500;
            // }
            // item_active[i]=0;
        // }
    // }

    // spawn_thresh=3+(score/40); if (spawn_thresh>30) spawn_thresh=30;
    // if (_XL_RAND()%100 < spawn_thresh){
        // for (i=0;i<MAX_ENEMIES;i++) if (!enemy_active[i]){
            // uint8_t side;
            // side=_XL_RAND()%4;
            // if (side==0){enemy_x[i]=0; enemy_y[i]=PLAY_TOP+(uint8_t)(_XL_RAND()%(YSize-PLAY_TOP-1));}
            // else if(side==1){enemy_x[i]=XSize-1; enemy_y[i]=PLAY_TOP+(uint8_t)(_XL_RAND()%(YSize-PLAY_TOP-1));}
            // else if(side==2){enemy_x[i]=(uint8_t)(_XL_RAND()%XSize); enemy_y[i]=PLAY_TOP;}
            // else {enemy_x[i]=(uint8_t)(_XL_RAND()%XSize); enemy_y[i]=YSize-2;}
            // enemy_active[i]=1; enemy_dir[i]=1;
            // draw_enemy(enemy_x[i],enemy_y[i],enemy_dir[i]);
            // enemy_prev_x[i]=enemy_x[i]; enemy_prev_y[i]=enemy_y[i]; enemy_prev_active[i]=1; break;
        // }
    // }
// }



int main(void)
{
    _XL_INIT_GRAPHICS(); _XL_INIT_INPUT(); _XL_INIT_SOUND();
    init_game();
    while (1){
        update_game(); _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        if (lives<=0){_XL_SET_TEXT_COLOR(_XL_WHITE);_XL_PRINT(XSize/2-4,YSize/2,"GAME OVER");_XL_WAIT_FOR_INPUT();init_game();}
    }
    return 0;
}