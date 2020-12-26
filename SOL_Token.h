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
 * @file    SOL_Token.h
 * @author  SamuNatsu
 * @version 0.1.2
 * @brief   Token class
**/

#ifndef SOL_TOKEN_H_INCLUDED
#define SOL_TOKEN_H_INCLUDED

#include <string>

/**
 * @namespace
 * @brief   Simple object language namespace
**/
namespace SOL {

/**
 * @enum
 * @brief   Token types
**/
using TokenType = enum _tt {
    Token_EOF = 0,
    Token_Error,
    Token_LCBracket,
    Token_LSBracket,
    Token_RCBracket,
    Token_RSBracket,
    Token_Equal,
    Token_Key,
    Token_Value,
    Token_Comma
};

/**
 * @class
 * @brief   Token
**/
class Token {
    public:
        /**
         * @brief   Token names (C-style string)
        **/
        static const char* TokenTypeName[10];

        /**
         * @brief   Default constructor
        **/
        Token() = default;
        /**
         * @brief   Construct token from parameters
         * @param   type    Type enum
         * @param   value   Value string
         * @param   line    Line of the token
         * @param   row     Row of the token
        **/
        Token(TokenType type, const std::string& value, int line, int row);
        /**
         * @brief   Construct token from parameters
         * @param   type    Type enum
         * @param   value   Value string
         * @param   line    Line of the token
         * @param   row     Row of the token
        **/
        Token(TokenType type, std::string&& value, int line, int row);
        /**
         * @brief   Copy constructor
         * @param   tmp Source token
        **/
        Token(const Token& tmp);
        /**
         * @brief   Move constructor
         * @param   tmp Source token
        **/
        Token(Token&&);
        /**
         * @brief   Default destructor
        **/
        ~Token() = default;

        /**
         * @brief   Assign the value to this token
         * @param   tmp Source token
        **/
        Token& operator=(const Token& tmp);
        /**
         * @brief   Move the value to this token
         * @param   tmp Source token
        **/
        Token& operator=(Token&& tmp);

        /**
         * @brief   Get token type
        **/
        TokenType GetType() const;
        /**
         * @brief   Get token value
        **/
        const std::string& GetValue() const;
        /**
         * @brief   Get token position in source file
        **/
        std::string GetPosition() const;
        /**
         * @brief   Get token position in source file
         * @param   line    Line of the token
         * @param   row     Row of the token
        **/
        void GetPosition(int& line, int& row) const;

        /**
         * @brief   Set token attributes
         * @param   type    Type enum
         * @param   value   Value string
         * @param   line    Line of the token
         * @param   row     Row of the token
        **/
        void Set(TokenType type, const std::string& value, int line, int row);
        /**
         * @brief   Set token attributes
         * @param   type    Type enum
         * @param   value   Value string
         * @param   line    Line of the token
         * @param   row     Row of the token
        **/
        void Set(TokenType type, std::string&& value, int line, int row);

    private:
        TokenType m_Type = Token_EOF;
        std::string m_Value = "EOF";
        int m_Line = 1, m_Row = 1;
};

const char* Token::TokenTypeName[10] = {
    "Token_EOF",
    "Token_Error",
    "Token_LCBracket",
    "Token_LSBracket",
    "Token_RCBracket",
    "Token_RSBracket",
    "Token_Equal",
    "Token_Key",
    "Token_Value",
    "Token_Comma"
};

Token::Token(TokenType type, const std::string& value, int line, int row) {
    Set(type, value, line, row);
}
Token::Token(TokenType type, std::string&& value, int line, int row) {
    Set(type, value, line, row);
}
Token::Token(const Token& tmp) {
    *this = tmp;
}
Token::Token(Token&& tmp) {
    *this = std::forward<Token>(tmp);
}

Token& Token::operator=(const Token& tmp) {
    m_Type = tmp.m_Type;
    m_Value = tmp.m_Value;
    m_Line = tmp.m_Line;
    m_Row = tmp.m_Row;
    return *this;
}
Token& Token::operator=(Token&& tmp) {
    m_Type = tmp.m_Type;
    m_Value.swap(tmp.m_Value);
    m_Line = tmp.m_Line;
    m_Row = tmp.m_Row;
    return *this;
}

TokenType Token::GetType() const {
    return m_Type;
}
const std::string& Token::GetValue() const {
    return m_Value;
}
std::string Token::GetPosition() const {
    return std::string("Line: ") + std::to_string(m_Line) + " Row: " + std::to_string(m_Row);
}
void Token::GetPosition(int& line, int& row) const {
    line = m_Line;
    row = m_Row;
}

void Token::Set(TokenType type, const std::string& value, int line, int row) {
    m_Type = type;
    m_Value = value;
    m_Line = line;
    m_Row = row;
}
void Token::Set(TokenType type, std::string&& value, int line, int row) {
    Set(type, value, line, row);
}

}

#endif
