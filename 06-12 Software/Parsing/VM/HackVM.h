#pragma once

#include "../Parsing.h"
#include <regex>

namespace parsing::HackVM
{
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

    class VMParser;
    class AssemblyGenerator;
    class VMTranslator;
}