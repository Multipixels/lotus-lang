#include "pch.h"

#include <map>
#include <utility>

#include "lexer-test.h"
#include "lexer.h"

typedef std::pair<token::TokenType, std::string> TokenLiteralPair;

TEST(LexerTest, LexNextToken)
{
    std::string inputCode = 
    R"(
boolean someTruthValue = false;
boolean anotherTruthValue = true;
integer someInteger = 1;
float someFloat = 1f;
float anotherFloat = 2.5f;
character theLetterA = 'a';
collection<integer> myCollection = [2, 1, 6, 3, 8];
dictionary<integer, integer> myDictionary = {0: 1, 5: 3, 6: 2};
myCollection[2];
string myString = "hello";
collection<character> sameString = ['h', 'e', 'l', 'l', 'o'];

integer myFunction(integer a, boolean b) 
{
    integer c = 23;
    return c;
}

if(someTruthValue) 
{

}
else if(anotherTruthValue) 
{

} 
else 
{

}

while (anotherTruthValue)
{

}

do {

} while (someTruthValue);

for (integer i = 0; i < 10; i = i + 1) {

}

iterate(var : myCollection) {

}

log("Hello World!"); 
length(myCollection);

a == b;
a != b;
a < b; a > b; a <= b; a >= b;
!a;
a && b;
a || b;
)";

    TokenLiteralPair tests[] =
    {
        TokenLiteralPair(token::BOOLEAN_TYPE, "boolean"),
        TokenLiteralPair(token::IDENTIFIER, "someTruthValue"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::FALSE_LITERAL, "false"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::BOOLEAN_TYPE, "boolean"),
        TokenLiteralPair(token::IDENTIFIER, "anotherTruthValue"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::TRUE_LITERAL, "true"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::INTEGER_TYPE, "integer"),
        TokenLiteralPair(token::IDENTIFIER, "someInteger"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::INTEGER_LITERAL, "1"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::FLOAT_TYPE, "float"),
        TokenLiteralPair(token::IDENTIFIER, "someFloat"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::FLOAT_LITERAL, "1f"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::FLOAT_TYPE, "float"),
        TokenLiteralPair(token::IDENTIFIER, "anotherFloat"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::FLOAT_LITERAL, "2.5f"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::CHARACTER_TYPE, "character"),
        TokenLiteralPair(token::IDENTIFIER, "theLetterA"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::CHARACTER_LITERAL, "a"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::COLLECTION_TYPE, "collection"),
        TokenLiteralPair(token::LCHEVRON, "<"),
        TokenLiteralPair(token::INTEGER_TYPE, "integer"),
        TokenLiteralPair(token::RCHEVRON, ">"),
        TokenLiteralPair(token::IDENTIFIER, "myCollection"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::LBRACKET, "["),
        TokenLiteralPair(token::INTEGER_LITERAL, "2"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::INTEGER_LITERAL, "1"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::INTEGER_LITERAL, "6"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::INTEGER_LITERAL, "3"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::INTEGER_LITERAL, "8"),
        TokenLiteralPair(token::RBRACKET, "]"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::DICTIONARY_TYPE, "dictionary"),
        TokenLiteralPair(token::LCHEVRON, "<"),
        TokenLiteralPair(token::INTEGER_TYPE, "integer"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::INTEGER_TYPE, "integer"),
        TokenLiteralPair(token::RCHEVRON, ">"),
        TokenLiteralPair(token::IDENTIFIER, "myDictionary"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::LBRACE, "{"),
        TokenLiteralPair(token::INTEGER_LITERAL, "0"),
        TokenLiteralPair(token::COLON, ":"),
        TokenLiteralPair(token::INTEGER_LITERAL, "1"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::INTEGER_LITERAL, "5"),
        TokenLiteralPair(token::COLON, ":"),
        TokenLiteralPair(token::INTEGER_LITERAL, "3"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::INTEGER_LITERAL, "6"),
        TokenLiteralPair(token::COLON, ":"),
        TokenLiteralPair(token::INTEGER_LITERAL, "2"),
        TokenLiteralPair(token::RBRACE, "}"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "myCollection"),
        TokenLiteralPair(token::LBRACKET, "["),
        TokenLiteralPair(token::INTEGER_LITERAL, "2"),
        TokenLiteralPair(token::RBRACKET, "]"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::STRING_TYPE, "string"),
        TokenLiteralPair(token::IDENTIFIER, "myString"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::STRING_LITERAL, "hello"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::COLLECTION_TYPE, "collection"),
        TokenLiteralPair(token::LCHEVRON, "<"),
        TokenLiteralPair(token::CHARACTER_TYPE, "character"),
        TokenLiteralPair(token::RCHEVRON, ">"),
        TokenLiteralPair(token::IDENTIFIER, "sameString"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::LBRACKET, "["),
        TokenLiteralPair(token::CHARACTER_LITERAL, "h"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::CHARACTER_LITERAL, "e"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::CHARACTER_LITERAL, "l"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::CHARACTER_LITERAL, "l"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::CHARACTER_LITERAL, "o"),
        TokenLiteralPair(token::RBRACKET, "]"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::INTEGER_TYPE, "integer"),
        TokenLiteralPair(token::IDENTIFIER, "myFunction"),
        TokenLiteralPair(token::LPARENTHESIS, "("),
        TokenLiteralPair(token::INTEGER_TYPE, "integer"),
        TokenLiteralPair(token::IDENTIFIER, "a"),
        TokenLiteralPair(token::COMMA, ","),
        TokenLiteralPair(token::BOOLEAN_TYPE, "boolean"),
        TokenLiteralPair(token::IDENTIFIER, "b"),
        TokenLiteralPair(token::RPARENTHESIS, ")"),
        TokenLiteralPair(token::LBRACE, "{"),
        TokenLiteralPair(token::INTEGER_TYPE, "integer"),
        TokenLiteralPair(token::IDENTIFIER, "c"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::INTEGER_LITERAL, "23"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::RETURN, "return"),
        TokenLiteralPair(token::IDENTIFIER, "c"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::RBRACE, "}"),
        TokenLiteralPair(token::IF, "if"),
        TokenLiteralPair(token::LPARENTHESIS, "("),
        TokenLiteralPair(token::IDENTIFIER, "someTruthValue"),
        TokenLiteralPair(token::RPARENTHESIS, ")"),
        TokenLiteralPair(token::LBRACE, "{"),
        TokenLiteralPair(token::RBRACE, "}"),
        TokenLiteralPair(token::ELSE, "else"),
        TokenLiteralPair(token::IF, "if"),
        TokenLiteralPair(token::LPARENTHESIS, "("),
        TokenLiteralPair(token::IDENTIFIER, "anotherTruthValue"),
        TokenLiteralPair(token::RPARENTHESIS, ")"),
        TokenLiteralPair(token::LBRACE, "{"),
        TokenLiteralPair(token::RBRACE, "}"),
        TokenLiteralPair(token::ELSE, "else"),
        TokenLiteralPair(token::LBRACE, "{"),
        TokenLiteralPair(token::RBRACE, "}"),
        TokenLiteralPair(token::WHILE, "while"),
        TokenLiteralPair(token::LPARENTHESIS, "("),
        TokenLiteralPair(token::IDENTIFIER, "anotherTruthValue"),
        TokenLiteralPair(token::RPARENTHESIS, ")"),
        TokenLiteralPair(token::LBRACE, "{"),
        TokenLiteralPair(token::RBRACE, "}"),
        TokenLiteralPair(token::DO, "do"),
        TokenLiteralPair(token::LBRACE, "{"),
        TokenLiteralPair(token::RBRACE, "}"),
        TokenLiteralPair(token::WHILE, "while"),
        TokenLiteralPair(token::LPARENTHESIS, "("),
        TokenLiteralPair(token::IDENTIFIER, "someTruthValue"),
        TokenLiteralPair(token::RPARENTHESIS, ")"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::FOR, "for"),
        TokenLiteralPair(token::LPARENTHESIS, "("),
        TokenLiteralPair(token::INTEGER_TYPE, "integer"),
        TokenLiteralPair(token::IDENTIFIER, "i"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::INTEGER_LITERAL, "0"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "i"),
        TokenLiteralPair(token::LCHEVRON, "<"),
        TokenLiteralPair(token::INTEGER_LITERAL, "10"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "i"),
        TokenLiteralPair(token::ASSIGN, "="),
        TokenLiteralPair(token::IDENTIFIER, "i"),
        TokenLiteralPair(token::PLUS, "+"),
        TokenLiteralPair(token::INTEGER_LITERAL, "1"),
        TokenLiteralPair(token::RPARENTHESIS, ")"),
        TokenLiteralPair(token::LBRACE, "{"),
        TokenLiteralPair(token::RBRACE, "}"),
        TokenLiteralPair(token::ITERATE, "iterate"),
        TokenLiteralPair(token::LPARENTHESIS, "("),
        TokenLiteralPair(token::IDENTIFIER, "var"),
        TokenLiteralPair(token::COLON, ":"),
        TokenLiteralPair(token::IDENTIFIER, "myCollection"),
        TokenLiteralPair(token::RPARENTHESIS, ")"),
        TokenLiteralPair(token::LBRACE, "{"),
        TokenLiteralPair(token::RBRACE, "}"),
        TokenLiteralPair(token::IDENTIFIER, "log"),
        TokenLiteralPair(token::LPARENTHESIS, "("),
        TokenLiteralPair(token::STRING_LITERAL, "Hello World!"),
        TokenLiteralPair(token::RPARENTHESIS, ")"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "length"),
        TokenLiteralPair(token::LPARENTHESIS, "("),
        TokenLiteralPair(token::IDENTIFIER, "myCollection"),
        TokenLiteralPair(token::RPARENTHESIS, ")"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "a"),
        TokenLiteralPair(token::EQ, "=="),
        TokenLiteralPair(token::IDENTIFIER, "b"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "a"),
        TokenLiteralPair(token::NEQ, "!="),
        TokenLiteralPair(token::IDENTIFIER, "b"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "a"),
        TokenLiteralPair(token::LCHEVRON, "<"),
        TokenLiteralPair(token::IDENTIFIER, "b"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "a"),
        TokenLiteralPair(token::RCHEVRON, ">"),
        TokenLiteralPair(token::IDENTIFIER, "b"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "a"),
        TokenLiteralPair(token::LEQ, "<="),
        TokenLiteralPair(token::IDENTIFIER, "b"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "a"),
        TokenLiteralPair(token::GEQ, ">="),
        TokenLiteralPair(token::IDENTIFIER, "b"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::BANG, "!"),
        TokenLiteralPair(token::IDENTIFIER, "a"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "a"),
        TokenLiteralPair(token::AND, "&&"),
        TokenLiteralPair(token::IDENTIFIER, "b"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::IDENTIFIER, "a"),
        TokenLiteralPair(token::OR, "||"),
        TokenLiteralPair(token::IDENTIFIER, "b"),
        TokenLiteralPair(token::SEMICOLON, ";"),
        TokenLiteralPair(token::END_OF_FILE, ""),
    };

    lexer::Lexer lexer(&inputCode);

    for (int i = 0; i < sizeof(tests) / sizeof(TokenLiteralPair); i++)
    {
        token::Token token = lexer.nextToken();

        EXPECT_EQ(token::tokenTypeToString.at(token.m_type), token::tokenTypeToString.at(tests[i].first))
            << "Test #" << i << '\n';
        EXPECT_EQ(token.m_literal, tests[i].second)
            << "Test #" << i << '\n';
    }
}