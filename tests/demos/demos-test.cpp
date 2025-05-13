#include <fstream>

#include <gtest/gtest.h>

#include "demos-test.h"
#include "evaluator-test.h"
#include "lexer.h"
#include "parser.h"

TEST(DemosTest, Demos)
{
	typedef struct TestCase
	{
		std::string fileName;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"..\\..\\..\\demos\\fibo.lotus", "55"},
		{"..\\..\\..\\demos\\merge.lotus", "[1, 2, 3, 4, 5, 6]"},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::cout.setstate(std::ios_base::failbit);
		std::shared_ptr<object::Object> evaluated = testEvaluationFile(&tests[i].fileName);
		std::cout.clear();

		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}



std::shared_ptr<object::Object> testEvaluationFile(std::string* p_input)
{
	std::ifstream file(*p_input, std::ios_base::in);
	std::stringstream buffer;
	std::string fileInput;

	if (file.is_open()) {
		buffer << file.rdbuf();
		fileInput = buffer.str();

		lexer::Lexer lexer = lexer::Lexer(&fileInput);
		parser::Parser parser = parser::Parser(lexer);
		std::shared_ptr<ast::Program> program = parser.ParseProgram();
		std::shared_ptr<object::Environment> environment(new object::Environment());

		for (int i = 0; i < parser.m_errors.size(); i++)
		{
			std::cout << "Parser error: " << parser.m_errors[i] << std::endl;
		}
		if (parser.m_errors.size() > 0)
		{
			file.close();
			return evaluator::createError("Parser errors found.");
		}

		std::shared_ptr<object::Object> output = evaluator::evaluate(program, environment);
		file.close();
		return output;
	}
	else
	{
		return evaluator::createError("Could not open the file. Demos are searched for in ..\..\..\demos.");
	}

	return evaluator::createError("Unreachable code");
}