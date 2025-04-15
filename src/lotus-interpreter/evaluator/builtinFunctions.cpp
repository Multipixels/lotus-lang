#include <map>
#include <string>
#include <sstream>
#include <iostream>

#include "builtinFunctions.h"
#include "evaluator.h"
#include "object.h"

namespace evaluator
{
	object::Object* logBuiltIn(std::vector<object::Object*>* params)
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

#ifndef NDEBUG
		// For lotus-interpreter-tests
		object::String* stringObj = new object::String(&outputString);
		return stringObj;
#endif

		return &object::NULL_OBJECT;
	}

	object::Object* sizeBuiltIn(std::vector<object::Object*>* params)
	{
		if (params->size() != 1)
		{
			std::ostringstream error;
			error << "Expected 1 argument, got " << params->size() << ".";
			return createError(error.str());
		}

		object::Object* objectToSize = params->front();
		switch (objectToSize->Type()) {
		case object::COLLECTION:
		{
			object::Collection* collection = (object::Collection*)objectToSize;
			int size = collection->m_values.size();
			return new object::Integer(size);
		}
		case object::STRING:
		{
			object::String* string = (object::String*)objectToSize;
			int size = string->m_value.length();
			return new object::Integer(size);
		}
		default:
		{
			std::ostringstream error;
			error << "Argument to `size` not supported, got " << object::objectTypeToString.at(objectToSize->Type()) << ".";
			return createError(error.str());
		}
		}
		
		return &object::NULL_OBJECT;
	}
}