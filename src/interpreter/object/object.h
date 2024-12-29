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
		COLLECTION,
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
		{COLLECTION, "collection"},
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
		{token::COLLECTION_TYPE, COLLECTION},
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

		// Gets value of identifier, looking through outer layers as welll.
		object::Object* getIdentifier(std::string* identifier);

		// Gets value of identifier only in the current level of environment.
		object::Object* getLocalIdentifier(std::string* identifier);

		// Assigns value to identifier in this level of environment.
		void setIdentifier(std::string* identifier, Object* value);

		// Checks outer level for identifier for assignment.
		void reassignIdentifier(std::string* identifier, Object* value);
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

	class Collection : public Object
	{
	public:
		Collection();
		Collection(ObjectType collection_type, std::vector<Object*> value);
		ObjectType Type();
		std::string Inspect();

		ObjectType m_collection_type;
		std::vector<Object*> m_values;
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