#include <sstream>

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
		registerPrefixFunction(token::LPARENTHESIS, &Parser::parseGroupedExpression);

		registerInfixFunction(token::PLUS, &Parser::parseInfixExpression);
		registerInfixFunction(token::MINUS, &Parser::parseInfixExpression);
		registerInfixFunction(token::ASTERIK, &Parser::parseInfixExpression);
		registerInfixFunction(token::SLASH, &Parser::parseInfixExpression);
		registerInfixFunction(token::RCHEVRON, &Parser::parseInfixExpression);
		registerInfixFunction(token::GEQ, &Parser::parseInfixExpression);
		registerInfixFunction(token::LCHEVRON, &Parser::parseInfixExpression);
		registerInfixFunction(token::LEQ, &Parser::parseInfixExpression);
		registerInfixFunction(token::EQ, &Parser::parseInfixExpression);
		registerInfixFunction(token::NEQ, &Parser::parseInfixExpression);
		registerInfixFunction(token::AND, &Parser::parseInfixExpression);
		registerInfixFunction(token::OR, &Parser::parseInfixExpression);
		registerInfixFunction(token::ASSIGN, &Parser::parseInfixExpression);
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

		expectedPeekError(tokenType);
		return false;
	}

	bool Parser::currentTokenIs(token::TokenType token)
	{
		return m_currentToken.m_type == token;
	}

	bool Parser::peekTokenIs(token::TokenType token) 
	{
		return m_peekToken.m_type == token;
	}

	Parser::Precedence Parser::peekPrecedence()
	{
		// Check if precedence table has token, and if it does, return precedence of it.
		if (precedenceOfTokenType.count(m_peekToken.m_type) >= 1)
		{
			return precedenceOfTokenType.at(m_peekToken.m_type);
		}

		return LOWEST;
	}

	Parser::Precedence Parser::currentPrecedence()
	{
		// Check if precedence table has token, and if it does, return precedence of it.
		if (precedenceOfTokenType.count(m_currentToken.m_type) >= 1)
		{
			return precedenceOfTokenType.at(m_currentToken.m_type);
		}

		return LOWEST;
	}

	void Parser::expectedPeekError(token::TokenType expectedToken)
	{
		std::ostringstream error;

		error << "Expected " << token::tokenTypeToString.at(expectedToken)
			<< ". Got " << token::tokenTypeToString.at(m_peekToken.m_type)
			<< " instead.";

		m_errors.push_back(error.str());
	}

	void Parser::noPrefixParseFunction(token::TokenType operatorError)
	{
		std::ostringstream error;

		error << "No prefix function defined for " 
			<< token::tokenTypeToString.at(operatorError)
			<< ".";

		m_errors.push_back(error.str());
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
		case token::IF:
			return parseIfStatement();
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

		return statement;
	}

	ast::BlockStatement* Parser::parseBlockStatement()
	{
		ast::BlockStatement* statement = new ast::BlockStatement;
		statement->m_token = m_currentToken;

		nextToken();

		while (!currentTokenIs(token::RBRACE) && !currentTokenIs(token::END_OF_FILE))
		{
			ast::Statement* subStatement = parseStatement();
			if (subStatement != NULL)
			{
				statement->m_statements.push_back(subStatement);
			}
			nextToken();
		}

		return statement;
	}

	ast::IfStatement* Parser::parseIfStatement()
	{
		ast::IfStatement* statement = new ast::IfStatement;
		statement->m_token = m_currentToken;
		
		if (!expectPeek(token::LPARENTHESIS))
		{
			return NULL;
		}

		nextToken();
		statement->m_condition = parseExpression(LOWEST);

		if (!expectPeek(token::RPARENTHESIS))
		{
			return NULL;
		}

		if (!expectPeek(token::LBRACE))
		{
			return NULL;
		}

		statement->m_consequence = parseBlockStatement();

		statement->m_alternative = 0;

		return statement;
	}

	ast::Expression* Parser::parseExpression(Precedence precedence)
	{
		PrefixParseFunction prefix;
		if (m_prefixParseFunctions.count(m_currentToken.m_type) >= 1)
		{
			prefix = m_prefixParseFunctions.at(m_currentToken.m_type);
		}
		else
		{
			noPrefixParseFunction(m_currentToken.m_type);
			return NULL;
		}
		
		// Dereference this member function pointer and call it
		ast::Expression* leftExpression = (this->*prefix)();

		while (!peekTokenIs(token::SEMICOLON) && precedence < peekPrecedence())
		{
			InfixParseFunction infix = m_infixParseFunctions.at(m_peekToken.m_type);
			
			if (infix == NULL)
			{
				return leftExpression;
			}

			nextToken();

			leftExpression = (this->*infix)(leftExpression);
		}

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

	ast::Expression* Parser::parseInfixExpression(ast::Expression* leftExpression)
	{
		ast::InfixExpression* expression = new ast::InfixExpression;

		expression->m_token = m_currentToken;
		expression->m_operator = m_currentToken.m_literal;
		expression->m_left_expression = leftExpression;

		Precedence precedence = currentPrecedence();
		nextToken();
		expression->m_right_expression = parseExpression(precedence);

		return expression;
	}

	ast::Expression* Parser::parseGroupedExpression()
	{
		nextToken();

		ast::Expression* expression = parseExpression(LOWEST);

		if(!expectPeek(token::RPARENTHESIS))
		{
			return NULL;
		}

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
}