#include "CheckHandler.hpp"

namespace ECE141 {
    bool isValidSQLName(std::string aName)
    {
        for (int i = 0; i < aName.size(); i++)
        {
            if (i == 0)
            {
                if (!((aName[i] >= 'a' && aName[i] <= 'z') || (aName[i] >= 'A' && aName[i] <= 'Z')))
                {
                    return false;
                }
            }
            else
            {
                if (!((aName[i] >= '0' && aName[i] <= '9') || (aName[i] >= 'a' && aName[i] <= 'z') || (aName[i] >= 'A' && aName[i] <= 'Z') || aName[i] == '_'))
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool isSQLExist(std::string aName)
    {
        std::string thePath = ECE141::StorageInfo::getDefaultStoragePath();
        //windows one
        std::string DBName = thePath + "\\" + aName + ".db";
        //linux one
        std::string DBName2 = thePath + "/" + aName + ".db";

        if (std::filesystem::exists(DBName) || std::filesystem::exists(DBName2))
            return true;
        else
            return false;
    }

    bool isRepeatedSQLName(std::string aName)
    {
        std::string thePath = ECE141::StorageInfo::getDefaultStoragePath();
        //windows one
        std::string DBName = thePath + "\\" + aName + ".db";
        //linux one
        std::string DBName2 = thePath + "/" + aName + ".db";

        for (auto& p : fs::directory_iterator(thePath))
        {
            std::string aPath = p.path().u8string();

            if (aPath == DBName || DBName2 == aPath)
                return true;
        }
        return false;
    }
}