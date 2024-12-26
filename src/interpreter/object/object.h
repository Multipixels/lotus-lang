#pragma once

#include <map>

namespace object
{
	enum ObjectType
	{
		INTEGER,
		FLOAT,
		BOOLEAN,
		CHARACTER,
		NULL_TYPE,
		RETURN,
		FUNCTION,
		ERROR,
	};

	const std::map<ObjectType, std::string> objectTypeToString =
	{
		{INTEGER, "integer"},
		{FLOAT, "float"},
		{BOOLEAN, "boolean"},
		{CHARACTER, "character"},
		{NULL_TYPE, "null"},
		{RETURN, "RETURN"},
		{FUNCTION, "FUNCTION"},
		{ERROR, "ERROR"},
	};

	const std::map<token::TokenType, ObjectType> nodeTypeToObjectType =
	{
		{token::INTEGER_TYPE, INTEGER},
		{token::FLOAT_TYPE, FLOAT},
		{token::BOOLEAN_TYPE, BOOLEAN},
		{token::CHARACTER_TYPE, CHARACTER},
	};

	class Object
	{
	public:
		virtual ObjectType Type() = 0;
		virtual std::string Inspect() = 0;
	};

	class Environment
	{
	public:
		Environment();
		Environment(Environment* m_outer);

		object::Object* getIdentifier(std::string* identifier);
		void setIdentifier(std::string* identifier, Object* value);
	private:
		std::map<std::string, Object*> m_store;
		Environment* m_outer;
	};

	class Integer : public Object
	{
	public:
		Integer();
		Integer(int value);
		ObjectType Type();
		std::string Inspect();
		int m_value;
	};

	class Float : public Object
	{
	public:
		Float();
		Float(float value);
		ObjectType Type();
		std::string Inspect();
		float m_value;
	};

	class Boolean : public Object
	{
	public:
		Boolean();
		Boolean(bool value);
		ObjectType Type();
		std::string Inspect();
		bool m_value;
	};

	class Character : public Object
	{
	public:
		Character();
		Character(char value);
		ObjectType Type();
		std::string Inspect();

		char m_value;
	};

	class Null : public Object
	{
	public:
		Null();
		ObjectType Type();
		std::string Inspect();
	};

	class Return : public Object
	{
	public:
		Return(Object* return_value);
		ObjectType Type();
		std::string Inspect();
		
		Object* m_return_value;
	};

	class Function : public Object
	{
	public:
		Function(ObjectType functionType, ast::DeclareFunctionStatement* functionDeclaration, Environment* environment);
		ObjectType Type();
		std::string Inspect();

		ObjectType m_function_type;
		ast::Identifier* m_function_name;
		std::vector<ast::DeclareVariableStatement*> m_parameters;
		ast::BlockStatement* m_body;
		Environment* m_environment;
	};

	class Error : public Object
	{
	public:
		Error(std::string error_message);
		ObjectType Type();
		std::string Inspect();

		std::string m_error_message;
	};

	extern Null NULL_OBJECT;
	extern Boolean TRUE_OBJECT;
	extern Boolean FALSE_OBJECT;
}