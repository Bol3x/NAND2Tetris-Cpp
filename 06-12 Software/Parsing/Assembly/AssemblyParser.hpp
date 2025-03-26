#include "..\Parser.cpp"

class parsing::AssemblyParser : public parsing::Parser {
    public: 
        
        /**
         * @brief Construct a new AssemblyParser object.
         * 
         * Opens the input file/stream and gets ready to parse it.
         * @param filename assembly text filename
         */
        AssemblyParser(const String& filename);

        /**
         * @brief determines the type of command being performed by the current line
         * 
         * There are 3 types of commands performed by the Hack language:
         * 
         * 1. `A_COMMAND` - address register set, used for @xxx where xxx is either a symbol or a decimal number
         * 
         * 2. `C_COMMAND` - compute command, used to compute, assign, or jump given the current register values 
         * 
         * 3. `L_COMMAND` - symbol command, a pseudo-command to set new symbols with the syntax (xxx), where xxx is the symbol name
         * 
         * @return enum type of the parsed command
         */
        AssemblyCommand getCommandType();

        /**
         * @brief Returns the symbol or decimal xxx from @xxx or (xxx).
         * 
         * Must only be used if `commandType` is `A_COMMAND` or `L_COMMAND`
         * 
         * @return extracted symbol or decimal as string
         */
        String getSymbol();
        
        /**
         * @brief Loads all the symbols detected in the file into the SymbolTable
         * 
         */
        void loadSymbols();

        /**
         * @brief Returns the destination register of the computation
         * 
         * Must only be used if `commandType` is `C_COMMAND`
         * 
         * @return extracted destination register as string
         * @throws
         */
        String getDest();

        /**
         * @brief Returns compute command (RHS) of line
         * 
         * Must only be used if `commandType` is `C_COMMAND`
         * 
         * @return extracted compute command as string
         */
        String getComp();

        /**
         * @brief Returns jump type of computation
         * 
         * Must only be used if `commandType` is `C_COMMAND`
         * 
         * @return extracted jump type of command 
         */
        String getJump();

        private:
            String filename;
            SymbolTable table;
};