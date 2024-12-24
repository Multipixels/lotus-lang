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