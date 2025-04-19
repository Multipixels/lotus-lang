#include "token.h"

namespace token 
{
	Token::Token()
	{
		m_type = ILLEGAL;
		m_literal = "ILLEGAL";
	}

	Token::Token(TokenType tokenType, std::string literal)
	{
		m_type = tokenType;
		m_literal = literal;
	}

	TokenType LookupIdentifier(std::string* identifier)
	{
		if (keywordToTokenType.count(*identifier) > 0) 
		{
			return keywordToTokenType.at(*identifier);
		}
		return IDENTIFIER;
	}
}