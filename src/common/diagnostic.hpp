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

struct Info {
    Stage stage;
    Position pos;
    std::string description;
};

class Diagnostics {
  public:
    // explicit Diagnostics(std::string file_name) : fname_(std::move(file_name)) {}
    explicit Diagnostics(const Source& program) : fname_(program.name) {}

    bool hasErrors() const;
    void report(Stage, Position, std::string msg);
    void print(std::ostream&) const;

  private:
    std::string fname_;
    std::vector<Info> items_;
};

struct MFYError : std::exception {
    Info item;
    explicit MFYError(Info i) : item(std::move(i)) {}
    const char* what() const noexcept override { return item.description.c_str(); }
};
