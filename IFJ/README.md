# IFJ Project 2018/2019 by 076 team
Project goal is based on implementing compiler for **IFJ18** programming language.
IFJ18 is subset of **Ruby 2.0**, which is the imperative(object-oriented)
language, dynamically typed with functional elements.

## Running project
1. Cloning directory- in directory which you want to use for your project type:

        git clone https://github.com/Petku/ifj2018_076.git .

2. Build:

        make build

3. Running:

        make run
    or

        ./build/ifj18

## Makefile

| Make command | Function                                         |
|--------------|--------------------------------------------------|
|   make       | Builds everything including tests                |
| make build   | Only builds files needed for functional compiler |
| make run     | Command runs compiler                            |
| make tests   | Compiles and run all tests                       |
| make commit  | Cleans all .o files & stages all modified files  |
| make zip     | Prepares archive to be submitted to WIS          |
| make clean   | Removes all object files and final binary        |

## Programming-style
> Any advised coding style is located in programming_style.md file.
