from os import walk


def dirs_in_path(mypath):
    dirs = []
    for (dirpath, dirnames, filenames) in walk(mypath):
        dirs.extend(dirnames)
        break
    return dirs

game_projects = dirs_in_path("./games")
example_projects = dirs_in_path("./examples")

    
        