#include "AssemblyParser.h"

namespace parsing::HackAssembly{

    AssemblyParser::AssemblyParser(const String& filename) :
    filename(filename)
    {
        openFile(filename);
        loadSymbols();
        closeFile();
        openFile(filename);
    }

    AssemblyCommand AssemblyParser::getCommandType()
    {
        if (currLine[0] == '@')
        {
            return AssemblyCommand::A_COMMAND;
        }
        else if (currLine[0] == '(')
        {
            return AssemblyCommand::L_COMMAND;
        }
        else if (currLine.length() == 0)
        {
            return AssemblyCommand::N_COMMAND;
        }
        else
        {
            return AssemblyCommand::C_COMMAND;
        }
    }

    String AssemblyParser::getSymbol()
    {
        AssemblyCommand type = getCommandType();

        if (type == AssemblyCommand::C_COMMAND)
        {
            throw ("C-Instructions are not permitted.");
        }

        if (type == AssemblyCommand::L_COMMAND)
        {
            return currLine.substr(1, currLine.length()-2);
        }
        else
        {
            String res = currLine.substr(1, currLine.length()-1);

            //if it's a symbol and not a number, find it in the SymbolTable and directly translate it into its address
            if (! (res.at(0) >= '0' && res.at(0) <= '9'))
            {
                if (table.containsEntry(res))
                {
                    int addr = table.getAddress(res);
                    return std::to_string(addr);
                }
                else    //if it's still not in the table, add as a new variable
                {
                    table.addVariableEntry(res);
                    return std::to_string(table.getAddress(res));
                }
            }

            return res;
        }
    }

    void AssemblyParser::loadSymbols()
    {
        while (hasMoreCommands())
        {
            advanceLine();
            
            AssemblyCommand cmd = getCommandType();

            //if symbol is a label
            if (cmd == AssemblyCommand::L_COMMAND)
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
    
    String AssemblyParser::getDest()
    {
        if (getCommandType() != AssemblyCommand::C_COMMAND)
        {
            throw ("Only C-Instructions are permitted.");
        }

        size_t eq = currLine.find('=');
        if (eq == currLine.npos) return "";

        return currLine.substr(0, eq);
    }

    String AssemblyParser::getComp()
    {
        if (getCommandType() != AssemblyCommand::C_COMMAND)
        {
            throw ("Only C-Instructions are permitted.");
        }

        return currLine.substr(currLine.find('=')+1, currLine.find(';'));
    }

    String AssemblyParser::getJump()
    {
        if (getCommandType() != AssemblyCommand::C_COMMAND)
        {
            throw ("Only C-Instructions are permitted.");
        }

        size_t jmp = currLine.find(';');
        if (jmp == currLine.npos) return "";

        return currLine.substr(jmp+1);
    }
}