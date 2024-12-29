#include "pch.h"

#include "evaluator-test.h"
#include "lexer.h"
#include "parser.h"

TEST(EvaluatorTest, IntegerExpression)
{
	typedef struct TestCase
	{
		std::string input;
		int expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"5;", 5},
		{"11;", 11},
		{"-5;", -5},
		{"-11;", -11},
		{"-0;", 0},
		{"5 + 5;", 10},
		{"5 - 5;", 0},
		{"5 * 5;", 25},
		{"5 / 5;", 1},
		{"5 / 4;", 1}, // floor division for integers
		{"5 / 6;", 0}, // floor division for integers
		{"(24+7) * -3 - (100/3);", (24 + 7) * -3 - (100 / 3)}, // -126
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		
		ASSERT_EQ(evaluated->Type(), object::INTEGER);
		object::Integer* integer = (object::Integer*)evaluated;

		EXPECT_NO_FATAL_FAILURE(testIntegerObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, FloatExpression)
{
	typedef struct TestCase
	{
		std::string input;
		float expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"5f;", 5},
		{"11.0f;", 11},
		{"-5.5f;", -5.5f},
		{"-11.2f;", -11.2f},
		{"-0.0f;", 0.0f},
		{"5.1f + 5.1f;", 5.1f + 5.1f},
		{"5.0f - 5;", 0.0f},
		{"5 * 5.25f;", 5.0f * 5.25f},
		{"5 / 5.0f;", 1.0f},
		{"5 / 4.0f;", 5.0f / 4.0f},
		{"5.0f / 6;", 5.0f / 6.0f},
		{"(24+7) * -3 - (100/3.0f);", (24 + 7) * -3 - (100 / 3.0f)}, // -126.333 
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		ASSERT_EQ(evaluated->Type(), object::FLOAT);
		object::Float* integer = (object::Float*)evaluated;

		EXPECT_NO_FATAL_FAILURE(testFloatObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, BooleanExpression)
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
		{"!true;", false},
		{"!!true;", true},
		{"!false;", true},
		{"!5;", false},
		{"!!5;", true},
		{"!0;", true},
		{"2 > 3;", false},
		{"1.2f > 1.3f;", false},
		{"2 >= 3;", false},
		{"1.2f >= 1.3f;", false},
		{"2 < 3;", true},
		{"1.2f < 1.3f;", true},
		{"2 <= 3;", true},
		{"1.2f <= 1.3f;", true},
		{"3 < 3;", false},
		{"1.2f < 1.2f;", false},
		{"3 >= 3;", true},
		{"1.2f >= 1.2f;", true},
		{"3 <= 3;", true},
		{"1.2f <= 1.2f;", true},
		{"1 == 2;", false},
		{"1.1f == 1.2f;", false},
		{"1 == 1;", true},
		{"1.1f == 1.1f;", true},
		{"1 != 2;", true},
		{"1.0f != 1.1f;", true},
		{"1 != 1;", false},
		{"1.0f != 1.0f;", false},
		{"true && true;", true},
		{"true && false;", false},
		{"true || true;", true},
		{"true || false;", true},
		{"false || false;", false},
		{"(3 > 5) || (3 > 1 + 1);", true},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		EXPECT_NO_FATAL_FAILURE(testBooleanObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, CharacterExpression)
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
		object::Object* evaluated = testEvaluation(&tests[i].input);

		EXPECT_NO_FATAL_FAILURE(testCharacterObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, ReturnStatement)
{
	typedef struct TestCase
	{
		std::string input;
		int expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"return 5;", 5},
		{"return 5 + 10;", 15},
		{"return 5 + 10; 9;", 15},
		{"9; return 5 + 10; 9;", 15},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		EXPECT_NO_FATAL_FAILURE(testIntegerObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, Error)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedError;
	} TestCase;

	TestCase tests[] =
	{
		{"5 + true;", "'integer + boolean' is not supported."},
		{"5 + true; 5;", "'integer + boolean' is not supported."},
		{"5; 5 + true; 5;", "'integer + boolean' is not supported."},
		{"-true;", "'-boolean' is not supported."},
		{"!'a';", "'!character' is not supported."},
		{"true + true;", "'boolean + boolean' is not supported."},
		{"undefinedIdentifier;", "'undefinedIdentifier' is not defined."},
		{"integer a = true;", "'a' is defined as type 'integer', not 'boolean'."},
		{"float(integer x) integerFunction { return x; }; integerFunction();", "'integerFunction' was supplied with 0 argument(s) instead of 1."},
		{"float(integer x) integerFunction { return x; }; integerFunction(6);", "'integerFunction(6)' produced a value of type 'integer' instead of type 'float'."},
		{"integer(integer x) integerFunction { return x; }; integerFunction(true);", "Parameter 'x' was supplied with a value of type 'boolean' instead of type 'integer' for the function call for 'integerFunction'."},
		{"integer(integer x) integerFunction { x; }; integerFunction(6);", "'integerFunction' has no return value."},
		{"integer myInt = 5; myInt = 6.5f; myInt;", "Cannot assign 'myInt' of type 'integer' a value of type 'float'."},
		{"integer myInt = 5; if ('a') { myInt = 6; } myInt;", "'a' is not a valid truthy value."},
		{"integer myInt = 5; integer myInt = 6;", "Redefinition of 'myInt'."},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		ASSERT_EQ(evaluated->Type(), object::ERROR);
		EXPECT_EQ(((object::Error*)evaluated)->m_error_message, tests[i].expectedError);
	}
}

TEST(EvaluatorTest, Declaration)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer a = 5; a;", 5},
		{"float b = 4.5f; b;", 4.5f},
		{"boolean c = false; c;", false},
		{"character d = 'e'; d;", 'e'},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		testLiteralObject(evaluated, tests[i].expectedValue);
		
	}
}

TEST(EvaluatorTest, Function)
{
	std::string input = "integer(integer x) myFunction { return x + 2; }";

	lexer::Lexer lexer = lexer::Lexer(&input);
	parser::Parser parser = parser::Parser(lexer);
	ast::Program* program = parser.ParseProgram();
	object::Environment environment = new object::Environment();

	object::Object* object = evaluator::evaluate(program, &environment);
	ASSERT_EQ(object->Type(), object::NULL_TYPE);

	std::string functionName = "myFunction";
	object::Object* functionObj = environment.getIdentifier(&functionName);
	ASSERT_EQ(functionObj->Type(), object::FUNCTION);

	object::Function* function = (object::Function*)functionObj;

	EXPECT_EQ(function->m_function_type, object::INTEGER);

	EXPECT_EQ(function->m_parameters.size(), 1);
	EXPECT_EQ(function->m_parameters[0]->m_token.m_literal, "integer");
	EXPECT_EQ(function->m_parameters[0]->m_name.m_name, "x");

	EXPECT_EQ(function->m_function_name->m_name, "myFunction");
	EXPECT_EQ(function->m_body->String(), "return (x + 2);\n");
}

TEST(EvaluatorTest, FunctionCall)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer() integerFunction { return 5; }; integerFunction();", 5},
		{"integer(integer x) integerFunction { return x; }; integerFunction(6);", 6},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		testLiteralObject(evaluated, tests[i].expectedValue);
	}
}

TEST(EvaluatorTest, Reassignment)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer myInt = 5; myInt = 6; myInt;", 6},
		{"integer myInt = 5; if (true) { myInt = 6; } myInt;", 6},
		{"integer myInt = 5; if (true) { integer myInt = 6; myInt = 7; } myInt;", 5},
		{"integer myInt = 5; integer() integerFunction { myInt = 6; return 5; }; integerFunction(); myInt;", 6},
		{"integer myInt = 5; integer() integerFunction { integer myInt = 6; myInt = 7; return 5; }; integerFunction(); myInt;", 5},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		testLiteralObject(evaluated, tests[i].expectedValue);
	}
}

TEST(EvaluatorTest, IfStatement)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer myInt = 5; if (true) { myInt = 6; } myInt;", 6},
		{"integer myInt = 5; if (false) { myInt = 6; } myInt;", 5},
		{"integer myInt = 5; if (true) { myInt = 6; } else { myInt = 7; } myInt;", 6},
		{"integer myInt = 5; if (false) { myInt = 6; } else { myInt = 7; } myInt;", 7},
		{"integer myInt = 5; if (true) { myInt = 6; } else if(true) { myInt = 7; } else { myInt = 8; } myInt;", 6},
		{"integer myInt = 5; if (false) { myInt = 6; } else if(true) { myInt = 7; } else { myInt = 8; } myInt;", 7},
		{"integer myInt = 5; if (false) { myInt = 6; } else if(false) { myInt = 7; } else { myInt = 8; } myInt;", 8},
		{"integer myInt = 5; if (false) { myInt = 6; } else if(false) { myInt = 7; } myInt;", 5},
		{"integer myInt = 5; if (0) { myInt = 6; } myInt;", 5},
		{"integer myInt = 5; if (1) { myInt = 6; } myInt;", 6},
		{"integer myInt = 5; if (0.0f) { myInt = 6; } myInt;", 5},
		{"integer myInt = 5; if (-1.2f) { myInt = 6; } myInt;", 6},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		testLiteralObject(evaluated, tests[i].expectedValue);
	}
}

TEST(EvaluatorTest, WhileLoop)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer i = 0; integer myInt = 5; while(false) { myInt = myInt + 1; } myInt;", 5},
		{"integer i = 0; integer myInt = 5; while( myInt < 10 ) { myInt = myInt + 1; } myInt;", 10},
		{"integer i = 0; integer myInt = 5; while( i < 5 ) { i = i + 1; myInt = myInt + 1; } myInt;", 10},
		{"integer i = 0; integer myInt = 5; while( i > 5 ) { i = i + 1; myInt = myInt + 1; } myInt;", 5},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		testLiteralObject(evaluated, tests[i].expectedValue);
	}
}

TEST(EvaluatorTest, DoWhileLoop)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer i = 0; integer myInt = 5; do { myInt = myInt + 1; } while(false); myInt;", 6},
		{"integer i = 0; integer myInt = 5; do { myInt = myInt + 1; } while( myInt < 10 ); myInt;", 10},
		{"integer i = 0; integer myInt = 5; do { i = i + 1; myInt = myInt + 1; } while( i < 5 ); myInt;", 10},
		{"integer i = 0; integer myInt = 5; do { i = i + 1; myInt = myInt + 1; } while( i > 5 ); myInt;", 6},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		testLiteralObject(evaluated, tests[i].expectedValue);
	}
}

object::Object* testEvaluation(std::string* input)
{
	lexer::Lexer lexer = lexer::Lexer(input);
	parser::Parser parser = parser::Parser(lexer);
	ast::Program* program = parser.ParseProgram();
	object::Environment environment = new object::Environment();

	return evaluator::evaluate(program, &environment);
}

void testLiteralObject(object::Object* object, std::any expectedValue)
{
	// Cannot use a switch on type as std::string is not integral
	// Minor efficiency tradeoff, but this is just a test suite, not the actual interpreter
	
	if (object == NULL) FAIL();

	switch (object->Type())
	{
	case object::INTEGER:
		EXPECT_NO_FATAL_FAILURE(testIntegerObject(object, std::any_cast<int>(expectedValue)));
		return;
	case object::FLOAT:
		EXPECT_NO_FATAL_FAILURE(testFloatObject(object, std::any_cast<float>(expectedValue)));
		return;
	case object::BOOLEAN:
		EXPECT_NO_FATAL_FAILURE(testBooleanObject(object, std::any_cast<bool>(expectedValue)));
		return;
	case object::CHARACTER:
		EXPECT_NO_FATAL_FAILURE(testCharacterObject(object, std::any_cast<char>(expectedValue)));
		return;
	}

	FAIL();
}

void testIntegerObject(object::Object* object, int expectedValue)
{
	ASSERT_EQ(object->Type(), object::INTEGER);
	object::Integer* integer = (object::Integer*)object;

	EXPECT_EQ(integer->m_value, expectedValue);
}

void testFloatObject(object::Object* object, float expectedValue)
{
	ASSERT_EQ(object->Type(), object::FLOAT);
	object::Float* floating = (object::Float*)object;

	EXPECT_EQ(floating->m_value, expectedValue);
}

void testBooleanObject(object::Object* object, bool expectedValue)
{
	ASSERT_EQ(object->Type(), object::BOOLEAN);
	object::Boolean* boolean = (object::Boolean*)object;

	EXPECT_EQ(boolean->m_value, expectedValue);
}

void testCharacterObject(object::Object* object, char expectedValue)
{
	ASSERT_EQ(object->Type(), object::CHARACTER);
	object::Character* character = (object::Character*)object;

	EXPECT_EQ(character->m_value, expectedValue);
}