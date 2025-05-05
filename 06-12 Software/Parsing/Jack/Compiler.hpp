#include "JackCompiler.hpp"
#include "FileGenerator.cpp"

namespace parsing::JackCompiler
{
    class Compiler : public FileGenerator
    {

        /**
         * @brief Creates a new Compiler object to produce VM instructions
         * 
         * @param filename - file output of the compiler
         */
        Compiler(const String& filename);

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
        void compileSubroutineBody();

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
    };
}