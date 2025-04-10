#include "VMParser.hpp"
#include <sstream>

namespace parsing{

    VMParser::VMParser(const String& inputfile) : Parser(inputfile)
    {}

    void VMParser::advanceLine()
    {
        Parser::advanceLine();

        words.clear();
        std::istringstream iss(currLine);
        String item;
        while (std::getline(iss, item, ' '))
        {
            if (item.length() != 0)
            {
                words.push_back(item);
            }
        }
    }

    VMCommand VMParser::getCommandType()
    {
        if (currLine.length() == 0)
        {
            return VMCommand::VM_NA;
        }

        String instruction = "";
        if (words.size() != 0)
        {
            instruction = words[0];
        }

        if (instruction == "push")
        {
            return VMCommand::VM_PUSH;
        }

        if (instruction == "pop")
        {
            return VMCommand::VM_POP;
        }

        if (instruction == "label")
        {
            return VMCommand::VM_LABEL;
        }

        if (instruction == "goto")
        {
            return VMCommand::VM_GOTO;
        }

        if (instruction == "if-goto")
        {
            return VMCommand::VM_IFGOTO;
        }

        if (instruction == "function")
        {
            return VMCommand::VM_FUNCTION;
        }

        if (instruction == "label")
        {
            return VMCommand::VM_LABEL;
        }

        if (instruction == "call")
        {
            return VMCommand::VM_CALL;
        }

        if (instruction == "return")
        {
            return VMCommand::VM_RETURN;
        }
        

        return VMCommand::VM_ARITHMETIC;
    }

    String VMParser::getFirstArg()
    {

        if (getCommandType() == VMCommand::VM_ARITHMETIC)
            return words[0];

        return words[1];
    }

    int VMParser::getSecondArg()
    {
        return std::stoi(words[2]);
    }
};