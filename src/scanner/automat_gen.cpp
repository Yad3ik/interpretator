#include "scanner/automat_gen.hpp"
#include "tokens/token.hpp"

namespace {
    constexpr std::string_view kAlpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    constexpr std::string_view kNumbers = "0123456789";
}

DFA BuildWordDFA() { // [a-zA-Z_][a-zA-Z_0-9]*
    DFA dfa;

    size_t finish = dfa.addState();
    dfa.setFinish(finish, TokenKind::Ident);

    dfa.addRule(0, kAlpha, finish);
    dfa.addRule(0, "-", finish);

    dfa.addRule(finish, kAlpha, finish);
    dfa.addRule(finish, kNumbers, finish);
    dfa.addRule(finish, "_", finish);

    return dfa;
}

