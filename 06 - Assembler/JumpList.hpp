#include "Parsing.hpp"
#include <map>

//todo: could create BinaryList interface for this and CommandList classes
class parsing::JumpList
{
    public:
        /**
         * @brief Initializes all jump options
         * 
         */
        JumpList();

        /**
         * @brief Checks if the jump command exists
         * 
         * @param j 
         * @return boolean
         */
        bool isValidCode(const String& j);

        /**
         * @brief get the binary code of the jump command
         * 
         * @param j 
         * @return String 
         */
        String getCode(const String& j);

    private:
        std::map<String, String> jumps;
};