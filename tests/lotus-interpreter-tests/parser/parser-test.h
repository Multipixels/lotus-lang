#pragma once

#include "ast.h"
#include "parser.h"

void checkParserErrors(parser::Parser* parser);

void testLiteralExpression(ast::Expression* expression, std::any expectedValue, int testNumber = -1);
void testIntegerLiteral(ast::Expression* expression, int expectedValue, int testNumber = -1);
void testFloatLiteral(ast::Expression* expression, float expectedValue, int testNumber = -1);
void testBooleanLiteral(ast::Expression* expression, bool expectedValue, int testNumber = -1);
void testCharacterLiteral(ast::Expression* expression, char expectedValue, int testNumber = -1);
void testCollectionLiteral(ast::Expression* expression, std::vector<std::any>* expectedValue, int testNumber = -1);
void testStringLiteral(ast::Expression* expression, std::string* expectedValue, int testNumber = -1);
void testIdentifier(ast::Expression* expression, std::string* expectedValue, int testNumber = -1);

void testInfixExpression(ast::Expression* expression, std::any leftValue, std::string op, std::any rightValue, int testNumber = -1);