#pragma once

#include <any>

#include "evaluator.h"

// Lexes and parses through a program
object::Object* testEvaluation(std::string* input);

// Checks type of object and defers it to proper testing function
void testLiteralObject(object::Object* object, std::any expectedValue);

// Tests object to see if they are integer with expected value
void testIntegerObject(object::Object* object, int expectedValue);

// Tests object to see if they are float with expected value
void testFloatObject(object::Object* object, float expectedValue);

// Tests object to see if they are boolean with expected value
void testBooleanObject(object::Object* object, bool expectedValue);

// Tests object to see if they are character with expected value
void testCharacterObject(object::Object* object, char expectedValue);

// Tests collection object to see if they match expected values
void testCollectionObject(object::Object* object, std::vector<std::any>* expectedValue, object::ObjectType expectedType);

// Tests dictionary object to see if they match expected values
void testDictionaryObject(object::Object* object, std::map<std::string, std::any>* expectedValue, object::ObjectType expectedKeyType, object::ObjectType expectedValueType);

// Tests object to see if they are string with expected value
void testStringObject(object::Object* object, std::string* expectedValue);
