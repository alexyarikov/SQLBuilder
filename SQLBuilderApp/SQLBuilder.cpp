#include "stdafx.h"
#include "SQLBuilder.h"

std::string to_string(const std::string& s) { return "\"" + s + "\""; }

namespace SQLBuilderNS
{
///////////////////////////////////////// SQL CONDITION /////////////////////////////////////////
	SQLCondition::SQLCondition(const std::string& strLeftSide, const ConditionOperator enOperator) :
		m_strLeftSide(strLeftSide),
		m_enOperator(enOperator),
		m_strRightSide("")
	{
	}

	SQLCondition::SQLCondition(const std::string& strLeftSide,
										const ConditionOperator enOperator,
										const std::string& strRightSide) :
		m_strLeftSide(strLeftSide),
		m_enOperator(enOperator),
		m_strRightSide(strRightSide)
	{
	}

	SQLCondition::SQLCondition(const std::string& strLeftSide, const ConditionOperator enOperator, const int& iRightSide = 0) :
		m_strLeftSide(strLeftSide),
		m_enOperator(enOperator)
	{
		m_strRightSide = std::to_string(iRightSide);
	}

	SQLCondition::SQLCondition(const std::string& strLeftSide, const ConditionOperator enOperator, const int64_t& iRightSide = 0) :
		m_strLeftSide(strLeftSide),
		m_enOperator(enOperator)
	{
		m_strRightSide = std::to_string(iRightSide);
	}

	SQLCondition::~SQLCondition()
	{
	}

	void SQLCondition::Clear()
	{
		m_enLink = ConditionLink::clUnknown;
		m_enOperator = ConditionOperator::UNKNOWN;
		m_strLeftSide = m_strRightSide = "";
		m_ptrSibling = nullptr;
		m_ptrChild = nullptr;
	}

	SQLCondition& SQLCondition::AND(const SQLCondition& scSubCondition)
	{
		m_ptrSibling = std::make_shared<SQLCondition>("", ConditionOperator::UNKNOWN, "");
		m_ptrSibling->m_enLink = ConditionLink::clAnd;
		m_ptrChild = std::make_shared<SQLCondition>(scSubCondition.m_strLeftSide, scSubCondition.m_enOperator, scSubCondition.m_strRightSide);
		return *m_ptrSibling;
	}

	SQLCondition& SQLCondition::OR(const SQLCondition& scSubCondition)
	{
		m_ptrSibling = std::make_shared<SQLCondition>("", ConditionOperator::UNKNOWN, "");
		m_ptrSibling->m_enLink = ConditionLink::clOr;
		m_ptrChild = std::make_shared<SQLCondition>(scSubCondition.m_strLeftSide, scSubCondition.m_enOperator, scSubCondition.m_strRightSide);
		return *m_ptrSibling;
	}
///////////////////////////////////////// SQL BUILDER /////////////////////////////////////////
	SQLBuilder::SQLBuilder() :
		m_scWhere("", ConditionOperator::UNKNOWN, "")
	{
	}

	SQLBuilder::~SQLBuilder()
	{
	}

	void SQLBuilder::Clear()
	{
		m_vTables.clear();
		m_vFields.clear();
		m_vSortedFields.clear();
		m_vJoins.clear();
		m_scWhere.Clear();
		m_vValues.clear();
	}

	SQLBuilder& SQLBuilder::DISTINCT()
	{
		m_bDistinct = true;
		return *this;
	}

	SQLBuilder& SQLBuilder::INNER_JOIN(const std::string& strTable1,
		const std::string& strField1,
		const ConditionOperator enOperator,
		const std::string& strTable2,
		const std::string& strField2)
	{
		m_vJoins.push_back({ JoinType::jtInner, enOperator, strTable1, strField1, strTable2, strField2 });
		return *this;
	}

	SQLBuilder& SQLBuilder::OUTER_JOIN(const std::string& strTable1,
		const std::string& strField1,
		const ConditionOperator enOperator,
		const std::string& strTable2,
		const std::string& strField2)
	{
		m_vJoins.push_back({ JoinType::jtOuter, enOperator,  strTable1, strField1, strTable2, strField2 });
		return *this;
	}

	SQLBuilder& SQLBuilder::ORDER_BY(const std::initializer_list<SortedField>& ilSortedFields)
	{
		m_vSortedFields = ilSortedFields;
		return *this;
	}

	SQLBuilder& SQLBuilder::LIMIT(const uint32_t iValue)
	{
		m_iLimit = iValue;
		return *this;
	}

	SQLCondition& SQLBuilder::WHERE(const std::string& strLeftSide, const ConditionOperator enOperator, const std::string& m_strRightSide)
	{
		m_scWhere = SQLCondition(strLeftSide, enOperator, m_strRightSide);
		return m_scWhere;
	}

	SQLBuilder& SQLBuilder::INSERT(const std::string& strTable)
	{
		m_enQueryType = QueryType::qtInsert;
		m_vTables.push_back(strTable);
		return *this;
	}

	SQLBuilder& SQLBuilder::UPDATE(const std::string& strTable)
	{
		m_enQueryType = QueryType::qtUpdate;
		m_vTables.push_back(strTable);
		return *this;
	}

	SQLBuilder& SQLBuilder::DELETE(const std::string& strTable)
	{
		m_enQueryType = QueryType::qtDelete;
		m_vTables.push_back(strTable);
		return *this;
	}

	std::string SQLBuilder::GetSQL() const
	{
		return "";
	}
}
