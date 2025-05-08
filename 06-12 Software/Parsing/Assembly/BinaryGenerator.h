#include "HackAssembly.h"
#include "..\ParseMap.cpp"
#include "..\FileGenerator.cpp"
#include <memory>

class parsing::HackAssembly::BinaryGenerator : public FileGenerator
{
    public:

        /**
         * @brief Constructor for a BinaryGenerator object, generating binary opcodes for Hack assembly instructions.
         * 
         * @param filename
         */
        BinaryGenerator(const String& filename);

        /**
         * @brief Generates the binary equivalent of the C-Instruction
         * 
         * @param comp comp operation 
         * @param dest destination of result
         * @param jump jump condition
         * @return String binary representation of resulting instruction
         */
        String generateCompBinary(const String& comp, const String& dest, const String& jump);

        /**
         * @brief Generates the binary equivalent of the A-Instruction
         * 
         * @param symbol symbol or literal of value to be loaded
         * @return String binary representation of resulting instruction
         */
        String generateAddressBinary(const String& symbol);
    
    private:
        ParseMap compMap;
        ParseMap jumpMap;
};