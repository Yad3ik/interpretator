#pragma once

#include "scanner/dfa.hpp"


DFA BuildWordDFA(); // {modword, keyword, ident} -> ident only
DFA BuildNumberDFA();  // real, int
DFA BuildOperatorDFA(); // operators only
