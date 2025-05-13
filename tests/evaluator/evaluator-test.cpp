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
		{"5 % 5;", 0},
		{"27 % 5;", 2},
		{"-7 % 5;", -2},
		{"(24+7) * -3 - (100/3);", (24 + 7) * -3 - (100 / 3)}, // -126
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
		
		ASSERT_EQ(evaluated->Type(), object::INTEGER);
		std::shared_ptr<object::Integer> integer = std::static_pointer_cast<object::Integer>(evaluated);

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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);

		ASSERT_EQ(evaluated->Type(), object::FLOAT);
		std::shared_ptr<object::Float> integer = std::static_pointer_cast<object::Float>(evaluated);

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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);

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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);

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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);

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
		{R"({'a': 3, 'b': 4};)", {{"a", 3}, {"b", 4}}, object::CHARACTER, object::INTEGER},
		{R"({7: 3, 8: 4};)", {{"7", 3}, {"8", 4}}, object::INTEGER, object::INTEGER},
		{R"({7: "hello", 8: "bye"};)", {{"7", "hello"}, {"8", "bye"}}, object::INTEGER, object::STRING},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);;

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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);

		ASSERT_EQ(evaluated->Type(), object::STRING);
		std::shared_ptr<object::String> string = std::static_pointer_cast<object::String>(evaluated);

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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);

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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);

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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);

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
		{"for(integer i = 0; i < 10; i = i + 1) { return i; }", 0},
		{"integer i = 0; while(true) { return i; i = i + 1; }", 0},
		{"integer i = 0; do { return i; i = i + 1; } while(true);", 0},
		{"integer i = 0; iterate(value : [1,2,3]) { return value; } ", 1},
		
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);

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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, Function)
{
	std::string input = "integer(integer x) myFunction { return x + 2; }";

	lexer::Lexer lexer = lexer::Lexer(&input);
	parser::Parser parser = parser::Parser(lexer);
	std::shared_ptr<ast::Program> program = parser.ParseProgram();
	std::shared_ptr<object::Environment> environment(new object::Environment);

	std::shared_ptr<object::Object> object = evaluator::evaluate(program, environment);
	ASSERT_EQ(object->Type(), object::NULL_TYPE);

	std::string functionName = "myFunction";
	std::shared_ptr<object::Object> functionObj = environment->getIdentifier(&functionName);
	ASSERT_EQ(functionObj->Type(), object::FUNCTION);

	std::shared_ptr<object::Function> function = std::static_pointer_cast<object::Function>(functionObj);

	EXPECT_EQ(function->m_functionType, object::INTEGER);

	EXPECT_EQ(function->m_parameters.size(), 1);
	EXPECT_EQ(function->m_parameters[0]->m_token.m_literal, "integer");
	EXPECT_EQ(function->m_parameters[0]->m_name.m_name, "x");

	EXPECT_EQ(function->m_functionName.m_name, "myFunction");
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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
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
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
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
		{"integer myInt = 5; for(integer i = 0; i < 5; i = i + 1) { myInt = myInt + 1; } myInt;", 10},
		{"integer myInt = 5; for(; myInt < 5; myInt = myInt + 1) { myInt = myInt + 1; } myInt;", 5},
		{"integer myInt = 5; for(; myInt < 10; myInt = myInt + 1) { myInt = myInt + 1; } myInt;", 11},
		{"integer myInt = 5; for(; myInt < 10; ) { myInt = myInt + 1; } myInt;", 10},
		{"integer myInt = 0; for(integer i = 0; i < 5; i = i + 1) { integer myInteger = i + 1; myInt = myInt + myInteger; } myInt;", 15},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
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
		{"integer myInt = 0; dictionary<character, integer> myDictionary = {'a': 0, 'b': 1, 'c': 2, 'd': 3, 'e': 4}; iterate(key : myDictionary) { myInt = myInt + myDictionary[key]; } myInt;", 10},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, BuiltinFunctions)
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
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, StringMemberFunctions)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{R"("Hello, World!".length;)", 13},
		{R"(string myString = "Hello, World!"; myString.length;)", 13},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}

TEST(EvaluatorTest, CollectionMemberFunctions)
{
	typedef struct TestCase
	{
		std::string input;
		std::vector<std::any> expectedValue;
		object::ObjectType objectType;
	} TestCase;

	TestCase tests[] =
	{
		{R"([].append(1); collection<integer> myCollection = [1, 2, 3]; myCollection;)", {1, 2, 3}, object::INTEGER},
		{R"([1].append(1); collection<integer> myCollection = [1, 2, 3]; myCollection;)", {1, 2, 3}, object::INTEGER},
		{R"(collection<integer> myCollection = [1, 2, 3]; collection<integer> returnValue = [myCollection.size]; myCollection;)", {1, 2, 3}, object::INTEGER},
		{R"(collection<integer> myCollection = [1, 2, 3]; collection<integer> returnValue = [myCollection.size]; myCollection;)", {1, 2, 3}, object::INTEGER},
		{R"(collection<integer> myCollection = [1, 2, 3]; myCollection.append(4); myCollection;)", {1, 2, 3, 4}, object::INTEGER},
		{R"(collection<integer> myCollection = [1, 2, 3]; myCollection.append(1); myCollection;)", {1, 2, 3, 1}, object::INTEGER},
		{R"(collection<integer> myCollection = [1, 2, 3]; myCollection.pop(); myCollection;)", {1, 2}, object::INTEGER},
		{R"(collection<integer> myCollection = [1]; myCollection.pop(); myCollection;)", {}, object::INTEGER},
		{R"(collection<integer> myCollection = [1, 2, 3]; myCollection.pop(0); myCollection;)", {2, 3}, object::INTEGER},
		{R"(collection<integer> myCollection = [1, 2, 3]; myCollection.pop(1); myCollection;)", {1, 3}, object::INTEGER},
		{R"(collection<integer> myCollection = [1]; myCollection.insert(0, 5); myCollection;)", {5, 1}, object::INTEGER},
		{R"(collection<integer> myCollection = [1, 2, 3]; myCollection.insert(1, 10); myCollection;)", {1, 10, 2, 3}, object::INTEGER},
		{R"(collection<integer> myCollection = [1, 2, 3]; myCollection.insert(3, 10); myCollection;)", {1, 2, 3, 10}, object::INTEGER},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);
		EXPECT_NO_FATAL_FAILURE(testCollectionObject(evaluated, &tests[i].expectedValue, tests[i].objectType));
	}
}


TEST(EvaluatorTest, DictionaryMemberFunctions)
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
		{R"(dictionary<integer, integer> myDictionary = {}; dictionary<integer, integer> returnValue = {0: myDictionary.size}; returnValue;)", {{"0", 0}}, object::INTEGER, object::INTEGER},
		{R"(dictionary<character, integer> myDictionary = {'a': 3, 'b': 4}; dictionary<integer, integer> returnValue = {0: myDictionary.size}; returnValue;)", {{"0", 2}}, object::INTEGER, object::INTEGER},
		{R"(dictionary<character, integer> myDictionary = {'a': 3, 'b': 4}; 
			collection<character> keys = myDictionary.keys(); 
			iterate(key : keys) { myDictionary[key]; } myDictionary;)", {{"a", 3}, {"b", 4}}, object::CHARACTER, object::INTEGER},
		{R"(dictionary<character, integer> myDictionary = {'a': 3, 'b': 4}; 
			collection<character> keys = myDictionary.keys(); 
			collection<integer> values = myDictionary.values(); 
			dictionary<integer, boolean> result = {};
			iterate(value : values) { 
				iterate(key : keys) {
					if(myDictionary[key] == value) {
						result[value] = true;
					}
				}
			} 
			result;)", {{"3", true}, {"4", true}}, object::INTEGER, object::BOOLEAN},

	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);;
		EXPECT_NO_FATAL_FAILURE(testDictionaryObject(evaluated, &tests[i].expectedValue, tests[i].expectedKeyType, tests[i].expectedValueType));
	}
}


TEST(EvaluatorTest, BreakStatement)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer myInteger = 0; while(true) { myInteger = myInteger + 1; if(myInteger == 5) { break; } } myInteger;", 5},
		{"integer myInteger = 0; do { myInteger = myInteger + 1; break; } while(true); myInteger;", 1},
		{"integer myInteger = 0; for(integer i = 0; i < 10; i = i + 1) { myInteger = myInteger + 1; break; } myInteger;", 1},
		{"integer myInteger = 0; iterate(value : [1, 2, 3, 4]) { myInteger = myInteger + 1; if(value == 3) { break; } } myInteger;", 3},

	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);;
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}


TEST(EvaluatorTest, ContinueStatement)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer myInteger = 0; integer i = 0; while(i < 10) {  i = i + 1; if(i > 5) { continue; } myInteger = myInteger + 1; } myInteger;", 5},
		{"integer myInteger = 0; integer i = 0; do { i = i + 1; if(i > 5) { continue; } myInteger = myInteger + 1; } while(i < 10); myInteger;", 5},
		{"integer myInteger = 0; for(integer i = 0; i < 10; i = i + 1) { if(i / 2 == 2) { log(i); continue; } myInteger = myInteger + 1; } myInteger;", 8},
		{"integer myInteger = 0; iterate(value : [1, 2, 3, 4]) { if(value == 3) { continue; } myInteger = myInteger + value; } myInteger;", 7},

	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);;
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}


TEST(EvaluatorTest, OperatorAssignments)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer myInteger = 12; myInteger += 1; myInteger;", 13},
		{"integer myInteger = 12; myInteger -= 2; myInteger;", 10},
		{"integer myInteger = 12; myInteger *= 3; myInteger;", 36},
		{"integer myInteger = 12; myInteger /= 4; myInteger;", 3},
		{"integer myInteger = 12; myInteger %= 5; myInteger;", 2},
		{"float myFloat = 12.5f; myFloat += 5; myFloat;", 17.5f},
		{"collection<integer> myCollection = [1, 2, 3, 4]; myCollection[3] += 5; myCollection[3];", 9},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);;
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(evaluated, tests[i].expectedValue));
	}
}


TEST(EvaluatorTest, IncrementDecrement)
{
	typedef struct TestCase
	{
		std::string input;
		std::any expectedValue;
	} TestCase;

	TestCase tests[] =
	{
		{"integer a = 5; a++;", 5},
		{"integer a = 5; a++; a;", 6},
		{"integer a = 5; a++ + 5;", 10},
		{"integer a = 5; a++ + 5; a;", 6},
		{"integer a = 5; a--;", 5},
		{"integer a = 5; a--; a;", 4},
		{"integer a = 5; a-- + 5;", 10},
		{"integer a = 5; a-- + 5; a;", 4},
		{"collection<integer> b = [1, 2, 3]; b[1]++;", 2},
		{"collection<integer> b = [1, 2, 3]; b[1]++; b[1];", 3},
		{"integer a = 5; 5 + a++ + 5;", 15},
		{"integer a = 23; (a++ +7) * -3 - (100/3.0f);", (23 + 7) * -3 - (100 / 3.0f)}, // -126.333 

		{"integer a = 5; ++a;", 6},
		{"integer a = 5; ++a; a;", 6},
		{"integer a = 5; ++a + 5;", 11},
		{"integer a = 5; ++a + 5; a;", 6},
		{"integer a = 5; --a;", 4},
		{"integer a = 5; --a; a;", 4},
		{"integer a = 5; --a + 5;", 9},
		{"integer a = 5; --a + 5; a;", 4},
		{"collection<integer> b = [1, 2, 3]; ++b[1];", 3},
		{"collection<integer> b = [1, 2, 3]; ++b[1]; b[1];", 3},
		{"integer a = 5; 5 + ++a + 5;", 16},
		{"integer a = 23; (++a +7) * -3 - (100/3.0f);", (24 + 7) * -3 - (100 / 3.0f)}, // -126.333 
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);;
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
		{"integer(integer x) integerFunction { }; integerFunction(6);", "'integerFunction' has no return value."},
		{"integer() integerFunction { }; integerFunction();", "'integerFunction' has no return value."},
		{"integer myInt = 5; myInt = 6.5f; myInt;", "Cannot assign 'myInt' of type 'integer' a value of type 'float'."},
		{"integer myInt = 5; if ('a') { myInt = 6; } myInt;", "'a' is not a valid truthy value."},
		{"integer myInt = 5; integer myInt = 6;", "Redefinition of 'myInt'."},
		{"[2, 3, 4, 5.5f];", "The collection [2, 3, 4, 5.5] must have uniform typing of elements."},
		{"collection<integer> myCollection = [2, 3, 4, 5.5f];", "The collection [2, 3, 4, 5.5] must have uniform typing of elements."},
		{"collection<integer> myCollection = ['a'];", "'myCollection' is a collection of 'integer's, but got a collection of type 'character's."},
		{"integer myInt = 0; iterate(value : [1, 'a', 3]) { myInt = myInt + value; } myInt;", "The collection [1, 'a', 3] must have uniform typing of elements."},
		{"integer myInt = 0; iterate(value : ['a', 'b', 'c']) { myInt = myInt + value; } myInt;", "'integer + character' is not supported."},
		{R"({1: 2, 2: 3, 'a': 4};)", "Dictionary has mismatching key types."},
		{R"({1: 2, 2: 3, 3: 'a'};)", "Dictionary has mismatching value types."},
		{R"({1: 2, 2: 3, 1: 1};)", "Dictionary initialized with duplicate key."},
		{R"({"hello": 2};)", "Invalid dictionary key type. string is not a hashable type."},
		{"{1: 2, 2: 3, 3: 4}[4];", "Index not in dictionary."},
		{"{1: 2, 2: 3, 3: 4}['a'];", "Dictionary has keys of type: 'integer'. Got type: 'character'"},
		{R"("this is a string".size;)", "size is not a member variable or function for an object of type string."},
		{R"("this is a string".length();)", R"('("this is a string" . length)' is not a function.)"},
		{"collection<integer> myCollection = [2, 3, 4]; myCollection.append(1, 2);", "Expected 1 parameter, got 2."},
		{"collection<integer> myCollection = [2, 3, 4]; myCollection.append('a');", "Collection is of type `integer', but tried to append a value of type `character`."},
		{"collection<integer> myCollection = []; myCollection.append('a');", "Collection is of type `integer', but tried to append a value of type `character`."},
		{"collection<integer> myCollection = [2, 3, 4]; myCollection.pop(-1);", "Attempted to pop an index that is out of bounds."},
		{"collection<integer> myCollection = [2, 3, 4]; myCollection.pop(3);", "Attempted to pop an index that is out of bounds."},
		{"collection<integer> myCollection = [2, 3, 4]; myCollection.insert(0, 'a');", "Collection is of type `integer', but tried to insert a value of type `character`."},
		{"collection<integer> myCollection = [2, 3, 4]; myCollection.insert(-1, 10);", "Attempted to insert into an index that is out of bounds."},
		{"collection<integer> myCollection = [2, 3, 4]; myCollection.insert(4, 10);", "Attempted to insert into an index that is out of bounds."},
		{"dictionary<character, integer> myDictionary = {'a': 0, 'b': 1, 'c': 2}; collection<integer> myCollection = myDictionary.keys();", "'myCollection' is a collection of 'integer's, but got a collection of type 'character's."},
		{"integer() myFunc { break; } while(true) { myFunc(); }", "Attempted to break outside a loop."},
		{"break;", "Attempted to break outside a loop."},
		{"integer() myFunc { continue; } while(true) { myFunc(); }", "Attempted to continue outside a loop."},
		{"continue;", "Attempted to continue outside a loop."},
		{"integer myInteger = 12; myInteger += 5.5f;", "Cannot assign 'myInteger' of type 'integer' a value of type 'float'."},
		{"integer myInteger = 12; myInteger += 'a';", "'integer + character' is not supported."},
		{"character myInteger = 'a'; myInteger %= 3;", "'character % integer' is not supported."},
		{"5 += 3;", "'integer += integer' is not supported."},
		{"5 / 0;", "Attempted division by zero."},
		{"5.5f / 0.0f;", "Attempted division by zero."},
		{"5 % 0;", "Attempted modulo by zero."},
	};

	for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++)
	{
		std::shared_ptr<object::Object> evaluated = testEvaluation(&tests[i].input);

		ASSERT_EQ(evaluated->Type(), object::ERROR);
		EXPECT_EQ(std::static_pointer_cast<object::Error>(evaluated)->m_errorMessage, tests[i].expectedError);
	}
}

std::shared_ptr<object::Object> testEvaluation(std::string* p_input)
{
	lexer::Lexer lexer = lexer::Lexer(p_input);
	parser::Parser parser = parser::Parser(lexer);
	std::shared_ptr<ast::Program> program = parser.ParseProgram();
	std::shared_ptr<object::Environment> environment(new object::Environment());

	return evaluator::evaluate(program, environment);
}

void testLiteralObject(std::shared_ptr<object::Object> p_object, std::any p_expectedValue)
{
	// Cannot use a switch on type as std::string is not integral
	// Minor efficiency tradeoff, but this is just a test suite, not the actual interpreter
	
	if (p_object == NULL) FAIL();

	switch (p_object->Type())
	{
	case object::INTEGER:
		EXPECT_NO_FATAL_FAILURE(testIntegerObject(p_object, std::any_cast<int>(p_expectedValue)));
		return;
	case object::FLOAT:
		EXPECT_NO_FATAL_FAILURE(testFloatObject(p_object, std::any_cast<float>(p_expectedValue)));
		return;
	case object::BOOLEAN:
		EXPECT_NO_FATAL_FAILURE(testBooleanObject(p_object, std::any_cast<bool>(p_expectedValue)));
		return;
	case object::CHARACTER:
		EXPECT_NO_FATAL_FAILURE(testCharacterObject(p_object, std::any_cast<char>(p_expectedValue)));
		return;
	case object::COLLECTION:
		EXPECT_NO_FATAL_FAILURE(testCollectionObject(p_object, std::any_cast<std::vector<std::any>*>(p_expectedValue), object::INTEGER));
		return;
	case object::DICTIONARY:
		EXPECT_NO_FATAL_FAILURE(testDictionaryObject(p_object, std::any_cast<std::map<std::string, std::any>*>(p_expectedValue), object::INTEGER, object::STRING));
		return;
	case object::STRING:
	{
		std::string expectedString = std::any_cast<const char*>(p_expectedValue);
		EXPECT_NO_FATAL_FAILURE(testStringObject(p_object, &expectedString));
		return;
	}
	case object::ERROR:
	{
		std::cout << std::static_pointer_cast<object::Error>(p_object)->m_errorMessage << std::endl;
		FAIL();
		return;
	}
	default:
	{
		std::cout << "Unexpected object type received. Got object of type " << object::c_objectTypeToString.at(p_object->Type()) << "." << std::endl;
		FAIL();
		return;
	}
	}

	FAIL();
}

void testIntegerObject(std::shared_ptr<object::Object> p_object, int p_expectedValue)
{
	ASSERT_EQ(p_object->Type(), object::INTEGER);
	std::shared_ptr<object::Integer> integer = std::static_pointer_cast<object::Integer>(p_object);

	EXPECT_EQ(integer->m_value, p_expectedValue);
}

void testFloatObject(std::shared_ptr<object::Object> p_object, float p_expectedValue)
{
	ASSERT_EQ(p_object->Type(), object::FLOAT);
	std::shared_ptr<object::Float> floating = std::static_pointer_cast<object::Float>(p_object);

	EXPECT_EQ(floating->m_value, p_expectedValue);
}

void testBooleanObject(std::shared_ptr<object::Object> p_object, bool p_expectedValue)
{
	ASSERT_EQ(p_object->Type(), object::BOOLEAN);
	std::shared_ptr<object::Boolean> boolean = std::static_pointer_cast<object::Boolean>(p_object);

	EXPECT_EQ(boolean->m_value, p_expectedValue);
}

void testCharacterObject(std::shared_ptr<object::Object> p_object, char p_expectedValue)
{
	ASSERT_EQ(p_object->Type(), object::CHARACTER);
	std::shared_ptr<object::Character> character = std::static_pointer_cast<object::Character>(p_object);

	EXPECT_EQ(character->m_value, p_expectedValue);
}

void testCollectionObject(std::shared_ptr<object::Object> p_object, std::vector<std::any>* p_expectedValue, object::ObjectType p_expectedType)
{
	ASSERT_EQ(p_object->Type(), object::COLLECTION);
	std::shared_ptr<object::Collection> collection = std::static_pointer_cast<object::Collection>(p_object);

	ASSERT_EQ(collection->m_collectionType, p_expectedType);

	for (int i = 0; i < collection->m_values.size(); i++)
	{
		ASSERT_EQ(collection->m_values[i]->Type(), p_expectedType);
		EXPECT_NO_FATAL_FAILURE(testLiteralObject(collection->m_values[i], (*p_expectedValue)[i]));
	}
}

void testDictionaryObject(std::shared_ptr<object::Object> p_object, std::map<std::string, std::any>* p_expectedValue, object::ObjectType p_expectedKeyType, object::ObjectType p_expectedValueType)
{
	ASSERT_EQ(p_object->Type(), object::DICTIONARY);
	std::shared_ptr<object::Dictionary> dictionary = std::static_pointer_cast<object::Dictionary>(p_object);

	ASSERT_EQ(dictionary->m_keyType, p_expectedKeyType);
	ASSERT_EQ(dictionary->m_valueType, p_expectedValueType);

	std::map<std::shared_ptr<object::Object>, std::shared_ptr<object::Object>>::iterator it;
	for (it = dictionary->m_map.begin(); it != dictionary->m_map.end(); it++)
	{
		ASSERT_EQ(it->first->Type(), p_expectedKeyType);
		ASSERT_EQ(it->second->Type(), p_expectedValueType);

		switch (it->first->Type()) {
		case object::INTEGER:
		{
			std::shared_ptr<object::Integer> integerLiteral = std::static_pointer_cast<object::Integer>(it->first);
			EXPECT_NO_FATAL_FAILURE(testLiteralObject(dictionary->m_map.at(it->first), p_expectedValue->at(std::to_string(integerLiteral->m_value))));
			break;
		}
		case object::FLOAT:
		{
			std::shared_ptr<object::Float> floatLiteral = std::static_pointer_cast<object::Float>(it->first);
			EXPECT_NO_FATAL_FAILURE(testLiteralObject(dictionary->m_map.at(it->first), p_expectedValue->at(std::to_string(floatLiteral->m_value))));
			break;
		}
		case object::BOOLEAN:
		{
			std::shared_ptr<object::Boolean> booleanLiteral = std::static_pointer_cast<object::Boolean>(it->first);
			EXPECT_NO_FATAL_FAILURE(testLiteralObject(dictionary->m_map.at(it->first), p_expectedValue->at(booleanLiteral->m_value ? "true" : "false")));
			break;
		}
		case object::CHARACTER:
		{
			std::shared_ptr<object::Character> characterLiteral = std::static_pointer_cast<object::Character>(it->first);
			EXPECT_NO_FATAL_FAILURE(testLiteralObject(dictionary->m_map.at(it->first), p_expectedValue->at(std::string(1, characterLiteral->m_value))));
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

void testStringObject(std::shared_ptr<object::Object> p_object, std::string* p_expectedValue)
{
	ASSERT_EQ(p_object->Type(), object::STRING);
	std::shared_ptr<object::String> string = std::static_pointer_cast<object::String>(p_object);

	EXPECT_EQ(string->m_value, *p_expectedValue);
}