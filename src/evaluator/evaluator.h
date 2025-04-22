#pragma once
#include <chrono>

#include "ast.h"
#include "object.h"

namespace evaluator
{
	// Evaluates a node
	object::Object* evaluate(ast::Node* node, object::Environment* environment, std::chrono::steady_clock::time_point timeout = std::chrono::steady_clock::time_point());

	// Evaluates a program
	object::Object* evaluateProgram(ast::Program* program, object::Environment* environment, std::chrono::steady_clock::time_point timeout);

	// Evaluates a block statement
	object::Object* evaluateBlockStatement(ast::BlockStatement* blockStatements, object::Environment* environment, std::chrono::steady_clock::time_point timeout);

	// Evaluates a list of expressions
	void evaluateExpressions(std::vector<ast::Expression*>* source, std::vector<object::Object*>* destination, object::Environment* environment, std::chrono::steady_clock::time_point timeout);

	// Evaluates an identifier
	object::Object* evaluateIdentifier(ast::Identifier* identifier, object::Environment* environment);

	// Evaluates a prefix expression
	object::Object* evaluatePrefixExpression(std::string* prefixOperator, object::Object* rightObject);
	
	// Evaluates an infix expression
	object::Object* evaluateInfixExpression(object::Object* leftObject, std::string* infixOperator, object::Object* rightObject);

	// Evaluates an integer infix expression
	object::Object* evaluateIntegerInfixExpression(object::Integer* leftObject, std::string* infixOperator, object::Integer* rightObject);
	
	// Evaluates a boolean infix expression
	object::Object* evaluateBooleanInfixExpression(object::Boolean* leftObject, std::string* infixOperator, object::Boolean* rightObject);

	// Evaluates a character infix expression
	object::Object* evaluateCharacterInfixExpression(object::Character* leftObject, std::string* infixOperator, object::Character* rightObject);
	
	// Evaluates an integer infix expression
	object::Object* evaluateFloatInfixExpression(object::Float* leftObject, std::string* infixOperator, object::Float* rightObject);

	// Applies bang operator
	object::Object* evaluateBangOperatorExpression(object::Object* expression);

	// Applies negative operator (prefix)
	object::Object* evaluateMinusPrefixOperatorExpression(object::Object* expression);

	// Evaluates a function call
	object::Object* evaluateCallExpression(ast::CallExpression* callExpression, object::Environment* environment, std::chrono::steady_clock::time_point timeout);

	// Evaluates an indexing on collections, strings, or dictionaries
	object::Object* evaluateIndexExpression(ast::IndexExpression* indexExpression, object::Environment* environment, std::chrono::steady_clock::time_point timeout);

	// Reassigns value in a collection
	object::Object* collectionValueReassignment(object::Collection* collection, object::Object* indexObject, object::Object* valueObject);

	// Reassigns value in a dictionary
	object::Object* dictionaryValueReassignment(object::Dictionary* dictionary, object::Object* keyObject, object::Object* valueObject);

	// Applies a function call to a function
	object::Object* applyFunction(object::Object* function, std::vector<object::Object*>* arguments, std::chrono::steady_clock::time_point timeout);

	// Helper function to extend a function's environment
	object::Environment* extendFunctionEnvironment(object::Function* function, std::vector<object::Object*>* arguments);

	// Unwraps return value
	object::Object* unwrapReturnValue(object::Object* object);

	// Checks value of a truthy object
	object::Object* isTruthy(object::Object* object);

	// Creates an error object with the provided error message
	object::Error* createError(std::string errorMessage);
}