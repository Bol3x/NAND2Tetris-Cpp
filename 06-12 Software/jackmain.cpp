#include <iostream>
#include <filesystem>
#include <vector>

#include "Parsing/Jack/Compiler.cpp"

using namespace parsing;
using namespace std;
namespace fs = std::filesystem;

int main()
{
    //check whether its a folder or a single file
    String source;
    cout << "enter path: ";
    cin >> source;

    cout << source << endl;
    fs::path filePath = source;


    /*
    JackCompiler::Tokenizer tokenizer;

    tokenizer.openFile(filePath.string());

    String token;
    while (tokenizer.hasMoreTokens() || tokenizer.hasMoreCommands())
    {
        if (!tokenizer.hasMoreTokens())
        {
            std::cout << "tokens queue is empty!" << std::endl;
            tokenizer.advanceToken();
        }
        else
        {
            token = tokenizer.getCurrToken();
            cout << "token: " << token << endl;
            tokenizer.advanceToken();
        }
    }

    tokenizer.closeFile();
    */

    JackCompiler::Compiler compiler;

    fs::path fileXML = filePath;
    fileXML.replace_extension("xml");

    compiler.openFile(fileXML.string());

    compiler.compileFile(filePath.string());

    compiler.closeFile();
    
    return 0;
}