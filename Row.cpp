//
//  Row.cpp
//  Assignment4
//
//  Created by rick gessner on 4/19/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "Row.hpp"

namespace ECE141 {

  //STUDENT: You need to fully implement these methods...

    Row::Row() {}
    Row::Row(const Row &aRow) {}
    Row& Row::operator=(const Row &aRow) {return *this;}
    bool Row::operator==(Row &aCopy) const {return false;}
    Row::~Row() {}

    Row& Row::addKV(std::string key, ValueType val)
    {
        data[key] = val;
        //std::cout << "{" << key << ": " << std::get<std::string>(val) << "}\n";
        return *this;
    }

    Row& Row::setKV(std::string& key, ValueType val)
    {
        data[key] = val;
        //std::cout << "{" << key << ": " << std::get<std::string>(val) << "}\n";
        return *this;
    }
    void Row::printAll(std::ostream& aWriter)
    {
        for (auto i : data) {
            //aWriter  << i.first << ": " << std::get<std::string>(i.second) << "\n";
            switch (i.second.index())
            {
                case 0: aWriter << i.first << ": " << std::get<uint32_t>(i.second) << ", idx=" << i.second.index() << "\n"; break;
                case 1: aWriter << i.first << ": " << std::get<int>(i.second) << ", idx=" << i.second.index() << "\n"; break;
                case 2: aWriter << i.first << ": " << std::get<float>(i.second) << ", idx=" << i.second.index() << "\n"; break;
                case 3: aWriter << i.first << ": " << std::get<bool>(i.second) << ", idx=" << i.second.index() << "\n"; break;
                case 4: aWriter << i.first << ": " << std::get<std::string>(i.second) << ", idx=" << i.second.index() << "\n"; break;
            }
            
        }
        aWriter << "\n";
    }
    StatusResult  Row::encode(std::ostream& aWriter) 
    {
        //std::cout << "inside row encode!\n";
        for (auto i : data)
        {
            switch (i.second.index())
            {
                case 0: 
                    //std::cout << "D " << tableName << " " << i.first << " " << std::get<uint32_t>(i.second) << " " << i.second.index() << "\n";
                    aWriter << "D " << tableName << " " << i.first << " " << std::get<uint32_t>(i.second) << " " << i.second.index() << " ";
                    break;
                case 1: 
                    //std::cout << "D " << tableName << " " << i.first << " " << std::get<int>(i.second) << " " << i.second.index() << "\n";
                    aWriter << "D " << tableName << " " << i.first << " " << std::get<int>(i.second) << " " << i.second.index() << " ";
                    break;
                case 2: 
                    //std::cout << "D " << tableName << " " << i.first << " " << std::get<float>(i.second) << " " << i.second.index() << "\n";
                    aWriter << "D " << tableName << " " << i.first << " " << std::get<float>(i.second) << " " << i.second.index() << " ";
                    break;
                case 3: 
                    //std::cout << "D " << tableName << " " << i.first << " " << std::get<bool>(i.second) << " " << i.second.index() << "\n";
                    aWriter << "D " << tableName << " " << i.first << " " << std::get<bool>(i.second) << " " << i.second.index() << " ";
                    break;
                case 4: 
                    //std::cout << "D " << tableName << " " << i.first << " " << std::get<std::string>(i.second) << " " << i.second.index() << "\n";
                    aWriter << "D " << tableName << " " << i.first  << " " << std::get<std::string>(i.second) << " " << i.second.index() << " ";
                    break;
                default:
                    return StatusResult(Errors::unknownError);
            }
        }
        return StatusResult(); 
    }

    bool stob(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        std::istringstream is(str);
        bool b;
        is >> std::boolalpha >> b;
        return b;
    }

    StatusResult  Row::decode(std::istream& aReader) 
    {
        //std::cout << "inside row decode!\n";
        while (!aReader.eof())
        {
            std::string dtype;
            std::string tbname;
            std::string key;
            std::string val;
            int idx;
            aReader >> dtype >> tbname >> key >> val >> idx;
            if (dtype != "D")
                break;
            //std::cout << dtype << " " << tbname<< " " << key << ": " << val << ", idx=" <<idx << std::endl;
            switch (idx)
            {
                case 0: data[key] = stoi(val); break;
                case 1: data[key] = stoi(val); break;
                case 2: data[key] = stof(val); break;
                case 3: data[key] = val == "1" ? true : false; break;
                case 4: data[key] = val; break;
                default: return StatusResult(Errors::unknownError);
            }
            
        }
        return StatusResult{};
    }
    BlockType     Row::getType() const { return BlockType::data_block; } //what kind of block is this?
    std::string   Row::getName() { return tableName; }

}
