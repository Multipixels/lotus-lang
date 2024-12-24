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

	// Evaluates a bang expression
	object::Object* evalBangOperatorExpression(object::Object* expression);
}