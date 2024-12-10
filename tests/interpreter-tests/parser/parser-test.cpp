#include "pch.h"

#include "parser-test.h"
#include "parser.h"


TEST(ParserTest, DeclaringIntegerStatement)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedIdentifier;
		int expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer a = 1;", "a", 1},
		{"integer b = 2;", "b", 2},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << '\n';

		ASSERT_EQ(statement->TokenLiteral(), "integer")
			<< "Test #" << i << '\n';

		// Test declaration identifier literal and name
		ASSERT_EQ(statement->NodeType(), "DeclareIntegerStatement");
		ast::DeclareIntegerStatement* declareIntegerStatement = (ast::DeclareIntegerStatement*)statement;

		EXPECT_EQ(declareIntegerStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << '\n';
		EXPECT_EQ(declareIntegerStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << '\n';

		// Test declaration integer literal and value
		ASSERT_EQ(declareIntegerStatement->m_value->NodeType(), "IntegerLiteral");
		ast::IntegerLiteral* integerLiteral = (ast::IntegerLiteral*)(declareIntegerStatement->m_value);

		EXPECT_EQ(integerLiteral->m_value, tests[i].expectedValue)
			<< "Test #" << i << '\n';
		EXPECT_EQ(integerLiteral->TokenLiteral(), std::to_string(tests[i].expectedValue))
			<< "Test #" << i << '\n';
	}
}