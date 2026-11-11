#include "scanner/dfa.hpp"

#include <cassert>

size_t DFA::addState() {
    states_.push_back({});
    return states_.size() - 1;
}

void DFA::addRule(size_t from, std::string_view chars, size_t to) {
    assert(from < states_.size() && to < states_.size());
    State& state = states_[from];

    for(char c : chars) {
        state.next[static_cast<unsigned char>(c)] = to; 
    }
}

void DFA::setFinish(size_t state, TokenKind kind) {
    assert(state != 0 && state < states_.size());
    states_[state].accept = kind;
}

size_t DFA::transit(size_t from, char c) const {
    return states_[from].next[static_cast<unsigned char>(c)];
}

std::optional<TokenKind> DFA::accept(size_t state) const {
    return states_[state].accept;
}

std::optional<Match> LongestMatch(const DFA& dfa, const Cursor& start) {
    Cursor current = start;
    std::optional<Match> match;
    size_t state = 0;

    while(!current.atEnd()) {
        char symbol = current.peek();
        state = dfa.transit(state, symbol);

        if (state == kNotAState) break;
        current.next();
        
        auto kind = dfa.accept(state);
        if (kind.has_value()) {
            match = Match{kind.value(), current};
        }
        
    }

    return match;
}
