#pragma once

#include "ast.h"

namespace object
{
	enum ObjectType
	{
		INTEGER,
		FLOAT,
		BOOLEAN,
		CHARACTER,
		COLLECTION,
		DICTIONARY,
		STRING,
		NULL_TYPE,
		RETURN,
		FUNCTION,
		ERROR,
		BUILTIN_FUNCTION,
	};

	const std::map<ObjectType, std::string> objectTypeToString =
	{
		{INTEGER, "integer"},
		{FLOAT, "float"},
		{BOOLEAN, "boolean"},
		{CHARACTER, "character"},
		{COLLECTION, "collection"},
		{DICTIONARY, "dictionary"},
		{STRING, "string"},
		{NULL_TYPE, "null"},
		{RETURN, "RETURN"},
		{FUNCTION, "FUNCTION"},
		{ERROR, "ERROR"},
		{BUILTIN_FUNCTION, "BUILTIN_FUNCTION"},
	};

	const std::map<token::TokenType, ObjectType> nodeTypeToObjectType =
	{
		{token::INTEGER_TYPE, INTEGER},
		{token::FLOAT_TYPE, FLOAT},
		{token::BOOLEAN_TYPE, BOOLEAN},
		{token::CHARACTER_TYPE, CHARACTER},
		{token::COLLECTION_TYPE, COLLECTION},
		{token::DICTIONARY_TYPE, DICTIONARY},
		{token::STRING_TYPE, STRING},
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
		Environment(std::shared_ptr<Environment> m_outer);

		// Gets value of identifier, looking through outer layers as welll.
		std::shared_ptr<Object> getIdentifier(std::string* identifier);

		// Gets value of identifier only in the current level of environment.
		std::shared_ptr<Object> getLocalIdentifier(std::string* identifier);

		// Assigns value to identifier in this level of environment.
		void setIdentifier(std::string* identifier, std::shared_ptr<Object> value);

		// Checks outer level for identifier for assignment.
		void reassignIdentifier(std::string* identifier, std::shared_ptr<Object> value);
	private:
		std::map<std::string, std::shared_ptr<Object>> m_store;
		std::shared_ptr<Environment> m_outer;
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
		Collection(ObjectType collection_type, std::vector<std::shared_ptr<Object>> value);
		ObjectType Type();
		std::string Inspect();

		ObjectType m_collection_type;
		std::vector<std::shared_ptr<Object>> m_values;
	};

	class Dictionary : public Object
	{
	private:
		struct ObjCmp {
			bool operator()(std::shared_ptr<Object> lhs, std::shared_ptr<Object> rhs) const;
		};
	public:
		Dictionary();
		Dictionary(ObjectType keyType, ObjectType valueType, std::vector<std::shared_ptr<Object>> keys, std::vector<std::shared_ptr<Object>> values);
		ObjectType Type();
		std::string Inspect();

		ObjectType m_key_type;
		ObjectType m_value_type;
		std::map<std::shared_ptr<Object>, std::shared_ptr<Object>, ObjCmp> m_map;
	};

	class String : public Object
	{
	public:
		String();
		String(std::string* value);
		ObjectType Type();
		std::string Inspect();

		std::string m_value;
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
		Return(std::shared_ptr<Object> return_value);
		ObjectType Type();
		std::string Inspect();
		
		std::shared_ptr<Object> m_return_value;
	};

	class Function : public Object
	{
	public:
		Function(ObjectType functionType, std::shared_ptr<ast::DeclareFunctionStatement> functionDeclaration, std::shared_ptr<Environment> environment);
		ObjectType Type();
		std::string Inspect();

		ObjectType m_function_type;
		std::shared_ptr<ast::Identifier> m_function_name;
		std::vector<std::shared_ptr<ast::DeclareVariableStatement>> m_parameters;
		std::shared_ptr<ast::BlockStatement> m_body;
		std::shared_ptr<Environment> m_environment;
	};

	class Error : public Object
	{
	public:
		Error(std::string error_message);
		ObjectType Type();
		std::string Inspect();

		std::string m_error_message;
	};

	class Builtin : public Object
	{
	public:
		typedef std::shared_ptr<Object> (*BuiltinFunctionPointer) (std::vector<std::shared_ptr<Object>>*);

		Builtin(BuiltinFunctionPointer fn);
		ObjectType Type();
		std::string Inspect();

		BuiltinFunctionPointer m_function;
	};

	extern std::shared_ptr<Null> NULL_OBJECT;
	extern std::shared_ptr<Boolean> TRUE_OBJECT;
	extern std::shared_ptr<Boolean> FALSE_OBJECT;
}