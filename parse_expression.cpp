#include <string>
#include <iostream>
#include <vector>
#include "compiler.h"

using std::shared_ptr;
using std::make_shared;
using std::to_string;

namespace 
{

shared_ptr<ASTNode> parseCmp(Analyzer* a);

template<Tokens T>
shared_ptr<ASTNode> parse(Analyzer* const a) 
{
    shared_ptr<ASTNode> parent;
    shared_ptr<ASTNode> left = parse<Tokens(T + 1)>(a);
    if(a->peekTok().token != T)
        return left;
    for(;;) 
    {
        Token t = a->nextTok();
        parent = make_shared<ASTNode>(t);
        parent->left = left;
        parent->right = parse<Tokens(T + 1)>(a);
        left = parent;
        if(a->peekTok().token != T)
            return parent;
    }
    return parent;
}

template<> shared_ptr<ASTNode> 
parse<EQUAL>(Analyzer* const a) {
    shared_ptr<ASTNode> parent;
    shared_ptr<ASTNode> left = parseCmp(a);
    Token t = a->peekTok();
    if(t.token != EQUAL && t.token != NOT_EQUAL)
        return left;
    for(;;) 
    {
        t = a->nextTok();
        parent = make_shared<ASTNode>(t);
        parent->left = left;
        parent->right = parseCmp(a);
        left = parent;
        t = a->peekTok();
        if(t.token != EQUAL && t.token != NOT_EQUAL) 
            return parent;
    }
    return parent;
}


template shared_ptr<ASTNode> parse<OR>(Analyzer*);
template shared_ptr<ASTNode> parse<XOR>(Analyzer*);
template shared_ptr<ASTNode> parse<AND>(Analyzer*);
template shared_ptr<ASTNode> parse<RIGHT_SHIFT>(Analyzer*);
template shared_ptr<ASTNode> parse<LEFT_SHIFT>(Analyzer*);
template shared_ptr<ASTNode> parse<MINUS>(Analyzer*);
template shared_ptr<ASTNode> parse<PLUS>(Analyzer*);
template shared_ptr<ASTNode> parse<MOD>(Analyzer*);
template shared_ptr<ASTNode> parse<DIVIDE>(Analyzer*);
template shared_ptr<ASTNode> parse<MULT>(Analyzer*);
template shared_ptr<ASTNode> parse<LOGIC_OR>(Analyzer* const a);
template shared_ptr<ASTNode> parse<LOGIC_AND>(Analyzer* const a);

shared_ptr<ASTNode> parse_funcy_boi(Token& t, Analyzer* const a)
{
    a->nextTok();
    t.token = FUNC;
    shared_ptr<ASTNode> unary = 
        make_shared<ASTNode>(t);
    if( a->peekTok().token == RPAREN )
    {
        a->nextTok();
        return unary;
    }
    unary->pCount = 0;
    for( shared_ptr<ASTNode> u = unary;; ) 
    {
        ++unary->pCount;
        u->middle = parse<LOGIC_OR>(a);
        u = u->middle;
        if(a->peekTok().token != COMMA)
            break;
        a->nextTok();
    }
    expect<RPAREN>(a);
    return unary;
}

template<> shared_ptr<ASTNode> parse<LOGIC_NOT>(Analyzer* const a) {
    Token t = a->nextTok();
    switch(t.token) 
    {
        case LPAREN: 
        {
            shared_ptr<ASTNode> r = parse<LOGIC_OR>(a);
            expect<RPAREN>(a);
            return r;
        }
        case INC:
        case DEC: 
        {
            shared_ptr<ASTNode> x = make_shared<ASTNode>(t);
            x->left = make_shared<ASTNode>(expect<VAR>(a));
            return x;
        }
        case NOT:
        case LOGIC_NOT:
        case BOOLEANIZE:
        case MINUS: 
        {
            shared_ptr<ASTNode> unary = 
                make_shared<ASTNode>(t);
            t = a->nextTok();
            if( t.token == VAR )
            {
                if( a->peekTok().token == LPAREN )
                {
                    unary->left = parse_funcy_boi( t, a );   
                    return unary; 
                }
                unary->left =
                    make_shared<ASTNode>(t);
                return unary;
            }
            if(t.token == NUM) 
            {
                unary->left =
                    make_shared<ASTNode>(t);
                return unary;
            } 
            if(t.token == LPAREN) 
            {
                unary->left = parse<LOGIC_OR>(a);
                expect<RPAREN>(a);
                return unary;
            } 
            error(a);
        }
        break;
        case VAR:
            if(a->peekTok().token == LBRACK) 
            {
                t.token = ARGYBOIS;
                goto argy;
            } 
            else if( a->peekTok().token == LPAREN )
            {
                return parse_funcy_boi( t, a );
            }
        case NUM:
            return make_shared<ASTNode>(t);
        case CHAR:
            t.lexeme = to_string(int(t.lexeme[1]));
            t.token = NUM;
            return make_shared<ASTNode>(t);
        case ARGYBOIS: 
        {
            if(a->peekTok().token == LBRACK) 
            {
            argy:
                shared_ptr<ASTNode> unary = 
                    make_shared<ASTNode>(t);
                expect<LBRACK>(a);
                unary->left = parse<LOGIC_OR>(a);
                expect<RBRACK>(a);
                return unary;
            } 
            else error(a);
        }
        default:
            error(a);
    }
    return nullptr;
}

shared_ptr<ASTNode> parseCmp(Analyzer* a) 
{
    shared_ptr<ASTNode> parent;
    shared_ptr<ASTNode> left = parse<OR>(a);
    Token t = a->peekTok();
    switch(t.token) 
    {
    case LESS:    case LESS_EQ:
    case GREATER: case GREAT_EQ:
        break;
    default: return left;
    }
    for(;;) 
    {
        t = a->nextTok();
        parent = make_shared<ASTNode>(t);
        parent->left = left;
        parent->right = parse<OR>(a);
        left = parent;
        t = a->peekTok();
        switch(t.token) 
        {
        case LESS:    case LESS_EQ:
        case GREATER: case GREAT_EQ:
            break;
        default: return parent;
        }
    }
    return parent;
}

}

shared_ptr<ASTNode> parse_exp_(Analyzer* const a)
{ return parse<LOGIC_OR>(a); }