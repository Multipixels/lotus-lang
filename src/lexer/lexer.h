#pragma once

#include "token.h"

namespace lexer 
{
	class Lexer
	{
	public:
		Lexer(std::string* p_input);

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

		// Skips through single comment (until new line)
		void eatSingleComment();

		// Skips through a multi-lined comment
		void eatMultiComment();

		// Reads numeric value and returns Token.
		token::Token readNumber();

		// Reads an identifier and saves it into the address provided.
		void readIdentifier(std::string* p_output);

		// Reads a string and saves it into the address provided.
		void readString(std::string* p_output);

		// Reads a character literal and saves it into the address provided.
		void readCharacter(std::string* p_output);

		bool isLetter(char p_character);
		bool isDigit(char p_character);

		// Checks if a character is valid in an identifier
		bool validIdentifierChar(char p_character);

		// Returns the character in 'm_nextPosition' without progressing through the input string.
		char peekChar();

	};
}
