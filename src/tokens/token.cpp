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

 

std::optional<TokenKind> KeywordKind(std::string_view token_text){
    if (token_text == ""){
        return std::nullopt;
    }
    for (auto& x : kTokenTable){
        if (token_text == x.text && std::isalpha(x.text[0])) { 
            return x.kind;
        }
    } return std::nullopt;
}


