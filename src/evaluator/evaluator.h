#pragma once
#include <chrono>

#include "ast.h"
#include "object.h"

namespace evaluator
{
	extern std::chrono::steady_clock::time_point g_timeout;

	// Evaluates a node
	std::shared_ptr<object::Object> evaluate(std::shared_ptr<ast::Node> p_node, std::shared_ptr<object::Environment> p_environment);

	// Evaluates a program
	std::shared_ptr<object::Object> evaluateProgram(std::shared_ptr<ast::Program> p_program, std::shared_ptr<object::Environment> p_environment);

	// Evaluates an identifier
	std::shared_ptr<object::Object> evaluateIdentifier(std::shared_ptr<ast::Identifier> p_identifier, std::shared_ptr<object::Environment> p_environment);

	// Evaluates a block statement
	std::shared_ptr<object::Object> evaluateBlockStatement(std::shared_ptr<ast::BlockStatement> p_blockStatements, std::shared_ptr<object::Environment> p_environment);

	// Evaluates an integer literal
	std::shared_ptr<object::Object> evaluateIntegerLiteral(std::shared_ptr<ast::IntegerLiteral> p_integerLiteral, std::shared_ptr<object::Environment> p_environment);

	// Evaluates an float literal
	std::shared_ptr<object::Object> evaluateFloatLiteral(std::shared_ptr<ast::FloatLiteral> p_floatLiteral, std::shared_ptr<object::Environment> p_environment);

	// Evaluates an boolean literal
	std::shared_ptr<object::Object> evaluateBooleanLiteral(std::shared_ptr<ast::BooleanLiteral> p_booleanLiteral, std::shared_ptr<object::Environment> p_environment);

	// Evaluates an character literal
	std::shared_ptr<object::Object> evaluateCharacterLiteral(std::shared_ptr<ast::CharacterLiteral> p_characterLiteral, std::shared_ptr<object::Environment> p_environment);

	// Evaluates a collection literal node
	std::shared_ptr<object::Object> evaluateCollectionLiteral(std::shared_ptr<ast::CollectionLiteral> p_collectionLiteral, std::shared_ptr<object::Environment> p_environment);

	// Evaluates a dictionary literal node
	std::shared_ptr<object::Object> evaluateDictionaryLiteral(std::shared_ptr<ast::DictionaryLiteral> p_dictionaryLiteral, std::shared_ptr<object::Environment> p_environment);

	// Evaluates a string literal node
	std::shared_ptr<object::Object> evaluateStringLiteral(std::shared_ptr<ast::StringLiteral> p_stringLiteral, std::shared_ptr<object::Environment> p_environment);

	// Evaluates a list of expressions
	void evaluateExpressions(std::vector<std::shared_ptr<ast::Expression>>* p_source, std::vector<std::shared_ptr<object::Object>>* p_destination, std::shared_ptr<object::Environment> p_environment);

	// Evaluates a prefix expression
	std::shared_ptr<object::Object> evaluatePrefixExpression(std::shared_ptr<ast::PrefixExpression> p_prefixOperator, std::shared_ptr<object::Environment> p_environment);
	
	// Evaluates an infix expression
	std::shared_ptr<object::Object> evaluateInfixExpression(std::shared_ptr<ast::InfixExpression> p_infixExpression, std::shared_ptr<object::Environment> p_environment);

	// Evaluates an integer infix expression
	std::shared_ptr<object::Object> evaluateIntegerInfixExpression(std::shared_ptr<object::Integer> p_leftObject, std::string* p_infixOperator, std::shared_ptr<object::Integer> p_rightObject);
	
	// Evaluates a boolean infix expression
	std::shared_ptr<object::Object> evaluateBooleanInfixExpression(std::shared_ptr<object::Boolean> p_leftObject, std::string* p_infixOperator, std::shared_ptr<object::Boolean> p_rightObject);

	// Evaluates a character infix expression
	std::shared_ptr<object::Object> evaluateCharacterInfixExpression(std::shared_ptr<object::Character> p_leftObject, std::string* p_infixOperator, std::shared_ptr<object::Character> p_rightObject);
	
	// Evaluates an integer infix expression
	std::shared_ptr<object::Object> evaluateFloatInfixExpression(std::shared_ptr<object::Float> p_leftObject, std::string* p_infixOperator, std::shared_ptr<object::Float> p_rightObject);

	// Applies bang operator
	std::shared_ptr<object::Object> evaluateBangOperatorExpression(std::shared_ptr<object::Object> p_expression);

	// Applies negative operator (prefix)
	std::shared_ptr<object::Object> evaluateMinusPrefixOperatorExpression(std::shared_ptr<object::Object> p_expression);

	// Evaluates a function call
	std::shared_ptr<object::Object> evaluateCallExpression(std::shared_ptr<ast::CallExpression> p_callExpression, std::shared_ptr<object::Environment> p_environment);

	// Evaluates an indexing on collections, strings, or dictionaries
	std::shared_ptr<object::Object> evaluateIndexExpression(std::shared_ptr<ast::IndexExpression> p_indexExpression, std::shared_ptr<object::Environment> p_environment);

	// Reassigns value in a collection
	std::shared_ptr<object::Object> collectionValueReassignment(std::shared_ptr<object::Collection> p_collection, std::shared_ptr<object::Object> p_indexObject, std::shared_ptr<object::Object> p_valueObject);

	// Reassigns value in a dictionary
	std::shared_ptr<object::Object> dictionaryValueReassignment(std::shared_ptr<object::Dictionary> p_dictionary, std::shared_ptr<object::Object> p_keyObject, std::shared_ptr<object::Object> p_valueObject);

	// Applies a function call to a function
	std::shared_ptr<object::Object> applyFunction(std::shared_ptr<object::Object> p_function, std::vector<std::shared_ptr<object::Object>>* p_arguments);

	// Helper function to extend a function's environment
	std::shared_ptr<object::Environment> extendFunctionEnvironment(std::shared_ptr<object::Function> p_function, std::vector<std::shared_ptr<object::Object>>* p_arguments);

	// Unwraps return value
	std::shared_ptr<object::Object> unwrapReturnValue(std::shared_ptr<object::Object> p_object);

	// Checks value of a truthy object
	std::shared_ptr<object::Object> isTruthy(std::shared_ptr<object::Object> p_object);

	// Creates an error object with the provided error message
	std::shared_ptr<object::Error> createError(std::string p_errorMessage);
}