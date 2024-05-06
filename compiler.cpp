
#include "compiler.h"

void boic::parse_flow(Analyzer* const a) 
{
    #define BY_ONE( func_ )                                             \
    {                                                                   \
        a->nextTok();                                                   \
        Token x = expect<VAR>( a );                                     \
        check_var( x );                                                 \
        check_not_arr( x );                                             \
        mov_( "rax", bqptr_( stackIdx[ x.lexeme ] ) );                  \
        func_( "rax" );                                                 \
        mov_( bqptr_( stackIdx[ x.lexeme ] ), "rax" );                  \
        expect<SEMICOLON>( a );                                         \
        goto top;                                                       \
    }      

    top:
    Token t = a->peekTok();
    switch(t.token) 
    {
        case INC:           BY_ONE( inc_ )              goto top;
        case DEC:           BY_ONE( dec_ )              goto top;
        case IFBOI:         parse_if_boi( a );          goto top;
        case LOOPYBOI:      parse_loopy_boi( a );       goto top;
        case SWITCHYBOI:    parse_switchy_boi( a );     goto top;
        case VAR:           if( parse_var( a, t ) )     goto top;
        case ARGYBOIS:      
            if( t.token == ARGYBOIS )
            {
                            parse_argy_boi( a, t );     goto top;
            }
        case FUNC:          parse_funcy_boi( a, t );    goto top;
        case PUTBOI:        parse_put_boi( a );         goto top;
        case PUTSMOLBOI:    parse_put_smol_boi( a );    goto top;
        case GETBOI:        parse_get_boi<Int>( a );    goto top;
        case GETSMOLBOI:    parse_get_boi<Char>( a );   goto top;
        case GETSMOLBOIS:   parse_get_str_boi( a );     goto top;
        case YEETBOI:       parse_yeet_boi( a );        goto top;
        case BONKBOI:
        {
            a->nextTok();
            expect<SEMICOLON>( a );       
            mov_( "rax", 60 ); 
            mov_( "rdi", 0 ); 
            syscall_();
            goto top;
        }
        case RCURLY:                                    return;
        case COMMENT:
        {
            for( a->nextTok() ;; a->nextTok() ) 
            {
                Token x = a->peekTok();
                if( x.line_no != t.line_no ) 
                    goto top;
                if( x.token == ERROR ) 
                    return;
            }
        }
        default: illegal_statement( t );
    }
    #undef BY_ONE
    #undef next
}

void boic::parse_boi( Analyzer* const a ) 
{
    arraySet.clear();
    stackIdx.clear();
    params.clear();
    used.clear();
    typeSet.clear();
    avail = {};
    vCount = 0;
    aCount = 0;
    Token x = a->nextTok();

    if( x.token == THATBOI )
    {
        expect<LPAREN>( a );
        expect<ARGYBOIS>( a );
        expect<RPAREN>( a );
    }
    else if( x.token == AT )
    {
        x = expect<VAR>( a );
        expect<LPAREN>( a );
        if( types.contains( x.lexeme ) )
            parse_params<true>( x, a, types[ x.lexeme ] );
        else 
            funcSet[ x.lexeme ] = 
            parse_params<false>( x, a, types[ x.lexeme ] = make_shared<vector<size_t>>() );
        expect<RPAREN>( a );
    }
    else if( x.token == ERROR ) return;
    else error( a );

    if( a->peekTok().token == SEMICOLON )
    { a->nextTok(); return; }

    if( funcDef.contains( x.lexeme ) )
        redef_error( x );

    funcDef[ x.lexeme ] = true;

    insert_lbl( x.token == THATBOI? string( "main" ): x.lexeme );
    for(int i = 0; i < 8; ++i) avail.push( i );

    expect<LCURLY>( a );
    parse_decl( a );
    
    int vc = vCount + 1; // Account for previous return address.
    for( const string& s: params )
    { stackIdx[ s ] = vc++ << 3U; }

    parse_flow( a );
    
    insert_lbl( retn_( rCount++ ) );
    expect<RCURLY>( a );
    if(aCount) add_( "rsp", aCount );
    pop_( "rbp" );
    if(vCount) add_("rsp", vCount << 3U);
    ret_();
}

boic::boic( Analyzer* const a ) : 
rCount(0), sCount(0), dCount(0), vCount(0), eCount(0), lCount(0), aCount(0)
{
    for( ;; )
    {
        Token x = a->peekTok();
        if( size_t( x.token ) >= END )
            break;
        if( x.token == COMMENT )
        {
            a->nextTok();
            while( a->peekTok().line_no == x.line_no )
                a->nextTok();
            continue;
        }
        parse_boi( a );
    }

    if( !funcDef.contains( "@datboi" ) )
        missing_datboi_error( a );

    ofstream off( "boii.s" );
    off << 
            ".intel_syntax noprefix\n"
            ".section .data\n"
            ".lcomm LINE_BUFF, 100\n"
            ".lcomm BUFFER, 10000\n"
            ".section .text\n"
            ".global main\n";
    for(string s: x86)
        off << s << '\n';
}

int main(int argc, char** argv) {

    // if(argc != 2) {
    //     std::cout << "FAIL\n";
    //     return 0;
    // }

    Analyzer a;
    a.loadSpec("lex_spec.txt");
    a.nextInput(argv[1]);

    static boic oh_lawd_he_compilin( &a );
    
    return 0;
}