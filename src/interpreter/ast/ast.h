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

	// REQUISITES

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

	class FunctionLiteral : public Expression
	{
	public:
		token::Token m_token;
		ast::BlockStatement* m_body;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "FunctionLiteral";
	};

	class PrefixExpression : public Expression
	{
	public:
		token::Token m_token;
		std::string m_operator;
		ast::Expression* m_right_expression;

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

	class CallExpression : public Expression
	{
	public:
		token::Token m_token;
		ast::Expression* m_function; // Either an identifier or function literal
		std::vector<ast::Expression*> m_parameters;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "CallExpression";
	};

	// STATEMENTS

	class DeclareVariableStatement : public Statement
	{
	public:
		token::Token m_token;
		Identifier m_name;
		Expression* m_value;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "DeclareVariableStatement";
	};

	class DeclareFunctionStatement : public Statement
	{
	public:
		token::Token m_token; // function type (integer, boolean, etc.)
		std::vector<ast::DeclareVariableStatement*> m_parameters;
		ast::Identifier m_name;
		ast::FunctionLiteral* m_body;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "DeclareFunctionStatement";
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

	// Check m_token to treat as a regular if statement or else statement. 
	class IfStatement : public Statement
	{
	public:
		token::Token m_token;
		Expression* m_condition;
		BlockStatement* m_consequence;
		IfStatement* m_alternative;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "IfStatement";
	};

	class WhileStatement : public Statement
	{
	public:
		token::Token m_token;
		Expression* m_condition;
		BlockStatement* m_consequence;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "WhileStatement";
	};

	class DoWhileStatement : public Statement
	{
	public:
		token::Token m_token;
		BlockStatement* m_consequence;
		Expression* m_condition;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "DoWhileStatement";
	};

	class IterateStatement : public Statement
	{
	public:
		token::Token m_token;
		Identifier* m_var;
		Expression* m_collection;
		BlockStatement* m_consequence;

		std::string TokenLiteral();
		std::string String();
		std::string NodeType() { return m_nodeType; }
	private:
		std::string m_nodeType = "IterateStatement";
	};
}
