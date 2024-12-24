#pragma once

namespace object
{
	enum ObjectType
	{
		INTEGER,
		FLOAT,
		BOOLEAN,
		CHARACTER,
		NULL_TYPE,
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

	extern Null NULL_OBJECT;
	extern Boolean TRUE_OBJECT;
	extern Boolean FALSE_OBJECT;
}