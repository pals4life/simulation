//============================================================================
// @name        : Utils.cpp
// @author      : Ward Gauderis
// @date        : 5/14/19
// @version     : 
// @copyright   : BA1 Informatica - Ward Gauderis - University of Antwerp
// @description : 
//============================================================================
#include "Utils.h"
#include <fstream>

// source: Serge Demeyer - TicTactToe in C++, Ansi-style
bool FileCompare(const std::string &leftFileName, const std::string &rightFileName) {
    std::ifstream leftFile, rightFile;
    char leftRead, rightRead;
    bool result;
    leftFile.open(leftFileName.c_str());
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
