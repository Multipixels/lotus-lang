#include <map>
#include <string>
#include <sstream>
#include <iostream>

#include "builtinFunctions.h"
#include "evaluator.h"
#include "object.h"

namespace evaluator
{
	std::shared_ptr<object::Object> logBuiltin(std::vector<std::shared_ptr<object::Object>>* p_params, object::Object* p_object)
	{
		if (p_object != 0)
		{
			return createError("Did not expect to see a parent object for `log`.");
		}

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

	std::shared_ptr<object::Object> collectionAppend(std::vector<std::shared_ptr<object::Object>>* p_params, object::Object* p_object)
	{
		if (p_object == 0)
		{
			return createError("Expected to see a parent object for collection `append`.");
		}

		if (p_object->Type() != object::COLLECTION)
		{
			return createError("Expected a collection to append to.");
		}

		if (p_params->size() != 1)
		{
			std::ostringstream error;
			error << "Expected 1 parameter, got " << p_params->size() << ".";
			return createError(error.str());

		}

		object::Collection* collection = (object::Collection*)(p_object);
		std::shared_ptr<object::Object> item = (*p_params)[0];

		if (item->Type() != collection->m_collectionType)
		{
			std::ostringstream error;
			error << "Collection is of type `" << object::c_objectTypeToString.at(collection->m_collectionType)
				<< "', but tried to append a value of type `" << object::c_objectTypeToString.at(item->Type())
				<< "`.";
			return createError(error.str());
		}

		collection->m_values.push_back(item);

		return object::NULL_OBJECT;

		// TODO: Create tests.
	}
}