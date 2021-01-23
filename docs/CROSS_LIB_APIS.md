
# CROSS-LIB APIs
===========================================================================================================================================


## GRAPHICS



- `\_XL\_INIT\_GRAPHICS()`

It initializes the graphics. Necessary for any build that wants to show anything on the screen.



- `XSize`: Number of tiles that can be displayed horizontally on the (usable) screen


- `YSize`: Number of tiles that can be displayed vertically on the (usable) screen


- `_TILE\_1`, ..., `\_TILE\_24`, `\_DOLLAR\_TILE`

Displayable tiles with \_XL\_DRAW



- `\_XL\_BLACK`, `\_XL\_WHITE`, `\_XL\_RED`, `\_XL\_CYAN`, `\_XL\_GREEN`, `\_XL\_YELLOW`, `\_XL\_BLUE`

Currently available colors


- `\_XL\_DRAW(X, Y, T, C)`

It displays tile T at position (X,Y) with color C if color C is available or its "closest" match when C is not supported by a target.


- `\_XL\_DELETE(X,Y)`

It deletes a tile-sized area of the screen at position (X,Y)


- `\_XL\_CLEAR\_SCREEN()`

It clears the whole screen.


- `\_XL\_WAIT\_VSYNC()`

It waits for the vertical synch if this is supported by the target. It does nothing otherwise.


## TEXT



A "Cross-Lib string" is string formed with only the following 37 possible characters
`\_XL\_SPACE`, `\_XL\_a`, ..., `\_XL\_z`, `\_XL\_A`, ..., `\_XL\_Z`, `'0'`, ..., `'9'`
where capital/small letter may be displayed as the same character depending on whether the target supports different display for capital vs small letters.

- `\_XL\_PRINT(X,Y,S)`

It prints the "Cross-Lib string" S starting a position (X,Y).


- `\_XL\_PRINT\_CENTERED(S)`

Same as `XL\_PRINT` but it just displays the "Cross-Lib string" S at the center of the screen

- `\_XL\_PRINT\_CENTERED\_ON\_ROW(Y,S)`

Same as `XL\_PRINT` but it just displays the "Cross-Lib string" S centered on row Y


- `\_XL\_PRINT\_CENTERED\_ON\_ROW\_WITH\_COLOR(Y,C,S)`

Same as `\_XL\_PRINT\_CENTERED\_ON\_ROW` but with color C (if the target supports text color)


- `\_XL\_PRINT\_CENTERED\_WITH\_COLOR(C,S)`

Same as `\_XL\_PRINT\_CENTERED` but with color C (if the target supports text color)


- `\_XL\_PRINTD(X,Y,L,N)`

It prints the positive integer number N in L digits (1<=L<=5) starting at position (X,Y)


- `\_XL\_SET\_TEXT\_COLOR(C)`

It sets the text color to C. Such color will be used on next PRINT and PRINTD for targets that support colored text.
If any other display operation is present (e.g., `\_XL\_DRAW`), then there is no guarantee that the color C will be used afterwards.


## INPUT



- `\_XL\_INIT\_INPUT()`

It initializes the input (joystick/keyboard). Necessary for any program that needs to receive an input



- `\_XL\_INPUT()`

It returns a value J that represents the specific received input or absence of input at the moment of its execution (keyboard may be buffered, though).
The result J is meant to be used with the Boolean functions below.


- `\_XL\_UP(J)`, `\_XL\_DOWN(J)`, `\_XL\_LEFT(J)`, `\_XL\_RIGHT(J)`, `\_XL\_FIRE(J)`

These Boolean functions return 1 (true) if J represents a specific movement or fire button/key, and 0 (false) otherwise.


- `\_XL\_KEY\_PRESSED()`

This Boolean function returns 1 (true) if any key or fire button is pressed.



- `\_XL\_WAIT\_FOR\_INPUT()`

It waits for any key or fire button to be pressed.



## SOUND



- `\_XL\_INIT\_SOUND()`

It initializes the sound system. Necessary for any program that produces sound.



- `\_XL\_ZAP\_SOUND()`

Long rising sound


- `\_XL\_PING\_SOUND()`

Short high-pitched sound


- `\_XL\_TICK\_SOUND()`

Very short high-pitched sound


- `\_XL\_TOCK\_SOUND()`

Very short low-pitched sound


- `\_XL\_EXPLOSION\_SOUND()`

Long noise


- `\_XL\_SHOOT\_SOUND()`

Short noise



## SLEEP


- `\_XL\_SLEEP(S)`

It waits  S seconds or an approximations of S seconds depending on the target


- `\_XL\_SLOW\_DOWN(T)`

It waits T loops (0<=T<=65535)



## RANDOM


- `\_XL\_RAND()`

It generates a pseudo-random number in the range \[0,32767\]


