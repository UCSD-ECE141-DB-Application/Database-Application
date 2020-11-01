#include "SQLStatement.hpp"
#include "CheckHandler.hpp"
namespace fs = std::filesystem;
namespace ECE141{
    bool s_tob(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        std::istringstream is(str);
        bool b;
        is >> std::boolalpha >> b;
        return b;
    }

    StatusResult CreateSQLStatement::parseAttributes(Tokenizer& aTokenizer)
    {
        //create table tasks (id INT AUTO_INCREMENT PRIMARY KEY,title VARCHAR(100) NOT NULL,price FLOAT DEFAULT 0.0,due_date TIMESTAMP, status BOOLEAN DEFAULT FALSE,)
        while (aTokenizer.more())
        {
            std::vector<Token> temp;
            while (theTokenizer.more() && theTokenizer.current().data != ",")
            {
                temp.push_back(theTokenizer.current());
                theTokenizer.next();
            }
            if (temp[0].data != ")") // pass the last ")"
            {
                attributes.push_back(parseAttribute(temp));

                //  attriMap[attributes.back().getName()] = attributes.back();
            }

            temp.clear();
            theTokenizer.next();
        }
        return StatusResult();
    }

    Attribute CreateSQLStatement::parseAttribute(std::vector<Token> temp)
    {
        Attribute theAttribute;
        if (temp[0].data != "(" && temp[0].data != ")" && temp.size() > 1)
            theAttribute.setName(temp[0].data);
        else
            theAttribute.setName(temp[1].data);
        for (int i = 0; i < temp.size(); i++)
        {
            if (temp[i].data != "(" && temp[i].data != ")" && temp[i].data != ",")
            {
                if (temp[i].data == "DEFAULT")
                {
                    theAttribute.setDefault(true);
                    theAttribute.setExtra(temp[i].data);
                    i++;
                    if (temp[i].data == "FALSE")
                        theAttribute.setDefaultValue(false);
                    else
                        theAttribute.setDefaultValue(true);
                }

                if (temp[i].type == TokenType::keyword)
                {
                    switch (temp[i].keyword)
                    {
                    case Keywords::integer_kw:
                        theAttribute.setType(DataType::int_type); break;
                    case Keywords::varchar_kw:
                        theAttribute.setType(DataType::varchar_type);
                        i += 2; theAttribute.setVarchar_num(temp[i].data); break;
                    case Keywords::float_kw:
                        theAttribute.setType(DataType::float_type); break;
                    case Keywords::boolean_kw:
                        theAttribute.setType(DataType::bool_type); break;
                    case Keywords::datetime_kw:
                        theAttribute.setType(DataType::datetime_type); break;
                    case Keywords::auto_increment_kw:
                        theAttribute.setAuto_Increment(true);
                        theAttribute.setExtra(temp[i].data); break;
                        //   case Keywords::d
                    case Keywords::primary_kw:
                        theAttribute.setExtra(temp[i].data); break;
                    case Keywords::key_kw:
                        theAttribute.setExtra(temp[i].data);
                        theAttribute.setKey(true); break;
                    case Keywords::not_kw:
                        if (temp[i++].keyword == Keywords::null_kw)
                            theAttribute.setNull(false);
                        break;
                    default:
                        std::cout << temp[i].data << " Not determined\n";
                    }
                }
                if (temp[i].type != TokenType::keyword && temp[i].type != TokenType::number)
                {
                    if (temp[i].data == "DEFAULT")
                    {
                        i += 1;
                        theAttribute.setDef(temp[i].data);
                    }
                }
            }
        }
        return theAttribute;
    }

    StatusResult CreateSQLStatement::parse(Tokenizer& aTokenizer)
    {
        tableName = theTokenizer.current().data;
        if (!theDatabase)
            return StatusResult(Errors::unknownDatabase);
        if (!isValidSQLName(tableName))
            return StatusResult(ECE141::Errors::illegalIdentifier);
        theTokenizer.next();
        return parseAttributes(theTokenizer);
    }

    StatusResult CreateSQLStatement::run(std::ostream& aStream)
    {
        Schema* theSchema = new Schema(tableName);
        //   Index* the=new Index();
        for (auto& i : attributes)
        {
            // i.printAll(std::cout);
            theSchema->addAttribute(i);
        }
        if (!theDatabase)
            return StatusResult{ unknownDatabase };
        return theDatabase->createTable(theSchema);
    }

    StatusResult DropSQLStatement::parse(Tokenizer& aTokenizer) {
        tableName = theTokenizer.current().data;
        theTokenizer.next();
        return StatusResult{};
    }
    StatusResult DropSQLStatement::run(std::ostream& aStream)
    {
        if (!theDatabase)
            return StatusResult{ unknownDatabase };
        return theDatabase->dropTable(tableName);
    }

    StatusResult UseSQLStatement::parse(Tokenizer& aTokenizer)
    {
        tableName = theTokenizer.current().data;
        if (!isSQLExist(theTokenizer.current().data))
            return StatusResult(Errors::unknownTable);

        theTokenizer.next();
        return StatusResult{};
    }
    StatusResult UseSQLStatement::run(std::ostream& aStream)
    {
        return StatusResult();
    }

    StatusResult DescribeSQLStatement::parse(Tokenizer& aTokenizer)
    {
        tableName = theTokenizer.current().data;
        theTokenizer.next();
        return StatusResult{};
    }
    StatusResult DescribeSQLStatement::run(std::ostream& aStream)
    {
        if (!theDatabase)
            return StatusResult{ unknownDatabase };
        return theDatabase->describeTable(tableName);
    }

    StatusResult ShowSQLStatement::parse(Tokenizer& aTokenizer)
    {
        if (theTokenizer.more())
            if (theTokenizer.current().data != ";")
                return StatusResult(Errors::syntaxError);
        return StatusResult();
    }
    StatusResult ShowSQLStatement::run(std::ostream& aStream)
    {
        if (!theDatabase)
            return StatusResult{ unknownDatabase };
        return theDatabase->showTables();
    }

    StatusResult InsertSQLStatement::parseRows(Tokenizer& aTokenizer)
    {
        //insert into tasks(id, title, status) values(9, 'foo', true), (12, 'bar', true), (16, 'xd', false)
        std::vector<std::string> keys;
        //std::vector<std::string> vals;
        std::vector<ValueType> vals;
        std::map<std::string, DataType> typeCollection;
        bool isVal = false;

        while (theTokenizer.more())
        {
            if (theTokenizer.remaining() == 1 && theTokenizer.current().data != ")")
            {
                return StatusResult(Errors::illegalIdentifier);
            }
            std::vector<Token> temp;
            while (theTokenizer.more() && theTokenizer.current().data != ")")
            {
                if (theTokenizer.remaining() == 1)
                {
                    std::cout << "Error\n"; //TODO: Fix the last missing ")"
                }
                temp.push_back(theTokenizer.current());
                theTokenizer.next();
            }
            int j = 0;
            for (int i = 0; i < temp.size(); i++)
            {
                if (temp[i].data != "(" && temp[i].data != ")" && temp[i].data != ",")
                {
                    if (temp[i].keyword == Keywords::values_kw)
                        isVal = true;
                    if (!isVal)
                    {
                        std::vector<Attribute> theAttributes = theSchema->getAttributes();
                        bool found = false;
                        for (auto& x : theAttributes)
                        {
                            if (x.getName() == temp[i].data)
                            {
                                keys.push_back(temp[i].data);
                                typeCollection[temp[i].data] = x.getType();
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                        {
                            return StatusResult(Errors::unknownAttribute);
                        }
                    }
                    else
                    {
                        if (temp[i].keyword != Keywords::values_kw)
                        {
                            if (typeCollection[keys[j]] == DataType::bool_type)
                                vals.push_back(s_tob(temp[i].data));
                            else if (typeCollection[keys[j]] == DataType::int_type)
                                vals.push_back(stoi(temp[i].data));
                            else if (typeCollection[keys[j]] == DataType::float_type)
                                vals.push_back(stof(temp[i].data));
                            else
                                vals.push_back(temp[i].data);
                            //j = j % keys.size() + 1;
                            j += 1;
                        }
                    }
                }
            }
            temp.clear();
            theTokenizer.next();
        }
        if (keys.size() == 0)
            return StatusResult(Errors::keyExpected);
        if (vals.size() % keys.size() != 0) // Size Mismatch
            return StatusResult(Errors::keyValueMismatch);

        int i = 0;
        while (i < vals.size())
        {
            Row* theRow = new Row();
            for (int j = 0; j < keys.size(); j++)
            {
                theRow->addKV(keys[i % keys.size()], vals[i]);
                i++;
            }
            rows.push_back(theRow);
        }

        return StatusResult();
    }

    StatusResult InsertSQLStatement::parse(Tokenizer& aTokenizer)
    {
        tableName = theTokenizer.current().data;
        Database* theDB = theDatabase;
        if (!theDB)
            return StatusResult(Errors::noDatabaseSpecified);
        if (!isValidSQLName(tableName))
            return StatusResult(ECE141::Errors::illegalIdentifier);
        std::map<std::string, Schema*> AllSchemas = theDB->getSchemas();
        if (!AllSchemas.count(tableName))
            return StatusResult(Errors::unknownTable);
        theSchema = AllSchemas[tableName];
        theTokenizer.next();
        return parseRows(theTokenizer);
    }
    StatusResult InsertSQLStatement::run(std::ostream& aStream)
    {
        std::vector<Row*> sr = theSchema->getRows();
        for (auto& j : sr)
        {
            KeyValues r = j->getData();
        }
        int num = 1;
        for (auto& i : rows)
        {
            AttributeList theAttributeList = theSchema->getAttributes();
            std::vector<std::string> AutoIncFeature;
            std::vector<std::string> DefaultFeautre;
            std::vector<bool> DefaultValues;
            for (int n = 0; n < theAttributeList.size(); n++)
            {
                if (theAttributeList[n].isAutoInc())
                    AutoIncFeature.push_back(theAttributeList[n].getName());
                if (theAttributeList[n].isDefualt())
                {
                    DefaultFeautre.push_back(theAttributeList[n].getName());
                    DefaultValues.push_back(theAttributeList[n].getDefaultValue());
                }

            }

            KeyValues theKeyValues = i->getData();
            for (int n = 0; n < DefaultFeautre.size(); n++)
            {
                if (!theKeyValues.count(DefaultFeautre[n]))
                {
                    ValueType theValueType = DefaultValues[n];
                    i->addKV(DefaultFeautre[n], theValueType);
                }

            }
            for (int n = 0; n < AutoIncFeature.size(); n++)
            {
                ValueType theValueType = num;
                i->addKV(AutoIncFeature[n], theValueType);
            }


            theSchema->addRow(i);
            num++;
        }
        rows.clear();
        return theDatabase->insertTable(tableName);
    }

    StatusResult DeleteSQLStatement::parse(Tokenizer& aTokenizer)
    {
        if (!theDatabase)
            return StatusResult(Errors::noDatabaseSpecified);
        std::map<std::string, Schema*> AllSchemas = theDatabase->getSchemas();
        bool found = false;
        for (auto& s : AllSchemas)
        {
            for (auto a : s.second->getAttributes())
            {
                AllAttriname.push_back(a.getName());
            }
        }
        tableName = theTokenizer.current().data;
        theTokenizer.next();
        deleteall = true;
        if (theTokenizer.more() && theTokenizer.current().keyword != Keywords::where_kw)
            return StatusResult(unknownCommand);
        StatusResult theStatusResult(noError);
        while (theTokenizer.more())
        {
            if (theTokenizer.more() && theTokenizer.current().keyword == Keywords::where_kw)
            {
                deleteall = false;
                theTokenizer.next();
                std::string key = theTokenizer.current().data;
                bool found = false;
                for (auto i : AllAttriname)
                {
                    if (i == key)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return StatusResult(Errors::unknownAttribute);
                theTokenizer.next();
                std::string theOperator = theTokenizer.current().data;
                theTokenizer.next();
                std::string Value = theTokenizer.current().data;
                theTokenizer.next();
                Keywords theLogic = Keywords::null_kw;
                theStatusResult = theDatabase->SelectWhere(RowCollection, key, theOperator, Value, tableName, theLogic);
                if (theStatusResult.code != Errors::noError)
                    return theStatusResult;
            }
            // For where's logic
            if (theTokenizer.more() && (theTokenizer.current().keyword == Keywords::and_kw || theTokenizer.current().keyword == Keywords::or_kw))
            {
                Keywords theLogic = theTokenizer.current().keyword;
                theTokenizer.next();
                std::string key = theTokenizer.current().data;
                bool found = false;
                for (auto i : AllAttriname)
                {
                    if (i == key)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return StatusResult(Errors::unknownAttribute);
                theTokenizer.next();
                std::string theOperator = theTokenizer.current().data;
                theTokenizer.next();
                std::string Value = theTokenizer.current().data;
                theTokenizer.next();
                theStatusResult = theDatabase->SelectWhere(RowCollection, key, theOperator, Value, tableName, theLogic);
                if (theStatusResult.code != Errors::noError)
                    return theStatusResult;
            }
        }
        return theStatusResult;
    }
    StatusResult DeleteSQLStatement::run(std::ostream& aStream)
    {
        if (!theDatabase)
            return StatusResult{ unknownDatabase };
        Timer theTimer; theTimer.start();
        StatusResult theResult = theDatabase->deleteRow(tableName, RowCollection, deleteall);
        theTimer.stop();
        theResult.elapsedTime = theTimer.elapsed();
        return theResult;
    }
    StatusResult parseTableField(Tokenizer& theTokenizer, std::string& field)
    {
        theTokenizer.next(2);
        field = theTokenizer.current().data;
        if (theTokenizer.more())
            theTokenizer.next();
        return StatusResult();
    }
    StatusResult SelectSQLStatement::parse(Tokenizer& aTokenizer)
    {
        // check the database
        if (!theDatabase)
            return StatusResult(Errors::noDatabaseSpecified);
        // get all Attriname
        std::map<std::string, Schema*> AllSchemas = theDatabase->getSchemas();
        std::vector<std::string> AllAttriname;
        bool found = false;
        for (auto& s : AllSchemas)
        {
            for (auto a : s.second->getAttributes())
            {
                AllAttriname.push_back(a.getName());
            }
        }

        while (theTokenizer.current().keyword != Keywords::from_kw)
        {
            std::string curr = theTokenizer.current().data;
            if (curr != ",")
            {
                for (auto i : AllAttriname)
                {
                    if (i == curr || curr == "*")
                    {
                        theSelectPhase.setcolumn(curr);
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return StatusResult(Errors::unknownAttribute);
            }
            theTokenizer.next();
        }
        theTokenizer.next();
        std::string tableName = theTokenizer.current().data;
        if (!AllSchemas.count(tableName))
            return StatusResult(Errors::unknownTable);
        theSelectPhase.setTableName(tableName);
        theTokenizer.next();

        while (theTokenizer.more())
        {
            if (theTokenizer.more() && (theTokenizer.current().data == "limit" || theTokenizer.current().data == "LIMIT"))
            {
                theTokenizer.next();
                std::string value = theTokenizer.current().data;
                int newvalue = stoi(value);
                theSelectPhase.setLimited(newvalue);
                theTokenizer.next();
            }

            if (theTokenizer.more() && theTokenizer.current().keyword == Keywords::order_kw && theTokenizer.peek(1).keyword == Keywords::by_kw)
            {
                theTokenizer.next(2);
                std::string orderName = theTokenizer.current().data;
                theTokenizer.next();
                theSelectPhase.setOrder(orderName);

            }

            if (theTokenizer.more() && theTokenizer.current().keyword == Keywords::where_kw)
            {
                theTokenizer.next();
                std::string key = theTokenizer.current().data;
                bool found = false;
                auto it = std::find(AllAttriname.begin(), AllAttriname.end(), key);
                if (it == AllAttriname.end())
                {
                    return StatusResult(Errors::unknownAttribute);
                }
                found = true;
                theTokenizer.next();
                std::string theOperatorStr = theTokenizer.current().data;
                Keywords theOperator = theTokenizer.current().keyword;
                theTokenizer.next();
                std::string Value = theTokenizer.current().data;
                theTokenizer.next();
                std::vector<condEqution> thewhereValues;
                condEqution whereValue;
                whereValue.leftOpt = key;
                whereValue.Opt = theOperatorStr;
                whereValue.rightOpt = Value; //string value; please convert to number if you want
                theSelectPhase.setWhere(whereValue);
                thewhereValues.push_back(whereValue);
                std::vector<Keywords> theLogics;
                while (theTokenizer.more() && (theTokenizer.current().keyword == Keywords::and_kw || theTokenizer.current().keyword == Keywords::or_kw))
                {
                    Keywords theLogic = theTokenizer.current().keyword;
                    theLogics.push_back(theLogic);
                    theTokenizer.next();
                    std::string key = theTokenizer.current().data;
                    bool found = false;
                    auto it = std::find(AllAttriname.begin(), AllAttriname.end(), key);
                    if (it == AllAttriname.end())
                    {
                        return StatusResult(Errors::unknownAttribute);
                    }
                    found = true;
                    theTokenizer.next();
                    std::string theOperatorStr = theTokenizer.current().data;
                    Keywords theOperator = theTokenizer.current().keyword;
                    theTokenizer.next();
                    std::string Value = theTokenizer.current().data;
                    theTokenizer.next();
                    // std::vector<condEqution> thewhereValues;
                    condEqution whereValue;
                    whereValue.leftOpt = key;
                    whereValue.Opt = theOperatorStr;
                    whereValue.rightOpt = Value; //string value; please convert to number if you want
                    thewhereValues.push_back(whereValue);
                }
                totalEquations thetotalEquations;
                thetotalEquations.condEqutions = thewhereValues;
                thetotalEquations.theLogics = theLogics;
                theSelectPhase.setWhere(thetotalEquations);
            }


            
            if (theTokenizer.more() && (theTokenizer.current().keyword == Keywords::left_kw || theTokenizer.current().keyword == Keywords::right_kw) && theTokenizer.peek(1).keyword == Keywords::join_kw)
            {
                Keywords theJoinType = theTokenizer.current().keyword; // Left or right
                theTokenizer.next();
                if (theTokenizer.skipIf(Keywords::join_kw))
                {
                    std::string theTable = theTokenizer.current().data;
                    theTokenizer.next();
                    Join theJoin(theTable, theJoinType, std::string(""), std::string(""));
                    if (theTokenizer.skipIf(Keywords::on_kw))
                    { //LHS field = RHS field
                        if ((parseTableField(theTokenizer, theJoin.onLeft)))
                        {
                            theTokenizer.next();
                            if ((parseTableField(theTokenizer, theJoin.onRight)))
                            {
                                theSelectPhase.setJoin(true, theJoin);
                            }
                        }
                    }
                }
            }
            
            
        }
        return StatusResult{};



    }
    StatusResult SelectSQLStatement::run(std::ostream& aStream)
    {
        // database-> selectTable(PhaseHandler)
        StatusResult theStatusResult(Errors::noError);
        std::vector<Row*> RowCollection;
        theDatabase->selectTable(theSelectPhase, RowCollection);


        TableView theTableView;
        theTableView.elaspedTime = theStatusResult.elapsedTime;
        theTableView.setcolumns(theSelectPhase.getcolumns());
        theTableView.setRowCollection(RowCollection);
        theTableView.show(aStream);
        return theStatusResult;
    }

    StatusResult UpdateSQLStatement::parse(Tokenizer& aTokenizer)
    {

        if (!theDatabase)
            return StatusResult(Errors::noDatabaseSpecified);
        std::map<std::string, Schema*> AllSchemas = theDatabase->getSchemas();
        bool found = false;
        for (auto& s : AllSchemas)
        {
            for (auto a : s.second->getAttributes())
            {
                AllAttriname.push_back(a.getName());
            }
        }
        tableName = theTokenizer.current().data;
        theTokenizer.next();
        if (theTokenizer.current().keyword != Keywords::set_kw)
            return StatusResult{ Errors::illegalIdentifier };
        theTokenizer.next();
        feature = theTokenizer.current().data;
        theTokenizer.next();
        if (theTokenizer.current().data != "=")
            return StatusResult{ Errors::illegalIdentifier };
        theTokenizer.next();
        feature_value = theTokenizer.current().data;
        theTokenizer.next();
        return StatusResult{};
    }
    StatusResult UpdateSQLStatement::run(std::ostream& aStream)
    {
        StatusResult theStatusResult(Errors::noError);
        Timer theTimer;
        while (theTokenizer.more())
        {
            if (theTokenizer.more() && theTokenizer.current().keyword == Keywords::where_kw)
            {
                theTokenizer.next();
                std::string key = theTokenizer.current().data;
                bool found = false;
                for (auto i : AllAttriname)
                {
                    if (i == key)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return StatusResult(Errors::unknownAttribute);
                theTokenizer.next();
                std::string theOperator = theTokenizer.current().data;
                theTokenizer.next();
                std::string Value = theTokenizer.current().data;
                theTokenizer.next();
                Keywords theLogic = Keywords::null_kw;
                // For more logic incoming
                //if (theTokenizer.more() && (theTokenizer.current().keyword == Keywords::and_kw || theTokenizer.current().keyword == Keywords::or_kw))
                //    theLogic = theTokenizer.current().keyword;

                theStatusResult = theDatabase->SelectWhere(RowCollection, key, theOperator, Value, tableName, theLogic);

                if (theStatusResult.code != Errors::noError)
                    return theStatusResult;
            }
            // For where's logic
            if (theTokenizer.more() && (theTokenizer.current().keyword == Keywords::and_kw || theTokenizer.current().keyword == Keywords::or_kw))
            {
                Keywords theLogic = theTokenizer.current().keyword;
                theTokenizer.next();
                std::string key = theTokenizer.current().data;
                bool found = false;
                for (auto i : AllAttriname)
                {
                    if (i == key)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return StatusResult(Errors::unknownAttribute);
                theTokenizer.next();
                std::string theOperator = theTokenizer.current().data;
                theTokenizer.next();
                std::string Value = theTokenizer.current().data;
                theTokenizer.next();
                theStatusResult = theDatabase->SelectWhere(RowCollection, key, theOperator, Value, tableName, theLogic);

                if (theStatusResult.code != Errors::noError)
                    return theStatusResult;
            }
        }
        if (!theDatabase)
            return StatusResult{ unknownDatabase };
        theDatabase->UpdateRow(RowCollection, tableName, feature, feature_value);
        TableView theTableView;
        theTableView.elaspedTime = theStatusResult.elapsedTime;
        std::vector<std::string> columns;
        columns.push_back(feature);
        theTableView.setcolumns(columns);
        theTableView.setRowCollection(RowCollection);
        theTableView.show(aStream);
        return theStatusResult;
    }

    StatusResult ShowIndexStatement::parse(Tokenizer& aTokenizer)
    {
        return StatusResult{};
    }
    StatusResult ShowIndexStatement::run(std::ostream& aStream) {
        if (!theDatabase)
            return StatusResult{ unknownDatabase };
        theDatabase->showIndexes();
        return StatusResult();
    }



}