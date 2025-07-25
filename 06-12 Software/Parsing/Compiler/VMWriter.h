#include "JackCompiler.h"
#include "../FileGenerator.cpp"

namespace parsing::JackCompiler
{
    class VMWriter : public FileGenerator
    {
        public:
            void writePushInstruction(const JackVMSegment& segment, const int& index);

            void writePopInstruction(const JackVMSegment& segment, const int& index);

            void writeArithmeticInstruction(const JackVMCommand& command);

            void writeLabel(const String& label);

            void writeGotoInstruction(const String& label);

            void writeIfInstruction(const String& label);

            void writeCallInstruction(const String& name, const int& nArgs);

            void writeFunctionDeclaration(const String& name, const int& nVars);

            void writeReturnInstruction();
    };
}