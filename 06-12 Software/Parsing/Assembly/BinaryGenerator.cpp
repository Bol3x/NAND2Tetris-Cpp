#include "BinaryGenerator.h"

namespace parsing::HackAssembly
{
        BinaryGenerator::BinaryGenerator(const String& filename) : 
        compMap(std::unordered_map<String, String> {
            {"0", "101010"},
            {"1", "111111"},
            {"-1", "111010"},
            {"D", "001100"},
            {"A", "110000"}, {"M","110000"},
            {"-D", "001111"},
            {"-A", "110011"}, {"-M", "110011"},
            {"!D", "001101"},
            {"!A", "110001"}, {"!M","110001"},
            {"D+1", "011111"},
            {"A+1", "110111"}, {"M+1", "110111"},
            {"D-1", "001110"},
            {"A-1", "110010"}, {"M-1", "110010"},
            {"D+A", "000010"}, {"D+M", "000010"},
            {"D-A", "010011"}, {"D-M", "010011"},
            {"A-D", "000111"}, {"M-D", "000111"},
            {"D&A", "000000"}, {"D&M", "000000"},
            {"D|A", "010101"}, {"D|M", "010101"}}),
            
            jumpMap(std::unordered_map<String, String>{
                {"JGT", "001"},
                {"JEQ", "010"},
                {"JGE", "011"},
                {"JLT", "100"},
                {"JNE", "101"},
                {"JLE", "110"},
                {"JMP", "111"}
            })
        {
            openFile(filename);
        }

        String BinaryGenerator::generateCompBinary(const String& comp, const String& dest, const String& jump)
        {
            String iBits = "111";
            String aBit = comp.find('M') != comp.npos ? "1" : "0";
            String cBits, dBits = "", jBits = "";

            if (compMap.isValidCode(comp))
            {
                cBits = compMap.getCode(comp);
            }
            else
            {
                //todo error check
                cBits = "000000";
            }

            dBits.append(dest.find('A') != dest.npos ? "1" : "0");
            dBits.append(dest.find('D') != dest.npos ? "1" : "0");
            dBits.append(dest.find('M') != dest.npos ? "1" : "0");
            

            if (jumpMap.isValidCode(jump))
            {
                jBits = jumpMap.getCode(jump);
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
}