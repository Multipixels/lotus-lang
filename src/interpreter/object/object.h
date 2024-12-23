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
		ObjectType Type();
		std::string Inspect();
		int m_value;
	};

	class Boolean : public Object
	{
	public:
		ObjectType Type();
		std::string Inspect();
		bool m_value;
	};

	class Null : public Object
	{
	public:
		ObjectType Type();
		std::string Inspect();
	};
}