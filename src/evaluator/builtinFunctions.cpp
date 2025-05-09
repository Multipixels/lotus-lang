#include <map>
#include <string>
#include <sstream>
#include <iostream>

#include "builtinFunctions.h"
#include "evaluator.h"
#include "object.h"

namespace evaluator
{
	std::shared_ptr<object::Object> logBuiltin(std::vector<std::shared_ptr<object::Object>>* p_params, std::shared_ptr<object::Object> p_object)
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

	std::shared_ptr<object::Object> collectionAppend(std::vector<std::shared_ptr<object::Object>>* p_params, std::shared_ptr<object::Object> p_object)
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

		std::shared_ptr<object::Collection> collection = std::static_pointer_cast<object::Collection>(p_object);
		std::shared_ptr<object::Object> item = (*p_params)[0];

		if (item->Type() != collection->m_collectionType && collection->m_collectionType != object::NULL_TYPE)
		{
			std::ostringstream error;
			error << "Collection is of type `" << object::c_objectTypeToString.at(collection->m_collectionType)
				<< "', but tried to append a value of type `" << object::c_objectTypeToString.at(item->Type())
				<< "`.";
			return createError(error.str());
		}

		collection->m_values.push_back(item);
		collection->m_collectionType = item->Type();

		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> collectionPop(std::vector<std::shared_ptr<object::Object>>* p_params, std::shared_ptr<object::Object> p_object)
	{
		if (p_object == 0)
		{
			return createError("Expected to see a parent object for collection `pop`.");
		}

		if (p_object->Type() != object::COLLECTION)
		{
			return createError("Expected a collection to pop from.");
		}

		if (p_params->size() >= 2)
		{
			std::ostringstream error;
			error << "Expected 0 or 1 parameters, got " << p_params->size() << ".";
			return createError(error.str());
		}

		if (p_params->size() == 1 && (*p_params)[0]->Type() != object::INTEGER)
		{
			std::ostringstream error;
			error << "Expected an integer index to pop from, got " << object::c_objectTypeToString.at((*p_params)[0]->Type()) << ".";
			return createError(error.str());
		}

		std::shared_ptr<object::Collection> collection = std::static_pointer_cast<object::Collection>(p_object);

		if (collection->m_values.size() == 0)
		{
			return createError("Cannot pop from an empty collection.");
		}

		if (p_params->size() == 1 && ((*p_params)[0] < 0 || std::static_pointer_cast<object::Integer>((*p_params)[0])->m_value >= collection->m_values.size()))
		{
			return createError("Attempted to pop an index that is out of bounds.");
		}

		if (p_params->size() == 1)
		{
			collection->m_values.erase(collection->m_values.begin() + std::static_pointer_cast<object::Integer>((*p_params)[0])->m_value);
		}
		else
		{
			collection->m_values.pop_back();
		}

		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> collectionInsert(std::vector<std::shared_ptr<object::Object>>* p_params, std::shared_ptr<object::Object> p_object)
	{
		if (p_object == 0)
		{
			return createError("Expected to see a parent object for collection `insert`.");
		}

		if (p_object->Type() != object::COLLECTION)
		{
			return createError("Expected a collection to insert into.");
		}

		if (p_params->size() != 2)
		{
			std::ostringstream error;
			error << "Expected 2 parameters, got " << p_params->size() << ".";
			return createError(error.str());

		}

		std::shared_ptr<object::Collection> collection = std::static_pointer_cast<object::Collection>(p_object);
		std::shared_ptr<object::Object> index = (*p_params)[0];
		std::shared_ptr<object::Object> item = (*p_params)[1];

		if (index->Type() != object::INTEGER)
		{
			std::ostringstream error;
			error << "Expected an integer index to insert into, got " << index->Type() << ".";
			return createError(error.str());
		}

		std::shared_ptr<object::Integer> integerIndex = std::static_pointer_cast<object::Integer>(index);

		if (integerIndex->m_value < 0 || integerIndex->m_value > collection->m_values.size())
		{
			return createError("Attempted to insert into an index that is out of bounds.");
		}

		if (item->Type() != collection->m_collectionType)
		{
			std::ostringstream error;
			error << "Collection is of type `" << object::c_objectTypeToString.at(collection->m_collectionType)
				<< "', but tried to insert a value of type `" << object::c_objectTypeToString.at(item->Type())
				<< "`.";
			return createError(error.str());
		}

		collection->m_values.insert(collection->m_values.begin() + integerIndex->m_value, item);

		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> dictionaryKeys(std::vector<std::shared_ptr<object::Object>>* p_params, std::shared_ptr<object::Object> p_object)
	{
		if (p_object == 0)
		{
			return createError("Expected to see a parent object for dictionary `keys`.");
		}

		if (p_object->Type() != object::DICTIONARY)
		{
			return createError("Expected a dictionary to get keys from.");
		}

		if (p_params->size() != 0)
		{
			std::ostringstream error;
			error << "Expected 0 parameters, got " << p_params->size() << ".";
			return createError(error.str());

		}

		std::shared_ptr<object::Dictionary> dictionary = std::static_pointer_cast<object::Dictionary>(p_object);

		std::vector<std::shared_ptr<object::Object>> keys;
		for (auto const& a : dictionary->m_map)
		{
			keys.push_back(a.first);
		}

		return std::make_shared<object::Collection>(dictionary->m_keyType, keys);
	}

	std::shared_ptr<object::Object> dictionaryValues(std::vector<std::shared_ptr<object::Object>>* p_params, std::shared_ptr<object::Object> p_object)
	{
		if (p_object == 0)
		{
			return createError("Expected to see a parent object for dictionary `values`.");
		}

		if (p_object->Type() != object::DICTIONARY)
		{
			return createError("Expected a dictionary to get values from.");
		}

		if (p_params->size() != 0)
		{
			std::ostringstream error;
			error << "Expected 0 parameters, got " << p_params->size() << ".";
			return createError(error.str());

		}

		std::shared_ptr<object::Dictionary> dictionary = std::static_pointer_cast<object::Dictionary>(p_object);

		std::vector<std::shared_ptr<object::Object>> keys;
		for (auto const& a : dictionary->m_map)
		{
			keys.push_back(a.second);
		}

		return std::make_shared<object::Collection>(dictionary->m_valueType, keys);
	}
}