#pragma once

#include "evaluator.h"

// Lexes and parses through a program
object::Object* testEvaluation(std::string* input);

// Tests object to see if they are integer with expected value
void testIntegerObject(object::Object* object, int expectedValue);

// Tests object to see if they are boolean with expected value
void testBooleanObject(object::Object* object, bool expectedValue);
