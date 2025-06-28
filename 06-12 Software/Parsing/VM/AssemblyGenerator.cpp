#include "AssemblyGenerator.h"
#include "HackVM.h"

namespace parsing::HackVM{
    AssemblyGenerator::AssemblyGenerator(const String& filename) :
    FileGenerator(filename)
    {
        std::regex filenameRegex(R"([^\\/]+?(?=\..*?$|[\\/]*$))");
        std::smatch match;

        if (std::regex_search(filename, match, filenameRegex))
        {
            VMName = match[0].str();
        }
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
                result = (index) ? "@THAT\n" : "@THIS\n";
                result =    result +
                            "D=M\n" +
                            "@SP\n" +
                            "A=M\n" +
                            "M=D\n" +
                            "@SP\n" +
                            "M=M+1";
            }
            else //local, argument, this, that, temp
            {
                result =    "@"+std::to_string(index)+"\n" +
                            "D=A\n";

                if (memorySegment == "local"){
                    result = result + 
                             "@LCL\n" +
                             "A=D+M\n";
                }
                else if (memorySegment == "argument"){
                    result = result + 
                             "@ARG\n" +
                             "A=D+M\n";
                }
                else if (memorySegment == "this"){
                    result = result + 
                             "@THIS\n" +
                             "A=D+M\n";
                }
                else if (memorySegment == "that"){
                    result = result + 
                             "@THAT\n" +
                             "A=D+M\n";
                }
                else if (memorySegment == "temp"){
                    result = result + 
                             "@TEMP\n" + 
                             "A=D+A\n";
                }

                result = result + 
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
                            "D=M\n" + 
                            "@"+std::to_string(index)+"\n" +
                            "M=D";
            }
            else if(memorySegment == "static")
            {
                result =    result +
                            "@SP\n" +
                            "AM=M-1\n" + 
                            "D=M\n" +
                            "@"+VMName+"."+std::to_string(index)+"\n" +
                            "M=D";
            }
            else if(memorySegment == "pointer")
            {
                result =    result + 
                            "@SP\n" +
                            "AM=M-1\n" +
                            "D=M\n";
                
                result =    result + ((index) ? "@THAT\n" : "@THIS\n")+
                            "M=D";
            }
            else
            {
                result =    "@"+std::to_string(index)+"\n" + 
                            "D=A\n";

                if (memorySegment == "local"){
                    result = result + 
                             "@LCL\n" +
                             "D=D+M\n";
                }
                else if (memorySegment == "argument"){
                    result = result + 
                             "@ARG\n" +
                             "D=D+M\n";
                }
                else if (memorySegment == "this"){
                    result = result + 
                             "@THIS\n" +
                             "D=D+M\n";
                }
                else if (memorySegment == "that"){
                    result = result + 
                             "@THAT\n" +
                             "D=D+M\n";
                }
                else if (memorySegment == "temp"){
                    result = result + 
                             "@TEMP\n" + 
                             "D=D+A\n";
                }

                result =    result + 
                            "@13\n" +
                            "M=D\n" +
                            "@SP\n" +
                            "AM=M-1\n" +
                            "D=M\n" +
                            "@13\n" +
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
                        "@TRUE." + VMName + "." + std::to_string(jumpIndex) + "\n";
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
                        "@RETURN." + VMName + "." + std::to_string(jumpIndex) + "\n" +
                        "0;JMP\n" +
                        "(TRUE." + VMName + "." + std::to_string(jumpIndex) + ")\n" +
                        "D=-1\n" +
                        "(RETURN." + VMName + "." + std::to_string(jumpIndex) + ")\n" +
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
                    "0;JMP\n";

        addLine(result);
    }

    void AssemblyGenerator::writeIfGotoInstruction(const String& label)
    {
        String result = "";
        
        result =        result + "@SP\n" +
                        "AM=M-1\n" +
                        "D=M\n" +
                        "@"+label+"\n" +
                        "D;JGT";

        addLine(result);
    }

    void AssemblyGenerator::writeFunctionDeclaration(const String& fileName, const String& functionName, const int& nLocals)
    {
        String result = "("+fileName+"."+functionName+")";
        addLine(result);

        //push nLocals variables onto stack
        for (int i = 0; i < nLocals; i++){
            AssemblyGenerator::writeStackInstruction(VMCommand::VM_PUSH, "constant", 0);
        }
    }

    void AssemblyGenerator::writeCallInstruction(const String& fileName, const String& functionName, const int& nArgs)
    {
        String returnAddr = fileName+"."+functionName+"$ret."+std::to_string(retIndex);
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
                        "@"+fileName+"."+functionName+"\n" + //jump to the function after preparing the frame
                        "0;JMP\n" +
                        "("+returnAddr+")"; //return address label

        retIndex++;
        addLine(result);
    }

    void AssemblyGenerator::writeReturnInstruction()
    {
        String result = "";
        result =    result +
                    "@5\n" +    //setup return address' memory location to jump to
                    "D=A\n" +
                    "@LCL\n" + 
                    "A=M\n"
                    "A=A-D\n" +
                    "D=M\n" +
                    "@15\n" +   
                    "M=D";
        addLine(result);

        //return value goes to top of stack before function call (ie. arg[0])
        AssemblyGenerator::writeStackInstruction(VMCommand::VM_POP, "argument", 0);

        result = "";
        result =    result +
                    "@ARG\n" +  //restore caller frame (SP = ARG+1)
                    "D=M+1\n" +
                    "@SP\n" +
                    "M=D\n" +
                    "@LCL\n" +  //setup pointer value
                    "A=M\n" +    
                    "A=A-1\n" +
                    "D=M\n" +
                    "@THAT\n" + //(*THAT) = EndOfFrame-1
                    "M=D\n" +
                    "@2\n" +    //setup pointer value
                    "D=A\n" +
                    "@LCL\n" + 
                    "A=M\n"
                    "A=A-D\n" +
                    "D=M\n" +
                    "@THIS\n" + //(*THIS) = EndOfFrame-2
                    "M=D\n" +
                    "@3\n" +    //setup pointer value
                    "D=A\n" +
                    "@LCL\n" + 
                    "A=M\n"
                    "A=A-D\n" +
                    "D=M\n" +
                    "@ARG\n" + //(*ARG) = EndOfFrame-3
                    "M=D\n" +
                    "@4\n" +    //setup pointer value
                    "D=A\n" +
                    "@LCL\n" + 
                    "A=M\n" +
                    "A=A-D\n" +
                    "D=M\n" +
                    "@LCL\n" + //(*LCL) = EndOfFrame-4
                    "M=D\n" +
                    "@15\n" +
                    "A=M\n" +
                    "0;JMP";

        addLine(result);
    }

    void AssemblyGenerator::updateVMName(const String& name){
        VMName = name;
    }
}