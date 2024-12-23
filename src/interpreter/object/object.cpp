#include <string>
#include <sstream>

#include "object.h"

namespace object
{
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

	ObjectType Boolean::Type()
	{
		return BOOLEAN;
	}

	std::string Boolean::Inspect()
	{
		std::ostringstream output;
		output << m_value;

		return output.str();
	}

	ObjectType Null::Type()
	{
		return NULL_TYPE;
	}

	std::string Null::Inspect()
	{
		return "null";
	}
}