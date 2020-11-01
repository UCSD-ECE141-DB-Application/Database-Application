//
//  Index.hpp
//  RGAssignment8
//
//  Created by rick gessner on 5/17/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Index_h
#define Index_h

#include "Storage.hpp"
#include "keywords.hpp"
#include <optional>
//#include "Value.hpp"
#include "ConverHandler.hpp"
namespace ECE141 {
  
  using IntOpt = std::optional<uint32_t>;
  
  //struct LessKey {
  //  bool operator()(const ValueType& anLHS, const ValueType& aRHS) const {
  //    return anLHS.value < aRHS.value;
  //  }
  //};

  class Index : public BlockIterator, public Storable {
  public:

    using ValueMap = std::map<ValueType, uint32_t>;

    Index() {}
    Index(const std::string &aTable, const std::string &aField, DataType aType,uint32_t theblockNUm)
      : tableName(aTable), field(aField), type(aType), blockNum(theblockNUm) {
        changed=false;
      }
    
    virtual ~Index() {}
    
    void                setSchemaId(uint32_t theschemaId) { schemaId =theschemaId; }
    void                setBlockNum(uint32_t theBlockNum) { blockNum = theBlockNum; }
    ValueMap&           getList() {return list;}
    void                setChanged(bool aValue=true) {changed=aValue;}
    bool                isChanged() {return changed;}
    const std::string&  getFieldName() const {return field;}
    uint32_t            getBlockNum() const {return blockNum;}
    
      //manage keys/values in the index...
    Index& addKeyValue(const ValueType& aKey, uint32_t aValue);
    Index& removeKeyValue(const ValueType &aKey);    
    bool contains(const ValueType &aKey);    
    uint32_t getValue(const ValueType &aKey);
    
      //don't forget to support the storable interface IF you want it...
    StatusResult encode(std::ostream &aWriter);    
    StatusResult decode(std::istream &aReader);
    //void initBlock(StorageBlock &aBlock);
    virtual BlockType     getType() const { return BlockType::index_block; } //what kind of block is this?
    virtual std::string   getName() { return tableName; }
      //and the blockIterator interface...
    bool each(BlockVisitor &aVisitor) { std::cout << "Inside Index each, but did nothing..." << std::endl; return true; }
    bool canIndexBy(const std::string &aField) { std::cout << "Inside Index canIndexBy, but did nothing..." << std::endl; return true; }

  protected:
    // saved
    std::string   field; //what field are we indexing?
    ValueMap      list;
    DataType     type;
    std::string tableName;
    uint32_t      blockNum;  //storage block# of index...
    //

    uint32_t      schemaId;
    bool          changed;

 //   BlockType

  };

}
#endif /* Index_h */
