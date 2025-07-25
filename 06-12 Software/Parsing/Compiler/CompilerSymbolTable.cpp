#include "CompilerSymbolTable.h"

namespace parsing::JackCompiler
{
    CompilerSymbolTable::CompilerSymbolTable()
    {
        argIndex = 0;
        varIndex = 0;
        fieldIndex = 0;
        staticIndex = 0;
    }

    void CompilerSymbolTable::clearTable()
    {
        argIndex = 0;
        varIndex = 0;
        fieldIndex = 0;
        staticIndex = 0;
        table.clear();
    }

    void CompilerSymbolTable::addSymbol(const String& varName, const String& varType, const JackCompilerType& VMKind)
    {
        if (table.find(varName) == table.end())
        {
            symbolData data;
            data.datatype = varType;
            data.kind = VMKind;

            switch (VMKind)
            {
                case JackCompilerType::JC_STATIC:
                    data.index = staticIndex++;
                    break;
                    
                case JackCompilerType::JC_FIELD:
                    data.index = fieldIndex++;
                    break;
                
                case JackCompilerType::JC_ARG:
                    data.index = argIndex++;
                    break;

                case JackCompilerType::JC_VAR:
                    data.index = varIndex++;
                    break;
            }

            table[varName] = data;
        }
    }

    int CompilerSymbolTable::getSymbolCountOf(const JackCompilerType& VMKind)
    {
        switch (VMKind)
        {
            case JackCompilerType::JC_STATIC:
                return staticIndex;
                
            case JackCompilerType::JC_FIELD:
                return fieldIndex;
            
            case JackCompilerType::JC_ARG:
                return argIndex;

            case JackCompilerType::JC_VAR:
                return varIndex;
            
            default:
                return 0;
        }
    }

    JackCompilerType CompilerSymbolTable::getVMKindOf(const String& varName)
    {
        if (table.find(varName) != table.end())
        {
            return table[varName].kind;
        }

        return JackCompilerType::JC_NONE;
    }

    String CompilerSymbolTable::getDataTypeOf(const String& varName)
    {
        if (table.find(varName) != table.end())
        {
            return table[varName].datatype;
        }

        return "";
    }

    int CompilerSymbolTable::getIndexOf(const String& varName)
    {
        if (table.find(varName) != table.end())
        {
            return table[varName].index;
        }

        return -1;
    }

    bool CompilerSymbolTable::isVarInTable(const String& varName)
    {
        if (table.find(varName) != table.end())
        {
            return true;
        }

        return false;
    }

}