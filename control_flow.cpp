#include "compiler.h"

void boic::post_expression( const Token& t, shared_ptr<ASTNode> n ) 
{
    if( n->reg == -1 ) 
    {
        if( n->t.token == VAR ) 
        {
            check_var( n->t );  
            if( stackIdx[ t.lexeme ] == stackIdx[ n->t.lexeme ] )
                return;
            mov_( "rax", bqptr_( stackIdx[ n->t.lexeme ] ) );
            mov_( bqptr_( stackIdx[ t.lexeme ] ), "rax" );
            return;
        }
        int64_t i = parse_int( n->t );
        if( i > INT32_MAX || i < INT32_MIN )
        {
            movabs_( "rax", to_string( i ) );
            mov_( bqptr_( stackIdx[ t.lexeme ] ), "rax" );
        } else mov_( bqptr_( stackIdx[ t.lexeme ] ), n->t.lexeme );
        return;
    }
    mov_( bqptr_( stackIdx[ t.lexeme ] ), regTable[ n->reg ][ 0 ] );
    avail.push(n->reg);
}

shared_ptr<ASTNode> boic::parse_assignment( Analyzer* const a ) 
{
    auto root = parse_exp_( a );
    compile_exp_( root );
    expect<SEMICOLON>( a );
    return root;
}

bool boic::parse_var( Analyzer* const a, Token& t ) 
{
    if( arraySet.contains( t.lexeme ) )
    {   
        check_var( t );
        t.token = ARGYBOIS;
        return false;
    }
    if( funcSet.contains( t.lexeme ) )
    {
        t.token = FUNC;
        return false;
    }
    check_var( t );
    a->nextTok();
    expect<ASSIGNMENT>( a );
    if( a->peekTok().token == STRING ) 
    {
        str_section_( sCount, t.lexeme );
        lea_( "rax", rbptr_( sCount++ ) );
        mov_( rqptr_( stackIdx[ t.lexeme ] ), "rax" );
        return true;
    }
    post_expression(t, parse_assignment(a));
    return true;
}

inline void boic::parse_body(Analyzer* const a) 
{
    expect<LCURLY>(a);
    parse_flow(a);
    expect<RCURLY>(a);
}

void boic::parse_argy_boi( Analyzer* const a, const Token& t )
{
    #define MOV_INTO( A )                                                       \
    {                                                                           \
        mov_                                                                    \
        (                                                                       \
            ( this->*str_ptr_defs[ arraySet[ t.lexeme ] ] ) ( left, idx ),      \
            regTable[ A ][ reg_idx[ arraySet[ t.lexeme ] ] ]                    \
        );                                                                      \
    }

    a->nextTok();
    expect<LBRACK>( a );
    auto n = parse_exp_( a );
    compile_exp_( n );
    string idx;
    int id1 = -1, id2 = -1;

    if( n->t.token == NUM ) 
    {
        int64_t i = parse_int( n->t );
        if( i > INT32_MAX || i < INT32_MIN )
        {
            idx = regTable[ avail.top() ][ 0 ];
            id1 = avail.top(); avail.pop();
            movabs_( idx, to_string( i ) );
            if( arraySet[ t.lexeme ] > 0 ) shl_( idx, arraySet[ t.lexeme ] );
        } else idx = to_string( i << arraySet[ t.lexeme ] );
    }
    else if( n->t.token == VAR ) 
    {
        check_var( n->t );
        idx = regTable[ avail.top() ][ 0 ];
        id1 = avail.top(); avail.pop();
        mov_( idx, bqptr_( stackIdx[ n->t.lexeme ] ) );
        if( arraySet[ t.lexeme ] > 0 ) shl_( idx, arraySet[ t.lexeme ] );
    } 
    else 
    {
        idx = regTable[ id1 = n->reg ][ 0 ];
        if( arraySet[ t.lexeme ] > 0 ) shl_( idx, arraySet[ t.lexeme ] );
    }

    expect<RBRACK>( a );
    expect<ASSIGNMENT>( a );

    string left = regTable[ id2 = avail.top() ][ 0 ]; avail.pop();
    mov_( left, bqptr_( stackIdx[ t.lexeme ] ) );
    n = parse_assignment( a );
    if( n->reg == -1 ) 
    {
        if( n->t.token == VAR ) 
        {
            check_var( n->t );
            mov_( "rax", bqptr_( stackIdx[ n->t.lexeme ] ) );
            MOV_INTO( 8 ) 
            if(id1 != -1) avail.push( id1 ); 
            avail.push( id2 );
            return;
        } 
        int64_t i = parse_int( n->t );
        if( i > INT32_MAX || i < INT32_MIN )
        {
            movabs_( "rax", to_string( i ) );
            MOV_INTO( 8 ) 
        } else mov_( ( this->*str_ptr_defs[ arraySet[ t.lexeme ] ] ) ( left, idx ), n->t.lexeme );
        if(id1 != -1) avail.push( id1 ); 
        avail.push( id2 );
        return;
    }
    MOV_INTO( n->reg ) 
    avail.push( n->reg );
    if( id1 != -1 ) avail.push( id1 ); 
    avail.push( id2 );
}

void boic::parse_loopy_boi(Analyzer* const a) 
{
    a->nextTok(); 
    string label = loopn_( lCount++ );
    string _exit = exitn_( eCount++ );
    insert_lbl( label );
    expect<LPAREN>( a );
    shared_ptr<ASTNode> n = parse_exp_( a );
    expect<RPAREN>( a );
    compile_exp_( n );
    if( n->reg == -1 ) 
    {
        if( n->t.token == VAR ) 
        {
            check_var( n->t );
            const char* reg = regTable[ avail.top() ][ 0 ];
            mov_( reg, bqptr_( stackIdx[ n->t.lexeme ] ) );
            test_( reg, reg ); 
            jz_( _exit ); 
            parse_body(a); 
            jmp_( label ); 
            insert_lbl( _exit );
            return;
        }
        if( n->t.lexeme[0] != '0' ) 
        { parse_body( a ); jmp_( label ); return; }
        return;
    }
    const char* reg = regTable[ n->reg ][ 0 ];
    test_( reg, reg );
    jz_( _exit );
    avail.push( n->reg );
    parse_body( a );
    jmp_( label );
    insert_lbl( _exit );
}

void boic::parse_if_boi( Analyzer* const a ) 
{
    vector<int> idx;
    string _exit;
    do 
    {
        a->nextTok();
        _exit = exitn_( eCount++ );  
        expect<LPAREN>( a );
        shared_ptr<ASTNode> n = parse_exp_( a );
        expect<RPAREN>( a );
        compile_exp_( n );
        if( n->reg == -1 ) 
        {
            if( n->t.token == VAR ) 
            {
                check_var( n->t );
                const char* reg = regTable[ avail.top() ][ 0 ];
                mov_( reg, bqptr_( stackIdx[ n->t.lexeme ] ) );
                test_( reg, reg ); 
                jz_( _exit ); 
                parse_body( a );
            }
            else if(n->t.lexeme[0] != '0') { parse_body( a ); }
            else { jmp_( _exit ); parse_body( a ); }
        }
        else
        {
            const char * reg = regTable[ n->reg ][ 3 ];
            test_( reg, reg );
            jz_( _exit );
            avail.push( n->reg );
            parse_body( a );
        }
        if(a->peekTok().token == ELSEBOI) 
        {
            a->nextTok();
            string _exit1 = exitn_( eCount++ );
            jmp_( _exit1 );
            insert_lbl( _exit );
            parse_body( a );
            insert_lbl( _exit1 );
            _exit = _exit1;
            break;
        }
        if( a->peekTok().token != ELIFBOI )
        {     
            insert_lbl( _exit );
            break;
        }
        idx.push_back( x86.size() );
        insert_lbl( _exit );
    } 
    while( true );
    x86_exit_insert( _exit, idx );
}

void boic::parse_switchy_boi( Analyzer* const a )
{
    string str; str.reserve( 1000 );
    a->nextTok();
    string _exit = exitn_( eCount++ );
    expect<LPAREN>( a );
    shared_ptr<ASTNode> n = parse_exp_( a );
    expect<RPAREN>( a );
    expect<LCURLY>( a );
    compile_exp_( n );
    string lreg;
    if( n->reg == -1 ) 
    {
        if( n->t.token != VAR ) 
            switchy_error( n->t );
        check_var( n->t );
        lreg = bqptr_( stackIdx[ n->t.lexeme ] );
    }
    else
    {
        mov_( "rax", regTable[ n->reg ][ 0 ] );
        lreg = "rax";
        avail.push( n->reg );
    }
    int save = x86.size();
    Token x;
    do 
    {
        x = a->nextTok();
        if( x.token == FALLBOI )
            jmp_( str, exitn_( eCount ) );
        else if( x.token == CASEYBOI )
        {
            x = a->nextTok();
            if( x.token != NUM && x.token != CHAR )
                compound_error( "NUM", "CHAR", x );
            cmp_( str, lreg, x.lexeme );
            je_( str, exitn_( eCount ) );

            x = a->peekTok();
            if( x.token == CASEYBOI || x.token == FALLBOI )
                continue;
        } 
        else
            compound_error( "fallboi", "caseyboi", x );
        insert_lbl( exitn_( eCount++ ) );
        parse_body( a );
        jmp_( _exit );
        if( x.token == FALLBOI )
            break;
        x = a->peekTok();
    } 
    while( x.token == CASEYBOI || x.token == FALLBOI );
    if( x.token != FALLBOI ) jmp_( str, _exit );
    insert_lbl( _exit );
    x86_code_insert( str, save );
    expect<RCURLY>( a );
}

void boic::parse_yeet_boi( Analyzer* const a ) 
{ 
    a->nextTok();
    shared_ptr<ASTNode> n = parse_exp_( a );
    compile_exp_( n );
    if( n->reg == -1 ) 
    {
        if( n->t.token == VAR )
        {
            check_var( n->t );
            mov_( "rax", bqptr_( stackIdx[ n->t.lexeme ] ) );
        }
        else
        {
            int64_t i = parse_int( n->t );
            if( i > INT32_MAX || i < INT32_MIN )
                movabs_( "rax", to_string( i ) );
            else 
                mov_( "rax", n->t.lexeme );
        }
    } 
    else 
    {
        mov_( "rax", regTable[ n->reg ][ 0 ] );
        avail.push( n->reg );
    }
    expect<SEMICOLON>( a );
    jmp_( retn_( rCount ) );
}