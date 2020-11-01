#include "BlockHandler.hpp"

namespace ECE141 {
	StatusResult BlockHandler::IteratorByIndex_All(std::vector<Row*>& RowCollection, Index& theIndex) 
	{
		RowCollection.clear();
		for (auto& i : theIndex.getList())
		{
			Row* theRowPtr=new Row();
			theStorage.load(*theRowPtr, i.second);
			RowCollection.push_back(theRowPtr);
		}
		return StatusResult{};
	}

	StatusResult BlockHandler::IteratorByIndex_Limit(std::vector<Row*>& RowCollection, Index& theIndex, uint32_t limitValue) {
		RowCollection.clear();
		int count = 0;
		for (auto& i : theIndex.getList())
		{
			Row* theRowPtr = new Row();
			theStorage.load(*theRowPtr, i.second);
			RowCollection.push_back(theRowPtr);
			count++;
			if (count > limitValue)
				break;
		}
		return StatusResult{};
	}

	StatusResult BlockHandler::IteratorBytableName(std::vector<Row*>& RowCollection, std::string tableName) {

		uint32_t size = std::filesystem::file_size(filePath);
		int blockSize = ECE141::BlockInfo::getBlockSize();
		int Totalnum = size / blockSize;
		std::fstream aStream(filePath, std::ios::binary | std::ios::out | std::ios::in);
		char *buf = new char[blockSize];
		RowCollection.clear();
		for (int i = 0; i < Totalnum; i++)
		{
			aStream.read(buf, blockSize);
			std::stringstream ss(buf);
			std::string type, thetableName;
			ss >> type >> thetableName;
			if (type == "D" && tableName == thetableName)
			{
				Row* theRow = new Row();
				theStorage.load(*theRow, i);
				RowCollection.push_back(theRow);
			}
		}
		return StatusResult{};
	}

}