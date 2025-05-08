#include "Tokenizer.hpp"

namespace parsing::JackCompiler
{
    Tokenizer::Tokenizer(const String& filename) :
    Parser(filename)
    {}

    bool Tokenizer::hasMoreTokens()
    {
        if (! tokens.size())
        {
            return false;
        }
        return true;
    }

    void Tokenizer::advanceLine(){
        Parser::advanceLine();
        
        //idea: iterate through string, building up a token string until a whitespace or other symbol is detected
        //repeat until end of currLine
        //this way all tokens can be extracted from the string before any operation is done on the current line of tokens
        String token = "";
        for (int i = 0; i < currLine.length(); i++){
            char ch = currLine[i];

            //if line contains a comment, identify the type
            if (ch == '/')
            {
                //ignore the rest of the line
                if (i+1 < currLine.length() && currLine[i+1] == '/'){
                    return;
                }

                //if it is a multi-line or API comment
                if (i+1 < currLine.length() && currLine[i+1] == '*'){
                    auto currLineIt = currLine.find("*/");

                    //find the end of the multi-line comment
                    while (currLineIt == currLine.npos && hasMoreCommands()){
                        advanceLine();
                        currLineIt = currLine.find("*/");
                    }

                    if (currLineIt != currLine.npos){
                        int newPos = currLineIt+2;
                        currLine = currLine.substr(newPos);
                        i = newPos;
                    }
                }
            }

            //seperate loop to complete a string constant
            //to avoid early stopping from strings with symbols inside
            else if (ch == '\"'){
                token += ch;
                for (int j = i+1; j < currLine.length(); j++){
                    ch = currLine[j];
                    token += ch;
                    if (ch == '\"'){
                        i = j;
                        break;
                    }

                    //todo: throw error if end quote is not found in line
                }
                tokens.push(token);
            } 

            //terminates tokens by detecting symbols and whitespaces
            else if (ch == ' ' || symbols.find(ch) != symbols.end()){
                if (token.length()){
                    tokens.push(token);
                    token = "";
                }
                
                //still pass the symbol as a token
                if (ch != ' '){
                    tokens.push(String(1, ch));
                }
            } else {
                token += ch;
            }
        }
    }

    void Tokenizer::advanceToken()
    {
        if (! hasMoreTokens() && hasMoreCommands())
        {
            advanceLine();
        }

        if (hasMoreTokens()){
            currToken = tokens.front();
            tokens.pop();
            return;
        }

        currToken = "";
    }

    JackTokenType Tokenizer::getTokenType()
    {
        //todo: keyword
        if (keywordMap.find(currToken) != keywordMap.end())
        {
            return JackTokenType::JT_KEYWORD;
        }

        //todo: identifier
        if (keywordMap.find(currToken) == keywordMap.end())
        {
            //identifiers must only start with letters and '_'
            if ((currToken[0] >= 'a' && currToken[0] <= 'z') || 
                (currToken[0] >= 'A' && currToken[0] <= 'Z') || (currToken[0] == '_')){
                    return JackTokenType::JT_IDENTIFIER;
            }
        }

        //todo: symbols
        //fix this, symbols identified as Invalid type
        if (symbols.find(currToken[0]) != symbols.end()){
            return JackTokenType::JT_SYMBOL;
        }

        //todo: string constant
        if (currToken[0] == '\"'){
            return JackTokenType::JT_STR_CONST;
        }

        //todo: int constant
        //validate int value
        try {
            int result = std::stoi(currToken);
            //0 to 
            if (result >= 0 || result < 0x10000){
                return JackTokenType::JT_KEYWORD;
            } else {
                return JackTokenType::JT_INVALID;
            }
        } catch (const std::invalid_argument& ia){
            return JackTokenType::JT_INVALID;
        }

        return JackTokenType::JT_INVALID;
    }

    JackKeyword Tokenizer::getKeyword(){
        return keywordMap.find(currToken)->second;
    }

    char Tokenizer::getSymbol(){
        return currToken[0];
    }

    String Tokenizer::getIdentifier(){
        return currToken;
    }

    int Tokenizer::getIntValue(){
        return std::stoi(currToken);
    }

    String Tokenizer::getStringValue(){
        return currToken;
    }

}