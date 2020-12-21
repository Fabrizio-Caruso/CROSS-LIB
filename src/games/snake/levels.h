#ifndef _LEVELS_H
#define _LEVELS_H

// LEVEL WALLS
const uint8_t level_walls[] = 
{
// level 0 (0)
    0,
    0,
    0,
// level 1 (3) 
    2, 
        XSize/3,       YSize/3,     XSize/3,
        XSize/3,     2*YSize/3,     XSize/3,
    2,  
        XSize/5,             1,     YSize/4,
        4*XSize/5,   3*YSize/4,     YSize/4,
    0,

// level 2 (18) 
    0, 
    2,
        XSize/3,                      0,    4*YSize/5,
       2*XSize/3,               YSize/5,    4*YSize/5,
    0,

// level 3 (27)
    4,
        0,                      YSize/4,      XSize/4,
       3*XSize/4,               YSize/4,      XSize/4,
        0,                    3*YSize/4,      XSize/4,
       3*XSize/4,             3*YSize/4,      XSize/4,
    2,
        XSize/2,                      1,      YSize/4,
        XSize/2,        YSize-1-YSize/4,      YSize/4,
    0,
        
// level 4
    4,
             0,                 YSize/5,      XSize/2,
       XSize-1-XSize/2,       2*YSize/5,      XSize/2,
             0,               3*YSize/5,      XSize/2,
       XSize-1-XSize/2,       4*YSize/5,      XSize/2,
    0,
    0,
// level 5 (Z88DK partial work-around for preprocessor bug on division with integer litterals)
    4,
        ((uint8_t) (XSize/4)),                ((uint8_t) (YSize/4)),              ((uint8_t) (XSize/5))+1,
        XSize-((uint8_t) (XSize/4))-((uint8_t) (XSize/5)),        ((uint8_t) (YSize/4)),      ((uint8_t) (XSize/5))+1, // (1)
        ((uint8_t) (XSize/4)),        YSize-1-((uint8_t) (YSize/4)),              ((uint8_t) (XSize/5))+1,
        XSize-((uint8_t) (XSize/4))-((uint8_t) (XSize/5)),YSize-1-((uint8_t) (YSize/4)),      ((uint8_t) (XSize/5))+1, // (2)
    4,  
        ((uint8_t) (XSize/4)),                ((uint8_t) (YSize/4)),              ((uint8_t) (YSize/5)),
        XSize-((uint8_t) (XSize/4)),((uint8_t) (YSize/4)),                      ((uint8_t) (YSize/5)), // (1)
        ((uint8_t) (XSize/4)),        YSize-((uint8_t) (YSize/4))-((uint8_t) (YSize/5)),        ((uint8_t) (YSize/5)),
        XSize-((uint8_t) (XSize/4)),YSize-((uint8_t) (YSize/4))-((uint8_t) (YSize/5)),        ((uint8_t) (YSize/5)), // (2)
    0,
// level 6
    0,
    2,
        XSize/3,                      0,    4*YSize/5,
       2*XSize/3,               YSize/5,    4*YSize/5,
    2,
        1,1,XSize/8,YSize/8,DEADLY,
        XSize-1-XSize/8,YSize-1-YSize/8,XSize/8,YSize/8,DEADLY,
// level 7 (109)
    2,
        0, YSize/2, XSize/4,
        XSize-XSize/4, YSize/2, XSize/4,
    0,
    2,
        XSize/2-XSize/8,1,XSize/4,YSize/4,DEADLY,
        XSize/2-XSize/8,YSize-1-YSize/4,XSize/4,YSize/4,DEADLY,
// level 8
    0,
    4, 
        XSize/6, 3, YSize-1-6,
        2*XSize/6, 3, YSize-1-6,
        XSize-1-2*XSize/6, 3, YSize-1-6,
        XSize-1-XSize/6, 3, YSize-1-6,
    0,
// level 9 (143)
    0,
    0,
    4,
        2,2,XSize/3,YSize/3,WALL,
        2,YSize-2-YSize/3,XSize/3,YSize/3,WALL,
        XSize-2-XSize/3,YSize-2-YSize/3,XSize/3,YSize/3,WALL,
        XSize-2-XSize/3,2,XSize/3,YSize/3,WALL,
// level 10
    2, 
        XSize/3-1,       YSize/3,     XSize/2-1,
        XSize/3-1,     2*YSize/3,     XSize/2-1,
    0,
    0,
// level 11
    2,
        XSize/4, YSize/2-2,XSize/2,
        XSize/4, YSize/2+2,XSize/2,
    0,
    4,
        2,2,XSize/4,YSize/4,WALL,
        2,YSize-2-YSize/4,XSize/4,YSize/4,WALL,
        XSize-2-XSize/4,YSize-2-YSize/4,XSize/4,YSize/4,WALL,
        XSize-2-XSize/4,2,XSize/4,YSize/4,WALL,
// level 12
    1,
        2,YSize/2-1,XSize-4,
    0,
    0,
// level 13 (210)
    0,
    2,
         XSize/5,                      1,   4*YSize/5,
       4*XSize/5,                      YSize-1-4*YSize/5,   4*YSize/5,
    0,
// level 14 (219)
    4,
        0,                      YSize/4,      XSize/4+1,
       3*XSize/4,               YSize/4,      XSize/4,
        0,                    3*YSize/4,      XSize/4+1,
       3*XSize/4,             3*YSize/4,      XSize/4,
    0,
    0,
// level 15 (234)
    1,
        5,YSize/2+1,XSize-10,
    0,
    1,
        XSize/2-2,YSize/2+3,4,4,DEADLY,
// level 16 (245)
    0,
    2,
        XSize/3, YSize-1-YSize/3, YSize/3,
        XSize-1-XSize/3, YSize-1-YSize/3, YSize/3,
    0,
};


const uint8_t level_walls_index[] = 
    {
        0,   // 0
        3,   // 1
        18,  // 2
        27,  // 3
        48,  // 4
        63,  // 5
        90,  // 6
        109, // 7
        128, // 8
        143, // 9
        166, // 10
        175, // 11
        204, // 12
        210, // 13
        219, // 14
        234, // 15
        245, // 16
        };


// HORIZONTAL MINES
const uint8_t horizontal_mines_on_level[] = 
    {
        2, // 0  (0)
            YSize/5,
            4*YSize/5,
        0, // 1  (3)
        0, // 2  (4)
        2, // 3  (5)
            YSize/2 - 3,
            YSize/2 + 3,
        3, // 4  (8)
            YSize/2 - 1,
            2,
            YSize-3,
        0, // 5  (12)
        3, // 6  (13)
            2,
            YSize-3,
            YSize/2-2,
        0, // 7  (17)
        2, // 8  (18)
            2,
            YSize-3,
        1, // 9  (21)
            YSize/2-1,
        2, // 10 (23)
            YSize/3 - 2,
            2*YSize/3+2,
        2, // 11 (26)
            YSize/2 - 3,
            YSize/2 + 3,
        3, // 12 (29)
            YSize/2 - 5,
            YSize/2 + 3,
            YSize/2 + 6,
        2, // 13 (33)
            2,
            YSize-3,
        4, // 14 (36)
            YSize/2 - 2,
            YSize/2 + 2,
            YSize/2 - 4,
            YSize/2 + 4,
        3, // 15 (41)
            YSize/2 - 3,
            YSize/2 - 4,
            YSize/2 - 5,
        3, // 16 (45)
            YSize/2 - 4,
            YSize/2 - 5,
            YSize/2 - 6,
        2, // 17 (49)
            YSize/2 - 3,
            YSize/2 + 3,
        2, // 18 (52)
            3,
            YSize - 3,
        4, // 19 (55)
            YSize/2 - 3,
            YSize/2 + 3,
            YSize/2 - 2,
            YSize/2 + 2,
        4, // 20 (60)
            YSize/2 - 1,
            YSize/2 + 1,
            2,
            YSize-3,
        2, // 21 (65)
            3,
            YSize - 3,
        4, // 22 (68)
            2,
            YSize-3,
            YSize/2-3,
            YSize/2+3,
        2, // 23 (73)
            YSize/2 - 3,
            YSize/2 + 2,
        3, // 24 (76)
            2,
            YSize-3,
            YSize/2 - 3,
        2, // 25 (80)
            YSize/2+1,
            YSize/2-1,
        4, // 26 (83)
            2,
            YSize-3,
            4,
            YSize-5,
        4, // 27 (88)
            YSize/2 - 3,
            YSize/2-1,
            YSize/2+1,
            YSize/2 + 3,
        4, // 28 (93)
            3,
            YSize - 4,
            5,
            YSize - 6,
        2, // 29 (98)
            2,
            YSize-3,
        4, // 30 (101)
            YSize/2 - 2,
            YSize/2 + 2,
            YSize/2 - 4,
            YSize/2 + 4,
        4, // 31 (106)
            YSize/2 - 4,
            YSize/2 - 5,
            YSize/2 - 6,
            YSize/2 - 7,
        4, // 32 (111)
            YSize/2 - 5,
            YSize/2 - 6,
            YSize/2 - 7,
            YSize/2 - 8,
    };


const uint8_t horizontal_mines_on_level_index[] =
    {
        0,  //  0
        3,  //  1
        4,  //  2
        5,  //  3
        8,  //  4
        12, //  5
        13, //  6
        17, //  7
        18, //  8
        21, //  9
        23, // 10
        26, // 11
        29, // 12
        33, // 13
        36, // 14
        41, // 15
        45, // 16
        49, // 17
        52, // 18
        55, // 19
        60, // 20
        65, // 21
        68, // 22
        73, // 23
        76, // 24
        80, // 25
        83, // 26
        88, // 27
        99, // 28
        98, // 29
       101, // 30
       106, // 31
       111  // 32
    };


// VERTICAL MINES
const uint8_t vertical_mines_on_level[] = 
    {
        1, // 0  (0)
            XSize/2,
        1, //  1 (2)
            XSize/2-1,
        1, //  2 (4)
            XSize/2-1,
        0, //  3 (6) 
        0, //  4 (7)
        2, //  5 (8)
            XSize/2,
            XSize/2-1,
        1, //  6 (11)
            XSize/2-1,
        2, //  7 (13)
            XSize/2+1,
            XSize/2-2,
        1, //  8 (16)
            XSize/2,
        2, //  9 (18)
            XSize/2+1,
            XSize/2-1,
        1, // 10 (21)
            XSize/2-1,
        0, // 11 (23)
        0, // 12 (24)
        1, // 13 (25)
            XSize/2+1,
        0, // 14 (27)
        1, // 15 (28)
            XSize/2-1,
        1, // 16 (30),
            XSize/2-1,
        1, // 17 (32)
            XSize/2-1,
        1, // 18 (34)
            XSize/2,
        0, // 19 (36) 
        0, // 20 (37)
        2, // 21 (38)
            XSize/2,
            XSize/2-1,
        1, // 22 (41)
            XSize/2-1,
        2, // 23 (43)
            XSize/2+1,
            XSize/2-1,
        1, // 24 (46)
            XSize/2,
        2, // 25 (48)
            XSize/2,
            XSize/2-1,
        1, // 26 (51)
            XSize/2,
        0, // 27 (53)
        0, // 28 (54)
        2, // 29 (55)
            XSize/2+1,
            XSize/2-1,
        1, // 30 (58)
            XSize/2,
        1, // 31 (60)
            XSize/2,
        2, // 32 (62),
            XSize/6,
            XSize-1-XSize/6
    };


const uint8_t vertical_mines_on_level_index[] =
    {
         0,
         2,
         4,
         6,
         7,
         8,
        11,
        13, // 7
        16, // 8
        18, // 9
        21, // 10
        23, // 11
        24, // 12
        25, // 13
        27, // 14
        28, // 15
        30, // 16
        32, // 17
        34, // 18
        36, // 19
        37, // 20
        38, // 21
        41, // 22
        43, // 23
        46, // 24
        48, // 25
        51, // 26
        53, // 27
        54, // 28
        55, // 29
        58, // 30
        60, // 31
        62  // 32
    };


#endif // _LEVELS_H

