#include <string>
#include <ctype.h>

#include "lexer.h"
#include <iostream>

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

		// Remove whitespace and comments so we don't tokenize them

		eatWhiteSpace();
		while (true)
		{
			if (m_currentChar == '-' && peekChar() == '-')
			{
				eatSingleComment();
			}
			else if (m_currentChar == '-' && peekChar() == '*')
			{
				eatMultiComment();
			}
			else 
			{
				break;
			}
			eatWhiteSpace();
		}

		// Generate token based on current (and possibly next) char
		switch(m_currentChar)
		{
		case '=':
			if (peekChar() == '=')
			{
				char prevChar = m_currentChar;
				readChar();
				token = token::Token(token::EQ, std::string{prevChar} + std::string{ m_currentChar });
			}
			else
			{
				token = token::Token(token::ASSIGN, std::string{ m_currentChar });
			}
			break;
		case '+':
			token = token::Token(token::PLUS, std::string{ m_currentChar });
			break;
		case '-':
			token = token::Token(token::MINUS, std::string{ m_currentChar });
			break;
		case '*':
			token = token::Token(token::ASTERIK, std::string{ m_currentChar });
			break;
		case '/':
			token = token::Token(token::SLASH, std::string{ m_currentChar });
			break;
		case '!':
			if (peekChar() == '=')
			{
				char prevChar = m_currentChar;
				readChar();
				token = token::Token(token::NEQ, std::string{ prevChar } + std::string{ m_currentChar });
			}
			else
			{
				token = token::Token(token::BANG, std::string{ m_currentChar });
			}
			break;
		case '&':
			if (peekChar() == '&')
			{
				char prevChar = m_currentChar;
				readChar();
				token = token::Token(token::AND, std::string{ prevChar } + std::string{ m_currentChar });
			}
			else
			{
				token = token::Token(token::AMPERSAND, std::string{ m_currentChar });
			}
			break;
		case '|':
			if (peekChar() == '|')
			{
				char prevChar = m_currentChar;
				readChar();
				token = token::Token(token::OR, std::string{ prevChar } + std::string{ m_currentChar });
			}
			else
			{
				token = token::Token(token::PIPE, std::string{ m_currentChar });
			}
			break;
		case ',':
			token = token::Token(token::COMMA, std::string{ m_currentChar });
			break;
		case ':':
			token = token::Token(token::COLON, std::string{ m_currentChar });
			break;
		case ';':
			token = token::Token(token::SEMICOLON, std::string{ m_currentChar });
			break;
		case '(':
			token = token::Token(token::LPARENTHESIS, std::string{ m_currentChar });
			break;
		case ')':
			token = token::Token(token::RPARENTHESIS, std::string{ m_currentChar });
			break;
		case '{':
			token = token::Token(token::LBRACE, std::string{ m_currentChar });
			break;
		case '}':
			token = token::Token(token::RBRACE, std::string{ m_currentChar });
			break;
		case '[':
			token = token::Token(token::LBRACKET, std::string{ m_currentChar });
			break;
		case ']':
			token = token::Token(token::RBRACKET, std::string{ m_currentChar });
			break;
		case '<':
			if (peekChar() == '=')
			{
				char prevChar = m_currentChar;
				readChar();
				token = token::Token(token::LEQ, std::string{ prevChar } + std::string{ m_currentChar });
			}
			else
			{
				token = token::Token(token::LCHEVRON, std::string{ m_currentChar });
			}
			break;
		case '>':
			if (peekChar() == '=')
			{
				char prevChar = m_currentChar;
				readChar();
				token = token::Token(token::GEQ, std::string{ prevChar } + std::string{ m_currentChar });
			}
			else
			{
				token = token::Token(token::RCHEVRON, std::string{ m_currentChar });
			}
			break;
		case '\'':
		{
			std::string char_literal;
			readCharacter(&char_literal);
			token = token::Token(token::CHARACTER_LITERAL, char_literal);
			break;
		}
		case '"':
		{
			std::string string_literal;
			readString(&string_literal);
			token = token::Token(token::STRING_LITERAL, string_literal);
			break;
		}
		case '\0':
			token = token::Token(token::END_OF_FILE, "");
			break;
		default:
		{
			// Generate a number or identifier
			if (isDigit(m_currentChar))
			{
				token = readNumber();
			}
			else if (isLetter(m_currentChar))
			{
				std::string identifier_literal;
				readIdentifier(&identifier_literal);

				if (token::keywordToTokenType.count(identifier_literal) > 0)
				{
					token = token::Token(token::keywordToTokenType.at(identifier_literal), identifier_literal);
				}
				else
				{
					token = token::Token(token::IDENTIFIER, identifier_literal);
				}
			}
			else
			{
				token = token::Token(token::ILLEGAL, "ILLEGAL");
			}
			break;
		}
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

	void Lexer::eatSingleComment()
	{
		while (m_currentChar != '\n' && m_currentChar != '\0')
		{
			readChar();
		}
		readChar();
	}

	void Lexer::eatMultiComment()
	{
		while (!(m_currentChar == '*' && peekChar() == '-') && m_currentChar != '\0')
		{
			readChar();
		}
		readChar();
		readChar();
	}

	token::Token Lexer::readNumber() 
	{
		int startPosition = m_currentPosition;
		bool seenDecimal = false;
		bool seenF = false;

		// Keep reading characters as long as they're valid numerical values (digits, ., and 'f' for floats)
		while ((isDigit(peekChar()) || peekChar() == '.' || peekChar() == 'f') && peekChar() != '\0')
		{
			if (isDigit(peekChar()))
			{
				readChar();
			}
			else if (peekChar() == '.')
			{
				readChar();
				if (seenDecimal) // We've already seen a decimal
				{
					return token::Token(token::ILLEGAL, "ILLEGAL");
				}
				else
				{
					seenDecimal = true;
				}
			}
			else if (peekChar() == 'f')
			{
				seenF = true;
				bool invalidValue = false;
				readChar();
				while ((isDigit(peekChar()) || peekChar() == '.' || peekChar() == 'f') && peekChar() != '\0')
				{
					invalidValue = true;
					readChar();
				}

				if (invalidValue) // We've seen an 'f' already, found unexpected digits, '.', or 'f'.
				{
					return token::Token(token::ILLEGAL, "ILLEGAL");
				}
			}
		}

		if (seenDecimal && !seenF) // Found a decimal but did not find an 'f'.
		{
			return token::Token(token::ILLEGAL, "ILLEGAL");
		}
		else if (!seenF)
		{
			std::string number_literal = m_input->substr(startPosition, m_nextPosition - startPosition);
			return token::Token(token::INTEGER_LITERAL, number_literal);
		}
		else
		{
			std::string number_literal = m_input->substr(startPosition, m_nextPosition - startPosition);
			return token::Token(token::FLOAT_LITERAL, number_literal);
		}
		
	}

	void Lexer::readIdentifier(std::string* output) 
	{
		int startPosition = m_currentPosition;
		while (isLetter(peekChar()))
		{
			readChar();
		}

		*output = m_input->substr(startPosition, m_nextPosition - startPosition);
	}

	void Lexer::readString(std::string* output) 
	{
		int startPosition = m_currentPosition+1;
		readChar();
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

	void Lexer::readCharacter(std::string* output)
	{
		int startPosition = m_currentPosition + 1;
		readChar();
		while (m_currentChar != '\'' && m_currentChar != '\0')
		{
			readChar();
		}

		if (m_currentChar == '\'')
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