#include "scanner/scanner.hpp"
#include "common/diagnostic.hpp"
#include "tokens/token.hpp"

#include <cassert>
#include <cctype>
#include <cstddef>

namespace {
    bool IsSpace(char ch) {
        return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
    }

    std::string modificators[] = {"c", "p", "cp", "pc", 
        "pp", "cpc", "ppc", "pcp", 
        "cpp", "pcpc", "cppc", "cpcp", 
        "pcpc", "cpcpc"};

    bool IsModword(std::string_view lex) {
       for(auto& mod : modificators) {
        if(lex == mod) return true;
       }

       return false;
    }
}

void Scanner::error(Position pos, std::string msg) const {
    throw MFYError({Stage::Lex, pos, std::move(msg)});
}

Match Scanner::scanWord() const {
    auto match = LongestMatch(word_dfa_, cursor_);

    

    assert(match.has_value() && "scanWord called not from [a-zA-Z_]");

    std::string_view lexeme = match->end.textSince(cursor_);
    auto keyword = KeywordKind(lexeme);

    if (keyword.has_value()) {
        match->kind = *keyword;
        return *match;
    }

    if (IsModword(lexeme)) {
        match->kind = TokenKind::Modword;
        return *match;
    }

    return *match;
}

Match Scanner::scanNumber() const {
    auto match = LongestMatch(number_dfa_, cursor_);

    assert(match.has_value() && "scanNumber called not from [0-9]");
    return *match;
}

Match Scanner::scanOperator(bool space_before) const {
    auto match = LongestMatch(operator_dfa_, cursor_);

    if (!match.has_value()) {
        std::string msg = "invalid symbol \'";
        msg.push_back(cursor_.peek());
        msg.push_back('\'');
        error(cursor_.position(), std::move(msg));
    }

    if (match->kind == TokenKind::CondArrow && !space_before) match->kind = TokenKind::PtrArrow;
    return *match;
}

Match Scanner::scanString() const {
    Cursor current = cursor_;
    current.next();
    // Cursor slash = current;

    while(!current.atEnd()) {
        char symbol = current.peek();
        if (symbol == '\n' || symbol == '\r') {
            error(cursor_.position(), "string literal without end symbol \'\"\'");
        }

        if (symbol == '\\') {
            symbol = current.peekNext();
            if (symbol == '\n' || symbol == '\r'  || symbol == '\0') {
                error(cursor_.position(), "string literal without end symbol \'\"\'");
            }

            char escape_sequence = symbol;
            if (escape_sequence != 'n' && escape_sequence != 'r' &&
                escape_sequence != 't' && escape_sequence != '\\' && escape_sequence != '"') {
                    error(current.position(), "incorrect escape sequence");
            }
            current.next();
            current.next();
            continue;
        }

        if (symbol == '"') {
            current.next();
            return {TokenKind::StrLit, current};
        }
        current.next();
    }

    error(cursor_.position(), "string literal without end symbol \'\"\'");
}

Token Scanner::makeToken(const Match& match, bool space_before, int offset = -1) const {
    Position end{match.end.position().line, match.end.position().column + offset};

    return {{cursor_.position(), end},
            std::string(match.end.textSince(cursor_)),
            match.kind, space_before};
}

bool Scanner::skipTrivia() {
    bool skip = false;
    while(!cursor_.atEnd()) {
        char symbol = cursor_.peek();

        if (IsSpace(symbol)) {
            cursor_.next();
            skip = true;
            continue;
        }

        if (symbol == '/' && cursor_.peekNext() == '/') {
            skip = true;
            size_t line = cursor_.position().line;
            while(!cursor_.atEnd() && cursor_.position().line == line) cursor_.next();
            continue;
        }

        if (symbol == '/' && cursor_.peekNext() == '*') {
            Position comment_start = cursor_.position();

            cursor_.next();
            cursor_.next();
            skip = true;

            bool has_end = false;

            while(!cursor_.atEnd())  {
                if (cursor_.peek() == '*' && cursor_.peekNext() == '/') {
                    cursor_.next();
                    cursor_.next();
                    has_end = true;

                    break;
                }

                cursor_.next();
            } 
            
            if (!has_end) {
                error(comment_start, "comment /* hasn't end");
            }

            continue;
        }

        break;
    }

    return skip;
}

Token Scanner::next() {
    bool spaces = skipTrivia();

    if (cursor_.atEnd()) {
        Match eof {TokenKind::End, cursor_};
        return makeToken(eof, false, 0);
    }

    char symbol = cursor_.peek();
    Match match{TokenKind::End, cursor_};

    if (std::isdigit(symbol)) {
        match = scanNumber();
    } else if (symbol == '\"') {
        match = scanString();
    }  else if (std::isalpha(symbol) || symbol == '_') {
        match = scanWord();
    } else {
        match = scanOperator(spaces);
    }

    auto token = makeToken(match, spaces);
    cursor_ = match.end;
    
    return token;
}

std::vector<Token> Tokenize(const Source& src) {
    Scanner scanner(src);

    std::vector<Token> tokens;

    Token token = scanner.next();
    while (token.kind != TokenKind::End) {
        if (token.kind == TokenKind::Modword) {
            if (tokens.empty() || tokens.back().kind != TokenKind::Colon)
                token.kind = TokenKind::Ident;
        }

        tokens.push_back(token);
        token = scanner.next();
    }

    tokens.push_back(token);
    return tokens;
}
