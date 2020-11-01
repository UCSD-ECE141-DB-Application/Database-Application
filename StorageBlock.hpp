//
//  StorageBlock.hpp
//  Assignment3
//
//  Created by rick gessner on 4/11/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef StorageBlock_hpp
#define StorageBlock_hpp
#include <variant>
#include <stdio.h>
#include <string>
#include <map>
#include <variant>
#include <iostream>
#include <cstring>
#include <sstream>
namespace ECE141 {
  struct BlockInfo
  {
    static const int getBlockSize();
  };
  //a "storage" file is comprised of fixed-sized blocks (defined below)

  const size_t kPayloadSize = 1012; //area reserved in storage-block for user data...
  const int BlockSize = ECE141::BlockInfo::getBlockSize();
  //using ValueType = std::variant<int, double, std::string>;

  //using KeyValues = std::map<const std::string, ValueType>;

  enum class BlockType {
    meta_block='T',
    data_block='D',
    entity_block='E',
    free_block='F',
    index_block='I',
    unknown_block='V',
    schema_block='S',
  };

  using NamedBlockNums = std::map<std::string, uint32_t>;

  struct BlockHeader {
   
    BlockHeader(BlockType aType=BlockType::data_block)
      : type(static_cast<char>(aType)), id(0) {}
   
    BlockHeader(const BlockHeader &aCopy) {
      *this=aCopy;
    }
   
    BlockHeader& operator=(const BlockHeader &aCopy) {
      type=aCopy.type;
      id=aCopy.id;
      return *this;
    }

    void setName(std::string aName) { name = aName; }
    void setType(char aType) { type = aType; }
    void setID(uint32_t anID) { id = anID; }
   
    char      type;     //char version of block type {[D]ata, [F]ree... }
    uint32_t  id;       //use this anyway you like
    std::string name;
  };
 
  struct StorageBlock {
        
    StorageBlock(BlockType aType=BlockType::data_block);
    
    StorageBlock(const StorageBlock &aCopy);
    StorageBlock& operator=(const StorageBlock &aCopy);

    StorageBlock& store(std::ostream &aStream);
    
    void setStored(std::stringstream& theStream) { stored << theStream.str(); }

    //we use attributes[0] as table name...
    BlockHeader   header;
    char          data[kPayloadSize];
    std::stringstream   stored;
  };
 
}

#endif /* StorageBlock_hpp */
