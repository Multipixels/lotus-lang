#include <string>
#include <ctype.h>

#include "lexer.h"

namespace lexer 
{
	void Lexer::readChar() 
	{

	}

	void Lexer::eatWhiteSpace() 
	{

	}

	int Lexer::readNumber() 
	{
		return 0;
	}

	void Lexer::readIdentifier(std::string* output) 
	{

	}

	void Lexer::readString(std::string* output) 
	{

	}

	bool Lexer::isLetter(char character)
	{
		return isalpha(character);
	}

	bool Lexer::isDigit(char character)
	{
		return isdigit(character);
	}

	char Lexer::peekChar()
	{
		return 'f';
	}

	Lexer::Lexer(std::string* input)
	{

	}

	token::Token Lexer::nextToken()
	{
		std::string aa = ";";
		return token::Token(token::SEMICOLON, &aa);
	}
}