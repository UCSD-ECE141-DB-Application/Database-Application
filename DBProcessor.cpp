
#include <iostream>
#include "DBProcessor.hpp"
#include "Tokenizer.hpp"
#include <memory>
#include <vector>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include "StorageBlock.hpp"
namespace fs = std::filesystem;
namespace ECE141
{

    bool isValidDBName(std::string aName);
    bool isRepeatedDBName(std::string aName);
    bool isDBExist(std::string aName);

    Database* DBProcessor::getActiveDatabase()
    {
        return actDBPtr;
    }
    class CreateDBStatement : public Statement
    {
    public:
        CreateDBStatement(Tokenizer& aTokenizer, DBProcessor& aProcessor) : Statement(Keywords::create_kw), theTokenizer(aTokenizer), processor(aProcessor) {}
        virtual const char *getStatementName() { return "Create Statement"; }
        virtual StatusResult parse(Tokenizer &aTokenizer)
        {

            theTokenizer.next();
            if (!(theTokenizer.current().keyword == Keywords::database_kw))
                return StatusResult(Errors::unknownAttribute);
            theTokenizer.next();
            while (theTokenizer.more())
            {
                if (theTokenizer.current().data == ";")
                    break;
                if(!isValidDBName(theTokenizer.current().data))
                    return StatusResult(ECE141::Errors::illegalIdentifier);
                if (isRepeatedDBName(theTokenizer.current().data))
                    return StatusResult(Errors::databaseExists);
                std::string aDBName = theTokenizer.current().data;
                DBCollection.push_back(aDBName);
                theTokenizer.next();
            }
            
            return StatusResult();
        }
        virtual StatusResult run(std::ostream &aStream)
        {
            std::ofstream myfile;
            for (std::vector<std::string>::iterator it = DBCollection.begin(); it != DBCollection.end(); it++)
            {

                Database* theDB = new Database(*it, CreateNewStorage());
                // std::string thePath = std::string( ECE141::StorageInfo::getDefaultStoragePath())+"/"+ *it+".db";
                // myfile.open(thePath);
                aStream << "created database " << *it << ".db (ok)\n";
                myfile.close();
            }
            return StatusResult{};
            
        }
    protected:
        Tokenizer& theTokenizer;
        std::vector<std::string> DBCollection;
        DBProcessor &processor;
    };

    class DropDBStatement : public Statement
    {
    public:
        DropDBStatement(Tokenizer &aTokenizer, DBProcessor &aProcessor) : Statement(Keywords::drop_kw), theTokenizer(aTokenizer), processor(aProcessor) {}
        virtual const char *getStatementName() { return "Drop Statement"; }
        virtual StatusResult parse(Tokenizer &aTokenizer)
        {
            theTokenizer.next();
            if (!(theTokenizer.current().keyword == Keywords::database_kw))
                return StatusResult(Errors::unknownCommand);
            theTokenizer.next();
            while (theTokenizer.more())
            {
                if (theTokenizer.current().data == ";")
                    break;
                if (!isValidDBName(theTokenizer.current().data))
                    return StatusResult(ECE141::Errors::illegalIdentifier);
                if (!isDBExist(theTokenizer.current().data))
                    return StatusResult(Errors::unknownDatabase);
                std::string aDBName = theTokenizer.current().data + ".db";
                DBCollection.push_back(aDBName);
                theTokenizer.next();
            }
            return StatusResult();
        }
        virtual StatusResult run(std::ostream &aStream)
        {
            for (std::vector<std::string>::iterator it = DBCollection.begin(); it != DBCollection.end(); it++) {
                std::string fileAddress = std::string(ECE141::StorageInfo::getDefaultStoragePath()) + "/" + *it;
                remove(fileAddress.c_str());
                std::cout << "dropped database " << *it << " (ok)";
            }
            return StatusResult{};
        }
    protected:
        Tokenizer& theTokenizer;
        std::vector<std::string> DBCollection;
        DBProcessor &processor;
    };

    class UseDBStatement : public Statement
    {
    public:
        UseDBStatement(Tokenizer &aTokenizer, DBProcessor &aProcessor) : Statement(Keywords::use_kw), theTokenizer(aTokenizer), processor(aProcessor) {}
        virtual const char *getStatementName() { return "Use Statement"; }
        virtual StatusResult parse(Tokenizer &aTokenizer) 
        {
            theTokenizer.next();
            if (!(theTokenizer.current().keyword == Keywords::database_kw))
                return StatusResult(Errors::unknownCommand);
            theTokenizer.next();
            while (theTokenizer.more())
            {
                if (theTokenizer.current().data == ";")
                    break;
                if (!isValidDBName(theTokenizer.current().data))
                    return StatusResult(ECE141::Errors::illegalIdentifier);
                if (!isDBExist(theTokenizer.current().data))
                    return StatusResult(Errors::unknownDatabase);
                std::string aDBName = theTokenizer.current().data;
                DBCollection.push_back(aDBName);
                theTokenizer.next();
            }
            return StatusResult();
        }
        virtual StatusResult run(std::ostream &aStream)
        {
            for (std::vector<std::string>::iterator it = DBCollection.begin(); it != DBCollection.end(); it++) {
                aStream << "using database " << *it << "\n";
                if (!processor.getActiveDatabase())
                {
                    Database* theActiveDatabase=processor.setActiveDB(*it);
                    //Database* theDB = new Database(*it, CreateNewStorage());
                }
                else
                {
                    delete processor.getActiveDatabase();
                    processor.setActiveDB(*it);
                }
                return StatusResult(); 
            }
            return StatusResult();
        }
    protected:
        Tokenizer& theTokenizer;
        std::vector<std::string> DBCollection;
        DBProcessor &processor;
    };

    class DescribeDBStatement : public Statement
    {
    public:
        DescribeDBStatement(Tokenizer &aTokenizer, DBProcessor &aProcessor) : Statement(Keywords::describe_kw), theTokenizer(aTokenizer), processor(aProcessor) {}
        virtual const char *getStatementName() { return "Describe Statement"; }
        virtual StatusResult parse(Tokenizer &aTokenizer) 
        {
            theTokenizer.next();
            if (!(theTokenizer.current().keyword == Keywords::database_kw))
                return StatusResult(Errors::unknownCommand);
            theTokenizer.next();
            while (theTokenizer.more())
            {
                if (theTokenizer.current().data == ";")
                    break;
                if (!isValidDBName(theTokenizer.current().data))
                    return StatusResult(ECE141::Errors::illegalIdentifier);
                if (!isDBExist(theTokenizer.current().data))
                    return StatusResult(Errors::unknownDatabase);
                std::string aDBName = theTokenizer.current().data;
                DBCollection.push_back(aDBName);
                theTokenizer.next();
            }
            return StatusResult();
        }
        std::string getDatabasePath(const std::string& aDBName) {
            std::string thePath = ECE141::StorageInfo::getDefaultStoragePath();
            //windows one
            std::string DBName = thePath + "\\" + aDBName + ".db";
            //linux one
            std::string DBName2 = thePath + "/" + aDBName + ".db";
            // build a full path (in default storage location) to a given db file..

            return DBName2; //TODO: Change to linux
        }
        virtual StatusResult run(std::ostream &aStream)
        {
            int num = 0;
            for (std::vector<std::string>::iterator it = DBCollection.begin(); it != DBCollection.end(); it++) 
            {
                aStream << "describe database " << *it << "\n";
                std::string seperator = "+---------- +--------------+-----------------------------+\n";
                aStream << seperator;
                aStream << "| Block#    | Type         | Other                       |\n";
                aStream << seperator;
                Database *theDB = processor.getActiveDatabase();
                Storage &theStorage = theDB->getStorage();
                const int blockSize = ECE141::BlockInfo::getBlockSize();
                
                std::string thePath = getDatabasePath(*it);
                uint32_t size = std::filesystem::file_size(thePath);
                std::ifstream is(thePath, std::ios::in | std::ios::binary);

                for (int i = 0; i < (size / blockSize); i++)
                {
                    is.seekg(i * blockSize);
                    char *buffer = new char[blockSize];
                    //char* buffer = new char[blockSize];
                    is.read(buffer, blockSize);
                    std::string type;
                    std::string other;
                    std::stringstream ss;
                    ss.write(buffer, blockSize);
                    ss >> type >> other;
                    if (buffer[0] == 'T')
                    {
                        num++;
                        type = "meta";
                        aStream << "| " << std::left << std::setw(10) << i << "| " << std::left << std::setw(13) << type << "| " << std::left << std::setw(28)<<" "<<"|\n";
                    }
                        
                    if (buffer[0] == 'S')
                    {
                        num++;
                        type = "schema";
                        std::string tableName;
                        ss >> tableName;
                        tableName = "\"" + tableName + "\"";
                        aStream << "| " << std::left << std::setw(10) << i << "| " << std::left << std::setw(13) << type << "| " << std::left << std::setw(28) << tableName << "|\n";
                    }
                        
                    if (buffer[0] == 'D')
                    {
                        num++;
                        type = "data";
                        std::string tableName;
                        //ss >> tableName;
                        tableName = "\"" + other + "\"";
                        aStream << "| " << std::left << std::setw(10) << i << "| " << std::left << std::setw(13) << type << "| " << std::left << std::setw(28) << tableName << "|\n";
                    }
                        
                    if (buffer[0] == 'F')
                        continue;
                }
                is.close();
                aStream << seperator;
                aStream<<num<<" rows in set\n";

            }
            return StatusResult();
        }
    protected:
        Tokenizer& theTokenizer;
        std::vector<std::string> DBCollection;
        DBProcessor &processor;
    };

    class ShowDBStatement : public Statement
    {
    public:
        ShowDBStatement(Tokenizer &aTokenizer, DBProcessor &aProcessor) : Statement(Keywords::show_kw), theTokenizer(aTokenizer), processor(aProcessor) {}
        virtual const char *getStatementName() { return "Show Statement"; }
        virtual StatusResult parse(Tokenizer &aTokenizer) 
        {
            theTokenizer.next();
            if (!(theTokenizer.current().keyword == Keywords::databases_kw))
                return StatusResult(Errors::unknownCommand);
            theTokenizer.next();
            if (theTokenizer.more())
            {
                if (!(theTokenizer.current().data == ";"))
                    return StatusResult(Errors::unknownCommand);
            }
            return StatusResult();
        }
        virtual StatusResult run(std::ostream &aStream)
        {
            std::string DBPath = std::string(ECE141::StorageInfo::getDefaultStoragePath());
            for (auto& p : fs::directory_iterator(DBPath))
            {
                int length = DBPath.length();
                std::string aPath = p.path().u8string();
                aPath = aPath.substr(length+1);
                int pos = aPath.find(".");
                aPath = aPath.substr(0, pos);
                std::cout << aPath << '\n';
            }
            return StatusResult{};
        }
    protected:
        Tokenizer& theTokenizer;
        DBProcessor &processor;
    };
    DBProcessor::DBProcessor(CommandProcessor *aNext) : CommandProcessor(aNext), activedb(""), actDBPtr(nullptr) {}

    DBProcessor::~DBProcessor() {}

    StatusResult DBProcessor::interpret(Statement &aStatement)
    {
        Tokenizer* aTokenizerPtr = nullptr;
        StatusResult theStatusResult=aStatement.parse(*aTokenizerPtr);
        if (theStatusResult.code == Errors::noError)
            return aStatement.run(std::cout);
        else return theStatusResult;
        
    }

    bool isValidDBName(std::string aName)
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

    bool isDBExist(std::string aName)
    {
        std::string thePath = ECE141::StorageInfo::getDefaultStoragePath();
        //windows one
        std::string DBName = thePath + "\\" + aName + ".db";
        //linux one
        std::string DBName2 = thePath + "/" + aName + ".db";

        if (std::filesystem::exists(DBName) || std::filesystem::exists(DBName2))
            return true;
        else return false;
    }

    bool isRepeatedDBName(std::string aName)
    {
        std::string thePath = ECE141::StorageInfo::getDefaultStoragePath();
        //windows one
        std::string DBName = thePath + "\\" + aName + ".db";
        //linux one
        std::string DBName2 = thePath + "/"+aName+".db";
        
        for (auto& p : fs::directory_iterator(thePath))
        {
            std::string  aPath = p.path().u8string();

            if(aPath == DBName|| DBName2== aPath)
                return true;
        }
        return false;
    }

    Statement *DBProcessor::getStatement(Tokenizer &aTokenizer)
    {
        if (aTokenizer.current().keyword == Keywords::create_kw && aTokenizer.peek(1).keyword == Keywords::database_kw)
        {
            return new CreateDBStatement(aTokenizer, *this);
        }
        if (aTokenizer.current().keyword == Keywords::drop_kw && aTokenizer.peek(1).keyword == Keywords::database_kw)
        {
            return new DropDBStatement(aTokenizer, *this);
        }
        if (aTokenizer.current().keyword == Keywords::use_kw && aTokenizer.peek(1).keyword == Keywords::database_kw)
        {
            return new UseDBStatement(aTokenizer, *this);
        }
        if (aTokenizer.current().keyword == Keywords::describe_kw && aTokenizer.peek(1).keyword == Keywords::database_kw)
        {
            return new DescribeDBStatement(aTokenizer, *this);
        }
        if (aTokenizer.current().keyword == Keywords::show_kw && aTokenizer.peek(1).keyword == Keywords::databases_kw)
        {
            return new ShowDBStatement(aTokenizer, *this);
        }
        return nullptr;
    }

} // namespace ECE141
