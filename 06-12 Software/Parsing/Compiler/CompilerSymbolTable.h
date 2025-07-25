#include "JackCompiler.h"

namespace parsing::JackCompiler
{
    class CompilerSymbolTable
    {
        public:
            /**
             * @brief Constructor for the compiler's symbol tables
             * 
             */
            CompilerSymbolTable();

            /**
             * @brief clears out the symbol table, resetting the indexes as well.
             * 
             * Must be called during every subroutine declaration being compiled
             * 
             */
            void clearTable();

            /**
             * @brief Adds a new symbol to the table, filling in its variable name, datatype, and memory type
             * 
             * @param varName - variable name identifier
             * @param varType - variable datatype
             * @param VMKind - type of memory the symbol is stored in
             */
            void addSymbol(const String& varName, const String& varType, const JackCompilerType& VMKind);

            /**
             * @brief Returns the number of symbols currently in the table matching the memory type
             * 
             * @param VMKind - type of memory the symbol is stored in
             * @return int
             */
            int getSymbolCountOf(const JackCompilerType& VMKind);

            /**
             * @brief Get the memory type of the symbol matching varName
             * 
             * @param varName - variable name identifier
             * @return JackCompilerType 
             */
            JackCompilerType getVMKindOf(const String& varName);

            /**
             * @brief Get the Data Type Of symbol matching varName
             * 
             * @param varName - variable name identifier
             * @return String 
             */
            String getDataTypeOf(const String& varName);

            /**
             * @brief Get the Index Of object in the symbol table matching varName
             * 
             * @param varName - variable name identifier
             * @return int 
             */
            int getIndexOf(const String& varName);

            /**
             * @brief Checks if variable exists in current table
             * 
             * @param varName - variable name identifier
             */
            bool isVarInTable(const String& varName);

        private:
            int staticIndex;
            int fieldIndex;
            int argIndex;
            int varIndex;

            std::unordered_map<String, symbolData> table;
    };
}