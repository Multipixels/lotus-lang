#include <string>
#include <sstream>

#include "object.h"

namespace object
{
	Null NULL_OBJECT;
	Boolean TRUE_OBJECT(true);
	Boolean FALSE_OBJECT(false);

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