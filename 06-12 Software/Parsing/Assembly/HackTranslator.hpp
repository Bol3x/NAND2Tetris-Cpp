#include "AssemblyParser.cpp"
#include "BinaryGenerator.cpp"

class parsing::HackTranslator
{
    public:
        /**
         * @brief Constructs a HackTranslator to read Hack assembly code and translates it into its equivalent opcodes.
         * 
         * @param in 
         * @param out 
         */
        HackTranslator(const String& in, const String& out);

        /**
         * @brief Generates the opcode equivalent of the input file's assembly code.
         * 
         */
        void generateOpcode();

        /**
         * @brief Closes the respective files for the parser and generator
         * 
         */
        void closeFiles();

    private:
        AssemblyParser reader;
        BinaryGenerator writer;
};