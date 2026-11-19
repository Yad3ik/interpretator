#include "common/diagnostic.hpp"

// Миша this file for you!
//   StageName: Lex → "lexical", Syntax → "syntax", Semantic → "semantic", Runtime → "runtime";
//   report: добавить ErrInfo в items_;
//   print: std::stable_sort по pos, формат "<файл>:<строка>:<столбец>: <этап> error: <сообщение>" (implement.md §7.1).

std::string_view StageName(Stage stage) {
    if (stage == Stage::Lex) {
        return "lexical";
    } else if (stage == Stage::Syntax) {
        return "syntax";
    } else if (stage == Stage::Semantic) {
        return "semantic";
    } else if (stage == Stage::Runtime) {
        return "runtime";
    }
    return "";
}

bool Diagnostics::hasErrors() const {
    return !items_.empty();
}

void Diagnostics::report(Stage stage, Position pos, std::string desc) {
    items_.emplace_back(stage, pos, std::move(desc));
}

bool Cmp(const ErrInfo& a, const ErrInfo& b) {
    return a.pos < b.pos;
}

void Diagnostics::print(std::ostream& out) const {
    auto errors = items_;
    std::stable_sort(errors.begin(), errors.end(), Cmp);

    for (const auto& i : errors) {
        out << std::format("{}:{}:{}: {} error: {}\n",
               fname_, i.pos.line, i.pos.column, StageName(i.stage), i.description);
    }
}