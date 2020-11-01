//
//  Storage.cpp
//  Assignment2
//
//  Created by rick gessner on 4/5/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Storage.hpp"
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sstream>  
#include <algorithm>
#include <string>
#include <iomanip>
#include <filesystem>
namespace ECE141 {
    
    // USE: Our main class for managing storage...
    const char* StorageInfo::getDefaultStoragePath() 
    {
        const char* thePath = std::getenv("DB_PATH");
        //const char *thePath = "C:\\Users\\haomi\\OneDrive\\Files\\Academy\\UCSD\\2020Spring\\ECE141B\\Assignments\\ECE141_Assignment9_update\\tmp"; //TODO: Change to linux
        return thePath;
    }

    //----------------------------------------------------------

    //path to the folder where you want to store your DB's...
    std::string getDatabasePath(const std::string& aDBName) {
        std::string thePath = ECE141::StorageInfo::getDefaultStoragePath();
        //windows one
        std::string DBName = thePath + "\\" + aDBName + ".db";
        //linux one
        std::string DBName2 = thePath + "/" + aDBName + ".db";
        // build a full path (in default storage location) to a given db file..

        return DBName2; //TODO: Change to linux
    }

    // USE: ctor ---------------------------------------
    Storage::Storage(const std::string aName, CreateNewStorage) : name(aName) {
        std::string thePath = getDatabasePath(name);
        //try to create a new db file in known storage location.
        //throw error if it fails...
    }

    // USE: ctor ---------------------------------------
    Storage::Storage(const std::string aName, OpenExistingStorage) : name(aName) {
        std::string thePath = getDatabasePath(aName);
        //try to OPEN a db file a given storage location
        //if it fails, throw an error
    }

    // USE: dtor ---------------------------------------
    Storage::~Storage() {
        filestream.close();
    }

    // USE: validate we're open and ready ---------------------------------------
    bool Storage::isReady() const {
        return filestream.is_open();
    }

    // USE: count blocks in file ---------------------------------------
    uint32_t Storage::getTotalBlockCount() {
        //how can we compute the total number of blocks in storage?
        uint32_t theCount = 0;
        return theCount;
    }

    // Call this to locate a free block in this storage file.
    // If you can't find a free block, then append a new block and return its blocknumber
    StatusResult Storage::findFreeBlockNum() {

        //std::string thePath = getDatabasePath(this->name);
        //uint32_t size = std::filesystem::file_size(thePath);
        //return StatusResult{ noError ,size/1024}; //return blocknumber in the 'value' field...
        int blockSize = ECE141::BlockInfo::getBlockSize();
        uint32_t num = 0;
        std::string thePath = getDatabasePath(this->name);
        uint32_t size = std::filesystem::file_size(thePath);
        int BlockNum = size / blockSize;
        std::ifstream is(thePath, std::ios::binary | std::ios::in);
        for (int i = 0; i < BlockNum; i++)
        {
            char* buffer = new char[blockSize];
            is.read(buffer, blockSize);
            if (buffer[0] == 'F')
                return StatusResult{ noError,num };
            num += 1;
        }
        return StatusResult{ noError ,num }; //return blocknumber in the 'value' field...
    }


    // USE: for use with "storable API" [NOT REQUIRED -- but very useful]

    // StatusResult Storage::save(Schema& aSchema) {
    StatusResult Storage::save(Storable& aStorable, uint32_t StorableNum) {
        //High-level IO: save a storable object (like a table row)...
       // filestream.seekg(StorableNum * BlockSize,filestream.beg);
        std::stringstream EncoderString;
        StatusResult theResult=aStorable.encode(EncoderString);// add all the content to stringstream EncoderString
        StorageBlock aBlock(aStorable.getType());
        aBlock.header.setName(this->name);
        aBlock.setStored(EncoderString);
        //aBlock.store(EncoderString);// load all the content to Block
        this->writeBlock(aBlock, StorableNum); // write the block to file

        return StatusResult{ noError };
    }

    StatusResult Storage::load(Storable& aStorable, uint32_t StorableNum) {
        //high-level IO: load a storable object (like a table row)
        StorageBlock aBlock(aStorable.getType());
        aBlock.header.setName(this->name);
        //std::cout << aStorable.getName() << std::endl;
        this->readBlock(aBlock, StorableNum);
        StatusResult theResult = aStorable.decode(aBlock.stored);// add all the content to stringstream EncoderString
        aStorable.pos = StorableNum;
        return StatusResult{ noError };
    }

    StatusResult Storage::drop(uint32_t StorableNum)
    {
        int blockSize = ECE141::BlockInfo::getBlockSize();
        std::string thePath = getDatabasePath(this->name);
        std::fstream theStream(thePath, std::ios::binary | std::ios::out | std::ios::in);
        theStream.seekp(StorableNum * BlockSize);
        theStream << 'D ' << std::setw(blockSize - 1) << std::setfill(char(0xFF)) << char(0xFF);
        return StatusResult{ noError };
    }


    // USE: write data a given block (after seek)
    StatusResult Storage::writeBlock(StorageBlock& aBlock, uint32_t aBlockNumber) {
        //STUDENT: Implement this; this is your low-level block IO...
        int blockSize = ECE141::BlockInfo::getBlockSize();
        std::string thePath = getDatabasePath(this->name);
        std::ofstream theStream(thePath, std::ios::out | std::ios::binary | std::ios::app | std::ios::ate);
        theStream.close();
        std::fstream aStream(thePath, std::ios::binary | std::ios::out | std::ios::in);
        aStream.seekp(aBlockNumber * BlockSize);
        aStream << aBlock.stored.str() << std::setw(blockSize - aBlock.stored.str().length()) << std::setfill(char(0xFF)) << char(0xFF);
        aStream.close();
        return StatusResult{};
    }

    // USE: read data from a given block (after seek)
    StatusResult Storage::readBlock(StorageBlock& aBlock, uint32_t aBlockNumber) 
    {
        //STUDENT: Implement this; this is your low-level block IO...
        std::string thePath = getDatabasePath(this->name);
        std::ifstream is(thePath, std::ios::in | std::ios::binary);
        int blockSize = ECE141::BlockInfo::getBlockSize();
        is.seekg(aBlockNumber* blockSize);
        char* buffer = new char[blockSize];
        is.read(buffer, blockSize);
        std::stringstream ss;
        ss.write(buffer, blockSize);
        aBlock.setStored(ss);
        is.close();
        return StatusResult{};
    }
    bool contains(ValueType theVal, std::vector<ValueType> tobedeleted)
    {
        for (auto i : tobedeleted)
        {
            if (i == theVal)
                return true;
        }
        return false;
    }
    bool stobool(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        std::istringstream is(str);
        bool b;
        is >> std::boolalpha >> b;
        return b;
    }
    StatusResult Storage::deleteRow(std::string aTableName, std::vector<ValueType> tobedeleted, bool deleteall)
    {
        std::string thePath = getDatabasePath(this->name);
        uint32_t size = std::filesystem::file_size(thePath);
        int blockSize = ECE141::BlockInfo::getBlockSize();
        int Totalnum = size / blockSize;

        std::fstream aStream(thePath, std::ios::binary | std::ios::out | std::ios::in);
        char *buf = new char[blockSize];

        std::stringstream ss;
        ss << "F " << std::setw(blockSize - 2) << std::setfill(char(0xFF)) << char(0xFF);

        std::vector<int> Rows;
        for (int i = 0; i < Totalnum; i++)
        {
            aStream.read(buf, blockSize);
            std::stringstream ss(buf);
            std::string type, tableName, field, fieldVal;
            ss >> type >> tableName >> field >> fieldVal;
            if (deleteall && type == "D" && tableName == aTableName)
            {
                Rows.push_back(i);
            }
            else if (!deleteall && type == "D" && tableName == aTableName)
            {
                switch (tobedeleted.begin()->index())
                {
                case 0:
                    {
                        if (contains(stoi(fieldVal), tobedeleted))
                            Rows.push_back(i);
                    }
                    break;
                case 1:
                    {
                        if (contains(stoi(fieldVal), tobedeleted))
                            Rows.push_back(i);
                    }                    
                    break;
                case 2:
                    {
                        if (contains(stof(fieldVal), tobedeleted))
                            Rows.push_back(i);
                    }                    
                    break;
                case 3:
                    {
                        if (contains(stobool(fieldVal), tobedeleted))
                            Rows.push_back(i);
                    }                    
                    break;
                case 4:
                    {
                        if (contains(fieldVal, tobedeleted))
                            Rows.push_back(i);
                    }                    
                    break;
                }
                
            }
        }

        for (std::vector<int>::iterator it = Rows.begin(); it < Rows.end(); it++)
        {
            aStream.seekp(*it * blockSize);
            aStream.write(ss.str().c_str(), blockSize);
        }
        aStream.close();
        return StatusResult{};   
    }
}
