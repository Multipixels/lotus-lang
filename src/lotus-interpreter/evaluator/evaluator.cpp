#include <sstream>

#include "evaluator.h"


namespace evaluator
{
	object::Object* evaluate(ast::Node* node, object::Environment* environment)
	{
		if (node == NULL) return &object::NULL_OBJECT;

		switch (node->Type())
		{
		case ast::PROGRAM_NODE:
			return evaluateProgram((ast::Program*)node, environment);
		case ast::IDENTIFIER_NODE:
		{
			object::Object* result = environment->getIdentifier(&((ast::Identifier*)node)->m_name);
			if (result == NULL)
			{
				std::ostringstream error;
				error << "'" << ((ast::Identifier*)node)->m_name << "' is not defined.";
				return createError(error.str());
			}

			return result;
		}
		case ast::BLOCK_STATEMENT_NODE:
		{
			ast::BlockStatement* blockStatement = (ast::BlockStatement*)node;
			return evaluateBlockStatement(blockStatement, environment);
		}
		case ast::INTEGER_LITERAL_NODE:
		{
			object::Integer* object = new object::Integer;
			object->m_value = ((ast::IntegerLiteral*)node)->m_value;
			return object;
		}
		case ast::FLOAT_LITERAL_NODE:
		{
			object::Float* object = new object::Float;
			object->m_value = ((ast::FloatLiteral*)node)->m_value;
			return object;
		}
		case ast::BOOLEAN_LITERAL_NODE:
		{
			object::Boolean* object = new object::Boolean;
			object->m_value = ((ast::BooleanLiteral*)node)->m_value;
			return object;
		}
		case ast::CHARACTER_LITERAL_NODE:
		{
			object::Character* object = new object::Character;
			object->m_value = ((ast::CharacterLiteral*)node)->m_value;
			return object;
		}
		case ast::COLLECTION_LITERAL_NODE:
		{
			ast::CollectionLiteral* collectionLiteral = (ast::CollectionLiteral*)node;

			if (collectionLiteral->m_values.size() == 0)
			{
				return new object::Collection(object::NULL_TYPE, {});
			}

			object::Collection* object = new object::Collection;
			
			for (int i = 0; i < collectionLiteral->m_values.size(); i++)
			{
				object::Object* evaluatedItem = evaluate(collectionLiteral->m_values[i], environment);

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
		case ast::STRING_LITERAL_NODE:
		{
			std::stringstream value;

			for (int i = 0; i < ((ast::StringLiteral*)node)->m_stringCollection->m_values.size(); i++)
			{
				// Take expressoin from string collection and cast to character literal pointer
				ast::CharacterLiteral* characterLiteral = (ast::CharacterLiteral*)((ast::StringLiteral*)node)->m_stringCollection->m_values[i];
				value << characterLiteral->m_value;
			}

			std::string valueStr = value.str();
			object::String* object = new object::String(&valueStr);
			return object;
		}
		case ast::PREFIX_EXPRESSION_NODE:
		{
			ast::PrefixExpression* prefixExpression = (ast::PrefixExpression*)node;

			object::Object* rightObject = evaluate(prefixExpression->m_right_expression, environment);
			if (rightObject->Type() == object::ERROR) return rightObject;

			return evaluatePrefixExpression(&prefixExpression->m_operator, rightObject);
		}
		case ast::INFIX_EXPRESSION_NODE:
		{
			ast::InfixExpression* infixExpression = (ast::InfixExpression*)node;

			if (infixExpression->m_left_expression->Type() == ast::IDENTIFIER_NODE && infixExpression->m_operator == "=")
			{
				ast::Identifier* identifier = (ast::Identifier*)(infixExpression->m_left_expression);
				object::Object* savedValue = environment->getIdentifier(&identifier->m_name);
				if (savedValue == NULL)
				{
					std::ostringstream error;
					error << "'" << identifier->m_name << "' is not defined.";
					return createError(error.str());
				}

				object::Object* rightObject = evaluate(infixExpression->m_right_expression, environment);

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

				return &object::NULL_OBJECT;
			}
			else
			{
				object::Object* leftObject = evaluate(infixExpression->m_left_expression, environment);
				if (leftObject->Type() == object::ERROR) return leftObject;

				object::Object* rightObject = evaluate(infixExpression->m_right_expression, environment);
				if (rightObject->Type() == object::ERROR) return rightObject;

				return evaluateInfixExpression(leftObject, &infixExpression->m_operator, rightObject);
			}
		}
		case ast::CALL_EXPRESSION_NODE:
		{
			ast::CallExpression* callExpression = (ast::CallExpression*)node;
			return evaluateCallExpression(callExpression, environment);
		}
		case ast::INDEX_EXPRESSION_NODE:
		{
			ast::IndexExpression* indexExpression = (ast::IndexExpression*)node;
			return evaluateIndexExpression(indexExpression, environment);
		}
		case ast::DECLARE_VARIABLE_STATEMENT_NODE:
		{
			ast::DeclareVariableStatement* declareVariableStatement = (ast::DeclareVariableStatement*)node;

			if (environment->getLocalIdentifier(&declareVariableStatement->m_name.m_name) != NULL)
			{
				std::ostringstream error;
				error << "Redefinition of '" << declareVariableStatement->m_name.m_name << "'.";
				return createError(error.str());
			}

			object::Object* object = evaluate(declareVariableStatement->m_value, environment);

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

			return &object::NULL_OBJECT;
		}
		case ast::DECLARE_COLLECTION_STATEMENT_NODE:
		{
			ast::DeclareCollectionStatement* declareCollectionStatement = (ast::DeclareCollectionStatement*)node;

			if (environment->getLocalIdentifier(&declareCollectionStatement->m_name.m_name) != NULL)
			{
				std::ostringstream error;
				error << "Redefinition of '" << declareCollectionStatement->m_name.m_name << "'.";
				return createError(error.str());
			}

			object::Object* object = evaluate(declareCollectionStatement->m_value, environment);

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

			object::Collection* collection = (object::Collection*)object;

			if (collection->m_collection_type != object::NULL_TYPE && declareCollectionStatement->m_typeToken.m_literal != object::objectTypeToString.at(collection->m_collection_type))
			{
				std::ostringstream error;
				error << "'" << declareCollectionStatement->m_name.m_name
					<< "' is a collection of '" << declareCollectionStatement->m_typeToken.m_literal 
					<< "'s, but got a collection of type '" << object::objectTypeToString.at(collection->m_collection_type) << "'s.";
				return createError(error.str());
			}

			environment->setIdentifier(&declareCollectionStatement->m_name.m_name, object);

			return &object::NULL_OBJECT;
		}
		case ast::DECLARE_FUNCTION_STATEMENT_NODE:
		{
			ast::DeclareFunctionStatement* declareFunctionStatement = (ast::DeclareFunctionStatement*)node;

			if (object::nodeTypeToObjectType.count(declareFunctionStatement->m_token.m_type) == 0)
			{
				std::ostringstream error;
				error << "'" << declareFunctionStatement->m_token.m_literal
					<< "' is not a valid function type.";
				return createError(error.str());
			}

			object::ObjectType functionType = object::nodeTypeToObjectType.at(declareFunctionStatement->m_token.m_type);
			object::Function* result = new object::Function(functionType, declareFunctionStatement, environment);

			environment->setIdentifier(&declareFunctionStatement->m_name.m_name, result);

			return &object::NULL_OBJECT;
		}
		case ast::RETURN_STATEMENT_NODE:
		{
			ast::ReturnStatement* returnStatement = (ast::ReturnStatement*)node;
			return new object::Return(evaluate(returnStatement->m_returnValue, environment));
		}
		case ast::EXPRESSION_STATEMENT_NODE:
			return evaluate(((ast::ExpressionStatement*)node)->m_expression, environment);
		case ast::IF_STATEMENT_NODE:
		{
			ast::IfStatement* ifStatement = (ast::IfStatement*)node;

			// Treat as else caluse
			if (ifStatement->m_condition == NULL)
			{
				object::Environment* ifEnvironment = new object::Environment(environment);
				return evaluate(ifStatement->m_consequence, ifEnvironment);
			}

			object::Object* evaluatedCondition = evaluate(ifStatement->m_condition, environment);

			if (evaluatedCondition->Type() == object::ERROR)
			{
				return evaluatedCondition;
			}

			object::Object* truthy = isTruthy(evaluatedCondition);

			if (truthy->Type() == object::ERROR)
			{
				return truthy;
			}

			object::Boolean* truthyBoolean = (object::Boolean*)truthy;
			object::Environment* ifEnvironment = new object::Environment(environment);

			if (truthyBoolean->m_value) return evaluate(ifStatement->m_consequence, ifEnvironment);
			else if (ifStatement->m_alternative != NULL) return evaluate(ifStatement->m_alternative, ifEnvironment);
			else return &object::NULL_OBJECT;
		}
		case ast::WHILE_STATEMENT_NODE:
		{
			ast::WhileStatement* whileStatement = (ast::WhileStatement*)node;
			object::Environment* whileEnvironment = new object::Environment(environment);

			while (true)
			{
				object::Object* evaluatedCondition = evaluate(whileStatement->m_condition, environment);
				if (evaluatedCondition->Type() == object::ERROR)
				{
					return evaluatedCondition;
				}

				object::Object* truthy = isTruthy(evaluatedCondition);
				if (truthy->Type() == object::ERROR)
				{
					return truthy;
				}

				object::Boolean* truthyBoolean = (object::Boolean*)truthy;
				if (!truthyBoolean->m_value) break;

				object::Object* evaluatedConsequence = evaluate(whileStatement->m_consequence, whileEnvironment);
				if (evaluatedConsequence->Type() == object::ERROR)
				{
					return evaluatedConsequence;
				}
			}
			
			return &object::NULL_OBJECT;
		}
		case ast::DO_WHILE_STATEMENT_NODE:
		{
			ast::DoWhileStatement* doWhileStatement = (ast::DoWhileStatement*)node;
			object::Environment* doWhileEnvironment = new object::Environment(environment);

			object::Object* evaluatedConsequence = evaluate(doWhileStatement->m_consequence, doWhileEnvironment);
			if (evaluatedConsequence->Type() == object::ERROR)
			{
				return evaluatedConsequence;
			}

			while (true)
			{
				object::Object* evaluatedCondition = evaluate(doWhileStatement->m_condition, environment);
				if (evaluatedCondition->Type() == object::ERROR)
				{
					return evaluatedCondition;
				}

				object::Object* truthy = isTruthy(evaluatedCondition);
				if (truthy->Type() == object::ERROR)
				{
					return truthy;
				}

				object::Boolean* truthyBoolean = (object::Boolean*)truthy;
				if (!truthyBoolean->m_value) break;

				object::Object* evaluatedConsequence = evaluate(doWhileStatement->m_consequence, doWhileEnvironment);
				if (evaluatedConsequence->Type() == object::ERROR)
				{
					return evaluatedConsequence;
				}
			}

			return &object::NULL_OBJECT;
		}
		case ast::FOR_STATEMENT_NODE:
		{
			ast::ForStatement* forStatement = (ast::ForStatement*)node;
			object::Environment* forConditionEnvironment = new object::Environment(environment);
			object::Environment* forEnvironment = new object::Environment(forConditionEnvironment);

			object::Object* evaluatedInitialization = evaluate(forStatement->m_initialization, forConditionEnvironment);
			if (evaluatedInitialization->Type() == object::ERROR)
			{
				return evaluatedInitialization;
			}

			while (true)
			{
				object::Object* evaluatedCondition = evaluate(forStatement->m_condition, forConditionEnvironment);
				if (evaluatedCondition->Type() == object::ERROR)
				{
					return evaluatedCondition;
				}

				object::Object* truthy = isTruthy(evaluatedCondition);
				if (truthy->Type() == object::ERROR)
				{
					return truthy;
				}

				object::Boolean* truthyBoolean = (object::Boolean*)truthy;
				if (!truthyBoolean->m_value) break;

				object::Object* evaluatedConsequence = evaluate(forStatement->m_consequence, forEnvironment);
				if (evaluatedConsequence->Type() == object::ERROR)
				{
					return evaluatedConsequence;
				}

				object::Object* evaluatedUpdation = evaluate(forStatement->m_updation, forConditionEnvironment);
				if (evaluatedUpdation->Type() == object::ERROR)
				{
					return evaluatedUpdation;
				}
			}
			
			return &object::NULL_OBJECT;
		}
		case ast::ITERATE_STATEMENT_NODE:
		{
			ast::IterateStatement* iterateStatement = (ast::IterateStatement*)node;
			object::Environment* iterateEnvironment = new object::Environment(environment);

			object::Object* evaluatedCollection = evaluate(iterateStatement->m_collection, environment);
			if (evaluatedCollection->Type() == object::ERROR) return evaluatedCollection;
			if(evaluatedCollection->Type() != object::COLLECTION) 
			{
				std::ostringstream error;
				error << "Expected to see a collection to iterate over. Instead got a(n) '"
					<< object::objectTypeToString.at(evaluatedCollection->Type()) << "'.";
				return createError(error.str());
			}
			object::Collection* collection = (object::Collection*)evaluatedCollection;

			for (object::Object* value : collection->m_values)
			{
				iterateEnvironment->setIdentifier(&iterateStatement->m_var->m_name, value);
				
				object::Object* evaluatedConsequence = evaluate(iterateStatement->m_consequence, iterateEnvironment);
				if (evaluatedConsequence->Type() == object::ERROR) return evaluatedConsequence;
			}

			return &object::NULL_OBJECT;
		}
		}

		return NULL;
	}

	object::Object* evaluateProgram(ast::Program* program, object::Environment* environment)
	{
		object::Object* result = &object::NULL_OBJECT;

		for (int i = 0; i < program->m_statements.size(); i++)
		{
			result = evaluate(program->m_statements[i], environment);

			if (result != NULL && result->Type() == object::RETURN)
			{
				object::Return* returnObj = (object::Return*)result;
				return returnObj->m_return_value;
			}

			if (result != NULL && result->Type() == object::ERROR)
			{
				return result;
			}
		}

		return result;
	}

	object::Object* evaluateBlockStatement(ast::BlockStatement* blockStatements, object::Environment* environment)
	{
		object::Object* result = &object::NULL_OBJECT;

		for (int i = 0; i < blockStatements->m_statements.size(); i++)
		{
			result = evaluate(blockStatements->m_statements[i], environment);

			if (result != NULL && result->Type() == object::RETURN)
			{
				return result;
			}

			if (result != NULL && result->Type() == object::ERROR)
			{
				return result;
			}
		}

		return &object::NULL_OBJECT;
	}


	void evaluateExpressions(std::vector<ast::Expression*>* source, std::vector<object::Object*>* destination, object::Environment* environment)
	{
		for (int i = 0; i < source->size(); i++)
		{
			object::Object* evaluatedExpression = evaluate((*source)[i], environment);

			if (evaluatedExpression->Type() == object::ERROR)
			{
				(*destination).clear();
				(*destination).push_back(evaluatedExpression);
				return;
			}
			(*destination).push_back(evaluatedExpression);
		}
	}

	object::Object* evaluatePrefixExpression(std::string* prefixOperator, object::Object* rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*prefixOperator == "!") return evaluateBangOperatorExpression(rightObject);
		if (*prefixOperator == "-") return evaluateMinusPrefixOperatorExpression(rightObject);

		std::ostringstream error;
		error << *prefixOperator << object::objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	object::Object* evaluateInfixExpression(object::Object* leftObject, std::string* infixOperator, object::Object* rightObject)
	{
		switch (leftObject->Type())
		{
		case object::INTEGER:
		{
			if (rightObject->Type() == object::INTEGER)
				return evaluateIntegerInfixExpression(
					(object::Integer*)leftObject, infixOperator, (object::Integer*)rightObject);
			if (rightObject->Type() == object::FLOAT)
				return evaluateFloatInfixExpression(
					new object::Float(((object::Integer*)leftObject)->m_value), infixOperator, (object::Float*)rightObject);
			break;
		}
		case object::FLOAT:
		{
			if (rightObject->Type() == object::INTEGER)
				return evaluateFloatInfixExpression(
					(object::Float*)leftObject, infixOperator, new object::Float(((object::Integer*)rightObject)->m_value));
			if (rightObject->Type() == object::FLOAT)
				return evaluateFloatInfixExpression(
					(object::Float*)leftObject, infixOperator, (object::Float*)rightObject);
			break;
		}
		case object::BOOLEAN:
		{
			if (rightObject->Type() == object::BOOLEAN)
				return evaluateBooleanInfixExpression(
					(object::Boolean*)leftObject, infixOperator, (object::Boolean*)rightObject);
			break;
		}
		}

		std::ostringstream error;
		error << "'" << object::objectTypeToString.at(leftObject->Type())
			<< ' ' << *infixOperator << ' '
			<< object::objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	object::Object* evaluateIntegerInfixExpression(object::Integer* leftObject, std::string* infixOperator, object::Integer* rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*infixOperator == "+") return new object::Integer(leftObject->m_value + rightObject->m_value);
		if (*infixOperator == "-") return new object::Integer(leftObject->m_value - rightObject->m_value);
		if (*infixOperator == "*") return new object::Integer(leftObject->m_value * rightObject->m_value);
		if (*infixOperator == "/") return new object::Integer(leftObject->m_value / rightObject->m_value);
		if (*infixOperator == "<") return new object::Boolean(leftObject->m_value < rightObject->m_value);
		if (*infixOperator == "<=") return new object::Boolean(leftObject->m_value <= rightObject->m_value);
		if (*infixOperator == ">") return new object::Boolean(leftObject->m_value > rightObject->m_value);
		if (*infixOperator == ">=") return new object::Boolean(leftObject->m_value >= rightObject->m_value);
		if (*infixOperator == "==") return new object::Boolean(leftObject->m_value == rightObject->m_value);
		if (*infixOperator == "!=") return new object::Boolean(leftObject->m_value != rightObject->m_value);

		std::ostringstream error;
		error << "'" << object::objectTypeToString.at(leftObject->Type())
			<< ' ' << *infixOperator << ' '
			<< object::objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	object::Object* evaluateFloatInfixExpression(object::Float* leftObject, std::string* infixOperator, object::Float* rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*infixOperator == "+") return new object::Float(leftObject->m_value + rightObject->m_value);
		if (*infixOperator == "-") return new object::Float(leftObject->m_value - rightObject->m_value);
		if (*infixOperator == "*") return new object::Float(leftObject->m_value * rightObject->m_value);
		if (*infixOperator == "/") return new object::Float(leftObject->m_value / rightObject->m_value);
		if (*infixOperator == "<") return new object::Boolean(leftObject->m_value < rightObject->m_value);
		if (*infixOperator == "<=") return new object::Boolean(leftObject->m_value <= rightObject->m_value);
		if (*infixOperator == ">") return new object::Boolean(leftObject->m_value > rightObject->m_value);
		if (*infixOperator == ">=") return new object::Boolean(leftObject->m_value >= rightObject->m_value);
		if (*infixOperator == "==") return new object::Boolean(leftObject->m_value == rightObject->m_value);
		if (*infixOperator == "!=") return new object::Boolean(leftObject->m_value != rightObject->m_value);

		std::ostringstream error;
		error << "'" << object::objectTypeToString.at(leftObject->Type())
			<< ' ' << *infixOperator << ' '
			<< object::objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	object::Object* evaluateBooleanInfixExpression(object::Boolean* leftObject, std::string* infixOperator, object::Boolean* rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*infixOperator == "&&") return new object::Boolean(leftObject->m_value && rightObject->m_value);
		if (*infixOperator == "||") return new object::Boolean(leftObject->m_value || rightObject->m_value);
		if (*infixOperator == "==") return new object::Boolean(leftObject->m_value == rightObject->m_value);
		if (*infixOperator == "!=") return new object::Boolean(leftObject->m_value != rightObject->m_value);

		std::ostringstream error;
		error << "'" << object::objectTypeToString.at(leftObject->Type())
			<< ' ' << *infixOperator << ' '
			<< object::objectTypeToString.at(rightObject->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	object::Object* evaluateBangOperatorExpression(object::Object* expression)
	{
		switch (expression->Type())
		{
		case object::INTEGER:
		{
			object::Integer* integer = (object::Integer*)expression;
			if (integer->m_value) return &object::FALSE_OBJECT;
			else return &object::TRUE_OBJECT;
		}
		case object::FLOAT:
		{
			object::Float* floating = (object::Float*)expression;
			if (floating->m_value) return &object::FALSE_OBJECT;
			else return &object::TRUE_OBJECT;
		}
		case object::BOOLEAN:
		{
			object::Boolean* boolean = (object::Boolean*)expression;
			if (boolean->m_value) return &object::FALSE_OBJECT;
			else return &object::TRUE_OBJECT;
		}
		}

		std::ostringstream error;
		error << "'!" << object::objectTypeToString.at(expression->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	object::Object* evaluateMinusPrefixOperatorExpression(object::Object* expression)
	{
		switch (expression->Type())
		{
		case object::INTEGER:
		{
			object::Integer* integer = (object::Integer*)expression;
			object::Integer* returnValue = new object::Integer(-integer->m_value);
			return returnValue;
		}
		case object::FLOAT:
		{
			object::Float* floating = (object::Float*)expression;
			object::Float* returnValue = new object::Float(-floating->m_value);
			return returnValue;
		}
		}

		std::ostringstream error;
		error << "'-" << object::objectTypeToString.at(expression->Type()) << "\' is not supported.";
		return createError(error.str());
	}

	object::Object* evaluateCallExpression(ast::CallExpression* callExpression, object::Environment* environment)
	{
		object::Object* expression = evaluate(callExpression->m_function, environment);
		if (expression->Type() == object::ERROR)
		{
			return expression;
		}

		object::Function* function = (object::Function*)expression;

		std::vector<object::Object*> evaluatedArguments;

		if (callExpression->m_parameters.size() != function->m_parameters.size())
		{
			std::ostringstream error;
			error << "'" << function->m_function_name->String() << "' was supplied with "
				<< callExpression->m_parameters.size() << " argument(s) instead of "
				<< function->m_parameters.size() << ".";
			return createError(error.str());
		}

		evaluateExpressions(&callExpression->m_parameters, &evaluatedArguments, environment);

		if (evaluatedArguments.size() == 1 && evaluatedArguments[0]->Type() == object::ERROR)
		{
			return evaluatedArguments[0];
		}
		for (int i = 0; i < function->m_parameters.size(); i++)
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

		object::Object* output = applyFunction(function, &evaluatedArguments);

		if (output->Type() == object::NULL_TYPE)
		{
			std::ostringstream error;
			error << "'" << function->m_function_name->String() << "' has no return value.";
			return createError(error.str());
		}

		if (output->Type() != function->m_function_type)
		{
			std::ostringstream error;
			error << "'" << callExpression->String() << "\' produced a value of type '"
				<< object::objectTypeToString.at(output->Type()) << "' instead of type '"
				<< object::objectTypeToString.at(function->m_function_type) << "'.";
			return createError(error.str());
		}

		return output;
	}

	object::Object* evaluateIndexExpression(ast::IndexExpression* indexExpression, object::Environment* environment)
	{
		// Get index
		object::Object* indexObject = evaluate(indexExpression->m_index, environment);
		if (indexObject->Type() == object::ERROR) return indexObject;
		if (indexObject->Type() != object::INTEGER)
		{
			std::ostringstream error;
			error << "Invalid index: '" << indexObject->Inspect() << "'";
			return createError(error.str());
		}
		object::Integer* index = (object::Integer*)indexObject;

		if (index->m_value < 0)
		{
			std::ostringstream error;
			error << "Invalid index: '" << index->Inspect() << "'";
			return createError(error.str());
		}

		// Evaluate expression and apply index to it
		object::Object* expression = evaluate(indexExpression->m_collection, environment);

		switch (expression->Type())
		{
		case object::ERROR:
			return expression;
		case object::COLLECTION:
			if (index->m_value >= ((object::Collection*)expression)->m_values.size()) return createError("Index out of bounds.");
			return ((object::Collection*)expression)->m_values[index->m_value];
		case object::STRING:
		{
			if (index->m_value >= ((object::String*)expression)->m_value.size()) return createError("Index out of bounds.");
			char value = ((object::String*)expression)->m_value[index->m_value];
			return new object::Character(value);
		}
		}

		// Error for unsupported types.
		std::ostringstream error;
		error << "'" << expression->Inspect() << "' is not an indexable value.";
		return createError(error.str());
	}

	object::Object* applyFunction(object::Function* function, std::vector<object::Object*>* arguments)
	{
		object::Environment* extendedEnvironment = extendFunctionEnvironment(function, arguments);
		object::Object* evaluated = evaluate(function->m_body, extendedEnvironment);
		return unwrapReturnValue(evaluated);
	}

	object::Environment* extendFunctionEnvironment(object::Function* function, std::vector<object::Object*>* arguments)
	{
		object::Environment* newEnvironment = new object::Environment(function->m_environment);

		for (int i = 0; i < arguments->size(); i++)
		{
			newEnvironment->setIdentifier(&function->m_parameters[i]->m_name.m_name, (*arguments)[i]);
		}

		return newEnvironment;
	}

	object::Object* unwrapReturnValue(object::Object* object)
	{
		if (object->Type() == object::RETURN)
		{
			object::Return* returnObject = (object::Return*)object;
			return returnObject->m_return_value;
		}

		return object;
	}

	object::Object* isTruthy(object::Object* object)
	{
		switch (object->Type())
		{
		case object::BOOLEAN:
		{
			object::Boolean* boolean = (object::Boolean*)object;
			if (boolean->m_value) return &object::TRUE_OBJECT;
			else return &object::FALSE_OBJECT;
		}
		case object::INTEGER:
		{
			object::Integer* integer = (object::Integer*)object;
			if (integer->m_value != 0) return &object::TRUE_OBJECT;
			else return &object::FALSE_OBJECT;
		}
		case object::FLOAT:
		{
			object::Float* floatValue = (object::Float*)object;
			if (floatValue->m_value != 0) return &object::TRUE_OBJECT;
			else return &object::FALSE_OBJECT;
		}
		default:
		{
			std::ostringstream error;
			error << "'" << object->Inspect() << "' is not a valid truthy value.";
			return createError(error.str());
		}
		}
	}


	object::Error* createError(std::string errorMessage)
	{
		return new object::Error(errorMessage);
	}

}
