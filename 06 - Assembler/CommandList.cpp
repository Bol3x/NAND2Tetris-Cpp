#include "CommandList.hpp"

namespace parsing{
    
    CommandList::CommandList()
    {
        comp["0"]   = "101010";
        comp["1"]   = "111111";
        comp["-1"]  = "111010";
        comp["D"]   = "001100";
        comp["A"]   = "110000"; comp["M"]   = "110000";
        comp["!D"]  = "001111";
        comp["!A"]  = "110001"; comp["!M"]  = "110001";
        comp["-D"]  = "001111";
        comp["-A"]  = "110011"; comp["-M"]  = "110011";
        comp["D+1"] = "011111";
        comp["A+1"] = "110111"; comp["M+1"] = "110111";
        comp["D-1"] = "001110";
        comp["A-1"] = "110010"; comp["M-1"] = "110010";
        comp["D+A"] = "000010"; comp["D+M"] = "000010";
        comp["D-A"] = "010011"; comp["D-M"] = "010011";
        comp["A-D"] = "000111"; comp["M-D"] = "000111";
        comp["D&A"] = "000000"; comp["D&M"] = "000000";
        comp["D|A"] = "010101"; comp["D|M"] = "010101";
    }

    bool CommandList::isValidCode(const String& c)
    {
        return comp.find(c) != comp.end();
    }

    String CommandList::getCode(const String& c)
    {
        return comp[c];
    }
}