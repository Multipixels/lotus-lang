#pragma once

#include "evaluator.h"

// Lexes and parses through a program
object::Object* testEvaluation(std::string* input);

// Tests object to see if they are integer with expected value
void testIntegerObject(object::Object* object, int expectedValue);

// Tests object to see if they are float with expected value
void testFloatObject(object::Object* object, float expectedValue);

// Tests object to see if they are boolean with expected value
void testBooleanObject(object::Object* object, bool expectedValue);

// Tests object to see if they are character with expected value
void testCharacterObject(object::Object* object, char expectedValue);