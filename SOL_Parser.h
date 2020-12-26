/**
 * MIT License
 * 
 * Copyright (c) 2020 SamuNatsu
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * @file    SOL_Parser.h
 * @author  SamuNatsu
 * @version 0.1.2
 * @brief   Parser class
**/

#ifndef SML_PARSER_H_INCLUDED
#define SML_PARSER_H_INCLUDED

#include "SOL_Value.h"
#include "SOL_Scanner.h"

/**
 * @namespace
 * @brief   Simple object language namespace
**/
namespace SOL {

/**
 * @class
 * @brief   Parser 
**/
class Parser {
    public:
        /**
         * @brief   Default constructor
        **/
        Parser() = default;
        Parser(const Parser&) = delete;
        /**
         * @brief   Default destructor
        **/
        ~Parser() = default;

        Parser& operator=(const Parser&) = delete;

        /**
         * @brief   Get error 
        **/
        const std::string& GetError() const;

        /**
         * @brief   Parser file
         * @param   path    File path
        **/
        bool Parse(const std::string& path);
        /**
         * @brief   Parser file
         * @param   path    File path
        **/
        bool Parse(std::string&&);

        /**
         * @brief   Get root object
        **/
        Object GetRoot() const;

    private:
        std::string m_Error;
        Scanner m_Scanner;
        Object m_Root;
        bool m_Exception;

        Value GetValue();
        std::pair<std::string, Value> GetPair();
        Object GetObject();
        Array GetArray();
};

const std::string& Parser::GetError() const {
    return m_Error;
}

bool Parser::Parse(const std::string& path) {
    m_Exception = false;
    if (!m_Scanner.Open(path)) {
        m_Error = "Cannot open file";
        return false;
    }
    m_Scanner.NextToken();
    if (m_Scanner.GetCurrentToken().GetType() != Token_LCBracket) {
        m_Error = std::string("Invalid object -> ") + m_Scanner.GetCurrentToken().GetPosition();
        return false;
    }
    m_Root = GetObject();
    return !m_Exception;
}
bool Parser::Parse(std::string&& path) {
    return Parse(path);
}

Object Parser::GetRoot() const {
    return m_Root;
}

Value Parser::GetValue() {
    Value _value;
    switch (m_Scanner.GetCurrentToken().GetType()) {
        case (Token_Value):
            return m_Scanner.GetCurrentToken().GetValue();
        case (Token_LCBracket):
            _value = GetObject();
            return m_Exception ? Value() : _value;
        case (Token_LSBracket):
            _value = GetArray();
            return m_Exception ? Value() : _value;
        default:
            if (m_Scanner.GetCurrentToken().GetType() == Token_Error)
                m_Error = m_Scanner.GetCurrentToken().GetValue();
            else
                m_Error = std::string("Invalid value -> ") + m_Scanner.GetCurrentToken().GetPosition();
            m_Exception = true;
            return Value();
    }
}
std::pair<std::string, Value> Parser::GetPair() {
    if (m_Scanner.GetCurrentToken().GetType() != Token_Key) {
        if (m_Scanner.GetCurrentToken().GetType() == Token_Error)
            m_Error = m_Scanner.GetCurrentToken().GetValue();
        else
            m_Error = std::string("Invalid key -> ") + m_Scanner.GetCurrentToken().GetPosition();
        m_Exception = true;
        return std::pair<std::string, Value>();
    }
    std::string _key = m_Scanner.GetCurrentToken().GetValue();
    m_Scanner.NextToken();
    if (m_Scanner.GetCurrentToken().GetType() != Token_Equal) {
        if (m_Scanner.GetCurrentToken().GetType() == Token_Error)
            m_Error = m_Scanner.GetCurrentToken().GetValue();
        else
            m_Error = std::string("KVP syntax error -> ") + m_Scanner.GetCurrentToken().GetPosition();
        m_Exception = true;
        return std::pair<std::string, Value>();
    }
    m_Scanner.NextToken();
    Value _value = GetValue();
    if (m_Exception)
        return std::pair<std::string, Value>();
    return std::pair<std::string, Value>(_key, _value);
}
Object Parser::GetObject() {
    Object _rtn;
    m_Scanner.NextToken();
    while (1) {
        if (m_Scanner.GetCurrentToken().GetType() == Token_RCBracket)
            return _rtn;
        std::pair<std::string, Value> _pair = GetPair();
        if (m_Exception)
            return Object();
        m_Scanner.NextToken();
        if (m_Scanner.GetCurrentToken().GetType() == Token_Comma)
            m_Scanner.NextToken();
        else if (m_Scanner.GetCurrentToken().GetType() != Token_RCBracket) {
            if (m_Scanner.GetCurrentToken().GetType() == Token_Error)
                m_Error = m_Scanner.GetCurrentToken().GetValue();
            else
                m_Error = std::string("Object syntax error -> ") + m_Scanner.GetCurrentToken().GetPosition();
            m_Exception = true;
            return Object();
        }
        _rtn.insert(_pair);
    }
    m_Error = std::string("Invalid object -> ") + m_Scanner.GetCurrentToken().GetPosition();
    m_Exception = true;
    return Object();
}
Array Parser::GetArray() {
    Array _rtn;
    m_Scanner.NextToken();
    while (1) {
        if (m_Scanner.GetCurrentToken().GetType() == Token_RSBracket)
            return _rtn;
        Value _value = GetValue();
        if (m_Exception)
            return Array();
        m_Scanner.NextToken();
        if (m_Scanner.GetCurrentToken().GetType() == Token_Comma)
            m_Scanner.NextToken();
        else if (m_Scanner.GetCurrentToken().GetType() != Token_RSBracket) {
            if (m_Scanner.GetCurrentToken().GetType() == Token_Error)
                m_Error = m_Scanner.GetCurrentToken().GetValue();
            else
                m_Error = std::string("Array syntax error -> ") + m_Scanner.GetCurrentToken().GetPosition();
            m_Exception = true;
            return Array();
        }
        _rtn.emplace_back(_value);
    }
    m_Error = std::string("Invalid array -> ") + m_Scanner.GetCurrentToken().GetPosition();
    m_Exception = true;
    return Array();
}

}

#endif
