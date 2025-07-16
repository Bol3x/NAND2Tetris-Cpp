#include "Compiler.h"

namespace parsing::JackCompiler
{

    void Compiler::compileFile(const String& in)
    {
        tokenizer.openFile(in);

        while (tokenizer.hasMoreTokens() || tokenizer.hasMoreCommands())
        {
            if (tokenizer.hasMoreTokens())
            {
                if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD && tokenizer.getKeyword() == JackKeyword::JK_CLASS)
                {
                    compileClass();
                }
                else 
                {
                    //fallback if token is not a part of a class
                    tokenizer.advanceToken();
                }
            }
            else
            {
                tokenizer.advanceToken();
            }
        }
    }

    void Compiler::compileClass()
    {
        addLine("<class>");
        processTerminalReserved("class");
        processTerminalDefined();       //class name
        processTerminalReserved("{");

        //loop to get all class variable definitions
        while (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD && 
              (tokenizer.getKeyword() == JackKeyword::JK_FIELD ||
               tokenizer.getKeyword() == JackKeyword::JK_STATIC))
        {
            compileClassVariable();
        }
        
        //loop to get all subroutine declarations in the class
        while (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD && 
              (tokenizer.getKeyword() == JackKeyword::JK_CONSTRUCTOR ||
               tokenizer.getKeyword() == JackKeyword::JK_FUNCTION    ||
               tokenizer.getKeyword() == JackKeyword::JK_METHOD))
            {
                compileSubroutine();    //compile class methods or functions
            }
        processTerminalReserved("}");
        addLine("</class>");
    }

    void Compiler::compileClassVariable(){
        String token;
        addLine("<classVarDec>");
        processTerminalReserved(tokenizer.getCurrToken()); //the field/static keyword
        processTerminalReserved(tokenizer.getCurrToken()); //the datatype keyword
        //loop for as many variables possibly declared
        do{
            processTerminalDefined();   //varName
            token = tokenizer.getCurrToken();   //',' or ';'
            processTerminalReserved(token);
        } while(token == ",");
        addLine("</classVarDec>");
    }

    void Compiler::compileSubroutine(){
        addLine("<subroutineDec>");
        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
        {
            switch(tokenizer.getKeyword()){
                case JackKeyword::JK_CONSTRUCTOR:
                    processTerminalReserved("constructor");
                    break;
                case JackKeyword::JK_FUNCTION:
                    processTerminalReserved("function");
                    break;
                case JackKeyword::JK_METHOD:
                    processTerminalReserved("method");
            }
        }

        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
        {
            switch(tokenizer.getKeyword()){
                case JackKeyword::JK_VOID:
                    processTerminalReserved("void");
                    break;
                case JackKeyword::JK_INT:
                    processTerminalReserved("int");
                    break;
                case JackKeyword::JK_BOOLEAN:
                    processTerminalReserved("boolean");
                    break;
                case JackKeyword::JK_CHAR:
                    processTerminalReserved("char");
            }
        }
        else
        {
            processTerminalReserved(tokenizer.getCurrToken()); //a user-defined class
        }

        processTerminalDefined();   //subroutine identifier
        processTerminalReserved("(");
        compileParameters();
        processTerminalReserved(")");
        compileSubroutineBody();
        addLine("</subroutineDec>");
    }

    void Compiler::compileParameters(){
        addLine("<parameterList>");

        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD){
            //process any more parameter definitions
            char symbol;
            do{
                if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
                {
                    switch(tokenizer.getKeyword()){
                        case JackKeyword::JK_INT:
                            processTerminalReserved("int");
                            break;
                        case JackKeyword::JK_BOOLEAN:
                            processTerminalReserved("boolean");
                            break;
                        case JackKeyword::JK_CHAR:
                            processTerminalReserved("char");
                    }
                }
                else
                {
                    processTerminalReserved(tokenizer.getCurrToken()); //a user-defined class
                }

                processTerminalDefined();   //varName
                symbol = tokenizer.getSymbol();
                if (symbol != ')'){
                    processTerminalReserved(String(1, symbol));
                }
            } while (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == ',');
        }

        addLine("</parameterList>");
    }

    void Compiler::compileSubroutineBody(){
        addLine("<subroutineBody>");
        processTerminalReserved("{");

        //variable declarations
        while (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD && tokenizer.getKeyword() == JackKeyword::JK_VAR)
        {
            compileVarDeclaration();
        }

        //statements
        compileStatements();
        processTerminalReserved("}");
        addLine("</subroutineBody>");
    }

    void Compiler::compileVarDeclaration(){
        addLine("<varDec>");
        processTerminalReserved("var");

        //if token is a keyword (ie. defined types)
        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
        {
            switch(tokenizer.getKeyword()){
                case JackKeyword::JK_INT:
                    processTerminalReserved("int");
                    break;
                case JackKeyword::JK_BOOLEAN:
                    processTerminalReserved("boolean");
                    break;
                case JackKeyword::JK_CHAR:
                    processTerminalReserved("char");
                    break;
            }
        }
        //if token refers to a class object
        else 
        {
            processTerminalDefined();
        }

        String token;
        do{
            processTerminalDefined();   //varName
            token = tokenizer.getCurrToken();   //',' or ';'
            processTerminalReserved(token);
        } while(token == ",");
        
        addLine("</varDec>");
    }

    void Compiler::compileStatements(){
        addLine("<statements>");
        JackKeyword keyword;
            keyword = tokenizer.getKeyword();
            while (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD && (keyword == JackKeyword::JK_LET || keyword == JackKeyword::JK_IF || keyword == JackKeyword::JK_WHILE ||
                  keyword == JackKeyword::JK_DO || keyword == JackKeyword::JK_RETURN)){
                    switch (keyword)
                    {
                    case JackKeyword::JK_LET:
                        compileLet();
                        break;

                    case JackKeyword::JK_IF:
                        compileIf();
                        break;
                    
                    case JackKeyword::JK_WHILE:
                        compileWhile();
                        break;

                    case JackKeyword::JK_DO:
                        compileDo();
                        break;

                    case JackKeyword::JK_RETURN:
                        compileReturn();
                        break;

                    default:
                        throw "Invalid statements!";

                    }
                    if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD){
                        keyword = tokenizer.getKeyword();
                    }
        }
        addLine("</statements>");
    }

    void Compiler::compileLet(){
        addLine("<LetStatement>");
        processTerminalReserved("let");
        processTerminalDefined();
        //look ahead if a array definition
        if (tokenizer.getCurrToken() == "["){
            processTerminalReserved("[");
            compileExpression();
            processTerminalReserved("]");
        }
        processTerminalReserved("=");
        compileExpression();
        processTerminalReserved(";");
        addLine("</LetStatement>");
    }

    void Compiler::compileIf(){
        addLine("<ifStatement>");
        processTerminalReserved("if");
        processTerminalReserved("(");
        compileExpression();
        processTerminalReserved(")");
        processTerminalReserved("{");
        compileStatements();
        processTerminalReserved("}");
        //look ahead for else
        if (tokenizer.getCurrToken() == "else"){
            processTerminalReserved("else");
            processTerminalReserved("{");
            compileExpression();
            processTerminalReserved("}");
        }
        processTerminalReserved(";");
        addLine("</ifStatement>");
    }

    void Compiler::compileWhile(){
        addLine("<whileStatement>");
        processTerminalReserved("while");
        processTerminalReserved("(");
        compileExpression();
        processTerminalReserved(")");
        processTerminalReserved("{");
        compileStatements();
        processTerminalReserved("}");
        addLine("</whileStatement>");
    }

    void Compiler::compileDo(){
        addLine("<doStatement>");
        processTerminalReserved("do");
        //subroutine call
        processTerminalDefined();
        //loop through possibly nested object attributes
        while (tokenizer.getCurrToken() == "."){
            processTerminalReserved(".");
            processTerminalDefined();
        }
        processTerminalReserved("(");
        compileExpressionList();
        processTerminalReserved(")");
        //end of subroutine call
        processTerminalReserved(";");
        addLine("</doStatement>");
    }

    void Compiler::compileReturn(){
        addLine("<returnStatement>");
        processTerminalReserved("return");
        compileExpression();
        processTerminalReserved(";");
        addLine("</returnStatement>");
    }

    void Compiler::compileExpression(){
        //todo: use a modified recursive descent parser to make use of the operator precedence
        //check RDP.cpp for the general idea of using trees to generate the order
        addLine("<expression>");
        compileTerm();
        char op = tokenizer.getCurrToken()[0];
        while (operators.find(op) != operators.end())
        {
            processTerminalReserved(String(1, op));
            compileTerm();

            op = tokenizer.getCurrToken()[0];
        }
        addLine("</expression>");
    }

    void Compiler::compileTerm(){
        addLine("<term>");
        switch(tokenizer.getTokenType())
        {
            case JackTokenType::JT_INT_CONST:
            case JackTokenType::JT_STR_CONST:
                processTerminalDefined();
                break;

            case JackTokenType::JT_KEYWORD:
                if (tokenizer.getKeyword() == JackKeyword::JK_TRUE)
                {
                    processTerminalReserved("true");
                }
                else if (tokenizer.getKeyword() == JackKeyword::JK_FALSE)
                {
                    processTerminalReserved("false");
                }
                else if (tokenizer.getKeyword() == JackKeyword::JK_NULL)
                {
                    processTerminalReserved("null");
                }
                else if (tokenizer.getKeyword() == JackKeyword::JK_THIS)
                {
                    processTerminalReserved("this");
                }
                break;

            case JackTokenType::JT_IDENTIFIER:
                processTerminalDefined();
                //can be an array
                if (tokenizer.getCurrToken() == "[")
                {
                    processTerminalReserved("[");
                    compileExpression();
                    processTerminalReserved("]");
                }
                //can be a subroutine call
                else 
                { 
                    //can also be in a nested object
                    while (tokenizer.getCurrToken() == "."){
                        processTerminalReserved(".");
                        processTerminalDefined();
                    }

                    if (tokenizer.getCurrToken() == "("){
                        processTerminalReserved("(");
                        compileExpressionList();
                        processTerminalReserved(")");
                    }
                }
                break;

            case JackTokenType::JT_SYMBOL:
                if (tokenizer.getCurrToken() == "~")
                {
                    processTerminalReserved("~");
                    //compileExpression() should (supposedly) handle these symbols, as well as operator precedence involving unary operators
                    compileTerm();
                }
                else if (tokenizer.getCurrToken() == "-")
                {
                    processTerminalReserved("-");
                    compileTerm();
                }
                else if (tokenizer.getCurrToken() == "(")
                {
                    processTerminalReserved("(");
                    compileExpression();
                    processTerminalReserved(")");
                }
                break;
        }
        addLine("</term>");
    }

    void Compiler::compileExpressionList(){
        addLine("<expressionList>");
        //check if next token is the closing parenthesis, which corresponds to an empty expressionList
        if (! (tokenizer.getCurrToken() == ")") )
        {
            compileExpression();
            while (tokenizer.getCurrToken() == ",")
            {
                processTerminalReserved(",");
                compileExpression();
            }
        }
        addLine("</expressionList>");
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
            //todo: error handler
            throw "Invalid syntax! Expected " + token + +", read " + currToken;
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
            case JackTokenType::JT_STR_CONST:
                addLine("<constant>"+currToken+"</constant>");
                break;
            default:
                throw "Cannot process user-defined terminal!";
        }
        tokenizer.advanceToken();
    }
}