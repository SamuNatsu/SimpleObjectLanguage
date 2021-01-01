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
**/

#ifndef SOL_TOKEN_HPP_INCLUDED
#define SOL_TOKEN_HPP_INCLUDED

#include <string>

namespace SOL {

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

class Token {
    public:
        static const char* TokenTypeName[10];

        Token() = default;
        Token(TokenType, const std::string&, int, int);
        Token(TokenType, std::string&&, int, int);
        Token(const Token&);
        Token(Token&&);
        ~Token() = default;

        Token& operator=(const Token&);
        Token& operator=(Token&&);

        TokenType GetType() const;
        const std::string& GetValue() const;
        std::string GetPosition() const;
        void GetPosition(int&, int&) const;

        void Set(TokenType, const std::string&, int, int);
        void Set(TokenType, std::string&&, int, int);

    private:
        TokenType m_Type = Token_EOF;
        std::string m_Value = "EOF";
        int m_Line = 1, m_Column = 1;
};

}

#endif

#ifdef SOL_IMPLEMENTATION

namespace SOL {

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

Token::Token(TokenType type, const std::string& value, int line, int column) {
    Set(type, value, line, column);
}
Token::Token(TokenType type, std::string&& value, int line, int column) {
    Set(type, value, line, column);
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
    m_Column = tmp.m_Column;
    return *this;
}
Token& Token::operator=(Token&& tmp) {
    m_Type = tmp.m_Type;
    m_Value.swap(tmp.m_Value);
    m_Line = tmp.m_Line;
    m_Column = tmp.m_Column;
    return *this;
}

TokenType Token::GetType() const {
    return m_Type;
}
const std::string& Token::GetValue() const {
    return m_Value;
}
std::string Token::GetPosition() const {
    return std::string("Line: ") + std::to_string(m_Line) + " Column: " + std::to_string(m_Column);
}
void Token::GetPosition(int& line, int& column) const {
    line = m_Line;
    column = m_Column;
}

void Token::Set(TokenType type, const std::string& value, int line, int column) {
    m_Type = type;
    m_Value = value;
    m_Line = line;
    m_Column = column;
}
void Token::Set(TokenType type, std::string&& value, int line, int column) {
    Set(type, value, line, column);
}

}

#endif