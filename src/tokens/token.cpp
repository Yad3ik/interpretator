#include "tokens/token.hpp"

std::string_view KindRepr(TokenKind kind) {
    #define TOKEN(name, repr, lexeme) case TokenKind::name: return repr;
    switch (kind) {
        #include "tokens/tokens.def"
    }
    #undef TOKEN
    return "???";
}

std::string_view KindLexeme(TokenKind kind) {
    #define TOKEN(name, repr, lexeme) case TokenKind::name: return lexeme;
    switch (kind) {
        #include "tokens/tokens.def"
    }
    #undef TOKEN
    return "???";
}

// федя тут дореализуй
// KeywordKind — статическая таблица из tokens.def,
// только строки, где написание начинается с буквы (README §2.5) - это и есть ключевые слова.