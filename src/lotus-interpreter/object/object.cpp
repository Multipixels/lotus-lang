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