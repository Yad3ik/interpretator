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
// нужно чтобы создвть token_table в tokens/token.cpp по факту поле display не очень нужно но вдруг в будущем в таблтце нужно будет
// таблицу создал в cpp мб в будущем нажо будет перенсти

struct Token {
    Range range;
    std::string lexeme;
    TokenKind kind;
    bool space_before = false; 
};

std::string_view KindRepr(TokenKind);
std::string_view KindLexeme(TokenKind);
std::optional<TokenKind> KeywordKind(std::string_view); // является ли слово ключевым если да возвращаем его tokenkind, иначе nullopt

#define TOKEN(name, repr, lexeme) + 1
constexpr size_t kTokenKindCount = 0
    #include "tokens/tokens.def"
;
#undef TOKEN
