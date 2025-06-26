#include <iostream>

#include "Parsing/Jack/Tokenizer.cpp"

using namespace parsing::JackCompiler;
using namespace std;

int main(){

    Tokenizer tkn("testfile.txt");

    JackTokenType type;

    cout << "testing input..." << endl;

    do{
        tkn.advanceLine();
        
        while (tkn.hasMoreTokens()){
            cout << "Current string: " << tkn.getLine() << endl;
            type = tkn.getTokenType();

            if (type == JackTokenType::JT_IDENTIFIER) cout << "Identifier" << endl;
            if (type == JackTokenType::JT_KEYWORD) cout << "Keyword" << endl;
            if (type == JackTokenType::JT_SYMBOL) cout << "Symbol" << endl;
            if (type == JackTokenType::JT_INT_CONST) cout << "Integer Literal" << endl;
            if (type == JackTokenType::JT_STR_CONST) cout << "String Literal" << endl;
            if (type == JackTokenType::JT_INVALID) cout << "N/A" << endl;
        }

    } while (tkn.hasMoreCommands());

    return 0;
}