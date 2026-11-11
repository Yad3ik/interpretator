#pragma once

#include "tokens/token.hpp"
#include "scanner/cursor.hpp"

#include <optional>
#include <array>
#include <string_view>
#include <cstddef>
#include <vector>
#include <limits>

constexpr size_t kNotAState = std::numeric_limits<std::size_t>::max();

class DFA {
  public:
    DFA() {
        addState();
    }

    size_t addState(); // return created state number
    void addRule(size_t from, std::string_view chars, size_t to); // all symbols from chars: from -> to
    void setFinish(size_t state, TokenKind kind); // сделать состояние заверщающим
    size_t transit(size_t from, char c) const; // where we go or kNotAState
    std::optional<TokenKind> finish(size_t state) const; // what kind accept state. if state is not accepting -> nothing

  private:
    struct State {
        State() : accept(std::nullopt){
            next.fill(kNotAState);
        }

        std::array<std::size_t, 256> next;
        std::optional<TokenKind> accept;
    };

    std::vector<State> states_;
};

struct Match { // вид распознаной лексемы и где она кончилась
    TokenKind kind;
    Cursor end;
};

std::optional<Match> LongestMatch(const DFA&, const Cursor&);

