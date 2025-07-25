#include "CompilerEngine.h"

namespace parsing::JackCompiler
{

    void CompilerEngine::compileFile(const String& in)
    {
        //std::cout << "compiling file" << std::endl;
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
        //std::cout << "compiling class" << std::endl;
        numLabels = 0;
        //addline("<class>");
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
        //addline("</class>");
    }

    void CompilerEngine::compileClassVariable(){
        //std::cout << "compiling class variables" << std::endl;
        String varName;
        JackCompilerType segment;
        String type;
        char symbol;
        //addline("<classVarDec>");
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

            //addline("<type>" + type + "</type>");
            if (segment == JackCompilerType::JC_FIELD)
            {
                //addline("<segment>field</segment>");
            }
            else
            {
                //addline("<segment>static</segment>");
            }

            //addline("<index>"+std::to_string(classSymbolTable.getIndexOf(varName))+"</index>");
            //addline("<use>declared</use>");
            
            processSymbol();
        } while(symbol == ',');
        //addline("</classVarDec>");
    }

    void CompilerEngine::compileSubroutine()
    {
        //std::cout << "compiling subroutine" << std::endl;
        String funcName;
        int numVars;

        //addline("<subroutineDec>");
        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD)
        {
            currSubroutineType = tokenizer.getKeyword();
            switch(currSubroutineType){
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
            switch(tokenizer.getKeyword())
            {
                case JackKeyword::JK_VOID:
                    currReturntype = "void";
                    expectKeyword("void");
                    break;
                case JackKeyword::JK_INT:
                    currReturntype = "int";
                    expectKeyword("int");
                    break;
                case JackKeyword::JK_BOOLEAN:
                    currReturntype = "boolean";
                    expectKeyword("boolean");
                    break;
                case JackKeyword::JK_CHAR:
                    currReturntype = "char";
                    expectKeyword("char");
            }
        }
        else
        {
            currReturntype = tokenizer.getIdentifier();
            processIdentifier(); //a user-defined class
        }

        funcName = tokenizer.getIdentifier();
        processIdentifier();   //subroutine identifier
        expectSymbol('(');
        compileParameters();
        expectSymbol(')');

        expectSymbol('{');
        compileSubroutineBody(funcName);        
        expectSymbol('}');

        //addline("</subroutineDec>");
        subroutineSymbolTable.clearTable();
    }

    void CompilerEngine::compileParameters(){
        //std::cout << "compiling parameters" << std::endl;
        //addline("<parameterList>");

        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD || tokenizer.getTokenType() == JackTokenType::JT_IDENTIFIER){
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
                
                //addline("<type>" + varType + "</type>");
                //addline("<segment>argument</segment>");
                //addline("<index>"+std::to_string(subroutineSymbolTable.getIndexOf(varName))+"</index>");
                //addline("<use>declared</use>");

                symbol = tokenizer.getSymbol();
                if (symbol != ')'){
                    processSymbol();
                }

            } while (symbol == ',');
        }

        //addline("</parameterList>");
    }

    void CompilerEngine::compileSubroutineBody(const String& funcName){
        //std::cout << "compiling subroutine body" << std::endl;
        //addline("<subroutineBody>");

        //variable declarations
        while (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD && tokenizer.getKeyword() == JackKeyword::JK_VAR)
        {
            compileVarDeclaration();
        }

        writer.writeFunctionDeclaration(className+"."+funcName, subroutineSymbolTable.getSymbolCountOf(JackCompilerType::JC_VAR));

        if (currSubroutineType == JackKeyword::JK_METHOD)
        {
            writer.writePushInstruction(JackVMSegment::JVT_ARGUMENT, 0);
            writer.writePopInstruction(JackVMSegment::JVT_POINTER, 0);
        } 
        else if (currSubroutineType == JackKeyword::JK_CONSTRUCTOR)
        {
            writer.writePushInstruction(JackVMSegment::JVT_CONSTANT, classSymbolTable.getSymbolCountOf(JackCompilerType::JC_FIELD));
            writer.writeCallInstruction("Memory.alloc", 1);         //allocate field variable memory
            writer.writePopInstruction(JackVMSegment::JVT_POINTER, 0);   //returns allocated memory location
        }

        //rest of the subroutine's statements
        compileStatements();
        //addline("</subroutineBody>");
    }

    void CompilerEngine::compileVarDeclaration(){
        //std::cout << "compiling variable declarations" << std::endl;
        //addline("<varDec>");
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
            
            //addline("<type>" + varType + "</type>");
            //addline("<segment>variable</segment>");
            //addline("<index>"+std::to_string(subroutineSymbolTable.getIndexOf(varName))+"</index>");
            //addline("<use>declared</use>");

            symbol = tokenizer.getSymbol();   //',' or ';'
            processSymbol();
        } while(symbol == ',');
        
        //addline("</varDec>");
    }

    void CompilerEngine::compileStatements(){
        //std::cout << "compiling statements" << std::endl;
        //addline("<statements>");
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
                return;

            default:
                throw "Invalid statements!";

            }
            if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD){
                keyword = tokenizer.getKeyword();
            }
        }
        //addline("</statements>");
    }

    void CompilerEngine::compileLet(){
        //std::cout << "compiling let statement" << std::endl;
        bool isArray = false;
        //addline("<LetStatement>");
        expectKeyword("let");

        String identifier = tokenizer.getIdentifier();
        symbolData data = getSymbolDataOf(identifier);
        tokenizer.advanceToken();   
        //processIdentifier();
        //look ahead if a array definition
        if (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == '['){
            isArray = true;

            //process the identifier we saved as the array's base address
            writer.writePushInstruction(compilerTypeToVMSegment.at(data.kind), data.index);

            expectSymbol('[');
            compileExpression();    //expression 1
            expectSymbol(']');

            writer.writeArithmeticInstruction(JackVMCommand::JVC_ADD); //(arr + expr1)
        }
        expectSymbol('=');
        compileExpression();    //expression 2 (value to be stored)
        expectSymbol(';');
        //addline("</LetStatement>");

        if (isArray)
        {
            writer.writePopInstruction(JackVMSegment::JVT_TEMP, 0); //store expr2 in temp
            writer.writePopInstruction(JackVMSegment::JVT_POINTER, 1); //set array pointer
            writer.writePushInstruction(JackVMSegment::JVT_TEMP, 0); //retrieve expr2
            writer.writePopInstruction(JackVMSegment::JVT_THAT, 0); //finally store in array location
        }
        //if not an array simply pop value into pointed memory location (defined by symbol table)
        else 
        {
            writer.writePopInstruction(compilerTypeToVMSegment.at(data.kind), data.index);
        }
    }

    void CompilerEngine::compileIf(){
        //std::cout << "compiling if statement" << std::endl;
        int label = numLabels;
        //addline("<ifStatement>");
        expectKeyword("if");
        expectSymbol('(');
        compileExpression();
        writer.writeArithmeticInstruction(JackVMCommand::JVC_NOT); //so that if-goto is true (jumps only if expression is false)
        writer.writeIfInstruction(className+"_L"+std::to_string(label));
        numLabels++;
        expectSymbol(')');
        expectSymbol('{');
        compileStatements();
        expectSymbol('}');


        //look ahead for else
        if (tokenizer.getTokenType() == JackTokenType::JT_KEYWORD && tokenizer.getKeyword() == JackKeyword::JK_ELSE){
            int label2 = numLabels;
            writer.writeGotoInstruction(className+"_L"+std::to_string(label2));
            numLabels++;
            writer.writeLabel(className+"_L"+std::to_string(label));
            expectKeyword("else");
            expectSymbol('{');
            compileStatements();
            expectSymbol('}');
            writer.writeLabel(className+"_L"+std::to_string(label2));
        }
        else 
        {
            writer.writeLabel(className+"_L"+std::to_string(label));
        }
        

        //addline("</ifStatement>");
    }

    void CompilerEngine::compileWhile(){
        //std::cout << "compiling while statement" << std::endl;
        int label = numLabels;
        int label2;
        //addline("<whileStatement>");
        expectKeyword("while");
        expectSymbol('(');
        writer.writeLabel(className+"_L"+std::to_string(label));
        label2 = ++numLabels;
        compileExpression();
        expectSymbol(')');
        writer.writeArithmeticInstruction(JackVMCommand::JVC_NOT);
        writer.writeIfInstruction(className+"_L"+std::to_string(label2));
        numLabels++;
        expectSymbol('{');
        compileStatements();
        expectSymbol('}');
        writer.writeGotoInstruction(className+"_L"+std::to_string(label));
        writer.writeLabel(className+"_L"+std::to_string(label2));
        //addline("</whileStatement>");
    }

    void CompilerEngine::compileDo(){
        //std::cout << "compiling do statement" << std::endl;
        String identifier;
        String funcName;
        int numExpr;
        //addline("<doStatement>");
        expectKeyword("do");
        //get identifier of either (className, varName, methodName)
        identifier = tokenizer.getIdentifier();
        processIdentifier();

        //case: className or varName
        //if it is a method, process its identifier too
        if (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == '.'){
            expectSymbol('.');

            funcName = tokenizer.getIdentifier();
            processIdentifier();
        }

        //case: methodName
        //if method of the same class, pass current object to the calling function
        if (funcName.empty())
        {
            writer.writePushInstruction(JackVMSegment::JVT_POINTER, 0);
        }
        

        expectSymbol('(');
        numExpr = compileExpressionList();
        expectSymbol(')');
        //end of subroutine call
        expectSymbol(';');
        //addline("</doStatement>");

        if (! funcName.empty())
        {
            //if subroutine is a class method
            if ((classSymbolTable.isVarInTable(identifier) || subroutineSymbolTable.isVarInTable(identifier)))
            {
                writer.writeCallInstruction(getSymbolDataOf(identifier).datatype+"."+funcName, numExpr+1);
            }
            //if subroutine is a function of another class
            else
            {
                writer.writeCallInstruction(identifier+"."+funcName, numExpr);
            }
        }
        else 
        {
            //if subroutine is a method of the current class
            writer.writeCallInstruction(className+"."+identifier, numExpr+1);
        }
        
        //ignore return value by popping into temp
        writer.writePopInstruction(JackVMSegment::JVT_TEMP, 0);
    }

    void CompilerEngine::compileReturn(){
        //std::cout << "compiling return statement" << std::endl;
        //addline("<returnStatement>");
        expectKeyword("return");
        if (! (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == ';'))
        {
            compileExpression();
        }
        expectSymbol(';');

        if (currReturntype == "void") 
        {
            writer.writePushInstruction(JackVMSegment::JVT_CONSTANT, 0);
        }

        //VM will pop stack head onto return frame address
        writer.writeReturnInstruction();
        //addline("</returnStatement>");
    }

    void CompilerEngine::compileExpression(){
        //std::cout << "compiling expression" << std::endl;
        //todo: use a modified recursive descent parser to make use of the operator precedence
        //check RDP.cpp for the general idea of using trees to generate the order
        //addline("<expression>");
        compileTerm();
        char op = tokenizer.getSymbol();
        while (operators.find(op) != operators.end())
        {
            processSymbol();
            compileTerm();

            //VM processes operators in postfix manner
            processOperator(op);

            op = tokenizer.getSymbol();
        }
        //addline("</expression>");
        //std::cout << "end compile expression" << std::endl;
    }

    void CompilerEngine::compileTerm(){
        //std::cout << "compiling term" << std::endl;
        //addline("<term>");
        char op;
        String funcName;
        String identifier;
        int numExpr;
        switch(tokenizer.getTokenType())
        {
            case JackTokenType::JT_INT_CONST:
                //std::cout << "compiling int const" << std::endl;
                processIntConstant();
                break;
            case JackTokenType::JT_STR_CONST:
                //std::cout << "compiling string const" << std::endl;
                processStringConstant();
                break;

            case JackTokenType::JT_KEYWORD:
                //std::cout << "compiling keyword const" << std::endl;
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
                //std::cout << "processing identifier" << std::endl;
                identifier = tokenizer.getIdentifier();
                processIdentifier();

                //can be an array
                if (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == '[')
                {
                    expectSymbol('[');
                    compileExpression();
                    expectSymbol(']');
                    
                    writer.writeArithmeticInstruction(JackVMCommand::JVC_ADD);
                    writer.writePopInstruction(JackVMSegment::JVT_POINTER, 1);
                    writer.writePushInstruction(JackVMSegment::JVT_THAT, 0);
                }
                //can be a function call of current class
                else if (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == '(')
                {
                    //case: methodName
                    //if method of the same class, pass current object to the calling function
                    writer.writePushInstruction(JackVMSegment::JVT_POINTER, 0);

                    expectSymbol('(');
                    numExpr = compileExpressionList();
                    expectSymbol(')');

                    writer.writeCallInstruction(className+"."+identifier, numExpr+1);
                }
                //can also be a method in a nested object within the class fields
                else if (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == '.')
                {
                    expectSymbol('.');
                    funcName = tokenizer.getIdentifier();
                    processIdentifier();

                    expectSymbol('(');
                    numExpr = compileExpressionList();
                    expectSymbol(')');

                    //possibly a method from a class variable
                    if (subroutineSymbolTable.isVarInTable(identifier) || classSymbolTable.isVarInTable(identifier))
                    {
                        writer.writeCallInstruction(getSymbolDataOf(identifier).datatype+"."+funcName, numExpr+1);
                        //+1 for implied object passed in method arguments
                    }
                    //or a class function (where identifier = className)
                    else
                    {
                        writer.writeCallInstruction(identifier+"."+funcName, numExpr);
                    }
                }
                break;

            case JackTokenType::JT_SYMBOL:
                //std::cout << "compiling symbol" << std::endl;
                if (tokenizer.getSymbol() == '~' || tokenizer.getSymbol() == '-')
                {
                    op = tokenizer.getSymbol();
                    processSymbol();
                    //compileExpression() should (supposedly) handle these symbols, as well as operator precedence involving unary operators
                    compileTerm();
                    processUnaryOperator(op);
                }
                else if (tokenizer.getSymbol() == '(')
                {
                    //std::cout << "expecting expression in parenthesis" << std::endl;
                    expectSymbol('(');
                    compileExpression();
                    expectSymbol(')');
                }
                break;

            default:
                return;
        }
        //std::cout << "end compile term" << std::endl;
        //addline("</term>");
    }

    int CompilerEngine::compileExpressionList(){
        //std::cout << "compiling expression list" << std::endl;
        int numExpr = 0;
        //addline("<expressionList>");
        //check if next token is the closing parenthesis, which corresponds to an empty expressionList
        if ( !(tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == ')') )
        {
            compileExpression();
            numExpr++;
            while (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && tokenizer.getSymbol() == ',')
            {
                expectSymbol(',');
                compileExpression();
                numExpr++;
            }
        }
        //addline("</expressionList>");
        return numExpr;
    }

    void CompilerEngine::processIdentifier()
    {
        String varName = tokenizer.getIdentifier();
        //addline("<identifier>");
        //addLine(varName);
        //addline("</identifier>");
        symbolData data = getSymbolDataOf(varName);
        

        if (subroutineSymbolTable.isVarInTable(varName) || classSymbolTable.isVarInTable(varName))
        {
            //addline("<type>" + data.datatype + "</type>");
            //addline("<segment>"+compilerTypeToVMString.at(data.kind)+"</segment>");
            //addline("<index>"+ std::to_string(data.index) +"</index>");

            writer.writePushInstruction(compilerTypeToVMSegment.at(data.kind), data.index);
            //addline("<use>used</use>");
        } 

        tokenizer.advanceToken();
    }

    void CompilerEngine::processIntConstant()
    {
        writer.writePushInstruction(JackVMSegment::JVT_CONSTANT, tokenizer.getIntValue());

        tokenizer.advanceToken();
    }

    void CompilerEngine::processStringConstant()
    {
        //use String built-in classes to parse strings
        String str = tokenizer.getStringValue();
        int i;
        writer.writePushInstruction(JackVMSegment::JVT_CONSTANT, str.length()-2);
        writer.writeCallInstruction("String.new", 1);
        for (i = 1; i < str.length()-1; i++)
        {
            short ch = str[i];
            writer.writePushInstruction(JackVMSegment::JVT_CONSTANT, ch);
            writer.writeCallInstruction("String.appendChar", 2);
        }

        //addline("<constant>");
        //addLine(str);
        //addline("</constant>");

        tokenizer.advanceToken();
    }

    void CompilerEngine::processKeyword()
    {
        //if constant keywords (true, false, this, null)
        switch(tokenizer.getKeyword())
        {
            case JackKeyword::JK_NULL:
            case JackKeyword::JK_FALSE:
                writer.writePushInstruction(JackVMSegment::JVT_CONSTANT, 0);
                break;

            case JackKeyword::JK_TRUE:
                writer.writePushInstruction(JackVMSegment::JVT_CONSTANT, 1);
                writer.writeArithmeticInstruction(JackVMCommand::JVC_NEG);
                break;

            case JackKeyword::JK_THIS:
                writer.writePushInstruction(JackVMSegment::JVT_POINTER, 0);
                break;
        }

        //addline("<keyword>");
        //addLine(tokenizer.getCurrToken());
        //addline("</keyword>");

        tokenizer.advanceToken();
    }

    void CompilerEngine::processSymbol()
    {
        //addline("<symbol>");
        //addLine(String(1, tokenizer.getSymbol()));
        //addline("</symbol>");

        tokenizer.advanceToken();
    }

    void CompilerEngine::processUnaryOperator(const char& op)
    {
        if (op == '-')
        {
            writer.writeArithmeticInstruction(JackVMCommand::JVC_NEG);
        } 
        else if (op == '~')
        {
            writer.writeArithmeticInstruction(JackVMCommand::JVC_NOT);
        }
        
    }

    void CompilerEngine::processOperator(const char& op)
    {
        switch (op)
        {
            case '=':
                writer.writeArithmeticInstruction(JackVMCommand::JVC_EQ);
                break;
            case '>':
                writer.writeArithmeticInstruction(JackVMCommand::JVC_GT);
                break;
            case '<':
                writer.writeArithmeticInstruction(JackVMCommand::JVC_LT);
                break;
            case '+':
                writer.writeArithmeticInstruction(JackVMCommand::JVC_ADD);
                break;
            case '-':
                writer.writeArithmeticInstruction(JackVMCommand::JVC_SUB);
                break;
            case '&':
                writer.writeArithmeticInstruction(JackVMCommand::JVC_AND);
                break;
            case '|':
                writer.writeArithmeticInstruction(JackVMCommand::JVC_OR);
                break;
            
            //mul/div are part of the Math built-in class
            case '*':
                writer.writeCallInstruction("Math.multiply", 2);
                break;
            case '/':
                writer.writeCallInstruction("Math.divide", 2);
                break;
        }
    }

    bool CompilerEngine::expectKeyword(const String& keyword)
    {
        //std::cout << "expecting " + keyword << std::endl;
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
        //std::cout << "expecting " + String(1, symbol) << std::endl;
        if (tokenizer.getTokenType() == JackTokenType::JT_SYMBOL && symbol == tokenizer.getSymbol())
        {
            processSymbol();
            return true;
        }

        throw "Expected " + String(1,symbol) + ", read " + tokenizer.getCurrToken();
        return false;
    }

    symbolData CompilerEngine::getSymbolDataOf(const String& varName)
    {
        symbolData result;

        //check subroutine table first
        if (subroutineSymbolTable.isVarInTable(varName))
        {
            result.kind = subroutineSymbolTable.getVMKindOf(varName);
            result.datatype = subroutineSymbolTable.getDataTypeOf(varName);
            result.index = subroutineSymbolTable.getIndexOf(varName);
        }
        else
        {
            result.kind = classSymbolTable.getVMKindOf(varName);
            result.datatype = classSymbolTable.getDataTypeOf(varName);
            result.index = classSymbolTable.getIndexOf(varName);
        }

        return result;
    }
}