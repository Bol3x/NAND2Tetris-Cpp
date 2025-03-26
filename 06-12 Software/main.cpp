#include <iostream>

#include "Parsing\Assembly\HackTranslator.cpp"

using namespace parsing;

int main(){
    String inputfile = ".\\testfiles\\Max.asm", outputfile = ".\\testfiles\\Max.hack";

    HackTranslator hack(inputfile, outputfile);

    hack.generateOpcode();
    
    return 0;
}