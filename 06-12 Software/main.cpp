#include <iostream>

#include "Parsing\Assembly\HackTranslator.cpp"
#include "Parsing\VM\VMTranslator.cpp"

using namespace parsing;

int main(){
    String inputASM = ".\\testfiles\\Max.asm", outputByte = ".\\testfiles\\Max.hack";
    String inputVM  = ".\\testfiles\\SimpleAdd.vm", outputASM = ".\\testfiles\\SimpleAdd.asm";

    HackTranslator hack(inputASM, outputByte);

    VMTranslator vm(inputVM, outputASM);

    hack.generateOpcode();

    vm.generateAssembly();
    
    return 0;
}