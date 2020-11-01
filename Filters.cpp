//
//  Filters.hpp
//  Assignement6
//
//  Created by rick gessner on 5/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//


#include <stdio.h>
#include <filesystem>
#include <fstream>
#include "BlockHandler.hpp"
#include "Filters.hpp"

namespace ECE141 {
	StatusResult Filters::SelectAll(std::vector<Row*>& RowCollection)
	{
		
		BlockHandler theBlockHandler(*thestorage,  DatabasePath);
		theBlockHandler.IteratorByIndex_All(RowCollection, theSchemaPtr->getIndex());
		return StatusResult{};
	}
	StatusResult Filters::SelectLimit(std::vector<Row*>& RowCollection, int value) 
	{
		if (!RowCollection.empty())
		{
			if (RowCollection.size() <= value)
				return StatusResult{};
			RowCollection.erase(RowCollection.begin() + value, RowCollection.end());
		}
		else
		{
			BlockHandler theBlockHandler(*thestorage, DatabasePath);
			theBlockHandler.IteratorByIndex_Limit(RowCollection, theSchemaPtr->getIndex(), value);
		}
		return StatusResult{};
	}

	struct RowComparator {
		//https://stackoverflow.com/questions/26476926/for-c-sort-how-to-pass-a-parameter-to-custom-compare-function
		explicit RowComparator(std::string theName) : orderName(theName) {}

		bool operator ()(Row* a, Row* b) const 
		{
			return (a->getData()[orderName] < b->getData()[orderName]);
		}

		std::string orderName;
	};

	StatusResult RowOrder(std::vector<Row*>& RowCollection, std::string orderName)
	{
		std::sort(RowCollection.begin(), RowCollection.end(), RowComparator(orderName));
		return StatusResult{};
	}


	StatusResult Filters::SelectOrder(std::vector<Row*>& RowCollection, std::string orderName)
	{
		uint32_t size = std::filesystem::file_size(DatabasePath);
		int blockSize = ECE141::BlockInfo::getBlockSize();
		int Totalnum = size / blockSize;
		std::fstream aStream(DatabasePath, std::ios::binary | std::ios::out | std::ios::in);
		char *buf = new char[blockSize];
		std::vector<ValueType> orders;
		std::map<ValueType, int> themap;
		if (RowCollection.empty())
		{
			BlockHandler theBlockHandler(*thestorage, DatabasePath);
			theBlockHandler.IteratorByIndex_All( RowCollection, theSchemaPtr->getIndex());
		}
		RowOrder(RowCollection, orderName);
		aStream.close();
		return StatusResult();
	}

	bool to_bool(std::string str) 
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		std::istringstream is(str);
		bool b;
		is >> std::boolalpha >> b;
		return b;
	}

	bool handleOps(std::string theOperator, ValueType theData, ValueType theComp)
	{
		if (theOperator == "=")  // =
			return theData == theComp;
		else if (theOperator == "<")  // <
			return theData < theComp;
		else if (theOperator == ">")  // >
			return theData > theComp;
		else if (theOperator == "!=")  // !=
			return theData != theComp;
		else if (theOperator == "<=")  // <=
			return theData <= theComp;
		else if (theOperator == ">=")  // >=
			return theData >= theComp;
		return false;
	}
	bool contains(std::vector<Row*>& RowCollection, Row* theRow)
	{
		KeyValues theData = theRow->getData();
		
		for (auto i : RowCollection)
		{
			bool contain = true;
			for (auto j : i->getData())
			{	
				if (theData[j.first] != j.second)
					contain = false;
			}
			if (contain)
				return true;
		}
		return false;
	}
	StatusResult RowWhere(std::vector<Row *> &RowCollection, std::string key, std::string theOperator, std::string val, std::map<std::string, DataType> typeCollection, Keywords theLogic, std::vector<Row*>& OrCollection)
	{
		// key -> attribute name, val -> attribute value
		std::vector<Row*> tempCollection;
		for (auto& row : RowCollection)
		{
			for (auto& data : row->getData())
			{
				std::string theAttrName = data.first;
				if (theAttrName == key) {
					if (typeCollection[theAttrName] == DataType::bool_type)
					{
						bool theData = std::get<bool>(data.second);
						bool theComp = to_bool(val);
						if (handleOps(theOperator, theData, theComp))
							tempCollection.push_back(row);
					}
					else if (typeCollection[theAttrName] == DataType::int_type)
					{
						int theData = std::get<int>(data.second);
						int theComp = stoi(val);
						if (handleOps(theOperator, theData, theComp))
							tempCollection.push_back(row);
					}
					else if (typeCollection[theAttrName] == DataType::float_type)
					{
						float theData = std::get<float>(data.second);
						float theComp = stof(val);
						if (handleOps(theOperator, theData, theComp))
							tempCollection.push_back(row);
					}
					else
					{
						if (data.second.index() == 1)
						{
							int theData = std::get<int>(data.second);
							int theComp = stoi(val);
							if (handleOps(theOperator, theData, theComp))
								tempCollection.push_back(row);
						}
						else {
							std::string theData = std::get<std::string>(data.second);
							if (handleOps(theOperator, theData, val))
								tempCollection.push_back(row);
						}

					}
				}
			}
		}
		// Contains logic
		if (theLogic != Keywords::null_kw)
		{
			if (theLogic == Keywords::and_kw)
			{
				// since the rowcollection already has the previous logical data
				RowCollection.clear();
				if (tempCollection.size() != 0)
					RowCollection = tempCollection;
			}
			else if(theLogic == Keywords::or_kw)
			{
				for (auto row : tempCollection)
				{
					if (!contains(OrCollection, row))
						OrCollection.push_back(row);
				}
				RowCollection = OrCollection;
			}
			else
			{
				std::cout << "Unknown keywords for where.\n";
				return StatusResult(Errors::unknownCommand);
			}
		}
		// No logic situation
		else
		{
			RowCollection.clear();
			if (tempCollection.size() != 0)
				RowCollection = tempCollection;
		}
		return StatusResult();
	}

	StatusResult Filters::SelectWhere(std::vector<Row *> &RowCollection, std::string key, std::string theOperator, std::string value, Keywords theLogic)
	{
		uint32_t size = std::filesystem::file_size(DatabasePath);
		int blockSize = ECE141::BlockInfo::getBlockSize();
		int Totalnum = size / blockSize;

		std::fstream aStream(DatabasePath, std::ios::binary | std::ios::out | std::ios::in);
		char *buf = new char[blockSize];

		std::vector<Row*> OrCollection;

		if (theLogic == Keywords::or_kw)
			OrCollection = RowCollection;

		if (RowCollection.empty() || theLogic == Keywords::or_kw)
		{
			for (int i = 0; i < Totalnum; i++)
			{
				aStream.read(buf, blockSize);
				std::stringstream ss(buf);
				std::string type, field1, field2;
				ss >> type >> field1 >> field2;
				if (type == "S" && tableName == field2)
				{
					Schema *theSchema = new Schema(tableName);
					thestorage->load(*theSchema, i);
					for (auto attr : theSchema->getAttributes())
					{
						char theType = char(static_cast<std::underlying_type<DataType>::type>(attr.getType()));
						switch (theType) {
							case 'N': typeCollection[attr.getName()] = DataType::no_type; break;
							case 'B': typeCollection[attr.getName()] = DataType::bool_type; break;
							case 'D': typeCollection[attr.getName()] = DataType::datetime_type; break;
							case 'F': typeCollection[attr.getName()] = DataType::float_type; break;
							case 'I': typeCollection[attr.getName()] = DataType::int_type; break;
							case 'V': typeCollection[attr.getName()] = DataType::varchar_type; break;
						}
						//std::cout << attr.getName() << ": " << theType << std::endl;
					}
				}
				if (type == "D" && tableName == field1)
				{
					Row *theRow = new Row();
					thestorage->load(*theRow, i);
					RowCollection.push_back(theRow);
				}
			}
		}
		RowWhere(RowCollection, key, theOperator, value, typeCollection, theLogic, OrCollection);
		aStream.close();
		return StatusResult{};
	}

	StatusResult Filters::SelectJoin(std::vector<Row *> &RowCollection, std::string tableName, std::string joinTableName, std::string lhs, std::string rhs, Keywords joinType)
	{
		BlockHandler theBlockHandler(*thestorage, DatabasePath);
		std::vector<Row*> origCollection;
		std::vector<Row*> joinCollection;
		theBlockHandler.IteratorBytableName(origCollection, tableName);
		theBlockHandler.IteratorBytableName(joinCollection, joinTableName);
		if (joinType == Keywords::left_kw)
		{
			for (int i = 0; i < origCollection.size(); i++)
			{
				KeyValues rowData = origCollection[i]->getData();
				Row *theRow = origCollection[i];
				int count = 0;
				for (int j = 0; j < joinCollection.size(); j++)
				{
					KeyValues joinData = joinCollection[j]->getData();
					if (rowData[lhs] == joinData[rhs])
					{
						for (auto &k : joinData)
						{
							theRow->addKV(k.first, k.second);
						}
						count++;
						RowCollection.push_back(theRow);
					}
				}
				if (count == 0)
				{
					for (auto& l : joinCollection[0]->getData())
					{
						theRow->addKV(l.first, std::string("NULL"));
					}
					RowCollection.push_back(theRow);
				}
			}
		}
		else if (joinType == Keywords::right_kw)
		{
			for (int i = 0; i < joinCollection.size(); i++)
			{
				KeyValues rowData = joinCollection[i]->getData();
				Row *theRow = joinCollection[i];
				int count = 0;
				for (int j = 0; j < origCollection.size(); j++)
				{
					KeyValues joinData = origCollection[j]->getData();
					if (rowData[lhs] == joinData[rhs])
					{
						for (auto &k : joinData)
						{
							theRow->addKV(k.first, k.second);
						}
						count++;
						RowCollection.push_back(theRow);
					}
				}
				if (count == 0)
				{
					for (auto& l : origCollection[0]->getData())
					{
						theRow->addKV(l.first, std::string("NULL"));
					}
					RowCollection.push_back(theRow);
				}
			}
		}
		else
		{
			return StatusResult(Errors::unknownAttribute);
		}
		
		return StatusResult();
	}
}



