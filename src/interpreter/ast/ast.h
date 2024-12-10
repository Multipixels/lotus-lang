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
		virtual std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "Node";
	};

	class Statement : public Node 
	{
	public:
		virtual std::string TokenLiteral() = 0;
		virtual std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "Statement";
	};

	class Expression : public Node 
	{
	public:
		virtual std::string TokenLiteral() = 0;
		virtual std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "Expression";
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

		std::string NodeType() override { return m_nodeType; }
		std::string TokenLiteral();
	private:
		std::string m_nodeType = "Identifier";
	};

	class DeclareIntegerStatement : public Statement
	{
	public:
		token::Token m_token;
		Identifier m_name;
		Expression *m_value;

		std::string NodeType() override { return m_nodeType; }
		std::string TokenLiteral();
	private:
		std::string m_nodeType = "DeclareIntegerStatement";
	};

	class DeclareFloatStatement : public Statement
	{
	public:
		token::Token m_token;
		Identifier m_name;
		Expression* m_value;

		std::string NodeType() override { return m_nodeType; }
		std::string TokenLiteral();
	private:
		std::string m_nodeType = "DeclareFloatStatement";
	};

	class DeclareBooleanStatement : public Statement
	{
	public:
		token::Token m_token;
		Identifier m_name;
		Expression* m_value;

		std::string NodeType() override { return m_nodeType; }
		std::string TokenLiteral();
	private:
		std::string m_nodeType = "DeclareBooleanStatement";
	};

	class DeclareCharacterStatement : public Statement
	{
	public:
		token::Token m_token;
		Identifier m_name;
		Expression* m_value;

		std::string NodeType() override { return m_nodeType; }
		std::string TokenLiteral();
	private:
		std::string m_nodeType = "DeclareCharacterStatement";
	};

	class IntegerLiteral : public Expression
	{
	public:
		token::Token m_token;
		int m_value;

		std::string NodeType() override { return m_nodeType; }
		std::string TokenLiteral();
	private:
		std::string m_nodeType = "IntegerLiteral";
	};

	class FloatLiteral : public Expression
	{
	public:
		token::Token m_token;
		float m_value;

		std::string NodeType() override { return m_nodeType; }
		std::string TokenLiteral();
	private:
		std::string m_nodeType = "FloatLiteral";
	};

	class BooleanLiteral : public Expression
	{
	public:
		token::Token m_token;
		bool m_value;

		std::string NodeType() override { return m_nodeType; }
		std::string TokenLiteral();
	private:
		std::string m_nodeType = "BooleanLiteral";
	};

	class CharacterLiteral : public Expression
	{
	public:
		token::Token m_token;
		char m_value;

		std::string NodeType() override { return m_nodeType; }
		std::string TokenLiteral();
	private:
		std::string m_nodeType = "CharacterLiteral";
	};
}