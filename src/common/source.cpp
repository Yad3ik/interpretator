#include "common/source.hpp"

#include <filesystem>
#include <system_error>
#include <fstream>

std::optional<Source> ReadSource(const std::string& path) {
    std::error_code ec;
    bool is_regular = std::filesystem::is_regular_file(path, ec);
    if(!is_regular or ec) {
        return std::nullopt;
    }

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return std::nullopt;
    }

    Source source;
    char buffer[4096];
    while(file.read(buffer, sizeof buffer)) {
        source.text.append(buffer, file.gcount());
    }

    if (file.eof() and !file.bad()) {
        source.text.append(buffer, file.gcount());
        source.name = path;

        return source;
    } else {
        return std::nullopt;
    }
}