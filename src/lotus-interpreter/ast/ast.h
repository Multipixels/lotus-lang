#pragma once

#include <string>
#include <vector>

#include "token.h"

namespace ast
{
	enum NodeType
	{
		NODE,
		STATEMENT_NODE,
		EXPRESSION_NODE,
		PROGRAM_NODE,

		IDENTIFIER_NODE,
		BLOCK_STATEMENT_NODE,
		INTEGER_LITERAL_NODE,
		FLOAT_LITERAL_NODE,
		BOOLEAN_LITERAL_NODE,
		CHARACTER_LITERAL_NODE,
		COLLECTION_LITERAL_NODE,
		DICTIONARY_LITERAL_NODE,
		STRING_LITERAL_NODE,
		FUNCTION_LITERAL_NODE,
		PREFIX_EXPRESSION_NODE,
		INFIX_EXPRESSION_NODE,
		CALL_EXPRESSION_NODE,
		INDEX_EXPRESSION_NODE,

		DECLARE_VARIABLE_STATEMENT_NODE,
		DECLARE_COLLECTION_STATEMENT_NODE,
		DECLARE_DICTIONARY_STATEMENT_NODE,
		DECLARE_FUNCTION_STATEMENT_NODE,
		RETURN_STATEMENT_NODE,
		EXPRESSION_STATEMENT_NODE,
		IF_STATEMENT_NODE,
		WHILE_STATEMENT_NODE,
		DO_WHILE_STATEMENT_NODE,
		FOR_STATEMENT_NODE,
		ITERATE_STATEMENT_NODE,
	};

	class Node
	{
	public:
		virtual std::string TokenLiteral() = 0;
		virtual std::string String() = 0;
		virtual NodeType Type() = 0;
	private:
		NodeType m_nodeType = NODE;
	};

	class Statement : public Node 
	{
	public:
		virtual std::string TokenLiteral() = 0;
		virtual std::string String() = 0;
		virtual NodeType Type() = 0;
	private:
		NodeType m_nodeType = STATEMENT_NODE;
	};

	class Expression : public Node 
	{
	public:
		virtual std::string TokenLiteral() = 0;
		virtual std::string String() = 0;
		virtual NodeType Type() = 0;
	private:
		NodeType m_nodeType = EXPRESSION_NODE;
	};

	class Program : public Node
	{
	public:
		std::vector<Statement*> m_statements;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = PROGRAM_NODE;
	};

	// REQUISITES

	class Identifier : public Expression
	{
	public:
		token::Token m_token;
		std::string m_name;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = IDENTIFIER_NODE;
	};

	class BlockStatement : public Statement
	{
	public:
		token::Token m_token;
		std::vector<Statement*> m_statements;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = BLOCK_STATEMENT_NODE;
	};

	// EXPRESSIONS

	class IntegerLiteral : public Expression
	{
	public:
		token::Token m_token;
		int m_value;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = INTEGER_LITERAL_NODE;
	};

	class FloatLiteral : public Expression
	{
	public:
		token::Token m_token;
		float m_value;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = FLOAT_LITERAL_NODE;
	};

	class BooleanLiteral : public Expression
	{
	public:
		token::Token m_token;
		bool m_value;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = BOOLEAN_LITERAL_NODE;
	};

	class CharacterLiteral : public Expression
	{
	public:
		token::Token m_token;
		char m_value;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = CHARACTER_LITERAL_NODE;
	};

	class CollectionLiteral : public Expression
	{
	public:
		token::Token m_token; // '['
		std::vector<Expression*> m_values;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = COLLECTION_LITERAL_NODE;
	};

	class DictionaryLiteral : public Expression
	{
	public:
		token::Token m_token; // '{'
		std::map<Expression*, Expression*> m_map;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = DICTIONARY_LITERAL_NODE;
	};

	class StringLiteral : public Expression
	{
	public:
		token::Token m_token; // '['
		CollectionLiteral* m_stringCollection;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = STRING_LITERAL_NODE;
	};

	class FunctionLiteral : public Expression
	{
	public:
		token::Token m_token;
		ast::BlockStatement* m_body;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = FUNCTION_LITERAL_NODE;
	};

	class PrefixExpression : public Expression
	{
	public:
		token::Token m_token;
		std::string m_operator;
		ast::Expression* m_right_expression;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = PREFIX_EXPRESSION_NODE;
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
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = INFIX_EXPRESSION_NODE;
	};

	class CallExpression : public Expression
	{
	public:
		token::Token m_token;
		ast::Expression* m_function; // Either an identifier or function literal
		std::vector<ast::Expression*> m_parameters;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = CALL_EXPRESSION_NODE;
	};

	class IndexExpression : public Expression
	{
	public:
		token::Token m_token;
		ast::Expression* m_collection; // Either an identifier or collection literal
		ast::Expression* m_index;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = INDEX_EXPRESSION_NODE;
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
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = DECLARE_VARIABLE_STATEMENT_NODE;
	};

	class DeclareCollectionStatement : public Statement
	{
	public:
		token::Token m_token;
		token::Token m_typeToken;
		Identifier m_name;
		Expression* m_value;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = DECLARE_COLLECTION_STATEMENT_NODE;
	};

	class DeclareDictionaryStatement : public Statement
	{
	public:
		token::Token m_token;
		token::Token m_keyTypeToken;
		token::Token m_valueTypeToken;
		Identifier m_name;
		Expression* m_value;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = DECLARE_DICTIONARY_STATEMENT_NODE;
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
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = DECLARE_FUNCTION_STATEMENT_NODE;
	};

	class ReturnStatement : public Statement
	{
	public:
		token::Token m_token;
		Expression* m_returnValue;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = RETURN_STATEMENT_NODE;
	};

	class ExpressionStatement : public Statement
	{
	public:
		token::Token m_token;
		Expression* m_expression;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = EXPRESSION_STATEMENT_NODE;
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
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = IF_STATEMENT_NODE;
	};

	class WhileStatement : public Statement
	{
	public:
		token::Token m_token;
		Expression* m_condition;
		BlockStatement* m_consequence;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = WHILE_STATEMENT_NODE;
	};

	class DoWhileStatement : public Statement
	{
	public:
		token::Token m_token;
		BlockStatement* m_consequence;
		Expression* m_condition;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = DO_WHILE_STATEMENT_NODE;
	};

	class ForStatement : public Statement
	{
	public:
		token::Token m_token;
		Statement* m_initialization;
		Statement* m_condition;
		Statement* m_updation;
		BlockStatement* m_consequence;

		std::string TokenLiteral();
		std::string String();
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = FOR_STATEMENT_NODE;
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
		NodeType Type() { return m_nodeType; }
	private:
		NodeType m_nodeType = ITERATE_STATEMENT_NODE;
	};
}
