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
	Environment::Environment(std::shared_ptr<Environment> p_outer)
		: m_outer(p_outer) {}

	std::shared_ptr<Object> Environment::getIdentifier(std::string* p_identifier)
	{
		if (m_store.count(*p_identifier) > 0)
		{
			return m_store.at(*p_identifier);
		}

		if (m_outer != NULL)
		{
			return m_outer->getIdentifier(p_identifier);
		}

		return NULL;
	}

	std::shared_ptr<Object> Environment::getLocalIdentifier(std::string* p_identifier)
	{
		if (m_store.count(*p_identifier) > 0)
		{
			return m_store.at(*p_identifier);
		}

		return NULL;
	}

	void Environment::setIdentifier(std::string* p_identifier, std::shared_ptr<Object> p_value)
	{
		m_store[*p_identifier] = p_value;
	}

	void Environment::reassignIdentifier(std::string* p_identifier, std::shared_ptr<Object> p_value)
	{
		if (m_store.count(*p_identifier) > 0)
		{
			m_store[*p_identifier] = p_value;
		}
		else
		{
			if(m_outer != NULL)
			{
				m_outer->reassignIdentifier(p_identifier, p_value);
			}
		}
	}

	Integer::Integer()
		: m_value(0)
	{
	}

	Integer::Integer(int p_value)
		: m_value(p_value)
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

	Float::Float(float p_value)
		: m_value(p_value)
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

	Boolean::Boolean(bool p_value)
		: m_value(p_value)
	{
	}

	ObjectType Boolean::Type()
	{
		return BOOLEAN;
	}

	std::string Boolean::Inspect()
	{
		if (m_value)
		{
			return "true";
		}
		return "false";
	}

	Character::Character()
		: m_value('\0')
	{
	}

	Character::Character(char p_value)
		: m_value(p_value)
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
		: m_collectionType(NULL_TYPE)
	{
	}

	Collection::Collection(ObjectType p_collectionType, std::vector<std::shared_ptr<Object>> p_value)
		: m_collectionType(p_collectionType), m_values(p_value)
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

	bool Dictionary::ObjCmp::operator()(std::shared_ptr<Object> p_lhs, std::shared_ptr<Object> p_rhs) const
	{
		switch (p_lhs->Type()) {
		case INTEGER:
		{
			if (p_rhs->Type() != INTEGER) return true;
			std::shared_ptr<Integer> lhsI = std::static_pointer_cast<Integer>(p_lhs);
			std::shared_ptr<Integer> rhsI = std::static_pointer_cast<Integer>(p_rhs);

			return lhsI->m_value < rhsI->m_value;
		}
		case FLOAT:
		{
			if (p_rhs->Type() != FLOAT) return true;
			std::shared_ptr<Float> lhsF = std::static_pointer_cast<Float>(p_lhs);
			std::shared_ptr<Float> rhsF = std::static_pointer_cast<Float>(p_rhs);

			return lhsF->m_value < rhsF->m_value;
		}
		case BOOLEAN:
		{
			if (p_rhs->Type() != BOOLEAN) return true;
			std::shared_ptr<Boolean> lhsB = std::static_pointer_cast<Boolean>(p_lhs);
			std::shared_ptr<Boolean> rhsB = std::static_pointer_cast<Boolean>(p_rhs);

			return lhsB->m_value < rhsB->m_value;
		}
		case CHARACTER:
		{
			if (p_rhs->Type() != CHARACTER) return true;
			std::shared_ptr<Character> lhsC = std::static_pointer_cast<Character>(p_lhs);
			std::shared_ptr<Character> rhsC = std::static_pointer_cast<Character>(p_rhs);

			return lhsC->m_value < rhsC->m_value;
		}
		default:
		{
			return true;
		}
		}
	}

	Dictionary::Dictionary()
		: m_keyType(NULL_TYPE), m_valueType(NULL_TYPE)
	{
	}

	Dictionary::Dictionary(ObjectType p_keyType, ObjectType p_valueType, std::vector<std::shared_ptr<Object>> p_keys, std::vector<std::shared_ptr<Object>> p_values)
		: m_keyType(p_keyType), m_valueType(p_valueType)
	{
		for (int i = 0; i < p_keys.size(); i++)
		{
			m_map.emplace(p_keys.at(i), p_values.at(i));
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

	String::String(std::string *p_value)
		: m_value(*p_value)
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

	Return::Return(std::shared_ptr<Object> p_returnValue)
		: m_returnValue(p_returnValue)
	{
	}

	ObjectType Return::Type()
	{
		return RETURN;
	}

	std::string Return::Inspect()
	{
		return m_returnValue->Inspect();
	}

	Function::Function(ObjectType p_functionType, std::shared_ptr<ast::DeclareFunctionStatement> p_functionDeclaration, std::shared_ptr<Environment> p_environment)
		: m_functionType(p_functionType)
		, m_functionName(&p_functionDeclaration->m_name)
		, m_body(p_functionDeclaration->m_body->m_body)
		, m_environment(p_environment)
	{
		m_parameters = p_functionDeclaration->m_parameters;
	}

	ObjectType Function::Type()
	{
		return FUNCTION;
	}

	std::string Function::Inspect()
	{
		std::ostringstream output;

		output << c_objectTypeToString.at(m_functionType) << "(";

		for (int i = 0; i < m_parameters.size(); i++)
		{
			output << m_parameters[i]->String();
			if (i != m_parameters.size() - 1) output << ", ";
		}

		output << ")" << std::endl << "{" << std::endl << m_body->String() << std::endl << "}";

		return output.str();
	}

	Error::Error(std::string p_errorMessage)
		: m_errorMessage(p_errorMessage)
	{
	}

	ObjectType Error::Type()
	{
		return ERROR;
	}

	std::string Error::Inspect()
	{
		std::ostringstream output;
		output << "Evaluation Error: " << m_errorMessage;
		return output.str();
	}


	Builtin::Builtin(Builtin::BuiltinFunctionPointer p_fn)
		: m_function(p_fn)
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



	std::shared_ptr<Boolean> getBoolean(bool condition)
	{
		if (condition)
		{
			return TRUE_OBJECT;
		}
		return FALSE_OBJECT;
	}
}