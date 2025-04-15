#pragma once

#include <map>

#include "object.h"

namespace evaluator
{
	object::Object* logBuiltIn(std::vector<object::Object*>* params);
	object::Object* sizeBuiltIn(std::vector<object::Object*>* params);

	const std::map<std::string, object::Object*> builtins =
	{
		{"log", new object::Builtin(&logBuiltIn)},
		{"size", new object::Builtin(&sizeBuiltIn)},
	};

}