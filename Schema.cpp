//
//  Schema.cpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Schema.hpp"
#include <iostream>
#include <string>

namespace ECE141 {

 //STUDENT: Implement the Schema class here...
    Schema::Schema(const std::string aName) : name(aName) {}
    Schema::Schema(const Schema &aCopy)
    {
        name = aCopy.name;
        attributes = aCopy.attributes;
        blockNum = aCopy.blockNum; //storage location.
        changed = aCopy.changed;
        theIndex = aCopy.theIndex;
    }
    Schema ::~Schema() {}
    Schema &Schema::addAttribute( Attribute &anAttribute)
    {
        attributes.push_back(anAttribute);
        attriMap[anAttribute.getName()]= anAttribute;
        return *this;
    }

    std::string Schema::getPrimaryKeyName() const
    {
        for (auto Attri : attributes)
        {
            if (Attri.getkey())
                return Attri.getName();
        }
        return "";
    }
    Schema& Schema::addRow(Row* aRow)
    {
        rows.push_back(aRow);
        return *this;
    }
    StatusResult Schema::encode(std::ostream& aWriter)
    {
        if (aWriter.good())
        {
            //aWriter << "S" << " " << "name " << name << " " << "blockNum " << blockNum << " " << "changed " << changed << " " << "\n";
            aWriter << "S" << " " << "indexNum " << indexNum << " " << "name " << name<<" ";
            //aWriter << "Attributes: ";
            std::vector<Attribute>::iterator it;
            for (it = attributes.begin(); it < attributes.end(); it++)
            {
                it->encoder(aWriter);
            }
        }
        return StatusResult(Errors::unknownDatabase);
    }
    StatusResult Schema::decode(std::istream& aReader)
    {
        char checkflag;
        aReader >> checkflag;
        if (checkflag != 'S')
            return StatusResult{ Errors::readError };
        std::string title;
        std::string content;
        aReader >> title >> content;
        if (title == "indexNum") indexNum = stoi(content);
        while (!aReader.eof())
        {

            aReader >> title >> content;
            if (title == "name") name = content;

          //  if (title == "blockNum") blockNum = std::stoi(content);
           // if (title == "changed") changed = std::stoi(content);
            if (title == "attriName")
            {
                Attribute* theAttribute =new Attribute;
                Attribute& aAttribute = *theAttribute;
                //Attribute aAttribute;
                aAttribute.setName(content);
 /*               aReader >> title >> content;
                if (title == "name") aAttribute.setName(content);*/
                aReader >> title >> content;
                if (title == "DataType")  aAttribute.setType(static_cast<DataType>(content[0]));
                aReader >> title >> content;
                if (title == "varchar_num" && content != "empty")  aAttribute.setVarchar_num(content);
                aReader >> title >> content;
                if (title == "null") aAttribute.setNull(std::stoi(content));
                aReader >> title >> content;
                if (title == "key") aAttribute.setKey(std::stoi(content));
                aReader >> title >> content;
                if (title == "def")
                {
                    std::string empty = "";
                    if (content == "NULL") aAttribute.setDef(empty);
                    else aAttribute.setDef(content);
                }
                aReader >> title;
                if (title == "extra")
                {
                    while (true)
                    {
                        aReader >> content;
                        if (content == "primary")
                        {
                            std::string buf;
                            aReader >> buf;
                            content = content+" "+ buf;
                        }
                        if (content == ",")
                            break;
                        aAttribute.setExtra(content);
                    }

                }
                attributes.push_back(aAttribute);

            }

        }
        return StatusResult{};

    }
}

