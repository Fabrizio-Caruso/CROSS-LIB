
# global extend_algorithm


TARGETS_WITH_XSIZE_6 = {"oric", "atmos", "comx_ntsc", "comx", "pecom", "micro"}
TARGETS_WITH_XSIZE_7 = {"apple2", "apple2enh"}

TARGETS_WITH_YSIZE_6 = {"atari_lynx"}
TARGETS_WITH_YSIZE_9 = {"comx", "micro", "pecom"}
TARGETS_WITH_YSIZE_8 = {"comx_ntsc", "micro_ntsc"} # Necessary to avoid comx and micro


def detect_xsize(target):
    for target_6_xsize in TARGETS_WITH_XSIZE_6:
        if target.startswith(target_6_xsize):
            return 6
    for target_7_xsize in TARGETS_WITH_XSIZE_7:
        if target.startswith(target_7_xsize):
            return 7
    return 8


# TODO: VG-5000 has 8x10 characters but it is currently treated as 8x8
# TODO: Cidelsa has 8x6 (rotated 6x8 in reality)
# comx_ntsc : 6x8
# comx (pal): 6x9
def detect_ysize(target):
    for target_8_ysize in TARGETS_WITH_YSIZE_8: # Necessary to have this before size 9 for NTSC case (comx, micro)
        if target.startswith(target_8_ysize):
            return 8
    for target_6_ysize in TARGETS_WITH_YSIZE_6:
        if target.startswith(target_6_ysize):
            return 6
    for target_9_ysize in TARGETS_WITH_YSIZE_9:
        if target.startswith(target_9_ysize):
            return 9
    return 8


# General strategies:
# xsize or ysize < 8 -> cut left/right or top/bottom columns/rows
# ysize > 8          -> repeat top/bottom rows 
def convert(tile_vect,xsize,ysize, extend_algorithm):

    if xsize==7:
        tile_vect = [vect[:-1] for vect in tile_vect]
    if xsize==6:
        tile_vect = [(vect[:-1])[1:] for vect in tile_vect]

    if(ysize==6):
        tile_vect = (tile_vect[:-1])[1:]
    # else:
        # print("error")
        # print(extend_to_smaller)
        # return
    # TODO: Implement switch row/column instead of cut
    # elif extend_to_smaller=="switch":
        # if xsize==7:
            # tile_vect = [vect[:-1] for vect in tile_vect]
        # if xsize==6:
            # tile_vect = [(vect[:-1])[1:] for vect in tile_vect]

        # if(ysize==6):
            # tile_vect = (tile_vect[:-1])[1:]    

    # TODO: Implement zero
    if extend_algorithm=="duplicate":
        if(ysize==9):
            tile_vect = tile_vect + [ tile_vect[-1]]
        if(ysize==10):
            tile_vect = tile_vect[0:1] + tile_vect + [ tile_vect[-1]]
    elif extend_algorithm=="zero":
        zero_row = [ "." * xsize ]
        if(ysize==9):
            tile_vect = tile_vect + zero_row
        if(ysize==10):
            tile_vect = zero_row + tile_vect + zero_row
    return tile_vect

