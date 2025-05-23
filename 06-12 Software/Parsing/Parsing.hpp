#pragma once

#include <string>
#include <fstream>
#include <bitset>
#include <algorithm>

namespace parsing{
    using String = std::string;
    using InputFile = std::ifstream;
    using OutputFile = std::ofstream;

    enum class VMCommand
    {
        VM_ARITHMETIC,
        VM_PUSH,
        VM_POP,
        VM_GOTO,
        VM_IFGOTO,
        VM_LABEL,
        VM_FUNCTION,
        VM_RETURN,
        VM_CALL,
        VM_NA
    };

    class Parser;
    class ParseMap;
    class FileGenerator;
}