#include <sstream>

#include "ast.h"


namespace ast
{
	std::string Program::TokenLiteral()
	{
		return "";
	}

	std::string Identifier::TokenLiteral()
	{
		return m_token.m_literal;
	}

	std::string DeclareIntegerStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}

	std::string DeclareFloatStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}

	std::string DeclareBooleanStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}

	std::string IntegerLiteral::TokenLiteral()
	{
		return m_token.m_literal;
	}

	std::string FloatLiteral::TokenLiteral()
	{
		std::ostringstream outputString;
		outputString << stof(m_token.m_literal);

		return outputString.str();
	}

	std::string BooleanLiteral::TokenLiteral()
	{
		return m_token.m_literal;
	}
}