//
//  Filters.hpp
//  RGAssignment6
//
//  Created by rick gessner on 5/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Filters_hpp
#define Filters_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Errors.hpp"
#include "Value.hpp"
#include "Tokenizer.hpp"
#include "Storage.hpp"
#include "Row.hpp"
#include "Schema.hpp"
#include "StorageBlock.hpp"

namespace ECE141 {
  
  class Row;
  class Schema;
  
  struct Operand {
    Operand() {}
    Operand(std::string &aName, TokenType aType, ValueType &aValue, uint32_t anId=0)
      : name(aName), type(aType), entityId(anId), value(aValue) {}
    
    TokenType   type; //so we know if it's a field, a const (number, string)...
    std::string name; //for named attr. in schema
    ValueType   value;
    uint32_t    entityId;
  };
  
  //---------------------------------------------------

  struct Expression {
    Operand     lhs;
    Operand     rhs;
    Operators   op;
    
    Expression(Operand &aLHSOperand, Operators anOp, Operand &aRHSOperand)
      : lhs(aLHSOperand), op(anOp), rhs(aRHSOperand) {}
    
    bool operator()(KeyValues &aList);
  };
  
  //---------------------------------------------------

  using Expressions = std::vector<Expression*>;

  //---------------------------------------------------

  class Filters {
  public:
    
    Filters() {}
    Filters(const Filters &aFilters) {}
    ~Filters() {}
    
    size_t        getCount() const {return expressions.size();}
    bool          matches(KeyValues &aList) const;
    Filters&      add(Expression *anExpression);
    StatusResult  setTableName(std::string thetableName) { tableName = thetableName; return StatusResult{}; }
    StatusResult  setDatabasePath(std::string theDatabasePath) {DatabasePath = theDatabasePath; return StatusResult{};}
    StatusResult  setStorage(Storage* storage) { thestorage = storage; return StatusResult{};}
    StatusResult SelectLimit(std::vector<Row*>& RowCollection, int value);
    StatusResult SelectOrder(std::vector<Row*>& RowCollection, std::string orderName);
    StatusResult SelectWhere(std::vector<Row*>& RowCollection, std::string key, std::string theOperator, std::string Value, Keywords aLogic);
    StatusResult SelectAll(std::vector<Row*>& RowCollection);
    friend class Tokenizer;
    StatusResult setSchema(Schema* aSchema) { theSchemaPtr = aSchema; return StatusResult{}; }
    bool canIndexby() { return isIndex; }
    StatusResult setIsIndex(bool value) { isIndex = value; return StatusResult{}; }
    StatusResult SelectJoin(std::vector<Row *> &RowCollection, std::string tableName, std::string joinTableName, std::string lhs, std::string rhs, Keywords joinType);

  protected:
    Expressions  expressions;
    std::string tableName;
    std::string DatabasePath;
    Storage* thestorage=nullptr;
    std::map<std::string, DataType> typeCollection;
    Schema* theSchemaPtr;
    bool isIndex;
  };

}

#endif /* Filters_hpp */

