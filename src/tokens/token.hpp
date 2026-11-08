#pragma once

#include "common/source.hpp"
#include <string_view>

enum class TokenKind {
    #define TOKEN(name, repr, lexeme) name,
    #include "tokens/tokens.def"
    #undef TOKEN
};

struct Token {
    Position pos;
    std::string lexeme;
    TokenKind kind;
    bool space_before;
};

std::string_view KindRepr(TokenKind);
std::string_view KindLexeme(TokenKind);
bool IsKeyword(std::string_view); // является ли слово ключевым

#define TOKEN(name, repr, lexeme) + 1
constexpr size_t kTokenKindCount = 0
    #include "tokens/tokens.def"
;
#undef TOKEN
