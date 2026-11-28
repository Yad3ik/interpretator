cmake_minimum_required(VERSION 3.20)
project(mfy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(mfy_common src/common/diagnostic.cpp src/common/source.cpp)
target_include_directories(mfy_common PUBLIC ${PROJECT_SOURCE_DIR}/src)
add_library(mfy_tokens src/tokens/token.cpp)
target_link_libraries(mfy_tokens PUBLIC mfy_common)
add_library(mfy_scanner src/scanner/cursor.cpp src/scanner/dfa.cpp src/scanner/automat_gen.cpp src/scanner/scanner.cpp)
target_link_libraries(mfy_scanner PUBLIC mfy_tokens)

add_executable(mfy src/main.cpp)
target_link_libraries(mfy PRIVATE mfy_scanner)
