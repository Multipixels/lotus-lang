#include <string>
#include <iostream>

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

			std::cout << "This'll be the output" << std::endl;
		}


		return 0;
	}
}