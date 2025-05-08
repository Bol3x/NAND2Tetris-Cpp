#pragma once

#include "Parser.hpp"
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
        //ignore whitespaces (including whitespace lines)
        
        //TODO: update whitespace removal to catch syntax errors for multi-word symbols
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

            //remove leading and trailing spaces
            line = std::regex_replace(line, std::regex("^ +$"), "");

            //todo: ignore comments
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