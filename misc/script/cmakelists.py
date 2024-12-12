import argparse, glob, os

CMAKE_MINIMUM_VERSION = "3.21"
EXTENSIONS_TO_SORT = ['h', 'hpp', 'c', 'cpp']
EXTENSIONS_TO_EXCLUDE = ['.swp']
EMPTY_KEY = '/empty/'

def list_directory(dir_path):
    files = []
    dirs = []
    for f in os.listdir(dir_path):
        fpath = os.path.join(dir_path, f)
        if os.path.isfile(fpath) and (f != 'CMakeLists.txt'):
            files.append(f)
        if os.path.isdir(fpath):
            dirs.append(f)
    return files, dirs

def group_by_extension(files):
    m = {}
    for f in files:
        fname, key = os.path.splitext(f)
        if not fname.startswith('.') and not key in EXTENSIONS_TO_EXCLUDE:
            if key == '':
                key = EMPTY_KEY
            if not key in m: 
                m[key] = []
            m[key].append(f)
    return m

def sort_files(files):
    fmap = group_by_extension(files)
    keys = []
    for ext in EXTENSIONS_TO_SORT:
        if ext in fmap:
            keys.append(ext)
    if EMPTY_KEY in fmap:
        keys.append(EMPTY_KEY)
    s = list(fmap.keys())
    s = sorted(s)
    for k in s:
        if not k in keys:
            keys.append(k)
    out = []
    for key in keys:
        c = sorted(fmap[key])
        for f in c:
            out.append(f)
    return out

def generate_cmakelists(target, files, dirs):
    text = "cmake_minimum_required(VERSION {})\n".format(
            CMAKE_MINIMUM_VERSION)
    files = sort_files(files)
    if dirs:
        text += "\n"
        for d in dirs:
            text += "add_subdirectory({})\n".format(d)
    if files:
        text += "\ntarget_sources({} PRIVATE\n".format(target)
        for f in files:
            text += f + "\n"
        text += ")"
    return text

def process_directory(target, dir_path, recursive, verbose):
    files, dirs = list_directory(dir_path)
    new_text = generate_cmakelists(target, files, dirs)
    target_file = os.path.join(dir_path, 'CMakeLists.txt')
    old_text = ''
    if os.path.exists(target_file):
        with open(target_file, 'r') as f:
            old_text = f.read()
    if new_text != old_text:
        if verbose:
            print("Updating {}".format(target_file))
        with open(target_file, 'w') as f:
            f.write(new_text)
    if recursive:
        for d in dirs:
            if not d.startswith('.'):
                process_directory(target, os.path.join(dir_path, d),
                                  recursive, verbose)

def do_main():
    parser = argparse.ArgumentParser(description="generate CMakeLists.txt.")
    parser.add_argument('directories', metavar='<dir>', nargs='+',
                        type=str, help="a directory to update.")
    parser.add_argument('-t', '--target', metavar='<name>', required=True,
                        type=str, help="a target to append source files.")
    parser.add_argument('-r', '--recursive', action='store_true',
                        help="update directories recursively.")
    parser.add_argument('-v', '--verbose', action='store_true',
                        help="print updated files.")

    args = parser.parse_args()
    if not args.directories:
        print("directory is required.\n")
        exit(-1)
    if not args.target:
        print("target is required.\n")
        exit(-1)
    for d in args.directories:
        process_directory(args.target, d, args.recursive, args.verbose)

if __name__ == '__main__':
    do_main()
