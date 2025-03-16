#pragma once

#include <string>
#include <fstream>
#include <bitset>
#include <algorithm>

namespace parsing{
    using String = std::string;
    using InputFile = std::ifstream;
    using OutputFile = std::ofstream;

    enum class Command
    {
        A_COMMAND,
        C_COMMAND,
        L_COMMAND,
        N_COMMAND
    };

    class Parser;
    class CommandList;
    class JumpList;
    class BinaryGenerator;
    class SymbolTable;
}