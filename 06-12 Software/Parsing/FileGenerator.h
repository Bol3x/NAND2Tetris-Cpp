#pragma once

#include "Parsing.h"

class parsing::FileGenerator
{
    public:
        /**
         * @brief Constructor for a FileGenerator object, outputting lines to the given filename
         * 
         * @param filename
         */
        FileGenerator(const String& filename);

        ~FileGenerator();

        /**
         * @brief Adds a line to the current output file stream
         * 
         * @param features - binary of command
         */
        void addLine(const String& binary);

        /**
         * @brief Closes outputfile.
         */
        void closeFile();

    protected:
        OutputFile outputFile;
};