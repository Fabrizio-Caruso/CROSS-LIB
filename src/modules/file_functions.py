from os import walk


def files_in_path(mypath):
    files = []
    for (_, _, filenames) in walk(mypath):
        files.extend(filenames)
        break
    return files