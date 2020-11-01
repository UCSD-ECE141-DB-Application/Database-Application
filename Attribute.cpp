//
//  Attribute.cpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Attribute.hpp"
#include <iostream>

namespace ECE141 {

    //STUDENT: Implement the attribute class here...
    Attribute &Attribute::setName(std::string &aName)
    {
        name = aName;
        return *this;
    }
    Attribute &Attribute::setType(DataType aType)
    {
        type = aType;
        return *this;
    }
    Attribute &Attribute::setNull(bool aNull)
    {
        null = aNull;
        return *this;
    }
    Attribute &Attribute::setKey(bool aKey)
    {
        key = aKey;
        return *this;
    }
    Attribute &Attribute::setDef(std::string &aDef)
    {
        def = aDef;
        return *this;
    }
    Attribute &Attribute::setExtra(std::string &anExtra)
    {
        extra.push_back(anExtra);
        return *this;
    }
    Attribute &Attribute::setVarchar_num(std::string &aVarchar_num)
    {
        varchar_num = aVarchar_num;
        return *this;
    }
    Attribute &Attribute::setAuto_Increment(bool isAI)
    {
        auto_increment = isAI;
        return *this;
    }
    Attribute::Attribute(std::string aName, DataType aType, bool aNull, bool aKey, std::string aDef, std::vector<std::string> anExtra, std::string aVarchar_num, int num)
        : name(aName), type(aType), null(aNull), key(aKey), def(aDef), extra(anExtra), varchar_num(aVarchar_num), num_attributes(0) {}
    Attribute::Attribute(const Attribute &aCopy)
    {
        name = aCopy.name;
        type = aCopy.type;
        null = aCopy.null;
        key = aCopy.key;
        def = aCopy.def;
        auto_increment = aCopy.auto_increment;
        IsDefault = aCopy.IsDefault;
        defualt_value = aCopy.defualt_value;
        for (auto i : aCopy.extra)
        {
            extra.push_back(i);
        }
        varchar_num = aCopy.varchar_num;
        num_attributes = aCopy.num_attributes;
    }
    Attribute::~Attribute() {}
    void Attribute::printAll(std::ostream &aWriter)
    {
        aWriter << "Name: " << name << std::endl;
        aWriter << "Type: ";
        char theType = char(static_cast<std::underlying_type<DataType>::type>(type));
        switch(theType){
            case 'N' : aWriter <<"No Type"<< std::endl; break;
            case 'B' : aWriter <<"Boolean"<< std::endl; break;
            case 'D' : aWriter <<"Datetime"<< std::endl; break;
            case 'F' : aWriter <<"float"<< std::endl; break;
            case 'I' : aWriter <<"integer"<< std::endl; break;
            case 'V' : aWriter <<"varchar"<< std::endl; break;
        }        
        if (null)
            aWriter << "Null: " << "YES" << std::endl;
        else
            aWriter << "Null: " << "NO" << std::endl;
        if(key)
            aWriter << "Key: " << "YES" << std::endl;
        else
            aWriter << "Key: " << "NO" << std::endl;
        aWriter << "Varchar_num: " << varchar_num << std::endl;
        aWriter << "Default: " << def << std::endl;
        aWriter << "Extra: ";
        for (auto i : extra)
        {
            aWriter << i << " ";
        }
        // aWriter << "Num_attributes: " << num_attributes <<std::endl;
        aWriter << std::endl;
        aWriter << "------" << std::endl;
    }

    void Attribute::encoder(std::ostream& aWriter)
    {
        if (def == "")
            def = "NULL";
        if (type == DataType::varchar_type)
            aWriter << "attriName " << name << " " << "DataType " << static_cast<char>(type) << " " << "varchar_num " << varchar_num << " " << "null " << null << " " << "key " << key << " " << "def " << def << " ";
        else
            aWriter << "attriName " << name << " " << "DataType " << static_cast<char>(type) << " " << "varchar_num " << "empty" << " " << "null " << null << " " << "key " << key << " " << "def " << def << " ";
        if (!extra.empty())
        {
            std::vector<std::string>::iterator it;
            aWriter << "extra ";
            for (it = extra.begin(); it < extra.end(); it++)
                aWriter << *it << " ";
            aWriter << ", ";
        }
        else
            aWriter << "extra " << ", ";
    }
}
