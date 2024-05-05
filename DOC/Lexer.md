# SNAIL (SNAIL's Not An Ideal Lexer)

A toy lexer.

Snail takes a lexical specification file and source file as input, and outputs a brief lexical analysis. Under the hood, snail parses your lexical specification and represents it as a list of non-deterministic finite automata via Thompson's construction. It simulates each of these finite automata as it passes over the source file, selecting the longest-possible match as it goes.

The current Snail is just a toy lexer. It isn't nearly as fast as a table-driven compiled lexer.

## What meta-characters does Snail support?

<table>
  <tr>
    <th>Operator</th>
    <th>Name</th>
    <th>Effect</th>
  </tr>
  <tr>
    <td>(implicit)</td>
    <td>Concatenation</td>
    <td>Matches a lexeme where rhs comes immediately after lhs.</td>
  </tr>
  <tr>
    <td>|</td>
    <td>Union</td>
    <td>Matches lhs or rhs.</td>
  </tr>
  <tr>
    <td>*</td>
    <td>Star</td>
    <td>Matches zero or more of lhs.</td>
  </tr>
  <tr>
    <td>+</td>
    <td>Plus</td>
    <td>Matches one or more of lhs.</td>
  </tr>
  <tr>
    <td>?</td>
    <td>Zero-Or-One</td>
    <td>Matches zero or one of lhs.</td>
  </tr>
  <tr>
    <td>.</td>
    <td>Dot</td>
    <td>Matches any ASCII char.</td>       
  </tr>
  <tr>
    <td>[]</td>
    <td>Brackets</td>
    <td>Matches the union of each individual character or escape character within.</td>       
  </tr>
  <tr>
    <td>-</td>
    <td>Range</td>
    <td>Must be used within brackets. Matches the union of the range between lhs and rhs inclusive.</td>       
  </tr>
  <tr>
    <td>()</td>
    <td>Parenthesis</td>
    <td>Allows for the grouping of expressions to control precedence.</td>       
  </tr>
  <tr>
    <td>\</td>
    <td>Escape</td>
    <td>Allows you to match control characters and meta-characters.</td>       
  </tr>
  <tr>
    <td>#</td>
    <td>Hash</td>
    <td>Allows you to assign a name to your token.</td>       
  </tr>
  <tr>
    <td>,</td>
    <td>Comma</td>
    <td>Allows you to chain token specifications.</td>       
  </tr>
</table>

## How do I write a lexical specification?

A lexical spec consists of comma separated Token specifications of the form 
\<REGEX\> # \<TOKEN\> 

### *Simple Expressions*       
To create a Token for the integer 1:
 
`1 # "ONE"`

### *Concatenation*
To create a Token for the word "supercalifragilisticexpialodocious":

`supercalifragilisticexpialodocious # "SUPERCAL"`

### *Union*
To create a Token for 1 or 0:

`1|0 # "BOOL"`

Or, to create a Token for any vowel:

`[aeiou] # "VOWEL"`

Or, to create a Token for any digit:

`[0-9] # "DIGIT"`

### *Kleene Star*
To create a Token for binary strings starting with 1:

`1(1|0)* # "BINARY_STRING"`

### *Kleene Plus*
To create a Token for all non-negative integers:

`[0-9]+ # "INTEGER"`

### *Zero-Or-One*
To create a Token for ">==", "<==" or "==":

`(>|<)?== # "STRANGE_COMPARISON"`
 
### *ASCII*
To create a Token for any sequence of ASCII characters:

`.+ # "ASCII"`

## What is an NFA?

An NFA is a Non-Deterministic Finite Automaton. In Thompson's construction, NFAs are composed recursively without simplification. Large NFAs are built from smaller NFAs using the following constructions:

1) Trivial - C

![](/NFA/_1.png)

2) Concatenation - CD

![](/NFA/_2.png)

3) Union - C|D

![](/NFA/_3.png)

4) Star - C*

![](/NFA/_4.png)

5) Plus - C+

![](/NFA/_5.png)

6) Zero-Or-One - C?

![](/NFA/_6.png)

## Example Lexical Specification (Input)

```
auto     # "AUTO",
inline   # "inline",
const    # "CONST",
\.       # "DOT",
,        # "COMMA",
\'       # "SINGLE_QUOTE",
\"       # "DOUBLE_QUOTE",
break    # "BREAK",
case     # "CASE",
char     # "CHAR",
const    # "CONST",
continue # "CONTINUE",
default  # "DEFAULT",
do       # "DO",
double   # "DOUBLE",
else     # "ELSE",
enum     # "ENUM",
extern   # "EXTERN",
float    # "FLOAT",
for      # "FOR",
goto     # "GOTO",
if       # "IF",
inline   # "INLINE",
int      # "INT",
long     # "LONG",
register # "REGISTER",
restrict # "RESTRICT",
return   # "RETURN",
short    # "SHORT",
signed   # "SIGNED",
sizeof   # "SIZEOF",
static   # "STATIC",
struct   # "STRUCT",
switch   # "SWITCH",
typedef  # "TYPEDEF",
union    # "UNION",
unsigned # "UNSIGNED",
void     # "VOID",
volatile # "VOLATILE",
while    # "WHILE",
\#       # "HASH",
elif     # "ELIF",
endif    # "ENDIF",
ifdef    # "IFDEF",
ifndef   # "IFNDEF",
define   # "DEFINE",
undef    # "UNDEF",
include  # "INCLUDE",
line     # "LINE",
error    # "ERROR",
pragma   # "PRAGMA",
==       # "EQUALS",
!=       # "NOT_EQUAL",
>        # "GREATER_THAN",
<        # "LESS_THAN",
>=       # "LT_EQUAL",
<=       # "GT_EQUAL",
&&       # "LOGIC_AND",
\|\|     # "LOGIC_OR",
!        # "LOGIC_NOT",
&        # "BIT_AND",
\|       # "BIT_OR",
^        # "BIT_XOR",
~        # "ONES_COMP",
<<       # "LEFT_SHIFT",
>>       # "RIGHT_SHIFT",
=        # "ASSIGNMENT",
\+=      # "PLUS_ASSIGN",
-=       # "MINUS_ASSIGN",
/=       # "DIVIDE_ASSIGN",
%=       # "MOD_ASSIGN",
<<=      # "LS_ASSIGN",
>>=      # "RS_ASSIGN",
&        # "AND_ASSIGN",
^=       # "XOR_ASSIGN",
\|=      # "OR_ASSIGN",
sizeof   # "SIZE_OF",
\?       # "QUESTION",
:        # "COLON",
\(       # "LPAREN",
\)       # "RPAREN",
\[       # "LBRACKET",
\]       # "RBRACKET",
{        # "LCURLY",
}        # "RCURLY",
\+\+     # "PLUSPLUS",
--       # "MINUSMINUS",
\+       # "PLUS",
-        # "MINUS",
\*       # "TIMES",
/        # "DIVIDE",
%        # "MOD",
"([\s!#-\[\[-~]|\\.)*"     
         # "STRING_LITERAL",
'\\?[aftnrb0v'"?\\]'|'.'
         # "CHAR_LITERAL",
;        # "SEMICOLON",
[_a-zA-Z][_a-zA-Z0-9]*\.h
         # "HEADER",
[_a-zA-Z][_a-zA-Z0-9]*
         # "ID",
[1-9][0-9]*([uU](ll?|LL?)?|(ll?|LL?)[uU]?)?
         # "INT",
0[bB][01]+([uU](ll?|LL?)?|(ll?|LL?)[uU]?)?
         # "BINARY",
0[0-7]*([uU](ll?|LL?)?|(ll?|LL?)[uU]?)?
         # "OCTAL",
0[xX][0-9a-fA-F]+([uU](ll?|LL?)?|(ll?|LL?)[uU]?)?
         # "HEX",
((([0-9]*\.[0-9]+)|([0-9]+\.))([eE][+-]?[0-9]+)?|[0-9]+[eE][+-]?[0-9]+)[flFL]?
         # "DECIMAL_FLOAT"
```

## Example source file (from Programiz.com)

```c
#include <stdio.h>

void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int partition(int array[], int low, int high) {
  int pivot = array[high];


  int i = (low - 1);

  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {

      i++;

      swap(&array[i], &array[j]);
    }
  }


  swap(&array[i + 1], &array[high]);

  return (i + 1);
}

void quickSort(int array[], int low, int high) {
  if (low < high) {

    int pi = partition(array, low, high);

    quickSort(array, low, pi - 1);
    quickSort(array, pi + 1, high);
  }
}

void printArray(int array[], int size) {
  for (int i = 0; i < size; ++i) {
      printf("%d  ", array[i]);
  }
  printf("\n");
}


int main() {
  int data[] = {8, 7, 2, 1, 0, 9, 6};

  int n = sizeof(data) / sizeof(data[0]);

  printf("Unsorted Array\n");
  printArray(data, n);
  quickSort(data, 0, n - 1);

  printf("Sorted array in ascending order: \n");
  printArray(data, n);
}
```

## Example Token List (Output)

```
          HASH, line 0 , "#"
       INCLUDE, line 0 , "include"
     LESS_THAN, line 0 , "<"
        HEADER, line 0 , "stdio.h"
  GREATER_THAN, line 0 , ">"
          VOID, line 3 , "void"
            ID, line 3 , "swap"
        LPAREN, line 3 , "("
           INT, line 3 , "int"
         TIMES, line 3 , "*"
            ID, line 3 , "a"
         COMMA, line 3 , ","
           INT, line 3 , "int"
         TIMES, line 3 , "*"
            ID, line 3 , "b"
        RPAREN, line 3 , ")"
        LCURLY, line 3 , "{"
           INT, line 4 , "int"
            ID, line 4 , "t"
    ASSIGNMENT, line 4 , "="
         TIMES, line 4 , "*"
            ID, line 4 , "a"
     SEMICOLON, line 4 , ";"
         TIMES, line 5 , "*"
            ID, line 5 , "a"
    ASSIGNMENT, line 5 , "="
         TIMES, line 5 , "*"
            ID, line 5 , "b"
     SEMICOLON, line 5 , ";"
         TIMES, line 6 , "*"
            ID, line 6 , "b"
    ASSIGNMENT, line 6 , "="
            ID, line 6 , "t"
     SEMICOLON, line 6 , ";"
        RCURLY, line 7 , "}"
           INT, line 9 , "int"
            ID, line 9 , "partition"
        LPAREN, line 9 , "("
           INT, line 9 , "int"
            ID, line 9 , "array"
      LBRACKET, line 9 , "["
      RBRACKET, line 9 , "]"
         COMMA, line 9 , ","
           INT, line 9 , "int"
            ID, line 9 , "low"
         COMMA, line 9 , ","
           INT, line 9 , "int"
            ID, line 9 , "high"
        RPAREN, line 9 , ")"
        LCURLY, line 9 , "{"
           INT, line 10 , "int"
            ID, line 10 , "pivot"
    ASSIGNMENT, line 10 , "="
            ID, line 10 , "array"
      LBRACKET, line 10 , "["
            ID, line 10 , "high"
      RBRACKET, line 10 , "]"
     SEMICOLON, line 10 , ";"
           INT, line 13 , "int"
            ID, line 13 , "i"
    ASSIGNMENT, line 13 , "="
        LPAREN, line 13 , "("
            ID, line 13 , "low"
         MINUS, line 13 , "-"
           INT, line 13 , "1"
        RPAREN, line 13 , ")"
     SEMICOLON, line 13 , ";"
           FOR, line 15 , "for"
        LPAREN, line 15 , "("
           INT, line 15 , "int"
            ID, line 15 , "j"
    ASSIGNMENT, line 15 , "="
            ID, line 15 , "low"
     SEMICOLON, line 15 , ";"
            ID, line 15 , "j"
     LESS_THAN, line 15 , "<"
            ID, line 15 , "high"
     SEMICOLON, line 15 , ";"
            ID, line 15 , "j"
      PLUSPLUS, line 15 , "++"
        RPAREN, line 15 , ")"
        LCURLY, line 15 , "{"
            IF, line 16 , "if"
        LPAREN, line 16 , "("
            ID, line 16 , "array"
      LBRACKET, line 16 , "["
            ID, line 16 , "j"
      RBRACKET, line 16 , "]"
      GT_EQUAL, line 16 , "<="
            ID, line 16 , "pivot"
        RPAREN, line 16 , ")"
        LCURLY, line 16 , "{"
            ID, line 18 , "i"
      PLUSPLUS, line 18 , "++"
     SEMICOLON, line 18 , ";"
            ID, line 20 , "swap"
        LPAREN, line 20 , "("
       BIT_AND, line 20 , "&"
            ID, line 20 , "array"
      LBRACKET, line 20 , "["
            ID, line 20 , "i"
      RBRACKET, line 20 , "]"
         COMMA, line 20 , ","
       BIT_AND, line 20 , "&"
            ID, line 20 , "array"
      LBRACKET, line 20 , "["
            ID, line 20 , "j"
      RBRACKET, line 20 , "]"
        RPAREN, line 20 , ")"
     SEMICOLON, line 20 , ";"
        RCURLY, line 21 , "}"
        RCURLY, line 22 , "}"
            ID, line 25 , "swap"
        LPAREN, line 25 , "("
       BIT_AND, line 25 , "&"
            ID, line 25 , "array"
      LBRACKET, line 25 , "["
            ID, line 25 , "i"
          PLUS, line 25 , "+"
           INT, line 25 , "1"
      RBRACKET, line 25 , "]"
         COMMA, line 25 , ","
       BIT_AND, line 25 , "&"
            ID, line 25 , "array"
      LBRACKET, line 25 , "["
            ID, line 25 , "high"
      RBRACKET, line 25 , "]"
        RPAREN, line 25 , ")"
     SEMICOLON, line 25 , ";"
        RETURN, line 27 , "return"
        LPAREN, line 27 , "("
            ID, line 27 , "i"
          PLUS, line 27 , "+"
           INT, line 27 , "1"
        RPAREN, line 27 , ")"
     SEMICOLON, line 27 , ";"
        RCURLY, line 28 , "}"
          VOID, line 30 , "void"
            ID, line 30 , "quickSort"
        LPAREN, line 30 , "("
           INT, line 30 , "int"
            ID, line 30 , "array"
      LBRACKET, line 30 , "["
      RBRACKET, line 30 , "]"
         COMMA, line 30 , ","
           INT, line 30 , "int"
            ID, line 30 , "low"
         COMMA, line 30 , ","
           INT, line 30 , "int"
            ID, line 30 , "high"
        RPAREN, line 30 , ")"
        LCURLY, line 30 , "{"
            IF, line 31 , "if"
        LPAREN, line 31 , "("
            ID, line 31 , "low"
     LESS_THAN, line 31 , "<"
            ID, line 31 , "high"
        RPAREN, line 31 , ")"
        LCURLY, line 31 , "{"
           INT, line 33 , "int"
            ID, line 33 , "pi"
    ASSIGNMENT, line 33 , "="
            ID, line 33 , "partition"
        LPAREN, line 33 , "("
            ID, line 33 , "array"
         COMMA, line 33 , ","
            ID, line 33 , "low"
         COMMA, line 33 , ","
            ID, line 33 , "high"
        RPAREN, line 33 , ")"
     SEMICOLON, line 33 , ";"
            ID, line 35 , "quickSort"
        LPAREN, line 35 , "("
            ID, line 35 , "array"
         COMMA, line 35 , ","
            ID, line 35 , "low"
         COMMA, line 35 , ","
            ID, line 35 , "pi"
         MINUS, line 35 , "-"
           INT, line 35 , "1"
        RPAREN, line 35 , ")"
     SEMICOLON, line 35 , ";"
            ID, line 36 , "quickSort"
        LPAREN, line 36 , "("
            ID, line 36 , "array"
         COMMA, line 36 , ","
            ID, line 36 , "pi"
          PLUS, line 36 , "+"
           INT, line 36 , "1"
         COMMA, line 36 , ","
            ID, line 36 , "high"
        RPAREN, line 36 , ")"
     SEMICOLON, line 36 , ";"
        RCURLY, line 37 , "}"
        RCURLY, line 38 , "}"
          VOID, line 40 , "void"
            ID, line 40 , "printArray"
        LPAREN, line 40 , "("
           INT, line 40 , "int"
            ID, line 40 , "array"
      LBRACKET, line 40 , "["
      RBRACKET, line 40 , "]"
         COMMA, line 40 , ","
           INT, line 40 , "int"
            ID, line 40 , "size"
        RPAREN, line 40 , ")"
        LCURLY, line 40 , "{"
           FOR, line 41 , "for"
        LPAREN, line 41 , "("
           INT, line 41 , "int"
            ID, line 41 , "i"
    ASSIGNMENT, line 41 , "="
         OCTAL, line 41 , "0"
     SEMICOLON, line 41 , ";"
            ID, line 41 , "i"
     LESS_THAN, line 41 , "<"
            ID, line 41 , "size"
     SEMICOLON, line 41 , ";"
      PLUSPLUS, line 41 , "++"
            ID, line 41 , "i"
        RPAREN, line 41 , ")"
        LCURLY, line 41 , "{"
            ID, line 42 , "printf"
        LPAREN, line 42 , "("
STRING_LITERAL, line 42 , ""%d  ""
         COMMA, line 42 , ","
            ID, line 42 , "array"
      LBRACKET, line 42 , "["
            ID, line 42 , "i"
      RBRACKET, line 42 , "]"
        RPAREN, line 42 , ")"
     SEMICOLON, line 42 , ";"
        RCURLY, line 43 , "}"
            ID, line 44 , "printf"
        LPAREN, line 44 , "("
STRING_LITERAL, line 44 , ""\n""
        RPAREN, line 44 , ")"
     SEMICOLON, line 44 , ";"
        RCURLY, line 45 , "}"
           INT, line 48 , "int"
            ID, line 48 , "main"
        LPAREN, line 48 , "("
        RPAREN, line 48 , ")"
        LCURLY, line 48 , "{"
           INT, line 49 , "int"
            ID, line 49 , "data"
      LBRACKET, line 49 , "["
      RBRACKET, line 49 , "]"
    ASSIGNMENT, line 49 , "="
        LCURLY, line 49 , "{"
           INT, line 49 , "8"
         COMMA, line 49 , ","
           INT, line 49 , "7"
         COMMA, line 49 , ","
           INT, line 49 , "2"
         COMMA, line 49 , ","
           INT, line 49 , "1"
         COMMA, line 49 , ","
         OCTAL, line 49 , "0"
         COMMA, line 49 , ","
           INT, line 49 , "9"
         COMMA, line 49 , ","
           INT, line 49 , "6"
        RCURLY, line 49 , "}"
     SEMICOLON, line 49 , ";"
           INT, line 51 , "int"
            ID, line 51 , "n"
    ASSIGNMENT, line 51 , "="
        SIZEOF, line 51 , "sizeof"
        LPAREN, line 51 , "("
            ID, line 51 , "data"
        RPAREN, line 51 , ")"
        DIVIDE, line 51 , "/"
        SIZEOF, line 51 , "sizeof"
        LPAREN, line 51 , "("
            ID, line 51 , "data"
      LBRACKET, line 51 , "["
         OCTAL, line 51 , "0"
      RBRACKET, line 51 , "]"
        RPAREN, line 51 , ")"
     SEMICOLON, line 51 , ";"
            ID, line 53 , "printf"
        LPAREN, line 53 , "("
STRING_LITERAL, line 53 , ""Unsorted Array\n""
        RPAREN, line 53 , ")"
     SEMICOLON, line 53 , ";"
            ID, line 54 , "printArray"
        LPAREN, line 54 , "("
            ID, line 54 , "data"
         COMMA, line 54 , ","
            ID, line 54 , "n"
        RPAREN, line 54 , ")"
     SEMICOLON, line 54 , ";"
            ID, line 55 , "quickSort"
        LPAREN, line 55 , "("
            ID, line 55 , "data"
         COMMA, line 55 , ","
         OCTAL, line 55 , "0"
         COMMA, line 55 , ","
            ID, line 55 , "n"
         MINUS, line 55 , "-"
           INT, line 55 , "1"
        RPAREN, line 55 , ")"
     SEMICOLON, line 55 , ";"
            ID, line 57 , "printf"
        LPAREN, line 57 , "("
STRING_LITERAL, line 57 , ""Sorted array in ascending order: \n""
        RPAREN, line 57 , ")"
     SEMICOLON, line 57 , ";"
            ID, line 58 , "printArray"
        LPAREN, line 58 , "("
            ID, line 58 , "data"
         COMMA, line 58 , ","
            ID, line 58 , "n"
        RPAREN, line 58 , ")"
     SEMICOLON, line 58 , ";"
        RCURLY, line 59 , "}"
```
