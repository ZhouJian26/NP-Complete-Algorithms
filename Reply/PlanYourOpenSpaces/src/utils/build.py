import os
import zipfile
import re

import utils.io as uio


def zipdir(name, path, ignoreDir=["__pycache__"]):
    print("Zipping...")

    zipPointer = zipfile.ZipFile(name+".zip", 'w', zipfile.ZIP_DEFLATED)
    status = {}
    for root, dirs, files in os.walk(path):
        if(bool(re.findall("({})".format("|".join(ignoreDir)), root))):
            print("Skipped: {}".format(root))
            continue
        status[root] = []
        for file in files:
            status[root].append(file)
            zipPointer.write(os.path.join(root, file), os.path.relpath(
                os.path.join(root, file), os.path.join(path, '..')))

    zipPointer.close()

    print("Zip status:")
    uio.printJsonObject(status)
