#include "compiler.h"

template<bool DIV = false>
void boic::get_right_hand( shared_ptr<ASTNode> n, string& rreg, int& rid ) 
{
    if( n->right->t.token == NUM ) 
    {
        if(DIV == true)  
        {
            rreg = regTable[ rid = avail.top() ][ 0 ];
            int64_t i = parse_int( n->right->t );
            if( i > INT32_MAX || i < INT32_MIN )
                movabs_( rreg, to_string( i ) );
            else
                mov_( rreg, n->right->t.lexeme ); 
        }
        else 
        {
            int64_t i = parse_int( n->right->t );
            if( i > INT32_MAX || i < INT32_MIN )
            {
                rreg = regTable[ rid = avail.top() ][ 0 ];
                movabs_( rreg, to_string( i ) );
            } else
                rreg = n->right->t.lexeme; 
        }
    } 
    else if( n->right->t.token == VAR ) 
    {
        check_var( n->right->t );
        rreg = bqptr_( stackIdx[ n->right->t.lexeme ] ); 
    } 
    else 
    {
        rreg = regTable[ rid = n->right->reg ][ 0 ];
        avail.push( rid );
    }
}

template void boic::get_right_hand<true>( shared_ptr<ASTNode> n, string& rreg, int& rid );
template void boic::get_right_hand<false>( shared_ptr<ASTNode> n, string& rreg, int& rid );

void boic::compile_exp_( shared_ptr<ASTNode> n ) 
{
    if(!n) return;

    compile_exp_( n->left );
    compile_exp_( n->right );
    
    if( n->t.token != FUNC && n->left == nullptr ) return;

    if( !n->right && n->t.token != FUNC && n->left->t.token == NUM ) 
    {
        int64_t op1 = parse_int( n->left->t );
        if( n->t.token == ARGYBOIS ) 
        {
            check_var( n->t );
            check_arr( n->t );
            int lid = avail.top();
            mov_( "rax", bqptr_( stackIdx[ n->t.lexeme ] ) );
            ( this->*chr_mov_defs[ arraySet[ n->t.lexeme ] ] )
            ( 
                regTable[ lid ][ 0 ], 
                ( this->*chr_ptr_defs[ arraySet[ n->t.lexeme ] ] )( "rax", to_string( op1 << arraySet[ n->t.lexeme ] ) ) 
            );
            avail.pop();
            n->reg = lid;
            used.add( lid );
            return;
        }
        n->left = nullptr;
        Tokens x = Tokens( n->t.token );
        n->t.token = NUM;
        switch(x) 
        {
        case INC:        n->t.lexeme = to_string( ++op1 ); return;
        case DEC:        n->t.lexeme = to_string( --op1 ); return;
        case MINUS:      n->t.lexeme = to_string(  -op1 ); return;
        case NOT:        n->t.lexeme = to_string(  ~op1 ); return;
        case LOGIC_NOT:  n->t.lexeme = to_string(  !op1 ); return;
        case BOOLEANIZE: n->t.lexeme = to_string( !!op1 ); return;
        default: assert( false );
        }
    }

    if( n->right && n->left->t.token == NUM && n->right->t.token == NUM )
    {
        int64_t op1 = parse_int( n->left->t ), op2 = parse_int( n->right->t );
        switch(n->t.token) 
        {
        case PLUS:  n->t.lexeme = to_string(op1 + op2); break;
        case MINUS: n->t.lexeme = to_string(op1 - op2); break;
        case DIVIDE:
            if(op2 == 0) division_by_zero_error( n->t ); 
            n->t.lexeme = to_string(op1 / op2); break;
        case MOD:
            if(op2 == 0) division_by_zero_error( n->t );
            n->t.lexeme = to_string(op1 % op2); break;
        case MULT:          n->t.lexeme = to_string(op1 * op2);         break;
        case XOR:           n->t.lexeme = to_string(op1 ^ op2);         break;
        case LEFT_SHIFT:    n->t.lexeme = to_string(op1 << op2);        break;
        case RIGHT_SHIFT:   n->t.lexeme = to_string(op1 >> op2);        break;
        case OR:            n->t.lexeme = to_string(op1 | op2);         break;
        case AND:           n->t.lexeme = to_string(op1 & op2);         break;
        case LOGIC_OR:      n->t.lexeme = to_string((!!op1) | (!!op2)); break;
        case LOGIC_AND:     n->t.lexeme = to_string((!!op1) & (!!op2)); break;
        case LESS:          n->t.lexeme = to_string(op1 < op2);         break;
        case GREATER:       n->t.lexeme = to_string(op1 > op2);         break;
        case LESS_EQ:       n->t.lexeme = to_string(op1 <= op2);        break;
        case GREAT_EQ:      n->t.lexeme = to_string(op1 >= op2);        break;
        case EQUAL:         n->t.lexeme = to_string(op1 == op2);        break;
        case NOT_EQUAL:     n->t.lexeme = to_string(op1 != op2);        break;
        }
        n->t.token = NUM; n->left = n->right = nullptr; return;
    }

    string lreg, rreg; int lid, rid;

    if( n->t.token == FUNC )
    {
        lreg = regTable[ lid = avail.top() ][ 0 ]; avail.pop();
    }
    else if( n->t.token == DIVIDE || n->t.token == MOD )
    {
        if(n->left->t.token == VAR) 
        {
            check_var( n->left->t );
            lreg = regTable[ lid = avail.top() ][ 0 ]; avail.pop();
            mov_( "rax", bqptr_( stackIdx[ n->left->t.lexeme ] ) );
        } 
        else if(n->left->t.token == NUM) 
        {
            lreg = regTable[ lid = avail.top() ][ 0 ]; avail.pop();
            int64_t i = parse_int( n->left->t );
            if( i > INT32_MAX || i < INT32_MIN )
                movabs_( "rax", to_string( i ) );
            else
                mov_( "rax", n->left->t.lexeme );
        } 
        else 
        {
            lreg = regTable[ lid = n->left->reg ][ 0 ];
            mov_( "rax", lreg );
        }    
    }
    else
    {
        if(n->left->t.token == VAR) 
        {
            check_var( n->left->t );
            lreg = regTable[ lid = avail.top() ][ 0 ]; avail.pop();
            if( arraySet.contains( n->t.lexeme ) )
            {
                mov_( lreg, bqptr_( stackIdx[ n->left->t.lexeme ] ) );
                if( arraySet[ n->t.lexeme ] > 0 ) shl_( lreg, arraySet[ n->t.lexeme ] );
            }
            else
                mov_( lreg, bqptr_( stackIdx[ n->left->t.lexeme ] ) );
        } 
        else if(n->left->t.token == NUM) 
        {
            lreg = regTable[ lid = avail.top() ][ 0 ]; avail.pop();
            int64_t i = parse_int( n->left->t );
            if( i > INT32_MAX || i < INT32_MIN )
                movabs_( lreg, to_string( i ) );
            else
                mov_( lreg, n->left->t.lexeme ); 
        }
        else 
        {
            switch( n->t.token )
            {
            case INC:
            case DEC:
            case NOT:
            case LOGIC_NOT:
            case BOOLEANIZE:
                lreg = regTable[ lid = n->left->reg ][ 0 ];
                break;
            case MINUS:
                if( !n->right )
                {
                    lreg = regTable[ lid = n->left->reg ][ 0 ];
                    break;
                }
            default:
                lreg = regTable[ lid = n->left->reg ][ 0 ];
            }
            if( arraySet.contains( n->t.lexeme ) )
                if( arraySet[ n->t.lexeme ] > 0 ) shl_( lreg, arraySet[ n->t.lexeme ] );
        }
    }

    #define BY_ONE( func_ )                                                             \
    func_( lreg ); mov_( bqptr_( stackIdx[ n->left->t.lexeme ] ), lreg );
    #define BOOLEAN( func_ )                                                            \
    test_( lreg, lreg ); func_( regTable[ lid ][ 3 ] ); and_( lreg, 1 );
    #define DIV( str )                                                                  \
    get_right_hand<true>(n, rreg, rid); cqo_(); idiv_( rreg ); mov_( lreg, str );
    #define SHIFT( func_ )                                                              \
    get_right_hand(n, rreg, rid); mov_( "rcx", rreg ); func_( lreg, "rcx" );
    #define ARITH( func_ )                                                              \
    get_right_hand(n, rreg, rid); func_( lreg, rreg );
    #define CMP( func_ )                                                                \
    get_right_hand(n, rreg, rid); cmp_( lreg, rreg );                                   \
    func_( regTable[ lid ][ 3 ] ); and_( lreg, 1 );

    switch( n->t.token ) 
    {
    case INC:                   BY_ONE( inc_ )      break;
    case DEC:                   BY_ONE( dec_ )      break;
    case NOT:                   not_( lreg );       break;
    case LOGIC_NOT:             BOOLEAN( sete_  )   break;
    case BOOLEANIZE:            BOOLEAN( setne_ )   break;
    case MINUS:
        if( !n->right ) { neg_( lreg ); break; }
        ARITH( sub_ ) break;
    case DIVIDE:                DIV( "rax" )        break;
    case MOD:                   DIV( "rdx" )        break;
    case MULT:                  ARITH( imul_ )      break;
    case PLUS:                  ARITH( add_  )      break;
    case XOR:                   ARITH( xor_  )      break;
    case OR:  case LOGIC_OR:    ARITH( or_  )       break;
    case AND: case LOGIC_AND:   ARITH( and_ )       break;
    case LEFT_SHIFT:            SHIFT( shl_ )       break;
    case RIGHT_SHIFT:           SHIFT( shr_ )       break;
    case LESS:                  CMP( setl_  )       break;
    case GREATER:               CMP( setg_  )       break;
    case LESS_EQ:               CMP( setle_ )       break;
    case GREAT_EQ:              CMP( setge_ )       break;
    case EQUAL:                 CMP( sete_  )       break;
    case NOT_EQUAL:             CMP( setne_ )       break;
    case ARGYBOIS:
    {
        check_var( n->t );
        check_arr( n->t );
        mov_( "rax", bqptr_( stackIdx[ n->t.lexeme ] ) );
        ( this->*chr_mov_defs[ arraySet[ n->t.lexeme ] ] )
        ( 
            regTable[ lid ][ 0 ], 
            ( this->*chr_ptr_defs[ arraySet[ n->t.lexeme ] ] ) ( "rax", lreg ) 
        );
        break;
    }
    case FUNC: function_call_( n, lreg ); break;
    default: assert( false );
    }
    n->reg = lid;
    used.add( lid );

    #undef CMP
    #undef ARITH
    #undef SHIFT
    #undef DIV
    #undef BOOLEAN
    #undef BY_ONE
}