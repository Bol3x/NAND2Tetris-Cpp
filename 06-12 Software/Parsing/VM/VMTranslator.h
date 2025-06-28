#include "VMParser.cpp"
#include "AssemblyGenerator.cpp"

class parsing::HackVM::VMTranslator
{
    public:
        VMTranslator(const String& filePath);

        void generateAssembly(const String& filePath, const String& fileName);

        void closeReader();

        void closeWriter();

    private:
        AssemblyGenerator writer;
        VMParser reader;
};
