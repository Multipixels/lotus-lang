#include <string>
#include <iostream>

#include "parser.h"
#include "evaluator.h"

namespace repl 
{
	const std::string prompt = ">> ";

	int Start() 
	{
		bool isRunning = true;
		object::Environment environment;

		while (isRunning) 
		{
			std::cout << prompt;

			std::string input;
			std::getline(std::cin, input);

			lexer::Lexer lexer = lexer::Lexer(&input);
			parser::Parser parser = parser::Parser(lexer);
			ast::Program* program = parser.ParseProgram();

			std::cout << evaluator::evaluate(program, &environment)->Inspect() << std::endl;
		}


		return 0;
	}
}