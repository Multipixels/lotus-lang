#include <gtest/gtest.h>

#include "evaluator-test.h"
#include "lexer.h"
#include "parser.h"

TEST(EvaluatorTest, IntegerExpression)
{
	typedef struct TestCase
	{
		std::string input;
		int expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"5;", 5},
		{"11;", 11},
		{"-5;", -5},
		{"-11;", -11},
		{"-0;", 0},
		{"5 + 5;", 10},
		{"5 - 5;", 0},
		{"5 * 5;", 25},
		{"5 / 5;", 1},
		{"5 / 4;", 1}, // floor division for integers
		{"5 / 6;", 0}, // floor division for integers
		{"(24+7) * -3 - (100/3);", (24 + 7) * -3 - (100 / 3)}, // -126
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		
		ASSERT_EQ(evaluated->Type(), object::INTEGER);
		object::Integer* integer = (object::Integer*)evaluated;

		EXPECT_NO_FATAL_FAILURE(testIntegerObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, FloatExpression)
{
	typedef struct TestCase
	{
		std::string input;
		float expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"5f;", 5},
		{"11.0f;", 11},
		{"-5.5f;", -5.5f},
		{"-11.2f;", -11.2f},
		{"-0.0f;", 0.0f},
		{"5.1f + 5.1f;", 5.1f + 5.1f},
		{"5.0f - 5;", 0.0f},
		{"5 * 5.25f;", 5.0f * 5.25f},
		{"5 / 5.0f;", 1.0f},
		{"5 / 4.0f;", 5.0f / 4.0f},
		{"5.0f / 6;", 5.0f / 6.0f},
		{"(24+7) * -3 - (100/3.0f);", (24 + 7) * -3 - (100 / 3.0f)}, // -126.333 
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		ASSERT_EQ(evaluated->Type(), object::FLOAT);
		object::Float* integer = (object::Float*)evaluated;

		EXPECT_NO_FATAL_FAILURE(testFloatObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, BooleanExpression)
{
	typedef struct TestCase
	{
		std::string input;
		bool expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"true;", true},
		{"false;", false},
		{"!true;", false},
		{"!!true;", true},
		{"!false;", true},
		{"!5;", false},
		{"!!5;", true},
		{"!0;", true},
		{"2 > 3;", false},
		{"1.2f > 1.3f;", false},
		{"2 >= 3;", false},
		{"1.2f >= 1.3f;", false},
		{"2 < 3;", true},
		{"1.2f < 1.3f;", true},
		{"2 <= 3;", true},
		{"1.2f <= 1.3f;", true},
		{"3 < 3;", false},
		{"1.2f < 1.2f;", false},
		{"3 >= 3;", true},
		{"1.2f >= 1.2f;", true},
		{"3 <= 3;", true},
		{"1.2f <= 1.2f;", true},
		{"1 == 2;", false},
		{"1.1f == 1.2f;", false},
		{"1 == 1;", true},
		{"1.1f == 1.1f;", true},
		{"1 != 2;", true},
		{"1.0f != 1.1f;", true},
		{"1 != 1;", false},
		{"1.0f != 1.0f;", false},
		{"true && true;", true},
		{"true && false;", false},
		{"true || true;", true},
		{"true || false;", true},
		{"false || false;", false},
		{"(3 > 5) || (3 > 1 + 1);", true},
		{"'a' == 'a';", true},
		{"'a' == 'b';", false},
		{"'a' != 'a';", false},
		{"'a' != 'b';", true},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		EXPECT_NO_FATAL_FAILURE(testBooleanObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, CharacterExpression)
{
	typedef struct TestCase
	{
		std::string input;
		char expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"'a';", 'a'},
		{"'d';", 'd'},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		EXPECT_NO_FATAL_FAILURE(testCharacterObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, CollectionExpression)
{
	typedef struct TestCase
	{
		std::string input;
		std::vector<std::any> expectedValue;
		object::ObjectType objectType;
	} TestCase;

	TestCase tests[] =
	{
		{"[];", {}, object::NULL_TYPE},
		{"[1, 2, 3, 4, 5];", {1, 2, 3, 4, 5}, object::INTEGER},
		{"[1.0f, 2.0f, 3.0f, 4.0f, 5f];", {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}, object::FLOAT},
		{"[true, false];", {true, false}, object::BOOLEAN},
		{"['h', 'e', 'l', 'l', 'o'];", {'h', 'e', 'l', 'l', 'o'}, object::CHARACTER},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		EXPECT_NO_FATAL_FAILURE(testCollectionObject(evaluated, &tests[i].expectedValue, tests[i].objectType));
	}
}

TEST(EvaluatorTest, DictionaryExpression)
{
	typedef struct TestCase
	{
		std::string input;
		std::map<std::string, std::any> expectedValue;
		object::ObjectType expectedKeyType;
		object::ObjectType expectedValueType;
	} TestCase;

	TestCase tests[] =
	{
		{R"({};)", {}, object::NULL_TYPE, object::NULL_TYPE},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);;

		EXPECT_NO_FATAL_FAILURE(testDictionaryObject(evaluated, &tests[i].expectedValue, tests[i].expectedKeyType, tests[i].expectedValueType));
	}
}

TEST(EvaluatorTest, StringExpression)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{R"("someString";)", "someString"},
		{R"("";)", ""},
		{R"("this is a longer STRING! 7";)", "this is a longer STRING! 7"},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		ASSERT_EQ(evaluated->Type(), object::STRING);
		object::String* string = (object::String*)evaluated;

		EXPECT_NO_FATAL_FAILURE(testStringObject(string, &tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, CollectionIndexing)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"[1,2,3][0];", 1},
		{"[1,2,3][1];", 2},
		{"[1,2,3][2];", 3},
		{"collection<integer> myCollection = [1,2,3]; myCollection[2];", 3},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, DictionaryIndexing)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"{1: 2, 2: 3, 3: 4}[2];", 3},
		{"dictionary<character, float> myDictionary = {'a': 0.0f, 'b': 1.0f, 'c': 1.5f}; myDictionary['b'];", 1.0f},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, StringIndexing)
{
	typedef struct TestCase
	{
		std::string input;
		char expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{R"("someString"[3];)", 'e'},
		{R"("someString"[4];)", 'S'},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, ReturnStatement)
{
	typedef struct TestCase
	{
		std::string input;
		int expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"return 5;", 5},
		{"return 5 + 10;", 15},
		{"return 5 + 10; 9;", 15},
		{"9; return 5 + 10; 9;", 15},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		EXPECT_NO_FATAL_FAILURE(testIntegerObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, Declaration)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer a = 5; a;", 5},
		{"float b = 4.5f; b;", 4.5f},
		{"boolean c = false; c;", false},
		{"character d = 'e'; d;", 'e'},
		{"collection<integer> e = [6, 4]; e;", new std::vector<std::any>({6, 4}) },
		{R"(dictionary<integer, string> f = {0: "", 1: "a", 2: "aa"}; f;)", new std::map<std::string, std::any>({{"0", ""}, {"1", "a"}, {"2", "aa"}})},
		{R"(string g = "Hello, World!"; g;)", "Hello, World!" },
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, Function)
{
	std::string input = "integer(integer x) myFunction { return x + 2; }";

	lexer::Lexer lexer = lexer::Lexer(&input);
	parser::Parser parser = parser::Parser(lexer);
	ast::Program* program = parser.ParseProgram();
	object::Environment environment = new object::Environment();

	object::Object* object = evaluator::evaluate(program, &environment);
	ASSERT_EQ(object->Type(), object::NULL_TYPE);

	std::string functionName = "myFunction";
	object::Object* functionObj = environment.getIdentifier(&functionName);
	ASSERT_EQ(functionObj->Type(), object::FUNCTION);

	object::Function* function = (object::Function*)functionObj;

	EXPECT_EQ(function->m_function_type, object::INTEGER);

	EXPECT_EQ(function->m_parameters.size(), 1);
	EXPECT_EQ(function->m_parameters[0]->m_token.m_literal, "integer");
	EXPECT_EQ(function->m_parameters[0]->m_name.m_name, "x");

	EXPECT_EQ(function->m_function_name->m_name, "myFunction");
	EXPECT_EQ(function->m_body->String(), "return (x + 2);\n");
}

TEST(EvaluatorTest, FunctionCall)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer() integerFunction { return 5; }; integerFunction();", 5},
		{"integer(integer x) integerFunction { return x; }; integerFunction(6);", 6},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, Reassignment)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer myInt = 5; myInt = 6; myInt;", 6},
		{"integer myInt = 5; if (true) { myInt = 6; } myInt;", 6},
		{"integer myInt = 5; if (true) { integer myInt = 6; myInt = 7; } myInt;", 5},
		{"integer myInt = 5; integer() integerFunction { myInt = 6; return 5; }; integerFunction(); myInt;", 6},
		{"integer myInt = 5; integer() integerFunction { integer myInt = 6; myInt = 7; return 5; }; integerFunction(); myInt;", 5},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, CollectionReassignment)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"collection<integer> myCollection = [5, 3, 7]; myCollection[1] = 6; myCollection[1];", 6},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, DictionaryReassignment)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"dictionary<integer, character> myDictionary = {5: 'a', 3: 'c', 7: 'c'}; myDictionary[5] = 'c'; myDictionary[5];", 'c'},
		{"dictionary<integer, character> myDictionary = {5: 'a', 3: 'c', 7: 'c'}; myDictionary[6] = 'c'; myDictionary[6];", 'c'},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, IfStatement)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer myInt = 5; if (true) { myInt = 6; } myInt;", 6},
		{"integer myInt = 5; if (false) { myInt = 6; } myInt;", 5},
		{"integer myInt = 5; if (true) { myInt = 6; } else { myInt = 7; } myInt;", 6},
		{"integer myInt = 5; if (false) { myInt = 6; } else { myInt = 7; } myInt;", 7},
		{"integer myInt = 5; if (true) { myInt = 6; } else if(true) { myInt = 7; } else { myInt = 8; } myInt;", 6},
		{"integer myInt = 5; if (false) { myInt = 6; } else if(true) { myInt = 7; } else { myInt = 8; } myInt;", 7},
		{"integer myInt = 5; if (false) { myInt = 6; } else if(false) { myInt = 7; } else { myInt = 8; } myInt;", 8},
		{"integer myInt = 5; if (false) { myInt = 6; } else if(false) { myInt = 7; } myInt;", 5},
		{"integer myInt = 5; if (0) { myInt = 6; } myInt;", 5},
		{"integer myInt = 5; if (1) { myInt = 6; } myInt;", 6},
		{"integer myInt = 5; if (0.0f) { myInt = 6; } myInt;", 5},
		{"integer myInt = 5; if (-1.2f) { myInt = 6; } myInt;", 6},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, WhileLoop)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer i = 0; integer myInt = 5; while(false) { myInt = myInt + 1; } myInt;", 5},
		{"integer i = 0; integer myInt = 5; while( myInt < 10 ) { myInt = myInt + 1; } myInt;", 10},
		{"integer i = 0; integer myInt = 5; while( i < 5 ) { i = i + 1; myInt = myInt + 1; } myInt;", 10},
		{"integer i = 0; integer myInt = 5; while( i > 5 ) { i = i + 1; myInt = myInt + 1; } myInt;", 5},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, DoWhileLoop)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer i = 0; integer myInt = 5; do { myInt = myInt + 1; } while(false); myInt;", 6},
		{"integer i = 0; integer myInt = 5; do { myInt = myInt + 1; } while( myInt < 10 ); myInt;", 10},
		{"integer i = 0; integer myInt = 5; do { i = i + 1; myInt = myInt + 1; } while( i < 5 ); myInt;", 10},
		{"integer i = 0; integer myInt = 5; do { i = i + 1; myInt = myInt + 1; } while( i > 5 ); myInt;", 6},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, ForLoop)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer myInt = 5; for(integer i = 0; i < 5; i = i + 1;) { myInt = myInt + 1; } myInt;", 10},
		{"integer myInt = 5; for(; myInt < 5; myInt = myInt + 1;) { myInt = myInt + 1; } myInt;", 5},
		{"integer myInt = 5; for(; myInt < 10; myInt = myInt + 1;) { myInt = myInt + 1; } myInt;", 11},
		{"integer myInt = 5; for(; myInt < 10; ;) { myInt = myInt + 1; } myInt;", 10},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, IterateLoop)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer myInt = 0; collection<integer> myCollection = [1, 2, 3, 4]; iterate(value : myCollection) { myInt = myInt + 1; } myInt;", 4},
		{"integer myInt = 0; collection<integer> myCollection = [1, 2, 3, 4]; iterate(value : myCollection) { myInt = myInt + value; } myInt;", 10},
		{"integer myInt = 0; iterate(value : [1, 2, 3, 4]) { myInt = myInt + 1; } myInt;", 4},
		{"integer myInt = 0; iterate(value : [1, 2, 3, 4]) { myInt = myInt + value; } myInt;", 10},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, BuiltInFunctions)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{R"(log("Hello, World!");)", "Hello, World!"},
		{R"(string myString = "Hello, World!"; log(myString);)", "Hello, World!"},
		{R"(integer x = 7 - 5; log(x);)", "2"},
		{R"(size("Hello, World!");)", 13},
		{R"(string myString = "Hello, World!"; size(myString);)", 13},
		{R"(string myString = ""; size(myString);)", 0},
		{R"(collection<integer> myCollection = [1, 2, 3, 4]; size(myCollection);)", 4},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, Error)
{
	typedef struct TestCase
	{
		std::string input;
		std::string expectedError;
	} TestCase;

	TestCase tests[] =
	{
		{"5 + true;", "'integer + boolean' is not supported."},
		{"5 + true; 5;", "'integer + boolean' is not supported."},
		{"5; 5 + true; 5;", "'integer + boolean' is not supported."},
		{"-true;", "'-boolean' is not supported."},
		{"!'a';", "'!character' is not supported."},
		{"true + true;", "'boolean + boolean' is not supported."},
		{"undefinedIdentifier;", "'undefinedIdentifier' is not defined."},
		{"integer a = true;", "'a' is defined as type 'integer', not 'boolean'."},
		{"float(integer x) integerFunction { return x; }; integerFunction();", "'integerFunction' was supplied with 0 argument(s) instead of 1."},
		{"float(integer x) integerFunction { return x; }; integerFunction(6);", "'integerFunction(6)' produced a value of type 'integer' instead of type 'float'."},
		{"integer(integer x) integerFunction { return x; }; integerFunction(true);", "Parameter 'x' was supplied with a value of type 'boolean' instead of type 'integer' for the function call for 'integerFunction'."},
		{"integer(integer x) integerFunction { x; }; integerFunction(6);", "'integerFunction' has no return value."},
		{"integer myInt = 5; myInt = 6.5f; myInt;", "Cannot assign 'myInt' of type 'integer' a value of type 'float'."},
		{"integer myInt = 5; if ('a') { myInt = 6; } myInt;", "'a' is not a valid truthy value."},
		{"integer myInt = 5; integer myInt = 6;", "Redefinition of 'myInt'."},
		{"[2, 3, 4, 5.5f];", "The collection [2, 3, 4, 5.5] must have uniform typing of elements."},
		{"collection<integer> myCollection = [2, 3, 4, 5.5f];", "The collection [2, 3, 4, 5.5] must have uniform typing of elements."},
		{"collection<integer> myCollection = ['a'];", "'myCollection' is a collection of 'integer's, but got a collection of type 'character's."},
		{"integer myInt = 0; iterate(value : [1, 'a', 3]) { myInt = myInt + value; } myInt;", "The collection [1, 'a', 3] must have uniform typing of elements."},
		{"integer myInt = 0; iterate(value : ['a', 'b', 'c']) { myInt = myInt + value; } myInt;", "'integer + character' is not supported."},
		{"integer myInt = 26; size(myInt);", "Argument to `size` not supported, got integer."},
		{R"({1: 2, 2: 3, 'a': 4};)", "Dictionary has mismatching key types."},
		{R"({1: 2, 2: 3, 3: 'a'};)", "Dictionary has mismatching value types."},
		{R"({1: 2, 2: 3, 1: 1};)", "Dictionary initialized with duplicate key."},
		{R"({"hello": 2};)", "Invalid dictionary key type. string is not a hashable type."},
		{"{1: 2, 2: 3, 3: 4}[4];", "Index not in dictionary."},
		{"{1: 2, 2: 3, 3: 4}['a'];", "Dictionary has keys of type: 'integer'. Got type: 'character'"},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		object::Object* evaluated = testEvaluation(&tests[i].input);

		ASSERT_EQ(evaluated->Type(), object::ERROR);
		EXPECT_EQ(((object::Error*)evaluated)->m_error_message, tests[i].expectedError);
	}
}

object::Object* testEvaluation(std::string* input)
{
	lexer::Lexer lexer = lexer::Lexer(input);
	parser::Parser parser = parser::Parser(lexer);
	ast::Program* program = parser.ParseProgram();
	object::Environment environment = new object::Environment();

	return evaluator::evaluate(program, &environment);
}

void testLiteralObject(object::Object* object, std::any expectedValue)
{
	// Cannot use a switch on type as std::string is not integral
	// Minor efficiency tradeoff, but this is just a test suite, not the actual interpreter
	
	if (object == NULL) FAIL();

	switch (object->Type())
	{
	case object::INTEGER:
		EXPECT_NO_FATAL_FAILURE(testIntegerObject(object, std::any_cast<int>(expectedValue)));
		return;
	case object::FLOAT:
		EXPECT_NO_FATAL_FAILURE(testFloatObject(object, std::any_cast<float>(expectedValue)));
		return;
	case object::BOOLEAN:
		EXPECT_NO_FATAL_FAILURE(testBooleanObject(object, std::any_cast<bool>(expectedValue)));
		return;
	case object::CHARACTER:
		EXPECT_NO_FATAL_FAILURE(testCharacterObject(object, std::any_cast<char>(expectedValue)));
		return;
	case object::COLLECTION:
		EXPECT_NO_FATAL_FAILURE(testCollectionObject(object, std::any_cast<std::vector<std::any>*>(expectedValue), object::INTEGER));
		return;
	case object::DICTIONARY:
		EXPECT_NO_FATAL_FAILURE(testDictionaryObject(object, std::any_cast<std::map<std::string, std::any>*>(expectedValue), object::INTEGER, object::STRING));
		return;
	case object::STRING:
	{
		std::string expectedString = std::any_cast<const char*>(expectedValue);
		EXPECT_NO_FATAL_FAILURE(testStringObject(object, &expectedString));
		return;
	}
	}

	FAIL();
}

void testIntegerObject(object::Object* object, int expectedValue)
{
	ASSERT_EQ(object->Type(), object::INTEGER);
	object::Integer* integer = (object::Integer*)object;

	EXPECT_EQ(integer->m_value, expectedValue);
}

void testFloatObject(object::Object* object, float expectedValue)
{
	ASSERT_EQ(object->Type(), object::FLOAT);
	object::Float* floating = (object::Float*)object;

	EXPECT_EQ(floating->m_value, expectedValue);
}

void testBooleanObject(object::Object* object, bool expectedValue)
{
	ASSERT_EQ(object->Type(), object::BOOLEAN);
	object::Boolean* boolean = (object::Boolean*)object;

	EXPECT_EQ(boolean->m_value, expectedValue);
}

void testCharacterObject(object::Object* object, char expectedValue)
{
	ASSERT_EQ(object->Type(), object::CHARACTER);
	object::Character* character = (object::Character*)object;

	EXPECT_EQ(character->m_value, expectedValue);
}

void testCollectionObject(object::Object* object, std::vector<std::any>* expectedValue, object::ObjectType expectedType)
{
	ASSERT_EQ(object->Type(), object::COLLECTION);
	object::Collection* collection = (object::Collection*)object;

	ASSERT_EQ(collection->m_collection_type, expectedType);

	for (int i = 0; i < collection->m_values.size(); i++)
	{
		ASSERT_EQ(collection->m_values[i]->Type(), expectedType);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(collection->m_values[i], (*expectedValue)[i]));
	}
}

void testDictionaryObject(object::Object* object, std::map<std::string, std::any>* expectedValue, object::ObjectType expectedKeyType, object::ObjectType expectedValueType)
{
	ASSERT_EQ(object->Type(), object::DICTIONARY);
	object::Dictionary* dictionary = (object::Dictionary*)object;

	ASSERT_EQ(dictionary->m_key_type, expectedKeyType);
	ASSERT_EQ(dictionary->m_value_type, expectedValueType);

	std::map<object::Object*, object::Object*>::iterator it;
	for (it = dictionary->m_map.begin(); it != dictionary->m_map.end(); it++)
	{
		ASSERT_EQ(it->first->Type(), expectedKeyType);
		ASSERT_EQ(it->second->Type(), expectedValueType);

		switch (it->first->Type()) {
		case object::INTEGER:
		{
			object::Integer* integerLiteral = (object::Integer*)it->first;
			EXPECT_NO_FATAL_FAILURE(testLiteralObject(dictionary->m_map.at(it->first), expectedValue->at(std::to_string(integerLiteral->m_value))));
			break;
		}
		case object::FLOAT:
		{
			object::Float* floatLiteral = (object::Float*)it->first;
			EXPECT_NO_FATAL_FAILURE(testLiteralObject(dictionary->m_map.at(it->first), expectedValue->at(std::to_string(floatLiteral->m_value))));
			break;
		}
		case object::BOOLEAN:
		{
			object::Boolean* booleanLiteral = (object::Boolean*)it->first;
			EXPECT_NO_FATAL_FAILURE(testLiteralObject(dictionary->m_map.at(it->first), expectedValue->at(booleanLiteral->m_value ? "true" : "false")));
			break;
		}
		case object::CHARACTER:
		{
			object::Character* characterLiteral = (object::Character*)it->first;
			EXPECT_NO_FATAL_FAILURE(testLiteralObject(dictionary->m_map.at(it->first), expectedValue->at(std::string(1, characterLiteral->m_value))));
			break;
		}
		default:
		{
			FAIL() << "Unexpected type found in dictionary.";
			break;
		}
		}
	}
}

void testStringObject(object::Object* object, std::string* expectedValue)
{
	ASSERT_EQ(object->Type(), object::STRING);
	object::String* string = (object::String*)object;

	EXPECT_EQ(string->m_value, *expectedValue);
}