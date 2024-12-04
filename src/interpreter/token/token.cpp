#include "token.h"

namespace token 
{

	TokenType LookupIdentifier(std::string* identifier)
	{
		if (keywordToTokenType.count(*identifier) > 0) 
		{
			return keywordToTokenType.at(*identifier);
		}
		return IDENTIFIER;
	}
}