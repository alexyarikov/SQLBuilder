#pragma once

using namespace std;

// additional std::to_string specification for std::string which might be used in SQLBuilder::VALUES template
std::string to_string(const std::string& s);

namespace SQLBuilderNS
{
	enum ConditionOperator { UNKNOWN, EQ, NOT_EQ, LESS, LESS_EQ, MORE, MORE_EQ, IS_NULL, IS_NOT_NULL, IN, NOT_IN, LIKE };
	enum SortDirection { ASC, DESC };

	class SQLCondition
	{
	public:
		SQLCondition(const std::string& strLeftSide, const ConditionOperator enOperator);
		SQLCondition(const std::string& strLeftSide, const ConditionOperator enOperator, const std::string& strRightSide);
		SQLCondition(const std::string& strLeftSide, const ConditionOperator enOperator, const int& iRightSide);
		SQLCondition(const std::string& strLeftSide, const ConditionOperator enOperator, const int64_t& iRightSide);
		~SQLCondition();

		void Clear();

		template <typename RigthSideType> SQLCondition& AND(const std::string& strLeftSide, const ConditionOperator enOperator, const RigthSideType& RightSide)
		{
			m_ptrSibling = std::make_shared<SQLCondition>(strLeftSide, enOperator, RightSide);
			m_ptrSibling->m_enLink = ConditionLink::clAnd;
			return *m_ptrSibling;
		}

		template <typename RigthSideType> SQLCondition& OR(const std::string& strLeftSide, const ConditionOperator enOperator, const RigthSideType& RightSide)
		{
			m_ptrSibling = std::make_shared<SQLCondition>(strLeftSide, enOperator, RightSide);
			m_ptrSibling->m_enLink = ConditionLink::clOr;
			return *m_ptrSibling;
		}

		SQLCondition& AND(const SQLCondition& scSubCondition);
		SQLCondition& OR(const SQLCondition& scSubCondition);

	private:
		enum ConditionLink { clUnknown, clAnd, clOr };

		ConditionLink m_enLink = ConditionLink::clUnknown;
		std::string m_strLeftSide;
		ConditionOperator m_enOperator = ConditionOperator::UNKNOWN;
		std::string m_strRightSide;
		std::shared_ptr<SQLCondition> m_ptrSibling;
		std::shared_ptr<SQLCondition> m_ptrChild;
	};

	class SQLBuilder
	{
	public:
		SQLBuilder();
		~SQLBuilder();

		typedef std::pair<std::string, SortDirection> SortedField;

		void Clear();

		template <typename Type, typename ... Types>
		SQLBuilder& SELECT(const Type& strField, const Types&... strFields)
		{
			m_enQueryType = QueryType::qtSelect;
			m_vFields = { strField, strFields... };
			return *this;
		}

		SQLBuilder& DISTINCT();

		template <typename Type, typename ... Types>
		SQLBuilder& FROM(const Type& strTable, const Types&... strTables)
		{
			m_vTables = { strTable, strTables... };
			return *this;
		}

		SQLBuilder& INNER_JOIN(const std::string& strTable1, const std::string& strField1, const ConditionOperator enCondition, const std::string& strTable2 = "", const std::string& strField2 = "");
		SQLBuilder& OUTER_JOIN(const std::string& strTable1, const std::string& strField1, const ConditionOperator enCondition, const std::string& strTable2 = "", const std::string& strField2 = "");
		SQLBuilder& ORDER_BY(const std::initializer_list<SortedField>& ilSortedFields);
		SQLBuilder& LIMIT(const uint32_t iValue);
		SQLCondition& WHERE(const std::string& strLeftSide, const ConditionOperator enOperator, const std::string& strRightSide);

		SQLBuilder& INSERT(const std::string& strTable);

		template <typename Type, typename ... Types>
		SQLBuilder& FIELDS(const Type& strField, const Types&... strFields)
		{
			m_vFields = { strField, strFields... };
			return *this;
		}

		template<typename... ValueTypes> SQLBuilder& VALUES(ValueTypes... values)
		{
			int unpack[]{ 0, (m_vValues.push_back(to_string(values)), 0)... };
			return *this;
		}

		SQLBuilder& UPDATE(const std::string& strTable);

		template<typename ValueType> SQLBuilder& SET(const std::string& strField, const ValueType& value)
		{
			m_vFields.push_back(strField);
			m_vValues.push_back(to_string(value));
			return *this;
		}

		SQLBuilder& DELETE(const std::string& strTable);

		std::string GetSQL() const;

	private:
		enum QueryType { qtSelect, qtInsert, qtUpdate, qtDelete };
		enum JoinType { jtInner, jtOuter };

		struct Join
		{
			JoinType enJoinType;
			ConditionOperator enOperator;
			std::string strTable1;
			std::string strField1;
			std::string strTable2;
			std::string strField2;
		};

		bool m_bDistinct = false;
		QueryType m_enQueryType = QueryType::qtSelect;
		std::vector<std::string> m_vTables;
		std::vector<std::string> m_vFields;
		std::vector<std::string> m_vValues;
		std::vector<SortedField> m_vSortedFields;
		std::vector<Join> m_vJoins;
		uint32_t m_iLimit = 0;
		SQLCondition m_scWhere;
	};
};
