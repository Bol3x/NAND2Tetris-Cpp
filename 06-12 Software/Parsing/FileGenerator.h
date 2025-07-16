#pragma once

#include "Parsing.h"

class parsing::FileGenerator
{
    public:

        /**
         * @brief Ensure that the file opened is closed if the object is destroyed
         * 
         */
        ~FileGenerator();

        /**
         * @brief Adds a line to the current output file stream
         * 
         * @param features - binary of command
         */
        void addLine(const String& binary);

        /**
         * @brief Opens/Creates a new file to output to
         * 
         * Make sure that there is no current file still open
         * 
         */
        void openFile(const String& filename);

        /**
         * @brief Closes outputfile.
         */
        void closeFile();

    protected:
        OutputFile outputFile;
};