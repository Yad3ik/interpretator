#pragma once

#include "common/source.hpp"
#include <string_view>
#include <optional>
#include <cctype>

enum class TokenKind {
    #define TOKEN(name, repr, lexeme) name,
    #include "tokens/tokens.def"
    #undef TOKEN
};

struct TokenInfo {
    TokenKind kind;
    std::string_view display;
    std::string_view text;
}; 

struct Token {
    Range range;
    std::string lexeme;
    TokenKind kind;
    bool space_before = false; 
};

constexpr TokenInfo kTokenTable[] = {
    #define TOKEN(name, repr, lexeme) {TokenKind::name, repr, lexeme},
    #include "tokens.def"
    #undef TOKEN
};

std::string_view KindRepr(TokenKind);
std::string_view KindLexeme(TokenKind);
std::optional<TokenKind> KeywordKind(std::string_view); // является ли слово ключевым если да возвращаем его tokenkind, иначе nullopt

#define TOKEN(name, repr, lexeme) + 1
constexpr size_t kTokenKindCount = 0
    #include "tokens/tokens.def"
;
#undef TOKEN
