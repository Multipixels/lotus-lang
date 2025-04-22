#include <set>
#include <sstream>

#include "builtinFunctions.h"
#include "evaluator.h"

namespace evaluator
{
	std::shared_ptr<object::Object> evaluate(std::shared_ptr<ast::Node> node, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout)
	{
		if (timeout != std::chrono::steady_clock::time_point() && timeout < std::chrono::steady_clock::now())
		{
			std::ostringstream error;
			error << "Evaluation of the program timed out.";
			return createError(error.str());
		}

		if (node == NULL) return object::NULL_OBJECT;

		switch (node->Type())
		{
		case ast::PROGRAM_NODE:
		{
			std::shared_ptr<object::Object> test = evaluateProgram(std::static_pointer_cast<ast::Program>(node), environment, timeout);
			return test;
		}
		case ast::IDENTIFIER_NODE:
		{
			return evaluateIdentifier(std::static_pointer_cast<ast::Identifier>(node), environment);
		}
		case ast::BLOCK_STATEMENT_NODE:
		{
			return evaluateBlockStatement(std::static_pointer_cast<ast::BlockStatement>(node), environment, timeout);
		}
		case ast::INTEGER_LITERAL_NODE:
		{
			std::shared_ptr<object::Integer> object(new object::Integer);
			object->m_value = std::static_pointer_cast<ast::IntegerLiteral>(node)->m_value;
			return object;
		}
		case ast::FLOAT_LITERAL_NODE:
		{
			std::shared_ptr<object::Float> object(new object::Float);
			object->m_value = std::static_pointer_cast<ast::FloatLiteral>(node)->m_value;
			return object;
		}
		case ast::BOOLEAN_LITERAL_NODE:
		{
			std::shared_ptr<object::Boolean> object(new object::Boolean);
			object->m_value = std::static_pointer_cast<ast::BooleanLiteral>(node)->m_value;
			return object;
		}
		case ast::CHARACTER_LITERAL_NODE:
		{
			std::shared_ptr<object::Character> object(new object::Character);
			object->m_value = std::static_pointer_cast<ast::CharacterLiteral>(node)->m_value;
			return object;
		}
		case ast::COLLECTION_LITERAL_NODE:
		{
			std::shared_ptr<ast::CollectionLiteral> collectionLiteral = std::static_pointer_cast<ast::CollectionLiteral>(node);

			if (collectionLiteral->m_values.size() == 0)
			{
				return std::shared_ptr<object::Collection>(new object::Collection(object::NULL_TYPE, {}));
			}

			std::shared_ptr<object::Collection> object(new object::Collection);

			for (int i = 0; i < collectionLiteral->m_values.size(); i++)
			{
				std::shared_ptr<object::Object> evaluatedItem = evaluate(collectionLiteral->m_values[i], environment, timeout);

				if (evaluatedItem->Type() == object::ERROR) return evaluatedItem;

				if (object->m_collection_type != object::NULL_TYPE && evaluatedItem->Type() != object->m_collection_type)
				{
					std::ostringstream error;
					error << "The collection " << node->String() << " must have uniform typing of elements.";
					return createError(error.str());
				}

				if (object->m_collection_type == object::NULL_TYPE) object->m_collection_type = evaluatedItem->Type();
				object->m_values.push_back(evaluatedItem);
			}

			return object;
		}
		case ast::DICTIONARY_LITERAL_NODE:
		{
			std::shared_ptr<ast::DictionaryLiteral> dictionaryLiteral = std::static_pointer_cast<ast::DictionaryLiteral>(node);

			if (dictionaryLiteral->m_map.size() == 0)
			{
				return std::shared_ptr<object::Dictionary>(new object::Dictionary);
			}

			std::shared_ptr<object::Dictionary> object(new object::Dictionary);
		
			std::map < std::shared_ptr<ast::Expression>, std::shared_ptr<ast::Expression>> ::iterator it;
			for (it = dictionaryLiteral->m_map.begin(); it != dictionaryLiteral->m_map.end(); it++)
			{
				// Checking the key
				std::shared_ptr<object::Object> evaluatedKey = evaluate(it->first, environment, timeout);
				if (evaluatedKey->Type() == object::ERROR) return evaluatedKey;

				if (evaluatedKey->Type() != object::INTEGER && evaluatedKey->Type() != object::FLOAT &&
					evaluatedKey->Type() != object::BOOLEAN && evaluatedKey->Type() != object::CHARACTER)
				{
					std::ostringstream error;
					error << "Invalid dictionary key type. " << 
						object::objectTypeToString.at(evaluatedKey->Type()) << " is not a hashable type.";
					return createError(error.str());
				}

				if (object->m_key_type != object::NULL_TYPE && evaluatedKey->Type() != object->m_key_type)
				{
					std::ostringstream error;
					error << "Dictionary has mismatching key types.";
					return createError(error.str());
				}

				if (object->m_key_type == object::NULL_TYPE) object->m_key_type = evaluatedKey->Type();
				if (object->m_map.find(evaluatedKey) != object->m_map.end())
				{
					std::ostringstream error;
					error << "Dictionary initialized with duplicate key.";
					return createError(error.str());
				}

				// Checking the value
				std::shared_ptr<object::Object> evaluatedValue = evaluate(it->second, environment, timeout);
				if (evaluatedValue->Type() == object::ERROR) return evaluatedValue;

				if (object->m_value_type != object::NULL_TYPE && evaluatedValue->Type() != object->m_value_type)
				{
					std::ostringstream error;
					error << "Dictionary has mismatching value types.";
					return createError(error.str());
				}

				if (object->m_value_type == object::NULL_TYPE) object->m_value_type = evaluatedValue->Type();

				// Storing key value pair
				object->m_map.emplace(evaluatedKey, evaluatedValue);
			}

			return object;
		}
		case ast::STRING_LITERAL_NODE:
		{
			std::stringstream value;

			for (int i = 0; i < (std::static_pointer_cast<ast::StringLiteral>(node))->m_stringCollection->m_values.size(); i++)
			{
				// Take expressoin from string collection and cast to character literal pointer
				std::shared_ptr<ast::CharacterLiteral> characterLiteral = std::static_pointer_cast<ast::CharacterLiteral>((std::static_pointer_cast<ast::StringLiteral>(node))->m_stringCollection->m_values[i]);
				value << characterLiteral->m_value;
			}

			std::string valueStr = value.str();
			std::shared_ptr<object::String> object(new object::String(&valueStr));
			return object;
		}
		case ast::PREFIX_EXPRESSION_NODE:
		{
			std::shared_ptr<ast::PrefixExpression> prefixExpression = std::static_pointer_cast<ast::PrefixExpression>(node);

			std::shared_ptr<object::Object> rightObject = evaluate(prefixExpression->m_right_expression, environment, timeout);
			if (rightObject->Type() == object::ERROR) return rightObject;

			return evaluatePrefixExpression(&prefixExpression->m_operator, rightObject);
		}
		case ast::INFIX_EXPRESSION_NODE:
		{
			std::shared_ptr<ast::InfixExpression> infixExpression = std::static_pointer_cast<ast::InfixExpression>(node);

			if (infixExpression->m_left_expression->Type() == ast::IDENTIFIER_NODE && infixExpression->m_operator == "=")
			{
				std::shared_ptr<ast::Identifier> identifier = std::static_pointer_cast<ast::Identifier>(infixExpression->m_left_expression);
				std::shared_ptr<object::Object> savedValue = environment->getIdentifier(&identifier->m_name);
				if (savedValue == NULL)
				{
					std::ostringstream error;
					error << "'" << identifier->m_name << "' is not defined.";
					return createError(error.str());
				}

				std::shared_ptr<object::Object> rightObject = evaluate(infixExpression->m_right_expression, environment, timeout);

				if (rightObject->Type() == object::ERROR) return rightObject;

				if (savedValue->Type() != rightObject->Type())
				{
					std::ostringstream error;
					error << "Cannot assign '" << identifier->m_name << "' of type '"
						<< object::objectTypeToString.at(savedValue->Type()) << "' a value of type '"
						<< object::objectTypeToString.at(rightObject->Type()) << "'.";
					return createError(error.str());
				}

				environment->reassignIdentifier(&identifier->m_name, rightObject);

				return object::NULL_OBJECT;
			}
			else if (infixExpression->m_left_expression->Type() == ast::INDEX_EXPRESSION_NODE && infixExpression->m_operator == "=")
			{
				std::shared_ptr<ast::IndexExpression> indexExpression = std::static_pointer_cast<ast::IndexExpression>(infixExpression->m_left_expression);
				
				std::shared_ptr<object::Object> object = evaluate(indexExpression->m_collection, environment, timeout);
				if (object->Type() == object::ERROR) return object;

				std::shared_ptr<object::Object> indexObject = evaluate(indexExpression->m_index, environment, timeout);
				if (indexObject->Type() == object::ERROR) return indexObject;

				std::shared_ptr<object::Object> valueObject = evaluate(infixExpression->m_right_expression, environment, timeout);
				if (valueObject->Type() == object::ERROR) return valueObject;

				switch (object->Type()) {
				case object::COLLECTION:
				{
					return collectionValueReassignment(std::static_pointer_cast<object::Collection>(object), indexObject, valueObject);
				}
				case object::DICTIONARY:
				{
					return dictionaryValueReassignment(std::static_pointer_cast<object::Dictionary>(object), indexObject, valueObject);
				}
				case object::STRING:
				{
					std::ostringstream error;
					error << "Strings are immutable.";
					return createError(error.str());
				}
				default:
				{
					std::ostringstream error;
					error << "This should be unreachable.";
					return createError(error.str());
				}
				}
			}
			else
			{
				std::shared_ptr<object::Object> leftObject = evaluate(infixExpression->m_left_expression, environment, timeout);
				if (leftObject->Type() == object::ERROR) return leftObject;

				std::shared_ptr<object::Object> rightObject = evaluate(infixExpression->m_right_expression, environment, timeout);
				if (rightObject->Type() == object::ERROR) return rightObject;

				return evaluateInfixExpression(leftObject, &infixExpression->m_operator, rightObject);
			}
		}
		case ast::CALL_EXPRESSION_NODE:
		{
			std::shared_ptr<ast::CallExpression> callExpression = std::static_pointer_cast<ast::CallExpression>(node);
			return evaluateCallExpression(callExpression, environment, timeout);
		}
		case ast::INDEX_EXPRESSION_NODE:
		{
			std::shared_ptr<ast::IndexExpression> indexExpression = std::static_pointer_cast<ast::IndexExpression>(node);
			return evaluateIndexExpression(indexExpression, environment, timeout);
		}
		case ast::DECLARE_VARIABLE_STATEMENT_NODE:
		{
			std::shared_ptr<ast::DeclareVariableStatement> declareVariableStatement = std::static_pointer_cast<ast::DeclareVariableStatement>(node);

			if (environment->getLocalIdentifier(&declareVariableStatement->m_name.m_name) != NULL)
			{
				std::ostringstream error;
				error << "Redefinition of '" << declareVariableStatement->m_name.m_name << "'.";
				return createError(error.str());
			}

			std::shared_ptr<object::Object> object = evaluate(declareVariableStatement->m_value, environment, timeout);

			if (object->Type() == object::ERROR)
			{
				return object;
			}

			if (declareVariableStatement->m_token.m_literal != object::objectTypeToString.at(object->Type()))
			{
				std::ostringstream error;
				error << "'" << declareVariableStatement->m_name.m_name
					<< "' is defined as type '" << declareVariableStatement->m_token.m_literal
					<< "', not '" << (object::objectTypeToString.at(object->Type())) << "'.";
				return createError(error.str());
			}

			environment->setIdentifier(&declareVariableStatement->m_name.m_name, object);

			return object::NULL_OBJECT;
		}
		case ast::DECLARE_COLLECTION_STATEMENT_NODE:
		{
			std::shared_ptr<ast::DeclareCollectionStatement> declareCollectionStatement = std::static_pointer_cast<ast::DeclareCollectionStatement>(node);

			if (environment->getLocalIdentifier(&declareCollectionStatement->m_name.m_name) != NULL)
			{
				std::ostringstream error;
				error << "Redefinition of '" << declareCollectionStatement->m_name.m_name << "'.";
				return createError(error.str());
			}

			std::shared_ptr<object::Object> object = evaluate(declareCollectionStatement->m_value, environment, timeout);

			if (object->Type() == object::ERROR)
			{
				return object;
			}

			if (declareCollectionStatement->m_token.m_literal != object::objectTypeToString.at(object->Type()))
			{
				std::ostringstream error;
				error << "'" << declareCollectionStatement->m_name.m_name
					<< "' is defined as type '" << declareCollectionStatement->m_token.m_literal
					<< "', not '" << (object::objectTypeToString.at(object->Type())) << "'.";
				return createError(error.str());
			}

			std::shared_ptr<object::Collection> collection = std::static_pointer_cast<object::Collection>(object);

			if (collection->m_collection_type != object::NULL_TYPE && declareCollectionStatement->m_typeToken.m_literal != object::objectTypeToString.at(collection->m_collection_type))
			{
				std::ostringstream error;
				error << "'" << declareCollectionStatement->m_name.m_name
					<< "' is a collection of '" << declareCollectionStatement->m_typeToken.m_literal
					<< "'s, but got a collection of type '" << object::objectTypeToString.at(collection->m_collection_type) << "'s.";
				return createError(error.str());
			}

			environment->setIdentifier(&declareCollectionStatement->m_name.m_name, object);

			return object::NULL_OBJECT;
		}
		case ast::DECLARE_DICTIONARY_STATEMENT_NODE:
		{
			std::shared_ptr<ast::DeclareDictionaryStatement> declareDictionaryStatement = std::static_pointer_cast<ast::DeclareDictionaryStatement>(node);

			if (environment->getLocalIdentifier(&declareDictionaryStatement->m_name.m_name) != NULL)
			{
				std::ostringstream error;
				error << "Redefinition of '" << declareDictionaryStatement->m_name.m_name << "'.";
				return createError(error.str());
			}

			std::shared_ptr<object::Object> object = evaluate(declareDictionaryStatement->m_value, environment, timeout);

			if (object->Type() == object::ERROR)
			{
				return object;
			}

			if (declareDictionaryStatement->m_token.m_literal != object::objectTypeToString.at(object->Type()))
			{
				std::ostringstream error;
				error << "'" << declareDictionaryStatement->m_name.m_name
					<< "' is defined as type '" << declareDictionaryStatement->m_token.m_literal
					<< "', not '" << (object::objectTypeToString.at(object->Type())) << "'.";
				return createError(error.str());
			}

			std::shared_ptr<object::Dictionary> dictionary = std::static_pointer_cast<object::Dictionary>(object);

			if (dictionary->m_key_type != object::NULL_TYPE && 
				(declareDictionaryStatement->m_keyTypeToken.m_literal != object::objectTypeToString.at(dictionary->m_key_type) || 
				 declareDictionaryStatement->m_valueTypeToken.m_literal != object::objectTypeToString.at(dictionary->m_value_type)))
			{
				std::ostringstream error;
				error << "'" << declareDictionaryStatement->m_name.m_name
					<< "' is a dictionary of <" << declareDictionaryStatement->m_keyTypeToken.m_literal << ", " << declareDictionaryStatement->m_valueTypeToken.m_literal
					<< "> pairs, but got a dictionary of type <" 
					<< object::objectTypeToString.at(dictionary->m_key_type) << ", " << object::objectTypeToString.at(dictionary->m_value_type) << "> pairs.";
				return createError(error.str());
			}

			environment->setIdentifier(&declareDictionaryStatement->m_name.m_name, object);

			return object::NULL_OBJECT;
		}
		case ast::DECLARE_FUNCTION_STATEMENT_NODE:
		{
			std::shared_ptr<ast::DeclareFunctionStatement> declareFunctionStatement = std::static_pointer_cast<ast::DeclareFunctionStatement>(node);

			if (object::nodeTypeToObjectType.count(declareFunctionStatement->m_token.m_type) == 0)
			{
				std::ostringstream error;
				error << "'" << declareFunctionStatement->m_token.m_literal
					<< "' is not a valid function type.";
				return createError(error.str());
			}

			object::ObjectType functionType = object::nodeTypeToObjectType.at(declareFunctionStatement->m_token.m_type);
			std::shared_ptr<object::Function> result(new object::Function(functionType, declareFunctionStatement, environment));

			environment->setIdentifier(&declareFunctionStatement->m_name.m_name, result);

			return object::NULL_OBJECT;
		}
		case ast::RETURN_STATEMENT_NODE:
		{
			std::shared_ptr<ast::ReturnStatement> returnStatement = std::static_pointer_cast<ast::ReturnStatement>(node);
			return std::shared_ptr<object::Object>(new object::Return(evaluate(returnStatement->m_returnValue, environment, timeout)));
		}
		case ast::EXPRESSION_STATEMENT_NODE:
			return evaluate(std::static_pointer_cast<ast::ExpressionStatement>(node)->m_expression, environment, timeout);
		case ast::IF_STATEMENT_NODE:
		{
			std::shared_ptr<ast::IfStatement> ifStatement = std::static_pointer_cast<ast::IfStatement>(node);

			// Treat as else caluse
			if (ifStatement->m_condition == NULL)
			{
				std::shared_ptr<object::Environment> ifEnvironment(new object::Environment(environment));
				return evaluate(ifStatement->m_consequence, ifEnvironment, timeout);
			}

			std::shared_ptr<object::Object> evaluatedCondition = evaluate(ifStatement->m_condition, environment, timeout);

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
			std::shared_ptr<object::Environment> ifEnvironment(new object::Environment(environment));

			if (truthyBoolean->m_value) return evaluate(ifStatement->m_consequence, ifEnvironment, timeout);
			else if (ifStatement->m_alternative != NULL) return evaluate(ifStatement->m_alternative, ifEnvironment, timeout);
			else return object::NULL_OBJECT;
		}
		case ast::WHILE_STATEMENT_NODE:
		{
			std::shared_ptr<ast::WhileStatement> whileStatement = std::static_pointer_cast<ast::WhileStatement>(node);
			std::shared_ptr<object::Environment> whileEnvironment(new object::Environment(environment));

			while (true)
			{
				std::shared_ptr<object::Object> evaluatedCondition = evaluate(whileStatement->m_condition, environment, timeout);
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

				std::shared_ptr<object::Object> evaluatedConsequence = evaluate(whileStatement->m_consequence, whileEnvironment, timeout);
				if (evaluatedConsequence->Type() == object::ERROR)
				{
					return evaluatedConsequence;
				}
			}

			return object::NULL_OBJECT;
		}
		case ast::DO_WHILE_STATEMENT_NODE:
		{
			std::shared_ptr<ast::DoWhileStatement> doWhileStatement = std::static_pointer_cast<ast::DoWhileStatement>(node);
			std::shared_ptr<object::Environment> doWhileEnvironment(new object::Environment(environment));

			std::shared_ptr<object::Object> evaluatedConsequence = evaluate(doWhileStatement->m_consequence, doWhileEnvironment, timeout);
			if (evaluatedConsequence->Type() == object::ERROR)
			{
				return evaluatedConsequence;
			}

			while (true)
			{
				std::shared_ptr<object::Object> evaluatedCondition = evaluate(doWhileStatement->m_condition, environment, timeout);
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

				std::shared_ptr<object::Object> evaluatedConsequence = evaluate(doWhileStatement->m_consequence, doWhileEnvironment, timeout);
				if (evaluatedConsequence->Type() == object::ERROR)
				{
					return evaluatedConsequence;
				}
			}

			return object::NULL_OBJECT;
		}
		case ast::FOR_STATEMENT_NODE:
		{
			std::shared_ptr<ast::ForStatement> forStatement = std::static_pointer_cast<ast::ForStatement>(node);
			std::shared_ptr<object::Environment> forConditionEnvironment(new object::Environment(environment));
			std::shared_ptr<object::Environment> forEnvironment(new object::Environment(forConditionEnvironment));

			std::shared_ptr<object::Object> evaluatedInitialization = evaluate(forStatement->m_initialization, forConditionEnvironment, timeout);
			if (evaluatedInitialization->Type() == object::ERROR)
			{
				return evaluatedInitialization;
			}

			while (true)
			{
				std::shared_ptr<object::Object> evaluatedCondition = evaluate(forStatement->m_condition, forConditionEnvironment, timeout);
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

				std::shared_ptr<object::Object> evaluatedConsequence = evaluate(forStatement->m_consequence, forEnvironment, timeout);
				if (evaluatedConsequence->Type() == object::ERROR)
				{
					return evaluatedConsequence;
				}

				std::shared_ptr<object::Object> evaluatedUpdation = evaluate(forStatement->m_updation, forConditionEnvironment, timeout);
				if (evaluatedUpdation->Type() == object::ERROR)
				{
					return evaluatedUpdation;
				}
			}

			return object::NULL_OBJECT;
		}
		case ast::ITERATE_STATEMENT_NODE:
		{
			std::shared_ptr<ast::IterateStatement> iterateStatement = std::static_pointer_cast<ast::IterateStatement>(node);
			std::shared_ptr<object::Environment> iterateEnvironment(new object::Environment(environment));

			std::shared_ptr<object::Object> evaluatedCollection = evaluate(iterateStatement->m_collection, environment, timeout);
			if (evaluatedCollection->Type() == object::ERROR) return evaluatedCollection;
			if (evaluatedCollection->Type() != object::COLLECTION)
			{
				std::ostringstream error;
				error << "Expected to see a collection to iterate over. Instead got a(n) '"
					<< object::objectTypeToString.at(evaluatedCollection->Type()) << "'.";
				return createError(error.str());
			}
			std::shared_ptr<object::Collection> collection = std::static_pointer_cast<object::Collection>(evaluatedCollection);

			for (std::shared_ptr<object::Object> value : collection->m_values)
			{
				iterateEnvironment->setIdentifier(&iterateStatement->m_var->m_name, value);

				std::shared_ptr<object::Object> evaluatedConsequence = evaluate(iterateStatement->m_consequence, iterateEnvironment, timeout);
				if (evaluatedConsequence->Type() == object::ERROR) return evaluatedConsequence;
			}

			return object::NULL_OBJECT;
		}
		}

		return NULL;
	}

	std::shared_ptr<object::Object> evaluateProgram(std::shared_ptr<ast::Program> program, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout)
	{
		std::shared_ptr<object::Object> result = object::NULL_OBJECT;

		for (int i = 0; i < program->m_statements.size(); i++)
		{
			result = evaluate(program->m_statements[i], environment, timeout);

			if (result != NULL && result->Type() == object::RETURN)
			{
				std::shared_ptr<object::Return> returnObj = std::static_pointer_cast<object::Return>(result);
				return returnObj->m_return_value;
			}

			if (result != NULL && result->Type() == object::ERROR)
			{
				return result;
			}
		}

		return result;
	}

	std::shared_ptr<object::Object> evaluateBlockStatement(std::shared_ptr<ast::BlockStatement> blockStatements, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout)
	{
		std::shared_ptr<object::Object> result = object::NULL_OBJECT;

		for (int i = 0; i < blockStatements->m_statements.size(); i++)
		{
			result = evaluate(blockStatements->m_statements[i], environment, timeout);

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


	void evaluateExpressions(std::vector<std::shared_ptr<ast::Expression>>* source, std::vector<std::shared_ptr<object::Object>>* destination, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout)
	{
		for (int i = 0; i < source->size(); i++)
		{
			std::shared_ptr<object::Object> evaluatedExpression = evaluate((*source)[i], environment, timeout);

			if (evaluatedExpression->Type() == object::ERROR)
			{
				(*destination).clear();
				(*destination).push_back(evaluatedExpression);
				return;
			}
			(*destination).push_back(evaluatedExpression);
		}
	}

	std::shared_ptr<object::Object> evaluateIdentifier(std::shared_ptr<ast::Identifier> identifier, std::shared_ptr<object::Environment> environment) {
		std::shared_ptr<object::Object> result = environment->getIdentifier(&(identifier->m_name));
		if (result != NULL)
		{
			return result;
		}

		if (builtins.find(identifier->m_name) != builtins.end()) {
			return builtins.at(identifier->m_name);
		}

		std::ostringstream error;
		error << "'" << identifier->m_name << "' is not defined.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluatePrefixExpression(std::string* prefixOperator, std::shared_ptr<object::Object> rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*prefixOperator == "!") return evaluateBangOperatorExpression(rightObject);
		if (*prefixOperator == "-") return evaluateMinusPrefixOperatorExpression(rightObject);

		std::ostringstream error;
		error << *prefixOperator << object::objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateInfixExpression(std::shared_ptr<object::Object> leftObject, std::string* infixOperator, std::shared_ptr<object::Object> rightObject)
	{
		switch (leftObject->Type())
		{
		case object::INTEGER:
		{
			if (rightObject->Type() == object::INTEGER)
				return evaluateIntegerInfixExpression(
					std::static_pointer_cast<object::Integer>(leftObject), infixOperator, std::static_pointer_cast<object::Integer>(rightObject));
			if (rightObject->Type() == object::FLOAT)
				return evaluateFloatInfixExpression(
					std::shared_ptr<object::Float>(new object::Float(std::static_pointer_cast<object::Integer>(leftObject)->m_value)), infixOperator, std::static_pointer_cast<object::Float>(rightObject));
			break;
		}
		case object::FLOAT:
		{
			if (rightObject->Type() == object::INTEGER)
				return evaluateFloatInfixExpression(
					std::static_pointer_cast<object::Float>(leftObject), infixOperator, std::shared_ptr<object::Float>(new object::Float(std::static_pointer_cast<object::Integer>(rightObject)->m_value)));
			if (rightObject->Type() == object::FLOAT)
				return evaluateFloatInfixExpression(
					std::static_pointer_cast<object::Float>(leftObject), infixOperator, std::static_pointer_cast<object::Float>(rightObject));
			break;
		}
		case object::BOOLEAN:
		{
			if (rightObject->Type() == object::BOOLEAN)
				return evaluateBooleanInfixExpression(
					std::static_pointer_cast<object::Boolean>(leftObject), infixOperator, std::static_pointer_cast<object::Boolean>(rightObject));
			break;
		}
		case object::CHARACTER:
		{
			if (rightObject->Type() == object::CHARACTER)
				return evaluateCharacterInfixExpression(
					std::static_pointer_cast<object::Character>(leftObject), infixOperator, std::static_pointer_cast<object::Character>(rightObject));
			break;
		}
		}

		std::ostringstream error;
		error << "'" << object::objectTypeToString.at(leftObject->Type())
			<< ' ' << *infixOperator << ' '
			<< object::objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateIntegerInfixExpression(std::shared_ptr<object::Integer> leftObject, std::string* infixOperator, std::shared_ptr<object::Integer> rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*infixOperator == "+") return std::shared_ptr<object::Integer>(new object::Integer(leftObject->m_value + rightObject->m_value));
		if (*infixOperator == "-") return std::shared_ptr<object::Integer>(new object::Integer(leftObject->m_value - rightObject->m_value));
		if (*infixOperator == "*") return std::shared_ptr<object::Integer>(new object::Integer(leftObject->m_value * rightObject->m_value));
		if (*infixOperator == "/") return std::shared_ptr<object::Integer>(new object::Integer(leftObject->m_value / rightObject->m_value));
		if (*infixOperator == "<") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value < rightObject->m_value));
		if (*infixOperator == "<=") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value <= rightObject->m_value));
		if (*infixOperator == ">") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value > rightObject->m_value));
		if (*infixOperator == ">=") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value >= rightObject->m_value));
		if (*infixOperator == "==") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value == rightObject->m_value));
		if (*infixOperator == "!=") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value != rightObject->m_value));

		std::ostringstream error;
		error << "'" << object::objectTypeToString.at(leftObject->Type())
			<< ' ' << *infixOperator << ' '
			<< object::objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateFloatInfixExpression(std::shared_ptr<object::Float> leftObject, std::string* infixOperator, std::shared_ptr<object::Float> rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*infixOperator == "+")  return std::shared_ptr<object::Float>(new object::Float(leftObject->m_value + rightObject->m_value));
		if (*infixOperator == "-")  return std::shared_ptr<object::Float>(new object::Float(leftObject->m_value - rightObject->m_value));
		if (*infixOperator == "*")  return std::shared_ptr<object::Float>(new object::Float(leftObject->m_value * rightObject->m_value));
		if (*infixOperator == "/")  return std::shared_ptr<object::Float>(new object::Float(leftObject->m_value / rightObject->m_value));
		if (*infixOperator == "<")  return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value < rightObject->m_value));
		if (*infixOperator == "<=") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value <= rightObject->m_value));
		if (*infixOperator == ">")  return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value > rightObject->m_value));
		if (*infixOperator == ">=") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value >= rightObject->m_value));
		if (*infixOperator == "==") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value == rightObject->m_value));
		if (*infixOperator == "!=") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value != rightObject->m_value));

		std::ostringstream error;
		error << "'" << object::objectTypeToString.at(leftObject->Type())
			<< ' ' << *infixOperator << ' '
			<< object::objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateBooleanInfixExpression(std::shared_ptr<object::Boolean> leftObject, std::string* infixOperator, std::shared_ptr<object::Boolean> rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*infixOperator == "&&") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value && rightObject->m_value));
		if (*infixOperator == "||") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value || rightObject->m_value));
		if (*infixOperator == "==") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value == rightObject->m_value));
		if (*infixOperator == "!=") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value != rightObject->m_value));

		std::ostringstream error;
		error << "'" << object::objectTypeToString.at(leftObject->Type())
			<< ' ' << *infixOperator << ' '
			<< object::objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateCharacterInfixExpression(std::shared_ptr<object::Character> leftObject, std::string* infixOperator, std::shared_ptr<object::Character> rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*infixOperator == "==") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value == rightObject->m_value));
		if (*infixOperator == "!=") return std::shared_ptr<object::Boolean>(new object::Boolean(leftObject->m_value != rightObject->m_value));

		std::ostringstream error;
		error << "'" << object::objectTypeToString.at(leftObject->Type())
			<< ' ' << *infixOperator << ' '
			<< object::objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateBangOperatorExpression(std::shared_ptr<object::Object> expression)
	{
		switch (expression->Type())
		{
		case object::INTEGER:
		{
			std::shared_ptr<object::Integer> integer = std::static_pointer_cast<object::Integer>(expression);
			if (integer->m_value) return object::FALSE_OBJECT;
			else return object::TRUE_OBJECT;
		}
		case object::FLOAT:
		{
			std::shared_ptr<object::Float> floating = std::static_pointer_cast<object::Float>(expression);
			if (floating->m_value) return object::FALSE_OBJECT;
			else return object::TRUE_OBJECT;
		}
		case object::BOOLEAN:
		{
			std::shared_ptr<object::Boolean> boolean = std::static_pointer_cast<object::Boolean>(expression);
			if (boolean->m_value) return object::FALSE_OBJECT;
			else return object::TRUE_OBJECT;
		}
		}

		std::ostringstream error;
		error << "'!" << object::objectTypeToString.at(expression->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateMinusPrefixOperatorExpression(std::shared_ptr<object::Object> expression)
	{
		switch (expression->Type())
		{
		case object::INTEGER:
		{
			std::shared_ptr<object::Integer> integer = std::static_pointer_cast<object::Integer>(expression);
			std::shared_ptr<object::Integer> returnValue(new object::Integer(-integer->m_value));
			return returnValue;
		}
		case object::FLOAT:
		{
			std::shared_ptr<object::Float> floating = std::static_pointer_cast<object::Float>(expression);
			std::shared_ptr<object::Float> returnValue(new object::Float(-floating->m_value));
			return returnValue;
		}
		}

		std::ostringstream error;
		error << "'-" << object::objectTypeToString.at(expression->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	std::shared_ptr<object::Object> evaluateCallExpression(std::shared_ptr<ast::CallExpression> callExpression, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout)
	{
		std::shared_ptr<object::Object> expression = evaluate(callExpression->m_function, environment, timeout);
		if (expression->Type() == object::ERROR)
		{
			return expression;
		}

		std::vector<std::shared_ptr<object::Object>> evaluatedArguments;
		evaluateExpressions(&callExpression->m_parameters, &evaluatedArguments, environment, timeout);

		if (evaluatedArguments.size() == 1 && evaluatedArguments[0]->Type() == object::ERROR)
		{
			return evaluatedArguments[0];
		}

		if(expression->Type() == object::FUNCTION)
		{ 
			std::shared_ptr<object::Function> function = std::static_pointer_cast<object::Function>(expression);

			if (callExpression->m_parameters.size() != function->m_parameters.size())
			{
				std::ostringstream error;
				error << "'" << function->m_function_name->String() << "' was supplied with "
					<< callExpression->m_parameters.size() << " argument(s) instead of "
					<< function->m_parameters.size() << ".";
				return createError(error.str());
			}

			for (int i = 0; i < evaluatedArguments.size(); i++)
			{
				if (evaluatedArguments[i]->Type() != object::nodeTypeToObjectType.at(function->m_parameters[i]->m_token.m_type))
				{
					std::ostringstream error;
					error << "Parameter '" << function->m_parameters[i]->m_name.m_name << "' was supplied with a value of type '"
						<< object::objectTypeToString.at(evaluatedArguments[i]->Type()) << "' instead of type '"
						<< function->m_parameters[i]->m_token.m_literal << "' for the function call for '"
						<< function->m_function_name->String() << "'.";
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
			error << "'" << callExpression->m_function->String() << "' is not a function.";
			return createError(error.str());
		}

		std::shared_ptr<object::Object> output = applyFunction(expression, &evaluatedArguments, timeout);

		// YOU WERE HERE

		if (expression->Type() == object::FUNCTION && output->Type() == object::NULL_TYPE)
		{
			std::ostringstream error;
			error << "'" << std::static_pointer_cast<object::Function>(expression)->m_function_name->String() << "' has no return value.";
			return createError(error.str());
		}

		if (expression->Type() == object::FUNCTION && output->Type() != std::static_pointer_cast<object::Function>(expression)->m_function_type)
		{
			std::ostringstream error;
			error << "'" << callExpression->String() << "\' produced a value of type '"
				<< object::objectTypeToString.at(output->Type()) << "' instead of type '"
				<< object::objectTypeToString.at(std::static_pointer_cast<object::Function>(expression)->m_function_type) << "'.";
			return createError(error.str());
		}

		return output;
	}

	std::shared_ptr<object::Object> evaluateIndexExpression(std::shared_ptr<ast::IndexExpression> indexExpression, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout)
	{
		// Evaluate expression and apply index to it
		std::shared_ptr<object::Object> expression = evaluate(indexExpression->m_collection, environment, timeout);

		// Get index
		std::shared_ptr<object::Object> indexObject = evaluate(indexExpression->m_index, environment, timeout);
		if (indexObject->Type() == object::ERROR) return indexObject;

		if (expression->Type() != object::DICTIONARY && indexObject->Type() != object::INTEGER)
		{
			std::ostringstream error;
			error << "Invalid index: '" << indexObject->Inspect() << "'";
			return createError(error.str());
		}
		else if (expression->Type() == object::DICTIONARY &&
			(indexObject->Type() != std::static_pointer_cast<object::Dictionary>(expression)->m_key_type))
		{
			std::ostringstream error;
			error << "Dictionary has keys of type: '" << 
				object::objectTypeToString.at(std::static_pointer_cast<object::Dictionary>(expression)->m_key_type) <<
				"'. Got type: '" << object::objectTypeToString.at(indexObject->Type()) << "'";
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


	std::shared_ptr<object::Object> collectionValueReassignment(std::shared_ptr<object::Collection> collection, std::shared_ptr<object::Object> indexObject, std::shared_ptr<object::Object> valueObject)
	{
		if (indexObject->Type() != object::INTEGER)
		{
			std::ostringstream error;
			error << "Invalid index: '" << indexObject->Inspect() << "'";
			return createError(error.str());
		}

		std::shared_ptr<object::Integer> index = std::static_pointer_cast<object::Integer>(indexObject);

		if (index->m_value < 0)
		{
			std::ostringstream error;
			error << "Invalid index: '" << index->Inspect() << "'";
			return createError(error.str());
		}

		if (index->m_value >= (collection->m_values.size())) return createError("Index out of bounds.");

		if (valueObject->Type() != collection->m_collection_type)
		{
			std::ostringstream error;
			error << "'The collection has values of type '" << object::objectTypeToString.at(collection->m_collection_type)
				<< "'. Got value of type '" << object::objectTypeToString.at(valueObject->Type()) << "'.";
			return createError(error.str());
		}

		collection->m_values[index->m_value] = valueObject;
		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> dictionaryValueReassignment(std::shared_ptr<object::Dictionary> dictionary, std::shared_ptr<object::Object> keyObject, std::shared_ptr<object::Object> valueObject)
	{
		if (keyObject->Type() != dictionary->m_key_type)
		{
			std::ostringstream error;
			error << "Dictionary has keys of type: '" <<
				object::objectTypeToString.at(dictionary->m_key_type) <<
				"'. Got type: '" << object::objectTypeToString.at(keyObject->Type()) << "'";
			return createError(error.str());
		}

		dictionary->m_map[keyObject] = valueObject;
		return object::NULL_OBJECT;
	}

	std::shared_ptr<object::Object> applyFunction(std::shared_ptr<object::Object> function, std::vector<std::shared_ptr<object::Object>>* arguments, std::chrono::steady_clock::time_point timeout)
	{
		switch (function->Type())
		{
		case object::BUILTIN_FUNCTION:
		{
			std::shared_ptr<object::Object> evaluated = std::static_pointer_cast<object::Builtin>(function)->m_function(arguments);
			return unwrapReturnValue(evaluated);
		}
		case object::FUNCTION:
		{
			std::shared_ptr<object::Environment> extendedEnvironment = extendFunctionEnvironment(std::static_pointer_cast<object::Function>(function), arguments);
			std::shared_ptr<object::Object> evaluated = evaluate(std::static_pointer_cast<object::Function>(function)->m_body, extendedEnvironment, timeout);
			return unwrapReturnValue(evaluated);
		}
		}

		// Error for unsupported types.
		std::ostringstream error;
		error << "'" << function->Inspect() << "' is not a callable function.";
		return createError(error.str());
	}

	std::shared_ptr<object::Environment> extendFunctionEnvironment(std::shared_ptr<object::Function> function, std::vector<std::shared_ptr<object::Object>>* arguments)
	{
		std::shared_ptr<object::Environment> newEnvironment(new object::Environment(function->m_environment));

		for (int i = 0; i < arguments->size(); i++)
		{
			newEnvironment->setIdentifier(&function->m_parameters[i]->m_name.m_name, (*arguments)[i]);
		}

		return newEnvironment;
	}

	std::shared_ptr<object::Object> unwrapReturnValue(std::shared_ptr<object::Object> object)
	{
		if (object->Type() == object::RETURN)
		{
			std::shared_ptr<object::Return> returnObject = std::static_pointer_cast<object::Return>(object);
			return returnObject->m_return_value;
		}

		return object;
	}

	std::shared_ptr<object::Object> isTruthy(std::shared_ptr<object::Object> object)
	{
		switch (object->Type())
		{
		case object::BOOLEAN:
		{
			std::shared_ptr<object::Boolean> boolean = std::static_pointer_cast<object::Boolean>(object);
			if (boolean->m_value) return object::TRUE_OBJECT;
			else return object::FALSE_OBJECT;
		}
		case object::INTEGER:
		{
			std::shared_ptr<object::Integer> integer = std::static_pointer_cast<object::Integer>(object);
			if (integer->m_value != 0) return object::TRUE_OBJECT;
			else return object::FALSE_OBJECT;
		}
		case object::FLOAT:
		{
			std::shared_ptr<object::Float> floatValue = std::static_pointer_cast<object::Float>(object);
			if (floatValue->m_value != 0) return object::TRUE_OBJECT;
			else return object::FALSE_OBJECT;
		}
		default:
		{
			std::ostringstream error;
			error << "'" << object->Inspect() << "' is not a valid truthy value.";
			return createError(error.str());
		}
		}
	}


	std::shared_ptr<object::Error> createError(std::string errorMessage)
	{
		return std::shared_ptr<object::Error>(new object::Error(errorMessage));
	}

}
