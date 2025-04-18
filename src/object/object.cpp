#include <string>
#include <sstream>

#include "ast.h"
#include "object.h"
#include "token.h"

namespace object
{
	Null NULL_OBJECT;
	Boolean TRUE_OBJECT(true);
	Boolean FALSE_OBJECT(false);

	Environment::Environment()
		: m_outer(NULL) {}
	Environment::Environment(Environment* outer)
		: m_outer(outer) {}

	object::Object* Environment::getIdentifier(std::string* identifier)
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

	object::Object* Environment::getLocalIdentifier(std::string* identifier)
	{
		if (m_store.count(*identifier) > 0)
		{
			return m_store.at(*identifier);
		}

		return NULL;
	}

	void Environment::setIdentifier(std::string* identifier, Object* value)
	{
		m_store[*identifier] = value;
	}

	void Environment::reassignIdentifier(std::string* identifier, Object* value)
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

	Collection::Collection(ObjectType collection_type, std::vector<Object*> value)
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

	bool Dictionary::ObjCmp::operator()(object::Object* lhs, object::Object* rhs) const
	{
		switch (lhs->Type()) {
		case object::INTEGER:
		{
			if (rhs->Type() != object::INTEGER) return true;
			object::Integer* lhsI = (object::Integer*) lhs;
			object::Integer* rhsI = (object::Integer*) rhs;

			return lhsI->m_value < rhsI->m_value;
		}
		case object::FLOAT:
		{
			if (rhs->Type() != object::FLOAT) return true;
			object::Float* lhsF = (object::Float*)lhs;
			object::Float* rhsF = (object::Float*)rhs;

			return lhsF->m_value < rhsF->m_value;
		}
		case object::BOOLEAN:
		{
			if (rhs->Type() != object::BOOLEAN) return true;
			object::Boolean* lhsB = (object::Boolean*)lhs;
			object::Boolean* rhsB = (object::Boolean*)rhs;

			return lhsB->m_value < rhsB->m_value;
		}
		case object::CHARACTER:
		{
			if (rhs->Type() != object::CHARACTER) return true;
			object::Character* lhsC = (object::Character*)lhs;
			object::Character* rhsC = (object::Character*)rhs;

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

	Dictionary::Dictionary(ObjectType keyType, ObjectType valueType, std::vector<Object*> keys, std::vector<Object*> values)
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

		std::map<Object*, Object*>::iterator it;
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

	Return::Return(Object* return_value)
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

	Function::Function(ObjectType functionType, ast::DeclareFunctionStatement* functionDeclaration, Environment* environment)
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