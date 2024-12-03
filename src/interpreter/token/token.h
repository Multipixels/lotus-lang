#pragma once

#include <string>
#include <map>

namespace token
{
	enum TokenType
	{
		ILLEGAL,	// Unexpected character
		END_OF_FILE,

		// Operators
		ASSIGN,
		PLUS,
		MINUS,
		ASTERIK,
		SLASH,
		BANG,
		AMPERSAND,
		PIPE,

		EQ,
		NOT_EQ,
		LT,
		LEQ,
		GT,
		GEQ,

		// Delimiters
		COMMA,
		COLON,
		SEMICOLON,

		LPARENTHESIS,
		RPARENTHESIS,
		LBRACE,
		RBRACE,
		LBRACKET,
		RBRACKET,

		// Keywords
		INTEGER_TYPE,
		BOOLEAN_TYPE,
		FLOAT_TYPE,
		CHARACTER_TYPE,
		COLLECTION_TYPE,
		DICTIONARY_TYPE,
		STRING_TYPE,
		IF,
		ELSE,
		DO,
		WHILE,
		ITERATE,

		// Literals
		TRUE_LITERAL,
		FALSE_LITERAL,
		INTEGER_LITERAL,
		CHARACTER_LITERAL,
		STRING_LITERAL,

		IDENTIFIER,
	};

	const std::map<std::string, TokenType> keywordToTokenType =
	{
		{"true", TRUE_LITERAL},
		{"false", FALSE_LITERAL},
		{"if", IF},
		{"else", ELSE},
		{"do", DO},
		{"while", WHILE},
		{"iterate", ITERATE},
	};

	typedef struct Token 
	{
		TokenType tokenType;
		std::string Literal;
	} Token;

	TokenType LookupIdentifier(std::string *identifier);
}