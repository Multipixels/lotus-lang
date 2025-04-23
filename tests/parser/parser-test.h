#pragma once

#include "ast.h"
#include "parser.h"

void checkParserErrors(parser::Parser* p_parser);

void testLiteralExpression(std::shared_ptr<ast::Expression> p_expression, std::any p_expectedValue, int p_testNumber = -1);
void testIntegerLiteral(std::shared_ptr<ast::Expression> p_expression, int p_expectedValue, int p_testNumber = -1);
void testFloatLiteral(std::shared_ptr<ast::Expression> p_expression, float p_expectedValue, int p_testNumber = -1);
void testBooleanLiteral(std::shared_ptr<ast::Expression> p_expression, bool p_expectedValue, int p_testNumber = -1);
void testCharacterLiteral(std::shared_ptr<ast::Expression> p_expression, char p_expectedValue, int p_testNumber = -1);
void testCollectionLiteral(std::shared_ptr<ast::Expression> p_expression, std::vector<std::any>* p_expectedValue, int p_testNumber = -1);
void testDictionaryLiteral(std::shared_ptr<ast::Expression> p_expression, std::map<std::string, std::any>* p_expectedValue, int p_testNumber = -1);
void testStringLiteral(std::shared_ptr<ast::Expression> p_expression, std::string* p_expectedValue, int p_testNumber = -1);
void testIdentifier(std::shared_ptr<ast::Expression> p_expression, std::string* p_expectedValue, int p_testNumber = -1);

void testInfixExpression(std::shared_ptr<ast::Expression> p_expression, std::any p_leftValue, std::string p_op, std::any p_rightValue, int p_testNumber = -1);