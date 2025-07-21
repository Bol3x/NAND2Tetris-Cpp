#include "CompilerEngine.h"

namespace parsing::JackCompiler
{

    void CompilerEngine::compileFile(const String& in)
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

    void CompilerEngine::compileClass()
    {
        addLine("<class>");
        expectKeyword("class");
        className = tokenizer.getIdentifier();
        processIdentifier();       //class name
        expectSymbol('{');

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
        expectSymbol('}');
        addLine("</class>");
    }

    void CompilerEngine::compileClassVariable(){
        String varName;
        JackCompilerType segment;
        String type;
        char symbol;
        addLine("<classVarDec>");
        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
        {
            if (tokenizer.getKeyword() == JackKeyword::JK_FIELD)
            {
                segment = JackCompilerType::JC_FIELD;
                expectKeyword("field");
                
            }
            else if (tokenizer.getKeyword() == JackKeyword::JK_STATIC)
            {
                segment = JackCompilerType::JC_STATIC;
                expectKeyword("static");
            }
        }

        type = tokenizer.getCurrToken();
        //todo: can split this into a seperate compileVariableType function (as subroutines also do this)
        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
        {
            processKeyword();
        }
        else if (tokenizer.getTokenType() == JackTokenType::JT_IDENTIFIER)
        {
            processIdentifier();
        }

        //loop for as many variables possibly declared
        do{
            varName = tokenizer.getIdentifier();
            processIdentifier();   //varName
            symbol = tokenizer.getSymbol();
            
            //add variable to class' symbol table for future reference
            classSymbolTable.addSymbol(varName, type, segment);

            addLine("<type>" + type + "</type>");
            if (segment == JackCompilerType::JC_FIELD)
            {
                addLine("<segment>field</segment>");
            }
            else
            {
                addLine("<segment>static</segment>");
            }

            addLine("<index>"+std::to_string(classSymbolTable.getIndexOf(varName))+"</index>");
            addLine("<use>declared</use>");
            
            processSymbol();
        } while(symbol == ',');
        addLine("</classVarDec>");
    }

    void CompilerEngine::compileSubroutine(){
        subroutineSymbolTable.clearTable();

        addLine("<subroutineDec>");
        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
        {
            switch(tokenizer.getKeyword()){
                case JackKeyword::JK_CONSTRUCTOR:
                    expectKeyword("constructor");
                    break;
                case JackKeyword::JK_FUNCTION:
                    expectKeyword("function");
                    break;
                case JackKeyword::JK_METHOD:
                    expectKeyword("method");
                    subroutineSymbolTable.addSymbol("this", className, JackCompilerType::JC_ARG);
            }
        }

        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
        {
            switch(tokenizer.getKeyword()){
                case JackKeyword::JK_VOID:
                    expectKeyword("void");
                    break;
                case JackKeyword::JK_INT:
                    expectKeyword("int");
                    break;
                case JackKeyword::JK_BOOLEAN:
                    expectKeyword("boolean");
                    break;
                case JackKeyword::JK_CHAR:
                    expectKeyword("char");
            }
        }
        else
        {
            processIdentifier(); //a user-defined class
        }

        processIdentifier();   //subroutine identifier
        expectSymbol('(');
        compileParameters();
        expectSymbol(')');
        compileSubroutineBody();
        addLine("</subroutineDec>");
    }

    void CompilerEngine::compileParameters(){
        addLine("<parameterList>");

        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD){
            //process any more parameter definitions
            char symbol;
            String varType;
            String varName;
            do{
                varType = tokenizer.getCurrToken();
                if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
                {
                    switch(tokenizer.getKeyword()){
                        case JackKeyword::JK_INT:
                            expectKeyword("int");
                            break;
                        case JackKeyword::JK_BOOLEAN:
                            expectKeyword("boolean");
                            break;
                        case JackKeyword::JK_CHAR:
                            expectKeyword("char");
                    }
                }
                else
                {
                    processIdentifier(); //a user-defined class
                }

                varName = tokenizer.getIdentifier();
                processIdentifier();   //varName

                subroutineSymbolTable.addSymbol(varName,varType, JackCompilerType::JC_ARG);
                
                addLine("<type>" + varType + "</type>");
                addLine("<segment>argument</segment>");
                addLine("<index>"+std::to_string(subroutineSymbolTable.getIndexOf(varName))+"</index>");
                addLine("<use>declared</use>");

                symbol = tokenizer.getSymbol();
                if (symbol != ')'){
                    processSymbol();
                }

            } while (symbol == ',');
        }

        addLine("</parameterList>");
    }

    void CompilerEngine::compileSubroutineBody(){
        addLine("<subroutineBody>");
        expectSymbol('{');

        //variable declarations
        while (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD && tokenizer.getKeyword() == JackKeyword::JK_VAR)
        {
            compileVarDeclaration();
        }

        //statements
        compileStatements();
        expectSymbol('}');
        addLine("</subroutineBody>");
    }

    void CompilerEngine::compileVarDeclaration(){
        addLine("<varDec>");
        expectKeyword("var");

        String varName;
        String varType = tokenizer.getCurrToken();

        //if token is a keyword (ie. defined types)
        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
        {
            switch(tokenizer.getKeyword()){
                case JackKeyword::JK_INT:
                    expectKeyword("int");
                    break;
                case JackKeyword::JK_BOOLEAN:
                    expectKeyword("boolean");
                    break;
                case JackKeyword::JK_CHAR:
                    expectKeyword("char");
                    break;
            }
        }
        //if token refers to a class object
        else 
        {
            processIdentifier();
        }

        char symbol;
        do{
            varName = tokenizer.getIdentifier();
            processIdentifier();   //varName

            subroutineSymbolTable.addSymbol(varName, varType, JackCompilerType::JC_VAR);
            
            addLine("<type>" + varType + "</type>");
            addLine("<segment>variable</segment>");
            addLine("<index>"+std::to_string(subroutineSymbolTable.getIndexOf(varName))+"</index>");
            addLine("<use>declared</use>");

            symbol = tokenizer.getSymbol();   //',' or ';'
            processSymbol();
        } while(symbol == ',');
        
        addLine("</varDec>");
    }

    void CompilerEngine::compileStatements(){
        addLine("<statements>");
        JackKeyword keyword;
            while (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD){
                keyword = tokenizer.getKeyword();
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

    void CompilerEngine::compileLet(){
        addLine("<LetStatement>");
        expectKeyword("let");
        processIdentifier();
        //look ahead if a array definition
        if (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == '['){
            expectSymbol('[');
            compileExpression();
            expectSymbol(']');
        }
        expectSymbol('=');
        compileExpression();
        expectSymbol(';');
        addLine("</LetStatement>");
    }

    void CompilerEngine::compileIf(){
        addLine("<ifStatement>");
        expectKeyword("if");
        expectSymbol('(');
        compileExpression();
        expectSymbol(')');
        expectSymbol('{');
        compileStatements();
        expectSymbol('}');
        //look ahead for else
        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD && tokenizer.getKeyword() == JackKeyword::JK_ELSE){
            expectKeyword("else");
            expectSymbol('{');
            compileStatements();
            expectSymbol('}');
        }
        addLine("</ifStatement>");
    }

    void CompilerEngine::compileWhile(){
        addLine("<whileStatement>");
        expectKeyword("while");
        expectSymbol('(');
        compileExpression();
        expectSymbol(')');
        expectSymbol('{');
        compileStatements();
        expectSymbol('}');
        addLine("</whileStatement>");
    }

    void CompilerEngine::compileDo(){
        addLine("<doStatement>");
        expectKeyword("do");
        //subroutine call
        processIdentifier();
        //loop through possibly nested object attributes
        while (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == '.'){
            expectSymbol('.');
            processIdentifier();
            //todo: nested function call functionality (ie. Square.getPoint().getx())
        }
        expectSymbol('(');
        compileExpressionList();
        expectSymbol(')');
        //end of subroutine call
        expectSymbol(';');
        addLine("</doStatement>");
    }

    void CompilerEngine::compileReturn(){
        addLine("<returnStatement>");
        expectKeyword("return");
        compileExpression();
        expectSymbol(';');
        addLine("</returnStatement>");
    }

    void CompilerEngine::compileExpression(){
        //todo: use a modified recursive descent parser to make use of the operator precedence
        //check RDP.cpp for the general idea of using trees to generate the order
        addLine("<expression>");
        compileTerm();
        char op = tokenizer.getSymbol();
        while (operators.find(op) != operators.end())
        {
            processSymbol();
            compileTerm();

            op = tokenizer.getSymbol();
        }
        addLine("</expression>");
    }

    void CompilerEngine::compileTerm(){
        addLine("<term>");
        switch(tokenizer.getTokenType())
        {
            case JackTokenType::JT_INT_CONST:
                processIntConstant();
                break;
            case JackTokenType::JT_STR_CONST:
                processStringConstant();
                break;

            case JackTokenType::JT_KEYWORD:
                if (tokenizer.getKeyword() == JackKeyword::JK_TRUE)
                {
                    expectKeyword("true");
                }
                else if (tokenizer.getKeyword() == JackKeyword::JK_FALSE)
                {
                    expectKeyword("false");
                }
                else if (tokenizer.getKeyword() == JackKeyword::JK_NULL)
                {
                    expectKeyword("null");
                }
                else if (tokenizer.getKeyword() == JackKeyword::JK_THIS)
                {
                    expectKeyword("this");
                }
                break;

            case JackTokenType::JT_IDENTIFIER:
                processIdentifier();

                //can be an array
                if (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == '[')
                {
                    expectSymbol('[');
                    compileExpression();
                    expectSymbol(']');
                }
                //can be a subroutine call
                else 
                { 
                    //can also be in a nested object
                    while (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == '.'){
                        expectSymbol('.');
                        processIdentifier();
                    }

                    if (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == '('){
                        expectSymbol('(');
                        compileExpressionList();
                        expectSymbol(')');
                    }
                }
                break;

            case JackTokenType::JT_SYMBOL:
                if (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && (tokenizer.getSymbol() == '~' || tokenizer.getSymbol() == '-'))
                {
                    processSymbol();
                    //compileExpression() should (supposedly) handle these symbols, as well as operator precedence involving unary operators
                    compileTerm();
                }
                else if (tokenizer.getCurrToken() == "(")
                {
                    expectSymbol('(');
                    compileExpressionList();
                    expectSymbol(')');
                }
                break;
        }
        addLine("</term>");
    }

    void CompilerEngine::compileExpressionList(){
        addLine("<expressionList>");
        //check if next token is the closing parenthesis, which corresponds to an empty expressionList
        if ( !(tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == ')') )
        {
            compileExpression();
            while (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == ',')
            {
                expectSymbol(',');
                compileExpression();
            }
        }
        addLine("</expressionList>");
    }


    void CompilerEngine::processIdentifier()
    {
        String varName = tokenizer.getIdentifier();
        addLine("<identifier>");
        addLine(varName);
        addLine("</identifier>");
        if (subroutineSymbolTable.isVarInTable(varName))
        {
            addLine("<type>" + subroutineSymbolTable.getDataTypeOf(varName) + "</type>");
            switch(subroutineSymbolTable.getVMKindOf(varName))
            {
                case JackCompilerType::JC_ARG:
                    addLine("<segment>argument</segment>");
                    break;

                case JackCompilerType::JC_VAR:
                    addLine("<segment>variable</segment>");
                    break;
                
                default:
                    addLine("<segment>none</segment>");
            }
            addLine("<index>"+ std::to_string(subroutineSymbolTable.getIndexOf(varName)) +"</index>");
            addLine("<use>used</use>");
        } 
        else if (classSymbolTable.isVarInTable(varName))
        {
            switch(classSymbolTable.getVMKindOf(varName))
            {
                case JackCompilerType::JC_FIELD:
                    addLine("<segment>field</segment>");
                    break;

                case JackCompilerType::JC_STATIC:
                    addLine("<segment>static</segment>");
                    break;

                default:
                    addLine("<segment>none</segment>");
            }
            addLine("<index>"+ std::to_string(classSymbolTable.getIndexOf(varName)) +"</index>");
            addLine("<use>used</use>");
        }

        tokenizer.advanceToken();
    }

    void CompilerEngine::processIntConstant()
    {
        addLine("<constant>");
        addLine(std::to_string(tokenizer.getIntValue()));
        addLine("</constant>");

        tokenizer.advanceToken();
    }

    void CompilerEngine::processStringConstant()
    {
        addLine("<constant>");
        addLine(tokenizer.getStringValue());
        addLine("</constant>");

        tokenizer.advanceToken();
    }

    void CompilerEngine::processKeyword()
    {
        addLine("<keyword>");
        addLine(tokenizer.getCurrToken());
        addLine("</keyword>");

        tokenizer.advanceToken();
    }

    void CompilerEngine::processSymbol()
    {
        addLine("<symbol>");
        addLine(String(1, tokenizer.getSymbol()));
        addLine("</symbol>");

        tokenizer.advanceToken();
    }

    bool CompilerEngine::expectKeyword(const String& keyword)
    {
        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD && keyword == tokenizer.getCurrToken())
        {
            processKeyword();
            return true;
        }

        throw "Expected " + keyword + ", read " + tokenizer.getCurrToken();
        return false;
    }

    bool CompilerEngine::expectSymbol(const char& symbol)
    {
        if (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && symbol == tokenizer.getSymbol())
        {
            processSymbol();
            return true;
        }

        throw "Expected " + String(1,symbol) + ", read " + tokenizer.getCurrToken();
        return false;
    }
}