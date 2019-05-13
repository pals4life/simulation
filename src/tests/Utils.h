//============================================================================
// @name        : Utils.h
// @author      : Ward Gauderis
// @date        : 5/13/19
// @version     : 2.0
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : Utils for input/output
//============================================================================
#ifndef SIMULATION_UTILS_H
#define SIMULATION_UTILS_H

#include <string>
#include <fstream>

// source: Serge Demeyer - TicTactToe in C++, Ansi-style
bool FileCompare(const std::string& leftFileName, const std::string& rightFileName) {
    std::ifstream leftFile, rightFile;
    char leftRead, rightRead;
    bool result;leftFile.open(leftFileName.c_str());
    if (!leftFile.is_open()) {
        return false;
    }
    rightFile.open(rightFileName.c_str());
    if (!rightFile.is_open()) {
        leftFile.close();
        return false;
    }
    result = true;
    while (result && leftFile.good() && rightFile.good()) {
        leftFile.get(leftRead);
        rightFile.get(rightRead);
        result = (leftRead == rightRead);
    }
    if (result) {
        result = (!leftFile.good()) && (!rightFile.good());
    }
    leftFile.close();
    rightFile.close();
    return result;
}

#endif //SIMULATION_UTILS_H
