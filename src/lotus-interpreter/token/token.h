#pragma once

#include <string>
#include <map>

namespace token
{
	enum TokenType
	{
		ILLEGAL,	// Unexpected character
		ILLEGAL_NUMERIC, // Unable to lex numeric value
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

		AND,
		OR,

		EQ,
		NEQ,
		LEQ,
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
		LCHEVRON,
		RCHEVRON,

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
		FOR,
		ITERATE,
		RETURN,

		// Literals
		TRUE_LITERAL,
		FALSE_LITERAL,
		INTEGER_LITERAL,
		FLOAT_LITERAL,
		CHARACTER_LITERAL,
		STRING_LITERAL,

		IDENTIFIER,
	};

	const std::map<TokenType, std::string> tokenTypeToString =
	{
		{ILLEGAL, "ILLEGAL"},
		{ILLEGAL_NUMERIC, "ILLEGAL_NUMERIC"},
		{END_OF_FILE, "END_OF_FILE"},
		{ASSIGN, "EQUALS"},
		{PLUS, "PLUS"},
		{MINUS, "MINUS"},
		{ASTERIK, "ASTERIK"},
		{SLASH, "SLASH"}, 
		{BANG, "BANG"}, 
		{AMPERSAND, "AMPERSAND"},
		{PIPE, "PIPE"},
		{AND, "AND"},
		{OR, "OR"},
		{EQ, "EQ"},
		{NEQ, "NEQ"}, 
		{LEQ, "LEQ"},
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
		{LCHEVRON, "LCHEVRON"},
		{RCHEVRON, "RCHEVRON"},
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
		{FOR, "FOR"},
		{ITERATE, "ITERATE"}, 
		{RETURN, "RETURN"},
		{TRUE_LITERAL, "TRUE_LITERAL"}, 
		{FALSE_LITERAL, "FALSE_LITERAL"},
		{INTEGER_LITERAL, "INTEGER_LITERAL"},
		{FLOAT_LITERAL, "FLOAT_LITERAL"},
		{CHARACTER_LITERAL, "CHARACTER_LITERAL"},
		{STRING_LITERAL, "STRING_LITERAL"},
		{IDENTIFIER, "IDENTIFIER"},
	};

	const std::map<std::string, TokenType> keywordToTokenType =
	{ 
		{"integer", INTEGER_TYPE},
		{"boolean", BOOLEAN_TYPE},
		{"float", FLOAT_TYPE},
		{"character", CHARACTER_TYPE},
		{"collection", COLLECTION_TYPE},
		{"dictionary", DICTIONARY_TYPE},
		{"string", STRING_TYPE},
		{"if", IF},
		{"else", ELSE},
		{"do", DO},
		{"while", WHILE},
		{"for", FOR},
		{"iterate", ITERATE},
		{"return", RETURN},
		{"true", TRUE_LITERAL},
		{"false", FALSE_LITERAL},
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