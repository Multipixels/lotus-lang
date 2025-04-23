#include <map>
#include <string>
#include <sstream>
#include <iostream>

#include "builtinFunctions.h"
#include "evaluator.h"
#include "object.h"

namespace evaluator
{
	std::shared_ptr<object::Object> logBuiltIn(std::vector<std::shared_ptr<object::Object>>* p_params)
	{
		std::ostringstream outputStringStream;

		for (int i = 0; i < p_params->size(); i++)
		{
			outputStringStream << p_params->at(i)->Inspect();
			if (i != p_params->size() - 1)
			{
				outputStringStream << " ";
			}
		}

		std::string outputString = outputStringStream.str();

		std::cout << outputString << std::endl;

#ifdef DEVELOPMENT_BUILD
		// For lotus-interpreter-tests
		std::shared_ptr<object::String> stringObj(new object::String(&outputString));
		return stringObj;
#endif

		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> sizeBuiltIn(std::vector<std::shared_ptr<object::Object>>* p_params)
	{
		if (p_params->size() != 1)
		{
			std::ostringstream error;
			error << "Expected 1 argument, got " << p_params->size() << ".";
			return createError(error.str());
		}

		std::shared_ptr<object::Object> objectToSize = p_params->front();
		switch (objectToSize->Type()) {
		case object::COLLECTION:
		{
			std::shared_ptr<object::Collection> collection = std::static_pointer_cast<object::Collection>(objectToSize);
			int size = collection->m_values.size();
			return std::shared_ptr<object::Integer>(new object::Integer(size));
		}
		case object::STRING:
		{
			std::shared_ptr<object::String> string = std::static_pointer_cast<object::String>(objectToSize);
			int size = string->m_value.length();
			return std::shared_ptr<object::Integer>(new object::Integer(size));
		}
		default:
		{
			std::ostringstream error;
			error << "Argument to `size` not supported, got " << object::c_objectTypeToString.at(objectToSize->Type()) << ".";
			return createError(error.str());
		}
		}

		return object::NULL_OBJECT;
	}
}