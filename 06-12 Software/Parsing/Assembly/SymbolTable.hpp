#include "..\Parsing.hpp"
#include <unordered_map>

class parsing::SymbolTable{
    public:

        /**
         * @brief Initializes defailt symbols (R0-R32, SP, etc.)
         * 
         */
        SymbolTable();

        /**
         * @brief Adds a new symbol to the table
         * 
         * @param symbol
         */
        void addVariableEntry(const String& symbol);

        /**
         * @brief 
         * 
         */
        void addInstructionEntry(const String& symbol);

        /**
         * @brief Checks if the table contains the symbol
         * 
         * @param symbol 
         * @return boolean
         */
        bool containsEntry(const String& symbol);

        /**
         * @brief Increments the address to the next available position.
         * 
         */
        void incrementVariableAddress();

        /**
         * @brief Increments the instruction address
         * 
         */
        void incrementInstructionAddress();

        /**
         * @brief Gets the address represented by the symbol
         * 
         * @param symbol 
         * @return int of address
         */
        int getAddress(const String& symbol);

    private:
        std::unordered_map<String, int> table;
        int instructionAddress = 0;
        int variableAddress = 0x400;
};