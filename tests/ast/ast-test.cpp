#include <gtest/gtest.h>
#include <memory>

#include "ast.h"
#include "ast-test.h"

TEST(ASTTest, TestProgramString)
{
	std::shared_ptr<ast::Program> program(new ast::Program);
	std::shared_ptr<ast::DeclareVariableStatement> declareInt(new ast::DeclareVariableStatement);
	std::shared_ptr<ast::Identifier> identifier(new ast::Identifier);

	identifier->m_token.m_type = token::IDENTIFIER;
	identifier->m_token.m_literal = "someInteger";
	identifier->m_name = "someInteger";

	std::shared_ptr<ast::IntegerLiteral> integer(new ast::IntegerLiteral);
	integer->m_token.m_type = token::INTEGER_LITERAL;
	integer->m_token.m_literal = 10;
	integer->m_value = 10;

	declareInt->m_token.m_type = token::INTEGER_TYPE;
	declareInt->m_token.m_literal = "int";
	declareInt->m_name = *identifier;
	declareInt->m_value = integer;

	program->m_statements.push_back(declareInt);
		
	EXPECT_EQ(program->String(), "int someInteger = 10;");
}