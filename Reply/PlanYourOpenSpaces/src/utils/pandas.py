import pandas as pd

import utils.io as uio


def objToPD(data):
    flagList = {
        flag: index for index, flag in enumerate(
            list(dict.fromkeys([flag for row in data for flag in row]))
        )}

    result = []

    zeroVector = [0 for i in range(len(flagList))]

    for row in data:
        parsedRow = zeroVector[:]
        for flag in row:
            parsedRow[flagList[flag]] = 1
        result.append(parsedRow)

    return {
        "flags": flagList,
        "parsedData": result
    }


def convert_developers_toDict(devs):
    devs_dict = dict()

    devs_dict["id"] = list()
    devs_dict["bonus"] = list()
    devs_dict["nSkills"] = list()

    for i, dev in enumerate(devs):
        devs_dict["id"].append(dev[0])
        devs_dict["bonus"].append(dev[1])
        devs_dict["nSkills"].append(dev[2])

        skills = dev[3:]
        for skill in skills:
            if skill in devs_dict.keys():
                padding = [0] * (i - len(devs_dict[skill]))
                devs_dict[skill] = devs_dict[skill] + padding
                devs_dict[skill].append(1)
            else:
                devs_dict[skill] = [0] * (i)
                devs_dict[skill].append(1)

    for skill in list(devs_dict.keys())[3:]:
        padding = [0] * (len(devs_dict["id"]) - len(devs_dict[skill]))
        devs_dict[skill] = devs_dict[skill] + padding

    return devs_dict
