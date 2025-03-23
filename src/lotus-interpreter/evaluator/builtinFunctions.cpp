#include <map>
#include <string>
#include <sstream>
#include <iostream>

#include "builtinFunctions.h"
#include "object.h"

namespace evaluator
{
	object::Object* logBuiltIn(std::vector<object::Object*> *params)
	{
		std::ostringstream outputStringStream;

		for (int i = 0; i < params->size(); i++)
		{
			outputStringStream << params->at(i)->Inspect();
			if (i != params->size() - 1)
			{
				outputStringStream << " ";
			}
		}

		std::string outputString = outputStringStream.str();

		std::cout << outputString << std::endl;

		// For lotus-interpreter-tests
		#ifndef NDEBUG
		object::String* stringObj = new object::String(&outputString);
		return stringObj;
		#endif

		return &object::NULL_OBJECT;
	}
}