#include "pch.h"

#include <any> // Testing suite uses C++ 17 for this
#include <map>
#include <sstream>

#include "parser-test.h"

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
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		ASSERT_EQ(statement->TokenLiteral(), "integer")
			<< "Test #" << i << std::endl;

		// Test declaration identifier literal and name
		ASSERT_EQ(statement->NodeType(), "DeclareIntegerStatement");
		ast::DeclareIntegerStatement* declareIntegerStatement = (ast::DeclareIntegerStatement*)statement;

		EXPECT_EQ(declareIntegerStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;
		EXPECT_EQ(declareIntegerStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;

		// Test integer literal and value
		ASSERT_NO_FATAL_FAILURE(testIntegerLiteral(declareIntegerStatement->m_value, tests[i].expectedValue, i));
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
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		ASSERT_EQ(statement->TokenLiteral(), "float")
			<< "Test #" << i << std::endl;

		// Test declaration identifier literal and name
		ASSERT_EQ(statement->NodeType(), "DeclareFloatStatement");
		ast::DeclareFloatStatement* declareFloatStatement = (ast::DeclareFloatStatement*)statement;

		EXPECT_EQ(declareFloatStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;
		EXPECT_EQ(declareFloatStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;

		// Test float literal and value
		ASSERT_NO_FATAL_FAILURE(testFloatLiteral(declareFloatStatement->m_value, tests[i].expectedValue, i));
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
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		ASSERT_EQ(statement->TokenLiteral(), "boolean")
			<< "Test #" << i << std::endl;

		// Test declaration identifier literal and name
		ASSERT_EQ(statement->NodeType(), "DeclareBooleanStatement");
		ast::DeclareBooleanStatement* declareBooleanStatement = (ast::DeclareBooleanStatement*)statement;

		EXPECT_EQ(declareBooleanStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;
		EXPECT_EQ(declareBooleanStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;

		// Test boolean literal and value
		ASSERT_NO_FATAL_FAILURE(testBooleanLiteral(declareBooleanStatement->m_value, tests[i].expectedValue, i));
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
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		ASSERT_EQ(statement->TokenLiteral(), "character")
			<< "Test #" << i << std::endl;

		// Test declaration identifier literal and name
		ASSERT_EQ(statement->NodeType(), "DeclareCharacterStatement");
		ast::DeclareCharacterStatement* declareCharacterStatement = (ast::DeclareCharacterStatement*)statement;

		EXPECT_EQ(declareCharacterStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;
		EXPECT_EQ(declareCharacterStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;

		// Test character literal and value
		ASSERT_NO_FATAL_FAILURE(testCharacterLiteral(declareCharacterStatement->m_value, tests[i].expectedValue, i));
	}
}


TEST(ParserTest, ReturnStatement)
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
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		ASSERT_EQ(statement->TokenLiteral(), "return")
			<< "Test #" << i << std::endl;

		// Test to see if this is a return statement
		ASSERT_EQ(statement->NodeType(), "ReturnStatement");
		ast::ReturnStatement* returnStatement = (ast::ReturnStatement*)statement;

		// Test expression separately
		ASSERT_NO_FATAL_FAILURE(testLiteralExpression(returnStatement->m_returnValue, tests[i].expectedValue, i));
	}
}


TEST(ParserTest, IdentifierExpression)
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
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		// Test to see if this is an expression statement
		ASSERT_EQ(statement->NodeType(), "ExpressionStatement");
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		ASSERT_NO_FATAL_FAILURE(testIdentifier(expressionStatement->m_expression, &tests[i].expectedIdentifer, i));
	}
}


TEST(ParserTest, IntegerLiteralExpression)
{
	typedef struct TestCase
	{
		std::string input;
		int expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"5;", 5},
		{"34;", 34},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		// Test to see if this is an expression statement
		ASSERT_EQ(statement->NodeType(), "ExpressionStatement");
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		ASSERT_NO_FATAL_FAILURE(testIntegerLiteral(expressionStatement->m_expression, tests[i].expectedValue, i));
	}
}


TEST(ParserTest, FloatLiteralExpression)
{
	typedef struct TestCase
	{
		std::string input;
		float expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"5.303f;", 5.303f},
		{"231.99f;", 231.99f},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		// Test to see if this is an expression statement
		ASSERT_EQ(statement->NodeType(), "ExpressionStatement");
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		ASSERT_NO_FATAL_FAILURE(testFloatLiteral(expressionStatement->m_expression, tests[i].expectedValue, i));
	}
}


TEST(ParserTest, BooleanLiteralExpression)
{
	typedef struct TestCase
	{
		std::string input;
		bool expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"true;", true},
		{"false;", false},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		// Test to see if this is an expression statement
		ASSERT_EQ(statement->NodeType(), "ExpressionStatement");
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		ASSERT_NO_FATAL_FAILURE(testBooleanLiteral(expressionStatement->m_expression, tests[i].expectedValue, i));
	}
}


TEST(ParserTest, CharacterLiteralExpression)
{
	typedef struct TestCase
	{
		std::string input;
		char expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"'a';", 'a'},
		{"'d';", 'd'},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		// Test to see if this is an expression statement
		ASSERT_EQ(statement->NodeType(), "ExpressionStatement");
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		ASSERT_NO_FATAL_FAILURE(testCharacterLiteral(expressionStatement->m_expression, tests[i].expectedValue, i));
	}
}


TEST(ParserTest, PrefixExpression)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedOperator;
		std::any expectedExpression;
	} TestCase;

	TestCase tests[] =
	{
		{"!true;", "!", true},
		{"!false;", "!", false},
		{"-5;", "-", 5},
		{"-5.5f;", "-", 5.5f},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		// Test to see if this is an expression statement
		ASSERT_EQ(statement->NodeType(), "ExpressionStatement");
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		ASSERT_EQ(expressionStatement->m_expression->NodeType(), "PrefixExpression");
		ast::PrefixExpression* prefixExpression = (ast::PrefixExpression*)(expressionStatement->m_expression);

		EXPECT_EQ(prefixExpression->m_operator, tests[i].expectedOperator);
		
		ASSERT_NO_FATAL_FAILURE(testLiteralExpression(prefixExpression->m_right_expression, tests[i].expectedExpression, i));
	}
}


TEST(ParserTest, InfixExpression)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedLeftExpression;
		std::string expectedOperator;
		std::any expectedRightExpression;
	} TestCase;

	TestCase tests[] =
	{
		{"5 + 6;", 5, "+", 6},
		{"5 - 6;", 5, "-", 6},
		{"5 * 6;", 5, "*", 6},
		{"5 / 6;", 5, "/", 6},
		{"5 > 6;", 5, ">", 6},
		{"5 >= 6;", 5, ">=", 6},
		{"5 < 6;", 5, "<", 6},
		{"5 <= 6;", 5, "<=", 6},
		{"5 == 6;", 5, "==", 6},
		{"5 != 6;", 5, "!=", 6},
		{"true && false;", true, "&&", false},
		{"true || false;", true, "||", false},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		ast::Statement* statement = program->m_statements[0];
		ASSERT_EQ(program->m_statements.size(), 1)
			<< "Test #" << i << std::endl;

		// Test to see if this is an expression statement
		ASSERT_EQ(statement->NodeType(), "ExpressionStatement");
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		ASSERT_EQ(expressionStatement->m_expression->NodeType(), "InfixExpression");
		ast::InfixExpression* infixExpression = (ast::InfixExpression*)(expressionStatement->m_expression);

		EXPECT_EQ(infixExpression->m_operator, tests[i].expectedOperator);

		ASSERT_NO_FATAL_FAILURE(testLiteralExpression(infixExpression->m_left_expression, tests[i].expectedLeftExpression, i));
		ASSERT_NO_FATAL_FAILURE(testLiteralExpression(infixExpression->m_right_expression, tests[i].expectedRightExpression, i));
	}
}


TEST(ParserTest, OperatorPrecedence)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expected;
	} TestCase;

	TestCase tests[] =
	{
		{"5 + 6;", "(5 + 6);"},
		{"!true;", "(!true);"},
		{"!5;", "(!5);"},
		{"!-5;", "(!(-5));"},
		{"a + b + c;", "((a + b) + c);"},
		{"a + b - c;", "((a + b) - c);"},
		{"a - b + c;", "((a - b) + c);"},
		{"a * b * c;", "((a * b) * c);"},
		{"a * b / c;", "((a * b) / c);"},
		{"a / b * c;", "((a / b) * c);"},
		{"a + b * c;", "(a + (b * c));"},
		{"a + b / c;", "(a + (b / c));"},
		{"a * b + c;", "((a * b) + c);"},
		{"a / b - c;", "((a / b) - c);"},
		{"a + b * c + d / e - f;", "(((a + (b * c)) + (d / e)) - f);"},
		{"a > b == b > c;", "((a > b) == (b > c));"},
		{"a >= b != b >= c;", "((a >= b) != (b >= c));"},
		{"3 + 4 * 6 == 3 * 1 + 4 * 6;", "((3 + (4 * 6)) == ((3 * 1) + (4 * 6)));"},
		{"a + (b + c);", "(a + (b + c));"},
		{"(5 + 6) * 7;", "((5 + 6) * 7);"},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();
		ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

		EXPECT_EQ(program->String(), tests[i].expected);
	}
}


TEST(ParserTest, Errors)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedError;
	} TestCase;

	TestCase tests[] =
	{
		{"5 + 6", "Expected SEMICOLON. Got END_OF_FILE instead."},
		{"5 + ;", "No prefix function defined for SEMICOLON."},
		{"+ 6;", "No prefix function defined for PLUS."},
		{"5 6;", "Expected SEMICOLON. Got INTEGER_LITERAL instead."},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		lexer::Lexer lexer(&tests[i].input);
		parser::Parser parser(lexer);
		ast::Program* program = parser.ParseProgram();

		EXPECT_EQ(parser.m_errors[0], tests[i].expectedError);
	}
}


TEST(ParserTest, IfStatement)
{
	std::string input = R"(
if(true) {
	integer b = 5;
	c = b + 3 * 2;
} 
)";

	std::string expectedString = 
R"(if (true)
{
integer b = 5;
(c = (b + (3 * 2)));
}
)";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is an if statement
	ASSERT_EQ(statement->NodeType(), "IfStatement");
	ast::IfStatement* ifStatement = (ast::IfStatement*)statement;

	ASSERT_EQ(ifStatement->m_consequence.size(), 1);

	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(ifStatement->m_condition[0], true, 0));
	ASSERT_EQ(ifStatement->m_consequence[0]->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_consequence[0]->m_statements[0]->NodeType(), "DeclareIntegerStatement");
	EXPECT_EQ(ifStatement->m_consequence[0]->m_statements[1]->NodeType(), "ExpressionStatement");
	EXPECT_TRUE(ifStatement->m_alternative == NULL);

	EXPECT_EQ(program->String(), expectedString);
}


TEST(ParserTest, IfElseStatement)
{
	std::string input = R"(
if(true) {
	integer b = 5;
	c = b + 3 * 2;
}
else
{
	boolean c = false;
	return c;
}
)";

	std::string expectedString =
		R"(if (true)
{
integer b = 5;
(c = (b + (3 * 2)));
}
else
{
boolean c = false;
return c;
}
)";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is an if statement
	ASSERT_EQ(statement->NodeType(), "IfStatement");
	ast::IfStatement* ifStatement = (ast::IfStatement*)statement;

	ASSERT_EQ(ifStatement->m_consequence.size(), 1);

	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(ifStatement->m_condition[0], true, 0));
	ASSERT_EQ(ifStatement->m_consequence[0]->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_consequence[0]->m_statements[0]->NodeType(), "DeclareIntegerStatement");
	EXPECT_EQ(ifStatement->m_consequence[0]->m_statements[1]->NodeType(), "ExpressionStatement");

	EXPECT_TRUE(ifStatement->m_alternative != NULL);
	ASSERT_EQ(ifStatement->m_alternative->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_alternative->m_statements[0]->NodeType(), "DeclareBooleanStatement");
	EXPECT_EQ(ifStatement->m_alternative->m_statements[1]->NodeType(), "ReturnStatement");

	EXPECT_EQ(program->String(), expectedString);
}


TEST(ParserTest, IfElseIfStatement)
{
	std::string input = R"(
if(true) {
	integer b = 5;
	c = b + 3 * 2;
}
else if(false)
{
	boolean c = true;
	return c;
}
)";

	std::string expectedString =
		R"(if (true)
{
integer b = 5;
(c = (b + (3 * 2)));
}
else if(false)
{
boolean c = true;
return c;
}
)";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is an if statement
	ASSERT_EQ(statement->NodeType(), "IfStatement");
	ast::IfStatement* ifStatement = (ast::IfStatement*)statement;

	ASSERT_EQ(ifStatement->m_consequence.size(), 2);

	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(ifStatement->m_condition[0], true, 0));
	ASSERT_EQ(ifStatement->m_consequence[0]->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_consequence[0]->m_statements[0]->NodeType(), "DeclareIntegerStatement");
	EXPECT_EQ(ifStatement->m_consequence[0]->m_statements[1]->NodeType(), "ExpressionStatement");

	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(ifStatement->m_condition[1], false, 0));
	ASSERT_EQ(ifStatement->m_consequence[1]->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_consequence[1]->m_statements[0]->NodeType(), "DeclareBooleanStatement");
	EXPECT_EQ(ifStatement->m_consequence[1]->m_statements[1]->NodeType(), "ReturnStatement");

	EXPECT_TRUE(ifStatement->m_alternative == NULL);

	EXPECT_EQ(program->String(), expectedString);
}


TEST(ParserTest, IfElseIfElseStatement)
{
	std::string input = R"(
if(true) {
	integer b = 5;
	c = b + 3 * 2;
}
else if(false)
{
	boolean c = true;
	return c;
}
else if(3 < 5.5f)
{

}
else
{
	float lol = 3.5f;
}
)";

	std::string expectedString =
		R"(if (true)
{
integer b = 5;
(c = (b + (3 * 2)));
}
else if(false)
{
boolean c = true;
return c;
}
else if((3 < 5.5))
{
}
else
{
float lol = 3.5;
}
)";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is an if statement
	ASSERT_EQ(statement->NodeType(), "IfStatement");
	ast::IfStatement* ifStatement = (ast::IfStatement*)statement;

	ASSERT_EQ(ifStatement->m_consequence.size(), 3);

	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(ifStatement->m_condition[0], true, 0));
	ASSERT_EQ(ifStatement->m_consequence[0]->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_consequence[0]->m_statements[0]->NodeType(), "DeclareIntegerStatement");
	EXPECT_EQ(ifStatement->m_consequence[0]->m_statements[1]->NodeType(), "ExpressionStatement");

	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(ifStatement->m_condition[1], false, 0));
	ASSERT_EQ(ifStatement->m_consequence[1]->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_consequence[1]->m_statements[0]->NodeType(), "DeclareBooleanStatement");
	EXPECT_EQ(ifStatement->m_consequence[1]->m_statements[1]->NodeType(), "ReturnStatement");

	EXPECT_NO_FATAL_FAILURE(testInfixExpression(ifStatement->m_condition[2], 3, "<", 5.5f), 0);
	ASSERT_EQ(ifStatement->m_consequence[1]->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_consequence[1]->m_statements[0]->NodeType(), "DeclareBooleanStatement");
	EXPECT_EQ(ifStatement->m_consequence[1]->m_statements[1]->NodeType(), "ReturnStatement");

	EXPECT_TRUE(ifStatement->m_alternative != NULL);
	ASSERT_EQ(ifStatement->m_alternative->m_statements.size(), 1);
	EXPECT_EQ(ifStatement->m_alternative->m_statements[0]->NodeType(), "DeclareFloatStatement");

	EXPECT_EQ(program->String(), expectedString);
}


void checkParserErrors(parser::Parser* parser)
{
	if (parser->m_errors.size() == 0)
	{
		return;
	}

	for (int i = 0; i < parser->m_errors.size(); i++)
	{
		std::cout << "Parser error: " << parser->m_errors[i]
			<< std::endl;
	}

	FAIL();
}

void testLiteralExpression(ast::Expression* expression, std::any expectedValue, int testNumber)
{
	// Cannot use a switch on type as std::string is not integral
	// Minor efficiency tradeoff, but this is just a test suite, not the actual interpreter

	std::string expectedValueType = expectedValue.type().name();
	if(expectedValueType == "int")
	{
		ASSERT_NO_FATAL_FAILURE(testIntegerLiteral(expression, std::any_cast<int>(expectedValue), testNumber));
	}
	else if (expectedValueType == "float")
	{
		ASSERT_NO_FATAL_FAILURE(testFloatLiteral(expression, std::any_cast<float>(expectedValue), testNumber));
	}
	else if (expectedValueType == "bool")
	{
		ASSERT_NO_FATAL_FAILURE(testBooleanLiteral(expression, std::any_cast<bool>(expectedValue), testNumber));
	}
	else if (expectedValueType == "char")
	{
		ASSERT_NO_FATAL_FAILURE(testCharacterLiteral(expression, std::any_cast<char>(expectedValue), testNumber));
	}
	else
	{
		FAIL()
			<< "Test #" << testNumber << std::endl;
	}
}

void testIntegerLiteral(ast::Expression* expression, int expectedValue, int testNumber)
{
	ASSERT_EQ(expression->NodeType(), "IntegerLiteral")
		<< "Test #" << testNumber << std::endl;

	ast::IntegerLiteral* integerLiteral = (ast::IntegerLiteral*)expression;

	EXPECT_EQ(integerLiteral->m_value, expectedValue)
		<< "Test #" << testNumber << std::endl;
	EXPECT_EQ(integerLiteral->TokenLiteral(), std::to_string(expectedValue))
		<< "Test #" << testNumber << std::endl;
}

void testFloatLiteral(ast::Expression* expression, float expectedValue, int testNumber)
{
	ASSERT_EQ(expression->NodeType(), "FloatLiteral")
		<< "Test #" << testNumber << std::endl;

	ast::FloatLiteral* floatLiteral = (ast::FloatLiteral*)expression;

	std::ostringstream outputString;
	outputString << expectedValue;

	EXPECT_EQ(floatLiteral->m_value, expectedValue)
		<< "Test #" << testNumber << std::endl;
	EXPECT_EQ(floatLiteral->TokenLiteral(), outputString.str())
		<< "Test #" << testNumber << std::endl;
}

void testBooleanLiteral(ast::Expression* expression, bool expectedValue, int testNumber)
{
	ASSERT_EQ(expression->NodeType(), "BooleanLiteral")
		<< "Test #" << testNumber << std::endl;

	ast::BooleanLiteral* booleanLiteral = (ast::BooleanLiteral*)expression;

	EXPECT_EQ(booleanLiteral->m_value, expectedValue)
		<< "Test #" << testNumber << std::endl;
	EXPECT_EQ(booleanLiteral->TokenLiteral(), expectedValue ? "true" : "false")
		<< "Test #" << testNumber << std::endl;
}

void testCharacterLiteral(ast::Expression* expression, char expectedValue, int testNumber)
{
	ASSERT_EQ(expression->NodeType(), "CharacterLiteral")
		<< "Test #" << testNumber << std::endl;

	ast::CharacterLiteral* characterLiteral = (ast::CharacterLiteral*)expression;

	char charToString[2] = { expectedValue, '\0' };

	EXPECT_EQ(characterLiteral->m_value, expectedValue)
		<< "Test #" << testNumber << std::endl;
	EXPECT_EQ(characterLiteral->TokenLiteral(), charToString)
		<< "Test #" << testNumber << std::endl;
}

void testIdentifier(ast::Expression* expression, std::string* expectedValue, int testNumber)
{
	ASSERT_EQ(expression->NodeType(), "Identifier");
	ast::Identifier* identifier = (ast::Identifier*)expression;

	// Test to see if identifier fields are right
	EXPECT_EQ(identifier->m_name, *expectedValue)
		<< "Test #" << testNumber << std::endl;
	EXPECT_EQ(identifier->TokenLiteral(), *expectedValue)
		<< "Test #" << testNumber << std::endl;
}

void testInfixExpression(ast::Expression* expression, std::any leftValue, std::string op, std::any rightValue, int testNumber)
{
	ASSERT_EQ(expression->NodeType(), "InfixExpression");
	ast::InfixExpression* infixExpression = (ast::InfixExpression*)expression;

	ASSERT_NO_FATAL_FAILURE(testLiteralExpression(infixExpression->m_left_expression, leftValue, testNumber));

	EXPECT_EQ(infixExpression->m_operator, op);

	ASSERT_NO_FATAL_FAILURE(testLiteralExpression(infixExpression->m_right_expression, rightValue, testNumber));
}