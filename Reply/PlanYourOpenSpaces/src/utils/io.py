import glob
import os
import json


def getPathFiles(path):
    return glob.glob(path)


def createFolder(name):
    try:
        os.mkdir(name)
    except OSError:
        print("Creation of the directory failed path: " + name)
    else:
        print("Successfully created directory: " + name)


def readFile(path, parser):
    filePointer = open(path, "r")
    result = parser(filePointer)
    filePointer.close()
    return result


def printJsonObject(obj):
    print(json.dumps(obj, indent=4))


def escapeReturn(line):
    return line[:-1]


def readMap(filePointer, rows):
    result = []
    for i in range(int(rows)):
        result.append(escapeReturn(filePointer.readline()))
    return result


def readTable(filePointer, rows, splitter=" "):
    result = [row.split(splitter) for row in readMap(filePointer, rows)]
    return result


def readLine(filePointer, splitter=" "):
    return escapeReturn(filePointer.readline()).split(splitter)
