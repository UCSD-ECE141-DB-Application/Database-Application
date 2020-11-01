//
//  Schema.hpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Schema_hpp
#define Schema_hpp

#include <stdio.h>
#include <vector>
#include "Attribute.hpp"
#include "Errors.hpp"
#include "Storage.hpp"
#include "StorageBlock.hpp"
#include "Row.hpp"
#include "Index.hpp"
namespace ECE141 {
  
  struct Block;
  struct Expression;
  class  Database;
  class  Tokenizer;
  // struct Storable;
  
  //using StringList = std::vector<std::string>;
  //using attribute_callback = std::function<bool(const Attribute &anAttribute)>;
  
  using AttributeList = std::vector<Attribute>;
  using RowList = std::vector<Row*>;
  
  //STUDENT: If you're using the Storable interface, add that to Schema class?

  class Schema : public Storable
  {
  public:
    Schema(const std::string aName);
    Schema(const Schema &aCopy);

    ~Schema();
        
    const std::string&    getName() const {return name;}
    const AttributeList&  getAttributes() const {return attributes;}
    uint32_t              getBlockNum() const {return blockNum;}
    void              setBlockNum(uint32_t theblockNum) { blockNum = theblockNum; }
    bool                  isChanged() {return changed;}
    
    Schema&               addAttribute( Attribute &anAttribute);

    Attribute& getAttribute(const std::string& aName) { return attriMap[aName]; }
    Schema& addRow(Row* aRow);
    RowList& getRows() { return rows; }

    virtual StatusResult encode(std::ostream &aWriter);
    virtual StatusResult decode(std::istream &aReader);
    virtual BlockType getType() const {return BlockType::schema_block;} //what kind of block is this?
    virtual std::string getName() { return name; }  //TODO: Changes needed

    //STUDENT: These methods will be implemented in the next assignment...
    
    //Value                 getDefaultValue(const Attribute &anAttribute) const;
    //StatusResult          validate(KeyValues &aList);
    
    std::string           getPrimaryKeyName() const;
    uint32_t              getNextAutoIncrementValue();
    
    
    //STUDENT: Do you want to provide an each() method for observers?
    StatusResult          setIndexNum(uint32_t theindexNum) { indexNum = theindexNum; return StatusResult();}
    uint32_t              getIndexNum() { return indexNum; }
    Index&                getIndex() { return theIndex; }
    StatusResult setIndex(Index& idx) {theIndex = idx; return StatusResult();}

    //friend class Database; //is this helpful?

  protected:
      // saved variable
      AttributeList   attributes;
      uint32_t        indexNum;
      std::string     name;
      //

      // loaded from memory
    std::map<std::string , Attribute> attriMap;
    Index          theIndex;
    uint32_t        blockNum;  //storage location.
    bool            changed;
    RowList         rows;
   // Index theIndex;
  };
}
#endif /* Schema_hpp */
