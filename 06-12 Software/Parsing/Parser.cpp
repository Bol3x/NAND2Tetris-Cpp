#pragma once

#include "Parser.h"
#include <regex>

namespace parsing{
    Parser::Parser(const String& filename) : 
    currLine("")
    {
        inputFile.open(filename);
    }

    Parser::~Parser()
    {
        inputFile.close();
    }

    String Parser::getLine()
    {
        return currLine;
    }

    bool Parser::hasMoreCommands()
    {
        return inputFile.eof() ? false : true;
    }

    void Parser::advanceLine()
    {   
        bool isInvalidLine = true;
        String line;
        do
        {
            if (! hasMoreCommands()) 
            {
                currLine = "";
                return;
            }
            std::getline(inputFile, line);

            //remove leading and trailing spaces and comments
            line = std::regex_replace(line, std::regex("^[\t ]+| *//.*"), "");

            if (line.length() == 0)
            {
                continue;
            }

            isInvalidLine = false;
            currLine = line;

        }while(isInvalidLine);
    }

    void Parser::closeFile()
    {
        inputFile.close();
    }
}