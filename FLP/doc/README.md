# Functional Project
## rv-2-rka

##### Author: Peter Horňák <xhorna14@stud.fit.vutbr.cz>

## Build
To build this project run command `make` that creates `rv-2-rka` binary.

## Run
Binary can be runned:
```bash
$ ./rv-2-rka (-t|-i) [FILE]
```
`FILE` is a path to file containing input. If not specified, the program
reads input from STDIN. 

Parameter `-t` transforms regular expression on input into finite state machine
using Thompson's construction. It prints the final automata onto STDOUT in
format defined in assignment. 

Parameter `-i` prints out on STDOUT regular expression representing that
was transformed from internal representation of input regular expression.

## Implementation
First the program reads the input, removes whitespaces and inserts '.' operator
that will be used internally. Then the regex is transformed into postfix notation
using Shunting-yard algorithm. 

After that, the regular expression is being transformed into data type representing
finite automata. This is being done by taking each character one by one and for each
type of input action defined in Thompson's algorithm is being performed. This approach
is very similar to evaluating arithmetic expression in postfix notation. 

For each alphabet character, new automata is created and pushed on stack.
Unary operator creates new automata is created from automata on top of the stack.
For binary operators two automatas are popped from stack and combined into new one.
Parenthesses are removed while doing transformation to postfix notation.

## Files
- `Main.hs` - The main program that controls the flow and performs input and output operations.
- `Helpers.hs` - Small functions that are used accros multiple modules.
- `Insert.hs` - Module containing functions that are used for inserting concatenation operator.
- `Shunting.hs` - Module implementing Shunting algorithm that converts regex into postfix notation.
- `Converter.hs` - Module containing data types and all functions converting regular expression into finite automata.
- `Infix.hs` - Module used to convert regex from internal representation back to original regex.
