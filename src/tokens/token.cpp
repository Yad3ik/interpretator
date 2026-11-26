#include "tokens/token.hpp"
#include <cctype>
#include <optional>
#include <string>

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

std::string StringToken(const Token & token){
    std::string answer = std::to_string(token.range.start.line) + ":" + std::to_string(token.range.start.column) + "-" + std::to_string(token.range.end.line) + ":" + std::to_string(token.range.end.column);
    answer += '\t';
    answer += KindRepr(token.kind);
    if (token.kind != TokenKind::End){
        answer += '\t';
        answer += token.lexeme;
    }
    return answer;
}

std::string StringCodeParse(const std::vector<Token>& tokens){
    std::string answer = "";
    for (auto& token : tokens){
        answer += (StringToken(token) + '\n');
    }
    return answer;
}

