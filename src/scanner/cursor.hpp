#pragma once

#include "common/source.hpp"

#include <string_view>

class Cursor {
  public:
    explicit Cursor(const Source& src) : text_(src.text) {}
    Cursor(Source&&) = delete;

    bool atEnd() const; // закончился ли текст
    char peek() const; // current byte. if we atEnd() -> '\0'
    char peekNext() const; // next byte
    void next();
    Position position() const;
    std::string_view textSince(const Cursor& start) const; //text since start to current cursor
    
  private:
    std::string_view text_;
    size_t offset_ = 0; // индекс байта в тексте программы
    Position pos_{1, 1};
};