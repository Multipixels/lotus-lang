#include <sstream>

#include "ast.h"


namespace ast
{
	std::string Program::TokenLiteral()
	{
		return "";
	}
	std::string Program::String()
	{
		std::ostringstream output;

		for (int i = 0; i < m_statements.size(); i++)
		{
			output << m_statements[i]->String();
		}

		return output.str();
	}

	std::string Identifier::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string Identifier::String()
	{
		return m_name;
	}

	std::string DeclareIntegerStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string DeclareIntegerStatement::String()
	{
		std::ostringstream output;

		output << TokenLiteral() << " "
			<< m_name.String() << " = "
			<< m_value->String() << ";";

		return output.str();
	}

	std::string DeclareFloatStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string DeclareFloatStatement::String()
	{
		std::ostringstream output;

		output << TokenLiteral() << " "
			<< m_name.String() << " = "
			<< m_value->String() << ";";

		return output.str();
	}

	std::string DeclareBooleanStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string DeclareBooleanStatement::String()
	{
		std::ostringstream output;

		output << TokenLiteral() << " "
			<< m_name.String() << " = "
			<< m_value->String() << ";";

		return output.str();
	}

	std::string DeclareCharacterStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string DeclareCharacterStatement::String()
	{
		std::ostringstream output;

		output << TokenLiteral() << " "
			<< m_name.String() << " = "
			<< m_value->String() << ";";

		return output.str();
	}

	std::string ReturnStatement::TokenLiteral()
	{
		return "return";
	}
	std::string ReturnStatement::String()
	{
		std::ostringstream output;

		output << TokenLiteral() << " "
			<< m_returnValue->String() << ";";

		return output.str();
	}

	std::string ExpressionStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string ExpressionStatement::String()
	{
		std::ostringstream output;

		output << m_expression->String() << ";";

		return output.str();
	}

	std::string IntegerLiteral::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string IntegerLiteral::String()
	{
		return std::to_string(m_value);
	}

	std::string FloatLiteral::TokenLiteral()
	{
		std::ostringstream outputString;
		outputString << stof(m_token.m_literal);

		return outputString.str();
	}
	std::string FloatLiteral::String()
	{
		std::ostringstream outputString;
		outputString << m_value;

		return outputString.str();
	}

	std::string BooleanLiteral::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string BooleanLiteral::String()
	{
		return m_value ? "true" : "false";
	}

	std::string CharacterLiteral::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string CharacterLiteral::String()
	{
		char charToString[2] = { m_value, '\0' };

		return charToString;
	}

	std::string PrefixExpression::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string PrefixExpression::String()
	{
		std::ostringstream outputString;
		outputString << "(" << m_operator
			<< m_right_expression->String() << ")";

		return outputString.str();
	}
}