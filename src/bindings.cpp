#include <chrono>
#include <iostream>
#include "emscripten/bind.h"
#include "repl.h"
#include "parser.h"
#include "evaluator.h"

using namespace emscripten;

void run(std::string p_input, int p_timeout = 0) {
    lexer::Lexer lexer = lexer::Lexer(&p_input);
    parser::Parser parser = parser::Parser(lexer);
    std::shared_ptr<ast::Program> program = parser.ParseProgram();
    std::shared_ptr<object::Environment> environment = std::make_shared<object::Environment>();

    for (int i = 0; i < parser.m_errors.size(); i++)
    {
        std::cout << "Parser error: " << parser.m_errors[i] << std::endl;
    }
    if (parser.m_errors.size() > 0) return;

    evaluator::g_timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(p_timeout);
    std::shared_ptr<object::Object> output = evaluator::evaluate(program, environment);

    // Output only if you get an error
    if (output->Type() == object::ERROR)
    {
        std::cout << output->Inspect() << std::endl;
    }
}

EMSCRIPTEN_BINDINGS(lotus) {
    function("run", &run);
}