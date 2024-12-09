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

	std::string IntegerLiteral::TokenLiteral()
	{
		return m_token.m_literal;
	}
}