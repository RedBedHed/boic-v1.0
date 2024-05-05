# Main Grammar (Hopefully Complete...)

```
<main>                := "@" "datboi" "(" "argybois" ")"
<main-def>            := <main> <func-body>

<func-sig>            := "@" <var> "(" <formal-param-list> ")"
<func-def>            := <func-sig> <func-body>
<func-decl>           := <func-sig> ";"
<func-call>           := <var> "(" <call-param-list> ")" <semicolon>
<func-body>           := "{" <decl> <code> "}"

<call-param-list>     := <expression> "," <call-param-list> | <expression>
<formal-array-type>   := <var> "[" <type> "]"
<formal-param>        := <var> | <formal-array-type>
<formal-param-list>   := <formal-param> "," <formal-param-list> | <formal-param>

<type>                := "smolboi" | "boi" | "chonkboi" | "gigaboi"

<decl>                := <type> <var> ";" <decl> | <type> <assignment> <decl> | 
                         <type> <var> "[" <num> "]" ";"

<var>                 := [a-zA-Z_][a-zA-Z_0-9]*
<num>                 := 0|([1-9][0-9]*)
<string>              := "([\s!#-\[\[-~]|\\.)+"
<char-num>            := <char> | <num>

<lvalue>              := <var> "[" <expression> "]" | <var>
<assign>              := <lvalue> "=" <expression> | <lvalue> "=" <string>
<assignment>          := <assign> ";"

<loop>                := "loopyboi" "(" <expression> ")" <body>
<elif>                := "elifboi" "(" <expression> ")" <body> <elif> | ""
<else>                := "elseboi" <body> | ""
<if>                  := "ifboi" "(" <expression> ")" <body> <elif> <else>
<case>                := "caseyboi" <char-num> <body> <case> | "caseyboi" <char-num> <case> | "caseyboi" <char-num> <body>
<fallboi>             := "fallboi" <body>
<opt-case>            := <case> | ""
<opt-fallboi>         := <fallboi> | ""
<case-statements>     := <opt-case> <fallboi> | <case> <opt-fallboi>
<switch>              := "switchyboi" "(" <expression> ")" "{" <case-statements> "}"
<return>              := "yeetboi" <expression> ";"

<code>                := <if> | <loop> | <switch> | <function-call> | <assignment> | <return>
<body>                := <lcurly> <code> <rcurly>

<decl-or-def>         := <func-def> | <func-decl> | ""

<program>             := <decl-or-def> <main-def> <decl-or-def>
```

# Expression Grammar

```
<expression> := <logic-or>
<logic-or>   := <logic-and> <e1>
<e1>         := "||" <logic-and> <e1> | ""
<logic-and>  := <logic-eq> <e2>
<e2>         := "&&" <logic-eq> <e2> | ""
<logic-eq>   := <logic-ne> <e3>
<e3>         := "==" <logic-ne> <e3> | ""
<logic-ne>   := <cmp> <e4>
<e4>         := "!=" <cmp> <e4> | ""
<cmp>        := <or> <e5>
<e5>         := "<" <or> <e5> | "<=" <or> <e5> | ">" <or> <e5> | ">=" <or> <e5> | ""
<or>         := <xor> <e6>
<e6>         := "|" <xor> <e6> | ""
<xor>        := <and> <e7>
<e7>         := ^ <and> <e7> | ""
<and>        := <shr> <e8>
<e8>         := & <shr> <e8> | ""
<shr>        := <shl> <e9>
<e9>         := ">>" <shl> <e9> | ""
<shl>        := <minus> <e10>
<e10>        := "<<" <minus> <e10> | ""
<minus>      := <plus> <e11>
<e11>        := "-" <plus> <e11> | ""
<plus>       := <mod> <e12> 
<e12>        := "+" <mod> <e12> | ""
<mod>        := <div> <e13>
<e13>        := "%" <div> <e13> | ""
<div>        := <mult> <e14>
<e14>        := "/" <mult> <e14> | ""
<mult>       := <base> <e15>
<e15>        := "*" <base> <e15> | ""
<base>       := "(" <expression> ")" | "--" <var> | "++" <var> |
                "~" <var> | "!" <var> | "!!" <var> | "-" <var> |
                "~" <num> | "!" <num> | "!!" <num> | "-" <num> |
                "~" "(" <expression> ")" | "!" "(" <expression> ")" | "!!" "(" <expression> ")" | "-" "(" <expression> ")" |
                <var> | <num> | <char> | <var> "[" <expression> "]" | <var> "(" <param-list> ")"
```
