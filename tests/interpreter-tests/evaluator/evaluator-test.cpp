#include "pch.h"

#include "evaluator-test.h"
#include "lexer.h"
#include "parser.h"

TEST(EvaluatorTest, IntegerLiteral)
{
	typedef struct TestCase
	{
		std::string input;
		int expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"5", 5},
		{"11", 11},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEval(&tests[i].input);
	}
}

TEST(EvaluatorTest, BooleanLiteral)
{
	typedef struct TestCase
	{
		std::string input;
		bool expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"true", true},
		{"false", false},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEval(&tests[i].input);
	}
}

object::Object* testEval(std::string* input)
{
	lexer::Lexer lexer = lexer::Lexer(input);
	parser::Parser parser = parser::Parser(lexer);
	ast::Program* program = parser.ParseProgram();

	return evaluator::evaluate(program);
}