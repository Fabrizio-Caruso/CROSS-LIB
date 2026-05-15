
## ORIC SUPPORT
I have recently greatly improved the support for the Oric 1 and Oric Atmos machines with 3 new video modes:
- `oric_mono` target: high resolution monochrome mode with 6x8 tiles in a 40x25 screen
- `oric` target: quad pixel resolution with color 6x8 tiles in a 19x12 or 19x13 screen (depending on game configuration)
- `oric_39x25` target: high resolution mode with limited color support  

### Monochrome mode support
The `oric_mono` target will use hardware text mode in Cross-Lib and hardware high resolution mode in Cross-Lib-HD (in order to provide 128 total tiles).

### Standard color mode
The `oric` target uses hardware high resolution mode and provides 64 or 128 total tiles with colors.
Colors are currently produced by having alternating cyan and yellow attributes.
Inversion is performed on fonts to render red and white colors.
Inversion on redefinable tiles is possible if the `__INVERSE_TILES` is set in the `ORIC_COLOR_OPTS` variable in `project_config.mk`.
Pixels have quadruple size compared to hardware high resolution and only 19x12 tiles are displayed on the screen.
If `Force_YSize=13` is set in `ORIC_COLOR_OPTS` then an extra monochromatic half-height line is displayed on the bottom of the screen.

### High-resolution mode with limited color support
Thr `oric_39x25` (alias `oric_alt`) uses the high resolution mode and provides 64 or 128 total tiles with limited colors.
We do define alternating cyan and yellow attributes but each line of each tile is either on a cyan line or a yellow line. We cannot just display even or odd lines without losing half the details of our tiles.
So we are forced to use color inversion more heavily if we want to get more colors. 

Colors are enabled by default on fonts. They be disabled if `__MONO_TEXT` is set in `ORIC_39x25_COLOR_OPTS`.
Colors can be enabled on redefinable tiles if the `__INVERSE_TILES` is set in the `ORIC_39x25_COLOR_OPTS` variable in `project_config.mk`.
Colors on such small fonts and tiles may not look great because they rely on color inversion. This is why Cross-Lib provides ways to disable colors. 



