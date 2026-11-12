#pragma once

#include "scanner/automat_gen.hpp"
#include <vector>

class Scanner {
  public:
    explicit Scanner(const Source& src) : cursor_(src), word_dfa_(BuildWordDFA()), 
        number_dfa_(BuildNumberDFA()), operator_dfa_(BuildOperatorDFA()) {
    }
    Scanner(Source&&) = delete;

    Token next();

  private:

    bool skipTrivia();

    Match scanWord() const;
    Match scanNumber() const;
    Match scanString() const;
    Match scanOperator(bool space_before) const;

    Token makeToken(const Match& match, bool space_before) const;

    [[noreturn]] void error(Position pos, std::string msg) const;

    Cursor cursor_;
    DFA word_dfa_;
    DFA number_dfa_;
    DFA operator_dfa_;
};

std::vector<Token> Tokenize(const Source&);