#include "compiler.h"

void boic::function_call_( shared_ptr<ASTNode> n, const string& lreg )         
{
    if( !funcSet.contains( n->t.lexeme ) )
        undeclared_error( n->t );
    if( funcSet[ n->t.lexeme ] != n->pCount )
        param_count_error( n, funcSet[ n->t.lexeme ] );
    shared_ptr<ASTNode> nx = n;
    int save = x86.size();
    sub_( "rsp", n->pCount << 3U );
    for( int i = 0; i < n->pCount; ++i ) 
    {
        nx = nx->middle;
        if( nx->t.token == NUM || nx->t.token == CHAR )
        {
            if( types.contains( n->t.lexeme ) && ( * types[ n->t.lexeme ] )[ i ] != 0 )
                param_type_error( n->t, ( * types[ n->t.lexeme ] )[ i ], 0 );
            mov_( sqptr_( i << 3U ), nx->t.lexeme );
        }
        else if( nx->t.token == VAR )
        { 
            check_var( nx->t );  
            if( types.contains( n->t.lexeme ) && 
              ( * types[ n->t.lexeme ] )[ i ] != uint64_t( typeSet[ nx->t.lexeme ] ) )
            {
                param_type_error( n->t, ( * types[ n->t.lexeme ] )[ i ], typeSet[ nx->t.lexeme ] );
            }
            else
            {
                mov_( "rax", bqptr_( stackIdx[ nx->t.lexeme ] ) );
                mov_( sqptr_( i << 3U ), "rax" );
            }
        }
        else 
        {
            compile_exp_( nx );
            if( types.contains( n->t.lexeme ) && ( * types[ n->t.lexeme ] )[ i ] != 0 )
                param_type_error( n->t, ( * types[ n->t.lexeme ] )[ i ], 0 );
            if( nx->reg == -1 ) 
            {
                int64_t u = parse_int( nx->t );
                if( u > INT32_MAX || u < INT32_MIN )
                {
                    movabs_( "rax", to_string( u ) );
                    mov_( sqptr_( i << 3U ), "rax" );
                } else
                    mov_( sqptr_( i << 3U ), nx->t.lexeme );
            } 
            else 
            {
                mov_( "rax", regTable[ nx->reg ][ 0 ] );
                mov_( sqptr_( i << 3U ), "rax" );
                avail.push( nx->reg );
            }
        }
    }
    call_( n->t.lexeme );
    add_( "rsp", n->pCount << 3U );
    string str; str.reserve( 1000 );
    if( 0 < used.size() )
        for( size_t i = 0;; )
        {
            push_( str, regTable[ used[ i ] ][ 0 ] );
            if( ++i >= used.size() ) break;
            str.push_back( '\n' );
        }
    for(int i = used.size() - 1; i >= 0; --i)
        pop_( regTable[ used[ i ] ][ 0 ] );
    mov_( lreg, "rax" );
    x86_code_insert( str, save );
}

void boic::parse_funcy_boi( Analyzer* const a, const Token& t )
{
    shared_ptr<ASTNode> n = make_shared<ASTNode>(t);
    a->nextTok();
    shared_ptr<ASTNode> nx = n;
    expect<LPAREN>( a );
    int pCount = 0;
    while( a->peekTok().token != RPAREN ) 
    {
        ++pCount;
        nx->middle = parse_exp_( a );
        nx = nx->middle;

        if( a->peekTok().token != COMMA )
            break;
        a->nextTok();
    }
    expect<RPAREN>( a );
    expect<SEMICOLON>( a );
    n->pCount = pCount;
    function_call_( n, "rax" );
}

template<bool SUBSEQUENT>
void boic::check_signature( const Token& t, const uint32_t i, const uint32_t j, shared_ptr<vector<size_t>>& v )
{
    if constexpr( SUBSEQUENT ) 
    {
        if( i >= v->size() )
            too_many_params_error( t );
        if( ( *v )[ i ] != j )
            formal_param_type_error( t, j, ( *v )[ i ] );
    } 
    else v->push_back( j );
}

template void boic::check_signature<true>( const Token& t, const uint32_t i, const uint32_t j, shared_ptr<vector<size_t>>& v );
template void boic::check_signature<false>( const Token& t, const uint32_t i, const uint32_t j, shared_ptr<vector<size_t>>& v );

template<bool SUBSEQUENT>
int boic::parse_params( const Token& t, Analyzer* const a, shared_ptr<vector<size_t>>& v )
{
    Token t1;
    size_t i = 0;
    if( a->peekTok().token == RPAREN )
        return 0;
    for( ;; )
    {
        t1 = expect<VAR>( a );
        params.push_back( t1.lexeme );
        if( a->peekTok().token == LBRACK )
        {
            a->nextTok();
            const Token t2 = a->nextTok();
            check_signature<SUBSEQUENT>( t, i, t2.token, v );
            record_type( t1, t2 );
            expect<RBRACK>( a ); 
        } 
        else check_signature<SUBSEQUENT>( t, i, 0, v ); 
        ++i;
        if( ( t1 = a->peekTok() ).token != COMMA )
            break;
        a->nextTok();
    }
    if constexpr( SUBSEQUENT )
        if( v->size() != i )
            too_few_params_error( t );
    return i;
}

template int boic::parse_params<true>( const Token& t, Analyzer* const a, shared_ptr<vector<size_t>>& v );
template int boic::parse_params<false>( const Token& t, Analyzer* const a, shared_ptr<vector<size_t>>& v );
