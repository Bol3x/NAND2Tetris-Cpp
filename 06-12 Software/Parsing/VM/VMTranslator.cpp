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

            String firstArg = "";
            int secondArg = 0;
            switch (command)
            {
                case VMCommand::VM_ARITHMETIC:
                    firstArg = reader.getFirstArg();
                    cout << "arg: " << firstArg << endl;
                    writer.writeArithmeticInstruction(firstArg);
                    break;
                
                case VMCommand::VM_POP:
                case VMCommand::VM_PUSH:
                    firstArg  = reader.getFirstArg();
                    secondArg = reader.getSecondArg();

                    cout << "Segment: " << firstArg << endl;
                    cout << "index: " << secondArg << endl;

                    writer.writeStackInstruction(command, firstArg, secondArg);
                    break;

                case VMCommand::VM_LABEL:
                    firstArg = reader.getFirstArg();
                    
                    cout << "Label: " << firstArg << endl;

                    writer.writeLabelInstruction(firstArg);
                    break;

                case VMCommand::VM_GOTO:
                    firstArg = reader.getFirstArg();

                    cout << "Jump Label: " << firstArg << endl;

                    writer.writeGotoInstruction(firstArg);
                    break;

                case VMCommand::VM_IFGOTO:
                    firstArg = reader.getFirstArg();

                    cout << "Jump Label: " << firstArg << endl;

                    writer.writeIfGotoInstruction(firstArg);
                    break;

                case VMCommand::VM_FUNCTION:
                    firstArg = reader.getFirstArg();
                    secondArg = reader.getSecondArg();

                    cout << "Function name: " << firstArg << endl;
                    cout << "Num Local vars: " << secondArg << endl;

                    writer.writeFunctionDeclaration(firstArg, secondArg);
                    break;

                case VMCommand::VM_CALL:
                    firstArg = reader.getFirstArg();
                    secondArg = reader.getSecondArg();

                    cout << "Function name: " << firstArg << endl;
                    cout << "Num Arguments: " << secondArg << endl;

                    writer.writeCallInstruction(firstArg, secondArg);
                    break;

                case VMCommand::VM_RETURN:
                    writer.writeReturnInstruction();
                    break;
            }
        }
    }

    void VMTranslator::closeFiles()
    {
        reader.closeFile();
        writer.closeFile();
    }
}