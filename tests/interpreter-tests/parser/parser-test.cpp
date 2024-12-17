#include "pch.h"

#include <any> // Testing suite uses C++ 17 for this
#include <map>
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

		// Test integer literal and value
		testIntegerLiteral(declareIntegerStatement->m_value, tests[i].expectedValue, i);
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

		// Test float literal and value
		testFloatLiteral(declareFloatStatement->m_value, tests[i].expectedValue, i);
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

		// Test boolean literal and value
		testBooleanLiteral(declareBooleanStatement->m_value, tests[i].expectedValue, i);
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

		// Test character literal and value
		testCharacterLiteral(declareCharacterStatement->m_value, tests[i].expectedValue, i);
	}
}


TEST(ParserTest, ReturnStatements)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"return 5;", 5},
		{"return 3.40f;", 3.40f},
		{"return true;", true},
		{"return 'a';", 'a'},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << '\n';

		ASSERT_EQ(statement->TokenLiteral(), "return")
			<< "Test #" << i << '\n';

		// Test to see if this is a return statement
		ASSERT_EQ(statement->NodeType(), "ReturnStatement");
		ast::ReturnStatement* returnStatement = (ast::ReturnStatement*)statement;

		// Test expression separately
		testLiteralExpression(returnStatement->m_returnValue, tests[i].expectedValue, i);
	}
}


TEST(ParserTest, Identifiers)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedIdentifer;
	} TestCase;

	TestCase tests[] =
	{
		{"someIdentifier;", "someIdentifier"},
		{"some_identifier;", "some_identifier"},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << '\n';

		// Test to see if this is an expression statement
		ASSERT_EQ(statement->NodeType(), "ExpressionStatement");
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		// Test to see if this is an identifier
		ASSERT_EQ(expressionStatement->m_expression->NodeType(), "Identifier");
		ast::Identifier* identifier = (ast::Identifier*)expressionStatement->m_expression;

		// Test to see if identifier fields are right
		EXPECT_EQ(identifier->m_name, tests[i].expectedIdentifer)
			<< "Test #" << i << '\n';
		EXPECT_EQ(identifier->TokenLiteral(), tests[i].expectedIdentifer)
			<< "Test #" << i << '\n';
	}
}

void testLiteralExpression(ast::Expression* expression, std::any expectedValue, int testNumber)
{
	// Cannot use a switch on type as std::string is not integral
	// Minor efficiency tradeoff, but this is just a test suite, not the actual interpreter

	std::string expectedValueType = expectedValue.type().name();
	if(expectedValueType == "int")
	{
		testIntegerLiteral(expression, std::any_cast<int>(expectedValue), testNumber);
	}
	else if (expectedValueType == "float")
	{
		testFloatLiteral(expression, std::any_cast<float>(expectedValue), testNumber);
	}
	else if (expectedValueType == "bool")
	{
		testBooleanLiteral(expression, std::any_cast<bool>(expectedValue), testNumber);
	}
	else if (expectedValueType == "char")
	{
		testCharacterLiteral(expression, std::any_cast<char>(expectedValue), testNumber);
	}
	else
	{
		FAIL()
			<< "Test #" << testNumber << '\n';
	}
}

void testIntegerLiteral(ast::Expression* expression, int expectedValue, int testNumber)
{
	ASSERT_EQ(expression->NodeType(), "IntegerLiteral")
		<< "Test #" << testNumber << '\n';

	ast::IntegerLiteral* integerLiteral = (ast::IntegerLiteral*)expression;

	EXPECT_EQ(integerLiteral->m_value, expectedValue)
		<< "Test #" << testNumber << '\n';
	EXPECT_EQ(integerLiteral->TokenLiteral(), std::to_string(expectedValue))
		<< "Test #" << testNumber << '\n';
}

void testFloatLiteral(ast::Expression* expression, float expectedValue, int testNumber)
{
	ASSERT_EQ(expression->NodeType(), "FloatLiteral")
		<< "Test #" << testNumber << '\n';

	ast::FloatLiteral* floatLiteral = (ast::FloatLiteral*)expression;

	std::ostringstream outputString;
	outputString << expectedValue;

	EXPECT_EQ(floatLiteral->m_value, expectedValue)
		<< "Test #" << testNumber << '\n';
	EXPECT_EQ(floatLiteral->TokenLiteral(), outputString.str())
		<< "Test #" << testNumber << '\n';
}

void testBooleanLiteral(ast::Expression* expression, bool expectedValue, int testNumber)
{
	ASSERT_EQ(expression->NodeType(), "BooleanLiteral")
		<< "Test #" << testNumber << '\n';

	ast::BooleanLiteral* booleanLiteral = (ast::BooleanLiteral*)expression;

	EXPECT_EQ(booleanLiteral->m_value, expectedValue)
		<< "Test #" << testNumber << '\n';
	EXPECT_EQ(booleanLiteral->TokenLiteral(), expectedValue ? "true" : "false")
		<< "Test #" << testNumber << '\n';
}

void testCharacterLiteral(ast::Expression* expression, char expectedValue, int testNumber)
{
	ASSERT_EQ(expression->NodeType(), "CharacterLiteral")
		<< "Test #" << testNumber << '\n';

	ast::CharacterLiteral* characterLiteral = (ast::CharacterLiteral*)expression;

	char charToString[2] = { expectedValue, '\0' };

	EXPECT_EQ(characterLiteral->m_value, expectedValue)
		<< "Test #" << testNumber << '\n';
	EXPECT_EQ(characterLiteral->TokenLiteral(), charToString)
		<< "Test #" << testNumber << '\n';
}