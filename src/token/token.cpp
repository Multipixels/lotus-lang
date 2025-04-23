#include "token.h"

namespace token 
{
	Token::Token()
	{
		m_type = ILLEGAL;
		m_literal = "ILLEGAL";
	}

	Token::Token(TokenType p_tokenType, std::string p_literal)
	{
		m_type = p_tokenType;
		m_literal = p_literal;
	}

	TokenType lookupIdentifier(std::string* p_identifier)
	{
		if (c_keywordToTokenType.count(*p_identifier) > 0) 
		{
			return c_keywordToTokenType.at(*p_identifier);
		}
		return IDENTIFIER;
	}
}