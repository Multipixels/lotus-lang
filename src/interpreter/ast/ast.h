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
		virtual std::string String() = 0;
		virtual std::string NodeType() = 0;
	private:
		std::string m_nodeType = "Node";
	};

	class Statement : public Node 
	{
	public:
		virtual std::string TokenLiteral() = 0;
		virtual std::string String() = 0;
		virtual std::string NodeType() = 0;
	private:
		std::string m_nodeType = "Statement";
	};

	class Expression : public Node 
	{
	public:
		virtual std::string TokenLiteral() = 0;
		virtual std::string String() = 0;
		virtual std::string NodeType() = 0;
	private:
		std::string m_nodeType = "Expression";
	};

	class Program
	{
	public:
		std::vector<Statement*> m_statements;

		std::string TokenLiteral();
		std::string String();
	};

	class Identifier : public Expression
	{
	public:
		token::Token m_token;
		std::string m_name;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "Identifier";
	};

	// STATEMENTS

	class DeclareIntegerStatement : public Statement
	{
	public:
		token::Token m_token;
		Identifier m_name;
		Expression *m_value;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "DeclareIntegerStatement";
	};

	class DeclareFloatStatement : public Statement
	{
	public:
		token::Token m_token;
		Identifier m_name;
		Expression* m_value;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "DeclareFloatStatement";
	};

	class DeclareBooleanStatement : public Statement
	{
	public:
		token::Token m_token;
		Identifier m_name;
		Expression* m_value;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "DeclareBooleanStatement";
	};

	class DeclareCharacterStatement : public Statement
	{
	public:
		token::Token m_token;
		Identifier m_name;
		Expression* m_value;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "DeclareCharacterStatement";
	};

	class ReturnStatement : public Statement
	{
	public:
		token::Token m_token;
		Expression* m_returnValue;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "ReturnStatement";
	};

	class ExpressionStatement : public Statement
	{
	public:
		token::Token m_token;
		Expression* m_expression;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "ExpressionStatement";
	};

	class BlockStatement : public Statement
	{
	public:
		token::Token m_token;
		std::vector<Statement*> m_statements;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "BlockStatement";
	};

	class IfStatement : public Statement
	{
	public:
		token::Token m_token;
		std::vector<Expression*> m_condition;
		std::vector<BlockStatement*> m_consequence;
		BlockStatement* m_alternative;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "IfStatement";
	};

	// EXPRESSIONS

	class IntegerLiteral : public Expression
	{
	public:
		token::Token m_token;
		int m_value;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "IntegerLiteral";
	};

	class FloatLiteral : public Expression
	{
	public:
		token::Token m_token;
		float m_value;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "FloatLiteral";
	};

	class BooleanLiteral : public Expression
	{
	public:
		token::Token m_token;
		bool m_value;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "BooleanLiteral";
	};

	class CharacterLiteral : public Expression
	{
	public:
		token::Token m_token;
		char m_value;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "CharacterLiteral";
	};

	class PrefixExpression : public Expression
	{
	public:
		token::Token m_token;
		std::string m_operator;
		ast::Expression *m_right_expression;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "PrefixExpression";
	};

	class InfixExpression : public Expression
	{
	public:
		token::Token m_token;
		ast::Expression* m_left_expression;
		std::string m_operator;
		ast::Expression* m_right_expression;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "InfixExpression";
	};
}
