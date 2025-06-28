#include "..\FileGenerator.cpp"
#include "HackVM.h"

class parsing::HackVM::AssemblyGenerator : public FileGenerator
{
    public:
        /**
         * @brief Initializes the FileGenerator for Assembly Instructions
         * 
         * @param filename 
         */
        AssemblyGenerator(const String& filename);
        
        /**
         * @brief Writes the equivalent assembly code for the resulting stack instruction
         * 
         * @param command - Push or Pop instruction
         * @param memorySegment - memory label
         * @param index - memory index at selected label
         */
        void writeStackInstruction(const VMCommand& command, const String& memorySegment, const int& index);

        /**
         * @brief Writes the equivalent assembly code for the resulting arithmetic instruction
         * 
         * @param operation - arithmetic operation
         */
        void writeArithmeticInstruction(const String& operation);


        /**
         * @brief Writes the equivalent assembly for declaring an instruction label
         * 
         * @param label - label string
         */
        void writeLabelInstruction(const String& label);

        /**
         * @brief Writes the jump to the label passed
         * 
         * @param label label to jump to
         */
        void writeGotoInstruction(const String& label);

        /**
         * @brief Writes a conditional jump to the label passed, checking the top of the stack as the decision variable
         * 
         * @param label label to jump to if condition is true
         */
        void writeIfGotoInstruction(const String& label);

        /**
         * @brief Writes the equivalent assembly for declaring the function with a set number of local variables allocated
         * 
         * @param fileName
         * @param functionName 
         * @param nLocals 
        */
        void writeFunctionDeclaration(const String& fileName, const String& functionName, const int& nLocals);

        /**
         * @brief Writes the equivalent assembly for calling a function with a set number of arguments passed
         * 
         * @param functionName 
         * @param nArgs 
         */
        void writeCallInstruction(const String& fileName, const String& functionName, const int& nArgs);

        /**
         * @brief Writes the equivalent assembly for returning a function's result to the caller and handing over program control
         * 
         */
        void writeReturnInstruction();

        void updateVMName(const String& name);

    private:
        String VMName;  //filename to identify static variables and function declarations
        int jumpIndex = 0;  //to avoid repeat labels
        int retIndex = 0;   //return index for function calls
};