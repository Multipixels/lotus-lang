#include "pch.h"

#include <sstream>

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

TEST(ParserTest, DeclaringFloatStatement)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedIdentifier;
		float expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"float a = 1f;", "a", 1.0f},
		{"float b = 200f;", "b", 200.0f},
		{"float b = 20.200f;", "b", 20.2f},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << '\n';

		ASSERT_EQ(statement->TokenLiteral(), "float")
			<< "Test #" << i << '\n';

		// Test declaration identifier literal and name
		ASSERT_EQ(statement->NodeType(), "DeclareFloatStatement");
		ast::DeclareFloatStatement* declareFloatStatement = (ast::DeclareFloatStatement*)statement;

		EXPECT_EQ(declareFloatStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << '\n';
		EXPECT_EQ(declareFloatStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << '\n';

		// Test declaration integer literal and value
		ASSERT_EQ(declareFloatStatement->m_value->NodeType(), "FloatLiteral");
		ast::FloatLiteral* floatLiteral = (ast::FloatLiteral*)(declareFloatStatement->m_value);

		std::ostringstream outputString;
		outputString << tests[i].expectedValue;

		EXPECT_EQ(floatLiteral->m_value, tests[i].expectedValue)
			<< "Test #" << i << '\n';
		EXPECT_EQ(floatLiteral->TokenLiteral(), outputString.str())
			<< "Test #" << i << '\n';
	}
}

TEST(ParserTest, DeclaringBooleanStatement)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedIdentifier;
		bool expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"boolean a = true;", "a", true},
		{"boolean b = false;", "b", false},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << '\n';

		ASSERT_EQ(statement->TokenLiteral(), "boolean")
			<< "Test #" << i << '\n';

		// Test declaration identifier literal and name
		ASSERT_EQ(statement->NodeType(), "DeclareBooleanStatement");
		ast::DeclareBooleanStatement* declareBooleanStatement = (ast::DeclareBooleanStatement*)statement;

		EXPECT_EQ(declareBooleanStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << '\n';
		EXPECT_EQ(declareBooleanStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << '\n';

		// Test declaration integer literal and value
		ASSERT_EQ(declareBooleanStatement->m_value->NodeType(), "BooleanLiteral");
		ast::BooleanLiteral* booleanLiteral = (ast::BooleanLiteral*)(declareBooleanStatement->m_value);

		EXPECT_EQ(booleanLiteral->m_value, tests[i].expectedValue)
			<< "Test #" << i << '\n';
		EXPECT_EQ(booleanLiteral->TokenLiteral(), tests[i].expectedValue ? "true" : "false")
			<< "Test #" << i << '\n';
	}
}

TEST(ParserTest, DeclaringCharacterStatement)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedIdentifier;
		char expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"character a = 'b';", "a", 'b'},
		{"character c = 'c';", "c", 'c'},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << '\n';

		ASSERT_EQ(statement->TokenLiteral(), "character")
			<< "Test #" << i << '\n';

		// Test declaration identifier literal and name
		ASSERT_EQ(statement->NodeType(), "DeclareCharacterStatement");
		ast::DeclareCharacterStatement* declareCharacterStatement = (ast::DeclareCharacterStatement*)statement;

		EXPECT_EQ(declareCharacterStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << '\n';
		EXPECT_EQ(declareCharacterStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << '\n';

		// Test declaration integer literal and value
		ASSERT_EQ(declareCharacterStatement->m_value->NodeType(), "CharacterLiteral");
		ast::CharacterLiteral* characterLiteral = (ast::CharacterLiteral*)(declareCharacterStatement->m_value);

		EXPECT_EQ(characterLiteral->m_value, tests[i].expectedValue)
			<< "Test #" << i << '\n';

		char charToString[2] = { tests[i].expectedValue, '\0' };
		EXPECT_EQ(characterLiteral->TokenLiteral(), charToString)
			<< "Test #" << i << '\n';
	}
}