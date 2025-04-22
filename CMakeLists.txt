# Made w/ help from Low Level Game Dev: https://www.youtube.com/watch?v=IBgfeZME2Vw

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(RELEASE_BUILD OFF)
else()
    set(RELEASE_BUILD ON)
endif()

# Embed the C++ static runtime library into program to ensure compatibility
# If file size is an issue, remove these lines of code
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

if(RELEASE_BUILD)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization
else()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()

project(
  "LotusLang"
  VERSION 1.0
  DESCRIPTION "Interpreter for Lotus"
  LANGUAGES CXX
)

# MY_SOURCES is a list of all .cpp files in the project
add_executable("LotusLang" 
    "src/main.cpp"
    "src/ast/ast.cpp"
    "src/evaluator/builtinFunctions.cpp"
    "src/evaluator/evaluator.cpp"
    "src/lexer/lexer.cpp"
    "src/object/object.cpp"
    "src/parser/parser.cpp"
    "src/repl/repl.cpp"
    "src/token/token.cpp"
)
set_property(TARGET "LotusLang" PROPERTY CXX_STANDARD 11)

if(RELEASE_BUILD)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RELEASE_BUILD=1) 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPMENT_BUILD=0) 
    message("Release Build")
else()
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RELEASE_BUILD=0) 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPMENT_BUILD=1) 
    message("Debug Build")
endif()

if(MSVC)
	target_compile_definitions("LotusLang" PUBLIC _CRT_SECURE_NO_WARNING)
endif()

target_include_directories(LotusLang PUBLIC 
    "src/ast"
    "src/evaluator"
    "src/lexer"
    "src/object"
    "src/parser"
    "src/repl"
    "src/token"
)

if(MSVC) # If using the VS compiler...
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

message("CMake Build Type: ${CMAKE_BUILD_TYPE}")



# TESTING

if(NOT ${RELEASE_BUILD})
    project(LotusTests)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(
        LotusTests
        "tests/ast/ast-test.cpp"
        "tests/evaluator/evaluator-test.cpp"
        "tests/lexer/lexer-test.cpp"
        "tests/parser/parser-test.cpp"
        "src/ast/ast.cpp"
        "src/evaluator/builtinFunctions.cpp"
        "src/evaluator/evaluator.cpp"
        "src/lexer/lexer.cpp"
        "src/object/object.cpp"
        "src/parser/parser.cpp"
        "src/repl/repl.cpp"
        "src/token/token.cpp"
    )

    target_link_libraries(
      LotusTests
      GTest::gtest_main
    )

	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RELEASE_BUILD=0) 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPMENT_BUILD=1) 
    
    target_include_directories(LotusTests PUBLIC 
        "src/ast"
        "src/evaluator"
        "src/lexer"
        "src/object"
        "src/parser"
        "src/repl"
        "src/token"
        "tests/ast"
        "tests/evaluator"
        "tests/lexer"
        "tests/parser"
    )
    
    include(GoogleTest)
    gtest_discover_tests(LotusTests)
endif()


## WEB BUILD

message(${CMAKE_SYSTEM_NAME})
if(${RELEASE_BUILD} AND CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    project("LotusLangWeb")
    add_executable("LotusLangWeb" 
        "src/bindings.cpp"
        "src/ast/ast.cpp"
        "src/evaluator/builtinFunctions.cpp"
        "src/evaluator/evaluator.cpp"
        "src/lexer/lexer.cpp"
        "src/object/object.cpp"
        "src/parser/parser.cpp"
        "src/repl/repl.cpp"
        "src/token/token.cpp"
    )

    target_include_directories(LotusLangWeb PUBLIC 
        "src/ast"
        "src/evaluator"
        "src/lexer"
        "src/object"
        "src/parser"
        "src/repl"
        "src/token"
    )

    set_target_properties(LotusLangWeb PROPERTIES 
        SUFFIX ".mjs"
        COMPILE_FLAGS "-Os"
        LINK_FLAGS "-lembind -Os -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS=['run']"
    )
endif()