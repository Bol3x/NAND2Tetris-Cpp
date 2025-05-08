#pragma once

#include "ParseMap.h"

namespace parsing{
    ParseMap::ParseMap(const std::unordered_map<String, String>& map)
    {
        symbols = map;
    }
    
    bool ParseMap::isValidCode(const String& symbol)
    {
        return symbols.find(symbol) != symbols.end();
    }

    String ParseMap::getCode(const String& j)
    {
        return symbols.find(j)->second;
    }
    
}