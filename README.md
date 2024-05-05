<p align="center">
  <image src="/art/herecomedatboi.jpg" width="500"></image>
</p>
<h1 align="center"><b>Boi Language</b></h1>
<h4 align="center"><b><i>The Language of Memes</i></b></h4>
<h5 align="center"><b><i>Version 1.0</i></b></h5>
<h6 align="center"><b><i>(art from openart.ai)</i></b></h6>

#

## Introduction

Boi is a toy programming language in which all keywords contain the substring "boi". As you can tell, it is meant to be taken very seriously, and you should definitely waste your time learning it.

## FAQ
> ### How Does the Boi Compiler Work?

Boic&mdash; at the highest level&mdash; is the bash shell script, ```boic.sh```.

Boic executes the ```comp``` binary. Comp lexes your file according to the Boi Lexical Specification, turning it into a list of "Tokens" (lexeme/token structures). Next, it feeds this list of tokens to a Recursive Descent parser, which transforms all expressions into Abstract Syntax Trees and generates x86 intel-syntax assembly as it parses the trees and control flow. Boic then tells GCC to assemble the generated assembly and link it with the library file, creating an executable binary with relative addresses. Boic then executes this binary from your shell, replacing its relative addresses with physical addresses, loading it into volatile memory, and running it on your CPU.

> ### Does the Boi Compiler Generate Optimal Assembly?

***No.***

Boic v1.0 generates assembly that *works.* It simplifies literal expressions, but leaves everything else for runtime.

<p align="center">
  <image src="/art/BoicGood.jpg" width="350"></image>
</p>

## Index
1. [Boilang - The Guide](DOC/Lang.md)
2. [Example Boi Programs](DOC/Example.md)
3. [Lexer Notes](DOC/Lexer.md)
4. [Parser Notes](DOC/Parser.md)
