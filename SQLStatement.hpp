
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stdlib.h>
#include <string>
#include <unordered_set>


#include "Database.hpp"
#include "Errors.hpp"
#include "Helpers.hpp"
#include "Schema.hpp"
#include "Statement.hpp"
#include "Storage.hpp"
#include "Tokenizer.hpp"
#include "TableView.hpp"
#include "Timer.hpp"
#include "Database.hpp"
#include "PhaseHandler.hpp"
namespace ECE141 {
    class SQLProcessor;
	class CreateSQLStatement : public Statement {
    public:
        CreateSQLStatement(Tokenizer& aTokenizer, Database* aDatabase) : Statement(Keywords::create_kw), theTokenizer(aTokenizer), theDatabase(aDatabase) {}
        virtual const char* getStatementName() { return "Create Statement"; }
        Attribute parseAttribute(std::vector<Token> temp);
        StatusResult parseAttributes(Tokenizer& aTokenizer);
        virtual   StatusResult  parse(Tokenizer& aTokenizer);
        virtual   StatusResult  run(std::ostream& aStream);

    protected:
        Tokenizer& theTokenizer;
        //  SQLProcessor& processor;
        Database* theDatabase;
        std::vector<Attribute> attributes;
        std::string tableName;
	};

    class DropSQLStatement : public Statement
    {
    public:
    public:
        DropSQLStatement(Tokenizer& aTokenizer, Database* aProcessor) : Statement(Keywords::drop_kw), theTokenizer(aTokenizer), theDatabase(aProcessor) {}
        virtual const char* getStatementName() { return "Drop Statement"; }
        virtual StatusResult parse(Tokenizer& aTokenizer);
        virtual StatusResult run(std::ostream& aStream);

    protected:
        Tokenizer& theTokenizer;
       //SQLProcessor& processor;
        Database* theDatabase;
        std::string tableName;
    };

    class UseSQLStatement : public Statement
    {
    public:
        UseSQLStatement(Tokenizer& aTokenizer, Database* aProcessor) : Statement(Keywords::use_kw), theTokenizer(aTokenizer), theDatabase(aProcessor) {}
        virtual const char* getStatementName() { return "Use Statement"; }
        virtual StatusResult parse(Tokenizer& aTokenizer);
        virtual StatusResult run(std::ostream& aStream);

    protected:
        Tokenizer& theTokenizer;
        Database* theDatabase;
        std::string tableName;
    };

    class DescribeSQLStatement : public Statement
    {
    public:
        DescribeSQLStatement(Tokenizer& aTokenizer, Database* aProcessor) : Statement(Keywords::describe_kw), theTokenizer(aTokenizer), theDatabase(aProcessor) {}
        virtual const char* getStatementName() { return "Describe Statement"; }
        virtual StatusResult parse(Tokenizer& aTokenizer);
        virtual StatusResult run(std::ostream& aStream);
    protected:
        Tokenizer& theTokenizer;
        //SQLProcessor& processor;
        std::string tableName;
        Database* theDatabase;
    };

    class ShowSQLStatement : public Statement
    {
    public:
        ShowSQLStatement(Tokenizer& aTokenizer, Database* aProcessor) : Statement(Keywords::show_kw), theTokenizer(aTokenizer), theDatabase(aProcessor) {}
        virtual const char* getStatementName() { return "Show Statement"; }
        virtual StatusResult parse(Tokenizer& aTokenizer);
        virtual StatusResult run(std::ostream& aStream);
    protected:
        Tokenizer& theTokenizer;
        //SQLProcessor& processor;
        std::string tableName;
        Database* theDatabase;
    };


    class InsertSQLStatement : public Statement
    {
    public:
        InsertSQLStatement(Tokenizer& aTokenizer, Database* aProcessor) : Statement(Keywords::insert_kw), theTokenizer(aTokenizer), theDatabase(aProcessor) {}
        virtual const char* getStatementName() { return "Insert Statement"; }
        StatusResult parseRows(Tokenizer& aTokenizer);
        

        virtual StatusResult parse(Tokenizer& aTokenizer);

        virtual StatusResult run(std::ostream& aStream);

    protected:
        Tokenizer& theTokenizer;
        //SQLProcessor& processor;
        std::string tableName;
        std::vector<Row*> rows;
        Schema* theSchema;
        Database* theDatabase;
    };

    class ShowIndexStatement : public Statement {
    protected:
        Tokenizer& theTokenizer;
       // SQLProcessor& processor;
        Database* theDatabase;
    public:
        ShowIndexStatement(Tokenizer& aTokenizer, Database* aProcessor) : Statement(Keywords::delete_kw), theTokenizer(aTokenizer), theDatabase(aProcessor) {}
        virtual const char* getStatementName() { return "show Indexes"; }
        virtual StatusResult parse(Tokenizer& aTokenizer);
        virtual StatusResult run(std::ostream& aStream);
    };

    // delete from 
    class DeleteSQLStatement : public Statement
    {
    protected:
        Tokenizer& theTokenizer;
        //SQLProcessor& processor;
        std::string tableName;
        std::vector<Row*> RowCollection;
        std::vector<std::string> AllAttriname;
        bool deleteall;
        Database* theDatabase;
    public:
        DeleteSQLStatement(Tokenizer& aTokenizer, Database* aProcessor) : Statement(Keywords::delete_kw), theTokenizer(aTokenizer), theDatabase(aProcessor) {}

        virtual const char* getStatementName() { return "Delete Statement"; }
        virtual StatusResult parse(Tokenizer& aTokenizer);
        virtual StatusResult run(std::ostream& aStream);
    };

    class SelectSQLStatement : public Statement
    {
    protected:
        Tokenizer& theTokenizer;
        SelectPhaseHandler theSelectPhase;
        

        Database* theDatabase;

    public:
        SelectSQLStatement(Tokenizer& aTokenizer, Database* aProcessor) : Statement(Keywords::select_kw), theTokenizer(aTokenizer), theDatabase(aProcessor) {}
        virtual const char* getStatementName() { return "Select Statement"; }
        virtual StatusResult parse(Tokenizer& aTokenizer);
        virtual StatusResult run(std::ostream& aStream);
    };

    class UpdateSQLStatement : public Statement
    {
    public:
        UpdateSQLStatement(Tokenizer& aTokenizer, Database* aProcessor) : Statement(Keywords::update_kw), theTokenizer(aTokenizer), theDatabase(aProcessor) {}
        virtual const char* getStatementName() { return "Update Statement"; }
        virtual StatusResult parse(Tokenizer& aTokenizer);
        virtual StatusResult run(std::ostream& aStream);
    protected:
        Tokenizer& theTokenizer;
       // SQLProcessor& processor;
        std::string tableName;
        std::string feature;
        std::string feature_value;
        std::vector<std::string> conditions;
        std::vector<std::string> condition_values;
        std::vector<Keywords> operators;
        std::vector<Row*> RowCollection;
        std::vector<std::string> AllAttriname;\
        Database* theDatabase;
    };




}
