#pragma once

#include <any>

#include "evaluator.h"

// Lexes and parses through a program from a file
std::shared_ptr<object::Object> testEvaluationFile(std::string* p_input);
