#include "JackCompiler.hpp"
#include "Parser.cpp"
#include <sstream>

namespace parsing::JackCompiler
{
    class Tokenizer : public Parser
    {
        /**
         * @brief Creates a new compiler object to parse the Jack langauge
         * 
         * @param filename - file output of the tokenizer
         */
        Tokenizer(const String& filename);

        /**
         * @brief checks if there are remaining tokens in the stream
         * 
         */
        bool hasMoreTokens();

        /**
         * @brief advances the stream to the next token
         * 
         */
        void advanceToken();

        /**
         * @brief Gets the type of the current token
         * 
         */
        JackTokenType getTokenType();

        /**
         * @brief Gets the keyword type if the current token is a keyword type
         * 
         * @return JackKeyword 
         */
        JackKeyword getKeyword();

        /**
         * @brief Gets the symbol if the current token is a symbol type
         * 
         */
        char getSymbol();

        /**
         * @brief Gets the identifier string if the current token is an identifier type
         * 
         */
        String getIdentifier();

        /**
         * @brief Gets the integer equivalent of an integer token
         * 
         */
        int getIntValue();

        /**
         * @brief Gets the literal string of a string token
         * 
         */
        String getStringValue();
    };
}