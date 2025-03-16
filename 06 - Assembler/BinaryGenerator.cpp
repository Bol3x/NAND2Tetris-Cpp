#include "BinaryGenerator.hpp"

namespace parsing{
        BinaryGenerator::BinaryGenerator(const String& filename){
            outputFile.open(filename, std::ios_base::ate);
            compList = CommandList();
            jumpList = JumpList();
        }

        String BinaryGenerator::generateCompBinary(const String& comp, const String& dest, const String& jump)
        {
            String iBits = "111";
            String aBit = comp.find('M') != comp.npos ? "1" : "0";
            String cBits, dBits = "", jBits = "";

            if (compList.isValidCode(comp))
            {
                cBits = compList.getCode(comp);
            }
            else
            {
                //todo error check
                cBits = "000000";
            }

            dBits.append(dest.find('A') != dest.npos ? "1" : "0");
            dBits.append(dest.find('D') != dest.npos ? "1" : "0");
            dBits.append(dest.find('M') != dest.npos ? "1" : "0");
            

            if (jumpList.isValidCode(jump))
            {
                jBits = jumpList.getCode(jump);
            }
            else
            {
                jBits = "000";
            }

            return iBits + aBit + cBits + dBits + jBits;
        }

        String BinaryGenerator::generateAddressBinary(const String& symbol)
        {
            std::bitset<15> bits(std::stoi(symbol));
            String addressBits = "0" + bits.to_string();
            return addressBits;
        }
     
        void BinaryGenerator::addLine(const String& binary)
        {
            if (outputFile.is_open())
            {
                outputFile << binary;
                outputFile << std::endl;
            }
        }

        void BinaryGenerator::closeFile()
        {
            outputFile.close();
        }
}