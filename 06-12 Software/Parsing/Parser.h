#include "Parsing.h"

class parsing::Parser{
    public:

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
         * @brief Opens a file.
         * 
         * @param file 
         */
        void openFile(const String& file);

        /**
         * @brief Closes inputfile.
         * 
         */
        void closeFile();
    
    protected:
        InputFile inputFile;
        String currLine = "";
};