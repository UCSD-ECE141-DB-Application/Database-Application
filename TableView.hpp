#ifndef TableView_hpp
#define TableView_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

#include "Errors.hpp"
#include "Value.hpp"
#include "Tokenizer.hpp"
#include "View.hpp"
#include "Row.hpp"
#include <variant>

namespace ECE141 {
	class TableView : public View
	{
	public:
		TableView(){}
		TableView(const TableView &aTableView) {}
		~TableView(){}
		bool    show(std::ostream& aStream);
		bool printSeperator(std::ostream& aStream);
	//	std::string convertToString(ValueType);
		
		StatusResult setcolumns(std::vector<std::string> aCol);
		StatusResult setRowCollection(std::vector<Row*> aCollection);
		double elaspedTime = 0;
	protected:
		std::vector<std::string> columns;
		std::vector<Row*> RowCollection;

	};

}

#endif /* TableView_hpp */