#include "parser.h"


namespace parser
{
	Parser::Parser(lexer::Lexer lexer)
		: m_lexer(lexer)
	{
		nextToken();
		nextToken();

		registerPrefixFunction(token::IDENTIFIER, &Parser::parseIdentifier);
		registerPrefixFunction(token::INTEGER_LITERAL, &Parser::parseIntegerLiteral);
		registerPrefixFunction(token::FLOAT_LITERAL, &Parser::parseFloatLiteral);
		registerPrefixFunction(token::TRUE_LITERAL, &Parser::parseBooleanLiteral);
		registerPrefixFunction(token::FALSE_LITERAL, &Parser::parseBooleanLiteral);
		registerPrefixFunction(token::CHARACTER_LITERAL, &Parser::parseCharacterLiteral);
		registerPrefixFunction(token::BANG, &Parser::parsePrefixExpression);
		registerPrefixFunction(token::MINUS, &Parser::parsePrefixExpression);
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

		statement->m_value = parseExpression(LOWEST);

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

		statement->m_value = parseExpression(LOWEST);

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

		statement->m_value = parseExpression(LOWEST);

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

		statement->m_value = parseExpression(LOWEST);

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

		statement->m_returnValue = parseExpression(LOWEST);

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
		statement->m_expression = parseExpression(LOWEST);

		if (!expectPeek(token::SEMICOLON))
		{
			return NULL;
		}
		nextToken();

		return statement;
	}

	ast::Expression* Parser::parseExpression(Precedence precedence)
	{
		ast::Expression* (Parser:: * prefix)() = m_prefixParseFunctions.at(m_currentToken.m_type);

		if (prefix == NULL)
		{
			return NULL;
		}
		
		// Dereference this member function pointer and call it
		ast::Expression* leftExpression = (this->*prefix)();

		return leftExpression;
	}

	ast::Expression* Parser::parsePrefixExpression()
	{
		ast::PrefixExpression* expression = new ast::PrefixExpression;

		expression->m_token = m_currentToken;
		expression->m_operator = m_currentToken.m_literal;

		nextToken();

		expression->m_right_expression = parseExpression(LOWEST);

		return expression;
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

	void Parser::registerPrefixFunction(token::TokenType tokenType, PrefixParseFunction prefixParseFunction)
	{
		m_prefixParseFunctions[tokenType] = prefixParseFunction;
	}

	void Parser::registerInfixFunction(token::TokenType tokenType, InfixParseFunction infixParseFunction)
	{
		m_infixParseFunctions[tokenType] = infixParseFunction;
	}

	bool Parser::currentTokenIs(token::TokenType token) {
		return m_currentToken.m_type == token;
	}
}