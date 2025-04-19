#include <iostream>
#include "emscripten/bind.h"
#include "repl.h"
#include "parser.h"
#include "evaluator.h"

using namespace emscripten;

void run(std::string input) {
    lexer::Lexer lexer = lexer::Lexer(&input);
    parser::Parser parser = parser::Parser(lexer);
    ast::Program* program = parser.ParseProgram();
    object::Environment environment;

    for (int i = 0; i < parser.m_errors.size(); i++)
    {
        std::cout << "Parser error: " << parser.m_errors[i] << std::endl;
    }
    if (parser.m_errors.size() > 0);

    object::Object* output = evaluator::evaluate(program, &environment);
    if (output->Type() != object::NULL_TYPE)
    {
        std::cout << output->Inspect() << std::endl;
    }
}

EMSCRIPTEN_BINDINGS(lotus) {
    function("run", &run);
}