@echo off
mkdir out
cd out
mkdir build-wasm
cd build-wasm

call emcmake cmake ../..  -D DCMAKE_SYSTEM_NAME=Emscripten

cmake --build . --target LotusLangWeb

pause