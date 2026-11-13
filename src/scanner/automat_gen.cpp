#include "scanner/automat_gen.hpp"
#include "tokens/token.hpp"

namespace {
    constexpr std::string_view kAlpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    constexpr std::string_view kDigits = "0123456789";
    void InsertSpelling(DFA& dfa, std::string_view spelling, TokenKind kind) {
        if(spelling.empty()){
            return;
        }

        size_t state = 0;
        size_t i = 0;
        
        while ((dfa.transit(state, spelling[i])) != kNotAState){
            state = dfa.transit(state, spelling[i]);
            i++;

            if (i >= spelling.size()){
                break;
            }
        }

        for (; i < spelling.size(); ++i){
            int next = dfa.addState();
            dfa.addRule(state, std::string_view(&spelling[i], 1), next);
            state = next;
        }

        dfa.setFinish(state, kind);
    }
}

DFA BuildWordDFA() { // [a-zA-Z_][a-zA-Z_0-9]*
    DFA dfa;

    size_t finish = dfa.addState();
    dfa.setFinish(finish, TokenKind::Ident);

    dfa.addRule(0, kAlpha, finish);
    dfa.addRule(0, "_", finish);

    dfa.addRule(finish, kAlpha, finish);
    dfa.addRule(finish, kDigits, finish);
    dfa.addRule(finish, "_", finish);

    return dfa;
}

DFA BuildNumberDFA(){  //[0-9]+ "." [0-9]+ ([eE][+-]?[0-9]+)?  |  [0-9]+ [eE][+-]?[0-9]+
    DFA dfa;
    int enter = 0;
    int integer_finish = dfa.addState();
    int real_with_point = dfa.addState();
    int end_real = dfa.addState();
    int eE = dfa.addState();
    int plus_minus = dfa.addState();
    int finish_state = dfa.addState();

    dfa.addRule(enter, kDigits, integer_finish);
    dfa.addRule(integer_finish, kDigits, integer_finish);
    dfa.addRule(integer_finish, ".", real_with_point);
    dfa.addRule(real_with_point, kDigits, end_real);
    dfa.addRule(end_real, "eE", eE);
    dfa.addRule(end_real, kDigits, end_real);
    dfa.addRule(eE, "+-", plus_minus);
    dfa.addRule(plus_minus, kDigits, finish_state);
    dfa.addRule(eE, kDigits, finish_state);
    dfa.addRule(finish_state, kDigits, finish_state);
    dfa.addRule(integer_finish, "eE", eE);

    dfa.setFinish(integer_finish, TokenKind::IntLit);
    dfa.setFinish(end_real, TokenKind::RealLit);
    dfa.setFinish(finish_state, TokenKind::RealLit);   
    
    return dfa; 
}

DFA BuildOperatorDFA(){
    DFA dfa;
    for (auto& x : kTokenTable){
        if (x.text == "->" && x.kind == TokenKind::PtrArrow){
            continue;
        }
        if (x.text != "" && !std::isalpha(static_cast<unsigned char>(x.text[0]))) {
            InsertSpelling(dfa, x.text, x.kind);
        }
    }

    return dfa;
}

