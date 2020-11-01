//
//  SQLProcessor.hpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef SQLProcessor_hpp
#define SQLProcessor_hpp

#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <map>
#include <utility>

#include "CommandProcessor.hpp"
#include "Schema.hpp"
#include "Row.hpp"
#include "View.hpp"
#include "Storage.hpp"
#include "Database.hpp"
#include "Attribute.hpp"
#include "Filters.hpp"
#include "TableView.hpp"
#include "Statement.hpp"
#include "Tokenizer.hpp"
#include "Timer.hpp"

class Statement;
class Database; //define this later...

namespace ECE141 {

  class SQLProcessor : public CommandProcessor {
  public:
    
    SQLProcessor(CommandProcessor *aNext=nullptr);
    virtual ~SQLProcessor();
    
    virtual Statement*    getStatement(Tokenizer &aTokenizer);
    virtual StatusResult  interpret(Statement &aStatement);
    
  protected:
      Filters theFilters;
      TableView theView;
    //do you need other data members?
  };

}
#endif
