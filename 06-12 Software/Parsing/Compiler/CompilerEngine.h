#include "JackCompiler.h"
#include "Tokenizer.cpp"
#include "CompilerSymbolTable.cpp"
#include "VMWriter.cpp"

namespace parsing::JackCompiler
{
    class CompilerEngine
    {
        public:

            /**
             * @brief Compiles the file from the first to last token detected
             * 
             * @param in - input filename or path
             */
            void compileFile(const String& in);
            
            /**
             * @brief generates VM instructions for a class declaration.
             * 
             */
            void compileClass();

            /**
             * @brief generates VM instructions for a class' field and static variable declaration.
             * 
             */
            void compileClassVariable();

            /**
             * @brief generates VM instructions for a subroutine definition.
             * 
             */
            void compileSubroutine();

            /**
             * @brief generates VM instructions for a subroutine's parameter list
             * 
             */
            void compileParameters();

            /**
             * @brief generates VM instructions for a subroutine's body
             * 
             */
            void compileSubroutineBody(const String& funcName);

            /**
             * @brief generates VM instructions for declaring variables
             * 
             */
            void compileVarDeclaration();

            /**
             * @brief generates VM instructions to perform statements
             * 
             */
            void compileStatements();

            /**
             * @brief generates VM instructions to perform a `let` statement
             * 
             */
            void compileLet();

            /**
             * @brief generates VM instructions to perform an `if` statement
             * 
             */
            void compileIf();

            /**
             * @brief generates VM instructions to perform a `while` statement
             * 
             */
            void compileWhile();

            /**
             * @brief generates VM instructions to perform a `do` statement
             * 
             */
            void compileDo();

            /**
             * @brief generates VM instructions to perform a `return` statement
             * 
             */
            void compileReturn();

            /**
             * @brief Compiles an expression 
             * 
             */
            void compileExpression();

            /**
             * @brief Compiles a term
             * 
             */
            void compileTerm();

            int compileExpressionList();

            VMWriter writer;

        private:

            void processIdentifier();

            void processIntConstant();

            void processStringConstant();

            void processKeyword();

            void processSymbol();

            void processUnaryOperator(const char& op);

            void processOperator(const char& op);

            bool expectKeyword(const String& keyword);

            bool expectSymbol(const char& symbol);

            symbolData getSymbolDataOf(const String& varName);

            String className;
            CompilerSymbolTable classSymbolTable;
            CompilerSymbolTable subroutineSymbolTable;
            JackKeyword currSubroutineType;
            String currReturntype;

            Tokenizer tokenizer;
            int numLabels = 0;
    };
}