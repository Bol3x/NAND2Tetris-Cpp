#include "Parser.hpp"
#include <regex>

namespace parsing{
    Parser::Parser(const String& filename) : 
    filename(filename),
    currLine("")
    {
        inputFile.open(filename);

        //load symbols on init
        loadSymbols();
        inputFile.close();
        inputFile.open(filename);
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

            //ignore comments
            if (line.length() == 0 || line.find("//") != line.npos)
            {
                continue;
            }
           
            //remove any space in the actual instruction
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end() );

            isInvalidLine = false;
            currLine = line;

        }while(isInvalidLine);
    }

    Command Parser::getCommandType()
    {
        if (currLine[0] == '@')
        {
            return Command::A_COMMAND;
        }
        else if (currLine[0] == '(')
        {
            return Command::L_COMMAND;
        }
        else if (currLine.length() == 0)
        {
            return Command::N_COMMAND;
        }
        else
        {
            return Command::C_COMMAND;
        }
    }

    String Parser::getSymbol()
    {
        Command type = getCommandType();

        if (type == Command::C_COMMAND)
        {
            throw ("C-Instructions are not permitted.");
        }

        if (type == Command::L_COMMAND)
        {
            return currLine.substr(1, currLine.length()-2);
        }
        else
        {
            String res = currLine.substr(1, currLine.length()-1);

            //if it's a symbol and not a number, find it in the SymbolTable
            if (! (res.at(0) >= 48 && res.at(0) <= 57))
            {
                if (table.containsEntry(res))
                {
                    int addr = table.getAddress(res);
                    return std::to_string(addr);
                }
                else    //if it's not in the table, add as a variable
                {
                    String symbol = res;
                    table.addVariableEntry(symbol);
                }
            }

            return res;
        }
    }

    void Parser::loadSymbols()
    {
        while (hasMoreCommands())
        {
            advanceLine();
            
            Command cmd = getCommandType();

            //if symbol is a label
            if (cmd == Command::L_COMMAND)
            {
                String symbol = currLine.substr(1, currLine.length()-2);
                table.addInstructionEntry(symbol);
            }
            else
            {
                table.incrementInstructionAddress();
            }
        }
    }
    
    String Parser::getDest()
    {
        if (getCommandType() != Command::C_COMMAND)
        {
            throw ("Only C-Instructions are permitted.");
        }

        size_t eq = currLine.find('=');
        if (eq == currLine.npos) return "";

        return currLine.substr(0, eq);
    }

    String Parser::getComp()
    {
        if (getCommandType() != Command::C_COMMAND)
        {
            throw ("Only C-Instructions are permitted.");
        }

        return currLine.substr(currLine.find('=')+1, currLine.find(';'));
    }

    String Parser::getJump()
    {
        if (getCommandType() != Command::C_COMMAND)
        {
            throw ("Only C-Instructions are permitted.");
        }

        size_t jmp = currLine.find(';');
        if (jmp == currLine.npos) return "";

        return currLine.substr(jmp+1);
    }

    void Parser::closeFile()
    {
        inputFile.close();
    }
}