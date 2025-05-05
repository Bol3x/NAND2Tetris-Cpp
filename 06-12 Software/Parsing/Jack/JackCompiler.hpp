#pragma once

#include "../Parsing.hpp"

namespace parsing::JackCompiler
{
    enum class JackTokenType
    {
        JT_KEYWORD,
        JT_IDENTIFIER,
        JT_INT_CONST,
        JT_STR_CONST
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

    class Tokenizer;
    class Compiler;
}