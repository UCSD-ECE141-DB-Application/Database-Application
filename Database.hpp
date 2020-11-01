//
//  Database.hpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//

#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <map>
#include "Schema.hpp"
#include "Storage.hpp"
#include "View.hpp"
#include "Index.hpp"
#include <iomanip>
#include "PhaseHandler.hpp"
namespace ECE141 {
  class Database  {
  public:
    
    Database(const std::string aPath, CreateNewStorage);
    Database(const std::string aPath, OpenExistingStorage);
    ~Database();
    // get function
    Storage&     getStorage() {return storage;}
    std::string& getName() {return name;}
    std::map<std::string, Schema*>& getSchemas() { return schemas; }
    SchemaTOC&   getTOC() { return toc; }

    // sql running function
    StatusResult createTable(Schema* aSchema);
    StatusResult createIndex(Schema& aSchema, Index& theIndex);
    StatusResult dropTable(std::string tableName);
    StatusResult insertTable(std::string);
    StatusResult deleteRow(std::string, std::vector<Row *> RowCollection, bool deleteall);
    StatusResult showIndexes();
    StatusResult describeTable(const std::string& aName);
    //  
    StatusResult showTables();
    StatusResult selectTable(SelectPhaseHandler& theSelectPhaseHandler, std::vector<Row*>& RowCollection);
    StatusResult SelectLimit(std::vector<Row*>& RowCollection, int value, std::string);
    StatusResult SelectOrder(std::vector<Row*>& RowCollection, std::string orderName, std::string);
    StatusResult SelectWhere(std::vector<Row*>& RowCollection, std::string key, std::string theOperator, std::string Value, std::string aTablename, Keywords aLogic);
    StatusResult SelectAll(std::vector<Row*>& RowCollection, std::string tableName);
    StatusResult UpdateRow(std::vector<Row*>& RowCollection, std::string tableName, std::string feature, std::string feature_value);
    StatusResult SelectJoin(std::vector<Row *> &RowCollection, std::string table1, Join& theJoin);


  protected:
    std::string     name;
    Storage         storage;
    std::map<std::string,uint32_t> tableAddr;
    std::map<std::string, Schema*> schemas;
    bool changed = false;
    SchemaTOC toc;
  };
}

#endif /* Database_hpp */
