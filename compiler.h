#pragma once
#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <iostream>
#include <vector>
#include <cassert>
#include "analyzer.h"
#include <deque>
#include <algorithm>
#include <cstdio>

using std::string;
using std::vector;
using std::ofstream;
using std::cout;

using std::deque;
using namespace lexer;

using std::to_string;
using std::exception;
using std::snprintf;

#define MAP_DEF( name, type ) \
class name final : public unordered_map<string, type>   \
{                                                       \
public:                                                 \
    bool                                                \
    contains(const string& n)                           \
    { return find(n) != end(); }                        \
};

class int_set final: public vector<int> 
{
public:
    void add( const int i )
    {
        if ( std::find( begin(), end(), i ) == end() )
            push_back( i );
    }
};

class vec_map final: public unordered_map<string, shared_ptr<vector<size_t>>>
{
public:                                                 
    bool                                                
    contains(const string& n)                           
    { return find(n) != end(); }                        
};

MAP_DEF( str_map, string );
MAP_DEF( int_map, int64_t );
MAP_DEF( boo_map, bool );

class str_list final : public vector<string> 
{ public: void pb( const string& str ) { push_back(str); } };

struct ASTNode final 
{
    shared_ptr<ASTNode> 
        left = nullptr, 
        right = nullptr,
        middle = nullptr;
    Token t;
    int pCount = 0, reg = -1;
    ASTNode(Token t): t(t) {} 
};

struct stack_arr_alloc final { int64_t idx, size; };

enum CommonType : uint8_t { Char, Int };

enum Tokens : uint32_t 
{
    NUM,
    COMMENT,
    DOT,
    COMMA,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    COLON,
    DOLLAR_SIGN,
    UNDERSCORE,
    BACK_TICK,
    QUESTION_MARK,
    BACK_SLASH,
    AT,
    ARGYBOIS,
    SMOLBOI,
    BOI,
    CHONKBOI,
    GIGABOI,
    IFBOI,
    ELSEBOI,
    ELIFBOI,
    LOOPYBOI,
    SWITCHYBOI,
    CASEYBOI,
    FALLBOI,
    BREAKYBOI,
    YEETBOI,
    BONKBOI,
    PUTBOI,
    PUTSMOLBOI,
    GETBOI,
    GETSMOLBOI,
    GETSMOLBOIS,
    THATBOI,
    CHAR,
    ASSIGNMENT,
    LOGIC_OR,
    LOGIC_AND,
    EQUAL,
    NOT_EQUAL,
    OR,
    XOR,
    AND,
    RIGHT_SHIFT,
    LEFT_SHIFT,
    MINUS,
    PLUS,
    MOD,
    DIVIDE,
    MULT,
    NOT,
    LOGIC_NOT,
    BOOLEANIZE,
    INC,
    DEC,
    LPAREN,
    RPAREN,
    LCURLY,
    RCURLY,
    LBRACK,
    RBRACK,
    LESS,
    GREATER,
    LESS_EQ,
    GREAT_EQ,
    SEMICOLON,
    VAR,
    STRING,
    ERROR,
    END,
    FUNC
};

constexpr const char* regTable[12][4] = 
{
    { "r15", "r15d", "r15w", "r15b" }, { "r14", "r14d", "r14w", "r14b" },
    { "r13", "r13d", "r13w", "r13b" }, { "r12", "r12d", "r12w", "r12b" },
    { "r11", "r11d", "r11w", "r11b" }, { "r10", "r10d", "r10w", "r10b" },
    {  "r9",  "r9d",  "r9w",  "r9b" }, {  "r8",  "r8d",  "r8w",  "r8b" },
    { "rax",  "eax",   "ax",   "al" }, { "rbx",  "ebx",   "bx",   "bl" },
    { "rcx",  "ecx",   "cx",   "cl" }, { "rdx",  "edx",   "dx",   "dl" },
};

constexpr const char* declType[] = 
{ ".byte", ".value", ".long", ".quad" };

constexpr uint64_t declLimits[] =
{ UINT8_MAX, UINT16_MAX, UINT32_MAX,  UINT64_MAX };

// template<Tokens T>
// ASTNode* parse(Analyzer* const a, ASTNode* const n);

constexpr const char* bois[] = 
{ 
    ":'(", ":(",  ":C", ":P", "<3",  ">:(", ">:)", 
    "T_T", "-_-", ":|", ":/", "O.O", "X_X", "OwO"
};

constexpr const char* bunboi = 
    "{\\_/} \n"
    "( *.*) \n"
    "/ > F  \n\t"
    "Well, at least you tried. ";

#define sizeofstr( str ) sizeof(str) / sizeof(char*)

template<Tokens T>
inline Token expect(Analyzer* const a) 
{
    Token t = a->nextTok(); 
    if(t.token != T) {
        std::cout
            << bunboi
            << bois[time(0) % sizeofstr( bois )] 
            << "\n\tExpected " 
            << a->translate<(int)T>() 
            << ", got " 
            << a->translate(t.token) 
            << ' '
            << t.lexeme
            << " at line "
            << t.line_no
            << '\n';
        exit(0);
    }
    return t;
}

inline void error(Analyzer* const a) 
{
    Token t = a->nextTok(); 
    std::cout 
        << bunboi 
        << bois[time(0) % sizeofstr( bois )] 
        << "\n\t"
        << a->translate(t.token) 
        << ' '
        << t.lexeme
        << " doesn't belong at line "
        << t.line_no
        << '\n';
    exit(0);
}

inline void redef_error(const Token& t) 
{
    std::cout 
        << bunboi 
        << bois[time(0) % sizeofstr( bois )] 
        << "\n\tRedefinition of "
        << t.lexeme
        << " at line "
        << t.line_no
        << '\n';
    exit(0);
}

inline void missing_datboi_error(Analyzer* const a)
{
    Token t = a->nextTok(); 
    std::cout 
        << bunboi 
        << bois[time(0) % sizeofstr( bois )] 
        << "\n\tERROR: "
        << "Reached line "
        << t.line_no
        << " and did not find datboi. "
        << "Where are you hiding himb?"
        << '\n';
    exit(0);
}

inline void undeclared_error(const Token& t)
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: "
         << t.lexeme 
         << " was not declared at line "
         << t.line_no
         << ".\n";
    exit( 0 );
}

inline void param_count_error(const shared_ptr<ASTNode>& n, const int count)
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: "
         << n->t.lexeme 
         << " was declared with "
         << count
         << " parameters, not "
         << n->pCount
         << " on line "
         << n->t.line_no
         << ".\n";;
    exit( 0 );
}

inline void param_count_error(const Token& t, const int pCount, const int count)
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: "
         << t.lexeme 
         << " was declared with "
         << count
         << " parameters, not "
         << pCount 
         << " on line "
         << t.line_no
         << ".\n";
    exit( 0 );
}

inline void variable_undeclared_error(const Token& t)
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: "
         << t.lexeme 
         << " on line "
         << t.line_no
         << " is undeclared."
         << " Plz declare your"
         << " bois before using them.\n";
    exit( 0 );
}

inline void switchy_error(const Token& t)
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: " 
         << "Constant expression used"
         << " in switch statement on line "
         << t.line_no
         << ".\n";
    exit( 0 );
}

constexpr const char* boitypes[] = 
{ "VARIABLE", "SMOLBOI", "BOI", "CHONKBOI", "GIGABOI" };

inline void param_type_error(const Token& t, int i, int j)
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: " 
         << "Passed a parameter of type "
         << boitypes[ j &= -( ( j -= ARGYBOIS ) >= 0 ) ]
         << " instead of type "
         << boitypes[ i &= -( ( i -= ARGYBOIS ) >= 0 ) ]
         << " on line "
         << t.line_no
         << " for function '"
         << t.lexeme
         << "()'.\n";
    exit( 0 );
}

inline void formal_param_type_error(const Token& t, int i, int j)
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: " 
         << "Redeclared a parameter of type "
         << boitypes[ j &= -( ( j -= ARGYBOIS ) >= 0 ) ]
         << " as type "
         << boitypes[ i &= -( ( i -= ARGYBOIS ) >= 0 ) ]
         << " on line "
         << t.line_no
         << " for function '"
         << t.lexeme
         << "()'.\n";
    exit( 0 );
}

inline void too_few_params_error( const Token& t )
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: " 
         << "Not enough param bois on line "
         << t.line_no
         << " for function '"
         << t.lexeme
         << "()'.\n";
    exit( 0 );    
}

inline void too_many_params_error( const Token& t )
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: " 
         << "Too many param bois on line "
         << t.line_no
         << " for function '"
         << t.lexeme
         << "()'.\n";
    exit( 0 );    
}

inline void not_an_array_error( const Token& t )
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: " 
         << "Variable "
         << t.lexeme
         << " on line "
         << t.line_no
         << " is not an array.\n";
    exit( 0 );    
}

inline void array_error( const Token& t )
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: "
         << "Illegal increment/decrement." 
         << " Variable "
         << t.lexeme
         << " on line "
         << t.line_no
         << " an array.\n";
    exit( 0 );    
}

inline void nums_too_big_error( const Token& t )
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: Number " 
         << t.lexeme
         << " on line " 
         << t.line_no 
         << " is too big\n";
    exit(0);
}  

inline void division_by_zero_error( const Token& t )
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: division by zero on"
         << " line " 
         << t.line_no 
         << "\n";
    exit(0);    
}

inline void array_type_error( const Token& t )
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: " 
         << "Illegal array type "
         << t.lexeme
         << " on line "
         << t.line_no
         << ".\n"; 
    exit(0);
}

inline void illegal_statement( const Token& t )
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: " 
         << "Illegal statement in local scope. Check "
         << t.lexeme
         << " on line "
         << t.line_no
         << ".\n"; 
    exit(0);    
}

inline void compound_error( const char* a, const char* b, const Token& t )
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: " 
         << "Expected " 
         << a 
         << " or "
         << b 
         << ", got "
         << t.lexeme
         << " on line "
         << t.line_no
         << ".\n"; 
    exit(0);    
}

inline void declaration_error( const Token& t )
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: " 
         << "Illegal symbol "
         << t.lexeme
         << " in declaration on line "
         << t.line_no
         << ".\n";
    exit(0);    
}

inline void smolboi_array_error( const Token& t )
{
    cout << bunboi 
         << bois[time(0) % sizeofstr( bois )] 
         << "\n\tERROR: " 
         << "VARIABLE "
         << t.lexeme
         << " on line "
         << t.line_no
         << " is not a SMOLBOI ARRAY.\n";
    exit(0);    
}

// Solution by user2622016 on stack overflow.
template< typename... Args >
string ssprintf( const char* format, Args... args ) 
{
    int length = snprintf( nullptr, 0, format, args... );
    assert( length >= 0 );
    char* buf = new char[length + 1];
    snprintf( buf, length + 1, format, args... );
    string str( buf );
    delete[] buf;
    return str;
}

shared_ptr<ASTNode> parse_exp_(Analyzer* const a);

#define BIN_DEF( NAME, TYPE )                                                           \
inline void NAME( const char* s1, const char* s2 )                                      \
{ x86.pb( ssprintf( TYPE" %s, %s", s1, s2 ) ); }    \
inline void NAME( const string& s1, const string& s2 )                                  \
{ NAME(s1.c_str(), s2.c_str()); }                                                       \
inline void NAME( const char* s1, const string& s2 )                                    \
{ NAME(s1, s2.c_str()); }                                                               \
inline void NAME( const string& s1, const char* s2 )                                    \
{ NAME(s1.c_str(), s2); }                                                               \
inline void NAME( const char* s1, const int i )                                         \
{ NAME(s1, to_string(i)); }                                                             \
inline void NAME( const string& s1, const int i )                                       \
{ NAME(s1, to_string(i)); }                                     

#define UNA_DEF( NAME, TYPE )                                                           \
inline void NAME( const char* s1 )                                                      \
{ x86.pb( ssprintf( TYPE" %s", s1 ) ); }            \
inline void NAME( const string& s1 )                                                    \
{ NAME(s1.c_str()); }      

#define MEM_DEF( NAME, TYPE )                                                           \
inline string NAME( const char* s1 )                                                    \
{ return ssprintf( TYPE"[ %s ]", s1 ); }            \
inline string o##NAME( const char* s1, const char* s2 )                                 \
{ return ssprintf( TYPE"[ %s+%s ]", s1, s2 ); }     \
inline string NAME( const string& s1 )                                                  \
{ return NAME(s1.c_str()); }                                                            \
inline string o##NAME( const string& s1, const string& s2 )                             \
{ return o##NAME( s1.c_str(), s2.c_str() ); }                                           \
inline string o##NAME( const string& s1, const char* s2 )                               \
{ return o##NAME( s1.c_str(), s2 ); }                                                   \
inline string o##NAME( const char* s1, const string& s2 )                               \
{ return o##NAME( s1, s2.c_str() ); }                                                   \
inline string o##NAME( const char* s1, const int s2 )                                   \
{ return o##NAME( s1, to_string(s2) ); }                                                \
inline string o##NAME( const string& s1, const int s2 )                                 \
{ return o##NAME( s1.c_str(), to_string(s2) ); }                                        \
inline string s##NAME( const string& s1 )                                               \
{ return o##NAME("rsp", s1); }                                                          \
inline string s##NAME( const char* s1 )                                                 \
{ return s##NAME(string(s1)); }                                                         \
inline string b##NAME( const string& s1 )                                               \
{ return o##NAME("rbp", s1); }                                                          \
inline string b##NAME( const char* s1 )                                                 \
{ return b##NAME(string(s1)); }                                                         \
inline string r##NAME( const string& s1 )                                               \
{ return o##NAME("rip", s1); }                                                          \
inline string r##NAME( const char* s1 )                                                 \
{ return r##NAME(string(s1)); }                                                         \
inline string s##NAME( const int s1 )                                                   \
{ return s##NAME(to_string(s1).c_str()); }                                              \
inline string b##NAME( const int s1 )                                                   \
{ return b##NAME(to_string(s1).c_str()); }                                              \
inline string r##NAME( const int s1 )                                                   \
{ return r##NAME(to_string(s1).c_str()); }                                              \

class boic final 
{
private:
str_list x86;
boo_map funcDef;
vec_map types;
int_map arraySet, typeSet, funcSet, stackIdx;
stack<int> avail;
int_set used;
vector<string> params;
int rCount, sCount, dCount, vCount, eCount, lCount, aCount;

inline void str_( const int s1, const string& s2 )
{ x86.pb( ssprintf( "S%d: .STRING ", s1 ) + s2 ); }

inline void arr_( const int s1, const string& s2 )
{ x86.pb( ssprintf( ".lcomm D%d, %s", s1, s2.c_str() ) ); }

void cmp_( string& str, const string& s1, const string& s2 )
{ str.append( ssprintf( "cmp %s, %s\n", s1.c_str(), s2.c_str() ) ); }
void je_( string& str, const string& s1 )
{ str.append( ssprintf( "je %s\n", s1.c_str() ) ); }
void jmp_( string& str, const string& s1 )
{ str.append( ssprintf( "jmp %s\n", s1.c_str() ) ); }

BIN_DEF( mov_  , "mov"   ) BIN_DEF( add_   , "add"    ) BIN_DEF( sub_  , "sub"   )
BIN_DEF( lea_  , "lea"   ) BIN_DEF( imul_  , "imul"   ) BIN_DEF( movzx_, "movzx" )
BIN_DEF( and_  , "and"   ) BIN_DEF( or_    , "or"     ) BIN_DEF( xor_  , "xor"   )
BIN_DEF( shl_  , "shl"   ) BIN_DEF( shr_   , "shr"    ) BIN_DEF( cmp_  , "cmp"   )
BIN_DEF( test_ , "test"  ) BIN_DEF( movabs_, "movabs" ) BIN_DEF( movsx_, "movsx" )

UNA_DEF( setne_, "setne" ) UNA_DEF( sete_  , "sete"   ) UNA_DEF( setle_, "setle" ) 
UNA_DEF( setge_, "setge" ) UNA_DEF( setg_  , "setg"   ) UNA_DEF( setl_ , "setl"  )
UNA_DEF( not_  , "not"   ) UNA_DEF( neg_   , "neg"    ) UNA_DEF( div_  , "div"   )
UNA_DEF( pop_  , "pop"   ) UNA_DEF( push_  , "push"   ) UNA_DEF( inc_  , "inc"   )
UNA_DEF( dec_  , "dec"   ) UNA_DEF( jmp_   , "jmp"    ) UNA_DEF( je_   , "je"    )
UNA_DEF( jne_  , "jne"   ) UNA_DEF( jz_    , "jz"     ) UNA_DEF( call_ , "call"  )
UNA_DEF( idiv_ , "idiv"  )

MEM_DEF( bptr_, "BYTE PTR "  ) MEM_DEF( wptr_, "WORD PTR "  )
MEM_DEF( dptr_, "DWORD PTR " ) MEM_DEF( qptr_, "QWORD PTR " )

inline void ret_() { x86.pb( "ret" ); } inline void cqo_() { x86.pb( "cqo" ); }
inline void syscall_() { x86.pb( "syscall" ); }

inline void section_( const char* s1 )
{ x86.pb( ssprintf( ".section %s", s1 ) ); }

void push_( string& str, const char* reg )
{ str.append( ssprintf( "push %s", reg ) ); }

inline void x86_stack_alloc( const int idx, const int i, const vector<stack_arr_alloc>& v )
{ 
    string s = ( i > 0? "sub rsp, " + to_string( i ) + "\n": "" ) +
               "mov rax, rbp\n"
               "mov rbp, rsp\n"
               "push rax\n";
    for( const stack_arr_alloc& p: v )
    {
        s.append( "sub rsp, " ); s.append( to_string( p.size ) );
        s.append( "\nlea rax, QWORD PTR [ rsp ]\n" );
        s.append( "mov " ); s.append( bqptr_( p.idx ) ); s.append( ", rax\n" );
    }
    x86.insert( x86.begin() + idx, s.substr( 0, s.size() - 1 ) );
}

inline void x86_exit_insert( const string& lb, const vector<int>& idx )
{
    for( int i = int( idx.size() ); --i >= 0; )
        x86.insert( x86.begin() + idx[ i ], "jmp " + lb ); 
}

inline void put_value( const char* s1, const string& s2 )
{ x86.pb( ssprintf( "%s %s", s1, s2.c_str() ) ); }

inline void x86_code_insert( const string& lb, const int idx )
{ x86.insert( x86.begin() + idx, lb ); }

inline void lbl_( const int i )
{ x86.pb( ssprintf( "L%d:", i ) );  }

#define LBL_DEF( name, str ) \
inline string name##n_( const int i ) { return str + to_string( i ); }

LBL_DEF( lbl, "L" ) LBL_DEF( exit, "E" ) LBL_DEF( loop, "X" ) LBL_DEF( ret, "RET" )
LBL_DEF( str, "S" ) LBL_DEF( arr,  "D" )

inline void insert_lbl( const string& lb ) { x86.pb( lb + ":" ); }

inline void str_section_( const int s1, string& s2 )
{ section_( ".data" ); s2.push_back( '\0' ); str_( s1, s2 ); section_( ".text" );  }

inline void arr_section_( const int s1, const string& s2 )
{ section_( ".data" ); arr_( s1, s2 ); section_( ".text" );  }

#define B_DEF( type, str, arg1, arg2 ) static constexpr type ( boic::*str##_defs[9] ) ( const arg1 x, const arg2 y )
#define A_DEF( str, arg1, arg2 )                                                        \
B_DEF( string, str, arg1, arg2 ) = { &boic::obptr_, &boic::owptr_, &boic::odptr_, &boic::oqptr_ };
#define C_DEF( str, arg1, arg2 )                                                        \
B_DEF( void, str, arg1, arg2 )   = { &boic::movsx_, &boic::movsx_, &boic::movsx_, &boic::mov_   };

A_DEF( chr_ptr, char*, string& ) A_DEF( lit_ptr, char*, int ) A_DEF( str_ptr, string&, string& )
C_DEF( chr_mov, char*, string& ) C_DEF( str_mov, string&, char* )
 
static constexpr uint8_t reg_idx[] = { 3, 2, 1, 0 };
static constexpr uint8_t boi_idx[] = { SMOLBOI, BOI,CHONKBOI, GIGABOI };
static constexpr uint8_t typ_idx[] = { 0, 1, 2, 3 };

inline void check_var( const Token& t ) 
{
    if( !stackIdx.contains( t.lexeme ) )
        variable_undeclared_error( t );
}

inline void check_arr( const Token& t )
{
    if( !arraySet.contains( t.lexeme ) )
        not_an_array_error( t );
}

inline void check_not_arr( const Token& t )
{
    if( arraySet.contains( t.lexeme ) )
        array_error( t );
}

inline int64_t parse_int( const Token& t) 
{
    int64_t op1;
    try 
    {
        op1 = stoll( t.lexeme );
    } 
    catch( exception& e ) 
    { nums_too_big_error( t ); }
    return op1;
}

inline uint64_t parse_unsigned_int( const Token& t) 
{
    uint64_t op1;
    try 
    {
        op1 = stoull( t.lexeme );
    } 
    catch( exception& e ) 
    { nums_too_big_error( t ); }
    return op1;
}

template<bool DIV = false>
void get_right_hand( shared_ptr<ASTNode> n, string& rreg, int& rid );
void compile_exp_( shared_ptr<ASTNode> n );
void function_call_( shared_ptr<ASTNode> n, const string& lreg );
void post_expression( const Token& t, shared_ptr<ASTNode> n );
shared_ptr<ASTNode> parse_assignment( Analyzer* const a );
void record_type( const Token& t, const Token& x );
void parse_decl( Analyzer* const a );
template<bool SUBSEQUENT>
void check_signature( const Token& t, const uint32_t i, const uint32_t j, shared_ptr<vector<size_t>>& v );
template<bool SUBSEQUENT>
int parse_params( const Token& t, Analyzer* const a, shared_ptr<vector<size_t>>& v );
bool parse_var( Analyzer* const a, Token& t );
template<CommonType CT>
void put_argy( Analyzer* const a );
template<CommonType CT>
void put_exp( Analyzer* const a );
template<CommonType CT>
void get_argy( Analyzer* const a, Token& t );
void parse_put_smol_boi( Analyzer* const a );
void parse_put_boi( Analyzer* const a );
template<CommonType CT>
void parse_get_boi( Analyzer* const a );
void get_str( Analyzer* const a, const Token& t );
void parse_get_str_boi( Analyzer* const a );
void parse_body(Analyzer* const a);
void parse_loopy_boi(Analyzer* const a);
void parse_if_boi( Analyzer* const a );
void parse_switchy_boi( Analyzer* const a );
void parse_argy_boi( Analyzer* const a, const Token& t );
void parse_yeet_boi( Analyzer* const a );
void parse_funcy_boi( Analyzer* const a, const Token& t );
void parse_flow(Analyzer* const a);
void parse_boi( Analyzer* const a );

public:

boic( Analyzer* const a );

};

#endif