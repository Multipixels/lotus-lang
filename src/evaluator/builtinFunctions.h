#pragma once

#include <map>

#include "object.h"

namespace evaluator
{
	std::shared_ptr<object::Object> logBuiltIn(std::vector<std::shared_ptr<object::Object>>* params);
	std::shared_ptr<object::Object> sizeBuiltIn(std::vector<std::shared_ptr<object::Object>>* params);

	const std::map<std::string, std::shared_ptr<object::Object>> builtins =
	{
		{"log", std::shared_ptr<object::Object>(new object::Builtin(&logBuiltIn))},
		{"size", std::shared_ptr<object::Object>(new object::Builtin(&sizeBuiltIn))},
	};

}