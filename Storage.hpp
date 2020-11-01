//
//  Storage.hpp
//  Assignment2
//
//  Created by rick gessner on 4/5/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Storage_hpp
#define Storage_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <fstream>
#include <variant>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Errors.hpp"
#include "StorageBlock.hpp"
#include "Value.hpp"
// #include "Schema.hpp"

namespace ECE141 {

    //first, some utility classes...
    std::string getDatabasePath(const std::string& aDBName);
    class StorageInfo {
    public:
        static const char* getDefaultStoragePath();
    };

    struct CreateNewStorage {};
    struct OpenExistingStorage {};

    struct Storable {
        virtual StatusResult  encode(std::ostream& aWriter) = 0;
        virtual StatusResult  decode(std::istream& aReader) = 0;
        virtual BlockType     getType() const = 0; //what kind of block is this?
        virtual std::string   getName() = 0;
        int pos=0;
    };

    struct SchemaTOC : public Storable
    {
        std::map<std::string, uint32_t> list;
        std::map<std::string, BlockType> list2;
        BlockType type;
        StatusResult  encode(std::ostream& aWriter) 
        {
            aWriter << "T";
            for (auto& i : list)
            {
                aWriter << " " << i.first << " " << i.second;
                //aWriter << i.second << ',' << i.first << ',' << char(static_cast<std::underlying_type<BlockType>::type>(type)) << '\0';
            }
            aWriter << " , \n";
            return StatusResult();
        }
        StatusResult decode(std::istream &aReader) 
        { 
            char ablocktype;
            aReader >> ablocktype;
            type=static_cast<BlockType>(ablocktype);
            std::string buf;
            std::getline(aReader, buf);
          //  std::cout << buf << std::endl;
            std::stringstream ss(buf);
            while (!ss.eof())
            {
                std::string first, second;
                ss >> first >> second;
                if (first == ",")
                    break;
                list[first] = std::stoi(second);
            }
            return StatusResult(); 
        }
        BlockType getType() const { return type; }
        std::string getName() { return list.begin()->first; }
        BlockType stob(std::string theString)
        {
            if(theString == "T")
                return BlockType::meta_block;
            else if (theString == "D")
                return BlockType::data_block;
            else if (theString == "E")
                return BlockType::entity_block;
            else if (theString == "F")
                return BlockType::free_block;
            else if (theString == "I")
                return BlockType::index_block;
            else if (theString == "S")
                return BlockType::schema_block;
            return BlockType::unknown_block;
        }
    };

    // USE: Our storage manager class...
    class Storage {
    public:

        Storage(const std::string aName, CreateNewStorage);
        Storage(const std::string aName, OpenExistingStorage);
        ~Storage();
        uint32_t        getTotalBlockCount();

        //high-level IO (you're not required to use this, but it may help)...    
         StatusResult    save(Storable& aStorable, uint32_t StorableNum); //using a stream api
         StatusResult    load(Storable& aStorable, uint32_t StorableNum); //using a stream api
        StatusResult    drop(uint32_t StorableNum);
      //  StatusResult save(std::map<std::string, uint32_t>& TOCList, uint32_t TOCBlockNum); //using a stream api//only save the TOCone
        //StatusResult load(std::map<std::string, uint32_t>& TOCList, uint32_t TOCBlockNum); //using a stream api

        //low-level IO...    
        StatusResult    readBlock(StorageBlock& aBlock, uint32_t aBlockNumber);
        StatusResult    writeBlock(StorageBlock& aBlock, uint32_t aBlockNumber);
        StatusResult    findFreeBlockNum();
        StatusResult    getSizeofFile(std::string aDBName);
        StatusResult    deleteRow(std::string aTableName, std::vector<ValueType> tobedeleted, bool deleteall);
     //   StatusResult UpdateRow(std::vector<Row*>& RowCollection, std::string feature, std::string feature_value);
     //   StatusResult 
    protected:
        bool            isReady() const;
        std::vector<uint32_t> FreeBlocks;
        std::string     name;
        std::fstream    filestream;
    };

    struct BlockVisitor {
        virtual bool operator()(StorageBlock& aStorageBlock, uint32_t aBlockNum) = 0;

    };
    
    struct BlockIterator {
        virtual bool each(BlockVisitor& aVisitor)=0;
        virtual bool canIndexBy(const std::string& aField) { return false; };
    };



}

#endif /* Storage_hpp */