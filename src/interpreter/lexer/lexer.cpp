#include <string>
#include <ctype.h>

#include "lexer.h"

namespace lexer 
{
	Lexer::Lexer(std::string* input)
	{
		m_input = input;
		m_currentChar = '\0';
		m_currentPosition = -1;
		m_nextPosition = 0;

		readChar();
	}

	token::Token Lexer::nextToken()
	{
		token::Token token;

		eatWhiteSpace();

		switch(m_currentChar)
		{
		case '+':
			token = token::Token(token::PLUS, "+");
			break;
		case '-':
			token = token::Token(token::MINUS, "-");
			break;
		case '=':
			token = token::Token(token::ASSIGN, "=");
			break;
		case ';':
			token = token::Token(token::SEMICOLON, ";");
			break;
		case '\0':
			token = token::Token(token::END_OF_FILE, "\0");
			break;
		default:
			// TODO: check integers or identifier
			token = token::Token(token::ILLEGAL, "ILLEGAL");
			break;
		}

		readChar();
		return token;
	}

	void Lexer::readChar() 
	{
		if (m_nextPosition >= m_input->length())
		{
			m_currentChar = '\0';
		}
		else
		{
			m_currentChar = (*m_input)[m_nextPosition];
		}

		m_currentPosition = m_nextPosition;
		m_nextPosition++;
	}

	void Lexer::eatWhiteSpace() 
	{
		while (m_currentChar == ' ' || m_currentChar == '\t' || m_currentChar == '\n' || m_currentChar == '\r')
		{
			readChar();
		}
	}

	int Lexer::readNumber() 
	{
		int startPosition = m_currentPosition;
		while (isDigit(m_currentChar))
		{
			readChar();
		}

		std::string number = m_input->substr(startPosition, m_currentPosition - startPosition);
		return stoi(number);
	}

	void Lexer::readIdentifier(std::string* output) 
	{
		int startPosition = m_currentPosition;
		while (isLetter(m_currentChar))
		{
			readChar();
		}

		*output = m_input->substr(startPosition, m_currentPosition - startPosition);
	}

	void Lexer::readString(std::string* output) 
	{
		int startPosition = m_currentPosition;
		while (m_currentChar != '"' && m_currentChar != '\0')
		{
			readChar();
		}

		if (m_currentChar == '"')
		{
			*output = m_input->substr(startPosition, m_currentPosition - startPosition);
		}
		else if (m_currentChar == '\0')
		{
			// TODO: Handle errors
			*output = m_input->substr(startPosition, m_currentPosition - startPosition);
		}
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
		if (m_nextPosition >= m_input->length())
		{
			return '\0';
		}

		return (*m_input)[m_nextPosition];
	}
}