#pragma once

#include <string>
#include <fstream>
#include <bitset>
#include <algorithm>

namespace parsing{
    using String = std::string;
    using InputFile = std::ifstream;
    using OutputFile = std::ofstream;

    enum class AssemblyCommand
    {
        A_COMMAND,
        C_COMMAND,
        L_COMMAND,
        N_COMMAND
    };

    enum class VMCommand
    {
        ARITHMETIC,
        PUSH,
        POP
    };

    class Parser;
    class AssemblyParser;
    class VMParser;
    class ParseMap;
    class FileGenerator;
    class BinaryGenerator;
    class SymbolTable;

    class HackTranslator;
}