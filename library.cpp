#include "compiler.h"

template<CommonType CT>
void boic::put_argy( Analyzer* const a ) 
{
    static_assert( CT == Char || CT == Int );
    shared_ptr<ASTNode> n = parse_exp_( a );
    compile_exp_( n );
    if( n->reg == -1 ) 
    {
        if( n->t.token == VAR )
        {
            check_var( n->t );  
            mov_( "rdi", bqptr_( stackIdx[ n->t.lexeme ] ) );
        }
        else
        {
            int64_t i = parse_int( n->t );
            if( i > INT32_MAX || i < INT32_MIN )
            {
                movabs_( "rax", to_string( i ) );
                mov_( "rdi", bqptr_( "rax" ) );
            } else 
                mov_( "rdi", bqptr_( n->t.lexeme ) );
        }
    } 
    else 
    {
        mov_( "rdi", regTable[ n->reg ][ 0 ] );
        avail.push( n->reg );
    }
    call_( CT == Char? "write_char": "write_int" );
}

template void boic::put_argy<Int>( Analyzer* const a );
template void boic::put_argy<Char>( Analyzer* const a );

template<CommonType CT>
void boic::put_exp( Analyzer* const a ) 
{
    static_assert( CT == Char || CT == Int );
    shared_ptr<ASTNode> n = parse_exp_( a );
    compile_exp_( n );
    if( n->reg == -1 ) 
    {
        if( n->t.token == VAR )
        { 
            check_var( n->t );  
            mov_( "rdi", bqptr_( stackIdx[ n->t.lexeme ] ) );
        }
        else
        {
            int64_t i = parse_int( n->t );
            if( i > INT32_MAX || i < INT32_MIN )
                movabs_( "rdi", to_string( i ) );
            else 
                mov_( "rdi", n->t.lexeme );
        }
    } 
    else 
    {
        mov_( "rdi", regTable[ n->reg ][ 0 ] );
        avail.push( n->reg );
    }
    call_( CT == Char? "write_char": "write_int" );
}

template void boic::put_exp<Int>( Analyzer* const a );
template void boic::put_exp<Char>( Analyzer* const a );

template<CommonType CT>
void boic::get_argy( Analyzer* const a, Token& t ) 
{
    static_assert( CT == Char || CT == Int );
    a->nextTok();
    expect<LBRACK>( a );
    Token x = a->nextTok();
    lea_( "rbx", rbptr_( "BUFFER" ) );
    call_( CT == Char? "read_char": "read_int" );
    if(x.token == NUM) // TODO: This gets chars only.
    {
        mov_( "rbx", bqptr_( stackIdx[ t.lexeme ] ) );
        string idx;
        int64_t i = parse_int( x );
        if( i > INT32_MAX || i < INT32_MIN )
        {
            idx = "rdx";
            movabs_( idx, to_string( i ) );
            if( arraySet[ t.lexeme ] > 0 ) shl_( idx, arraySet[ t.lexeme ] );
        } else idx = to_string( i << arraySet[ t.lexeme ] );
        mov_
        ( 
            ( this->*str_ptr_defs[ arraySet[ t.lexeme ] ] ) ( "rbx", idx ), 
            regTable[ 8 ][ reg_idx[ arraySet[ t.lexeme ] ] ] 
        );
    }
    else if(x.token == VAR) // TODO: This gets chars only.
    {
        check_var( x );
        mov_( "rbx", bqptr_( stackIdx[ t.lexeme ] ) );
        mov_( "rdx", bqptr_( stackIdx[ x.lexeme ] ) );
        if( arraySet[ t.lexeme ] > 0 ) shl_( "rdx", arraySet[ t.lexeme ] );
        mov_
        ( 
            ( this->*str_ptr_defs[ arraySet[ t.lexeme ] ] ) 
            ( "rbx", "rdx" ), 
            regTable[ 8 ][ reg_idx[ arraySet[ t.lexeme ] ] ] 
        );
    } 
    else
        compound_error( "NUM", "VAR", x );
    expect<RBRACK>( a );
}

template void boic::get_argy<Int>( Analyzer* const a, Token& t );
template void boic::get_argy<Char>( Analyzer* const a, Token& t );

void boic::parse_put_smol_boi( Analyzer* const a ) 
{
    a->nextTok();
    expect<LPAREN>( a );
    Token t = a->peekTok();
    put_exp<Char>( a );
    expect<RPAREN>( a );
    expect<SEMICOLON>( a );
}

void boic::parse_put_boi( Analyzer* const a ) 
{
    a->nextTok();
    expect<LPAREN>( a );
    Token t = a->peekTok();
    if( t.token == STRING ) 
    {
        str_section_( sCount, t.lexeme );
        lea_( "rdi", rbptr_( strn_( sCount++ ) ) );
        int size = -2;
        for( size_t i = 0; i < t.lexeme.size() - 1; ++i )
            if( t.lexeme[ i ] != '\\' ) ++size;
        mov_( "rsi", size );
        call_( "write" );
        a->nextTok();
    }
    else
    { 
        put_exp<Int>( a );
    } 
    expect<RPAREN>( a );
    expect<SEMICOLON>( a );
}

template<CommonType CT>
void boic::parse_get_boi( Analyzer* const a ) 
{
    static_assert( CT == Char || CT == Int );
    a->nextTok();
    expect<LPAREN>( a );
    Token t = a->peekTok();
    if( t.token == VAR ) 
    {
        check_var( t );
        if( arraySet.contains( t.lexeme ) ) 
        {
            t.token = ARGYBOIS;
            get_argy<CT>( a, t );
        }
        else 
        {
            call_( CT == Char? "read_char": "read_int" );
            mov_( bqptr_( stackIdx[ t.lexeme ] ), "rax" );
            a->nextTok();
        }
    } 
    else if( t.token == ARGYBOIS ) 
    {
        check_var( t );
        get_argy<CT>( a, t ); 
    }
    else
        compound_error( "VARIABLE", "ARRAY INDEX", t );
    expect<RPAREN>( a );
    expect<SEMICOLON>( a );
}

template void boic::parse_get_boi<Int>( Analyzer* const a );
template void boic::parse_get_boi<Char>( Analyzer* const a );

void boic::get_str( Analyzer* const a, const Token& t )
{
    expect<COMMA>( a );
    Token x = a->nextTok();
    mov_( "rdi", bqptr_( stackIdx[ t.lexeme ] ) );
    if( x.token == VAR )
        mov_( "rsi", bqptr_( stackIdx[ t.lexeme ] ) );
    else if( x.token == NUM )
    {
        int64_t i = parse_int( x );
        if( i > INT32_MAX || i < INT32_MIN )
            movabs_( "rsi", to_string( i ) );
        else 
            mov_( "rsi", to_string( i ) );
    }
    else
        compound_error( "VARIABLE", "NUM", t );
    call_( "read" );
}

void boic::parse_get_str_boi( Analyzer* const a )
{
    a->nextTok();
    expect<LPAREN>( a );
    Token t = a->nextTok();
    if( t.token == VAR ) 
    {
        check_var( t );
        if( arraySet.contains( t.lexeme ) && arraySet[ t.lexeme ] == 0 ) 
        {
            t.token = ARGYBOIS;
            get_str( a, t );
        }
        else
            smolboi_array_error( t );
    } 
    else if( t.token == ARGYBOIS ) 
    {
        check_var( t );
        get_str( a, t ); 
    }
    else
        compound_error( "VARIABLE", "SMOLBOI ARRAY", t );
    expect<RPAREN>( a );
    expect<SEMICOLON>( a );
}