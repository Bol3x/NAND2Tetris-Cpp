#include "VMWriter.h"

namespace parsing::JackCompiler
{

    void VMWriter::writePushInstruction(const JackVMSegment& segment, const int& index)
    {
        String instruction = "push";
        switch(segment)
        {
            case JackVMSegment::JVT_CONSTANT:
                instruction.append(" constant");
                break;

            case JackVMSegment::JVT_ARGUMENT:
                instruction.append(" argument");
                break;

            case JackVMSegment::JVT_LOCAL:
                instruction.append(" local");
                break;

            case JackVMSegment::JVT_STATIC:
                instruction.append(" static");
                break;

            case JackVMSegment::JVT_POINTER:
                instruction.append(" pointer");
                break;

            case JackVMSegment::JVT_THIS:
                instruction.append(" this");
                break;

            case JackVMSegment::JVT_THAT:
                instruction.append(" that");
                break;

            case JackVMSegment::JVT_TEMP:
                instruction.append(" temp");
                break;
        }

        instruction.append(" " + std::to_string(index));

        addLine(instruction);
    }

    void VMWriter::writePopInstruction(const JackVMSegment& segment, const int& index)
    {
        String instruction = "pop";
        switch(segment)
        {
            case JackVMSegment::JVT_CONSTANT:
                instruction.append(" constant ");
                break;

            case JackVMSegment::JVT_ARGUMENT:
                instruction.append(" argument ");
                break;

            case JackVMSegment::JVT_LOCAL:
                instruction.append(" local ");
                break;

            case JackVMSegment::JVT_STATIC:
                instruction.append(" static ");
                break;

            case JackVMSegment::JVT_POINTER:
                instruction.append(" pointer ");
                break;

            case JackVMSegment::JVT_THIS:
                instruction.append(" this ");
                break;

            case JackVMSegment::JVT_THAT:
                instruction.append(" that ");
                break;

            case JackVMSegment::JVT_TEMP:
                instruction.append(" temp ");
                break;
        }

        instruction.append(std::to_string(index));

        addLine(instruction);
    }

    void VMWriter::writeArithmeticInstruction(const JackVMCommand& command)
    {
        switch(command)
        {
            case JackVMCommand::JVC_ADD:
                addLine("add");
                break;
            
            case JackVMCommand::JVC_SUB:
                addLine("sub");
                break;

            case JackVMCommand::JVC_AND:
                addLine("and");
                break;

            case JackVMCommand::JVC_OR:
                addLine("or");
                break;

            case JackVMCommand::JVC_NOT:
                addLine("not");
                break;

            case JackVMCommand::JVC_EQ:
                addLine("eq");
                break;

            case JackVMCommand::JVC_GT:
                addLine("gt");
                break;

            case JackVMCommand::JVC_LT:
                addLine("lt");
                break;

        }
    }

    void VMWriter::writeLabel(const String& label)
    {
        addLine("label " + label);
    }

    void VMWriter::writeGotoInstruction(const String& label)
    {
        addLine("goto " + label);
    }

    void VMWriter::writeIfInstruction(const String& label)
    {
        addLine("if-goto " + label);
    }

    void VMWriter::writeCallInstruction(const String& name, const int& nArgs)
    {
        addLine("call " + name + " " + std::to_string(nArgs));
    }

    void VMWriter::writeFunctionDeclaration(const String& name, const int& nVars)
    {
        addLine("function " + name + " " + std::to_string(nVars));
    }

    void VMWriter::writeReturnInstruction()
    {
        addLine("return");
    }

}