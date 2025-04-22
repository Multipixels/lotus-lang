#pragma once
#include <chrono>

#include "ast.h"
#include "object.h"

namespace evaluator
{
	// Evaluates a node
	std::shared_ptr<object::Object> evaluate(std::shared_ptr<ast::Node> node, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout = std::chrono::steady_clock::time_point());

	// Evaluates a program
	std::shared_ptr<object::Object> evaluateProgram(std::shared_ptr<ast::Program> program, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout);

	// Evaluates a block statement
	std::shared_ptr<object::Object> evaluateBlockStatement(std::shared_ptr<ast::BlockStatement> blockStatements, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout);

	// Evaluates a list of expressions
	void evaluateExpressions(std::vector<std::shared_ptr<ast::Expression>>* source, std::vector<std::shared_ptr<object::Object>>* destination, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout);

	// Evaluates an identifier
	std::shared_ptr<object::Object> evaluateIdentifier(std::shared_ptr<ast::Identifier> identifier, std::shared_ptr<object::Environment> environment);

	// Evaluates a prefix expression
	std::shared_ptr<object::Object> evaluatePrefixExpression(std::string* prefixOperator, std::shared_ptr<object::Object> rightObject);
	
	// Evaluates an infix expression
	std::shared_ptr<object::Object> evaluateInfixExpression(std::shared_ptr<object::Object> leftObject, std::string* infixOperator, std::shared_ptr<object::Object> rightObject);

	// Evaluates an integer infix expression
	std::shared_ptr<object::Object> evaluateIntegerInfixExpression(std::shared_ptr<object::Integer> leftObject, std::string* infixOperator, std::shared_ptr<object::Integer> rightObject);
	
	// Evaluates a boolean infix expression
	std::shared_ptr<object::Object> evaluateBooleanInfixExpression(std::shared_ptr<object::Boolean> leftObject, std::string* infixOperator, std::shared_ptr<object::Boolean> rightObject);

	// Evaluates a character infix expression
	std::shared_ptr<object::Object> evaluateCharacterInfixExpression(std::shared_ptr<object::Character> leftObject, std::string* infixOperator, std::shared_ptr<object::Character> rightObject);
	
	// Evaluates an integer infix expression
	std::shared_ptr<object::Object> evaluateFloatInfixExpression(std::shared_ptr<object::Float> leftObject, std::string* infixOperator, std::shared_ptr<object::Float> rightObject);

	// Applies bang operator
	std::shared_ptr<object::Object> evaluateBangOperatorExpression(std::shared_ptr<object::Object> expression);

	// Applies negative operator (prefix)
	std::shared_ptr<object::Object> evaluateMinusPrefixOperatorExpression(std::shared_ptr<object::Object> expression);

	// Evaluates a function call
	std::shared_ptr<object::Object> evaluateCallExpression(std::shared_ptr<ast::CallExpression> callExpression, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout);

	// Evaluates an indexing on collections, strings, or dictionaries
	std::shared_ptr<object::Object> evaluateIndexExpression(std::shared_ptr<ast::IndexExpression> indexExpression, std::shared_ptr<object::Environment> environment, std::chrono::steady_clock::time_point timeout);

	// Reassigns value in a collection
	std::shared_ptr<object::Object> collectionValueReassignment(std::shared_ptr<object::Collection> collection, std::shared_ptr<object::Object> indexObject, std::shared_ptr<object::Object> valueObject);

	// Reassigns value in a dictionary
	std::shared_ptr<object::Object> dictionaryValueReassignment(std::shared_ptr<object::Dictionary> dictionary, std::shared_ptr<object::Object> keyObject, std::shared_ptr<object::Object> valueObject);

	// Applies a function call to a function
	std::shared_ptr<object::Object> applyFunction(std::shared_ptr<object::Object> function, std::vector<std::shared_ptr<object::Object>>* arguments, std::chrono::steady_clock::time_point timeout);

	// Helper function to extend a function's environment
	std::shared_ptr<object::Environment> extendFunctionEnvironment(std::shared_ptr<object::Function> function, std::vector<std::shared_ptr<object::Object>>* arguments);

	// Unwraps return value
	std::shared_ptr<object::Object> unwrapReturnValue(std::shared_ptr<object::Object> object);

	// Checks value of a truthy object
	std::shared_ptr<object::Object> isTruthy(std::shared_ptr<object::Object> object);

	// Creates an error object with the provided error message
	std::shared_ptr<object::Error> createError(std::string errorMessage);
}