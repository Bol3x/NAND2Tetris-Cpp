#pragma once

#include "../Parsing.hpp"
#include <unordered_map>
#include <unordered_set>

namespace parsing::JackCompiler
{
    enum class JackTokenType
    {
        JT_KEYWORD,
        JT_IDENTIFIER,
        JT_INT_CONST,
        JT_STR_CONST,
        JT_SYMBOL,
        JT_INVALID
    };

    enum class JackKeyword
    {
        JK_CLASS, JK_METHOD, JK_FUNCTION, JK_CONSTRUCTOR,
        JK_INT, JK_BOOLEAN, JK_CHAR, JK_VOID,
        JK_VAR, JK_STATIC, JK_FIELD,
        JK_LET, JK_DO, 
        JK_IF, JK_ELSE,
        JK_WHILE,
        JK_RETURN,
        JK_TRUE, JK_FALSE, JK_NULL,
        JK_THIS
    };

    const std::unordered_map<String, JackKeyword> keywordMap = 
    {
        {"class", JackKeyword::JK_CLASS},
        {"method", JackKeyword::JK_METHOD},
        {"function", JackKeyword::JK_FUNCTION},
        {"constructor", JackKeyword::JK_CONSTRUCTOR},
        {"int", JackKeyword::JK_INT},
        {"boolean", JackKeyword::JK_BOOLEAN},
        {"char", JackKeyword::JK_CHAR},
        {"void", JackKeyword::JK_VOID},
        {"var", JackKeyword::JK_VAR},
        {"static", JackKeyword::JK_STATIC},
        {"field", JackKeyword::JK_FIELD},
        {"let", JackKeyword::JK_LET},
        {"do", JackKeyword::JK_DO},
        {"if", JackKeyword::JK_IF},
        {"else", JackKeyword::JK_ELSE},
        {"while", JackKeyword::JK_WHILE},
        {"return", JackKeyword::JK_RETURN},
        {"true", JackKeyword::JK_TRUE},
        {"false", JackKeyword::JK_FALSE},
        {"null", JackKeyword::JK_NULL},
        {"this", JackKeyword::JK_THIS}
    };

    const std::unordered_set<char> symbols = {
        '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~'
    };


    class Tokenizer;
    class Compiler;
}