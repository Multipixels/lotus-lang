#pragma once

#include "ast.h"
#include "lexer.h"

namespace parser
{

	class Parser
	{
	public:
		typedef std::shared_ptr<ast::Expression> (Parser::*PrefixParseFunction)();
		typedef std::shared_ptr<ast::Expression> (Parser::*PostfixParseFunction)(std::shared_ptr<ast::Expression>);
		typedef std::shared_ptr<ast::Expression> (Parser::*InfixParseFunction) (std::shared_ptr<ast::Expression>);

		lexer::Lexer m_lexer;

		token::Token m_currentToken;
		token::Token m_peekToken;

		std::map<token::TokenType, PrefixParseFunction> m_prefixParseFunctions;
		std::map<token::TokenType, PostfixParseFunction> m_postfixParseFunctions;
		std::map<token::TokenType, InfixParseFunction> m_infixParseFunctions;

		std::vector<std::string> m_errors;

		Parser(lexer::Lexer p_lexer);
		
		// Parses the program described by the lexer and returns a parsed program
		std::shared_ptr<ast::Program> ParseProgram();
	private:
		typedef enum Precedence
		{
			LOWEST,			// default
			ASSIGNMENT,		// =
			LOGICAL_OR,		// ||
			LOGICAL_AND,	// &&
			EQUALS,			// ==
			LESSGREATER,	// < or >
			SUM,			// a + b
			PRODUCT,		// a * b
			PREFIX,			// -X or !x
			CALL,			// function call, increment/decrements
			INDEX,			// collection indexing
			MEMBER_ACCESS,  // dot operator
		} Precedence;

		const std::map<token::TokenType, Precedence> c_precedenceOfTokenType =
		{
			{token::ASSIGN, ASSIGNMENT},
			{token::PLUS_ASSIGN, ASSIGNMENT},
			{token::MINUS_ASSIGN, ASSIGNMENT},
			{token::ASTERIK_ASSIGN, ASSIGNMENT},
			{token::SLASH_ASSIGN, ASSIGNMENT},
			{token::PERCENT_ASSIGN, ASSIGNMENT},
			{token::OR, LOGICAL_OR},
			{token::AND, LOGICAL_AND},
			{token::EQ, EQUALS},
			{token::NEQ, EQUALS},
			{token::LEQ, LESSGREATER},
			{token::LCHEVRON, LESSGREATER},
			{token::GEQ, LESSGREATER},
			{token::RCHEVRON, LESSGREATER},
			{token::PLUS, SUM},
			{token::MINUS, SUM},
			{token::ASTERIK, PRODUCT},
			{token::SLASH, PRODUCT},
			{token::PERCENT, PRODUCT},
			{token::LPARENTHESIS, CALL},
			{token::INCREMENT, CALL},
			{token::DECREMENT, CALL},
			{token::LBRACKET, INDEX},
			{token::DOT, MEMBER_ACCESS},
		};

		// Cycles through to the next token in the lexer
		void nextToken();

		// Checks if current token type is as expected. If it is, cycle to it
		bool expectCurrent(token::TokenType p_tokenType);

		// Checks if peek token type is as expected. If it is, cycle to it
		bool expectPeek(token::TokenType p_tokenType);

		// Checks if the current token type is as given
		bool currentTokenIs(token::TokenType p_token);

		// Checks if the peek token type is as given
		bool peekTokenIs(token::TokenType p_token);

		// Returns the precedence of the token in front of current token.
		Precedence peekPrecedence();

		// Returns the precedence of the token in the current token.
		Precedence currentPrecedence();

		// ERRORS

		// Peek token is not as expected.
		void expectedPeekError(token::TokenType p_expectedToken);

		// Prefix function not defined.
		void noPrefixParseFunction(token::TokenType p_operatorError);


		// STATEMENTS

		std::shared_ptr<ast::Statement> parseStatement();

		// Removes the semicolon requirement for certain statements - used for 'for' statements
		std::shared_ptr<ast::Statement> parseStatementNoSemicolon(); 

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
		std::shared_ptr<ast::BreakStatement> parseBreakStatement();
		std::shared_ptr<ast::ContinueStatement> parseContinueStatement();

		// EXPRESSIONS

		std::shared_ptr<ast::Expression> parseExpression(Precedence p_precedence);
		std::shared_ptr<ast::Expression> parsePrefixExpression();
		std::shared_ptr<ast::Expression> parsePostfixExpression(std::shared_ptr<ast::Expression> p_leftExpression);
		std::shared_ptr<ast::Expression> parseInfixExpression(std::shared_ptr<ast::Expression> p_leftExpression);
		std::shared_ptr<ast::Expression> parseGroupedExpression();
		std::shared_ptr<ast::Expression> parseIntegerLiteral();
		std::shared_ptr<ast::Expression> parseFloatLiteral();
		std::shared_ptr<ast::Expression> parseBooleanLiteral();
		std::shared_ptr<ast::Expression> parseCharacterLiteral();
		std::shared_ptr<ast::Expression> parseCollectionLiteral();
		std::shared_ptr<ast::Expression> parseDictionaryLiteral();
		std::shared_ptr<ast::Expression> parseStringLiteral();
		std::shared_ptr<ast::Expression> parseIdentifier();
		std::shared_ptr<ast::Expression> parseCallExpression(std::shared_ptr<ast::Expression> p_leftExpression);
		std::shared_ptr<ast::Expression> parseIndexExpression(std::shared_ptr<ast::Expression> p_leftExpression);

		// HELPERS

		// Registers a prefix function into the parser
		void registerPrefixFunction(token::TokenType p_tokenType, PrefixParseFunction p_prefixParseFunction);

		// Registers a postfix function into the parser
		void registerPostfixFunction(token::TokenType p_tokenType, PostfixParseFunction p_postfixParseFunction);
		
		// Registers an infix function into the parser
		void registerInfixFunction(token::TokenType p_tokenType, InfixParseFunction p_infixParseFunction);
	
		void parseParameters(std::vector<std::shared_ptr<ast::DeclareVariableStatement>> *p_parameters);

		// Parses a list of literals, with given separator token and end token.
		void parseLiterals(std::vector<std::shared_ptr<ast::Expression>>* p_destination, token::TokenType p_separator, token::TokenType p_ender);

		// Parses a list of key value pairs, with given separator token and end token.
		void parseKeyValuePairs(std::map< std::shared_ptr<ast::Expression>, std::shared_ptr<ast::Expression>>* p_, token::TokenType p_separator, token::TokenType p_ender);
	};
}