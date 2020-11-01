#ifndef BlockHandler_hpp
#define BlockHandler_hpp
#include "Value.hpp"
#include "Index.hpp"
#include "Row.hpp"
#include "StorageBlock.hpp"

#include <filesystem>
namespace ECE141 {
	enum class IteratorKeywords {all, limit, order_by, where};
	class BlockHandler {
	public:
		BlockHandler(Storage& aStorage,std::string theFilePath) : theStorage(aStorage),filePath(theFilePath) {}
		StatusResult IteratorByIndex_All(std::vector<Row*>& RowCollection,Index& theIndex);
		StatusResult IteratorByIndex_Limit(std::vector<Row*>& RowCollection, Index& theIndex,uint32_t limitValue);
	//	StatusResult;
		StatusResult IteratorBytableName(std::vector<Row*>& RowCollection,std::string tableName);
	//	StatusResult
	protected:
		std::string filePath;
		Storage& theStorage;

	};
}
#endif
