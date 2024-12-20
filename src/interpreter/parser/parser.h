#pragma once

#include "ast.h"
#include "lexer.h"

namespace parser
{

	typedef enum Precedence
	{
		LOWEST,			// default
		EQUALS,			// ==
		LESSGREATER,	// < or >
		SUM,			// a + b
		PRODUCT,		// a * b
		PREFIX,			// -X or !x
		CALL,			// function call
	} Precedence;

	class Parser
	{
	public:
		typedef ast::Expression* (Parser::*PrefixParseFunction)();
		typedef ast::Expression* (Parser::*InfixParseFunction) (ast::Expression*);

		lexer::Lexer m_lexer;

		token::Token m_currentToken;
		token::Token m_peekToken;

		std::map<token::TokenType, PrefixParseFunction> m_prefixParseFunctions;
		std::map<token::TokenType, InfixParseFunction> m_infixParseFunctions;

		Parser(lexer::Lexer lexer);
		
		// Parses the program described by the lexer and returns a parsed program
		ast::Program* ParseProgram();
	private:
		// Cycles through to the next token in the lexer
		void nextToken();

		// Checks if peek token type is as expected. If it is, cycle to it
		bool expectPeek(token::TokenType tokenType);

		// STATEMENTS

		ast::Statement* parseStatement();
		ast::DeclareIntegerStatement* parseIntegerDeclaration();
		ast::DeclareFloatStatement* parseFloatDeclaration();
		ast::DeclareBooleanStatement* parseBooleanDeclaration();
		ast::DeclareCharacterStatement* parseCharacterDeclaration();
		ast::ReturnStatement* parseReturnStatement();
		ast::ExpressionStatement* parseExpressionStatement();

		// EXPRESSIONS

		ast::Expression* parseExpression(Precedence precedence);
		ast::Expression* parsePrefixExpression();
		ast::Expression* parseIntegerLiteral();
		ast::Expression* parseFloatLiteral();
		ast::Expression* parseBooleanLiteral();
		ast::Expression* parseCharacterLiteral();
		ast::Expression* parseIdentifier();

		// HELPERS

		// Registers a prefix function into the parser
		void registerPrefixFunction(token::TokenType tokenType, PrefixParseFunction prefixParseFunction);
		
		// Registers an infix function into the parser
		void registerInfixFunction(token::TokenType tokenType, InfixParseFunction infixParseFunction);
		
		// Checks if the current token type is as given
		bool currentTokenIs(token::TokenType token);
	};
}