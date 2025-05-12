#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "parser.h"
#include "evaluator.h"

namespace repl 
{
	const std::string c_prompt = ">> ";

	int Start() 
	{
		bool isRunning = true;
		std::shared_ptr<object::Environment> environment = std::make_shared<object::Environment>();

		while (isRunning) 
		{
			std::cout << c_prompt;

			std::string input;
			std::getline(std::cin, input);

			lexer::Lexer lexer = lexer::Lexer(&input);
			parser::Parser parser = parser::Parser(lexer);
			std::shared_ptr<ast::Program> program = parser.ParseProgram();

			for(int i = 0; i < parser.m_errors.size(); i++)
			{
				std::cout << "Parser error: " << parser.m_errors[i] << std::endl;
			}
			if (parser.m_errors.size() > 0) continue;

#ifdef DEVELOPMENT_BUILD	
			// Disable timeout in debug mode
#else
			evaluator::g_timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000);
#endif
			std::shared_ptr<object::Object> output = evaluator::evaluate(program, environment);

			if (output->Type() != object::NULL_TYPE)
			{
				std::cout << output->Inspect() << std::endl;
			}
		}

		return 0;
	}

	int Run(const char* p_fileName)
	{
		std::ifstream file(p_fileName, std::ios_base::in);
		std::stringstream buffer;
		std::string fileInput;

		if (file.is_open()) {
			buffer << file.rdbuf();
			fileInput = buffer.str();

			lexer::Lexer lexer = lexer::Lexer(&fileInput);
			parser::Parser parser = parser::Parser(lexer);
			std::shared_ptr<ast::Program> program = parser.ParseProgram();
			object::Environment environment;

			for (int i = 0; i < parser.m_errors.size(); i++)
			{
				std::cout << "Parser error: " << parser.m_errors[i] << std::endl;
			}
			if (parser.m_errors.size() > 0)
			{
				file.close();
				return -1;
			}

			std::shared_ptr<object::Object> output = evaluator::evaluate(program, std::make_shared<object::Environment>(environment));
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