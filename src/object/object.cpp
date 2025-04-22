#include <string>
#include <sstream>

#include "ast.h"
#include "object.h"
#include "token.h"

namespace object
{
	std::shared_ptr<Null> NULL_OBJECT = std::make_shared<object::Null>();
	std::shared_ptr<Boolean> TRUE_OBJECT = std::make_shared<object::Boolean>(true);
	std::shared_ptr<Boolean> FALSE_OBJECT = std::make_shared<object::Boolean>(false);

	Environment::Environment()
		: m_outer(NULL) {}
	Environment::Environment(std::shared_ptr<Environment> outer)
		: m_outer(outer) {}

	std::shared_ptr<Object> Environment::getIdentifier(std::string* identifier)
	{
		if (m_store.count(*identifier) > 0)
		{
			return m_store.at(*identifier);
		}

		if (m_outer != NULL)
		{
			return m_outer->getIdentifier(identifier);
		}

		return NULL;
	}

	std::shared_ptr<Object> Environment::getLocalIdentifier(std::string* identifier)
	{
		if (m_store.count(*identifier) > 0)
		{
			return m_store.at(*identifier);
		}

		return NULL;
	}

	void Environment::setIdentifier(std::string* identifier, std::shared_ptr<Object> value)
	{
		m_store[*identifier] = value;
	}

	void Environment::reassignIdentifier(std::string* identifier, std::shared_ptr<Object> value)
	{
		if (m_store.count(*identifier) > 0)
		{
			m_store[*identifier] = value;
		}
		else
		{
			if(m_outer != NULL)
			{
				m_outer->reassignIdentifier(identifier, value);
			}
		}
	}

	Integer::Integer()
		: m_value(0)
	{
	}

	Integer::Integer(int value)
		: m_value(value)
	{
	}

	ObjectType Integer::Type()
	{
		return INTEGER;
	}

	std::string Integer::Inspect()
	{
		std::ostringstream output;
		output << m_value;

		return output.str();
	}

	Float::Float()
		: m_value(0)
	{
	}

	Float::Float(float value)
		: m_value(value)
	{
	}

	ObjectType Float::Type()
	{
		return FLOAT;
	}

	std::string Float::Inspect()
	{
		std::ostringstream output;
		output << m_value;

		return output.str();
	}

	Boolean::Boolean()
		: m_value(false)
	{
	}

	Boolean::Boolean(bool value)
		: m_value(value)
	{
	}

	ObjectType Boolean::Type()
	{
		return BOOLEAN;
	}

	std::string Boolean::Inspect()
	{
		if (m_value) return "true";
		return "false";
	}

	Character::Character()
		: m_value('\0')
	{
	}

	Character::Character(char value)
		: m_value(value)
	{
	}

	ObjectType Character::Type()
	{
		return CHARACTER;
	}

	std::string Character::Inspect()
	{
		std::ostringstream output;
		output << m_value;

		return output.str();
	}

	Collection::Collection()
		: m_collection_type(NULL_TYPE)
	{
	}

	Collection::Collection(ObjectType collection_type, std::vector<std::shared_ptr<Object>> value)
		: m_collection_type(collection_type), m_values(value)
	{
	}

	ObjectType Collection::Type()
	{
		return COLLECTION;
	}

	std::string Collection::Inspect()
	{
		std::ostringstream output;
		output << "[";

		for (int i = 0; i < m_values.size(); i++)
		{
			output << m_values[i]->Inspect();
			if (i != m_values.size() - 1) output << ", ";
		}
		output << "]";

		return output.str();
	}

	bool Dictionary::ObjCmp::operator()(std::shared_ptr<Object> lhs, std::shared_ptr<Object> rhs) const
	{
		switch (lhs->Type()) {
		case INTEGER:
		{
			if (rhs->Type() != INTEGER) return true;
			std::shared_ptr<Integer> lhsI = std::static_pointer_cast<Integer>(lhs);
			std::shared_ptr<Integer> rhsI = std::static_pointer_cast<Integer>(rhs);

			return lhsI->m_value < rhsI->m_value;
		}
		case FLOAT:
		{
			if (rhs->Type() != FLOAT) return true;
			std::shared_ptr<Float> lhsF = std::static_pointer_cast<Float>(lhs);
			std::shared_ptr<Float> rhsF = std::static_pointer_cast<Float>(rhs);

			return lhsF->m_value < rhsF->m_value;
		}
		case BOOLEAN:
		{
			if (rhs->Type() != BOOLEAN) return true;
			std::shared_ptr<Boolean> lhsB = std::static_pointer_cast<Boolean>(lhs);
			std::shared_ptr<Boolean> rhsB = std::static_pointer_cast<Boolean>(rhs);

			return lhsB->m_value < rhsB->m_value;
		}
		case CHARACTER:
		{
			if (rhs->Type() != CHARACTER) return true;
			std::shared_ptr<Character> lhsC = std::static_pointer_cast<Character>(lhs);
			std::shared_ptr<Character> rhsC = std::static_pointer_cast<Character>(rhs);

			return lhsC->m_value < rhsC->m_value;
		}
		default:
		{
			return true;
		}
		}
	}

	Dictionary::Dictionary()
		: m_key_type(NULL_TYPE), m_value_type(NULL_TYPE)
	{
	}

	Dictionary::Dictionary(ObjectType keyType, ObjectType valueType, std::vector<std::shared_ptr<Object>> keys, std::vector<std::shared_ptr<Object>> values)
		: m_key_type(keyType), m_value_type(valueType)
	{
		for (int i = 0; i < keys.size(); i++)
		{
			// https://en.cppreference.com/w/cpp/utility/functional/less
			m_map.emplace(keys.at(i), values.at(i));
		}
	}

	ObjectType Dictionary::Type()
	{
		return DICTIONARY;
	}

	std::string Dictionary::Inspect()
	{
		std::ostringstream output;
		output << "{";

		std::map<std::shared_ptr<Object>, std::shared_ptr<Object>>::iterator it;
		for (it = m_map.begin(); it != m_map.end(); it++)
		{
			output << it->first->Inspect() << ": "
				<< it->second->Inspect();

			if (std::next(it) != m_map.end()) output << ", ";
		}

		output << "}";

		return output.str();
	}

	String::String()
		: m_value("")
	{
	}

	String::String(std::string *value)
		: m_value(*value)
	{
	}

	ObjectType String::Type()
	{
		return STRING;
	}

	std::string String::Inspect()
	{
		return m_value;
	}

	Null::Null() {};

	ObjectType Null::Type()
	{
		return NULL_TYPE;
	}

	std::string Null::Inspect()
	{
		return "null";
	}

	Return::Return(std::shared_ptr<Object> return_value)
		: m_return_value(return_value)
	{
	}

	ObjectType Return::Type()
	{
		return RETURN;
	}

	std::string Return::Inspect()
	{
		return m_return_value->Inspect();
	}

	Function::Function(ObjectType functionType, std::shared_ptr<ast::DeclareFunctionStatement> functionDeclaration, std::shared_ptr<Environment> environment)
		: m_function_type(functionType)
		, m_function_name(&functionDeclaration->m_name)
		, m_body(functionDeclaration->m_body->m_body)
		, m_environment(environment)
	{
		m_parameters = functionDeclaration->m_parameters;
	}

	ObjectType Function::Type()
	{
		return FUNCTION;
	}

	std::string Function::Inspect()
	{
		std::ostringstream output;

		output << objectTypeToString.at(m_function_type) << "(";

		for (int i = 0; i < m_parameters.size(); i++)
		{
			output << m_parameters[i]->String();
			if (i != m_parameters.size() - 1) output << ", ";
		}

		output << ")" << std::endl << "{" << std::endl << m_body->String() << std::endl << "}";

		return output.str();
	}

	Error::Error(std::string error_message)
		: m_error_message(error_message)
	{
	}

	ObjectType Error::Type()
	{
		return ERROR;
	}

	std::string Error::Inspect()
	{
		std::ostringstream output;
		output << "Evaluation Error: " << m_error_message;
		return output.str();
	}


	Builtin::Builtin(Builtin::BuiltinFunctionPointer fn)
		: m_function(fn)
	{
	}

	ObjectType Builtin::Type()
	{
		return BUILTIN_FUNCTION;
	}

	std::string Builtin::Inspect()
	{
		std::ostringstream output;
		output << "builtin function";
		return output.str();
	}
}