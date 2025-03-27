#include "AssemblyGenerator.hpp"

namespace parsing{
    AssemblyGenerator::AssemblyGenerator(const String& filename) :
    FileGenerator(filename),
    ops(std::unordered_map<String, String>
        {
            {"add", "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=D+M\n@SP\nA=M\nM=D\n@SP\nM=M+1"},
            {"sub", "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@SP\nA=M\nM=D\n@SP\nM=M+1"},
            {"neg", "@SP\nAM=M-1\nD=M\nD=-D\n@SP\nA=M\nM=D\n@SP\nM=M+1"},
            {"and", "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=D&M\n@SP\nA=M\nM=D\n@SP\nM=M+1"},
            {"or",  "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=D|M\n@SP\nA=M\nM=D\n@SP\nM=M+1"},
            {"not", "@SP\nAM=M-1\nD=!D\n@SP\nA=M\nM=D\n@SP\nM=M+1"},
            {"eq",  "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@TRUE\nD;JEQ\nD=0\n@RETURN\n0;JMP\n(TRUE)\nD=-1\n(RETURN)\n@SP\nA=M\nM=D\n@SP\nM=M+1"},
            {"gt",  "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@TRUE\nD;JGT\nD=0\n@RETURN\n0;JMP\n(TRUE)\nD=-1\n(RETURN)\n@SP\nA=M\nM=D\n@SP\nM=M+1"},
            {"lt",  "@SP\nAM=M-1\nD=M\n@SP\nAM=M-1\nD=M-D\n@TRUE\nD;JLT\nD=0\n@RETURN\n0;JMP\n(TRUE)\nD=-1\n(RETURN)\n@SP\nA=M\nM=D\n@SP\nM=M+1"}
        }
    )
    {
        VMName = filename.substr(0, filename.find('.'));

    }

    void AssemblyGenerator::writeStackInstruction(const VMCommand& command, const String& memorySegment, const int& index) 
    {
        if (command == VMCommand::ARITHMETIC)
        {
            return;
        }

        String result = "";
        if (command == VMCommand::PUSH)
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

        if (command == VMCommand::POP)
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
                            "A=D+A\n" +
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
        if (ops.isValidCode(operation))
        {
            result = ops.getCode(operation);
        }
        
        addLine(result);
    }
}