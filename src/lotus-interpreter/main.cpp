#include <iostream>

#include "repl.h"

int main(int argc, const char* argv[])
{
	if (argc == 1)
	{
		repl::Start();
	}
	else if (argc == 2)
	{
		repl::Run(argv[1]);
	}
	else
	{
		std::cout << "Unexpected number of args.";
		return -1;
	}

	return 0;
}