#include "AssemblyGenerator.hpp"

namespace parsing{
    AssemblyGenerator::AssemblyGenerator(const String& filename) :
    FileGenerator(filename)
    {
        VMName = filename.substr(0, filename.find('.'));
    }

    void AssemblyGenerator::writeStackInstruction(const VMCommand& command, const String& memorySegment, const int& index) 
    {
        String result = "";
        if (command == VMCommand::VM_PUSH)
        {
            if (memorySegment == "constant")
            {
                result = "@"+std::to_string(index)+"\n" +
                                "D=A\n" + 
                                "@SP\n" +
                                "A=M\n" + 
                                "M=D\n" +
                                "@SP\n" + 
                                "M=M+1";
            }
            else if (memorySegment == "static")
            {
                result =    "@"+VMName+"."+std::to_string(index)+"\n" +
                            "D=M\n" +
                            "@SP\n" +
                            "A=M\n" +
                            "M=D\n" +
                            "@SP\n" +
                            "M=M+1";
            }
            else if (memorySegment == "pointer")
            {
                result = (index) ? "THAT\n" : "THIS\n";
                result =    result +
                            "A=M\n" +
                            "D=M\n" +
                            "@SP\n" +
                            "A=M\n" +
                            "M=D\n" +
                            "@SP\n" +
                            "M=M+1";
            }
            else //local, argument, this, that
            {
                result =    "@"+VMName+"."+std::to_string(index)+"\n" +
                            "D=A\n";

                if (memorySegment == "local"){
                    result = result + "@LCL\n";
                }
                else if (memorySegment == "argument"){
                    result = result + "@ARG\n";
                }
                else if (memorySegment == "this"){
                    result = result + "@THIS\n";
                }
                else if (memorySegment == "that"){
                    result = result + "@THAT\n";
                }

                result = result + 
                        "A=D+A\n" + 
                        "D=M\n" +
                        "@SP\n" +
                        "A=M\n" +
                        "M=D\n" +
                        "@SP\n" +
                        "M=M+1";
            }
        }

        if (command == VMCommand::VM_POP)
        {
            if (memorySegment == "constant")
            {
                result =    result +
                            "@SP\n" + 
                            "AM=M-1\n" +
                            "A=M\n" +
                            "D=M\n" + 
                            "@"+std::to_string(index)+"\n" +
                            "M=D";
            }
            else if(memorySegment == "static")
            {
                result =    result +
                            "@SP\n" +
                            "M=M-1\n" + 
                            "A=M\n" +
                            "D=M\n" +
                            "@"+VMName+"\n" +
                            "M=D";
            }
            else if(memorySegment == "pointer")
            {
                result =    result + 
                            "@SP\n" +
                            "M=M-1\n" +
                            "A=M\n" +
                            "D=M\n";
                
                result =    result + ((index) ? "@THIS\n" : "@THAT\n")+
                            "A=M\n" +
                            "M=D";
            }
            else
            {
                result =    "@"+std::to_string(index)+"\n" + 
                            "D=A\n";

                if (memorySegment == "local"){
                    result = result + "@LCL\n";
                }
                else if (memorySegment == "argument"){
                    result = result + "@ARG\n";
                }
                else if (memorySegment == "this"){
                    result = result + "@THIS\n";
                }
                else if (memorySegment == "that"){
                    result = result + "@THAT\n";
                }

                result =    result + 
                            "D=D+A\n" +
                            "@5\n" +
                            "M=D\n" +
                            "@SP\n" +
                            "AM=M-1\n" +
                            "D=M\n" +
                            "@5\n" +
                            "A=M\n" +
                            "M=D";

            }
        }
        
        addLine(result);
    }

    void AssemblyGenerator::writeArithmeticInstruction(const String& operation)
    {
        String result = "";

        if (operation == "add")
        {
            result = "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=D+M\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        }

        else if (operation == "sub")
        {
            result = "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        }

        else if (operation == "neg")
        {
            result = "@SP\nAM=M-1\nD=M\nD=-D\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        }

        else if (operation == "and")
        {
            result = "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=D&M\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        }

        else if (operation == "or")
        {
            result = "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=D|M\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        }

        else if (operation == "not")
        {
            result = "@SP\nAM=M-1\nD=!D\n@SP\nA=M\nM=D\n@SP\nM=M+1";
        }
        
        else
        {

            result =    result + "@SP\n" + 
                        "AM=M-1\n" +
                        "D=M\n" + 
                        "@SP\n" +
                        "AM=M-1\n" +
                        "D=M-D\n" +
                        "@TRUE." + std::to_string(jumpIndex) + "\n";
            if (operation == "eq")
            {
                result = result + "D;JEQ\n";
            }
            if (operation == "gt")
            {
                result = result + "D;JGT\n";
            }
            if (operation == "lt")
            {
                result = result + "D;JLT\n";
            }

            result =    result + "D=0\n" +
                        "@RETURN." + std::to_string(jumpIndex) + "\n" +
                        "0;JMP\n" +
                        "(TRUE." + std::to_string(jumpIndex) + ")\n" +
                        "D=-1\n" +
                        "(RETURN." + std::to_string(jumpIndex) + ")\n" +
                        "@SP\n" +
                        "A=M\n" +
                        "M=D\n" +
                        "@SP\n" +
                        "M=M+1";
            
            jumpIndex++;
        }
        
        addLine(result);
    }

    void AssemblyGenerator::writeLabelInstruction(const String& label)
    {
        String result = "(" + label + ")";
        addLine(result);
    }

    void AssemblyGenerator::writeGotoInstruction(const String& label)
    {
        String result = "";
        
        result =    result + "@"+label+"\n" +
                    "0;JMP";

        addLine(result);
    }

    void AssemblyGenerator::writeIfGotoInstruction(const String& label)
    {
        String result = result + "@SP\n" +
                        "AM=M-1\n" +
                        "D=M\n" +
                        "@"+label+"\n" +
                        "D;JGT";

        addLine(result);
    }

    void AssemblyGenerator::writeFunctionDeclaration(const String& functionName, const int& nLocals)
    {
        String result = "("+VMName+functionName+")\n" +
                        "@" + std::to_string(nLocals) + "\n" + //move SP +nLocals forward
                        "D=D+A\n" +
                        "@SP\n" +
                        "M=D";
        
        addLine(result);
    }

    void AssemblyGenerator::writeCallInstruction(const String& functionName, const int& nArgs)
    {
        String returnAddr = VMName+"."+functionName+"$ret."+std::to_string(retIndex);
        String result = "";
        
        result =        result + 
                        "@" + returnAddr + "\n" + //push return address
                        "D=A\n" +
                        "@SP\n" +
                        "A=M\n" + 
                        "M=D\n" +
                        "@SP\n" + 
                        "M=M+1\n" + //next, push all pointer registers to save the frame before the call
                        "@LCL\n" +  //push LCL pointer
                        "D=M\n" +
                        "@SP\n" +
                        "A=M\n" + 
                        "M=D\n" +
                        "@SP\n" + 
                        "M=M+1\n" + 
                        "@ARG\n" +  //push ARG pointer
                        "D=M\n" +
                        "@SP\n" +
                        "A=M\n" + 
                        "M=D\n" +
                        "@SP\n" + 
                        "M=M+1\n" +
                        "@THIS\n" + //push THIS pointer
                        "D=M\n" +
                        "@SP\n" +
                        "A=M\n" + 
                        "M=D\n" +
                        "@SP\n" + 
                        "M=M+1\n" + 
                        "@THAT\n" + //push THAT pointer
                        "D=M\n" +
                        "@SP\n" +
                        "A=M\n" + 
                        "M=D\n" +
                        "@SP\n" + 
                        "M=M+1\n" +
                        "@" + std::to_string(nArgs) + "\n" +    //set ARG to specified offset (SP - 5 - nArgs)
                        "D=A\n" +
                        "@5\n" +
                        "D=D+A\n" +
                        "@SP\n" +
                        "D=M-D\n" +
                        "@ARG\n" +
                        "M=D\n" +
                        "@SP\n" +  //set LCL to top of stack
                        "D=M\n" +
                        "@LCL\n" +
                        "M=D\n" +
                        "@"+functionName+"\n" + //jump to the function
                        "0;JMP\n" +
                        "("+returnAddr+")"; //return address label

        retIndex++;
        addLine(result);
    }

    void AssemblyGenerator::writeReturnInstruction()
    {
        String result = "";

        result =    result + //POP ARG 0
                    "@SP\n" +
                    "A=M-1\n" +
                    "D=M\n" +
                    "@ARG\n" +
                    "A=M\n" +
                    "M=D\n" +
                    "@ARG\n" +    //SP = ARG + 1
                    "D=M+1\n" +
                    "@SP\n" +
                    "M=D\n" + //restore caller frame
                    "@LCL\n" + 
                    "D=M\n" +
                    "@5\n" +    //(*THAT) = EndOfFrame-1
                    "AM=M-1\n" + 
                    "D=M\n" +
                    "@THAT\n" +
                    "M=D\n" +
                    "@5\n" +    //(*THIS) = EndOfFrame-2
                    "AM=M-1\n" + 
                    "D=M\n" +
                    "@THAT\n" +
                    "M=D\n" +
                    "@5\n" +    //(*ARG) = EndOfFrame-3
                    "AM=M-1\n" +
                    "D=M\n" +
                    "@ARG\n" +  
                    "M=D\n"
                    "@5\n" +    //(*LCL) = EndOfFrame-4
                    "AM=M-1\n" +
                    "D=M\n" +
                    "@LCL\n" +  
                    "M=D\n" +
                    "@5\n" +   //Return Address (EndOfFrame-5)
                    "AM=M-1\n" +
                    "0;JMP";

        addLine(result);
    }

}