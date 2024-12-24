#include "evaluator.h"

namespace evaluator
{
	object::Object* evaluate(ast::Node* node)
	{
		switch (node->Type())
		{
		case ast::PROGRAM_NODE:
			return evaluateProgram((ast::Program*)node);
		case ast::EXPRESSION_STATEMENT_NODE:
			return evaluate(((ast::ExpressionStatement*)node)->m_expression);
		case ast::INTEGER_LITERAL_NODE:
		{
			object::Integer* object = new object::Integer;
			object->m_value = ((ast::IntegerLiteral*)node)->m_value;
			return object;
		}
		case ast::BOOLEAN_LITERAL_NODE:
		{
			object::Boolean* object = new object::Boolean;
			object->m_value = ((ast::BooleanLiteral*)node)->m_value;
			return object;
		}
		case ast::PREFIX_EXPRESSION_NODE:
		{
			ast::PrefixExpression* prefixExpression = (ast::PrefixExpression*)node;
			object::Object* rightObject = evaluate(prefixExpression->m_right_expression);
			return evaluatePrefixExpression(&prefixExpression->m_operator, rightObject);
		}
		}

		return NULL;
	}

	object::Object* evaluateProgram(ast::Program* program)
	{
		object::Object* result = NULL;

		for (int i = 0; i < program->m_statements.size(); i++)
		{
			result = evaluate(program->m_statements[i]);
		}

		return result;
	}

	object::Object* evaluatePrefixExpression(std::string* prefixOperator, object::Object* rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*prefixOperator == "!")
		{
			return evalBangOperatorExpression(rightObject);
		}

		// Temporary
		return NULL;
	}

	object::Object* evalBangOperatorExpression(object::Object* expression)
	{
		switch (expression->Type())
		{
		case object::INTEGER:
		{
			object::Integer* integer = (object::Integer*)expression;
			if (integer->m_value) return &object::FALSE_OBJECT;
			else return &object::TRUE_OBJECT;
		}
		case object::BOOLEAN:
		{
			object::Boolean* boolean = (object::Boolean*)expression;
			if (boolean->m_value) return &object::FALSE_OBJECT;
			else return &object::TRUE_OBJECT;
		}
		}

		return &object::NULL_OBJECT;
	}
}
