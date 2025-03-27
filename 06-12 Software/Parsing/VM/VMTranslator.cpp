#include "VMTranslator.hpp"
#include <iostream>

using namespace std;

namespace parsing
{
    VMTranslator::VMTranslator(const String& in, const String& out) :
    reader(in), writer(out)
    {}

    void VMTranslator::generateAssembly()
    {
        while (reader.hasMoreCommands())
        {
            reader.advanceLine();

            String instruction = reader.getLine();

            VMCommand command = reader.getCommandType();

            if (command == VMCommand::ARITHMETIC)
            {
                String arg = reader.getFirstArg();
                cout << "arg: " << arg << endl;
                writer.writeArithmeticInstruction(arg);
            }
            else if (command != VMCommand::NA)
            {
                String segment = reader.getFirstArg();
                int index = reader.getSecondArg();

                cout << "Segment: " << segment << endl;
                cout << "index: " << index << endl;

                writer.writeStackInstruction(command, segment, index);
            }
        }
    }

    void VMTranslator::closeFiles()
    {
        reader.closeFile();
        writer.closeFile();
    }
}