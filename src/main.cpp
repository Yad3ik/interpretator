#include "common/source.hpp"
#include "scanner/scanner.hpp"
#include "tokens/token.hpp"
#include "common/diagnostic.hpp"
#include <iostream>
#include <optional>

enum class ExitCode { 
    Ok = 0, 
    CompileError = 1, 
    RuntimeError = 2, 
    UsageError = 3 
};

void PrintUsage(std::ostream& out) {
  out << "использование: mfy tokens <файл>\n";
}

int RunTokens(const Source& source, std::ostream& out){
    std::vector<Token> tokens;
    try{
        tokens = Tokenize(source);
    } catch(const MFYError& err) {
        Diagnostics diag(source);
        diag.report(err.item.stage, err.item.pos, err.item.description);
        diag.print(std::cerr);
        return static_cast<int>(ExitCode::CompileError);
    }
    out << StringCodeParse(tokens);
    return static_cast<int>(ExitCode::Ok);
}

int main(int argc, char** argv){
    if (argc < 2){
        PrintUsage(std::cerr);
        return static_cast<int>(ExitCode::UsageError);
    } else if(argc == 2 and std::string_view(argv[1]) == "tokens"){
        std::cerr << "команде tokens нужен файл" << "\n";
        PrintUsage(std::cerr);
        return static_cast<int>(ExitCode::UsageError);
    } else if (argc > 3 and std::string_view(argv[1]) == "tokens"){
        PrintUsage(std::cerr);
        return static_cast<int>(ExitCode::UsageError);
    } else if(std::string_view(argv[1]) != "tokens"){
        std::cerr << "неизвестная команда" << " '" << argv[1] <<"'" << "\n";
        PrintUsage(std::cerr);
        return static_cast<int>(ExitCode::UsageError);
    } else if (argc == 3 and  std::string_view(argv[1]) == "tokens"){
        auto source = ReadSource(argv[2]);
        if (source == std::nullopt){
            std::cerr << "не удалось открыть файл '" << argv[2] << "'" << "\n";
            return static_cast<int>(ExitCode::UsageError);
        } else {
            return RunTokens(*source, std::cout);
        }
    }



}