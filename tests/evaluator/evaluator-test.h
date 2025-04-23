#pragma once

#include <any>

#include "evaluator.h"

// Lexes and parses through a program
std::shared_ptr<object::Object> testEvaluation(std::string* p_input);

// Checks type of object and defers it to proper testing function
void testLiteralObject(std::shared_ptr<object::Object> p_object, std::any p_expectedValue);

// Tests object to see if they are integer with expected value
void testIntegerObject(std::shared_ptr<object::Object> p_object, int p_expectedValue);

// Tests object to see if they are float with expected value
void testFloatObject(std::shared_ptr<object::Object> p_object, float p_expectedValue);

// Tests object to see if they are boolean with expected value
void testBooleanObject(std::shared_ptr<object::Object> p_object, bool p_expectedValue);

// Tests object to see if they are character with expected value
void testCharacterObject(std::shared_ptr<object::Object> p_object, char p_expectedValue);

// Tests collection object to see if they match expected values
void testCollectionObject(std::shared_ptr<object::Object> p_object, std::vector<std::any>* p_expectedValue, object::ObjectType p_expectedType);

// Tests dictionary object to see if they match expected values
void testDictionaryObject(std::shared_ptr<object::Object> p_object, std::map<std::string, std::any>* p_expectedValue, object::ObjectType p_expectedKeyType, object::ObjectType p_expectedValueType);

// Tests object to see if they are string with expected value
void testStringObject(std::shared_ptr<object::Object> p_object, std::string* p_expectedValue);
