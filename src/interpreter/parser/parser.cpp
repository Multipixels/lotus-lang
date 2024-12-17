#include "parser.h"

namespace parser
{
	Parser::Parser(lexer::Lexer lexer)
		: m_lexer(lexer)
	{
		nextToken();
		nextToken();
	}

	ast::Program* Parser::ParseProgram()
	{
		ast::Program* program = new ast::Program();
		
		while (m_currentToken.m_type != token::END_OF_FILE)
		{
			ast::Statement* statement = parseStatement();

			if (statement != NULL)
			{
				program->m_statements.push_back(statement);
			}

			nextToken();
		}

		return program;
	}

	void Parser::nextToken()
	{
		m_currentToken = m_peekToken;
		m_peekToken = m_lexer.nextToken();
	}

	bool Parser::expectPeek(token::TokenType tokenType)
	{
		if (m_peekToken.m_type == tokenType)
		{
			nextToken();
			return true;
		}
		return false;
	}

	ast::Statement* Parser::parseStatement()
	{
		switch (m_currentToken.m_type)
		{
		case token::INTEGER_TYPE:
			return parseIntegerDeclaration();
		case token::FLOAT_TYPE:
			return parseFloatDeclaration();
		case token::BOOLEAN_TYPE:
			return parseBooleanDeclaration();
		case token::CHARACTER_TYPE:
			return parseCharacterDeclaration();
		case token::RETURN:
			return parseReturnStatement();
		default:
			return parseExpressionStatement();
		}
	}

	ast::DeclareIntegerStatement* Parser::parseIntegerDeclaration()
	{
		ast::DeclareIntegerStatement* statement = new ast::DeclareIntegerStatement;
		statement->m_token = m_currentToken;
		
		if (!expectPeek(token::IDENTIFIER))
		{
			return NULL;
		}

		statement->m_name.m_token = m_currentToken;
		statement->m_name.m_name = m_currentToken.m_literal;

		if (!expectPeek(token::ASSIGN))
		{
			return NULL;
		}
		nextToken();

		statement->m_value = parseExpression();

		if (!expectPeek(token::SEMICOLON))
		{
			return NULL;
		}
		nextToken();

		return statement;
	}

	ast::DeclareFloatStatement* Parser::parseFloatDeclaration()
	{
		ast::DeclareFloatStatement* statement = new ast::DeclareFloatStatement;
		statement->m_token = m_currentToken;

		if (!expectPeek(token::IDENTIFIER))
		{
			return NULL;
		}

		statement->m_name.m_token = m_currentToken;
		statement->m_name.m_name = m_currentToken.m_literal;

		if (!expectPeek(token::ASSIGN))
		{
			return NULL;
		}
		nextToken();

		statement->m_value = parseExpression();

		if (!expectPeek(token::SEMICOLON))
		{
			return NULL;
		}
		nextToken();

		return statement;
	}

	ast::DeclareBooleanStatement* Parser::parseBooleanDeclaration()
	{
		ast::DeclareBooleanStatement* statement = new ast::DeclareBooleanStatement;
		statement->m_token = m_currentToken;

		if (!expectPeek(token::IDENTIFIER))
		{
			return NULL;
		}

		statement->m_name.m_token = m_currentToken;
		statement->m_name.m_name = m_currentToken.m_literal;

		if (!expectPeek(token::ASSIGN))
		{
			return NULL;
		}
		nextToken();

		statement->m_value = parseExpression();

		if (!expectPeek(token::SEMICOLON))
		{
			return NULL;
		}
		nextToken();

		return statement;
	}

	ast::DeclareCharacterStatement* Parser::parseCharacterDeclaration()
	{
		ast::DeclareCharacterStatement* statement = new ast::DeclareCharacterStatement;
		statement->m_token = m_currentToken;

		if (!expectPeek(token::IDENTIFIER))
		{
			return NULL;
		}

		statement->m_name.m_token = m_currentToken;
		statement->m_name.m_name = m_currentToken.m_literal;

		if (!expectPeek(token::ASSIGN))
		{
			return NULL;
		}
		nextToken();

		statement->m_value = parseExpression();

		if (!expectPeek(token::SEMICOLON))
		{
			return NULL;
		}
		nextToken();

		return statement;
	}

	ast::ReturnStatement* Parser::parseReturnStatement()
	{
		ast::ReturnStatement* statement = new ast::ReturnStatement;
		statement->m_token = m_currentToken;

		nextToken();

		statement->m_returnValue = parseExpression();

		if (!expectPeek(token::SEMICOLON))
		{
			return NULL;
		}
		nextToken();

		return statement;
	}

	ast::ExpressionStatement* Parser::parseExpressionStatement()
	{
		ast::ExpressionStatement* statement = new ast::ExpressionStatement;
		statement->m_token = m_currentToken;
		statement->m_expression = parseExpression();

		if (!expectPeek(token::SEMICOLON))
		{
			return NULL;
		}
		nextToken();

		return statement;
	}

	ast::Expression* Parser::parseExpression()
	{
		switch (m_currentToken.m_type) 
		{
		case token::INTEGER_LITERAL:
			return parseIntegerLiteral();
		case token::FLOAT_LITERAL:
			return parseFloatLiteral();
		case token::TRUE_LITERAL:
			return parseBooleanLiteral();
		case token::FALSE_LITERAL:
			return parseBooleanLiteral();
		case token::CHARACTER_LITERAL:
			return parseCharacterLiteral();
		case token::IDENTIFIER:
			return parseIdentifier();
		}
	}

	ast::Expression* Parser::parseIntegerLiteral()
	{
		ast::IntegerLiteral* expression = new ast::IntegerLiteral;
		expression->m_token = m_currentToken;
		expression->m_value = stoi(m_currentToken.m_literal);
		return expression;
	}

	ast::Expression* Parser::parseFloatLiteral()
	{
		ast::FloatLiteral* expression = new ast::FloatLiteral;
		expression->m_token = m_currentToken;
		expression->m_value = stof(m_currentToken.m_literal);
		return expression;
	}

	ast::Expression* Parser::parseBooleanLiteral()
	{
		ast::BooleanLiteral* expression = new ast::BooleanLiteral;
		expression->m_token = m_currentToken;
		expression->m_value = currentTokenIs(token::TRUE_LITERAL);
		return expression;
	}

	ast::Expression* Parser::parseCharacterLiteral()
	{
		ast::CharacterLiteral* expression = new ast::CharacterLiteral;
		expression->m_token = m_currentToken;
		expression->m_value = m_currentToken.m_literal[0];
		return expression;
	}

	ast::Expression* Parser::parseIdentifier()
	{
		ast::Identifier* expression = new ast::Identifier;
		expression->m_token = m_currentToken;
		expression->m_name = m_currentToken.m_literal;
		return expression;
	}

	bool Parser::currentTokenIs(token::TokenType token) {
		return m_currentToken.m_type == token;
	}
}