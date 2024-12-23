#include "evaluator.h"

namespace evaluator
{
	object::Object* evaluate(ast::Node* node)
	{
		switch (node->Type())
		{
		case ast::INTEGER_LITERAL_NODE:
			object::Integer* object = new object::Integer;
			object->m_value = ((ast::IntegerLiteral*)object)->m_value;
			return object;
		}

		return NULL;
		
	}
}
