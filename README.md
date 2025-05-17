[![Contributors][contributors-shield]][contributors-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<br />
<div align="center">
  <a href="https://github.com/Multipixels/lotus-lang" >
    <picture>
      <source media="(prefers-color-scheme: dark)" srcset="repo/white-lotus.svg" width=130px>
      <source media="(prefers-color-scheme: light)" srcset="repo/lotus.svg" width=130px>
      <img src="repo/lotus.svg">
  </picture>
  </a>
  <h3 align="center">Lotus</h3>
  <p align="center">
    A statically-typed procedural programming language with an interpreter built in C++.
    <br />
    <a href="https://multipixels.dev/projects/lotus-lang/">Try it out on the web!</a>
    ·
    <a href="https://github.com/Multipixels/Keep-Talking-and-Nobody-Explodes-Bot/issues">Report Bug</a>
    ·
    <a href="https://github.com/Multipixels/Keep-Talking-and-Nobody-Explodes-Bot/issues">Request Feature</a>
  </p>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#features">Features</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

## About The Project

Lotus is a programming language I developed to explore the principles of statically-typed procedural languages, test-driven development, and software testing. With Lotus, you can write programs using familiar constructs like collections, dictionaries, and control structures, all parsed and interpreted through an interpreter written in C++.

A web version of the Lotus interpreter can be found here: <https://multipixels.dev/projects/lotus-lang/>.

### Built With

* C++
* [CMake](https://cmake.org/)
* [Emscripten](https://emscripten.org/)
* [Google Test](https://google.github.io/googletest/)
* [Visual Studio](https://visualstudio.microsoft.com/)

## Getting Started

Follow these steps to set up and run the Lotus interpreter on your local machine.

### Prerequisites

Ensure you have the following installed:

* A C++ compiler
* CMake

### Setup/Installation

1. Clone the repository:
   ```sh
   git clone https://github.com/Multipixels/lotus-lang.git
   ```
2. Navigate to the project's root directory:
   ```sh
   cd lotus-lang
   ```
3. Generate a new directory `out` and navigate to it:
   ```sh
   mkdir out
   cd out
   ```
4. Run CMake:
   
   For just the Lotus interpreter, run
   ```sh
   cmake ..
   ```
   For the Lotus interpreter and test suites, run
   ```sh
   cmake .. CMAKE_BUILD_TYPE=Debug
   ```
   
5. Compile the project.
   
   i. (Windows) Open solution `out\LotusLang.sln` in Visual Studio and build.
   
   ii. (Mac/Linux) Run `make`

6. (Optional, Windows only) Compile the project for the web w/ Emscripten. Requires Emscripten installed and setup.
   ```sh
   mkdir out
   cd out
   mkdir build-wasm
   cd build-wasm
   
   call emcmake cmake ../..  -D DCMAKE_SYSTEM_NAME=Emscripten
   
   cmake --build . --target LotusLangWeb
   ```

## Usage

### CLI Interpreter

To open up the interpreter in your CLI, simply run the binary.

- (Windows):
```sh
.\LotusLang.exe
```

- (Mac/Linux):
```sh
./LotusLang
```

### Interpret Lotus Code from Files

Write Lotus programs in `.lotus` files and run them through the interpreter. Here's an example program:

```lotus
-> Find the sum of the values in a collection
integer sum = 0;
collection<integer> myCollection = [2, 4, 6];

iterate(value : myCollection) {
  sum += value;
}

log(sum);
```

Run the program

- Windows:
```sh
.\LotusLang.exe example.lotus
```
- Mac/Linux:
```sh
./LotusLang example.lotus
```

## Features

- **Statically-Typed Variables**: Includes primitive types like `boolean`, `integer`, `float`, `character`, and `string`.
- **Collections and Dictionaries**: Flexible and easy-to-use data structures.
- **Functions**: Define reusable blocks of code with return types and parameters.
- **Control Structures**: Use logic and loop structures, like `if-else`, `while`, `for`, and more.
- **Built-in Functions**: Log messages to the console, modify collection contents, and more.

## Contact

Richard Motorgeanu - richard.motorgeanu@gmail.com

Project Link: [https://github.com/Multipixels/lotus-lang](https://github.com/Multipixels/lotus-lang)

[contributors-shield]: https://img.shields.io/github/contributors/Multipixels/lotus-lang.svg?style=for-the-badge
[contributors-url]: https://github.com/Multipixels/lotus-lang/graphs/contributors
[stars-shield]: https://img.shields.io/github/stars/Multipixels/lotus-lang.svg?style=for-the-badge
[stars-url]: https://github.com/Multipixels/lotus-lang/stargazers
[issues-shield]: https://img.shields.io/github/issues/Multipixels/lotus-lang.svg?style=for-the-badge
[issues-url]: https://github.com/Multipixels/lotus-lang/issues
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/richard-motorgeanu/

