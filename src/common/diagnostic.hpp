#pragma once

#include "source.hpp"

#include <vector>
#include <ostream>

enum class Stage {
    Lex,
    Syntax,
    Semantic,
    Runtime
};

const char* stageName(Stage stage); // human-read repr: Lex -> "lexical"

struct ErrInfo {
    Stage stage;
    Position pos;
    std::string description;
};

class Diagnostics {
  public:
    // explicit Diagnostics(std::string file_name) : fname_(std::move(file_name)) {}
    explicit Diagnostics(const Source& program) : fname_(program.name) {}

    bool hasErrors() const;
    void report(Stage, Position, std::string desc);
    void print(std::ostream&) const;

  private:
    std::string fname_;
    std::vector<ErrInfo> items_;
};

struct MFYError : std::exception {
    ErrInfo item;
    explicit MFYError(ErrInfo i) : item(std::move(i)) {}
    const char* what() const noexcept override { return item.description.c_str(); }
};
