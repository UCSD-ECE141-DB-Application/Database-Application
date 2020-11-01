
#ifndef DBProcessor_hpp
#define DBProcessor_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

#include "CommandProcessor.hpp"
#include "Storage.hpp"
#include "Database.hpp"

namespace ECE141
{
    class DBProcessor : public CommandProcessor
    {
    public:
        DBProcessor(CommandProcessor *aNext = nullptr);
        virtual ~DBProcessor();

        virtual Statement *getStatement(Tokenizer &aTokenizer);
        virtual StatusResult interpret(Statement &aStatement);

        virtual Database* getActiveDatabase();

        std::string getActiveDBName() {return activedb;}
        Database* setActiveDB(std::string aDBName) 
        { 
            activedb = aDBName; 
            actDBPtr = new Database(getActiveDBName(), OpenExistingStorage()); 
            return actDBPtr;
        }
    protected:
        std::string activedb;
        Database* actDBPtr;
    };
} // namespace ECE141

#endif /* DBProcessor_hpp */
