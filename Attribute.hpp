//
//  Attribute.hpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Attribute_hpp
#define Attribute_hpp

#include <stdio.h>
#include <string>
#include "keywords.hpp"
#include "Value.hpp"
#include <vector>
namespace ECE141 {
      
  //enum class DataType {
    //no_type='N', bool_type='B', datetime_type='D', float_type='F', int_type='I',  varchar_type='V',
  //};

  class Attribute {
  protected:
    std::string   name;
    DataType      type;
    bool              null;   // true = yes, null,  false = no, not null, at first all entries are true
    bool              key;
    std::string     def;
    std::vector<std::string>     extra;
    std::string     varchar_num;
    bool auto_increment = false;
    bool IsDefault=false;
    bool defualt_value=false;
    
    //STUDENT: What other data should you save in each attribute?
    
  public:

    Attribute(DataType aType=DataType::no_type, bool aNull=true, bool aKey=false) : type(aType), null(aNull), key(aKey) {}
    Attribute(std::string aName, DataType aType, bool aNull, bool aKey, std::string aDef, std::vector<std::string> anExtra, std::string aVarchar_num, int num);
    Attribute(const Attribute &aCopy);
    ~Attribute();
    bool getkey() { return key; }
    bool isAutoInc() { return auto_increment; }
    bool isDefualt() { return IsDefault; }
    bool getDefaultValue() { return defualt_value; }
    Attribute &setName(std::string &aName);
    Attribute &setType(DataType aType);
    Attribute &setNull(bool aNull);
    Attribute &setKey(bool aKey);
    Attribute &setDef(std::string &aDef);
    Attribute &setExtra(std::string &anExtra);
    Attribute &setVarchar_num(std::string &aVarchar_num);
    Attribute &setAuto_Increment(bool isAI);
    Attribute& setDefault(bool isAI) { IsDefault = isAI; return *this; }
    Attribute& setDefaultValue(bool isAI) { defualt_value = isAI; return *this; }
    void encoder(std::ostream& aWriter);
    bool          isValid(); //is this schema valid? Are all the attributes value?
    
    std::string  getName() {return name;}
    DataType getType() {return type;}
    void printAll(std::ostream &aWriter);
    //STUDENT: are there other getter/setters to manage other attribute properties?
    int num_attributes;
    };
  

}


#endif /* Attribute_hpp */
