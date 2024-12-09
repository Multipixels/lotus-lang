#pragma once

#include <string>
#include <vector>

#include "token.h"

namespace ast
{
	class Node
	{
	public:
		virtual std::string TokenLiteral() = 0;
	};

	class Statement : public Node 
	{
	public:
		virtual std::string TokenLiteral() = 0;
	};

	class Expression : public Node 
	{
	public:
		virtual std::string TokenLiteral() = 0;
	};

	class Program
	{
	public:
		std::vector<Statement*> m_statements;

		std::string TokenLiteral();
	};

	class Identifier : public Expression
	{
	public:
		token::Token m_token;
		std::string m_name;

		std::string TokenLiteral();
	};

	class DeclareIntegerStatement : public Statement
	{
	public:
		token::Token m_token;
		Identifier m_name;
		Expression *m_value;

		std::string TokenLiteral();
	};

	class IntegerLiteral : public Expression
	{
	public:
		token::Token m_token;
		int m_value;

		std::string TokenLiteral();
	};
}