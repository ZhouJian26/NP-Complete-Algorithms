import utils.io as uio
import utils.build as uBuild
import utils.pandas as uPandas

import pandas as pd
import time


def readFileHandler(f):
    mapInfo = {
        "size": uio.readLine(f),
        "map": []
    }

    mapInfo["rangeRows"] = int(mapInfo["size"][1])
    mapInfo["rangeColumns"] = int(mapInfo["size"][0])
    mapInfo["map"] = uio.readMap(f, mapInfo["rangeRows"])

    staffInfo = {
        "developers": int(uio.readLine(f)[0])
    }

    staffInfo["developersInfo"] = [[
        developer[0],
        int(developer[1]),
        developer[3:]
    ] for developer in uio.readTable(f, staffInfo["developers"])]

    staffInfo["managers"] = int(uio.readLine(f)[0])
    staffInfo["managersInfo"] = uio.readTable(f, staffInfo["managers"])

    f.close()

    return {
        "mapInfo": mapInfo,
        "staffInfo": staffInfo
    }


def main():
    """
    files = uio.getPathFiles("Input/*")

    fileData = uio.readFile(files[0], readFileHandler)

    start = time.time()
    parsedDeveloperSkills = uPandas.objToPD(
        [developer[2] for developer in fileData["staffInfo"]["developersInfo"]])

    for i in range(len(parsedDeveloperSkills["parsedData"])):
        fileData["staffInfo"]["developersInfo"][i][2:] = parsedDeveloperSkills["parsedData"][i]

    end = time.time()

    print(pd.DataFrame(
        fileData["staffInfo"]["developersInfo"],
        columns=['company', 'bonus'] +
        list(parsedDeveloperSkills["flags"])
    ))
    """

    files = uio.getPathFiles("Input/*")

    fileData = uio.readFile(files[0], readFileHandler)
    messoData = [developer[:2]+[len(developer[2])]+developer[2]
                 for developer in fileData["staffInfo"]["developersInfo"]]

    start = time.time()
    parsedMesso = uPandas.convert_developers_toDict(messoData)
    end = time.time()
    print(end - start)


if __name__ == "__main__":
    main()
