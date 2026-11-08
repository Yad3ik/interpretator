#pragma once

#include <string>
#include <cstddef>

struct Source {
    std::string name; // path or "<repl>"
    std::string text;
};

struct Position {
    size_t line;
    size_t column;

    auto operator<=>(const Position&) const = default;
};

struct Range {
    Position start;
    Position end;
};
