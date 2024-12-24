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
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		
		ASSERT_EQ(evaluated->Type(), object::INTEGER);
		object::Integer* integer = (object::Integer*)evaluated;

		EXPECT_NO_FATAL_FAILURE(testIntegerObject(evaluated, tests[i].expectedValue));
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
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		EXPECT_NO_FATAL_FAILURE(testBooleanObject(evaluated, tests[i].expectedValue));
	}
}

object::Object* testEvaluation(std::string* input)
{
	lexer::Lexer lexer = lexer::Lexer(input);
	parser::Parser parser = parser::Parser(lexer);
	ast::Program* program = parser.ParseProgram();

	return evaluator::evaluate(program);
}

void testIntegerObject(object::Object* object, int expectedValue)
{
	ASSERT_EQ(object->Type(), object::INTEGER);
	object::Integer* integer = (object::Integer*)object;

	EXPECT_EQ(integer->m_value, expectedValue);
}

void testBooleanObject(object::Object* object, bool expectedValue)
{
	ASSERT_EQ(object->Type(), object::BOOLEAN);
	object::Boolean* boolean = (object::Boolean*)object;

	EXPECT_EQ(boolean->m_value, expectedValue);
}