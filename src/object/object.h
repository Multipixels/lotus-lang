#pragma once

#include <functional>

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
		BREAK,
	};

	const std::map<ObjectType, std::string> c_objectTypeToString =
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
		{BREAK, "break"},
	};

	const std::map<token::TokenType, ObjectType> c_nodeTypeToObjectType =
	{
		{token::INTEGER_TYPE, INTEGER},
		{token::FLOAT_TYPE, FLOAT},
		{token::BOOLEAN_TYPE, BOOLEAN},
		{token::CHARACTER_TYPE, CHARACTER},
		{token::COLLECTION_TYPE, COLLECTION},
		{token::DICTIONARY_TYPE, DICTIONARY},
		{token::STRING_TYPE, STRING},
		{token::RETURN, RETURN},
		{token::BREAK, BREAK},
	};

	class Object : public std::enable_shared_from_this<Object>
	{
	protected:
		std::map<std::string, std::function<std::shared_ptr<Object>()>> m_members;
	public:
		virtual ObjectType Type() = 0;
		virtual std::string Inspect() = 0;

		std::shared_ptr<Object> Member(std::string p_memberName);
	};

	class Environment
	{
	public:
		Environment();
		Environment(std::shared_ptr<Environment> p_outer);

		// Gets value of identifier, looking through outer layers as welll.
		std::shared_ptr<Object> getIdentifier(std::string* p_identifier);

		// Gets value of identifier only in the current level of environment.
		std::shared_ptr<Object> getLocalIdentifier(std::string* p_identifier);

		// Assigns value to identifier in this level of environment.
		void setIdentifier(std::string* p_identifier, std::shared_ptr<Object> p_value);

		// Checks outer level for identifier for assignment.
		void reassignIdentifier(std::string* p_identifier, std::shared_ptr<Object> p_value);
	private:
		std::map<std::string, std::shared_ptr<Object>> m_store;
		std::shared_ptr<Environment> m_outer;
	};

	class Integer : public Object
	{
	public:
		Integer();
		Integer(int p_value);
		ObjectType Type();
		std::string Inspect();

		int m_value;
	};

	class Float : public Object
	{
	public:
		Float();
		Float(float p_value);
		ObjectType Type();
		std::string Inspect();

		float m_value;
	};

	class Boolean : public Object
	{
	public:
		Boolean();
		Boolean(bool p_value);
		ObjectType Type();
		std::string Inspect();
		
		bool m_value;
	};

	class Character : public Object
	{
	public:
		Character();
		Character(char p_value);
		ObjectType Type();
		std::string Inspect();

		char m_value;
	};

	class Collection : public Object
	{
	public:
		Collection();
		Collection(ObjectType p_collection_type, std::vector<std::shared_ptr<Object>> p_value);
		ObjectType Type();
		std::string Inspect();

		ObjectType m_collectionType;
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
		Dictionary(ObjectType p_keyType, ObjectType p_valueType, std::vector<std::shared_ptr<Object>> p_keys, std::vector<std::shared_ptr<Object>> p_values);
		ObjectType Type();
		std::string Inspect();

		ObjectType m_keyType;
		ObjectType m_valueType;
		std::map<std::shared_ptr<Object>, std::shared_ptr<Object>, ObjCmp> m_map;
	};

	class String : public Object
	{
	public:
		String();
		String(std::string* p_value);
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
		Return(std::shared_ptr<Object> p_returnValue);
		ObjectType Type();
		std::string Inspect();
		
		std::shared_ptr<Object> m_returnValue;
	};

	class Function : public Object
	{
	public:
		Function(ObjectType p_functionType, std::shared_ptr<ast::DeclareFunctionStatement> p_functionDeclaration, std::shared_ptr<Environment> p_environment);
		ObjectType Type();
		std::string Inspect();

		ObjectType m_functionType;
		ast::Identifier m_functionName;
		std::vector<std::shared_ptr<ast::DeclareVariableStatement>> m_parameters;
		std::shared_ptr<ast::BlockStatement> m_body;
		std::shared_ptr<Environment> m_environment;
	};

	class Error : public Object
	{
	public:
		Error(std::string p_errorMessage);
		ObjectType Type();
		std::string Inspect();

		std::string m_errorMessage;
	};

	class Builtin : public Object
	{
	public:
		typedef std::shared_ptr<Object> (*BuiltinFunctionPointer) (std::vector<std::shared_ptr<Object>>*, std::shared_ptr<object::Object>);

		Builtin(BuiltinFunctionPointer p_fn, std::shared_ptr<Object> p_object = 0);
		ObjectType Type();
		std::string Inspect();

		BuiltinFunctionPointer m_function;
		std::shared_ptr<Object> m_object; // Refers to "parent" object. myCollection.append() for example.
	};

	class Break : public Object
	{
	public:
		Break();
		ObjectType Type();
		std::string Inspect();
	};

	extern std::shared_ptr<Null> NULL_OBJECT;
	extern std::shared_ptr<Boolean> TRUE_OBJECT;
	extern std::shared_ptr<Boolean> FALSE_OBJECT;
	extern std::shared_ptr<Break> BREAK_OBJECT;

	std::shared_ptr<Boolean> getBoolean(bool condition);
}