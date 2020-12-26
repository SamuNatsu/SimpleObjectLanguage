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

#ifndef SOL_SCANNER_H_INCLUDED
#define SOL_SCANNER_H_INCLUDED

#include <fstream>

#include "SOL_Token.h"

namespace SOL {

class Scanner {
    public:
        Scanner() = default;
        Scanner(const Scanner&) = delete;
        ~Scanner();

        Scanner& operator=(const Scanner&) = delete;

        bool Open(const std::string&);
        bool Open(std::string&&);
        void Close();

        void NextToken();
        const Token& GetCurrentToken() const;

    private:
        std::ifstream m_Fin;
        Token m_CurrentToken;
        int m_Line = 0, m_Row = 0;

        bool IsSkippedChar(char);
        void SkipChar();

        Token GetValue();
        Token GetKey(char);
};

Scanner::~Scanner() {
    Close();
}

bool Scanner::Open(const std::string& path) {
    Close();
    m_Fin.open(path);
    return m_Fin.is_open();
}
bool Scanner::Open(std::string&& path) {
    return Open(path);
}
void Scanner::Close() {
    if (m_Fin.is_open())
        m_Fin.close();
    m_Line = 1;
    m_Row = 0;
}

void Scanner::NextToken() {
    SkipChar();
    if (m_Fin.eof()) {
        m_CurrentToken.Set(Token_EOF, "EOF", m_Line, m_Row);
        return;
    }
    char _buff = m_Fin.get();
    ++m_Row;
    switch (_buff) {
        case ('{'):
            m_CurrentToken.Set(Token_LCBracket, "{", m_Line, m_Row);
            break;
        case ('['):
            m_CurrentToken.Set(Token_LSBracket, "[", m_Line, m_Row);
            break;
        case ('}'):
            m_CurrentToken.Set(Token_RCBracket, "}", m_Line, m_Row);
            break;
        case (']'):
            m_CurrentToken.Set(Token_RSBracket, "]", m_Line, m_Row);
            break;
        case ('='):
            m_CurrentToken.Set(Token_Equal, "=", m_Line, m_Row);
            break;
        case (','):
            m_CurrentToken.Set(Token_Comma, ",", m_Line, m_Row);
            break;
        case ('"'):
            m_CurrentToken = GetValue();
            break;
        default:
            m_CurrentToken = GetKey(_buff);
    }
}
const Token& Scanner::GetCurrentToken() const {
    return m_CurrentToken;
}

bool Scanner::IsSkippedChar(char tmp) {
    return tmp == ' ' || iscntrl(tmp);
}
void Scanner::SkipChar() {
    if (m_Fin.eof())
        return;
    char _buff = m_Fin.peek();
    while (IsSkippedChar(_buff) && !m_Fin.eof()) {
        if (m_Fin.get() == '\n') {
            ++m_Line;
            m_Row = 0;
        }
        else
            ++m_Row;
        _buff = m_Fin.peek();
    }
}

Token Scanner::GetValue() {
    if (m_Fin.eof())
        return Token(Token_EOF, "EOF", m_Line, m_Row);
    std::string _rtn;
    char _buff;
    int _line = m_Line, _row = m_Row;
    while (!m_Fin.eof()) {
        _buff = m_Fin.get();
        ++m_Row;
        switch (_buff) {
            case ('"'):
                return Token(Token_Value, _rtn, _line,  _row);
            case ('\\'): {
                if (!m_Fin.eof())
                    return Token(Token_Error, "Invalid value", m_Line, m_Row);
                char __buff = m_Fin.get();
                ++m_Row;
                switch (__buff) {
                    case ('n'):
                        _rtn += '\n';
                        break;
                    case ('r'):
                        _rtn += '\r';
                        break;
                    case ('t'):
                        _rtn += '\t';
                        break;
                    case ('\\'):
                        _rtn += '\\';
                        break;
                    case ('"'):
                        _rtn += '\"';
                        break;
                    default:
                        _rtn += '\\';
                        _rtn += __buff;
                        break;
                }
                break;
            }
            default:
                if (iscntrl(_buff))
                    return Token(Token_Error, "Invalid value", m_Line, m_Row);
                _rtn += _buff;
        }
    }
    return Token(Token_Error, "Invalid value", m_Line, m_Row);
}
Token Scanner::GetKey(char tmp) {
    if (m_Fin.eof())
        return Token(Token_EOF, "EOF", m_Line, m_Row);
    std::string _rtn(1, tmp);
    char _buff = m_Fin.peek();
    int _line = m_Line, _row = m_Row;
    while (!m_Fin.eof()) {
        ++m_Row;
        if (isdigit(_buff) || isalpha(_buff) || _buff == '_')
            _rtn += _buff;
        else
            return Token(Token_Key, _rtn, _line, _row);
        m_Fin.get();
        _buff = m_Fin.peek();
    }
    return Token(Token_Key, _rtn, _line, _row);
}

}

#endif