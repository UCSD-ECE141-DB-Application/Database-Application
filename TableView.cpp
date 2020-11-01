#include "TableView.hpp"
#include "ConverHandler.hpp"
namespace ECE141 {

	
	bool TableView::printSeperator(std::ostream& aStream)
	{
		for (int i = 0; i < columns.size(); i++)
		{
			aStream << "+" << std::setw(columns[i].size() + 15) << std::setfill('-');
		}
		aStream << "-";
		aStream << "+\n";
		return true;
	}

	StatusResult TableView::setcolumns(std::vector<std::string> aCol) 
	{ 
		columns.assign(aCol.begin(), aCol.end()); 
		return StatusResult(); 
	}

	StatusResult TableView::setRowCollection(std::vector<Row*> aCollection) 
	{ 
		//for (auto &it: aCollection)
		//	std::cout << it->getName() << "\n";
		RowCollection.assign(aCollection.begin(), aCollection.end()); 
		return StatusResult(); 
	}

	bool  TableView::show(std::ostream& aStream)
	{
		if (columns[0] == "*" && RowCollection.size() != 0)
		{
			KeyValues theKeyValues = RowCollection[0]->getData();
			columns.clear();
			for (auto i : theKeyValues)
			{
				columns.push_back(i.first);
			}

		}
		printSeperator(aStream);
		for (int i = 0; i < columns.size(); i++)
		{
			aStream << "| " << columns[i] << std::setw(15) << std::setfill(' ');
		}
		aStream << "|\n";

		printSeperator(aStream);

			
		for (int j = 0; j < RowCollection.size(); j++) // every row
		{
			KeyValues theKeyValue=RowCollection[j]->getData();
			for (int i = 0; i < columns.size(); i++)
			{
				ValueType theValueType = theKeyValue[columns[i]];
				std::string out;
				int idx = theValueType.index();
				std::string theVal;
				switch (idx)
				{
					case 0: 
						theVal = std::to_string(std::get<uint32_t>(theKeyValue[columns[i]]));
						break;
					case 1: 
						theVal = std::to_string(std::get<int>(theKeyValue[columns[i]]));
						break;
					case 2: 
						theVal = std::to_string(std::get<float>(theKeyValue[columns[i]]));
						break;
					case 3: 
						theVal = std::to_string(std::get<bool>(theKeyValue[columns[i]]));
						if (theVal == "0")
							theVal = "FALSE";
						else
							theVal = "True";
						break;
					case 4: 
						theVal = std::get<std::string>(theKeyValue[columns[i]]); 
						break;
					default: return StatusResult(Errors::unknownError);
				}
				aStream << "| " << theVal << std::setw(columns[i].size() + 15 - theVal.size()) << std::setfill(' ');
				//aStream << "| " << theVal;
			}
			aStream << "|\n";
		}
		printSeperator(aStream);
		aStream << RowCollection.size() << " rows in set "<<"("<<elaspedTime<<" ms.)\n";
		return StatusResult{};
	}
}