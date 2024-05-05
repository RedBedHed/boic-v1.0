# Index
1. [Types](#types)
2. [Keywords](#keywords)
3. [Functions](#functions)
    1. [Main Function](#main-function)
    2. [Custom Functions](#custom-functions)
4. [Declarations](#declarations)
    1. [Variables](#variables)
    2. [Arrays and Strings](#arrays-and-strings)
5. [Expressions](#expressions)
    1. [Express Math](#express-math)
    2. [Express Function Calls](#express-function-calls)
    3. [Express Logic](#express-logic)
    4. [Express Array Access](#express-array-access)
6. [Control Flow](#control-flow)
    1. [Conditionals](#conditionals)
    2. [Loops](#loops)
    3. [Switch](#switch)
7. [Exit](#exit)
8. [Library Calls](#library-calls)
    1. [Print Integer](#print-integer)
    2. [Get Integer](#get-integer)
    3. [Print Character](#print-character)
    4. [Get Character](#get-character)
    5. [Print String Literal](#print-string-literal)
    6. [Get String Input](#get-string-input)
    7. [Print Stack String](#print-stack-string)
    8. [Print Static String](#print-static-string)
9. [Compiling](#compiling)
    1. [Building](#building)
    2. [Compiling Your First Boi Program](#compiling-your-first-boi-program)

# Types
| Type     | Size (in bits) | C equivalent   |
|----------|----------------|----------------|
| smolboi  | 8              | char           |
| boi      | 16             | short          |
| chonkboi | 32             | int            |
| gigaboi  | 64             | long long int  |

# Keywords
| Boi Keyword | Approximate C Equivalent |
|-------------|--------------------------|
| @datboi     | main                     |
| argybois    | none                     |
| ifboi       | if                       |
| elseboi     | else                     |
| elifboi     | else if                  |
| loopyboi    | while                    |
| switchyboi  | switch                   |
| caseyboi    | case                     |
| fallboi     | default                  |
| yeetboi     | return                   |
| bonkboi     | exit(0)                  |

# Functions
## Main Function

In Boi, the main function is named "datboi". it has the following signature. (Note: the argybois are not accessible in v 1.0, and the "yeetboi 0;" statement is optional).

```py
@datboi(argybois)
{
    yeetboi 0;
}
```

## Custom Functions

You can create any number of custom functions for your program. As in C, functions must be declared or defined before use. Functions may take any number of formal parameters as input. All functions return an ignorable 64-bit integer explicitly or implicitly. To return additional values, you may pass and mutate array parameters.

### Variable Parameters
```py
#=============================
# A function with two params.
#=============================
@foo(param1, param2)
{
    yeetboi param1 + param2;
}
```

### Array Parameters
```py
#============================
# A function with two params:
# an array of 32-bit integers
# and one 64-bit integer.
#============================
@foo(arr[chonkboi], param1)
{
    yeetboi arr[param1];
}
```

### Forward Declaration
```py
#============================
# Forward declaration.
#============================
@foo(param1, param2);

#============================
# Calling forward declared
# function.
#============================
@bar()
{
    yeetboi foo(0, 3);
}

#============================
# Definition of foward-
# declared function.
#============================
@foo(param1, param2)
{
    yeetboi param1 + param2;
}
```

### Recursion
```py
#============================
# Factorial.
#============================
@foo(n)
{
    ifboi(n < 1) 
    {
        yeetboi 1; 
    }
    yeetboi n * foo(n - 1); 
}
```

# Declarations

As in old C, declarations must not follow executable code. Declarations belong at the top of your function body.

## Variables

For simplicity, all variables are 64-bit types in stack memory. Here, a smolboi takes up the same amount of memory as a gigaboi. Variables may only be declared and referenced within the scope of a function body.

### Declaring Variables
```py
#=========================
# Declare
#=========================
smolboi a;
boi b;
chonkboi c;
gigaboi d;
```

### Declaring and Assigning
```py
#=========================
# Declare + Assign
#=========================
smolboi e = 1;
boi f = 2 * 5;
chonkboi g = f + e;
gigaboi h = g * f;
```

### Assigning Declared Variables
```py
#=========================
# Assign post-declaration.
#=========================
chonkboi i;
i = 42;
```

## Arrays and Strings

In contrast to variables, arrays are sized according to their type (see Types above). Here, a smolboi really does take up exactly 8 bits of stack memory. Arrays may only be declared and referenced within a function body. Static array initializers may contain integer literals only (no character literals).

### Declare a Stack Array
```py
boi stackArray[100];
```

### Declare a Static Array
```py
boi staticArray = { 1, 2, 3 };
```

### Declare a String (Character Array)
```py
smolboi string = "Hello World";
```

### Assign to an Index
```py
i[0] = 'h';
i[1] = 'e';
i[2] = 'l';
i[3] = 'l';
i[4] = 'o';
```

### Assign from an Index
```py
j = i[0];
```

# Expressions

Expressions are the cornerstone of Boilang and extremely versatile.

## Express Math

In Boilang, you may perform calculations with complex mathematical expressions.

```py
#=======================
# Pointless math UwU
#=======================
r = ((33 + p) - (33 * q)) / 5;
r = r << 1;
r = r ^ r;
```

## Express Function Calls

Functions are *almost* first-class citizens. Function calls are treated as expressions and may be nested within other expressions.

```py
#=======================
# Moar pointless math UwU
#=======================
r = foo(1) + bar(23);
r = 300 % foo(bar(1));
r = 23 + foo(bar(2 * (2 + 1) + 1) + 1) - 42;
```

## Express Logic

Boolean expressions are every bit as flexible as math expressions.

```py
#=======================
# Just bool things
#=======================
r = i < 23 && i > 5;
r = !(i < 23 && i > 5);
r = i >= 23 || i <= 5;
```

Boolean expressions may be nested within math expressions, allowing a Boi program to make decisions without branching.

```py
#=======================
# Branchless things
#=======================
r = r + (r > 0);
r = r & -(r > 0);
```

## Express Array Access

You can also perform calculations on elements at array indices.

```py
r = a[1] + a[i];
r = foo(a[i] * 4) - foo(a[i - 1] * 5);
r = a[a[1]] + a[a[a[0]]];
```

# Control Flow

## Conditionals

In Boilang, the "if" keyword is "ifboi", the "else if" keyword is "elifboi", and the "else" keyword is "elseboi".

### Branch
```py
ifboi(i & MASK)
{
    i = 42;
}
```

### Branch With Else
```py
ifboi(i & MASK)
{
    i = 42;
}
elseboi
{
    i = 5;
}
```

### Branch With Else If
```py
ifboi(i & MASK)
{
    i = 42;
}
elifboi(i & OTHER_MASK)
{
    i = 23;
}
elseboi
{
    i = 5;
}
```

## Loops

In Boilang, the "while" keyword is "loopyboi".

### Iterate
```py
#=======================
# Pointless loop
#=======================
i = 0;
loopyboi(i < 39)
{
    ++i;
}
```

### Nest Loops
```py
#=======================
# Pointless loop
#=======================
i = 0;
loopyboi(i < 39)
{
    j = 0;
    loopyboi(j < 39)
    {
        ++j;
    }
    ++i;
}
```

## Switch

In Boilang, the "switch" keyword is "switchyboi", the "case" keyword is "caseyboi", and the "default" keyword is "fallboi". The "break" keyword is implicit.

### Break on Any Case
```py
#=======================
# switch with normal
# break-ing cases.
#=======================
switchyboi(i)
{
caseyboi 1 { j = 3; }
caseyboi 2 { j = 2; }
caseyboi 3 { j = 1; }
}
```

### Use Default Case
```py
#=======================
# Switch with default
# case.
#=======================
switchyboi(i)
{
caseyboi 1 { j = 3; }
caseyboi 2 { j = 2; }
caseyboi 3 { j = 1; }
fallboi { j = 0; }
}
```

### Use Fallthrough
```py
#=======================
# Switch with fall-
# through
#=======================
switchyboi(i)
{
caseyboi 'a'
caseyboi 'e'
caseyboi 'i'
caseyboi 'o'
caseyboi 'u'
    {
        vowel = 1;
    }
fallboi
    {
        vowel = 0;
    }
}
```

# Exit

To immediately exit the program from any function, use the "bonkboi" keyword.

```py
bonkboi;
```

# Library Calls

Boilang has a small library of pseudo-functions for standard input and output.

## Print Integer

```py
#=======================
# Print i as an integer
#=======================
putBoi(i);
```

## Get Integer
```py
#=======================
# Get an integer and
# store it in variable
# i
#=======================
getBoi(i);
```

## Print Character
```py
#=======================
# Print i as a character
#=======================
putSmolBoi(i);
```

## Get Character
```py
#=======================
# Get a character and
# store it in variable
# i
#=======================
getSmolBoi(i);
```

## Print String Literal
```py
#=======================
# Print "Hello"
#=======================
putBoi("Hello");
```

## Get String Input
```py
#=======================
# Get a string and
# store it in character
# array i
#=======================
getSmolBois(i);
```

## Print Stack String
```py
#=======================
# Echo Echo
#=======================
smolboi i[50];
boi b;
getSmolBois(i, 50);

b = 0;
loopyboi( b < 50 && i[b] != 0 )
{
    putSmolBoi(i[b]);
    ++b;
}
```

## Print Static String
```py
#=======================
# Say Hello
#=======================
smolboi i = "Hello";
boi b = 0;
loopyboi( i[b] != 0 )
{
    putSmolBoi(i[b]);
    ++b;
}
```

# Compiling

The following steps assume that you've installed bash, gcc, and make.

## Building

To Build the compiler, navigate to the boic-v1.0 root directory in your bash shell and execute ```make```.

## Compiling Your First Boi Program

Once you've built the compiler, it's time to write and compile your first program in Boi.

1. Create a new file in the EXMP folder. Name it "hello.boi".
2. Open "hello.boi" in your favorite text editor and type:
```py
@datboi(argybois)
{
    putBoi("Hello World!");
}
```
3. Save and close the file.
4. Compile and run the file by executing ```./boic.sh EXMP/hello.boi``` in your bash shell.

<p align="center">
  <image src="../art/CatTranscendence.gif" width="350"></image>
</p>

LET'S GO!!! You are now an official Boi programmer, and that's totally *not* a worthless title. 
