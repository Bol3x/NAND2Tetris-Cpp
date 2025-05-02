#pragma once

#include "../Parsing.hpp"

namespace parsing::HackAssembly
{
    enum class AssemblyCommand
    {
        A_COMMAND,
        C_COMMAND,
        L_COMMAND,
        N_COMMAND
    };

    class AssemblyParser;
    class SymbolTable;
    class BinaryGenerator;
    class HackTranslator;
}