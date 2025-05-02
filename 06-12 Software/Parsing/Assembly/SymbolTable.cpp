#include "SymbolTable.hpp"

namespace parsing::HackAssembly{

    SymbolTable::SymbolTable()
    {
        table["SP"] = 0;
        table["LCL"] = 1;
        table["ARG"] = 2;
        table["THIS"] = 3;
        table["THAT"] = 4;
        
        String reg = "R";
        for(int i = 0; i < 16; i++)
        {
            String r = reg + std::to_string(i);
            table[r] = i;
        }

        table["SCREEN"] = 0x4000;
        table["KBD"] = 0x6000;
    }

    void SymbolTable::addVariableEntry(const String& symbol)
    {
        if (! containsEntry(symbol))
        {
            //overflow if beyond address 0x4000 (16384)
            if (variableAddress >= 0x4000) throw ("variable label address overflow");
            table.insert({symbol, variableAddress});

            incrementVariableAddress();
        }
    }

    void SymbolTable::addInstructionEntry(const String& symbol)
    {
        if (! containsEntry(symbol))
        {
            table.insert({symbol, instructionAddress});
        }
    }

    bool SymbolTable::containsEntry(const String& symbol)
    {
        if (table.find(symbol) == table.end()) return false;

        return true;
    }

    void SymbolTable::incrementVariableAddress()
    {
        variableAddress++;
    }
    
    void SymbolTable::incrementInstructionAddress()
    {
        instructionAddress++;
    }

    int SymbolTable::getAddress(const String& symbol)
    {
        if (! containsEntry(symbol)) return -1;

        return table.find(symbol)->second;
    }
}