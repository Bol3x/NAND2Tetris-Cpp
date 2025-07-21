#include "Tokenizer.h"

namespace parsing::JackCompiler
{
    bool Tokenizer::hasMoreTokens()
    {
        if (tokens.size() == 0)
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
                if (i+1 < currLine.length() && currLine[i+1] == '/' && tokens.size() > 0){
                    return;
                }

                //if it is a multi-line or API comment
                else if (i+1 < currLine.length() && currLine[i+1] == '*'){
                    auto currLineIt = currLine.find("*/");

                    //find the end of the multi-line comment
                    while (currLineIt == currLine.npos && hasMoreCommands()){
                        Parser::advanceLine();
                        currLineIt = currLine.find("*/");
                    }

                    if (currLineIt != currLine.npos){
                        int newPos = currLineIt+2;
                        currLine = currLine.substr(newPos);
                        i = newPos;
                    }
                }

                //otherwise it's a normal divide symbol
                else {
                    tokens.push(String(1,ch));
                }

                //ensure that tokens are trying to be grabbed still
                //if the comment results in no tokens, keep trying the next line
                if (tokens.size() == 0 && hasMoreCommands()){
                    Parser::advanceLine();
                    i = -1;
                }
            }

            //seperate loop to complete a string constant
            //to avoid early stopping from strings with symbols inside
            else if (ch == '\"'){
                token.push_back(ch);
                for (int j = i+1; j < currLine.length(); j++){
                    ch = currLine[j];
                    token.push_back(ch);
                    if (ch == '\"'){
                        i = j;
                        break;
                    }

                    //todo: throw error if end quote is not found in line
                }
                if (token.length() > 0)
                {
                    tokens.push(token);
                    token.clear();
                }
            } 

            //terminates tokens by detecting symbols and whitespaces
            else if (ch == '\t' || ch == ' ' || symbols.find(ch) != symbols.end()){
                if (token.length() > 0){
                    tokens.push(token);
                    token.clear();
                }
                
                //still pass the symbol as a token
                if (ch != ' '){
                    tokens.push(String(1, ch));
                }
            } else {
                token.push_back(ch);
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
            std::cout << "current token: " << currToken << std::endl;
            return;
        }

        currToken.clear();
    }

    String Tokenizer::getCurrToken(){
        return currToken;
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
            if (result >= 0 && result < 0x10000){
                return JackTokenType::JT_INT_CONST;
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