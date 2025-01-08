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
		ASSERT_EQ(statement->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
		ast::DeclareVariableStatement* declareVariableStatement = (ast::DeclareVariableStatement*)statement;
		ASSERT_EQ(declareVariableStatement->m_token.m_type, token::INTEGER_TYPE);

		EXPECT_EQ(declareVariableStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;
		EXPECT_EQ(declareVariableStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;

		// Test integer literal and value
		ASSERT_NO_FATAL_FAILURE(testIntegerLiteral(declareVariableStatement->m_value, tests[i].expectedValue, i));
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
		ASSERT_EQ(statement->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
		ast::DeclareVariableStatement* declareVariableStatement = (ast::DeclareVariableStatement*)statement;
		ASSERT_EQ(declareVariableStatement->m_token.m_type, token::FLOAT_TYPE);

		EXPECT_EQ(declareVariableStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;
		EXPECT_EQ(declareVariableStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;

		// Test float literal and value
		ASSERT_NO_FATAL_FAILURE(testFloatLiteral(declareVariableStatement->m_value, tests[i].expectedValue, i));
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
		ASSERT_EQ(statement->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
		ast::DeclareVariableStatement* declareVariableStatement = (ast::DeclareVariableStatement*)statement;
		ASSERT_EQ(declareVariableStatement->m_token.m_type, token::BOOLEAN_TYPE);

		EXPECT_EQ(declareVariableStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;
		EXPECT_EQ(declareVariableStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;

		// Test boolean literal and value
		ASSERT_NO_FATAL_FAILURE(testBooleanLiteral(declareVariableStatement->m_value, tests[i].expectedValue, i));
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
		ASSERT_EQ(statement->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
		ast::DeclareVariableStatement* declareVariableStatement = (ast::DeclareVariableStatement*)statement;
		ASSERT_EQ(declareVariableStatement->m_token.m_type, token::CHARACTER_TYPE);

		EXPECT_EQ(declareVariableStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;
		EXPECT_EQ(declareVariableStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;

		// Test character literal and value
		ASSERT_NO_FATAL_FAILURE(testCharacterLiteral(declareVariableStatement->m_value, tests[i].expectedValue, i));
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
		ASSERT_EQ(statement->Type(), ast::RETURN_STATEMENT_NODE);
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
		ASSERT_EQ(statement->Type(), ast::EXPRESSION_STATEMENT_NODE);
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
		ASSERT_EQ(statement->Type(), ast::EXPRESSION_STATEMENT_NODE);
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
		ASSERT_EQ(statement->Type(), ast::EXPRESSION_STATEMENT_NODE);
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
		ASSERT_EQ(statement->Type(), ast::EXPRESSION_STATEMENT_NODE);
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
		ASSERT_EQ(statement->Type(), ast::EXPRESSION_STATEMENT_NODE);
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
		ASSERT_EQ(statement->Type(), ast::EXPRESSION_STATEMENT_NODE);
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		ASSERT_EQ(expressionStatement->m_expression->Type(), ast::PREFIX_EXPRESSION_NODE);
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
		ASSERT_EQ(statement->Type(), ast::EXPRESSION_STATEMENT_NODE);
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		ASSERT_EQ(expressionStatement->m_expression->Type(), ast::INFIX_EXPRESSION_NODE);
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
		{"(24+7) * -3 + (100/3);", "(((24 + 7) * (-3)) + (100 / 3));"},
		{"24 * [1,2,3,4][2];", "(24 * ([1, 2, 3, 4][2]));"},
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
})";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is an if statement
	ASSERT_EQ(statement->Type(), ast::IF_STATEMENT_NODE);
	ast::IfStatement* ifStatement = (ast::IfStatement*)statement;

	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(ifStatement->m_condition, true, 0));
	ASSERT_EQ(ifStatement->m_consequence->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_consequence->m_statements[0]->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
	EXPECT_EQ(ifStatement->m_consequence->m_statements[0]->TokenLiteral(), "integer");
	EXPECT_EQ(ifStatement->m_consequence->m_statements[1]->Type(), ast::EXPRESSION_STATEMENT_NODE);

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
})";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is an if statement
	ASSERT_EQ(statement->Type(), ast::IF_STATEMENT_NODE);
	ast::IfStatement* ifStatement = (ast::IfStatement*)statement;

	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(ifStatement->m_condition, true, 0));
	ASSERT_EQ(ifStatement->m_consequence->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_consequence->m_statements[0]->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
	EXPECT_EQ(ifStatement->m_consequence->m_statements[0]->TokenLiteral(), "integer");
	EXPECT_EQ(ifStatement->m_consequence->m_statements[1]->Type(), ast::EXPRESSION_STATEMENT_NODE);

	ast::IfStatement* elseStatement = ifStatement->m_alternative;
	ASSERT_EQ(elseStatement->m_token.m_type, token::ELSE);
	ASSERT_EQ(elseStatement->m_consequence->m_statements.size(), 2);
	EXPECT_EQ(elseStatement->m_consequence->m_statements[0]->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
	EXPECT_EQ(elseStatement->m_consequence->m_statements[0]->TokenLiteral(), "boolean");
	EXPECT_EQ(elseStatement->m_consequence->m_statements[1]->Type(), ast::RETURN_STATEMENT_NODE);

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
else if (false)
{
boolean c = true;
return c;
})";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is an if statement
	ASSERT_EQ(statement->Type(), ast::IF_STATEMENT_NODE);
	ast::IfStatement* ifStatement = (ast::IfStatement*)statement;

	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(ifStatement->m_condition, true, 0));
	ASSERT_EQ(ifStatement->m_consequence->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_consequence->m_statements[0]->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
	EXPECT_EQ(ifStatement->m_consequence->m_statements[0]->TokenLiteral(), "integer");
	EXPECT_EQ(ifStatement->m_consequence->m_statements[1]->Type(), ast::EXPRESSION_STATEMENT_NODE);

	ast::IfStatement* elseIfStatement = ifStatement->m_alternative;
	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(elseIfStatement->m_condition, false, 0));
	ASSERT_EQ(elseIfStatement->m_token.m_type, token::IF);
	ASSERT_EQ(elseIfStatement->m_consequence->m_statements.size(), 2);
	EXPECT_EQ(elseIfStatement->m_consequence->m_statements[0]->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
	EXPECT_EQ(elseIfStatement->m_consequence->m_statements[0]->TokenLiteral(), "boolean");
	EXPECT_EQ(elseIfStatement->m_consequence->m_statements[1]->Type(), ast::RETURN_STATEMENT_NODE);

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
else if (false)
{
boolean c = true;
return c;
}
else if ((3 < 5.5))
{
}
else
{
float lol = 3.5;
})";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is an if statement
	ASSERT_EQ(statement->Type(), ast::IF_STATEMENT_NODE);
	ast::IfStatement* ifStatement = (ast::IfStatement*)statement;

	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(ifStatement->m_condition, true, 0));
	ASSERT_EQ(ifStatement->m_consequence->m_statements.size(), 2);
	EXPECT_EQ(ifStatement->m_consequence->m_statements[0]->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
	EXPECT_EQ(ifStatement->m_consequence->m_statements[0]->TokenLiteral(), "integer");
	EXPECT_EQ(ifStatement->m_consequence->m_statements[1]->Type(), ast::EXPRESSION_STATEMENT_NODE);

	ast::IfStatement* elseIfStatement1 = ifStatement->m_alternative;
	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(elseIfStatement1->m_condition, false, 0));
	ASSERT_EQ(elseIfStatement1->m_token.m_type, token::IF);
	ASSERT_EQ(elseIfStatement1->m_consequence->m_statements.size(), 2);
	EXPECT_EQ(elseIfStatement1->m_consequence->m_statements[0]->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
	EXPECT_EQ(elseIfStatement1->m_consequence->m_statements[0]->TokenLiteral(), "boolean");
	EXPECT_EQ(elseIfStatement1->m_consequence->m_statements[1]->Type(), ast::RETURN_STATEMENT_NODE);

	ast::IfStatement* elseIfStatement2 = elseIfStatement1->m_alternative;
	EXPECT_NO_FATAL_FAILURE(testInfixExpression(elseIfStatement2->m_condition, 3, "<", 5.5f), 0);
	ASSERT_EQ(elseIfStatement2->m_token.m_type, token::IF);
	ASSERT_EQ(elseIfStatement2->m_consequence->m_statements.size(), 0);

	ast::IfStatement* elseStatement = elseIfStatement2->m_alternative;
	ASSERT_EQ(elseStatement->m_token.m_type, token::ELSE);
	ASSERT_EQ(elseStatement->m_consequence->m_statements.size(), 1);
	EXPECT_EQ(elseStatement->m_consequence->m_statements[0]->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);
	EXPECT_EQ(elseStatement->m_consequence->m_statements[0]->TokenLiteral(), "float");

	EXPECT_EQ(program->String(), expectedString);
}


TEST(ParserTest, WhileStatement)
{
	std::string input = R"(
while(true) {
	5;
	true;
} 
)";

	std::string expectedString =
		R"(while (true)
{
5;
true;
})";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is a while statement
	ASSERT_EQ(statement->Type(), ast::WHILE_STATEMENT_NODE);
	ast::WhileStatement* whileStatement = (ast::WhileStatement*)statement;

	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(whileStatement->m_condition, true, 0));
	ASSERT_EQ(whileStatement->m_consequence->m_statements.size(), 2);
	EXPECT_EQ(whileStatement->m_consequence->m_statements[0]->Type(), ast::EXPRESSION_STATEMENT_NODE);
	EXPECT_EQ(whileStatement->m_consequence->m_statements[1]->Type(), ast::EXPRESSION_STATEMENT_NODE);

	EXPECT_EQ(program->String(), expectedString);
}


TEST(ParserTest, DoWhileStatement)
{
	std::string input = R"(
do {
	5;
	true;
} while(false);
)";

	std::string expectedString =
		R"(do
{
5;
true;
} while (false);)";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is a do while statement
	ASSERT_EQ(statement->Type(), ast::DO_WHILE_STATEMENT_NODE);
	ast::DoWhileStatement* doWhileStatement = (ast::DoWhileStatement*)statement;


	ASSERT_EQ(doWhileStatement->m_consequence->m_statements.size(), 2);
	EXPECT_EQ(doWhileStatement->m_consequence->m_statements[0]->Type(), ast::EXPRESSION_STATEMENT_NODE);
	EXPECT_EQ(doWhileStatement->m_consequence->m_statements[1]->Type(), ast::EXPRESSION_STATEMENT_NODE);
	EXPECT_NO_FATAL_FAILURE(testLiteralExpression(doWhileStatement->m_condition, false, 0));
	EXPECT_EQ(program->String(), expectedString);

}


TEST(ParserTest, ForStatement)
{
	std::string input = R"(
for (integer i = 0; i < 10; i = i + 1;) {
	5;
	true;
}
)";

	std::string expectedString =
		R"(for (integer i = 0; (i < 10); (i = (i + 1));)
{
5;
true;
})";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is an iterate loop
	ASSERT_EQ(statement->Type(), ast::FOR_STATEMENT_NODE);
	ast::ForStatement* forStatement = (ast::ForStatement*)statement;

	// Test m_initialization
	EXPECT_EQ(forStatement->m_initialization->Type(), ast::DECLARE_VARIABLE_STATEMENT_NODE);

	// Test m_condition
	EXPECT_EQ(forStatement->m_condition->Type(), ast::EXPRESSION_STATEMENT_NODE);

	// Test m_updation
	EXPECT_EQ(forStatement->m_updation->Type(), ast::EXPRESSION_STATEMENT_NODE);
	
	// Test m_consequence
	ASSERT_EQ(forStatement->m_consequence->m_statements.size(), 2);
	EXPECT_EQ(forStatement->m_consequence->m_statements[0]->Type(), ast::EXPRESSION_STATEMENT_NODE);
	EXPECT_EQ(forStatement->m_consequence->m_statements[1]->Type(), ast::EXPRESSION_STATEMENT_NODE);

	EXPECT_EQ(program->String(), expectedString);
}

TEST(ParserTest, IterateStatement)
{
	std::string input = R"(
iterate(var : myCollection) {
	5;
	true;
}
)";

	std::string expectedString =
		R"(iterate (var : myCollection)
{
5;
true;
})";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is an iterate loop
	ASSERT_EQ(statement->Type(), ast::ITERATE_STATEMENT_NODE);
	ast::IterateStatement* iterateStatement = (ast::IterateStatement*)statement;

	EXPECT_EQ(iterateStatement->m_var->m_name, "var");
	
	ASSERT_EQ(iterateStatement->m_collection->Type(), ast::IDENTIFIER_NODE);
	ast::Identifier* collection = (ast::Identifier*)iterateStatement->m_collection;
	EXPECT_EQ(collection->m_name, "myCollection");

	ASSERT_EQ(iterateStatement->m_consequence->m_statements.size(), 2);
	EXPECT_EQ(iterateStatement->m_consequence->m_statements[0]->Type(), ast::EXPRESSION_STATEMENT_NODE);
	EXPECT_EQ(iterateStatement->m_consequence->m_statements[1]->Type(), ast::EXPRESSION_STATEMENT_NODE);
	
	EXPECT_EQ(program->String(), expectedString);
}


TEST(ParserTest, DeclareFunctionStatement)
{
	std::string input = R"(
float(integer a, boolean b) myFunction {
	return 5.1f;
}
)";

	std::string expectedString =
		R"(float(integer a, boolean b) myFunction
{
return 5.1;
})";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is a function declaration
	ASSERT_EQ(statement->Type(), ast::DECLARE_FUNCTION_STATEMENT_NODE);
	ast::DeclareFunctionStatement* declareFunctionStatement = (ast::DeclareFunctionStatement*)statement;

	// Check m_token
	EXPECT_EQ(declareFunctionStatement->m_token.m_type, token::FLOAT_TYPE);

	// Check m_parameters
	
	ASSERT_EQ(declareFunctionStatement->m_parameters.size(), 2);
	EXPECT_EQ(declareFunctionStatement->m_parameters[0]->m_token.m_type, token::INTEGER_TYPE);
	EXPECT_EQ(declareFunctionStatement->m_parameters[0]->m_name.m_name, "a");
	EXPECT_TRUE(declareFunctionStatement->m_parameters[0]->m_value == NULL);
	EXPECT_EQ(declareFunctionStatement->m_parameters[1]->m_token.m_type, token::BOOLEAN_TYPE);
	EXPECT_EQ(declareFunctionStatement->m_parameters[1]->m_name.m_name, "b");
	EXPECT_TRUE(declareFunctionStatement->m_parameters[1]->m_value == NULL);

	// Check m_name
	EXPECT_EQ(declareFunctionStatement->m_name.m_name, "myFunction");

	// Check body
	ASSERT_EQ(declareFunctionStatement->m_body->m_body->m_statements.size(), 1);
	EXPECT_EQ(declareFunctionStatement->m_body->m_body->m_statements[0]->Type(), ast::RETURN_STATEMENT_NODE);

	// Check output
	EXPECT_EQ(program->String(), expectedString);
}


TEST(ParserTest, CallExpression)
{
	std::string input = "add(1, 2, 3 * 4, test);";
	std::string expectedString = "add(1, 2, (3 * 4), test);";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

	ast::Statement* statement = program->m_statements[0];
	ASSERT_EQ(program->m_statements.size(), 1)
		<< "Test #0" << std::endl;

	// Test to see if this is this is a call expression
	ASSERT_EQ(statement->Type(), ast::EXPRESSION_STATEMENT_NODE);
	ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;
	ASSERT_EQ(expressionStatement->m_expression->Type(), ast::CALL_EXPRESSION_NODE);
	ast::CallExpression* callExpression = (ast::CallExpression*)expressionStatement->m_expression;

	// Check m_token
	EXPECT_EQ(callExpression->m_token.m_type, token::LPARENTHESIS);

	// Check m_function
	ASSERT_EQ(callExpression->m_function->Type(), ast::IDENTIFIER_NODE);
	ast::Identifier* identifier2 = (ast::Identifier*)callExpression->m_function;
	EXPECT_EQ(identifier2->m_name, "add");

	// Check m_parameters
	ASSERT_EQ(callExpression->m_parameters.size(), 4);
	EXPECT_EQ(callExpression->m_parameters[0]->Type(), ast::INTEGER_LITERAL_NODE);
	EXPECT_EQ(callExpression->m_parameters[1]->Type(), ast::INTEGER_LITERAL_NODE);
	EXPECT_EQ(callExpression->m_parameters[2]->Type(), ast::INFIX_EXPRESSION_NODE);
	EXPECT_EQ(callExpression->m_parameters[3]->Type(), ast::IDENTIFIER_NODE);

	// Check output
	EXPECT_EQ(program->String(), expectedString);
}


TEST(ParserTest, CollectionLiteralExpression)
{
	typedef struct TestCase
	{
		std::string input;
		std::vector<std::any> values;
	} TestCase;

	TestCase tests[] =
	{
		{"[];", {}},
		{"[1, 2, 3, 4, 5];", {1, 2, 3, 4, 5}},
		{"[1.0f, 2.0f, 3.0f, 4.0f, 5f];", {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}},
		{"[true, false];", {true, false}},
		{"['h', 'e', 'l', 'l', 'o'];", {'h', 'e', 'l', 'l', 'o'}},
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
		ASSERT_EQ(statement->Type(), ast::EXPRESSION_STATEMENT_NODE);
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		EXPECT_NO_FATAL_FAILURE(testCollectionLiteral(expressionStatement->m_expression, &tests[i].values, 0));
	}
}


TEST(ParserTest, DeclaringCollectionStatement)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedIdentifier;
		token::TokenType expectedType;
		std::vector<std::any> expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"collection<integer> myCollection = [];", "myCollection", token::INTEGER_TYPE, {}},
		{"collection<integer> myCollection = [1, 2, 3, 4, 5];", "myCollection", token::INTEGER_TYPE, {1, 2, 3, 4, 5}},
		{"collection<float> myCollection = [1.0f, 2.0f, 3.0f, 4.0f, 5f];", "myCollection", token::FLOAT_TYPE, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}},
		{"collection<boolean> myCollection = [true, false];", "myCollection", token::BOOLEAN_TYPE, {true, false}},
		{"collection<character> myCollection = ['h', 'e', 'l', 'l', 'o'];", "myCollection", token::CHARACTER_TYPE, {'h', 'e', 'l', 'l', 'o'}},
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

		// Test declaration identifier literal and name
		ASSERT_EQ(statement->Type(), ast::DECLARE_COLLECTION_STATEMENT_NODE)
			<< "Test #" << i << std::endl;
		ast::DeclareCollectionStatement* declareCollectionStatement = (ast::DeclareCollectionStatement*)statement;

		ASSERT_EQ(declareCollectionStatement->m_token.m_type, token::COLLECTION_TYPE)
			<< "Test #" << i << std::endl;
		ASSERT_EQ(declareCollectionStatement->m_typeToken.m_type, tests[i].expectedType)
			<< "Test #" << i << std::endl;

		EXPECT_EQ(declareCollectionStatement->m_name.m_name, tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;
		EXPECT_EQ(declareCollectionStatement->m_name.TokenLiteral(), tests[i].expectedIdentifier)
			<< "Test #" << i << std::endl;

		// Test to see if we're holding a collection
		ASSERT_EQ(declareCollectionStatement->m_value->Type(), ast::COLLECTION_LITERAL_NODE);
		ast::CollectionLiteral* collectionLiteral = (ast::CollectionLiteral*)declareCollectionStatement->m_value;

		// Test collection value
		ASSERT_EQ(collectionLiteral->m_values.size(), tests[i].expectedValue.size());
		for (int j = 0; j < collectionLiteral->m_values.size(); j++)
		{
			EXPECT_NO_FATAL_FAILURE(testLiteralExpression(collectionLiteral->m_values[j], tests[i].expectedValue[j], i));
		}
	}
}


TEST(ParserTest, CollectionIndexing)
{
	typedef struct TestCase
	{
		std::string input;
		ast::NodeType expectedLeft;
		ast::NodeType expectedRight;
	} TestCase;

	TestCase tests[] =
	{
		{"[1,2,3,4,5][2];", ast::COLLECTION_LITERAL_NODE, ast::INTEGER_LITERAL_NODE},
		{"[1,2,3,4,5][2+1];", ast::COLLECTION_LITERAL_NODE, ast::INFIX_EXPRESSION_NODE},
		{"myCollection[1];", ast::IDENTIFIER_NODE, ast::INTEGER_LITERAL_NODE},
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

		ASSERT_EQ(statement->Type(), ast::EXPRESSION_STATEMENT_NODE);
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		ASSERT_EQ(expressionStatement->m_expression->Type(), ast::INDEX_EXPRESSION_NODE);
		ast::IndexExpression* indexExpression = (ast::IndexExpression*)expressionStatement->m_expression;

		EXPECT_EQ(indexExpression->m_collection->Type(), tests[i].expectedLeft);
		EXPECT_EQ(indexExpression->m_index->Type(), tests[i].expectedRight);
	}
}


TEST(ParserTest, StringLiteral)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{R"("someString";)", "someString"},
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

		ASSERT_EQ(statement->Type(), ast::EXPRESSION_STATEMENT_NODE);
		ast::ExpressionStatement* expressionStatement = (ast::ExpressionStatement*)statement;

		testStringLiteral(expressionStatement->m_expression, &tests[i].expectedValue, i);
	}
}


TEST(ParserTest, ExampleLotus)
{
	std::string input =
		R"(
-- These are comments
-- Double dashes create a single-line comment

-* These 
                    are
    multi
            line
    comments
*-

-- Atomic data types
boolean someTruthValue -* this is an inner comment *- = false;
boolean another_truth_value = true;
integer _someInteger = 1;
float someFloat = 1f;
float anotherFloat = 2.5f;
character theLetterA = 'a';

-- Collections and dictionaries
collection<integer> myCollection = [2, 1, 6, 3, 8];
-- dictionary<integer, integer> myDictionary = {0: 1, 5: 3, 6: 2}; -- TODO
myCollection[2];
-- string myString = "hello"; -- TODO
collection<character> sameString = ['h', 'e', 'l', 'l', 'o'];

-- Functions
integer(integer a, boolean b) myFunction
{
    integer c = 23;
    return c;
}

-- Control structures
if (someTruthValue)
{

}
else if (anotherTruthValue)
{

} 
else 
{

}

while (anotherTruthValue)
{

}

do {

} while (someTruthValue);

for (integer i = 0; i < 10; i = i + 1;) {

}

iterate(var : myCollection) {

}

-- Built in functions
--log("Hello World!");  TODO
length(myCollection);

-- Operators
a == b;
a != b;
a < b; a > b; a <= b; a >= b;
!a;
a && b;
a || b;
)";

std::string expectedString = R"(boolean someTruthValue = false;
boolean another_truth_value = true;
integer _someInteger = 1;
float someFloat = 1;
float anotherFloat = 2.5;
character theLetterA = 'a';
collection<integer> myCollection = [2, 1, 6, 3, 8];
(myCollection[2]);
collection<character> sameString = ['h', 'e', 'l', 'l', 'o'];
integer(integer a, boolean b) myFunction
{
integer c = 23;
return c;
}
if (someTruthValue)
{
}
else if (anotherTruthValue)
{
}
else
{
}
while (anotherTruthValue)
{
}
do
{
} while (someTruthValue);
for (integer i = 0; (i < 10); (i = (i + 1));)
{
}
iterate (var : myCollection)
{
}
length(myCollection);
(a == b);
(a != b);
(a < b);
(a > b);
(a <= b);
(a >= b);
(!a);
(a && b);
(a || b);)";

	lexer::Lexer lexer(&input);
	parser::Parser parser(lexer);
	ast::Program* program = parser.ParseProgram();
	ASSERT_NO_FATAL_FAILURE(checkParserErrors(&parser));

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
	ASSERT_EQ(expression->Type(), ast::INTEGER_LITERAL_NODE)
		<< "Test #" << testNumber << std::endl;

	ast::IntegerLiteral* integerLiteral = (ast::IntegerLiteral*)expression;

	EXPECT_EQ(integerLiteral->m_value, expectedValue)
		<< "Test #" << testNumber << std::endl;
	EXPECT_EQ(integerLiteral->TokenLiteral(), std::to_string(expectedValue))
		<< "Test #" << testNumber << std::endl;
}

void testFloatLiteral(ast::Expression* expression, float expectedValue, int testNumber)
{
	ASSERT_EQ(expression->Type(), ast::FLOAT_LITERAL_NODE)
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
	ASSERT_EQ(expression->Type(), ast::BOOLEAN_LITERAL_NODE)
		<< "Test #" << testNumber << std::endl;

	ast::BooleanLiteral* booleanLiteral = (ast::BooleanLiteral*)expression;

	EXPECT_EQ(booleanLiteral->m_value, expectedValue)
		<< "Test #" << testNumber << std::endl;
	EXPECT_EQ(booleanLiteral->TokenLiteral(), expectedValue ? "true" : "false")
		<< "Test #" << testNumber << std::endl;
}

void testCharacterLiteral(ast::Expression* expression, char expectedValue, int testNumber)
{
	ASSERT_EQ(expression->Type(), ast::CHARACTER_LITERAL_NODE)
		<< "Test #" << testNumber << std::endl;

	ast::CharacterLiteral* characterLiteral = (ast::CharacterLiteral*)expression;

	char charToString[2] = { expectedValue, '\0' };

	EXPECT_EQ(characterLiteral->m_value, expectedValue)
		<< "Test #" << testNumber << std::endl;
	EXPECT_EQ(characterLiteral->TokenLiteral(), charToString)
		<< "Test #" << testNumber << std::endl;
}

void testCollectionLiteral(ast::Expression* expression, std::vector<std::any>* expectedValue, int testNumber)
{
	// Test to see if this is a collection literal expression
	ASSERT_EQ(expression->Type(), ast::COLLECTION_LITERAL_NODE);
	ast::CollectionLiteral* collectionLiteral = (ast::CollectionLiteral*)expression;

	ASSERT_EQ(collectionLiteral->m_values.size(), expectedValue->size());
	for (int j = 0; j < collectionLiteral->m_values.size(); j++)
	{
		EXPECT_NO_FATAL_FAILURE(testLiteralExpression(collectionLiteral->m_values[j], (*expectedValue)[j], testNumber));
	}
}

void testStringLiteral(ast::Expression* expression, std::string* expectedValue, int testNumber)
{
	ASSERT_EQ(expression->Type(), ast::STRING_LITERAL_NODE)
		<< "Test #" << testNumber << std::endl;

	ast::StringLiteral* stringLiteral = (ast::StringLiteral*)expression;

	std::stringstream expected;
	expected << '"' << *expectedValue << '"';

	EXPECT_EQ(stringLiteral->String(), expected.str());
}

void testIdentifier(ast::Expression* expression, std::string* expectedValue, int testNumber)
{
	ASSERT_EQ(expression->Type(), ast::IDENTIFIER_NODE);
	ast::Identifier* identifier = (ast::Identifier*)expression;

	// Test to see if identifier fields are right
	EXPECT_EQ(identifier->m_name, *expectedValue)
		<< "Test #" << testNumber << std::endl;
	EXPECT_EQ(identifier->TokenLiteral(), *expectedValue)
		<< "Test #" << testNumber << std::endl;
}

void testInfixExpression(ast::Expression* expression, std::any leftValue, std::string op, std::any rightValue, int testNumber)
{
	ASSERT_EQ(expression->Type(), ast::INFIX_EXPRESSION_NODE);
	ast::InfixExpression* infixExpression = (ast::InfixExpression*)expression;

	ASSERT_NO_FATAL_FAILURE(testLiteralExpression(infixExpression->m_left_expression, leftValue, testNumber));

	EXPECT_EQ(infixExpression->m_operator, op);

	ASSERT_NO_FATAL_FAILURE(testLiteralExpression(infixExpression->m_right_expression, rightValue, testNumber));
}