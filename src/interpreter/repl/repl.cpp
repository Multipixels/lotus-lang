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

		while (isRunning) 
		{
			std::cout << prompt;

			std::string input;
			std::getline(std::cin, input);

			lexer::Lexer lexer = lexer::Lexer(&input);
			parser::Parser parser = parser::Parser(lexer);
			ast::Program* program = parser.ParseProgram();

			std::cout << evaluator::evaluate(program)->Inspect() << std::endl;
		}


		return 0;
	}
}