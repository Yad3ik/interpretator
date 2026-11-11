#include "scanner/cursor.hpp"

#include <cassert>

namespace {
    constexpr int kMask = 0b11000000;
    constexpr int kBits = 0b10000000; // байт продолжения для не ascii символов в utf-8

    bool IsContinueByte(unsigned char c) {
        return (c & kMask) == kBits;
    }
}

bool Cursor::atEnd() const {
    return (offset_ >= text_.size());
}

char Cursor::peek() const {
    if (atEnd()) return '\0';

    return text_[offset_];
}

char Cursor::peekNext() const {
    if (offset_ + 1 >= text_.size()) return '\0';

    return text_[offset_ + 1];
}

void Cursor::next() {
    if (atEnd()) return;

    char current = peek();
    ++offset_;

    if (IsContinueByte(current)) return;
    if (current == '\r' && peek() == '\n') return;

    if (current == '\n') {
        ++pos_.line;
        pos_.column = 1;
    } else {
        ++pos_.column;
    }
}

Position Cursor::position() const {
    return pos_;
}

std::string_view Cursor::textSince(const Cursor& start) const {
    assert(start.offset_ <= offset_);
    return text_.substr(start.offset_, offset_ - start.offset_);
}