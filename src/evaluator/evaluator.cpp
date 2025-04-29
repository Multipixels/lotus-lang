#include <set>
#include <sstream>

#include "builtinFunctions.h"
#include "evaluator.h"

namespace evaluator
{
	std::chrono::steady_clock::time_point g_timeout = std::chrono::steady_clock::time_point();

	std::shared_ptr<object::Object> evaluate(std::shared_ptr<ast::Node> p_node, std::shared_ptr<object::Environment> p_environment)
	{
		if (g_timeout != std::chrono::steady_clock::time_point() && g_timeout < std::chrono::steady_clock::now())
		{
			return createError("Evaluation of the program timed out.");
		}

		if (p_node == NULL) return object::NULL_OBJECT;

		switch (p_node->Type())
		{
			case ast::PROGRAM_NODE:                      return evaluateProgram(std::static_pointer_cast<ast::Program>(p_node), p_environment);
			case ast::IDENTIFIER_NODE:                   return evaluateIdentifier(std::static_pointer_cast<ast::Identifier>(p_node), p_environment);
			case ast::BLOCK_STATEMENT_NODE:              return evaluateBlockStatement(std::static_pointer_cast<ast::BlockStatement>(p_node), p_environment);
			case ast::INTEGER_LITERAL_NODE:              return evaluateIntegerLiteral(std::static_pointer_cast<ast::IntegerLiteral>(p_node), p_environment);
			case ast::FLOAT_LITERAL_NODE:                return evaluateFloatLiteral(std::static_pointer_cast<ast::FloatLiteral>(p_node), p_environment);
			case ast::BOOLEAN_LITERAL_NODE:              return evaluateBooleanLiteral(std::static_pointer_cast<ast::BooleanLiteral>(p_node), p_environment);
			case ast::CHARACTER_LITERAL_NODE:            return evaluateCharacterLiteral(std::static_pointer_cast<ast::CharacterLiteral>(p_node), p_environment);
			case ast::COLLECTION_LITERAL_NODE:           return evaluateCollectionLiteral(std::static_pointer_cast<ast::CollectionLiteral>(p_node), p_environment);
			case ast::DICTIONARY_LITERAL_NODE:           return evaluateDictionaryLiteral(std::static_pointer_cast<ast::DictionaryLiteral>(p_node), p_environment);
			case ast::STRING_LITERAL_NODE:               return evaluateStringLiteral(std::static_pointer_cast<ast::StringLiteral>(p_node), p_environment);
			case ast::PREFIX_EXPRESSION_NODE:            return evaluatePrefixExpression(std::static_pointer_cast<ast::PrefixExpression>(p_node), p_environment);
			case ast::INFIX_EXPRESSION_NODE:             return evaluateInfixExpression(std::static_pointer_cast<ast::InfixExpression>(p_node), p_environment);
			case ast::CALL_EXPRESSION_NODE:              return evaluateCallExpression(std::static_pointer_cast<ast::CallExpression>(p_node), p_environment);
			case ast::INDEX_EXPRESSION_NODE:             return evaluateIndexExpression(std::static_pointer_cast<ast::IndexExpression>(p_node), p_environment);
			case ast::DECLARE_VARIABLE_STATEMENT_NODE:   return evaluateDeclareVariable(std::static_pointer_cast<ast::DeclareVariableStatement>(p_node), p_environment);
			case ast::DECLARE_COLLECTION_STATEMENT_NODE: return evaluateDeclareCollection(std::static_pointer_cast<ast::DeclareCollectionStatement>(p_node), p_environment);
			case ast::DECLARE_DICTIONARY_STATEMENT_NODE: return evaluateDeclareDictionary(std::static_pointer_cast<ast::DeclareDictionaryStatement>(p_node), p_environment);
			case ast::DECLARE_FUNCTION_STATEMENT_NODE:   return evaluateDeclareFunction(std::static_pointer_cast<ast::DeclareFunctionStatement>(p_node), p_environment);
			case ast::RETURN_STATEMENT_NODE:             return evaluateReturnStatement(std::static_pointer_cast<ast::ReturnStatement>(p_node), p_environment);
			case ast::EXPRESSION_STATEMENT_NODE:         return evaluate(std::static_pointer_cast<ast::ExpressionStatement>(p_node)->m_expression, p_environment);
			case ast::IF_STATEMENT_NODE:                 return evaluateIfStatement(std::static_pointer_cast<ast::IfStatement>(p_node), p_environment);
			case ast::WHILE_STATEMENT_NODE:              return evaluateWhileStatement(std::static_pointer_cast<ast::WhileStatement>(p_node), p_environment);
			case ast::DO_WHILE_STATEMENT_NODE:           return evaluateDoWhileStatement(std::static_pointer_cast<ast::DoWhileStatement>(p_node), p_environment);
			case ast::FOR_STATEMENT_NODE:                return evaluateForStatement(std::static_pointer_cast<ast::ForStatement>(p_node), p_environment);
			case ast::ITERATE_STATEMENT_NODE:            return evaluateIterateStatement(std::static_pointer_cast<ast::IterateStatement>(p_node), p_environment);
		}

		return createError("Encountered an unexpected AST node");
	}

	std::shared_ptr<object::Object> evaluateProgram(std::shared_ptr<ast::Program> p_program, std::shared_ptr<object::Environment> p_environment)
	{
		std::shared_ptr<object::Object> result = object::NULL_OBJECT;

		for (int i = 0; i < p_program->m_statements.size(); i++)
		{
			result = evaluate(p_program->m_statements[i], p_environment);
			if (result->Type() == object::ERROR) return result;

			if (result->Type() == object::RETURN)
			{
				std::shared_ptr<object::Return> returnObj = std::static_pointer_cast<object::Return>(result);
				return returnObj->m_returnValue;
			}
		}

		return result;
	}

	std::shared_ptr<object::Object> evaluateIdentifier(std::shared_ptr<ast::Identifier> p_identifier, std::shared_ptr<object::Environment> p_environment) 
	{
		std::shared_ptr<object::Object> result = p_environment->getIdentifier(&(p_identifier->m_name));
		if (result != NULL)
		{
			return result;
		}

		if (c_builtins.find(p_identifier->m_name) != c_builtins.end()) {
			return c_builtins.at(p_identifier->m_name);
		}

		std::ostringstream error;
		error << "'" << p_identifier->m_name << "' is not defined.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateBlockStatement(std::shared_ptr<ast::BlockStatement> p_blockStatements, std::shared_ptr<object::Environment> p_environment)
	{
		std::shared_ptr<object::Object> result = object::NULL_OBJECT;

		for (int i = 0; i < p_blockStatements->m_statements.size(); i++)
		{
			result = evaluate(p_blockStatements->m_statements[i], p_environment);

			if (result != NULL && result->Type() == object::RETURN)
			{
				return result;
			}

			if (result != NULL && result->Type() == object::ERROR)
			{
				return result;
			}
		}

		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> evaluateIntegerLiteral(std::shared_ptr<ast::IntegerLiteral> p_integerLiteral, std::shared_ptr<object::Environment> p_environment)
	{
		std::shared_ptr<object::Integer> object(new object::Integer);
		object->m_value = p_integerLiteral->m_value;
		return object;
	}

	std::shared_ptr<object::Object> evaluateFloatLiteral(std::shared_ptr<ast::FloatLiteral> p_floatLiteral, std::shared_ptr<object::Environment> p_environment)
	{
		std::shared_ptr<object::Float> object(new object::Float);
		object->m_value = p_floatLiteral->m_value;
		return object;
	}

	std::shared_ptr<object::Object> evaluateBooleanLiteral(std::shared_ptr<ast::BooleanLiteral> p_booleanLiteral, std::shared_ptr<object::Environment> p_environment)
	{
		if (p_booleanLiteral->m_value)
		{
			return object::TRUE_OBJECT;
		}
		return object::FALSE_OBJECT;
	}

	std::shared_ptr<object::Object> evaluateCharacterLiteral(std::shared_ptr<ast::CharacterLiteral> p_characterLiteral, std::shared_ptr<object::Environment> p_environment)
	{
		std::shared_ptr<object::Character> object(new object::Character);
		object->m_value = p_characterLiteral->m_value;
		return object;
	}

	std::shared_ptr<object::Object> evaluateCollectionLiteral(std::shared_ptr<ast::CollectionLiteral> p_collectionLiteral, std::shared_ptr<object::Environment> p_environment)
	{
		if (p_collectionLiteral->m_values.size() == 0)
		{
			return std::shared_ptr<object::Collection>(new object::Collection(object::NULL_TYPE, {}));
		}

		std::shared_ptr<object::Collection> object(new object::Collection);

		for (int i = 0; i < p_collectionLiteral->m_values.size(); i++)
		{
			std::shared_ptr<object::Object> evaluatedItem = evaluate(p_collectionLiteral->m_values[i], p_environment);

			if (evaluatedItem->Type() == object::ERROR) return evaluatedItem;

			if (object->m_collectionType != object::NULL_TYPE && evaluatedItem->Type() != object->m_collectionType)
			{
				std::ostringstream error;
				error << "The collection " << p_collectionLiteral->String() << " must have uniform typing of elements.";
				return createError(error.str());
			}

			if (object->m_collectionType == object::NULL_TYPE) object->m_collectionType = evaluatedItem->Type();
			object->m_values.push_back(evaluatedItem);
		}

		return object;
	}

	std::shared_ptr<object::Object> evaluateDictionaryLiteral(std::shared_ptr<ast::DictionaryLiteral> p_dictionaryLiteral, std::shared_ptr<object::Environment> p_environment)
	{
		if (p_dictionaryLiteral->m_map.size() == 0)
		{
			return std::shared_ptr<object::Dictionary>(new object::Dictionary);
		}

		std::shared_ptr<object::Dictionary> object(new object::Dictionary);

		std::map < std::shared_ptr<ast::Expression>, std::shared_ptr<ast::Expression>> ::iterator it;
		for (it = p_dictionaryLiteral->m_map.begin(); it != p_dictionaryLiteral->m_map.end(); it++)
		{
			// Checking the key
			std::shared_ptr<object::Object> evaluatedKey = evaluate(it->first, p_environment);
			if (evaluatedKey->Type() == object::ERROR) return evaluatedKey;

			if (evaluatedKey->Type() != object::INTEGER && evaluatedKey->Type() != object::FLOAT &&
				evaluatedKey->Type() != object::BOOLEAN && evaluatedKey->Type() != object::CHARACTER)
			{
				std::ostringstream error;
				error << "Invalid dictionary key type. " <<
					object::c_objectTypeToString.at(evaluatedKey->Type()) << " is not a hashable type.";
				return createError(error.str());
			}

			if (object->m_keyType != object::NULL_TYPE && evaluatedKey->Type() != object->m_keyType)
			{
				return createError("Dictionary has mismatching key types.");
			}

			if (object->m_keyType == object::NULL_TYPE) object->m_keyType = evaluatedKey->Type();
			if (object->m_map.find(evaluatedKey) != object->m_map.end())
			{
				return createError("Dictionary initialized with duplicate key.");
			}

			// Checking the value
			std::shared_ptr<object::Object> evaluatedValue = evaluate(it->second, p_environment);
			if (evaluatedValue->Type() == object::ERROR) return evaluatedValue;

			if (object->m_valueType != object::NULL_TYPE && evaluatedValue->Type() != object->m_valueType)
			{
				return createError("Dictionary has mismatching value types.");
			}

			if (object->m_valueType == object::NULL_TYPE) object->m_valueType = evaluatedValue->Type();

			// Storing key value pair
			object->m_map.emplace(evaluatedKey, evaluatedValue);
		}

		return object;
	}

	std::shared_ptr<object::Object> evaluateStringLiteral(std::shared_ptr<ast::StringLiteral> p_stringLiteral, std::shared_ptr<object::Environment> p_environment)
	{
		std::stringstream value;

		for (int i = 0; i < p_stringLiteral->m_stringCollection->m_values.size(); i++)
		{
			// Take expression from string collection and cast to character literal pointer
			std::shared_ptr<ast::CharacterLiteral> characterLiteral = std::static_pointer_cast<ast::CharacterLiteral>(p_stringLiteral->m_stringCollection->m_values[i]);
			value << characterLiteral->m_value;
		}

		std::string valueStr = value.str();
		std::shared_ptr<object::String> object(new object::String(&valueStr));
		return object;
	}

	void evaluateExpressions(std::vector<std::shared_ptr<ast::Expression>>* p_source, std::vector<std::shared_ptr<object::Object>>* p_destination, std::shared_ptr<object::Environment> p_environment)
	{
		for (int i = 0; i < p_source->size(); i++)
		{
			std::shared_ptr<object::Object> evaluatedExpression = evaluate((*p_source)[i], p_environment);

			if (evaluatedExpression->Type() == object::ERROR)
			{
				(*p_destination).clear();
				(*p_destination).push_back(evaluatedExpression);
				return;
			}
			(*p_destination).push_back(evaluatedExpression);
		}
	}

	std::shared_ptr<object::Object> evaluatePrefixExpression(std::shared_ptr<ast::PrefixExpression> p_prefixExpression, std::shared_ptr<object::Environment> p_environment)
	{
		std::shared_ptr<object::Object> rightObject = evaluate(p_prefixExpression->m_rightExpression, p_environment);
		if (rightObject->Type() == object::ERROR) return rightObject;

		// TODO: Change operator to an enum for performance gain
		if (p_prefixExpression->m_operator == "!") return evaluateBangOperatorExpression(rightObject);
		if (p_prefixExpression->m_operator == "-") return evaluateMinusPrefixOperatorExpression(rightObject);

		std::ostringstream error;
		error << p_prefixExpression->m_operator << object::c_objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateBangOperatorExpression(std::shared_ptr<object::Object> p_expression)
	{
		switch (p_expression->Type())
		{
		case object::INTEGER:
		{
			std::shared_ptr<object::Integer> integer = std::static_pointer_cast<object::Integer>(p_expression);
			if (integer->m_value) return object::FALSE_OBJECT;
			else return object::TRUE_OBJECT;
		}
		case object::FLOAT:
		{
			std::shared_ptr<object::Float> floating = std::static_pointer_cast<object::Float>(p_expression);
			if (floating->m_value) return object::FALSE_OBJECT;
			else return object::TRUE_OBJECT;
		}
		case object::BOOLEAN:
		{
			std::shared_ptr<object::Boolean> boolean = std::static_pointer_cast<object::Boolean>(p_expression);
			if (boolean->m_value) return object::FALSE_OBJECT;
			else return object::TRUE_OBJECT;
		}
		}

		std::ostringstream error;
		error << "'!" << object::c_objectTypeToString.at(p_expression->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateMinusPrefixOperatorExpression(std::shared_ptr<object::Object> p_expression)
	{
		switch (p_expression->Type())
		{
		case object::INTEGER:
		{
			std::shared_ptr<object::Integer> integer = std::static_pointer_cast<object::Integer>(p_expression);
			std::shared_ptr<object::Integer> returnValue(new object::Integer(-integer->m_value));
			return returnValue;
		}
		case object::FLOAT:
		{
			std::shared_ptr<object::Float> floating = std::static_pointer_cast<object::Float>(p_expression);
			std::shared_ptr<object::Float> returnValue(new object::Float(-floating->m_value));
			return returnValue;
		}
		}

		std::ostringstream error;
		error << "'-" << object::c_objectTypeToString.at(p_expression->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateInfixExpression(std::shared_ptr<ast::InfixExpression> p_infixExpression, std::shared_ptr<object::Environment> p_environment)
	{
		// identifier = newValue;
		if (p_infixExpression->m_leftExpression->Type() == ast::IDENTIFIER_NODE && p_infixExpression->m_operator == "=")
		{
			std::shared_ptr<ast::Identifier> identifier = std::static_pointer_cast<ast::Identifier>(p_infixExpression->m_leftExpression);
			std::shared_ptr<object::Object> savedValue = p_environment->getIdentifier(&identifier->m_name);
			if (savedValue == NULL)
			{
				std::ostringstream error;
				error << "'" << identifier->m_name << "' is not defined.";
				return createError(error.str());
			}

			std::shared_ptr<object::Object> rightObject = evaluate(p_infixExpression->m_rightExpression, p_environment);

			if (rightObject->Type() == object::ERROR) return rightObject;

			if (savedValue->Type() != rightObject->Type())
			{
				std::ostringstream error;
				error << "Cannot assign '" << identifier->m_name << "' of type '"
					<< object::c_objectTypeToString.at(savedValue->Type()) << "' a value of type '"
					<< object::c_objectTypeToString.at(rightObject->Type()) << "'.";
				return createError(error.str());
			}

			p_environment->reassignIdentifier(&identifier->m_name, rightObject);

			return object::NULL_OBJECT;
		}

		// variables[index] = newValue;
		else if (p_infixExpression->m_leftExpression->Type() == ast::INDEX_EXPRESSION_NODE && p_infixExpression->m_operator == "=")
		{
			std::shared_ptr<ast::IndexExpression> indexExpression = std::static_pointer_cast<ast::IndexExpression>(p_infixExpression->m_leftExpression);

			std::shared_ptr<object::Object> object = evaluate(indexExpression->m_collection, p_environment);
			if (object->Type() == object::ERROR) return object;

			std::shared_ptr<object::Object> indexObject = evaluate(indexExpression->m_index, p_environment);
			if (indexObject->Type() == object::ERROR) return indexObject;

			std::shared_ptr<object::Object> valueObject = evaluate(p_infixExpression->m_rightExpression, p_environment);
			if (valueObject->Type() == object::ERROR) return valueObject;

			switch (object->Type()) {
			case object::COLLECTION:
				return collectionValueReassignment(std::static_pointer_cast<object::Collection>(object), indexObject, valueObject);
			case object::DICTIONARY:
				return dictionaryValueReassignment(std::static_pointer_cast<object::Dictionary>(object), indexObject, valueObject);
			case object::STRING:
				return createError("Strings are immutable.");
			default:
				return createError("This should be an unreachable piece of code.");
			}
		}

		// member access
		else if (p_infixExpression->m_operator == ".")
		{
			std::shared_ptr<object::Object> object = evaluate(p_infixExpression->m_leftExpression, p_environment);
			if (object->Type() == object::ERROR) return object;

			if (p_infixExpression->m_rightExpression->Type() != ast::IDENTIFIER_NODE)
			{
				std::ostringstream error;
				error << "Expected to see a member variable or function, got " <<
					p_infixExpression->m_rightExpression->String() << ".";
				return createError(error.str());
			}
			std::shared_ptr<ast::Identifier> name = std::static_pointer_cast<ast::Identifier>(p_infixExpression->m_rightExpression);
			
			return object->Member(name->m_name);
		}

		// General infix expressions
		std::shared_ptr<object::Object> leftObject = evaluate(p_infixExpression->m_leftExpression, p_environment);
		if (leftObject->Type() == object::ERROR) return leftObject;

		std::shared_ptr<object::Object> rightObject = evaluate(p_infixExpression->m_rightExpression, p_environment);
		if (rightObject->Type() == object::ERROR) return rightObject;

		switch (leftObject->Type())
		{
		case object::INTEGER:
		{
			if (rightObject->Type() == object::INTEGER)
			{
				return evaluateIntegerInfixExpression(std::static_pointer_cast<object::Integer>(leftObject), &p_infixExpression->m_operator, std::static_pointer_cast<object::Integer>(rightObject));
			}
			if (rightObject->Type() == object::FLOAT)
			{
				std::shared_ptr<object::Float> castedInteger(new object::Float(std::static_pointer_cast<object::Integer>(leftObject)->m_value));
				return evaluateFloatInfixExpression(castedInteger, &p_infixExpression->m_operator, std::static_pointer_cast<object::Float>(rightObject));
			}
			break;
		}
		case object::FLOAT:
		{
			if (rightObject->Type() == object::INTEGER)
			{
				std::shared_ptr<object::Float> castedInteger(new object::Float(std::static_pointer_cast<object::Integer>(rightObject)->m_value));
				return evaluateFloatInfixExpression(std::static_pointer_cast<object::Float>(leftObject), &p_infixExpression->m_operator, castedInteger);
			}
			if (rightObject->Type() == object::FLOAT)
			{
				return evaluateFloatInfixExpression(std::static_pointer_cast<object::Float>(leftObject), &p_infixExpression->m_operator, std::static_pointer_cast<object::Float>(rightObject));
			}
			break;
		}
		case object::BOOLEAN:
		{
			if (rightObject->Type() == object::BOOLEAN)
			{
				return evaluateBooleanInfixExpression(std::static_pointer_cast<object::Boolean>(leftObject), &p_infixExpression->m_operator, std::static_pointer_cast<object::Boolean>(rightObject));
			}
			break;
		}
		case object::CHARACTER:
		{
			if (rightObject->Type() == object::CHARACTER)
			{
				return evaluateCharacterInfixExpression(std::static_pointer_cast<object::Character>(leftObject), &p_infixExpression->m_operator, std::static_pointer_cast<object::Character>(rightObject));
			}
			break;
		}
		}

		std::ostringstream error;
		error << "'" << object::c_objectTypeToString.at(leftObject->Type())
			<< ' ' << *&p_infixExpression->m_operator << ' '
			<< object::c_objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateIntegerInfixExpression(std::shared_ptr<object::Integer> p_leftObject, std::string* p_infixOperator, std::shared_ptr<object::Integer> p_rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*p_infixOperator == "+") return std::shared_ptr<object::Integer>(new object::Integer(p_leftObject->m_value + p_rightObject->m_value));
		if (*p_infixOperator == "-") return std::shared_ptr<object::Integer>(new object::Integer(p_leftObject->m_value - p_rightObject->m_value));
		if (*p_infixOperator == "*") return std::shared_ptr<object::Integer>(new object::Integer(p_leftObject->m_value * p_rightObject->m_value));
		if (*p_infixOperator == "/") return std::shared_ptr<object::Integer>(new object::Integer(p_leftObject->m_value / p_rightObject->m_value));

		if (*p_infixOperator == "<")  return object::getBoolean(p_leftObject->m_value < p_rightObject->m_value);
		if (*p_infixOperator == "<=") return object::getBoolean(p_leftObject->m_value <= p_rightObject->m_value);
		if (*p_infixOperator == ">")  return object::getBoolean(p_leftObject->m_value > p_rightObject->m_value);
		if (*p_infixOperator == ">=") return object::getBoolean(p_leftObject->m_value >= p_rightObject->m_value);
		if (*p_infixOperator == "==") return object::getBoolean(p_leftObject->m_value == p_rightObject->m_value);
		if (*p_infixOperator == "!=") return object::getBoolean(p_leftObject->m_value != p_rightObject->m_value);

		std::ostringstream error;
		error << "'" << object::c_objectTypeToString.at(p_leftObject->Type())
			<< ' ' << *p_infixOperator << ' '
			<< object::c_objectTypeToString.at(p_rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateFloatInfixExpression(std::shared_ptr<object::Float> p_leftObject, std::string* p_infixOperator, std::shared_ptr<object::Float> p_rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*p_infixOperator == "+")  return std::shared_ptr<object::Float>(new object::Float(p_leftObject->m_value + p_rightObject->m_value));
		if (*p_infixOperator == "-")  return std::shared_ptr<object::Float>(new object::Float(p_leftObject->m_value - p_rightObject->m_value));
		if (*p_infixOperator == "*")  return std::shared_ptr<object::Float>(new object::Float(p_leftObject->m_value * p_rightObject->m_value));
		if (*p_infixOperator == "/")  return std::shared_ptr<object::Float>(new object::Float(p_leftObject->m_value / p_rightObject->m_value));
		if (*p_infixOperator == "<")  return object::getBoolean(p_leftObject->m_value < p_rightObject->m_value);
		if (*p_infixOperator == "<=") return object::getBoolean(p_leftObject->m_value <= p_rightObject->m_value);
		if (*p_infixOperator == ">")  return object::getBoolean(p_leftObject->m_value > p_rightObject->m_value);
		if (*p_infixOperator == ">=") return object::getBoolean(p_leftObject->m_value >= p_rightObject->m_value);
		if (*p_infixOperator == "==") return object::getBoolean(p_leftObject->m_value == p_rightObject->m_value);
		if (*p_infixOperator == "!=") return object::getBoolean(p_leftObject->m_value != p_rightObject->m_value);

		std::ostringstream error;
		error << "'" << object::c_objectTypeToString.at(p_leftObject->Type())
			<< ' ' << *p_infixOperator << ' '
			<< object::c_objectTypeToString.at(p_rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateBooleanInfixExpression(std::shared_ptr<object::Boolean> p_leftObject, std::string* p_infixOperator, std::shared_ptr<object::Boolean> p_rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*p_infixOperator == "&&") return object::getBoolean(p_leftObject->m_value && p_rightObject->m_value);
		if (*p_infixOperator == "||") return object::getBoolean(p_leftObject->m_value || p_rightObject->m_value);
		if (*p_infixOperator == "==") return object::getBoolean(p_leftObject->m_value == p_rightObject->m_value);
		if (*p_infixOperator == "!=") return object::getBoolean(p_leftObject->m_value != p_rightObject->m_value);

		std::ostringstream error;
		error << "'" << object::c_objectTypeToString.at(p_leftObject->Type())
			<< ' ' << *p_infixOperator << ' '
			<< object::c_objectTypeToString.at(p_rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateCharacterInfixExpression(std::shared_ptr<object::Character> p_leftObject, std::string* p_infixOperator, std::shared_ptr<object::Character> p_rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*p_infixOperator == "==") return object::getBoolean(p_leftObject->m_value == p_rightObject->m_value);
		if (*p_infixOperator == "!=") return object::getBoolean(p_leftObject->m_value != p_rightObject->m_value);

		std::ostringstream error;
		error << "'" << object::c_objectTypeToString.at(p_leftObject->Type())
			<< ' ' << *p_infixOperator << ' '
			<< object::c_objectTypeToString.at(p_rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> collectionValueReassignment(std::shared_ptr<object::Collection> p_collection, std::shared_ptr<object::Object> p_indexObject, std::shared_ptr<object::Object> p_valueObject)
	{
		if (p_indexObject->Type() != object::INTEGER)
		{
			std::ostringstream error;
			error << "Invalid index: '" << p_indexObject->Inspect() << "'";
			return createError(error.str());
		}

		std::shared_ptr<object::Integer> index = std::static_pointer_cast<object::Integer>(p_indexObject);

		if (index->m_value < 0)
		{
			std::ostringstream error;
			error << "Invalid index: '" << index->Inspect() << "'";
			return createError(error.str());
		}

		if (index->m_value >= (p_collection->m_values.size())) return createError("Index out of bounds.");

		if (p_valueObject->Type() != p_collection->m_collectionType)
		{
			std::ostringstream error;
			error << "'The collection has values of type '" << object::c_objectTypeToString.at(p_collection->m_collectionType)
				<< "'. Got value of type '" << object::c_objectTypeToString.at(p_valueObject->Type()) << "'.";
			return createError(error.str());
		}

		p_collection->m_values[index->m_value] = p_valueObject;
		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> dictionaryValueReassignment(std::shared_ptr<object::Dictionary> p_dictionary, std::shared_ptr<object::Object> p_keyObject, std::shared_ptr<object::Object> p_valueObject)
	{
		if (p_keyObject->Type() != p_dictionary->m_keyType)
		{
			std::ostringstream error;
			error << "Dictionary has keys of type: '" <<
				object::c_objectTypeToString.at(p_dictionary->m_keyType) <<
				"'. Got type: '" << object::c_objectTypeToString.at(p_keyObject->Type()) << "'";
			return createError(error.str());
		}

		p_dictionary->m_map[p_keyObject] = p_valueObject;
		return object::NULL_OBJECT;
	}


	std::shared_ptr<object::Object> evaluateCallExpression(std::shared_ptr<ast::CallExpression> p_callExpression, std::shared_ptr<object::Environment> p_environment)
	{
		std::shared_ptr<object::Object> expression = evaluate(p_callExpression->m_function, p_environment);
		if (expression->Type() == object::ERROR)
		{
			return expression;
		}

		std::vector<std::shared_ptr<object::Object>> evaluatedArguments;
		evaluateExpressions(&p_callExpression->m_parameters, &evaluatedArguments, p_environment);

		if (evaluatedArguments.size() == 1 && evaluatedArguments[0]->Type() == object::ERROR)
		{
			return evaluatedArguments[0];
		}

		if(expression->Type() == object::FUNCTION)
		{ 
			std::shared_ptr<object::Function> function = std::static_pointer_cast<object::Function>(expression);

			if (p_callExpression->m_parameters.size() != function->m_parameters.size())
			{
				std::ostringstream error;
				error << "'" << function->m_functionName.String() << "' was supplied with "
					<< p_callExpression->m_parameters.size() << " argument(s) instead of "
					<< function->m_parameters.size() << ".";
				return createError(error.str());
			}

			for (int i = 0; i < evaluatedArguments.size(); i++)
			{
				if (evaluatedArguments[i]->Type() != object::c_nodeTypeToObjectType.at(function->m_parameters[i]->m_token.m_type))
				{
					std::ostringstream error;
					error << "Parameter '" << function->m_parameters[i]->m_name.m_name << "' was supplied with a value of type '"
						<< object::c_objectTypeToString.at(evaluatedArguments[i]->Type()) << "' instead of type '"
						<< function->m_parameters[i]->m_token.m_literal << "' for the function call for '"
						<< function->m_functionName.String() << "'.";
					return createError(error.str());
				}
			}
		}
		else if (expression->Type() == object::BUILTIN_FUNCTION) 
		{
			// do nothing, checks handled by function itself
		}
		else
		{
			std::ostringstream error;
			error << "'" << p_callExpression->m_function->String() << "' is not a function.";
			return createError(error.str());
		}

		std::shared_ptr<object::Object> output = applyFunction(expression, &evaluatedArguments);
		if (output->Type() == object::ERROR) return output;

		if (expression->Type() == object::FUNCTION && output->Type() == object::NULL_TYPE)
		{
			std::ostringstream error;
			error << "'" << std::static_pointer_cast<object::Function>(expression)->m_functionName.String() << "' has no return value.";
			return createError(error.str());
		}

		if (expression->Type() == object::FUNCTION && output->Type() != std::static_pointer_cast<object::Function>(expression)->m_functionType)
		{
			std::ostringstream error;
			error << "'" << p_callExpression->String() << "\' produced a value of type '"
				<< object::c_objectTypeToString.at(output->Type()) << "' instead of type '"
				<< object::c_objectTypeToString.at(std::static_pointer_cast<object::Function>(expression)->m_functionType) << "'.";
			return createError(error.str());
		}

		return output;
	}

	std::shared_ptr<object::Object> evaluateIndexExpression(std::shared_ptr<ast::IndexExpression> p_indexExpression, std::shared_ptr<object::Environment> p_environment)
	{
		// Evaluate expression and apply index to it
		std::shared_ptr<object::Object> expression = evaluate(p_indexExpression->m_collection, p_environment);

		// Get index
		std::shared_ptr<object::Object> indexObject = evaluate(p_indexExpression->m_index, p_environment);
		if (indexObject->Type() == object::ERROR) return indexObject;

		if (expression->Type() != object::DICTIONARY && indexObject->Type() != object::INTEGER)
		{
			std::ostringstream error;
			error << "Invalid index: '" << indexObject->Inspect() << "'";
			return createError(error.str());
		}
		else if (expression->Type() == object::DICTIONARY &&
			(indexObject->Type() != std::static_pointer_cast<object::Dictionary>(expression)->m_keyType))
		{
			std::ostringstream error;
			error << "Dictionary has keys of type: '" << 
				object::c_objectTypeToString.at(std::static_pointer_cast<object::Dictionary>(expression)->m_keyType) <<
				"'. Got type: '" << object::c_objectTypeToString.at(indexObject->Type()) << "'";
			return createError(error.str());
		}


		switch (expression->Type())
		{
		case object::ERROR:
			return expression;
		case object::COLLECTION:
		{
			std::shared_ptr<object::Integer> index = std::static_pointer_cast<object::Integer>(indexObject);

			if (index->m_value < 0)
			{
				std::ostringstream error;
				error << "Invalid index: '" << index->Inspect() << "'";
				return createError(error.str());
			}

			if (index->m_value >= std::static_pointer_cast<object::Collection>(expression)->m_values.size()) return createError("Index out of bounds.");
			return std::static_pointer_cast<object::Collection>(expression)->m_values[index->m_value];
		}
		case object::STRING:
		{
			std::shared_ptr<object::Integer> index = std::static_pointer_cast<object::Integer>(indexObject);

			if (index->m_value < 0)
			{
				std::ostringstream error;
				error << "Invalid index: '" << index->Inspect() << "'";
				return createError(error.str());
			}

			if (index->m_value >= std::static_pointer_cast<object::String>(expression)->m_value.size()) return createError("Index out of bounds.");
			char value = std::static_pointer_cast<object::String>(expression)->m_value[index->m_value];
			return std::shared_ptr<object::Character>(new object::Character(value));
		}
		case object::DICTIONARY:
		{
			if (std::static_pointer_cast<object::Dictionary>(expression)->m_map.find(indexObject) == std::static_pointer_cast<object::Dictionary>(expression)->m_map.end()) return createError("Index not in dictionary.");
			return std::static_pointer_cast<object::Dictionary>(expression)->m_map.at(indexObject);
		}
		}

		// Error for unsupported types.
		std::ostringstream error;
		error << "'" << expression->Inspect() << "' is not an indexable value.";
		return createError(error.str());
	}


	std::shared_ptr<object::Object> evaluateDeclareVariable(std::shared_ptr<ast::DeclareVariableStatement> p_declareVariable, std::shared_ptr<object::Environment> p_environment)
	{
		if (p_environment->getLocalIdentifier(&p_declareVariable->m_name.m_name) != NULL)
		{
			std::ostringstream error;
			error << "Redefinition of '" << p_declareVariable->m_name.m_name << "'.";
			return createError(error.str());
		}

		std::shared_ptr<object::Object> object = evaluate(p_declareVariable->m_value, p_environment);

		if (object->Type() == object::ERROR)
		{
			return object;
		}

		if (p_declareVariable->m_token.m_literal != object::c_objectTypeToString.at(object->Type()))
		{
			std::ostringstream error;
			error << "'" << p_declareVariable->m_name.m_name
				<< "' is defined as type '" << p_declareVariable->m_token.m_literal
				<< "', not '" << (object::c_objectTypeToString.at(object->Type())) << "'.";
			return createError(error.str());
		}

		p_environment->setIdentifier(&p_declareVariable->m_name.m_name, object);

		return object::NULL_OBJECT;
	}


	std::shared_ptr<object::Object> evaluateDeclareCollection(std::shared_ptr<ast::DeclareCollectionStatement> p_declareCollection, std::shared_ptr<object::Environment> p_environment)
	{
		if (p_environment->getLocalIdentifier(&p_declareCollection->m_name.m_name) != NULL)
		{
			std::ostringstream error;
			error << "Redefinition of '" << p_declareCollection->m_name.m_name << "'.";
			return createError(error.str());
		}

		std::shared_ptr<object::Object> object = evaluate(p_declareCollection->m_value, p_environment);

		if (object->Type() == object::ERROR)
		{
			return object;
		}

		if (p_declareCollection->m_token.m_literal != object::c_objectTypeToString.at(object->Type()))
		{
			std::ostringstream error;
			error << "'" << p_declareCollection->m_name.m_name
				<< "' is defined as type '" << p_declareCollection->m_token.m_literal
				<< "', not '" << (object::c_objectTypeToString.at(object->Type())) << "'.";
			return createError(error.str());
		}

		std::shared_ptr<object::Collection> collection = std::static_pointer_cast<object::Collection>(object);

		if (collection->m_collectionType != object::NULL_TYPE && p_declareCollection->m_typeToken.m_literal != object::c_objectTypeToString.at(collection->m_collectionType))
		{
			std::ostringstream error;
			error << "'" << p_declareCollection->m_name.m_name
				<< "' is a collection of '" << p_declareCollection->m_typeToken.m_literal
				<< "'s, but got a collection of type '" << object::c_objectTypeToString.at(collection->m_collectionType) << "'s.";
			return createError(error.str());
		}

		p_environment->setIdentifier(&p_declareCollection->m_name.m_name, object);

		return object::NULL_OBJECT;
	}


	std::shared_ptr<object::Object> evaluateDeclareDictionary(std::shared_ptr<ast::DeclareDictionaryStatement> p_declareDictionary, std::shared_ptr<object::Environment> p_environment)
	{
		if (p_environment->getLocalIdentifier(&p_declareDictionary->m_name.m_name) != NULL)
		{
			std::ostringstream error;
			error << "Redefinition of '" << p_declareDictionary->m_name.m_name << "'.";
			return createError(error.str());
		}

		std::shared_ptr<object::Object> object = evaluate(p_declareDictionary->m_value, p_environment);

		if (object->Type() == object::ERROR)
		{
			return object;
		}

		if (p_declareDictionary->m_token.m_literal != object::c_objectTypeToString.at(object->Type()))
		{
			std::ostringstream error;
			error << "'" << p_declareDictionary->m_name.m_name
				<< "' is defined as type '" << p_declareDictionary->m_token.m_literal
				<< "', not '" << (object::c_objectTypeToString.at(object->Type())) << "'.";
			return createError(error.str());
		}

		std::shared_ptr<object::Dictionary> dictionary = std::static_pointer_cast<object::Dictionary>(object);

		if (dictionary->m_keyType != object::NULL_TYPE &&
			(p_declareDictionary->m_keyTypeToken.m_literal != object::c_objectTypeToString.at(dictionary->m_keyType) ||
				p_declareDictionary->m_valueTypeToken.m_literal != object::c_objectTypeToString.at(dictionary->m_valueType)))
		{
			std::ostringstream error;
			error << "'" << p_declareDictionary->m_name.m_name
				<< "' is a dictionary of <" << p_declareDictionary->m_keyTypeToken.m_literal << ", " << p_declareDictionary->m_valueTypeToken.m_literal
				<< "> pairs, but got a dictionary of type <"
				<< object::c_objectTypeToString.at(dictionary->m_keyType) << ", " << object::c_objectTypeToString.at(dictionary->m_valueType) << "> pairs.";
			return createError(error.str());
		}

		p_environment->setIdentifier(&p_declareDictionary->m_name.m_name, object);

		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> evaluateDeclareFunction(std::shared_ptr<ast::DeclareFunctionStatement> p_declareFunction, std::shared_ptr<object::Environment> p_environment)
	{
		if (object::c_nodeTypeToObjectType.count(p_declareFunction->m_token.m_type) == 0)
		{
			std::ostringstream error;
			error << "'" << p_declareFunction->m_token.m_literal
				<< "' is not a valid function type.";
			return createError(error.str());
		}

		object::ObjectType functionType = object::c_nodeTypeToObjectType.at(p_declareFunction->m_token.m_type);
		std::shared_ptr<object::Function> result(new object::Function(functionType, p_declareFunction, p_environment));

		p_environment->setIdentifier(&p_declareFunction->m_name.m_name, result);

		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> evaluateReturnStatement(std::shared_ptr<ast::ReturnStatement> p_returnStatement, std::shared_ptr<object::Environment> p_environment)
	{
		return std::shared_ptr<object::Object>(new object::Return(evaluate(p_returnStatement->m_returnValue, p_environment)));
	}

	std::shared_ptr<object::Object> evaluateIfStatement(std::shared_ptr<ast::IfStatement> p_ifStatement, std::shared_ptr<object::Environment> p_environment)
	{
		// Treat as else caluse
		if (p_ifStatement->m_condition == NULL)
		{
			std::shared_ptr<object::Environment> ifEnvironment(new object::Environment(p_environment));
			return evaluate(p_ifStatement->m_consequence, ifEnvironment);
		}

		std::shared_ptr<object::Object> evaluatedCondition = evaluate(p_ifStatement->m_condition, p_environment);

		if (evaluatedCondition->Type() == object::ERROR)
		{
			return evaluatedCondition;
		}

		std::shared_ptr<object::Object> truthy = isTruthy(evaluatedCondition);

		if (truthy->Type() == object::ERROR)
		{
			return truthy;
		}

		std::shared_ptr<object::Boolean> truthyBoolean = std::static_pointer_cast<object::Boolean>(truthy);
		std::shared_ptr<object::Environment> ifEnvironment(new object::Environment(p_environment));

		if (truthyBoolean->m_value)
		{
			return evaluate(p_ifStatement->m_consequence, ifEnvironment);
		}
		else if (p_ifStatement->m_alternative != NULL)
		{
			return evaluate(p_ifStatement->m_alternative, ifEnvironment);
		}
		else
		{
			return object::NULL_OBJECT;
		}
	}
	
	std::shared_ptr<object::Object> evaluateWhileStatement(std::shared_ptr<ast::WhileStatement> p_whileStatement, std::shared_ptr<object::Environment> p_environment)
	{
		std::shared_ptr<object::Environment> whileEnvironment(new object::Environment(p_environment));

		while (true)
		{
			std::shared_ptr<object::Object> evaluatedCondition = evaluate(p_whileStatement->m_condition, p_environment);
			if (evaluatedCondition->Type() == object::ERROR)
			{
				return evaluatedCondition;
			}

			std::shared_ptr<object::Object> truthy = isTruthy(evaluatedCondition);
			if (truthy->Type() == object::ERROR)
			{
				return truthy;
			}

			std::shared_ptr<object::Boolean> truthyBoolean = std::static_pointer_cast<object::Boolean>(truthy);
			if (!truthyBoolean->m_value) break;

			std::shared_ptr<object::Object> evaluatedConsequence = evaluate(p_whileStatement->m_consequence, whileEnvironment);
			if (evaluatedConsequence->Type() == object::ERROR)
			{
				return evaluatedConsequence;
			}
		}

		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> evaluateDoWhileStatement(std::shared_ptr<ast::DoWhileStatement> p_doWhileStatement, std::shared_ptr<object::Environment> p_environment)
	{
		std::shared_ptr<object::Environment> doWhileEnvironment(new object::Environment(p_environment));

		std::shared_ptr<object::Object> evaluatedConsequence = evaluate(p_doWhileStatement->m_consequence, doWhileEnvironment);
		if (evaluatedConsequence->Type() == object::ERROR)
		{
			return evaluatedConsequence;
		}

		while (true)
		{
			std::shared_ptr<object::Object> evaluatedCondition = evaluate(p_doWhileStatement->m_condition, p_environment);
			if (evaluatedCondition->Type() == object::ERROR)
			{
				return evaluatedCondition;
			}

			std::shared_ptr<object::Object> truthy = isTruthy(evaluatedCondition);
			if (truthy->Type() == object::ERROR)
			{
				return truthy;
			}

			std::shared_ptr<object::Boolean> truthyBoolean = std::static_pointer_cast<object::Boolean>(truthy);
			if (!truthyBoolean->m_value) break;

			std::shared_ptr<object::Object> evaluatedConsequence = evaluate(p_doWhileStatement->m_consequence, doWhileEnvironment);
			if (evaluatedConsequence->Type() == object::ERROR)
			{
				return evaluatedConsequence;
			}
		}

		return object::NULL_OBJECT;
	}

	// Evaluates an while statement
	std::shared_ptr<object::Object> evaluateForStatement(std::shared_ptr<ast::ForStatement> p_forStatement, std::shared_ptr<object::Environment> p_environment)
	{
		std::shared_ptr<object::Environment> forConditionEnvironment(new object::Environment(p_environment));

		std::shared_ptr<object::Object> evaluatedInitialization = evaluate(p_forStatement->m_initialization, forConditionEnvironment);
		if (evaluatedInitialization->Type() == object::ERROR)
		{
			return evaluatedInitialization;
		}

		while (true)
		{
			std::shared_ptr<object::Environment> forEnvironment(new object::Environment(forConditionEnvironment));
			std::shared_ptr<object::Object> evaluatedCondition = evaluate(p_forStatement->m_condition, forConditionEnvironment);
			if (evaluatedCondition->Type() == object::ERROR)
			{
				return evaluatedCondition;
			}

			std::shared_ptr<object::Object> truthy = isTruthy(evaluatedCondition);
			if (truthy->Type() == object::ERROR)
			{
				return truthy;
			}

			std::shared_ptr<object::Boolean> truthyBoolean = std::static_pointer_cast<object::Boolean>(truthy);
			if (!truthyBoolean->m_value) break;

			std::shared_ptr<object::Object> evaluatedConsequence = evaluate(p_forStatement->m_consequence, forEnvironment);
			if (evaluatedConsequence->Type() == object::ERROR)
			{
				return evaluatedConsequence;
			}

			std::shared_ptr<object::Object> evaluatedUpdation = evaluate(p_forStatement->m_updation, forConditionEnvironment);
			if (evaluatedUpdation->Type() == object::ERROR)
			{
				return evaluatedUpdation;
			}
		}

		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> evaluateIterateStatement(std::shared_ptr<ast::IterateStatement> p_iterateStatement, std::shared_ptr<object::Environment> p_environment)
	{
		std::shared_ptr<object::Environment> iterateEnvironment(new object::Environment(p_environment));

		std::shared_ptr<object::Object> evaluatedCollection = evaluate(p_iterateStatement->m_collection, p_environment);
		if (evaluatedCollection->Type() == object::ERROR)
		{
			return evaluatedCollection;
		}

		if (evaluatedCollection->Type() != object::COLLECTION)
		{
			std::ostringstream error;
			error << "Expected to see a collection to iterate over. Instead got a(n) '"
				<< object::c_objectTypeToString.at(evaluatedCollection->Type()) << "'.";
			return createError(error.str());
		}
		std::shared_ptr<object::Collection> collection = std::static_pointer_cast<object::Collection>(evaluatedCollection);

		for (std::shared_ptr<object::Object> value : collection->m_values)
		{
			iterateEnvironment->setIdentifier(&p_iterateStatement->m_var->m_name, value);

			std::shared_ptr<object::Object> evaluatedConsequence = evaluate(p_iterateStatement->m_consequence, iterateEnvironment);
			if (evaluatedConsequence->Type() == object::ERROR) return evaluatedConsequence;
		}

		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> applyFunction(std::shared_ptr<object::Object> p_function, std::vector<std::shared_ptr<object::Object>>* p_arguments)
	{
		switch (p_function->Type())
		{
		case object::BUILTIN_FUNCTION:
		{
			std::shared_ptr<object::Object> evaluated = std::static_pointer_cast<object::Builtin>(p_function)->m_function(p_arguments);
			return unwrapReturnValue(evaluated);
		}
		case object::FUNCTION:
		{
			std::shared_ptr<object::Environment> extendedEnvironment = extendFunctionEnvironment(std::static_pointer_cast<object::Function>(p_function), p_arguments);
			std::shared_ptr<object::Object> evaluated = evaluate(std::static_pointer_cast<object::Function>(p_function)->m_body, extendedEnvironment);
			return unwrapReturnValue(evaluated);
		}
		}

		// Error for unsupported types.
		std::ostringstream error;
		error << "'" << p_function->Inspect() << "' is not a callable function.";
		return createError(error.str());
	}

	std::shared_ptr<object::Environment> extendFunctionEnvironment(std::shared_ptr<object::Function> p_function, std::vector<std::shared_ptr<object::Object>>* p_arguments)
	{
		std::shared_ptr<object::Environment> newEnvironment(new object::Environment(p_function->m_environment));

		for (int i = 0; i < p_arguments->size(); i++)
		{
			newEnvironment->setIdentifier(&p_function->m_parameters[i]->m_name.m_name, (*p_arguments)[i]);
		}

		return newEnvironment;
	}

	std::shared_ptr<object::Object> unwrapReturnValue(std::shared_ptr<object::Object> p_object)
	{
		if (p_object->Type() == object::RETURN)
		{
			std::shared_ptr<object::Return> returnObject = std::static_pointer_cast<object::Return>(p_object);
			return returnObject->m_returnValue;
		}

		return p_object;
	}

	std::shared_ptr<object::Object> isTruthy(std::shared_ptr<object::Object> p_object)
	{
		switch (p_object->Type())
		{
		case object::BOOLEAN:
		{
			std::shared_ptr<object::Boolean> booleanValue = std::static_pointer_cast<object::Boolean>(p_object);
			if (booleanValue->m_value) return object::TRUE_OBJECT;
			else return object::FALSE_OBJECT;
		}
		case object::INTEGER:
		{
			std::shared_ptr<object::Integer> integerValue = std::static_pointer_cast<object::Integer>(p_object);
			if (integerValue->m_value != 0) return object::TRUE_OBJECT;
			else return object::FALSE_OBJECT;
		}
		case object::FLOAT:
		{
			std::shared_ptr<object::Float> floatValue = std::static_pointer_cast<object::Float>(p_object);
			if (floatValue->m_value != 0) return object::TRUE_OBJECT;
			else return object::FALSE_OBJECT;
		}
		default:
		{
			std::ostringstream error;
			error << "'" << p_object->Inspect() << "' is not a valid truthy value.";
			return createError(error.str());
		}
		}
	}


	std::shared_ptr<object::Error> createError(std::string errorMessage)
	{
		return std::shared_ptr<object::Error>(new object::Error(errorMessage));
	}

}
