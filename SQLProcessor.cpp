//
//  SQLProcessor.cpp
//  Assignment4
//
//  Created by rick gessner on 4/18/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#include "SQLProcessor.hpp"
#include "SQLStatement.hpp"


namespace fs = std::filesystem;

namespace ECE141
{
    SQLProcessor::SQLProcessor(CommandProcessor *aNext) : CommandProcessor(aNext) {}

    SQLProcessor::~SQLProcessor() {}

    StatusResult SQLProcessor::interpret(Statement &aStatement)
    {
        Tokenizer *aTokenizerPtr = nullptr;
        StatusResult theStatusResult = aStatement.parse(*aTokenizerPtr);
        if (theStatusResult.code == Errors::noError)
            return aStatement.run(std::cout);
        else
            return theStatusResult;
    }

    Statement *SQLProcessor::getStatement(Tokenizer &aTokenizer)
    {
        if (aTokenizer.current().keyword == Keywords::create_kw && aTokenizer.peek(1).keyword == Keywords::table_kw)
        {
            aTokenizer.next(2);
            Database* thedatabase =this->getActiveDatabase();
            return new CreateSQLStatement(aTokenizer, thedatabase);
        }
        if (aTokenizer.current().keyword == Keywords::drop_kw && aTokenizer.peek(1).keyword == Keywords::table_kw)
        {
            aTokenizer.next(2);
            Database* thedatabase = this->getActiveDatabase();
            return new DropSQLStatement(aTokenizer, thedatabase);
        }
        if (aTokenizer.current().keyword == Keywords::use_kw && aTokenizer.peek(1).keyword == Keywords::table_kw)
        {
            Database* thedatabase = this->getActiveDatabase();
            return new UseSQLStatement(aTokenizer, thedatabase);
        }
        if (aTokenizer.current().keyword == Keywords::describe_kw && aTokenizer.peek(1).keyword != Keywords::database_kw)
        {
            aTokenizer.next(1);
            Database* thedatabase = this->getActiveDatabase();
            return new DescribeSQLStatement(aTokenizer, thedatabase);
        }
        if (aTokenizer.current().keyword == Keywords::show_kw && aTokenizer.peek(1).keyword == Keywords::tables_kw)
        {
            aTokenizer.next(2);
            Database* thedatabase = this->getActiveDatabase();
            return new ShowSQLStatement(aTokenizer, thedatabase);
        }
        if (aTokenizer.current().keyword == Keywords::insert_kw && aTokenizer.peek(1).keyword == Keywords::into_kw)
        {
            aTokenizer.next(2);
            Database* thedatabase = this->getActiveDatabase();
            return new InsertSQLStatement(aTokenizer, thedatabase);
        }
        if (aTokenizer.current().keyword == Keywords::delete_kw && aTokenizer.peek(1).keyword == Keywords::from_kw)
        {
            aTokenizer.next(2);
            Database* thedatabase = this->getActiveDatabase();
            return new DeleteSQLStatement(aTokenizer, thedatabase);
        }
        if (aTokenizer.current().keyword == Keywords::select_kw)
        {
            aTokenizer.next();
            Database* thedatabase = this->getActiveDatabase();
            return new SelectSQLStatement(aTokenizer, thedatabase);
        }
        if (aTokenizer.current().keyword == Keywords::update_kw)
        {
            aTokenizer.next();
            Database* thedatabase = this->getActiveDatabase();
            return new UpdateSQLStatement(aTokenizer, thedatabase);
        }

        if (aTokenizer.current().keyword == Keywords::show_kw && aTokenizer.peek(1).data == "indexes") {
            aTokenizer.next(2);
            Database* thedatabase = this->getActiveDatabase();
            return new ShowIndexStatement(aTokenizer, thedatabase);
        }

        return nullptr;
    }

} // namespace ECE141
