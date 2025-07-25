#include "VMTranslator.h"

namespace parsing::HackVM
{
    VMTranslator::VMTranslator(const String& filePath) :
    writer((filePath+"\\exec.asm"))
    {}

    void VMTranslator::generateAssembly(const String& filePath, const String& fileName)
    {
        //std::cout << filePath+"\\"+fileName+".vm" << std::endl;
        reader.openFile(filePath+"\\"+fileName+".vm");
        writer.updateVMName(fileName);

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
                    cout << "POP" << endl;
                case VMCommand::VM_PUSH:
                    if (command == VMCommand::VM_PUSH)
                        cout << "PUSH" << endl;
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

                    cout << "Conditional Jump Label: " << firstArg << endl;

                    writer.writeIfGotoInstruction(firstArg);
                    break;

                case VMCommand::VM_FUNCTION:
                    firstArg = reader.getFirstArg();
                    secondArg = reader.getSecondArg();

                    cout << "Function name: " << firstArg << endl;
                    cout << "Num Local vars: " << secondArg << endl;

                    writer.writeFunctionDeclaration(firstArg.substr(0, firstArg.find('.')),
                                                    firstArg.substr(firstArg.find('.')+1), 
                                                    secondArg);
                    break;

                case VMCommand::VM_CALL:
                    firstArg = reader.getFirstArg();
                    secondArg = reader.getSecondArg();

                    cout << "Function name: " << firstArg << endl;
                    cout << "Num Arguments: " << secondArg << endl;

                    writer.writeCallInstruction(firstArg.substr(0, firstArg.find('.')),
                                                firstArg.substr(firstArg.find('.')+1), 
                                                secondArg);
                    break;

                case VMCommand::VM_RETURN:
                    writer.writeReturnInstruction();
                    break;
            }
        }

        reader.closeFile();
    }

    void VMTranslator::closeReader(){
        reader.closeFile();
    }

    void VMTranslator::closeWriter(){
        writer.closeFile();
    }
}