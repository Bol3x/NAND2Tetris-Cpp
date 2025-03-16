#include "JumpList.hpp"

namespace parsing{
    JumpList::JumpList()
    {
        jumps["JGT"] = "001";
        jumps["JEQ"] = "010";
        jumps["JGE"] = "011";
        jumps["JLT"] = "100";
        jumps["JNE"] = "101";
        jumps["JLE"] = "110";
        jumps["JMP"] = "111";
    }
    
    bool JumpList::isValidCode(const String& j)
    {
        return jumps.find(j) != jumps.end();
    }

    String JumpList::getCode(const String& j)
    {
        return jumps.find(j)->second;
    }
    
}