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
        
// level 4 (48)
    0,
    2,
        XSize/3,                      0,    4*YSize/5,
       2*XSize/3,               YSize/5,    4*YSize/5,
    2,
        1,1,XSize/8,YSize/8,DEADLY,
        XSize-1-XSize/8,YSize-1-YSize/8,XSize/8,YSize/8,DEADLY,

// level 5 (67)
    4,
             0,                 YSize/5,      XSize/2,
       XSize-1-XSize/2,       2*YSize/5,      XSize/2,
             0,               3*YSize/5,      XSize/2,
       XSize-1-XSize/2,       4*YSize/5,      XSize/2,
    0,
    0,

// level 6 (82) (Z88DK partial work-around for preprocessor bug on division with integer litterals)
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

// level 7 (109)
    2,
        0, YSize/2, XSize/4,
        XSize-XSize/4, YSize/2, XSize/4,
    0,
    2,
        XSize/2-XSize/10,1,XSize/5,YSize/5,DEADLY,
        XSize/2-XSize/10,YSize-1-YSize/5,XSize/5,YSize/5,DEADLY,
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
// level 12(204)
    0,
    2,
         XSize/5,                      1,   4*YSize/5,
       4*XSize/5,                      YSize-1-4*YSize/5,   4*YSize/5,
    0,
// level 13 (213)
    1,
        2,YSize/2-1,XSize-4,
    0,
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
        67,  // 5
        82,  // 6
        109, // 7
        128, // 8
        143, // 9
        166, // 10
        175, // 11
        204, // 12
        213, // 13
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
        2, // 4 (8)
            2,
            YSize-3,
        3, // 5 (11)
            YSize/2 - 1,
            2,
            YSize-3,
        0, // 6  (15)
        0, // 7  (16)
        2, // 8  (17)
            2,
            YSize-3,
        1, // 9  (20)
            YSize/2-1,
        2, // 10 (22)
            YSize/3 - 2,
            2*YSize/3+2,
        2, // 11 (25)
            YSize/2 - 3,
            YSize/2 + 3,
        2, // 12 (28)
            2,
            YSize-3,
        3, // 13 (31)
            YSize/2 - 5,
            YSize/2 + 3,
            YSize/2 + 6,
        4, // 14 (35)
            YSize/2 - 2,
            YSize/2 + 2,
            YSize/2 - 4,
            YSize/2 + 4,
        3, // 15 (40)
            YSize/2 - 3,
            YSize/2 - 4,
            YSize/2 - 5,
        3, // 16 (44)
            YSize/2 - 4,
            YSize/2 - 5,
            YSize/2 - 6,
        2, // 17 (48)
            YSize/2 - 3,
            YSize/2 + 3,
        2, // 18 (51)
            3,
            YSize - 3,
        4, // 19 (54)
            YSize/2 - 3,
            YSize/2 + 3,
            YSize/2 - 2,
            YSize/2 + 2,
        4, // 20 (59)
            2,
            YSize-3,
            YSize/2-3,
            YSize/2+3,
        4, // 21(64)
            YSize/2 - 1,
            YSize/2 + 1,
            2,
            YSize-3,
        2, // 22(69)
            3,
            YSize - 3,
        2, // 23 (72)
            YSize/2 - 3,
            YSize/2 + 2,
        3, // 24 (75)
            2,
            YSize-3,
            YSize/2 - 3,
        2, // 25 (79)
            YSize/2+1,
            YSize/2-1,
        4, // 26 (82)
            2,
            YSize-3,
            4,
            YSize-5,
        4, // 27 (87)
            YSize/2 - 3,
            YSize/2-1,
            YSize/2+1,
            YSize/2 + 3,
        2, // 28 (92)
            2,
            YSize-3,
        4, // 29 (95)
            3,
            YSize - 4,
            5,
            YSize - 6,
        4, // 30 (100)
            YSize/2 - 2,
            YSize/2 + 2,
            YSize/2 - 4,
            YSize/2 + 4,
        4, // 31 (105)
            YSize/2 - 4,
            YSize/2 - 5,
            YSize/2 - 6,
            YSize/2 - 7,
        4, // 32 (110)
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
        11, //  5
        15, //  6
        16, //  7
        17, //  8
        20, //  9
        22, // 10
        25, // 11
        28, // 12
        31, // 13
        35, // 14
        40, // 15
        44, // 16
        48, // 17
        51, // 18
        54, // 19
        59, // 20
        64, // 21
        69, // 22
        72, // 23
        75, // 24
        79, // 25
        82, // 26
        87, // 27
        92, // 28
        95, // 29
       100, // 30
       105, // 31
       110  // 32
    };


// VERTICAL MINES
const uint8_t vertical_mines_on_level[] = 
    {
        1, //  0 (0)
            XSize/2,
        1, //  1 (2)
            XSize/2-1,
        1, //  2 (4)
            XSize/2-1,
        0, //  3 (6) 
        0, //  6 -> 4(7)
        0, //  4 -> 5(8)
        2, //  5 -> 6(9)
            XSize/2,
            XSize/2-1,
        2, //  7 (12)
            XSize/2+1,
            XSize/2-2,
        1, //  8 (15)
            XSize/2,
        2, //  9 (17)
            XSize/2+1,
            XSize/2-1,
        1, // 10 (20)
            XSize/2-1,
        0, // 11 (22)
        1, // 13 -> 12 (23)
            XSize/2+1,
        0, // 13 -> 12 (25)
        0, // 14 (26)
        1, // 15 (27)
            XSize/2-1,
        1, // 16 (29),
            XSize/2-1,
        1, // 17 (31)
            XSize/2-1,
        1, // 18 (33)
            XSize/2,
        0, // 19 (35)
        1, // 22 -> 20 (36)
            XSize/2-1,        
        0, // 20 -> 21 (38)
        2, // 21 -> 22 (39)
            XSize/2,
            XSize/2-1,
        2, // 23 (42)
            XSize/2+1,
            XSize/2-1,
        1, // 24 (45)
            XSize/2,
        2, // 25 (47)
            XSize/2,
            XSize/2-1,
        1, // 26 (50)
            XSize/2,
        0, // 27 (52)
        2, // 29 -> 28 (53)
            XSize/2+1,
            XSize/2-1,
        0, // 28 -> 29 (56)
        1, // 30 (57)
            XSize/2,
        1, // 31 (59)
            XSize/2,
        2, // 32 (61),
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
         9,
        12, // 7
        15, // 8
        17, // 9
        20, // 10
        22, // 11
        23, // 12
        25, // 13
        26, // 14
        27, // 15
        29, // 16
        31, // 17
        33, // 18
        35, // 19
        36, // 20
        38, // 21
        39, // 22
        42, // 23
        45, // 24
        47, // 25
        50, // 26
        52, // 27
        53, // 28
        56, // 29
        57, // 30
        59, // 31
        61  // 32
    };


#endif // _LEVELS_H

