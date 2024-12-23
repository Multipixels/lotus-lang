#pragma once

#include "ast.h"
#include "object.h"

namespace evaluator
{
	object::Object* evaluate(ast::Node* node);
}