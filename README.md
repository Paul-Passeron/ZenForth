# ZenForth

SImple stack-based Forth-like language implemented in C.
This porject is inspired by Tsoding's Porth: https://gitlab.com/tsoding/porth
And uses his string-view implementation: https://github.com/tsoding/sv

# Quickstart:
Compile the compiler using cc :
```console
cc -Wall -Wextra -pedantic src/zen_forth.c -o zf
```
Then compile files like so:
```console
./zf <input name> <output name>
```

Compiling and then running the program:
```console
./zf prog.zf output
```

It produces the following files:
- output.asm
- output.o
- output (which is the executable that you want to run)


# Syntax

```
# push an integer value on the stack (123 for instaance):
123

# push a boolean on the stack
true false

# print the value on top of the stack (it consumes it):
.

# arithmetic operations:
+ - * / %

# comparison operations:
> < =

# boolean logic:
not

# if control flow : a boolean value must be on top of the stack:

if
    <code here>
end

# while control flow : condition should only add a single bool on the stack 

while <condition> do
    <code here>
end

# declare an integer variable:
int <name_of_variable>

# push a pointer pointing to the variable on the stack:
<name_of_variable>

# push the value of the variable on the stack:
<name_of_variable> @

# store the value on top of the stack in the variable:
<name_of_variable> !
```


Example: Programs that prints the digit of a number in reverse order:
(prog.zf)
```
int number

123456 number !

while number @ 0 > do
    number @ 10 % .
    number @ 10 / number !
end
```
