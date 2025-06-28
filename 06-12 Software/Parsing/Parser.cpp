#pragma once

#include "Parser.h"
#include <regex>

namespace parsing{
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
        if (inputFile.is_open()){
            return inputFile.eof() ? false : true;
        }
        return false;
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

    void Parser::openFile(const String& file)
    {
        inputFile.open(file);
    }

    void Parser::closeFile()
    {
        inputFile.close();
    }
}