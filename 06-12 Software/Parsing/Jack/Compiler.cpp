#include "Compiler.h"

namespace parsing::JackCompiler
{
    Compiler::Compiler(const String& inputFile, const String& outputFile) : 
    tokenizer(inputFile),
    FileGenerator(outputFile)
    {}

    void Compiler::compileClass(){
        addLine("<class>");
        processTerminalReserved("class");
        processTerminalDefined();       //class name
        processTerminalReserved("{");
        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
        {
            while (tokenizer.getKeyword() == JackKeyword::JK_FIELD ||
                   tokenizer.getKeyword() == JackKeyword::JK_STATIC)
            {
                compileClassVariable(); //compile class variables (if any)
            }
        }


        compileSubroutine();    //compile class methods or functions
        processTerminalReserved("}");
        addLine("</class>");
    }

    void Compiler::compileClassVariable(){
        String token;
        addLine("<classVarDec>");
        processTerminalReserved(tokenizer.getCurrToken()); //the field/static keyword
        processTerminalReserved(tokenizer.getCurrToken()); //the datatype keyword
        //loop for as many variables possibly declared in the same line
        do{
            processTerminalDefined();   //varName
            token = tokenizer.getCurrToken();   //',' or ';'
            processTerminalReserved(token);
        } while(token == ",");
        addLine("</classVarDec>");
    }

    void Compiler::compileSubroutine(){
        addLine("<subroutineDec>");
        processTerminalReserved(tokenizer.getCurrToken()); //the type of subroutine
        processTerminalReserved(tokenizer.getCurrToken()); //the datatype keyword
        processTerminalDefined();   //identifier
        processTerminalReserved("(");
        compileParameters();
        processTerminalReserved(")");
        compileSubroutineBody();
        addLine("</subroutineDec>");
    }

    void Compiler::compileParameters(){
        addLine("<parameterList");

        addLine("</parameterList");
    }

    void Compiler::compileSubroutineBody(){
        addLine("<subroutineBody>");

        addLine("</subroutineBody>");
    }

    void Compiler::compileVarDeclaration(){
        addLine("<varDec>");

        addLine("</varDec>");
    }

    void Compiler::compileStatements(){
        addLine("<statements>");

        addLine("</statements>");
    }

    void Compiler::compileLet(){
        
    }

    void Compiler::compileIf(){

    }

    void Compiler::compileWhile(){

    }

    void Compiler::compileDo(){

    }

    void Compiler::compileReturn(){

    }

    void compileExpression(){

    }

    void compileTerm(){

    }

    void compileExpressionList(){

    }

    void Compiler::processTerminalReserved(const String& token){
        String currToken = tokenizer.getCurrToken();
        if (token == currToken){
            switch(tokenizer.getTokenType()){
                case JackTokenType::JT_KEYWORD:
                    addLine("<keyword>"+currToken+"</keyword>");
                    break;
                case JackTokenType::JT_SYMBOL:
                    addLine("<symbol>"+currToken+"</symbol>");
                    break;
            }
        } else {
            //todo: error
            addLine("SYNTAX ERROR");
        }
        tokenizer.advanceToken();
    }

    void Compiler::processTerminalDefined(){
        String currToken = tokenizer.getCurrToken();
        switch (tokenizer.getTokenType()){
            case JackTokenType::JT_IDENTIFIER:
                addLine("<identifier>"+currToken+"</identifier>");
                break;
            case JackTokenType::JT_INT_CONST:
                addLine("<integerLiteral>"+currToken+"</integerLiteral>");
                break;
            case JackTokenType::JT_STR_CONST:
                addLine("<stringLiteral>"+currToken+"</stringLiteral>");
                break;
        }
        tokenizer.advanceToken();
    }
}