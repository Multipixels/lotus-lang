#pragma once

#include "ast.h"
#include "lexer.h"

namespace parser
{

	class Parser
	{
	public:
		typedef std::shared_ptr<ast::Expression> (Parser::*PrefixParseFunction)();
		typedef std::shared_ptr<ast::Expression> (Parser::*InfixParseFunction) (std::shared_ptr<ast::Expression>);

		lexer::Lexer m_lexer;

		token::Token m_currentToken;
		token::Token m_peekToken;

		std::map<token::TokenType, PrefixParseFunction> m_prefixParseFunctions;
		std::map<token::TokenType, InfixParseFunction> m_infixParseFunctions;

		std::vector<std::string> m_errors;

		Parser(lexer::Lexer lexer);
		
		// Parses the program described by the lexer and returns a parsed program
		std::shared_ptr<ast::Program> ParseProgram();
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
			INDEX,			// collection indexing
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
			{token::LPARENTHESIS, CALL},
			{token::LBRACKET, INDEX},
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

		std::shared_ptr<ast::Statement> parseStatement();
		std::shared_ptr<ast::DeclareVariableStatement> parseVariableDeclaration();
		std::shared_ptr<ast::DeclareCollectionStatement> parseCollectionDeclaration();
		std::shared_ptr<ast::DeclareDictionaryStatement> parseDictionaryDeclaration();
		std::shared_ptr<ast::DeclareFunctionStatement> parseFunctionDeclaration();
		std::shared_ptr<ast::ReturnStatement> parseReturnStatement();
		std::shared_ptr<ast::ExpressionStatement> parseExpressionStatement();
		std::shared_ptr<ast::BlockStatement> parseBlockStatement();
		std::shared_ptr<ast::IfStatement> parseIfStatement();
		std::shared_ptr<ast::IfStatement> parseElseStatement();
		std::shared_ptr<ast::WhileStatement> parseWhileStatement();
		std::shared_ptr<ast::DoWhileStatement> parseDoWhileStatement();
		std::shared_ptr<ast::ForStatement> parseForStatement();
		std::shared_ptr<ast::IterateStatement> parseIterateStatement();

		// EXPRESSIONS

		std::shared_ptr<ast::Expression> parseExpression(Precedence precedence);
		std::shared_ptr<ast::Expression> parsePrefixExpression();
		std::shared_ptr<ast::Expression> parseInfixExpression(std::shared_ptr<ast::Expression> leftExpression);
		std::shared_ptr<ast::Expression> parseGroupedExpression();
		std::shared_ptr<ast::Expression> parseIntegerLiteral();
		std::shared_ptr<ast::Expression> parseFloatLiteral();
		std::shared_ptr<ast::Expression> parseBooleanLiteral();
		std::shared_ptr<ast::Expression> parseCharacterLiteral();
		std::shared_ptr<ast::Expression> parseCollectionLiteral();
		std::shared_ptr<ast::Expression> parseDictionaryLiteral();
		std::shared_ptr<ast::Expression> parseStringLiteral();
		std::shared_ptr<ast::Expression> parseIdentifier();
		std::shared_ptr<ast::Expression> parseCallExpression(std::shared_ptr<ast::Expression> leftExpression);
		std::shared_ptr<ast::Expression> parseIndexExpression(std::shared_ptr<ast::Expression> leftExpression);

		// HELPERS

		// Registers a prefix function into the parser
		void registerPrefixFunction(token::TokenType tokenType, PrefixParseFunction prefixParseFunction);
		
		// Registers an infix function into the parser
		void registerInfixFunction(token::TokenType tokenType, InfixParseFunction infixParseFunction);
	
		void parseParameters(std::vector<std::shared_ptr<ast::DeclareVariableStatement>> *parameters);

		// Parses a list of literals, with given separator token and end token.
		void parseLiterals(std::vector<std::shared_ptr<ast::Expression>>* destination, token::TokenType separator, token::TokenType ender);

		// Parses a list of key value pairs, with given separator token and end token.
		void parseKeyValuePairs(std::map< std::shared_ptr<ast::Expression>, std::shared_ptr<ast::Expression>>* destination, token::TokenType separator, token::TokenType ender);
	};
}