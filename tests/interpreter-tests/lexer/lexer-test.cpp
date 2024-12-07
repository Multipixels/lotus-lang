#include "pch.h"

#include <map>
#include <utility>

#include "lexer-test.h"
#include "lexer.h"

typedef std::pair<token::TokenType, std::string> TokenLiteralPair;

TEST(LexerTest, LexNextToken)
{
    std::string inputCode = 
    R"(+=-;
+=-;
)";

    TokenLiteralPair tests[] =
    {
        TokenLiteralPair(token::PLUS, "+"),
        TokenLiteralPair(token::EQ, "="),
        TokenLiteralPair(token::MINUS, "-"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::PLUS, "+"),
        TokenLiteralPair(token::EQ, "="),
        TokenLiteralPair(token::MINUS, "-"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::END_OF_FILE, ""),
    };

    lexer::Lexer lexer(&inputCode);

    for (int i = 0; i < sizeof(tests) / sizeof(TokenLiteralPair); i++)
    {
        token::Token token = lexer.nextToken();

        EXPECT_EQ(token::tokenTypeToString.at(token.m_type), token::tokenTypeToString.at(tests[i].first));
        EXPECT_EQ(token.m_literal, tests[i].second);
    }
}