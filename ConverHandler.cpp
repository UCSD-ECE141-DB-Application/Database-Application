#include "ConverHandler.hpp"
namespace ECE141 {
	DataType convertToDataType(std::string theType) {
		if (theType == "bool")
			return DataType::bool_type;
		if (theType == "datatime")
			return DataType::datetime_type;
		if (theType == "float")
			return DataType::float_type;
		if (theType == "int")
			return DataType::int_type;
		if (theType == "varchar")
			return DataType::varchar_type;
		return DataType::no_type;

	}

	std::string convertToString(DataType theType) {
		switch (theType)
		{
		case DataType::bool_type:
			return "bool";
		case DataType::datetime_type:
			return "datatime";
		case DataType::float_type:
			return "float";
		case DataType::int_type:
			return "int";
		case DataType::varchar_type:
			return "varchar";
		default:
			return "noType";

		}
		return "noType";
	}

	std::string convertToString(ValueType theValueType)
	{
		int idx = theValueType.index();
		std::string theVal;
		switch (idx)
		{
		case 0:
			theVal = std::to_string(std::get<uint32_t>(theValueType));
			break;
		case 1:
			theVal = std::to_string(std::get<int>(theValueType));
			break;
		case 2:
			theVal = std::to_string(std::get<float>(theValueType));
			break;
		case 3:
			theVal = std::to_string(std::get<bool>(theValueType));
			if (theVal == "0")
				theVal = "FALSE";
			else
				theVal = "True";
			break;
		case 4:
			theVal = std::get<std::string>(theValueType);
			break;
		default:
			return "wrong";
		}
		return theVal;
	}

	ValueType convertToValueType(std::string theValue, uint32_t idx)
	{
		ValueType theVal;
		switch (idx)
		{
		case 0:
			theVal = static_cast<uint32_t>(std::stoi(theValue));
			break;
		case 1:
			theVal = stoi(theValue);
			break;
		case 2:
			theVal = stof(theValue);
			break;
		case 3:
			if (theValue == "0")
				theVal = false;
			else
				theVal = true;
			break;
		case 4:
			theVal = theValue;
			break;
		default:
			return "wrong";
		}
	}


}