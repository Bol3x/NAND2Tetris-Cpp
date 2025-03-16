#include "Parsing.hpp"
#include <map>

class parsing::CommandList
{
    public:
        CommandList();

        bool isValidCode(const String& c);

        String getCode(const String& c);
    private:
        std::map<String, String> comp;
};