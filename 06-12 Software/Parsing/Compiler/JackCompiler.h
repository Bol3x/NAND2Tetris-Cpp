#pragma once

#include "../Parsing.h"
#include "../FileGenerator.h"
#include <unordered_map>
#include <unordered_set>

namespace parsing::JackCompiler
{

    const int MAX_INT_VALUE = 0x7FFF; //32767, last bit for negative

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

    const std::unordered_set<char> operators = {
        '+', '-', '*', '/', '&', '|', '<', '>', '=', '~'
    };
    
    const std::unordered_map<char, int> operatorPrec = {
        {'*', 5}, {'/', 5},
        {'+', 4}, {'-', 4},
        {'<', 3}, {'>', 3},
        {'=', 2},
        {'&', 1},
        {'|', 0}
    };

    enum class JackCompilerType
    {
        JC_STATIC,
        JC_FIELD,
        JC_ARG,
        JC_VAR,
        JC_NONE
    };

    enum class JackVMSegment
    {
        JVT_CONSTANT,
        JVT_ARGUMENT,
        JVT_LOCAL,
        JVT_STATIC,
        JVT_THIS,
        JVT_THAT,
        JVT_POINTER,
        JVT_TEMP
    };

    enum class JackVMCommand
    {
        JVC_ADD,
        JVC_SUB,
        JVC_NEG,
        JVC_EQ,
        JVC_GT,
        JVC_LT,
        JVC_AND,
        JVC_OR,
        JVC_NOT
    };

    typedef struct symbol_data_t {
        int index;
        String datatype;
        JackCompilerType kind;
    } symbolData;

    class Tokenizer;
    class CompilerEngine;
    class CompilerSymbolTable;
    class VMWriter;
}