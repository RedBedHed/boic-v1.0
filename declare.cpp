#include "compiler.h"

inline void boic::record_type( const Token& t, const Token& x )
{
    switch( x.token )
    {
    case SMOLBOI: case BOI: case CHONKBOI: case GIGABOI:   
        arraySet[ t.lexeme ] = typ_idx[ x.token - SMOLBOI ]; 
        break;
    default: array_type_error( x );// array_type_error( a );
    }
    typeSet[ t.lexeme ] = x.token;    
}

void boic::parse_decl( Analyzer* const a ) 
{
    vector<stack_arr_alloc> v;
    int idx = x86.size();
    for( ;; ) 
    {
        Token x, t = x = a->peekTok();
        if( t.token != SMOLBOI && 
            t.token != BOI && 
            t.token != CHONKBOI &&
            t.token != GIGABOI ) break;
        a->nextTok();
        Token t1 = expect<VAR>(a);

        int cnt = vCount << 3U;
        stackIdx[ t1.lexeme ] = cnt;
        
        switch( ( t = a->nextTok() ).token )
        {
        case ASSIGNMENT:
            if( ( t = a->peekTok() ).token == STRING )
            {
                arraySet[ t1.lexeme ] = 0;
                typeSet[ t1.lexeme ] = SMOLBOI;
                a->nextTok();
                expect<SEMICOLON>( a );
                str_section_( sCount, t.lexeme );
                lea_( "rax" , rbptr_( strn_( sCount++ ) ) );
                mov_( bqptr_( cnt ), "rax" );
            }
            else if( t.token == LCURLY )
            {
                a->nextTok();
                record_type( t1, x );
                const int idx = x.token - SMOLBOI;
                const char* decl_type = declType[ idx ];
                const int64_t upper_limit = declLimits[ idx ];
                section_( ".data" );
                insert_lbl( arrn_( dCount ) );
                for(;;)
                {
                    t = expect<NUM>( a );
                    int64_t i = parse_unsigned_int( t );
                    if( i > upper_limit )
                        nums_too_big_error( t );
                    put_value( decl_type, t.lexeme );
                    if( a->peekTok().token == RCURLY )
                        break;
                    expect<COMMA>( a );
                }
                expect<RCURLY>( a );
                expect<SEMICOLON>( a );
                section_( ".text" ); 
                lea_( "rax", rqptr_( arrn_( dCount++ ) ) );
                mov_( bqptr_( cnt ), "rax" );
            }
            else
                post_expression( t1, parse_assignment( a ) );
            break;
        case LBRACK:
        {
            record_type( t1, x );
            t = expect<NUM>( a );
            expect<RBRACK>( a );
            expect<SEMICOLON>( a );
            int64_t i = parse_int( t );
            aCount += ( i << arraySet[ t1.lexeme ] );
            v.push_back( { cnt, i << arraySet[ t1.lexeme ] } );
            break;
        }
        case SEMICOLON:
            mov_( bqptr_( cnt ), 0 );
            break;
        default:
            declaration_error( t );
        }
        ++vCount;
    }
    x86_stack_alloc( idx, vCount << 3U, v );
}

