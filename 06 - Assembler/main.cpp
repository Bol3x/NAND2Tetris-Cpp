#include <iostream>

#include "Parser.cpp"
#include "BinaryGenerator.cpp"

using namespace std;

int main(){
    string inputfile = ".\\testfiles\\Pong.asm", outputfile = ".\\testfiles\\pong.hack";

    parsing::Parser in = parsing::Parser(inputfile);

    parsing::BinaryGenerator out = parsing::BinaryGenerator(outputfile);

    while (in.hasMoreCommands())
    {
        in.advanceLine();
        cout << "Next line: ";
        string instruction = in.getLine();
        cout << instruction << endl;

        parsing::Command instructionType = in.getCommandType();

        if (instructionType == parsing::Command::A_COMMAND)
        {
            string address = in.getSymbol();
            string res = out.generateAddressBinary(address);
            cout << res << endl;

            out.addLine(res);
        }

        if (instructionType == parsing::Command::C_COMMAND)
        {
            string comp = in.getComp();
            string dest = in.getDest();
            string jump = in.getJump();

            cout << "comp: " << comp << endl;
            cout << "dest: " << dest << endl;
            cout << "jump: " << jump << endl;

            string res = out.generateCompBinary(comp, dest, jump);
            cout << res << endl;

            out.addLine(res);
        }
    }

    in.closeFile();
    out.closeFile();
    return 0;
}