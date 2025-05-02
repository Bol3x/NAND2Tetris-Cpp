#include "..\Parser.cpp"
#include "HackVM.hpp"
#include <vector>

class parsing::HackVM::VMParser : public Parser
{
    public:
        /**
         * @brief Initializes the Parser for Virtual Machine Parsing
         * 
         * @param filename 
         */
        VMParser(const String& filename);

        /**
         * @brief Gets the current line 
         * 
         */
        void advanceLine();

        /**
         * @brief Gets the VM Command type of the current line
         * 
         * There are 3 types of commands performed by the VM language (As of Chapter 7):
         * 
         * Arithmetic - ALU operations between 1-2 stack values
         * 
         * Push - Push value into stack
         * 
         * Pop - Pop value out of stack
         * 
         * @return VMCommand 
         */
        VMCommand getCommandType();

        /**
         * @brief Gets the first argument in the current instruction
         * 
         * Typically this is the memory segment for stack instructions,
         * or the command itself for arithmetic instructions
         * 
         * @return String 
         */
        String getFirstArg();

        /**
         * @brief Gets the second argument in the current instruction
         * 
         * Only used in Stack instructions.
         * This is the index of the memory segment in a stack instruction.
         * 
         * @return String 
         */
        int getSecondArg();

        private:
            std::vector<String> words;
};