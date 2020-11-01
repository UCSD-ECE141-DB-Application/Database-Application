#ifndef ConverHandler_hpp
#define ConverHandler_hpp
#include "Helpers.hpp"
#include "Value.hpp"
namespace ECE141 {
	DataType convertToDataType(std::string theType);
	std::string convertToString(DataType theType);
	std::string convertToString(ValueType theValueType);
	ValueType convertToValueType(std::string theValue, uint32_t idx);
	std::string convertToString(Keywords theKeyword);
}



#endif 


