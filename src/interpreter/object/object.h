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
		{ERROR, "ERROR"},
	};

	class Object
	{
	public:
		virtual ObjectType Type() = 0;
		virtual std::string Inspect() = 0;
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

	class Environment
	{
	public:
		Environment();

		object::Object* getIdentifier(std::string* identifier);
		void setIdentifier(std::string* identifier, Object* value);
	private:
		std::map<std::string, Object*> m_store;
	};
}