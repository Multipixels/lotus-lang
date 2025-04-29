#pragma once

#include <map>

#include "object.h"

namespace evaluator
{
	std::shared_ptr<object::Object> logBuiltIn(std::vector<std::shared_ptr<object::Object>>* p_params);
	std::shared_ptr<object::Object> sizeBuiltIn(std::vector<std::shared_ptr<object::Object>>* p_params);

	const std::map<std::string, std::shared_ptr<object::Object>> c_builtins =
	{
		{"log", std::make_shared<object::Builtin>(&logBuiltIn)},
		{"size", std::make_shared<object::Builtin>(&sizeBuiltIn)},
	};

}