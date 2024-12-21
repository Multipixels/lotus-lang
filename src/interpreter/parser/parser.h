#pragma once

#include "ast.h"
#include "lexer.h"

namespace parser
{

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

		std::vector<std::string> m_errors;

		Parser(lexer::Lexer lexer);
		
		// Parses the program described by the lexer and returns a parsed program
		ast::Program* ParseProgram();
	private:
		typedef enum Precedence
		{
			LOWEST,			// default
			ASSIGNMENT,		// =
			EQUALS,			// ==
			LESSGREATER,	// < or >
			SUM,			// a + b
			PRODUCT,		// a * b
			PREFIX,			// -X or !x
			CALL,			// function call
		} Precedence;

		const std::map<token::TokenType, Precedence> precedenceOfTokenType =
		{
			{token::ASSIGN, ASSIGNMENT},
			{token::EQ, EQUALS},
			{token::NEQ, EQUALS},
			{token::LEQ, LESSGREATER},
			{token::LCHEVRON, LESSGREATER},
			{token::GEQ, LESSGREATER},
			{token::RCHEVRON, LESSGREATER},
			{token::PLUS, SUM},
			{token::MINUS, SUM},
			{token::OR, SUM},
			{token::ASTERIK, PRODUCT},
			{token::SLASH, PRODUCT},
			{token::AND, PRODUCT},
		};

		// Cycles through to the next token in the lexer
		void nextToken();

		// Checks if peek token type is as expected. If it is, cycle to it
		bool expectPeek(token::TokenType tokenType);

		// Checks if the current token type is as given
		bool currentTokenIs(token::TokenType token);

		// Checks if the peek token type is as given
		bool peekTokenIs(token::TokenType token);

		// Returns the precedence of the token in front of current token.
		Precedence peekPrecedence();

		// Returns the precedence of the token in the current token.
		Precedence currentPrecedence();

		// ERRORS

		// Peek token is not as expected.
		void expectedPeekError(token::TokenType expectedToken);

		// Prefix function not defined.
		void noPrefixParseFunction(token::TokenType operatorError);


		// STATEMENTS

		ast::Statement* parseStatement();
		ast::DeclareIntegerStatement* parseIntegerDeclaration();
		ast::DeclareFloatStatement* parseFloatDeclaration();
		ast::DeclareBooleanStatement* parseBooleanDeclaration();
		ast::DeclareCharacterStatement* parseCharacterDeclaration();
		ast::ReturnStatement* parseReturnStatement();
		ast::ExpressionStatement* parseExpressionStatement();
		ast::BlockStatement* parseBlockStatement();
		ast::IfStatement* parseIfStatement();
		ast::IfStatement* parseElseStatement();

		// EXPRESSIONS

		ast::Expression* parseExpression(Precedence precedence);
		ast::Expression* parsePrefixExpression();
		ast::Expression* parseInfixExpression(ast::Expression* leftExpression);
		ast::Expression* parseGroupedExpression();
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
	};
}