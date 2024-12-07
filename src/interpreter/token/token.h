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
		NEQ,
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

	const std::map<TokenType, std::string> tokenTypeToString =
	{
		{ILLEGAL, "ILLEGAL"},
		{END_OF_FILE, "END_OF_FILE"},
		{ASSIGN, "EQUALS"},
		{PLUS, "PLUS"},
		{MINUS, "MINUS"},
		{ASTERIK, "ASTERIK"},
		{SLASH, "SLASH"}, 
		{BANG, "BANG"}, 
		{AMPERSAND, "AMPERSAND"},
		{PIPE, "PIPE"},
		{EQ, "EQ"},
		{NEQ, "NEQ"}, 
		{LT, "LT"}, 
		{LEQ, "LEQ"},
		{GT, "GT"}, 
		{GEQ, "GEQ"},
		{COMMA, "COMMA"},
		{COLON, "COLON"},
		{SEMICOLON, "SEMICOLON"},
		{LPARENTHESIS, "LPARENTHESIS"},
		{RPARENTHESIS, "RPARENTHESIS"},
		{LBRACE, "LBRACE"},
		{RBRACE, "RBRACE"},
		{LBRACKET, "LBRACKET"},
		{RBRACKET, "RBRACKET"},
		{INTEGER_TYPE, "INTEGER_TYPE"},
		{BOOLEAN_TYPE, "BOOLEAN_TYPE"},
		{FLOAT_TYPE, "FLOAT_TYPE"},
		{CHARACTER_TYPE, "CHARACTER_TYPE"},
		{COLLECTION_TYPE, "COLLECTION_TYPE"},
		{DICTIONARY_TYPE, "DICTIONARY_TYPE"},
		{STRING_TYPE, "STRING_TYPE"},
		{IF, "IF"},
		{ELSE, "ELSE"},
		{DO, "DO"}, 
		{WHILE, "WHILE"}, 
		{ITERATE, "ITERATE"}, 
		{TRUE_LITERAL, "TRUE_LITERAL"}, 
		{FALSE_LITERAL, "FALSE_LITERAL"},
		{INTEGER_LITERAL, "INTEGER_LITERAL"}, 
		{CHARACTER_LITERAL, "CHARACTER_LITERAL"},
		{STRING_LITERAL, "STRING_LITERAL"},
		{IDENTIFIER, "IDENTIFIER"},
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

	class Token 
	{
	public:
		TokenType m_type;
		std::string m_literal;

		Token();
		Token(TokenType tokenType, std::string m_literal);
	};

	TokenType LookupIdentifier(std::string *identifier);
}