# Made w/ help from Low Level Game Dev: https://www.youtube.com/watch?v=IBgfeZME2Vw

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)

if(${CMAKE_BUILD_TYPE} EQUAL "DEBUG_BUILD")
set(RELEASE_BUILD ON)
else()
set(RELEASE_BUILD OFF)
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

message("${CMAKE_BUILD_TYPE}")