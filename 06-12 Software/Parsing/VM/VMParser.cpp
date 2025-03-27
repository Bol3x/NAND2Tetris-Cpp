#include "VMParser.hpp"
#include <sstream>

namespace parsing{

    VMParser::VMParser(const String& inputfile) : Parser(inputfile)
    {}

    void VMParser::advanceLine()
    {
        Parser::advanceLine();

        std::cout << "Extracting words from: " << currLine << std::endl;
        

        words.clear();
        std::istringstream iss(currLine);
        String item;
        while (std::getline(iss, item, ' '))
        {
            if (item.length() != 0)
            {
                std::cout << "word: " << item << std::endl;
                
                words.push_back(item);
            }
        }
    }

    VMCommand VMParser::getCommandType()
    {
        if (currLine.length() == 0)
        {
            return VMCommand::NA;
        }

        String instruction = "";
        if (words.size() != 0)
        {
            instruction = words[0];
        }

        if (instruction == "push")
        {
            return VMCommand::PUSH;
        }

        if (instruction == "pop")
        {
            return VMCommand::POP;
        }

        return VMCommand::ARITHMETIC;
    }

    String VMParser::getFirstArg()
    {

        if (getCommandType() == VMCommand::ARITHMETIC)
            return words[0];

        return words[1];
    }

    int VMParser::getSecondArg()
    {
        return std::stoi(words[2]);
    }
};