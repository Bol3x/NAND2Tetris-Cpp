#include "Parsing.h"
#include <unordered_map>

class parsing::ParseMap
{
    public:
        /**
         * @brief copies a map of the symbols into the class
         * 
         */
        ParseMap(const std::unordered_map<String, String>& map);

        /**
         * @brief Checks if the symbol exists
         * 
         * @param symbol
         * @return boolean
         */
        bool isValidCode(const String& symbol);

        /**
         * @brief get the binary code of the symbol
         * 
         * @param symbol
         * @return String 
         */
        String getCode(const String& symbol);

    private:
        std::unordered_map<String, String> symbols;
};