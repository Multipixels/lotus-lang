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
		registerPrefixFunction(token::STRING_LITERAL, &Parser::parseStringLiteral);
		registerPrefixFunction(token::LBRACKET, &Parser::parseCollectionLiteral);
		registerPrefixFunction(token::LBRACE, &Parser::parseDictionaryLiteral);
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
		registerInfixFunction(token::LPARENTHESIS, &Parser::parseCallExpression);
		registerInfixFunction(token::LBRACKET, &Parser::parseIndexExpression);
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
			if (peekTokenIs(token::LPARENTHESIS))
			{
				return parseFunctionDeclaration();
			}
			return parseVariableDeclaration();
		case token::FLOAT_TYPE:
			if (peekTokenIs(token::LPARENTHESIS))
			{
				return parseFunctionDeclaration();
			}
			return parseVariableDeclaration();
		case token::BOOLEAN_TYPE:
			if (peekTokenIs(token::LPARENTHESIS))
			{
				return parseFunctionDeclaration();
			}
			return parseVariableDeclaration();
		case token::CHARACTER_TYPE:
			if (peekTokenIs(token::LPARENTHESIS))
			{
				return parseFunctionDeclaration();
			}
			return parseVariableDeclaration();
		case token::STRING_TYPE:
			if (peekTokenIs(token::LPARENTHESIS))
			{
				return parseFunctionDeclaration();
			}
			return parseVariableDeclaration();
		case token::COLLECTION_TYPE:
			return parseCollectionDeclaration();
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

	ast::DeclareVariableStatement* Parser::parseVariableDeclaration()
	{
		ast::DeclareVariableStatement* statement = new ast::DeclareVariableStatement;
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
			nextToken();
			return statement;
		}

		// Declaration with assignment
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
	};

	ast::DeclareCollectionStatement* Parser::parseCollectionDeclaration()
	{
		ast::DeclareCollectionStatement* statement = new ast::DeclareCollectionStatement;
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
			nextToken();
			return statement;
		}

		// Declaration with assignment
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
	};

	ast::DeclareFunctionStatement* Parser::parseFunctionDeclaration()
	{
		ast::DeclareFunctionStatement* statement = new ast::DeclareFunctionStatement;
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

		statement->m_name = *((ast::Identifier*)parseIdentifier());

		if (!expectPeek(token::LBRACE))
		{
			return NULL;
		}

		statement->m_body = new ast::FunctionLiteral;
		statement->m_body->m_token = m_currentToken;
		statement->m_body->m_body = parseBlockStatement();

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

		// Handling else ifs and elses
		if (peekTokenIs(token::ELSE))
		{
			nextToken();

			statement->m_alternative = parseElseStatement();
			return statement;
		}

		statement->m_alternative = 0;
		return statement;
	}

	ast::IfStatement* Parser::parseElseStatement()
	{
		if (peekTokenIs(token::IF))
		{
			nextToken();
			return parseIfStatement();
		}

		ast::IfStatement* statement = new ast::IfStatement;
		statement->m_token = m_currentToken;

		if (!expectPeek(token::LBRACE))
		{
			return NULL;
		}

		statement->m_consequence = parseBlockStatement();

		return statement;
	}

	ast::WhileStatement* Parser::parseWhileStatement()
	{
		ast::WhileStatement* statement = new ast::WhileStatement;
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
		return statement;
	}

	ast::DoWhileStatement* Parser::parseDoWhileStatement()
	{
		ast::DoWhileStatement* statement = new ast::DoWhileStatement;
		statement->m_token = m_currentToken;

		if (!expectPeek(token::LBRACE))
		{
			return NULL;
		}

		statement->m_consequence = parseBlockStatement();

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

	ast::ForStatement* Parser::parseForStatement()
	{
		ast::ForStatement* statement = new ast::ForStatement;
		statement->m_token = m_currentToken;

		if (!expectPeek(token::LPARENTHESIS))
		{
			return NULL;
		}
		nextToken();

		statement->m_initialization = parseStatement();
		nextToken();

		statement->m_condition = parseStatement();
		nextToken();

		statement->m_updation = parseStatement();

		if (!expectPeek(token::RPARENTHESIS))
		{
			return NULL;
		}

		if (!expectPeek(token::LBRACE))
		{
			return NULL;
		}

		statement->m_consequence = parseBlockStatement();

		return statement;
	}

	ast::IterateStatement* Parser::parseIterateStatement()
	{
		ast::IterateStatement* statement = new ast::IterateStatement;
		statement->m_token = m_currentToken;

		if (!expectPeek(token::LPARENTHESIS))
		{
			return NULL;
		}
		nextToken();

		statement->m_var = (ast::Identifier*)parseIdentifier();

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

		expression->m_right_expression = parseExpression(PREFIX);

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
		if (m_currentToken.m_literal.size() != 1) {
			std::ostringstream error;

			error << "Expected to see a single character. Got " 
				<< m_currentToken.m_literal.size()
				<< " instead.";

			m_errors.push_back(error.str());
		}

		ast::CharacterLiteral* expression = new ast::CharacterLiteral;
		expression->m_token = m_currentToken;
		expression->m_value = m_currentToken.m_literal[0];
		return expression;
	}

	ast::Expression* Parser::parseCollectionLiteral()
	{
		ast::CollectionLiteral* expression = new ast::CollectionLiteral;
		expression->m_token = m_currentToken;
		parseLiterals(&expression->m_values, token::COMMA, token::RBRACKET);
		return expression;
	}

	ast::Expression* Parser::parseDictionaryLiteral()
	{
		ast::DictionaryLiteral* expression = new ast::DictionaryLiteral;
		expression->m_token = m_currentToken;
		parseKeyValuePairs(&expression->m_map, token::COMMA, token::RBRACE);
		return expression;
	}

	ast::Expression* Parser::parseStringLiteral()
	{
		ast::StringLiteral* stringLiteral = new ast::StringLiteral;
		stringLiteral->m_token = m_currentToken;
		stringLiteral->m_stringCollection = new ast::CollectionLiteral;

		for (int i = 0; i < m_currentToken.m_literal.size(); i++)
		{
			ast::CharacterLiteral* expression = new ast::CharacterLiteral;

			expression->m_token.m_type = token::CHARACTER_LITERAL;
			expression->m_token.m_literal = m_currentToken.m_literal[i];
			expression->m_value = m_currentToken.m_literal[i];

			stringLiteral->m_stringCollection->m_values.push_back(expression);
		}

		return stringLiteral;
	}

	ast::Expression* Parser::parseIdentifier()
	{
		ast::Identifier* expression = new ast::Identifier;
		expression->m_token = m_currentToken;
		expression->m_name = m_currentToken.m_literal;
		return expression;
	}

	ast::Expression* Parser::parseCallExpression(ast::Expression* leftExpression)
	{
		ast::CallExpression* expression = new ast::CallExpression;
		expression->m_token = m_currentToken;
		expression->m_function = leftExpression;
		parseLiterals(&expression->m_parameters, token::COMMA, token::RPARENTHESIS);

		return expression;
	}

	ast::Expression* Parser::parseIndexExpression(ast::Expression* leftExpression)
	{
		ast::IndexExpression* expression = new ast::IndexExpression;
		expression->m_token = m_currentToken;
		expression->m_collection = leftExpression;

		nextToken();
		expression->m_index = parseExpression(LOWEST);

		if (!expectPeek(token::RBRACKET))
		{
			return NULL;
		}

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

	void Parser::parseParameters(std::vector<ast::DeclareVariableStatement*>* parameters)
	{
		while (!peekTokenIs(token::RPARENTHESIS))
		{
			nextToken();

			ast::DeclareVariableStatement* statement = new ast::DeclareVariableStatement;
			statement->m_token = m_currentToken;

			if (!expectPeek(token::IDENTIFIER))
			{
				parameters->push_back(NULL);
				continue;
			}

			statement->m_name.m_token = m_currentToken;
			statement->m_name.m_name = m_currentToken.m_literal;

			if (peekTokenIs(token::COMMA))
			{
				nextToken();
			}

			parameters->push_back(statement);
		}
	}

	void Parser::parseLiterals(std::vector<ast::Expression*>* destination, token::TokenType separator, token::TokenType ender)
	{
		while (!peekTokenIs(ender) && m_currentToken.m_type != token::END_OF_FILE)
		{
			nextToken();

			ast::Expression* statement = parseExpression(LOWEST);

			if (peekTokenIs(separator))
			{
				nextToken();
			}

			destination->push_back(statement);
		}
		nextToken();
	}

	void Parser::parseKeyValuePairs(std::map<ast::Expression*, ast::Expression*>* destination, token::TokenType separator, token::TokenType ender)
	{
		while (!peekTokenIs(ender) && m_currentToken.m_type != token::END_OF_FILE)
		{
			nextToken();

			ast::Expression* key = parseExpression(LOWEST);

			if (expectPeek(token::COLON))
			{
				nextToken();
			}

			ast::Expression* value = parseExpression(LOWEST);

			if (peekTokenIs(separator))
			{
				nextToken();
			}

			destination->emplace(key, value);
		}
		nextToken();
	}
}