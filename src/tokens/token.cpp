#include "tokens/token.hpp"
#include <cctype>
#include <optional>

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
constexpr TokenInfo token_table[] = {
    #define TOKEN(name, repr, lexeme) {TokenKind::name, repr, lexeme},
    #include "tokens.def"
    #undef TOKEN
}; 

std::optional<TokenKind> KeywordKind(std::string_view token_text){
    if (token_text == ""){
        return std::nullopt;
    }
    for (auto& x : token_table){
        if (token_text == x.text && std::isalpha(x.text[0])) { // проверяет что первый симвл это именно буква [a-zA-Z]
            return x.kind;
        }
    } return std::nullopt;
}


