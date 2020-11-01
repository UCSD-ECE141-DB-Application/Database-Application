//
//  Database.cpp
//  Database1
//
//  Created by rick gessner on 4/12/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include <sstream>
#include "Database.hpp"
#include "Filters.hpp"
#include "Timer.hpp"
//this class represents the database object.
//This class should do actual database related work,
//we called upon by your db processor or commands

namespace ECE141 {
    uint32_t kSchemaTOCBlockNum = 0;
    Database::Database(const std::string aName, CreateNewStorage)
    : name(aName), storage(aName, CreateNewStorage{}) 
    {
        toc.type = BlockType::meta_block;
        storage.save(toc, kSchemaTOCBlockNum);
    }

    Database::Database(const std::string aName, OpenExistingStorage)
    : name(aName), storage(aName, OpenExistingStorage{}) {
        // toc.list[name] = kSchemaTOCBlockNum;
        storage.load(toc, kSchemaTOCBlockNum);
        for (std::map<std::string, uint32_t>::iterator it = toc.list.begin(); it != toc.list.end(); ++it)
        {
            Schema* aSchema=new Schema(it->first);
            storage.load(*aSchema, it->second);
            schemas[it->first] = aSchema;
            Index* theIdx = new Index();
            storage.load(*theIdx, aSchema->getIndexNum());
            aSchema->setIndex(*theIdx);
        }

    }

    Database::~Database() {
    }
    StatusResult      createTable(Schema*);

    StatusResult Database::showIndexes()
    {
        //seperator
        //for (int i = 0; i < 2; i++)
        //{
        //    std::cout << "+" << std::setw(15) << std::setfill('-');
        //}
        //std::cout << "-";
        //std::cout << "+\n";
        std::cout << "+" <<"---------\n";
        //
        std::cout << "| " << "table" << " " << std::left << "| " << "field" << "|\n";
        //
        for (auto& i : schemas)
        {
            Index& theIndex=i.second->getIndex();
            std::cout << "| " << i.second->getName() << "   ";
            std::cout << "| " << i.second->getPrimaryKeyName() << "   \n";
          //  std::cout << "|\n";

        }
        std::cout <<schemas.size() <<" rows in set\n";
       // std::cout << "Inside showindexes but did noting..." << std::endl;
        return StatusResult();
    }

    StatusResult Database::createTable(Schema* aSchema)
    {
    // I/O part
        uint32_t FreeBlockNum=storage.findFreeBlockNum().value;

       // aSchema->setIndexNum(FreeBlockNum);
        DataType theDatatype=aSchema->getAttribute(aSchema->getPrimaryKeyName()).getType();
        Index theIndex(aSchema->getName(), aSchema->getPrimaryKeyName(), theDatatype, FreeBlockNum);
        aSchema->setIndex(theIndex);
        aSchema->setIndexNum(FreeBlockNum);
        StatusResult theResult = storage.save(theIndex, FreeBlockNum);
        uint32_t idxBlockNum = FreeBlockNum;
        FreeBlockNum = storage.findFreeBlockNum().value;
        // memory part
        if (theResult) 
        {
            if (schemas.count(aSchema->getName()) == 0)
            {
                std::string theName = aSchema->getName();
                aSchema->setBlockNum(FreeBlockNum);
                tableAddr[theName] = FreeBlockNum;
                schemas[theName] = aSchema;
                toc.list[theName] = FreeBlockNum;  // Stores index block number
                storage.save(toc, kSchemaTOCBlockNum); //update the toc;
                storage.save(*aSchema, FreeBlockNum);
                changed = true;
                return StatusResult();
            }
            else
            {
                return StatusResult(Errors::tableExists);
            }
            
        }
        return StatusResult{Errors::unknownError};
    }

    StatusResult Database::dropTable(std::string tableName)
    {
        std::map<std::string, Schema*>& theSchemas = this->getSchemas();
        Schema* aSchema=theSchemas[tableName];
        if (theSchemas.erase(tableName) != 1)
            return StatusResult(Errors::unknownTable);
        else
        {
            storage.drop(aSchema->getBlockNum());
            storage.drop(aSchema->getIndexNum());
            std::cout << aSchema->getName() << " Erased.\n";
        }
        return StatusResult();
    }

    StatusResult Database::deleteRow(std::string tableName, std::vector<Row *> RowCollection, bool deleteall)
    {
        std::map<std::string, Schema*>& theSchemas = this->getSchemas();
        if (theSchemas.count(tableName) == 0)
            return StatusResult{Errors::unknownTable};
        Schema* aSchema = theSchemas[tableName];
        RowList aRowList = theSchemas[tableName]->getRows();
        std::vector<ValueType> tobedeleted;
        if (deleteall)
        {
            for (std::vector<Row *>::iterator it = aRowList.begin(); it < aRowList.end(); it++)
            {
                delete *it;
            }
            aRowList.clear();
        }
        else
        {
            for (auto i : RowCollection)
            {
                switch (i->getData().begin()->second.index())
                {
                case 0:
                    tobedeleted.push_back(std::get<uint32_t>(i->getData().begin()->second));
                    break;
                case 1:
                    tobedeleted.push_back(std::get<int>(i->getData().begin()->second));
                    break;
                case 2:
                    tobedeleted.push_back(std::get<float>(i->getData().begin()->second));
                    break;
                case 3:
                    tobedeleted.push_back(std::get<bool>(i->getData().begin()->second));
                    break;
                case 4:
                    tobedeleted.push_back(std::get<std::string>(i->getData().begin()->second));
                    break;
                }
                
            }
            //aSchema->getRows().clear();
        }
        StatusResult theResult=storage.deleteRow(tableName, tobedeleted, deleteall);
        return theResult;
    }

    StatusResult Database::insertTable(std::string aName)
    {
        std::map<std::string, Schema*>& theSchemas = getSchemas();
        Storage& theStorage = getStorage();
        if (!theSchemas.count(aName))
        {
            return StatusResult{Errors::unknownTable};
        }
        Schema* theSchema = theSchemas[aName];
        std::vector<Row*>& rows = theSchema->getRows();
        theSchema->getPrimaryKeyName();
        
        //std::cout << rows.size();
        for (auto& i : rows)
        {
            StatusResult theResult = theStorage.findFreeBlockNum();
            uint32_t blockNum = theResult.value;
            i->tableName = theSchema->getName();
            theStorage.save(*i, blockNum);

            //std::cout << "----\n";

            // set index
            std::string thePrimaryKey = theSchema->getPrimaryKeyName();
            KeyValues theData = i->getData();
            ValueType theValue=theData[thePrimaryKey];
            theSchema->getIndex().addKeyValue(theValue, blockNum);

        }
        int temp = theSchema->getIndexNum();
        theStorage.save(theSchema->getIndex(), theSchema->getIndexNum());
        rows.clear();
        return StatusResult{};
    }

    StatusResult Database::describeTable(const std::string& aName)
    {
        std::map<std::string, Schema*>& theSchemas = getSchemas();
        int count = 0;
        if (theSchemas.count(aName) != 0)
        {
            std::cout << "Current tables: \n";
            for (auto i : theSchemas)
            {
                if (i.first == aName)
                {
                    for (auto j : i.second->getAttributes())
                    {
                        j.printAll(std::cout);
                        count += 1;
                    }
                    for (auto k : i.second->getRows())
                    {
                        k->printAll(std::cout);
                        count += 1;
                    }
                }
            }
            std::cout << count << " rows in set\n";
            return StatusResult();
        }
        return StatusResult(Errors::unknownTable);
    }

    StatusResult Database::showTables()
    {
        std::map<std::string, Schema*>& theSchemas =getSchemas();
        int count = 0;
        std::cout << "Current tables: \n";
        for (auto i : theSchemas)
        {
            std::cout << i.first << std::endl;
            count++;
        }
        std::cout << count << " rows in set\n";

        return StatusResult();
    }

    StatusResult Database::UpdateRow(std::vector<Row*>& RowCollection, std::string tableName, std::string feature, std::string feature_value)
    {
        for (int i = 0; i < RowCollection.size(); i++)
        {
            Row* theRow = RowCollection[i];
            theRow->setKV(feature, feature_value);// set the KV value
            theRow->tableName = tableName;
            getStorage().save(*theRow, theRow->pos);
        }
        return StatusResult();
    }

    StatusResult Database::selectTable(SelectPhaseHandler& theSelectPhaseHandler, std::vector<Row*>& RowCollection)
    {
        
        if (theSelectPhaseHandler.isJoin())
        {
            SelectJoin(RowCollection, theSelectPhaseHandler.getTableName(), theSelectPhaseHandler.getJoin());
        }
        if (theSelectPhaseHandler.isWhere())
        {
            totalEquations thetotalEquations=theSelectPhaseHandler.getWhereValues();
            SelectWhere(RowCollection, theSelectPhaseHandler.getWhereValue().leftOpt, theSelectPhaseHandler.getWhereValue().Opt, convertToString( theSelectPhaseHandler.getWhereValue().rightOpt), theSelectPhaseHandler.getTableName(),Keywords::null_kw);
            int i = 0;
            while (thetotalEquations.theLogics.size() > i)
            {
                
                SelectWhere(RowCollection, thetotalEquations.condEqutions[i + 1].leftOpt, thetotalEquations.condEqutions[i + 1].Opt, convertToString(thetotalEquations.condEqutions[i + 1].rightOpt), theSelectPhaseHandler.getTableName(), thetotalEquations.theLogics[i]);
                i++;
            }
        }
        if (theSelectPhaseHandler.isOrder())
        {
            //SelectOrder(std::vector<Row*>& RowCollection, std::string orderName, std::string tableName)
            SelectOrder(RowCollection, theSelectPhaseHandler.getOrderValue(), theSelectPhaseHandler.getTableName());
        }
        if (theSelectPhaseHandler.isLimited())
        {
            SelectLimit(RowCollection, theSelectPhaseHandler.getLimitedValue(), theSelectPhaseHandler.getTableName());
        }
        if (!theSelectPhaseHandler.isJoin() && !theSelectPhaseHandler.isWhere() && !theSelectPhaseHandler.isOrder() && !theSelectPhaseHandler.isLimited())
            SelectAll(RowCollection, theSelectPhaseHandler.getTableName());
        return StatusResult{};
    }

    StatusResult Database::SelectAll(std::vector<Row*>& RowCollection, std::string tableName)
    {

        std::string thePath = getDatabasePath(this->getName());
        Filters theFilters;
        theFilters.setTableName(tableName);
        theFilters.setSchema(getSchemas()[tableName]);
        theFilters.setDatabasePath(thePath);
        theFilters.setStorage(&getStorage());
        //   theFilters.set

        Timer theTimer; theTimer.start();
        StatusResult theStatus = theFilters.SelectAll(RowCollection);
        theTimer.stop();
        theStatus.elapsedTime = theTimer.elapsed();
        return theStatus;
    }
    StatusResult Database::SelectLimit(std::vector<Row*>& RowCollection, int value, std::string tableName)
    {
        std::string thePath = getDatabasePath(this->getName());
        Filters theFilters;
        theFilters.setTableName(tableName);
        theFilters.setSchema(getSchemas()[tableName]);
        theFilters.setDatabasePath(thePath);
        theFilters.setStorage(&getStorage());
        Timer theTimer; theTimer.start();
        StatusResult theStatus = theFilters.SelectLimit(RowCollection, value);
        theTimer.stop();
        theStatus.elapsedTime = theTimer.elapsed();
        return theStatus;

    }
    StatusResult Database::SelectOrder(std::vector<Row*>& RowCollection, std::string orderName, std::string tableName)
    {
        std::string thePath = getDatabasePath(this->getName());
        Filters theFilters;
        theFilters.setTableName(tableName);
        theFilters.setSchema(getSchemas()[tableName]);
        theFilters.setDatabasePath(thePath);
        theFilters.setStorage(&this->getStorage());
        Timer theTimer; theTimer.start();
        StatusResult theStatus = theFilters.SelectOrder(RowCollection, orderName);
        theTimer.stop();
        theStatus.elapsedTime = theTimer.elapsed();
        return theStatus;
    }
    StatusResult Database::SelectWhere(std::vector<Row*>& RowCollection, std::string key, std::string theOperator, std::string Value, std::string tableName, Keywords theLogic)
    {
        std::string thePath = getDatabasePath(this->getName());
        Filters theFilters;
        theFilters.setTableName(tableName);
        theFilters.setDatabasePath(thePath);
        theFilters.setStorage(&this->getStorage());
        Timer theTimer; theTimer.start();
        StatusResult theStatus = theFilters.SelectWhere(RowCollection, key, theOperator, Value, theLogic);
        theTimer.stop();
        theStatus.elapsedTime = theTimer.elapsed();
        return theStatus;
    }
    StatusResult Database::SelectJoin(std::vector<Row *> &RowCollection, std::string tableName, Join &theJoin)
    {
        std::string thePath = getDatabasePath(this->getName());
        Filters theFilters;
        theFilters.setTableName(tableName);
        theFilters.setDatabasePath(thePath);
        theFilters.setStorage(&this->getStorage());
        std::string joinTableName = theJoin.table;
        std::string lhs = theJoin.onLeft;
        std::string rhs = theJoin.onRight;
        Keywords joinType = theJoin.joinType;
        Timer theTimer;
        theTimer.start();
        StatusResult theStatus = theFilters.SelectJoin(RowCollection, tableName, joinTableName, lhs, rhs, joinType);
        theTimer.stop();
        theStatus.elapsedTime = theTimer.elapsed();
        return theStatus;
        return StatusResult();
    }
}

