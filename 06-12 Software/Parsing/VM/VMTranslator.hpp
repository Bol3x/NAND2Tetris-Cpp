#include "VMParser.cpp"
#include "AssemblyGenerator.cpp"

class parsing::VMTranslator
{
    public:
        VMTranslator(const String& in, const String& out);
        
        void generateAssembly();

        void closeFiles();

    private:
        VMParser reader;
        AssemblyGenerator writer;
};
