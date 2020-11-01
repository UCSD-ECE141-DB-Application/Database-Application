//
//  CommandProcessor.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/18.
//  Copyright © 2018 rick gessner. All rights reserved.
//

#include <iostream>
#include "AppProcessor.hpp"
#include "Tokenizer.hpp"
#include <memory>
#include <vector>

namespace ECE141 {
  
  class VersionStatement : public Statement {
  public:
    VersionStatement() :  Statement(Keywords::version_kw) {}
    virtual const char* getStatementName() {return "Version Statement";}
    virtual StatusResult run(std::ostream &aStream)
    {
      aStream << "version ECE141b-1" << std::endl;
      return StatusResult();
    }
  };

  class HelpStatement: public Statement {
    public:
      std::vector<Token> tokens;

      HelpStatement() : Statement(Keywords::help_kw) {}
      void addTokens(Token tok)
      {
        tokens.push_back(tok);
      }
      virtual const char *getStatementName() { return "Help Statement"; }

      virtual StatusResult run(std::ostream &aStream)
      {
        aStream << "help -- the available list of commands shown below:\n";
        std::string help = "     -- help - shows this list of commands\n";
        std::string version = "     --version-- shows the current version of this application\n";
        std::string quit = "     --quit-- terminates the execution of this DB application\n";
        std::string create = "     --create database<name> --creates a new database\n";
        std::string drop = "     --drop database<name> --drops the given database\n";
        std::string use = "     --use database<name> --uses the given database\n";
        std::string describe = "     --describe database<name> --describes the given database\n";
        std::string show = "     -- show databases-- shows the list of databases available\n";
        if (tokens.size() == 0)
        {
          aStream << help << version << quit << create << drop << use << describe << show;
        }
        else
        {
          for (int i = 0; i < tokens.size(); i++)
          {
            if (tokens[i].type == TokenType::punctuation)
            {
              aStream << help << version << quit << create << drop << use << describe << show;
              break;
            }
            switch (tokens[i].keyword)
            {
              case Keywords::help_kw: aStream << help; break;
              case Keywords::version_kw: aStream << version; break;
              case Keywords::quit_kw: aStream << quit; break;
              case Keywords::create_kw: aStream << create; break;
              case Keywords::drop_kw: aStream << drop; break;
              case Keywords::use_kw: aStream << use; break;
              case Keywords::describe_kw: aStream << describe; break;
              case Keywords::show_kw: aStream << show; break;
              default: aStream << "Stay tuned.." << std::endl;
            }
          }
        }
        return StatusResult();
      }
  };

  class QuitStatement: public Statement {
    public:
      QuitStatement() : Statement(Keywords::quit_kw) {}
      virtual const char *getStatementName() { return "Quit Statement"; }
      virtual StatusResult run(std::ostream &aStream)
      {
        aStream << "Shutting down..." << std::endl;
        return StatusResult(Errors::userTerminated);
      }
  };

  AppCmdProcessor::AppCmdProcessor(CommandProcessor *aNext) : CommandProcessor(aNext) {
  }
  
  AppCmdProcessor::~AppCmdProcessor() {}

  StatusResult AppCmdProcessor::interpret(Statement &aStatement) {
    switch (aStatement.getType()) {
      case Keywords::help_kw:
        return aStatement.run(std::cout);
      case Keywords::version_kw:
        return aStatement.run(std::cout);
      case Keywords::quit_kw:
        return aStatement.run(std::cout);
      default:
        std::clog << "Not handling this time. Stay tuned..." << std::endl;
    }
    return StatusResult();
  }
  
  Statement* AppCmdProcessor::getStatement(Tokenizer &aTokenizer) {
      if (aTokenizer.current().keyword == Keywords::help_kw)
      {
        HelpStatement *theHelps = new HelpStatement();
        aTokenizer.next();  // Skip the first help
        while(aTokenizer.remaining() != 0)
        {
          theHelps->addTokens(aTokenizer.current());
          aTokenizer.next();
        }
        return theHelps;
      }
      if (aTokenizer.current().keyword == Keywords::quit_kw)
      {
        aTokenizer.next();
        return new QuitStatement();
      }
      if (aTokenizer.current().keyword == Keywords::version_kw)
      {
        aTokenizer.next();
        return new VersionStatement();
      }
      return nullptr;
  }
  
}
