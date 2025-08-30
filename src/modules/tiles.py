
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

