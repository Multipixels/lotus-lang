#pragma once

#include "ast.h"
#include "object.h"

namespace evaluator
{
	// Evaluates a node
	object::Object* evaluate(ast::Node* node);

	// Evaluates a program
	object::Object* evaluateProgram(ast::Program* program);

	// Evaluates a prefix expression
	object::Object* evaluatePrefixExpression(std::string* prefixOperator, object::Object* rightObject);
	
	// Evaluates an infix expression
	object::Object* evaluateInfixExpression(object::Object* leftObject, std::string* infixOperator, object::Object* rightObject);

	// Evaluates an integer infix expression
	object::Object* evaluateIntegerInfixExpression(object::Integer* leftObject, std::string* infixOperator, object::Integer* rightObject);
	
	// Evaluates a boolean infix expression
	object::Object* evaluateBooleanInfixExpression(object::Boolean* leftObject, std::string* infixOperator, object::Boolean* rightObject);

	// Applies bang operator
	object::Object* evalBangOperatorExpression(object::Object* expression);

	// Applies negative operator (prefix)
	object::Object* evalMinusPrefixOperatorExpression(object::Object* expression);

}