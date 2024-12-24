#pragma once

namespace object
{
	enum ObjectType
	{
		INTEGER,
		BOOLEAN,
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

	class Boolean : public Object
	{
	public:
		Boolean();
		Boolean(bool value);
		ObjectType Type();
		std::string Inspect();
		bool m_value;
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