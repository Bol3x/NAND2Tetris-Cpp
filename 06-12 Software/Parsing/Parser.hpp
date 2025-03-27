#include "Parsing.hpp"
#include "Assembly\SymbolTable.cpp"

class parsing::Parser{
    public:

        /**
         * @brief Construct a new Parser object.
         * 
         * Opens the input file/stream and gets ready to parse it.
         * @param filename assembly text filename
         */
        Parser(const String& filename);

        ~Parser();

        /**
         * @brief Get the currLine of Parser
         * 
         * @return String 
         */
        String getLine();

        /**
         * @brief Checks if there are more commands to parse from the input file.
         * 
         * @return true or
         * @return false
         */
        bool hasMoreCommands();

        /**
         * @brief advances the text file to the next line to parse.
         * 
         * Only advances if `hasMoreCommands()` returns `true`.
         */
        void advanceLine();

        /**
         * @brief Closes inputfile.
         * 
         */
        void closeFile();
    
    protected:
        InputFile inputFile;
        String currLine;
};