#include "Index.hpp"

namespace ECE141 {
	Index &Index::addKeyValue(const ValueType &aKey, uint32_t aValue)
	{
		list[aKey] = aValue;
		return *this;
	}
	Index &Index::removeKeyValue(const ValueType &aKey)
	{
		list.erase(aKey);
		return *this;
	}
	bool Index::contains(const ValueType &aKey)
	{
		return list.count(aKey);
	}
	uint32_t Index::getValue(const ValueType &aKey)
	{
		return list[aKey];
	}

	StatusResult Index::encode(std::ostream& aWriter) {
		aWriter << "I" << " " << "tableName " << tableName << " " << "field " << field << " " << "type " << convertToString(type) << " " << "blockNum " << blockNum << " ";

		for (auto& it : list)
		{

			aWriter << convertToString(it.first) << " " << it.second<<" ";

		}
		aWriter << ". ";
		return StatusResult{};
	}
	StatusResult Index::decode(std::istream& aReader) {
		std::string key, value;
		aReader >> key;
		if (key != "I")
			return StatusResult{ Errors::identifierExpected };

		aReader >> key >> value;
		if (key != "tableName")
			return StatusResult{ Errors::identifierExpected };
		tableName = value;

		aReader >> key >> value;
		if (key != "field")
			return StatusResult{ Errors::identifierExpected };
		field = value;

		aReader >> key >> value;
		if (key != "type")
			return StatusResult{ Errors::identifierExpected };
		type = convertToDataType(value);

		aReader >> key >> value;
		if (key != "blockNum")
			return StatusResult{ Errors::identifierExpected };
		blockNum = stoi(value);
		while (!aReader.eof())
		{
			ValueType theVal;
			aReader >> key >> value;
			if (key == ".")
				break;
			switch (type)
			{
			case DataType::int_type:
			//	theVal = convertToValueType(key, 1);
				theVal = stoi(key);
				break;
			case DataType::float_type:
				//theVal = convertToValueType(key, 2);
				theVal = stof(key);
				break;
			case DataType::varchar_type:
				//theVal = convertToValueType(key, 4);
				theVal = key;
				break;
			case DataType::bool_type:
				if (key == "0")
					theVal = false;
				else
					theVal = true;
				break;
			}
			list[theVal] = stoi(value);
		}
		return StatusResult{};
	}


}