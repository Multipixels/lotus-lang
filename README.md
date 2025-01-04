[![Contributors][contributors-shield]][contributors-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<br />
<div align="center">
  <h3 align="center">Lotus</h3>
  <p align="center">
    A statically-typed, procedural programming language with an interpreter built in C++.
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

### Built With

* C++
* [Google Test](https://google.github.io/googletest/)
* [Visual Studio](https://visualstudio.microsoft.com/)

## Getting Started

Follow these steps to set up and run the Lotus interpreter on your local machine.

### Prerequisites

Ensure you have the following installed:

* A C++ compiler
* [Visual Studio](https://visualstudio.microsoft.com/)

### Installation

1. Clone the repository:
   ```sh
   git clone https://github.com/Multipixels/lotus-lang.git
   ```
2. Navigate to the project's src directory:
   ```sh
   cd lotus-lang/src
   ```
3. Open the `lotus-lang.sln` file in Visual Studio.
4. Build and run the `lotus-interpreter-tests` tests.
5. Build the `lotus-interpreter` project.

## Usage

Write Lotus programs in `.lotus` files and run them through the interpreter. Here's an example program:

```lotus
-- Find the sum of the values in a collection
integer sum = 0;
collection<integer> myCollection = [2, 4, 6];

iterate(value : myCollection) {
  sum = sum + myCollection;
}

log(sum);
```

Run the program:
```sh
./lotus-interpreter example.lotus
```

## Features

- **Statically-Typed Variables**: Includes primitive types like `boolean`, `integer`, `float`, `character`, and `string`.
- **Collections and Dictionaries**: Flexible and easy-to-use data structures.
- **Functions**: Define reusable blocks of code with return types and parameters.
- **Control Structures**: Use `if-else`, `while`, `for`, and more.
- **Built-in Functions**: Log messages, calculate lengths, and more.

## Roadmap

- [X] Basic Syntax and Grammar
- [X] Functions
- [X] Control Structures
- [X] Collections
- [ ] Dictionaries
- [ ] Strings
- [ ] Error Reporting Improvements
- [ ] Garbage Collection
- [ ] Standard Library Functions

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

