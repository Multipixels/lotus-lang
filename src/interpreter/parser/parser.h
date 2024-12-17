#pragma once

#include "ast.h"
#include "lexer.h"

namespace parser
{
	class Parser
	{
	public:
		lexer::Lexer m_lexer;

		token::Token m_currentToken;
		token::Token m_peekToken;

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

		ast::Expression* parseExpression();
		ast::Expression* parseIntegerLiteral();
		ast::Expression* parseFloatLiteral();
		ast::Expression* parseBooleanLiteral();
		ast::Expression* parseCharacterLiteral();
		ast::Expression* parseIdentifier();

		// HELPERS
		bool currentTokenIs(token::TokenType token);
	};
}