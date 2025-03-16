#include "parsing.hpp"
#include "CommandList.cpp"
#include "JumpList.cpp"

class parsing::BinaryGenerator{
    public:

        /**
         * @brief Constructor for a BinaryGenerator object, outputting to the given filename
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


        /**
         * @brief Adds a line to the current output file stream
         * 
         * @param features - binary of command
         */
        void addLine(const String& binary);

        /**
         * @brief Closes outputfile.
         * 
         */
        void closeFile();
    
    private:
        OutputFile outputFile;
        CommandList compList;
        JumpList jumpList;
};