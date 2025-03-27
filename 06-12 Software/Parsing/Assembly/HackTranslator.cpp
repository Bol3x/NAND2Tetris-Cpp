#include "HackTranslator.hpp"

//using namespace std;

namespace parsing
{
    HackTranslator::HackTranslator(const String& in, const String& out) :
    reader(in), writer(out)
    {}

    void HackTranslator::generateOpcode()
    {
        while (reader.hasMoreCommands())
        {
            reader.advanceLine();
            //cout << "Next line: ";
            String instruction = reader.getLine();
            //cout << instruction << endl;
    
            AssemblyCommand instructionType = reader.getCommandType();
    
            if (instructionType == AssemblyCommand::A_COMMAND)
            {
                String address = reader.getSymbol();

                String res = writer.generateAddressBinary(address);
                //cout << "result: " << res << endl;
    
                writer.addLine(res);
            }
    
            if (instructionType == AssemblyCommand::C_COMMAND)
            {
                String comp = reader.getComp();
                String dest = reader.getDest();
                String jump = reader.getJump();
    
                //cout << "comp: " << comp << endl;
                //cout << "dest: " << dest << endl;
                //cout << "jump: " << jump << endl;
    
                String res = writer.generateCompBinary(comp, dest, jump);
                //cout << "result: " << res << endl;
    
                writer.addLine(res);
            }
        }
    }

    void HackTranslator::closeFiles()
    {
        reader.closeFile();
        writer.closeFile();
    }

}