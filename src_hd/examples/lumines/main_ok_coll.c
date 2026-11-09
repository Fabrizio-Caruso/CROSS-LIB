#include "cross_lib.h"

#define W 16
#define H YSize

int main(void)
{
    uint8_t grid[H][W];
    uint8_t prev_grid[H][W];
    uint8_t block[2][2];
    short active_x, active_y;
    short prev_ax, prev_ay;
    short can_move;
    short gy, gx;
    uint8_t has_active;
    uint16_t score;
    uint8_t timeline_x;
    uint8_t game_over;
    uint8_t i, j;
    uint8_t input;
    uint8_t tmp;

    _XL_INIT_GRAPHICS();
    _XL_INIT_INPUT();
    _XL_INIT_SOUND();

    for(i=0;i<H;i++) for(j=0;j<W;j++) prev_grid[i][j]=0xFF;

    while(1)
    {
        score=0;
        timeline_x=0;
        game_over=0;
        for(i=0;i<H;i++) for(j=0;j<W;j++) grid[i][j]=0;
        has_active=0;
        prev_ax=-1;
        prev_ay=-1;
        _XL_CLEAR_SCREEN();
        _XL_SET_TEXT_COLOR(_XL_WHITE);
        _XL_PRINTD(0,0,4,score);

        while(!game_over)
        {
            input=_XL_INPUT();

            if(has_active)
            {
                if(_XL_LEFT(input))  { if(active_x>0) active_x-=2; }
                if(_XL_RIGHT(input)) { if(active_x+2<W-2) active_x+=2; }
                if(_XL_FIRE(input))
                {
                    tmp = block[0][0];
                    block[0][0] = block[1][0];
                    block[1][0] = block[1][1];
                    block[1][1] = block[0][1];
                    block[0][1] = tmp;
                }
            }

            timeline_x++;
            if(timeline_x>=W) timeline_x=0;

            if(!has_active)
            {
                active_x = (short)((_XL_RAND()%((W/2)-1))*2);
                active_y = -2;
                prev_ax = active_x;
                prev_ay = active_y;
                for(i=0;i<2;i++) for(j=0;j<2;j++)
                    block[i][j] = (uint8_t)((_XL_RAND()&1)?1:2);
                if(block[0][0]==block[0][1] && block[0][0]==block[1][0] && block[0][0]==block[1][1])
                    block[1][1] = (uint8_t)(block[0][0]==1?2:1);
                has_active = 1;
            }

            if(has_active)
            {
                if(prev_ax>=0)
                {
                    for(i=0;i<2;i++) for(j=0;j<2;j++)
                    {
                        gy = prev_ay + i;
                        gx = prev_ax + j;
                        if(gy>=0 && gy<H && gx>=0 && gx<W) _XL_DELETE(gx,gy);
                    }
                }

                can_move = 1;
                for(i=0;i<2;i++) for(j=0;j<2;j++)
                {
                    gy = active_y + 1 + i;
                    gx = active_x + j;
                    if(gy>=H || (gy>=0 && grid[gy][gx]!=0)) { can_move = 0; }
                }

                if(can_move) active_y++;
                else
                {
                    for(i=0;i<2;i++) for(j=0;j<2;j++)
                    {
                        gy = active_y + i;
                        gx = active_x + j;
                        if(gy>=0 && gy<H && gx>=0 && gx<W)
                            grid[gy][gx] = block[i][j];
                    }
                    has_active = 0;
                    prev_ax = -1;
                    prev_ay = -1;
                }

                for(i=0;i<2;i++) for(j=0;j<2;j++)
                {
                    gy = active_y + i;
                    gx = active_x + j;
                    if(gy>=0 && gy<H && gx>=0 && gx<W)
                    {
                        uint8_t col = block[i][j]==1?_XL_CYAN:_XL_YELLOW;
                        _XL_DRAW(gx,gy,_TILE_0,col);
                    }
                }
                prev_ax = active_x;
                prev_ay = active_y;
            }

            for(i=0;i<H-1;i++) for(j=0;j<W-1;j++)
            {
                uint8_t c = grid[i][j];
                if(c!=0 && c==grid[i][j+1] && c==grid[i+1][j] && c==grid[i+1][j+1])
                {
                    if(timeline_x>=j && timeline_x<=j+1)
                    {
                        grid[i][j]=0;
                        grid[i][j+1]=0;
                        grid[i+1][j]=0;
                        grid[i+1][j+1]=0;
                        score+=10;
                        _XL_PING_SOUND();
                    }
                }
            }

            for(i=0;i<W;i++) if(grid[0][i]!=0) game_over=1;

            for(i=0;i<H;i++) for(j=0;j<W;j++)
            {
                if(grid[i][j]!=prev_grid[i][j])
                {
                    if(grid[i][j]==0) _XL_DELETE(j,i);
                    else
                    {
                        uint8_t col = grid[i][j]==1?_XL_CYAN:_XL_YELLOW;
                        _XL_DRAW(j,i,_TILE_0,col);
                    }
                    prev_grid[i][j]=grid[i][j];
                }
            }

            _XL_SET_TEXT_COLOR(_XL_WHITE);
            _XL_PRINTD(0,0,4,score);
            _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
        }

        _XL_WAIT_FOR_INPUT();
    }
    return 0;
}
