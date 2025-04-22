#pragma once

#include "ast.h"
#include "parser.h"

void checkParserErrors(parser::Parser* parser);

void testLiteralExpression(std::shared_ptr<ast::Expression> expression, std::any expectedValue, int testNumber = -1);
void testIntegerLiteral(std::shared_ptr<ast::Expression> expression, int expectedValue, int testNumber = -1);
void testFloatLiteral(std::shared_ptr<ast::Expression> expression, float expectedValue, int testNumber = -1);
void testBooleanLiteral(std::shared_ptr<ast::Expression> expression, bool expectedValue, int testNumber = -1);
void testCharacterLiteral(std::shared_ptr<ast::Expression> expression, char expectedValue, int testNumber = -1);
void testCollectionLiteral(std::shared_ptr<ast::Expression> expression, std::vector<std::any>* expectedValue, int testNumber = -1);
void testDictionaryLiteral(std::shared_ptr<ast::Expression> expression, std::map<std::string, std::any>* expectedValue, int testNumber = -1);
void testStringLiteral(std::shared_ptr<ast::Expression> expression, std::string* expectedValue, int testNumber = -1);
void testIdentifier(std::shared_ptr<ast::Expression> expression, std::string* expectedValue, int testNumber = -1);

void testInfixExpression(std::shared_ptr<ast::Expression> expression, std::any leftValue, std::string op, std::any rightValue, int testNumber = -1);