#pragma once

#include "FileGenerator.h"

namespace parsing
{

    FileGenerator::~FileGenerator()
    {
        outputFile.close();
    }

    void FileGenerator::addLine(const String& line)
    {
        if (outputFile.is_open())
        {
            outputFile << line << std::endl;
            /*
            if (outputFile.fail())
            {
                std::cout << "ERROR: Failed to write to file" << std::endl;
            }
            else
            {
                std::cout << "line has been written" << std::endl;
            }
            */
        }
    }

    void FileGenerator::openFile(const String& fileName)
    {
        if (outputFile.is_open())
        {
            closeFile();
        }
        outputFile.open(fileName+".vm", std::ios::out);
    }

    void FileGenerator::closeFile()
    {
        outputFile.close();
    }
}