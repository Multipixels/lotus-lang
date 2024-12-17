#pragma once

#include "ast.h"


void testLiteralExpression(ast::Expression* expression, std::any expectedValue, int testNumber = -1);
void testIntegerLiteral(ast::Expression* expression, int expectedValue, int testNumber = -1);
void testFloatLiteral(ast::Expression* expression, float expectedValue, int testNumber = -1);
void testBooleanLiteral(ast::Expression* expression, bool expectedValue, int testNumber = -1);
void testCharacterLiteral(ast::Expression* expression, char expectedValue, int testNumber = -1);