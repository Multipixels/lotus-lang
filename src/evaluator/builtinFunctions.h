#pragma once

#include <map>

#include "object.h"

namespace evaluator
{
	std::shared_ptr<object::Object> logBuiltin(std::vector<std::shared_ptr<object::Object>>* p_params, object::Object* p_object);

	std::shared_ptr<object::Object> collectionAppend(std::vector<std::shared_ptr<object::Object>>* p_params, object::Object* p_object);

	const std::map<std::string, std::shared_ptr<object::Builtin>> c_builtins =
	{
		{"log", std::make_shared<object::Builtin>(&logBuiltin)},
	};

}