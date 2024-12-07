#pragma once

#include "token.h"

namespace lexer 
{
	class Lexer
	{
	public:
		Lexer(std::string* input);

		// Returns the token we are currently on.
		// Increments position to next token.
		token::Token nextToken();

	private:
		std::string* m_input;
		char m_currentChar;
		int m_currentPosition;
		int m_nextPosition;

		// Reads the character in the 'nextPosition' and saves it to 'currentChar'.
		// Increments 'nextPosition'
		void readChar();

		// Skips through whitespace characters, like tabs, spaces, and newlines.
		void eatWhiteSpace();

		// Reads integer value.
		int readNumber();

		// Reads an identifier and saves it into the address provided.
		void readIdentifier(std::string* output);

		// Reads a string and saves it into the address provided.
		void readString(std::string* output);

		bool isLetter(char character);
		bool isDigit(char character);

		// Returns the character in 'm_nextPosition' without progressing through the input string.
		char peekChar();

	};
}