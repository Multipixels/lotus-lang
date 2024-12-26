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

	Environment::Environment() {}

	object::Object* Environment::getIdentifier(std::string* identifier)
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

}