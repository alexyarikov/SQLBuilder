#include "stdafx.h"
#include "SQLBuilder.h"

int main()
{
	using namespace SQLBuilderNS;

	SQLCondition sc("field3", IS_NOT_NULL);
	sc.OR("field4", LIKE, "%pattern%");

	// SELECT
	SQLBuilder sb;
	sb.SELECT("field1", "field2", "field2").
		DISTINCT().
		FROM("table1").
		INNER_JOIN("table1", "field1", EQ, "table2", "field1").
		OUTER_JOIN("table3", "field1", MORE_EQ, "table3", "field1").
		ORDER_BY({{"field2", ASC}, {"field3", DESC}}).
		LIMIT(128).
		WHERE("field1", EQ, "some text").
		AND(sc).
		AND("field2", MORE, 8);

	std::string strSQL = sb.GetSQL();

	// INSERT
	sb.Clear();

	sb.INSERT("table1").
		FIELDS("field1", "field2", "field2").
		VALUES(123, 123.2, "12");

	// UPDATE
	sb.Clear();

	sb.UPDATE("table1").
		SET("field1", 123).
		SET("field2", 123.1).
		SET("field3", "a123").
		WHERE("field1", EQ, "some text").
		AND(sc).
		AND("field2", MORE, 8);

	// DELETE
	sb.Clear();

	sb.DELETE("table1").
		WHERE("field1", EQ, "some text").
		AND(sc).
		AND("field2", MORE, 8);

	return 0;
}
