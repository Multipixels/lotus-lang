#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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

	int Run(const char* fileName)
	{
		std::ifstream file(fileName, std::ios_base::in);
		std::stringstream buffer;
		std::string fileInput;

		if (file.is_open()) {
			buffer << file.rdbuf();
			fileInput = buffer.str();

			lexer::Lexer lexer = lexer::Lexer(&fileInput);
			parser::Parser parser = parser::Parser(lexer);
			ast::Program* program = parser.ParseProgram();
			object::Environment environment;

			std::cout << evaluator::evaluate(program, &environment)->Inspect() << std::endl;
			file.close();
		}
		else
		{
			std::cout << "Error opening file.";
			return -1;
		}

		return 0;
	}
}