#include "..\FileGenerator.cpp"
#include "..\ParseMap.cpp"

class parsing::AssemblyGenerator : public FileGenerator
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

    private:
        ParseMap ops;
        String VMName;
};