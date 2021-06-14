import argparse
import os
import hashlib
#taking all the flags and set their defult value and also taking directories
parser = argparse.ArgumentParser()
parser.add_argument("-d", action="store_true", default=False)
parser.add_argument("-f", action="store_true", default=True)
parser.add_argument("-c", action="store_true", default=False)
parser.add_argument("-n", action="store_true", default=False)
parser.add_argument("-s", action="store_true", default=False)
parser.add_argument('directorys', nargs=argparse.REMAINDER)
args = parser.parse_args()
olddirList = args.directorys
dirList = []
#for converting relative paths to absolute paths
for abc in olddirList:
    dirList.append(os.path.abspath(abc))

#if there is no directory in command line argument it sets direktory to current dir
if len(dirList) == 0:
    dirList.append(".")

def getDirSize(direktory,sizeofDirs):
    tempsize = 0
    for dirs in os.listdir(direktory):
        path2 = os.path.join(direktory,dirs)
        if os.path.isdir(path2):
            tempsize += getDirSize(path2,sizeofDirs)
        else:
            tempsize += os.path.getsize(path2)
    sizeofDirs[direktory] = tempsize
    return tempsize


#traverses directories for -c flag recursively
def traverse_dir_for_C(direktory, setofDirs):
    temp = []
    for dirs in os.listdir(direktory):
        path1 = os.path.join(direktory, dirs)
        if os.path.isdir(path1):
            temp.append(traverse_dir_for_C(path1, setofDirs))
        else:
            with open(path1, "rb") as f:
                file_hash = hashlib.sha256(f.read()).hexdigest()
                temp.append(file_hash)
    temp.sort()
    lastVersion = ""
    #concatenates all elements in the list
    for elem in temp:
        lastVersion += elem
    #adds hashed value to dict
    hashed_dir = hashlib.sha256(lastVersion.encode()).hexdigest()
    if hashed_dir in setofDirs:
        if direktory not in setofDirs[hashed_dir]:
            setofDirs[hashed_dir].append(direktory)
    else:
        setofDirs[hashed_dir] = [direktory]
    return hashed_dir

#traverses directories for -n flag recursively
def traverse_dir_for_N(direktory, setofDirs):
    temp = []
    for dirs in os.listdir(direktory):
        path1 = os.path.join(direktory, dirs)

        if os.path.isdir(path1):
            temp.append(traverse_dir_for_N(path1, setofDirs))
        else:
            file_hash = hashlib.sha256(dirs.encode()).hexdigest()
            temp.append(file_hash)

    temp.sort()
    lastVersion = hashlib.sha256(
        os.path.basename(direktory).encode()).hexdigest()

    for elem in temp:
        lastVersion += elem
    hashed_dir = hashlib.sha256(lastVersion.encode()).hexdigest()
    if hashed_dir in setofDirs:
        if direktory not in setofDirs[hashed_dir]:
            setofDirs[hashed_dir].append(direktory)
    else:
        setofDirs[hashed_dir] = [direktory]
    return hashed_dir

#traverses directories for -cn flag recursively
def traverse_dir_for_CN(direktory, setofDirs):
    temp = []
    for dirs in os.listdir(direktory):
        path1 = os.path.join(direktory, dirs)
        if os.path.isdir(path1):
            temp.append(traverse_dir_for_CN(path1, setofDirs))
        else:
            with open(path1, "rb") as f:
                file_hash = hashlib.sha256(f.read()).hexdigest()
            name_hash = hashlib.sha256(dirs.encode()).hexdigest()
            composit = file_hash + name_hash
            temp.append(composit)
    temp.sort()
    lastVersion = hashlib.sha256(
        os.path.basename(direktory).encode()).hexdigest()

    for elem in temp:
        lastVersion += elem

    hashed_dir = hashlib.sha256(lastVersion.encode()).hexdigest()
    if hashed_dir in setofDirs:
        if direktory not in setofDirs[hashed_dir]:
            setofDirs[hashed_dir].append(direktory)
    else:
        setofDirs[hashed_dir] = [direktory]
    return hashed_dir


# if -d flag set to true than this block calls necessary function
if args.d:
    hash_dir = {}
    sizeofDirs = {}
    if args.c and args.n:
        for givenDirs in dirList:
            traverse_dir_for_CN(givenDirs, hash_dir)
        
        if args.s:
            getDirSize(givenDirs,sizeofDirs)
            newlist = []
            newHash ={}
            sortedList = []
            for hashes in hash_dir:
                if len(hash_dir[hashes]) > 1:
                    newlist.append(hash_dir[hashes][0])
                    newHash[hash_dir[hashes][0]] = sizeofDirs[hash_dir[hashes][0]]
            for w in sorted(newHash, key=newHash.get, reverse=True):
                sortedList.append(w)
            for smalllist in sortedList:
                for key, value in hash_dir.items():
                    if value[0]== smalllist:
                        a = hash_dir[key]
                        a.sort()
                        for b in a:
                            print(b,"\t",sizeofDirs[b])
                        print("\n")
        else:
            for hashes in hash_dir:
                if len(hash_dir[hashes]) > 1:
                    dups = hash_dir[hashes]
                    dups.sort()
                    for dup in dups:
                        print(dup)
                    print("\n")
    elif args.n:
        for givenDirs in dirList:
            traverse_dir_for_N(givenDirs, hash_dir)
        
        for hashes in hash_dir:
            if len(hash_dir[hashes]) > 1:
                dups = hash_dir[hashes]
                dups.sort()
                for dup in dups:
                    print(dup)
                print("\n")
    else:
        for givenDirs in dirList:
            traverse_dir_for_C(givenDirs, hash_dir)
        if args.s:
            getDirSize(givenDirs,sizeofDirs)
            newlist = []
            newHash ={}
            sortedList = []
            for hashes in hash_dir:
                if len(hash_dir[hashes]) > 1:
                    newlist.append(hash_dir[hashes][0])
                    newHash[hash_dir[hashes][0]] = sizeofDirs[hash_dir[hashes][0]]
            for w in sorted(newHash, key=newHash.get, reverse=True):
                sortedList.append(w)
            for smalllist in sortedList:
                for key, value in hash_dir.items():
                    if value[0]== smalllist:
                        a = hash_dir[key]
                        a.sort()
                        for b in a:
                            print(b,"\t",sizeofDirs[b])
                        print("\n")    
        else:
            for hashes in hash_dir:
                if len(hash_dir[hashes]) > 1:
                    dups = hash_dir[hashes]
                    dups.sort()
                    for dup in dups:
                        print(dup)
                    print("\n")

#if -f flag set to true than this block executes its code and prints duplicate files
else:
    hash_file = {}
    #this for loop finds all duplicate files
    for directorys in dirList:
        for root, dirs, files in os.walk(directorys):
            for file in files:
                if args.c and args.n:
                    if len(files) != 0:
                        path = os.path.join(root, file)
                        with open(path, "rb") as f:
                            content = f.read()
                        hash_content = hashlib.sha256(content).hexdigest()
                        nameHash = hashlib.sha256(file.encode()).hexdigest()
                        cnHash = hash_content + nameHash
                        if cnHash in hash_file:
                            if path not in hash_file[cnHash]:
                                hash_file[cnHash].append(path)
                        else:
                            hash_file[cnHash] = [path]
                elif args.n:
                    if len(files) != 0:
                        path = os.path.join(root, file)
                        hash_content = hashlib.sha256(file.encode()).hexdigest()
                        if hash_content in hash_file:
                            if path not in hash_file[hash_content]:
                                hash_file[hash_content].append(path)
                        else:
                            hash_file[hash_content] = [path]
                else:
                    if len(files) != 0:
                        path = os.path.join(root, file)
                        with open(path, "rb") as f:
                            content = f.read()
                        hash_content = hashlib.sha256(content).hexdigest()
                        if hash_content in hash_file:
                            if path not in hash_file[hash_content]:
                                hash_file[hash_content].append(path)
                        else:
                            hash_file[hash_content] = [path]
    #this if and else statements are just for printing duplicate files 
    if args.c and args.n:
        if args.s:  # print ederken alfabetik yapmıyor olabilirim ona bak
            newPaths =[]
            for hashes in hash_file:
                if len(hash_file[hashes]) > 1:
                    newPaths.append(hash_file[hashes][0])
            newPaths.sort(key=lambda f: os.stat(f).st_size, reverse=True)
            for smallPath in newPaths:
                for key, value in hash_file.items():
                    if value[0]== smallPath:
                        a = hash_file[key]
                        a.sort()
                        for b in a:
                            print(b,"\t",os.path.getsize(b))
                        print("\n")
        else:
            for hashes in hash_file:
                if len(hash_file[hashes]) > 1:
                    a = hash_file[hashes]
                    a.sort()
                    for lp in a:
                        print(lp)
                    print("\n")
    elif args.n:
        for hashes in hash_file:
            if len(hash_file[hashes]) > 1:
                dups = hash_file[hashes]
                dups.sort()
                for dup in dups:
                    print(dup)
                print("\n")
    else:
        if args.s:
            newPaths1 =[]
            for hashes in hash_file:
                if len(hash_file[hashes]) > 1:
                    newPaths1.append(hash_file[hashes][0])
            newPaths1.sort(key=lambda f: os.stat(f).st_size, reverse=True)
            for smallPath in newPaths1:
                for key, value in hash_file.items():
                    if value[0]== smallPath:
                        a = hash_file[key]
                        a.sort()
                        for b in a:
                            print(b,"\t",os.path.getsize(b))
                        print("\n")
        else:
            for hashes in hash_file:
                if len(hash_file[hashes]) > 1:
                    dups = hash_file[hashes]
                    dups.sort()
                    for dup in dups:
                        print(dup)
                    print("\n")
