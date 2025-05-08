#include <iostream>

#include "Parsing\Assembly\HackTranslator.cpp"
#include "Parsing\VM\VMTranslator.cpp"

using namespace parsing;

int main(){
    String inputASM = ".\\testfiles\\SimpleFunction.asm", outputByte = ".\\testfiles\\SimpleFunction.hack";
    String inputVM  = ".\\testfiles\\SimpleFunction.vm", outputASM = ".\\testfiles\\SimpleFunction.asm";

    HackVM::VMTranslator vm(inputVM, outputASM);

    vm.generateAssembly();

    HackAssembly::HackTranslator hack(inputASM, outputByte);

    hack.generateOpcode();
    
    return 0;
}