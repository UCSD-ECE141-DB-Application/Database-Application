#ifndef CheckHandler_hpp
#define CheckHandler_hpp
#include<string>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "Storage.hpp"

namespace fs = std::filesystem;
namespace ECE141 {
    bool isValidSQLName(std::string aName);
    bool isSQLExist(std::string aName);
    bool isRepeatedSQLName(std::string aName);
}

#endif
