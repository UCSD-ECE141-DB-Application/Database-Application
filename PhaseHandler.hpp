#ifndef PhaseHandler_hpp
#define PhaseHandler_hpp
#include <sstream>  
#include "Errors.hpp"
#include <vector>
#include "Tokenizer.hpp"
#include "Row.hpp"
#include <utility>
#include <functional>
#include "keywords.hpp"

namespace ECE141 {
	class Join
	{
	public:
		Join() {}
		Join(const std::string& aTable, Keywords aType, const std::string& aLHS, const std::string& aRHS)
			: table(aTable), joinType(aType), onLeft(aLHS), onRight(aRHS) {}
		Join(const Join& aJoin)
		{
			joinType = aJoin.joinType;
			table = aJoin.table;
			onLeft = aJoin.onLeft;
			onRight = aJoin.onRight;
		}
		~Join() {}

		Keywords joinType;
		std::string table;
		std::string onLeft;
		std::string onRight;
	};
	enum conditionVariable { where, limit, order_by };
	struct cdOperator {
		conditionVariable index;
	};
	struct WhereOperator : cdOperator
	{
		//auto p1 = std::make_pair(n, a[1]);
		std::vector<std::pair<std::string, std::string>>conditions;
		std::vector<Keywords> operators;
	};

	class UpdatePhaseHandler {
	public:
		UpdatePhaseHandler(std::vector<Token>& aTokenVector) {
			TokenVector = aTokenVector;
		}
		StatusResult parseAttribute();
	protected:
		std::vector<Token> TokenVector;
		std::string tableName;
		std::pair<std::string, std::string> updatePair;
		WhereOperator theoperator;
		std::vector<std::string> AllAttriname;
	};
	struct condEqution {
		std::string leftOpt;
		std::string Opt;
		ValueType rightOpt;
	};
	struct totalEquations {
		std::vector<condEqution> condEqutions;
		std::vector<Keywords> theLogics;
	};

	class SelectPhaseHandler {
	public:
		bool isOrder() { return isOrderby; }
		void setOrder(std::string theOrderValue) { isOrderby = true; OrderValue = theOrderValue; }
		std::string getOrderValue() { return OrderValue; }

		bool isLimited() { return isLimit; }
		void setLimited(uint32_t theLimitValue) { isLimit = true; LimitValue = theLimitValue; }
		uint32_t getLimitedValue() { return LimitValue; }

		bool isWhere() { return isWhereOpt; }
		void setWhere(condEqution thewhereValue) { isWhereOpt = true; whereValue = thewhereValue; }
		condEqution getWhereValue() { return whereValue; }
		void setWhere(totalEquations thewhereValue) { isWhereOpt = true; whereValues = thewhereValue; }
		totalEquations getWhereValues() { return whereValues; }

		bool isJoin() { return isJoinOn; }
		void setJoin(bool theisJoinOn,Join& theJoinValue) 
		{ 
			isJoinOn = theisJoinOn; 
			JoinValue = theJoinValue; 
		}
		Join& getJoin() { return JoinValue; }

		void setcolumn(std::string column) { columns.push_back(column); }
		std::vector<std::string> getcolumns() { return columns; }

		void setTableName(std::string thetableName) { tableName = thetableName; }
		std::string getTableName() { return tableName; }

		StatusResult parseAttribute(Tokenizer& theTokenizer);

	protected:
		// judge variable
		bool isOrderby = false;
		bool isLimit=false ;
		bool isWhereOpt = false;
		bool isJoinOn = false;

		// value
		std::string OrderValue;
		uint32_t LimitValue;
		//std::vector<condEqution> whereValues;
		condEqution whereValue;
		totalEquations whereValues;
		Join JoinValue;

		// 
		std::vector<std::string> columns;
		std::string tableName;
	};
}
#endif