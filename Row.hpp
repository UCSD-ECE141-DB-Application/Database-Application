//
//  Row.hpp
//  Assignment4
//
//  Created by rick gessner on 4/19/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef Row_hpp
#define Row_hpp

#include <stdio.h>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Storage.hpp"
//#include "Database.hpp"
#include "Value.hpp"

namespace ECE141 {


  class Row : public Storable {
  public:

    Row();
    Row(const Row &aRow);
    ~Row();
    Row& operator=(const Row &aRow);
    bool operator==(Row &aCopy) const;
    
    Row& addKV(std::string key, ValueType val);
    Row& setKV(std::string& key, ValueType val);
    void printAll(std::ostream& aWriter);
      //STUDENT: What other methods do you require?
    virtual StatusResult  encode(std::ostream& aWriter);
    virtual StatusResult  decode(std::istream& aReader);
    virtual BlockType     getType() const; //what kind of block is this?
    virtual std::string   getName();
    KeyValues getData() { return data; }
    std::string tableName;
  protected:
    KeyValues data;  //you're free to change this if you like...
  };

}

#endif /* Row_hpp */
