#include <sstream>

#include "ast.h"


namespace ast
{
	std::string Program::TokenLiteral()
	{
		return "";
	}
	std::string Program::String()
	{
		std::ostringstream output;

		for (int i = 0; i < m_statements.size(); i++)
		{
			output << m_statements[i]->String();
		}

		return output.str();
	}

	std::string Identifier::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string Identifier::String()
	{
		return m_name;
	}

	std::string DeclareIntegerStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string DeclareIntegerStatement::String()
	{
		std::ostringstream output;

		output << TokenLiteral() << " "
			<< m_name.String() << " = "
			<< m_value->String() << ";";

		return output.str();
	}

	std::string DeclareFloatStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string DeclareFloatStatement::String()
	{
		std::ostringstream output;

		output << TokenLiteral() << " "
			<< m_name.String() << " = "
			<< m_value->String() << ";";

		return output.str();
	}

	std::string DeclareBooleanStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string DeclareBooleanStatement::String()
	{
		std::ostringstream output;

		output << TokenLiteral() << " "
			<< m_name.String() << " = "
			<< m_value->String() << ";";

		return output.str();
	}

	std::string DeclareCharacterStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string DeclareCharacterStatement::String()
	{
		std::ostringstream output;

		output << TokenLiteral() << " "
			<< m_name.String() << " = "
			<< m_value->String() << ";";

		return output.str();
	}

	std::string ReturnStatement::TokenLiteral()
	{
		return "return";
	}
	std::string ReturnStatement::String()
	{
		std::ostringstream output;

		output << TokenLiteral() << " "
			<< m_returnValue->String() << ";";

		return output.str();
	}

	std::string ExpressionStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string ExpressionStatement::String()
	{
		std::ostringstream output;

		output << m_expression->String() << ";";

		return output.str();
	}

	std::string BlockStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string BlockStatement::String()
	{
		std::ostringstream output;

		for (int i = 0; i < m_statements.size(); i++)
		{
			output << m_statements[i]->String() << std::endl;
		}

		return output.str();
	}

	std::string IfStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string IfStatement::String()
	{
		std::ostringstream output;

		if (m_token.m_type == token::IF)
		{
			output << "if (" << m_condition->String() << ")" << std::endl;
		}

		output << "{" << std::endl
			<< m_consequence->String()
			<< "}" << std::endl;

		if (m_alternative != NULL)
		{
			output << "else";
			if (m_alternative->m_token.m_type == token::IF)
			{
				output << " " << m_alternative->String();
			} 
			else
			{
				output << std::endl << m_alternative->String();
			}
		}

		return output.str();
	}

	std::string WhileStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string WhileStatement::String()
	{
		std::ostringstream output;

		output << "while (" << m_condition->String() << ")" << std::endl
			<< "{" << std::endl
			<< m_consequence->String()
			<< "}" << std::endl;

		return output.str();
	}

	std::string DoWhileStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string DoWhileStatement::String()
	{
		std::ostringstream output;

		output << "do" << std::endl
			<< "{" << std::endl
			<< m_consequence->String()
			<< "}" << std::endl
			<< "while (" << m_condition->String() << ");" << std::endl;

		return output.str();
	}

	std::string IterateStatement::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string IterateStatement::String()
	{
		std::ostringstream output;

		output << "iterate (" << m_var->String() << " : " << m_collection->String() << ")" << std::endl
			<< "{" << std::endl
			<< m_consequence->String()
			<< "}" << std::endl;

		return output.str();
	}

	std::string IntegerLiteral::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string IntegerLiteral::String()
	{
		return std::to_string(m_value);
	}

	std::string FloatLiteral::TokenLiteral()
	{
		std::ostringstream outputString;
		outputString << stof(m_token.m_literal);

		return outputString.str();
	}
	std::string FloatLiteral::String()
	{
		std::ostringstream outputString;
		outputString << m_value;

		return outputString.str();
	}

	std::string BooleanLiteral::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string BooleanLiteral::String()
	{
		return m_value ? "true" : "false";
	}

	std::string CharacterLiteral::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string CharacterLiteral::String()
	{
		char charToString[2] = { m_value, '\0' };

		return charToString;
	}

	std::string PrefixExpression::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string PrefixExpression::String()
	{
		std::ostringstream outputString;
		outputString << "(" << m_operator
			<< m_right_expression->String() << ")";

		return outputString.str();
	}

	std::string InfixExpression::TokenLiteral()
	{
		return m_token.m_literal;
	}
	std::string InfixExpression::String()
	{
		std::ostringstream outputString;
		outputString << "("
			<< m_left_expression->String() 
			<< " " << m_operator << " "
			<< m_right_expression->String() 
			<< ")";

		return outputString.str();
	}
}