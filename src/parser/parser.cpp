#include <sstream>

#include "parser.h"


namespace parser
{
	Parser::Parser(lexer::Lexer p_lexer)
		: m_lexer(p_lexer)
	{
		nextToken();
		nextToken();

		registerPrefixFunction(token::IDENTIFIER, &Parser::parseIdentifier);
		registerPrefixFunction(token::INTEGER_LITERAL, &Parser::parseIntegerLiteral);
		registerPrefixFunction(token::FLOAT_LITERAL, &Parser::parseFloatLiteral);
		registerPrefixFunction(token::TRUE_LITERAL, &Parser::parseBooleanLiteral);
		registerPrefixFunction(token::FALSE_LITERAL, &Parser::parseBooleanLiteral);
		registerPrefixFunction(token::CHARACTER_LITERAL, &Parser::parseCharacterLiteral);
		registerPrefixFunction(token::STRING_LITERAL, &Parser::parseStringLiteral);
		registerPrefixFunction(token::LBRACKET, &Parser::parseCollectionLiteral);
		registerPrefixFunction(token::LBRACE, &Parser::parseDictionaryLiteral);
		registerPrefixFunction(token::BANG, &Parser::parsePrefixExpression);
		registerPrefixFunction(token::MINUS, &Parser::parsePrefixExpression);
		registerPrefixFunction(token::LPARENTHESIS, &Parser::parseGroupedExpression);

		registerPostfixFunction(token::INCREMENT, &Parser::parsePostfixExpression);
		registerPostfixFunction(token::DECREMENT, &Parser::parsePostfixExpression);

		registerInfixFunction(token::PLUS, &Parser::parseInfixExpression);
		registerInfixFunction(token::MINUS, &Parser::parseInfixExpression);
		registerInfixFunction(token::ASTERIK, &Parser::parseInfixExpression);
		registerInfixFunction(token::SLASH, &Parser::parseInfixExpression);
		registerInfixFunction(token::PERCENT, &Parser::parseInfixExpression);
		registerInfixFunction(token::RCHEVRON, &Parser::parseInfixExpression);
		registerInfixFunction(token::GEQ, &Parser::parseInfixExpression);
		registerInfixFunction(token::LCHEVRON, &Parser::parseInfixExpression);
		registerInfixFunction(token::LEQ, &Parser::parseInfixExpression);
		registerInfixFunction(token::EQ, &Parser::parseInfixExpression);
		registerInfixFunction(token::NEQ, &Parser::parseInfixExpression);
		registerInfixFunction(token::AND, &Parser::parseInfixExpression);
		registerInfixFunction(token::OR, &Parser::parseInfixExpression);
		registerInfixFunction(token::ASSIGN, &Parser::parseInfixExpression);
		registerInfixFunction(token::PLUS_ASSIGN, &Parser::parseInfixExpression);
		registerInfixFunction(token::MINUS_ASSIGN, &Parser::parseInfixExpression);
		registerInfixFunction(token::ASTERIK_ASSIGN, &Parser::parseInfixExpression);
		registerInfixFunction(token::SLASH_ASSIGN, &Parser::parseInfixExpression);
		registerInfixFunction(token::PERCENT_ASSIGN, &Parser::parseInfixExpression);
		registerInfixFunction(token::LPARENTHESIS, &Parser::parseCallExpression);
		registerInfixFunction(token::LBRACKET, &Parser::parseIndexExpression);
		registerInfixFunction(token::DOT, &Parser::parseInfixExpression);
	}

	std::shared_ptr<ast::Program> Parser::ParseProgram()
	{
		std::shared_ptr<ast::Program> program(new ast::Program);
		
		while (m_currentToken.m_type != token::END_OF_FILE)
		{
			std::shared_ptr<ast::Statement> statement = parseStatement();

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

	bool Parser::expectCurrent(token::TokenType p_tokenType)
	{
		if (m_currentToken.m_type == p_tokenType)
		{
			return true;
		}

		expectedPeekError(p_tokenType);
		return false;
	}

	bool Parser::expectPeek(token::TokenType p_tokenType)
	{
		if (m_peekToken.m_type == p_tokenType)
		{
			nextToken();
			return true;
		}

		expectedPeekError(p_tokenType);
		return false;
	}

	bool Parser::currentTokenIs(token::TokenType p_token)
	{
		return m_currentToken.m_type == p_token;
	}

	bool Parser::peekTokenIs(token::TokenType p_token) 
	{
		return m_peekToken.m_type == p_token;
	}

	Parser::Precedence Parser::peekPrecedence()
	{
		// Check if precedence table has token, and if it does, return precedence of it.
		if (c_precedenceOfTokenType.count(m_peekToken.m_type) >= 1)
		{
			return c_precedenceOfTokenType.at(m_peekToken.m_type);
		}

		return LOWEST;
	}

	Parser::Precedence Parser::currentPrecedence()
	{
		// Check if precedence table has token, and if it does, return precedence of it.
		if (c_precedenceOfTokenType.count(m_currentToken.m_type) >= 1)
		{
			return c_precedenceOfTokenType.at(m_currentToken.m_type);
		}

		return LOWEST;
	}

	void Parser::expectedPeekError(token::TokenType p_expectedToken)
	{
		std::ostringstream error;

		error << "Expected " << token::c_tokenTypeToString.at(p_expectedToken)
			<< ". Got " << token::c_tokenTypeToString.at(m_peekToken.m_type)
			<< " instead.";

		m_errors.push_back(error.str());
	}

	void Parser::noPrefixParseFunction(token::TokenType p_operatorError)
	{
		std::ostringstream error;

		error << "No prefix function defined for " 
			<< token::c_tokenTypeToString.at(p_operatorError)
			<< ".";

		m_errors.push_back(error.str());
	}

	std::shared_ptr<ast::Statement> Parser::parseStatement()
	{
		std::shared_ptr<ast::Statement> output;
		switch (m_currentToken.m_type)
		{
		case token::INTEGER_TYPE:
		case token::FLOAT_TYPE:
		case token::BOOLEAN_TYPE:
		case token::CHARACTER_TYPE:
		case token::STRING_TYPE:
			if (peekTokenIs(token::LPARENTHESIS))
			{
				output = parseFunctionDeclaration();
			}
			else
			{
				output = parseVariableDeclaration();
				if (!expectPeek(token::SEMICOLON)) return NULL;
			}
			return output;

		case token::COLLECTION_TYPE:
			output = parseCollectionDeclaration();

			if (!expectPeek(token::SEMICOLON)) return NULL;
			return output;
		case token::DICTIONARY_TYPE:
			output = parseDictionaryDeclaration();

			if (!expectPeek(token::SEMICOLON)) return NULL;
			return output;
		case token::RETURN:   return parseReturnStatement();
		case token::IF:       return parseIfStatement();
		case token::WHILE:    return parseWhileStatement();
		case token::DO:       return parseDoWhileStatement();
		case token::FOR:      return parseForStatement();
		case token::ITERATE:  return parseIterateStatement();
		case token::BREAK:    return parseBreakStatement();
		case token::CONTINUE: return parseContinueStatement();
		default:
			output = parseExpressionStatement();

			if (!expectPeek(token::SEMICOLON)) return NULL;
			return output;
		}
	}

	std::shared_ptr<ast::Statement> Parser::parseStatementNoSemicolon()
	{
		std::shared_ptr<ast::Statement> output;
		switch (m_currentToken.m_type)
		{
		case token::INTEGER_TYPE:
		case token::FLOAT_TYPE:
		case token::BOOLEAN_TYPE:
		case token::CHARACTER_TYPE:
		case token::STRING_TYPE:
			if (peekTokenIs(token::LPARENTHESIS)) return parseFunctionDeclaration();
			else return parseVariableDeclaration();
		case token::COLLECTION_TYPE:
			return parseCollectionDeclaration();
		case token::DICTIONARY_TYPE:
			return parseDictionaryDeclaration();
		case token::RETURN:
			return parseReturnStatement();
		case token::IF:
			return parseIfStatement();
		case token::WHILE:
			return parseWhileStatement();
		case token::DO:
			return parseDoWhileStatement();
		case token::FOR:
			return parseForStatement();
		case token::ITERATE:
			return parseIterateStatement();
		default:
			return parseExpressionStatement();
		}
	}

	std::shared_ptr<ast::DeclareVariableStatement> Parser::parseVariableDeclaration()
	{
		std::shared_ptr<ast::DeclareVariableStatement> statement(new ast::DeclareVariableStatement);
		statement->m_token = m_currentToken;

		if (!expectPeek(token::IDENTIFIER))
		{
			return NULL;
		}

		statement->m_name.m_token = m_currentToken;
		statement->m_name.m_name = m_currentToken.m_literal;

		// Declaration without assignment
		if (peekTokenIs(token::SEMICOLON))
		{
			return statement;
		}

		// Declaration with assignment
		if (!expectPeek(token::ASSIGN))
		{
			return NULL;
		}
		nextToken();

		statement->m_value = parseExpression(LOWEST);

		return statement;
	};

	std::shared_ptr<ast::DeclareCollectionStatement> Parser::parseCollectionDeclaration()
	{
		std::shared_ptr<ast::DeclareCollectionStatement> statement(new ast::DeclareCollectionStatement);
		statement->m_token = m_currentToken;

		if (!expectPeek(token::LCHEVRON))
		{
			return NULL;
		}
		nextToken();

		statement->m_typeToken = m_currentToken;

		if (!expectPeek(token::RCHEVRON))
		{
			return NULL;
		}

		if (!expectPeek(token::IDENTIFIER))
		{
			return NULL;
		}

		statement->m_name.m_token = m_currentToken;
		statement->m_name.m_name = m_currentToken.m_literal;

		// Declaration without assignment
		if (peekTokenIs(token::SEMICOLON))
		{
			return statement;
		}

		// Declaration with assignment
		if (!expectPeek(token::ASSIGN))
		{
			return NULL;
		}
		nextToken();

		statement->m_value = parseExpression(LOWEST);

		return statement;
	};

	std::shared_ptr<ast::DeclareDictionaryStatement> Parser::parseDictionaryDeclaration()
	{
		std::shared_ptr<ast::DeclareDictionaryStatement> statement(new ast::DeclareDictionaryStatement);
		statement->m_token = m_currentToken;

		if (!expectPeek(token::LCHEVRON))
		{
			return NULL;
		}
		nextToken();

		statement->m_keyTypeToken = m_currentToken;

		if (!expectPeek(token::COMMA))
		{
			return NULL;
		}
		nextToken();

		statement->m_valueTypeToken = m_currentToken;

		if (!expectPeek(token::RCHEVRON))
		{
			return NULL;
		}

		if (!expectPeek(token::IDENTIFIER))
		{
			return NULL;
		}

		statement->m_name.m_token = m_currentToken;
		statement->m_name.m_name = m_currentToken.m_literal;

		// Declaration without assignment
		if (peekTokenIs(token::SEMICOLON))
		{
			return statement;
		}

		// Declaration with assignment
		if (!expectPeek(token::ASSIGN))
		{
			return NULL;
		}
		nextToken();

		statement->m_value = parseExpression(LOWEST);

		return statement;
	};

	std::shared_ptr<ast::DeclareFunctionStatement> Parser::parseFunctionDeclaration()
	{
		std::shared_ptr<ast::DeclareFunctionStatement> statement(new ast::DeclareFunctionStatement);
		statement->m_token = m_currentToken;

		if (!expectPeek(token::LPARENTHESIS))
		{
			return NULL;
		}

		parseParameters(&statement->m_parameters);

		if (!expectPeek(token::RPARENTHESIS))
		{
			return NULL;
		}

		if (!expectPeek(token::IDENTIFIER))
		{
			return NULL;
		}

		statement->m_name = *std::static_pointer_cast<ast::Identifier>(parseIdentifier());

		if (!expectPeek(token::LBRACE))
		{
			return NULL;
		}

		statement->m_body = std::shared_ptr<ast::FunctionLiteral>(new ast::FunctionLiteral);
		statement->m_body->m_token = m_currentToken;
		statement->m_body->m_body = parseBlockStatement();

		if (!expectCurrent(token::RBRACE))
		{
			return NULL;
		}

		return statement;
	}

	std::shared_ptr<ast::ReturnStatement> Parser::parseReturnStatement()
	{
		std::shared_ptr<ast::ReturnStatement> statement(new ast::ReturnStatement);
		statement->m_token = m_currentToken;

		nextToken();

		statement->m_returnValue = parseExpression(LOWEST);

		if (!expectPeek(token::SEMICOLON))
		{
			return NULL;
		}

		return statement;
	}

	std::shared_ptr<ast::ExpressionStatement> Parser::parseExpressionStatement()
	{
		std::shared_ptr<ast::ExpressionStatement> statement(new ast::ExpressionStatement);
		statement->m_token = m_currentToken;
		statement->m_expression = parseExpression(LOWEST);

		return statement;
	}

	std::shared_ptr<ast::BlockStatement> Parser::parseBlockStatement()
	{
		std::shared_ptr<ast::BlockStatement> statement(new ast::BlockStatement);
		statement->m_token = m_currentToken;

		nextToken();

		while (!currentTokenIs(token::RBRACE) && !currentTokenIs(token::END_OF_FILE))
		{
			std::shared_ptr<ast::Statement> subStatement = parseStatement();
			if (subStatement != NULL)
			{
				statement->m_statements.push_back(subStatement);
			}
			nextToken();
		}

		return statement;
	}

	std::shared_ptr<ast::IfStatement> Parser::parseIfStatement()
	{
		std::shared_ptr<ast::IfStatement> statement(new ast::IfStatement);
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

		// Handling else ifs and elses
		if (peekTokenIs(token::ELSE))
		{
			nextToken();

			statement->m_alternative = parseElseStatement();
			return statement;
		}
		statement->m_alternative = 0;
		
		if (!expectCurrent(token::RBRACE))
		{
			return NULL;
		}

		return statement;
	}

	std::shared_ptr<ast::IfStatement> Parser::parseElseStatement()
	{
		if (peekTokenIs(token::IF))
		{
			nextToken();
			return parseIfStatement();
		}

		std::shared_ptr<ast::IfStatement> statement(new ast::IfStatement);
		statement->m_token = m_currentToken;

		if (!expectPeek(token::LBRACE))
		{
			return NULL;
		}

		statement->m_condition = NULL;
		statement->m_consequence = parseBlockStatement();
		statement->m_alternative = NULL;

		if (!expectCurrent(token::RBRACE))
		{
			return NULL;
		}

		return statement;
	}

	std::shared_ptr<ast::WhileStatement> Parser::parseWhileStatement()
	{
		std::shared_ptr<ast::WhileStatement> statement(new ast::WhileStatement);
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

		if (!expectCurrent(token::RBRACE))
		{
			return NULL;
		}

		return statement;
	}

	std::shared_ptr<ast::DoWhileStatement> Parser::parseDoWhileStatement()
	{
		std::shared_ptr<ast::DoWhileStatement> statement(new ast::DoWhileStatement);
		statement->m_token = m_currentToken;

		if (!expectPeek(token::LBRACE))
		{
			return NULL;
		}

		statement->m_consequence = parseBlockStatement();

		if (!expectCurrent(token::RBRACE))
		{
			return NULL;
		}

		if (!expectPeek(token::WHILE))
		{
			return NULL;
		}

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

		if (!expectPeek(token::SEMICOLON))
		{
			return NULL;
		}

		return statement;
	}

	std::shared_ptr<ast::ForStatement> Parser::parseForStatement()
	{
		std::shared_ptr<ast::ForStatement> statement(new ast::ForStatement);
		statement->m_token = m_currentToken;

		if (!expectPeek(token::LPARENTHESIS))
		{
			return NULL;
		}
		nextToken();

		if (!currentTokenIs(token::SEMICOLON)) statement->m_initialization = parseStatement();
		nextToken();

		statement->m_condition = parseStatement();

		if (peekTokenIs(token::RPARENTHESIS)) 
		{
			statement->m_updation = NULL;
		}
		else
		{
			nextToken();
			statement->m_updation = parseStatementNoSemicolon();
		}

		if (!expectPeek(token::RPARENTHESIS))
		{
			return NULL;
		}

		if (!expectPeek(token::LBRACE))
		{
			return NULL;
		}

		statement->m_consequence = parseBlockStatement();

		if (!expectCurrent(token::RBRACE))
		{
			return NULL;
		}

		return statement;
	}

	std::shared_ptr<ast::IterateStatement> Parser::parseIterateStatement()
	{
		std::shared_ptr<ast::IterateStatement> statement(new ast::IterateStatement);
		statement->m_token = m_currentToken;

		if (!expectPeek(token::LPARENTHESIS))
		{
			return NULL;
		}
		nextToken();

		statement->m_var = std::static_pointer_cast<ast::Identifier>(parseIdentifier());

		if (!expectPeek(token::COLON))
		{
			return NULL;
		}
		nextToken();

		statement->m_collection = parseExpression(LOWEST);

		if (!expectPeek(token::RPARENTHESIS))
		{
			return NULL;
		}

		if (!expectPeek(token::LBRACE))
		{
			return NULL;
		}

		statement->m_consequence = parseBlockStatement();

		if (!expectCurrent(token::RBRACE))
		{
			return NULL;
		}

		return statement;
	}

	std::shared_ptr<ast::BreakStatement> Parser::parseBreakStatement()
	{
		std::shared_ptr<ast::BreakStatement> statement(new ast::BreakStatement);
		statement->m_token = m_currentToken;

		if (!expectPeek(token::SEMICOLON))
		{
			return NULL;
		}

		return statement;
	}

	std::shared_ptr<ast::ContinueStatement> Parser::parseContinueStatement()
	{
		std::shared_ptr<ast::ContinueStatement> statement(new ast::ContinueStatement);
		statement->m_token = m_currentToken;

		if (!expectPeek(token::SEMICOLON))
		{
			return NULL;
		}

		return statement;
	}

	std::shared_ptr<ast::Expression> Parser::parseExpression(Precedence p_precedence)
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
		std::shared_ptr<ast::Expression> leftExpression = (this->*prefix)();

		while (!peekTokenIs(token::SEMICOLON) && p_precedence < peekPrecedence())
		{
			InfixParseFunction infix = m_infixParseFunctions[m_peekToken.m_type];
			
			if (infix != NULL)
			{
				nextToken();

				leftExpression = (this->*infix)(leftExpression);
				continue;
			}

			PostfixParseFunction postfix = m_postfixParseFunctions[m_peekToken.m_type];
			if (postfix != NULL)
			{
				nextToken();

				leftExpression = (this->*postfix)(leftExpression);
				continue;
			}

			return leftExpression;
		}

		return leftExpression;
	}

	std::shared_ptr<ast::Expression> Parser::parsePrefixExpression()
	{
		std::shared_ptr<ast::PrefixExpression> expression(new ast::PrefixExpression);

		expression->m_token = m_currentToken;
		expression->m_operator = m_currentToken.m_literal;

		nextToken();

		expression->m_rightExpression = parseExpression(PREFIX);

		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parsePostfixExpression(std::shared_ptr<ast::Expression> p_leftExpression)
	{
		std::shared_ptr<ast::PostfixExpression> expression(new ast::PostfixExpression);

		expression->m_token = m_currentToken;
		expression->m_operator = m_currentToken.m_literal;
		expression->m_leftExpression = p_leftExpression;

		Precedence precedence = currentPrecedence();

		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parseInfixExpression(std::shared_ptr<ast::Expression> p_leftExpression)
	{
		std::shared_ptr<ast::InfixExpression> expression(new ast::InfixExpression);

		expression->m_token = m_currentToken;
		expression->m_operator = m_currentToken.m_literal;
		expression->m_leftExpression = p_leftExpression;

		Precedence precedence = currentPrecedence();
		nextToken();
		expression->m_rightExpression = parseExpression(precedence);

		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parseGroupedExpression()
	{
		nextToken();

		std::shared_ptr<ast::Expression> expression = parseExpression(LOWEST);

		if(!expectPeek(token::RPARENTHESIS))
		{
			return NULL;
		}

		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parseIntegerLiteral()
	{
		std::shared_ptr<ast::IntegerLiteral> expression(new ast::IntegerLiteral);
		expression->m_token = m_currentToken;
		expression->m_value = stoi(m_currentToken.m_literal);
		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parseFloatLiteral()
	{
		std::shared_ptr<ast::FloatLiteral> expression(new ast::FloatLiteral);
		expression->m_token = m_currentToken;
		expression->m_value = stof(m_currentToken.m_literal);
		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parseBooleanLiteral()
	{
		std::shared_ptr<ast::BooleanLiteral> expression(new ast::BooleanLiteral);
		expression->m_token = m_currentToken;
		expression->m_value = currentTokenIs(token::TRUE_LITERAL);
		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parseCharacterLiteral()
	{
		if (m_currentToken.m_literal.size() != 1) {
			std::ostringstream error;

			error << "Expected to see a single character. Got " 
				<< m_currentToken.m_literal.size()
				<< " instead.";

			m_errors.push_back(error.str());
		}

		std::shared_ptr<ast::CharacterLiteral> expression(new ast::CharacterLiteral);
		expression->m_token = m_currentToken;
		expression->m_value = m_currentToken.m_literal[0];
		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parseCollectionLiteral()
	{
		std::shared_ptr<ast::CollectionLiteral> expression(new ast::CollectionLiteral);
		expression->m_token = m_currentToken;
		parseLiterals(&expression->m_values, token::COMMA, token::RBRACKET);
		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parseDictionaryLiteral()
	{
		std::shared_ptr<ast::DictionaryLiteral> expression(new ast::DictionaryLiteral);
		expression->m_token = m_currentToken;
		parseKeyValuePairs(&expression->m_map, token::COMMA, token::RBRACE);
		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parseStringLiteral()
	{
		std::shared_ptr<ast::StringLiteral> stringLiteral(new ast::StringLiteral);
		stringLiteral->m_token = m_currentToken;
		stringLiteral->m_stringCollection = std::shared_ptr<ast::CollectionLiteral>(new ast::CollectionLiteral);

		for (int i = 0; i < m_currentToken.m_literal.size(); i++)
		{
			std::shared_ptr<ast::CharacterLiteral> expression(new ast::CharacterLiteral);

			expression->m_token.m_type = token::CHARACTER_LITERAL;
			expression->m_token.m_literal = m_currentToken.m_literal[i];
			expression->m_value = m_currentToken.m_literal[i];

			stringLiteral->m_stringCollection->m_values.push_back(expression);
		}

		return stringLiteral;
	}

	std::shared_ptr<ast::Expression> Parser::parseIdentifier()
	{
		std::shared_ptr<ast::Identifier> expression(new ast::Identifier);
		expression->m_token = m_currentToken;
		expression->m_name = m_currentToken.m_literal;
		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parseCallExpression(std::shared_ptr<ast::Expression> p_leftExpression)
	{
		std::shared_ptr<ast::CallExpression> expression(new ast::CallExpression);
		expression->m_token = m_currentToken;
		expression->m_function = p_leftExpression;
		parseLiterals(&expression->m_parameters, token::COMMA, token::RPARENTHESIS);

		return expression;
	}

	std::shared_ptr<ast::Expression> Parser::parseIndexExpression(std::shared_ptr<ast::Expression> p_leftExpression)
	{
		std::shared_ptr<ast::IndexExpression> expression(new ast::IndexExpression);
		expression->m_token = m_currentToken;
		expression->m_collection = p_leftExpression;

		nextToken();
		expression->m_index = parseExpression(LOWEST);

		if (!expectPeek(token::RBRACKET))
		{
			return NULL;
		}

		return expression;
	}

	void Parser::registerPrefixFunction(token::TokenType p_tokenType, PrefixParseFunction p_prefixParseFunction)
	{
		m_prefixParseFunctions[p_tokenType] = p_prefixParseFunction;
	}

	void Parser::registerPostfixFunction(token::TokenType p_tokenType, PostfixParseFunction p_postfixParseFunction)
	{
		m_postfixParseFunctions[p_tokenType] = p_postfixParseFunction;
	}

	void Parser::registerInfixFunction(token::TokenType p_tokenType, InfixParseFunction p_infixParseFunction)
	{
		m_infixParseFunctions[p_tokenType] = p_infixParseFunction;
	}

	void Parser::parseParameters(std::vector<std::shared_ptr<ast::DeclareVariableStatement>>* p_parameters)
	{
		while (!peekTokenIs(token::RPARENTHESIS))
		{
			nextToken();

			std::shared_ptr<ast::DeclareVariableStatement> statement(new ast::DeclareVariableStatement);
			statement->m_token = m_currentToken;

			if (!expectPeek(token::IDENTIFIER))
			{
				p_parameters->push_back(NULL);
				continue;
			}

			statement->m_name.m_token = m_currentToken;
			statement->m_name.m_name = m_currentToken.m_literal;
			statement->m_value = NULL;

			if (peekTokenIs(token::COMMA))
			{
				nextToken();
			}

			p_parameters->push_back(statement);
		}
	}

	void Parser::parseLiterals(std::vector<std::shared_ptr<ast::Expression>>* p_destination, token::TokenType p_separator, token::TokenType p_ender)
	{
		while (!peekTokenIs(p_ender) && m_currentToken.m_type != token::END_OF_FILE)
		{
			nextToken();

			std::shared_ptr<ast::Expression> statement = parseExpression(LOWEST);

			if (peekTokenIs(p_separator))
			{
				nextToken();
			}

			p_destination->push_back(statement);
		}
		nextToken();
	}

	void Parser::parseKeyValuePairs(std::map<std::shared_ptr<ast::Expression>, std::shared_ptr<ast::Expression>>* p_destination, token::TokenType p_separator, token::TokenType p_ender)
	{
		while (!peekTokenIs(p_ender) && m_currentToken.m_type != token::END_OF_FILE)
		{
			nextToken();

			std::shared_ptr<ast::Expression> key = parseExpression(LOWEST);

			if (expectPeek(token::COLON))
			{
				nextToken();
			}

			std::shared_ptr<ast::Expression> value = parseExpression(LOWEST);

			if (peekTokenIs(p_separator))
			{
				nextToken();
			}

			p_destination->emplace(key, value);
		}
		nextToken();
	}
}