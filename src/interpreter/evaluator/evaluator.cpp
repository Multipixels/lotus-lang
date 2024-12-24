#include "evaluator.h"

namespace evaluator
{
	object::Object* evaluate(ast::Node* node)
	{
		if (node == NULL) return &object::NULL_OBJECT;

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
		case ast::PREFIX_EXPRESSION_NODE:
		{
			ast::PrefixExpression* prefixExpression = (ast::PrefixExpression*)node;
			object::Object* rightObject = evaluate(prefixExpression->m_right_expression);
			return evaluatePrefixExpression(&prefixExpression->m_operator, rightObject);
		}
		case ast::INFIX_EXPRESSION_NODE:
		{
			ast::InfixExpression* infixExpression = (ast::InfixExpression*)node;
			object::Object* leftObject = evaluate(infixExpression->m_left_expression);
			object::Object* rightObject = evaluate(infixExpression->m_right_expression);
			return evaluateInfixExpression(leftObject, &infixExpression->m_operator, rightObject);
		}
		}

		return &object::NULL_OBJECT;
	}

	object::Object* evaluateProgram(ast::Program* program)
	{
		object::Object* result = &object::NULL_OBJECT;

		for (int i = 0; i < program->m_statements.size(); i++)
		{
			result = evaluate(program->m_statements[i]);
		}

		return result;
	}

	object::Object* evaluatePrefixExpression(std::string* prefixOperator, object::Object* rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*prefixOperator == "!") return evalBangOperatorExpression(rightObject);
		if (*prefixOperator == "-") return evalMinusPrefixOperatorExpression(rightObject);

		// Temporary
		return &object::NULL_OBJECT;
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

		return &object::NULL_OBJECT;
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

		return &object::NULL_OBJECT;
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

		return &object::NULL_OBJECT;
	}

	object::Object* evaluateBooleanInfixExpression(object::Boolean* leftObject, std::string* infixOperator, object::Boolean* rightObject)
	{
		// TODO: Change operator to an enum for performance gain
		if (*infixOperator == "&&") return new object::Boolean(leftObject->m_value && rightObject->m_value);
		if (*infixOperator == "||") return new object::Boolean(leftObject->m_value || rightObject->m_value);
		if (*infixOperator == "==") return new object::Boolean(leftObject->m_value == rightObject->m_value);
		if (*infixOperator == "!=") return new object::Boolean(leftObject->m_value != rightObject->m_value);

		return &object::NULL_OBJECT;
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

		return &object::NULL_OBJECT;
	}

	object::Object* evalMinusPrefixOperatorExpression(object::Object* expression)
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

		return &object::NULL_OBJECT;
	}

}
