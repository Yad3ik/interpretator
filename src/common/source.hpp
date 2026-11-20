#pragma once

#include <string>
#include <cstddef>
#include <optional>
#include <filesystem>
#include <system_error>
#include <fstream>


struct Source {
    std::string name; // path or "<repl>"
    std::string text;
};

std::optional<Source> ReadSource(const std::string& path);

struct Position {
    size_t line;
    size_t column;

    auto operator<=>(const Position&) const = default;
};

struct Range {
    Position start;
    Position end;
};
