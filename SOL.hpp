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

/**
 * https://github.com/SamuNatsu/SimpleObjectLanguage
**/

#ifndef SOL_HPP
#define SOL_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#define SOL_VERSION "2.0.0"
#define SOL_VERSION_MAJOR 2
#define SOL_VERSION_MINOR 0
#define SOL_VERSION_PATCH 0

namespace SOL {

class Value;
using Object = std::unordered_map<std::string, Value>;
using Array = std::vector<Value>;

enum ValueType {
    VALUE_NULL,
    VALUE_OBJECT,
    VALUE_ARRAY,
    VALUE_STRING
};

class Value {
    public:
        Value() = default;
        Value(const Value& tmp) {
            *this = tmp;
        }
        Value(Value&& tmp) {
            *this = std::forward<Value>(tmp);
        }
        Value(const Object& tmp) {
            *this = tmp;
        }
        Value(Object&& tmp) {
            *this = std::forward<Object>(tmp);
        }
        Value(const Array& tmp) {
            *this = tmp;
        }
        Value(Array&& tmp) {
            *this = std::forward<Array>(tmp);
        }
        Value(const std::string& tmp) {
            *this = tmp;
        }
        Value(std::string&& tmp) {
            *this = std::forward<std::string>(tmp);
        }
        ~Value() {
            Clear();
        }

        Value& operator=(const Value& tmp) {
            Clear();
            m_Type = tmp.m_Type;
            switch (m_Type) {
                case (VALUE_NULL):
                    break;
                case (VALUE_OBJECT):
                    m_Data = new Object(tmp);
                    break;
                case (VALUE_ARRAY):
                    m_Data = new Array(tmp);
                    break;
                case (VALUE_STRING):
                    m_Data = new std::string(tmp);
                    break;
            }
            return *this;
        }
        Value& operator=(Value&& tmp) {
            Clear();
            m_Type = tmp.m_Type;
            m_Data = tmp.m_Data;
            tmp.m_Type = VALUE_NULL;
            tmp.m_Data = nullptr;
            return *this;
        }
        Value& operator=(const Object& tmp) {
            Clear();
            m_Type = VALUE_OBJECT;
            m_Data = new Object(tmp);
            return *this;
        }
        Value& operator=(Object&& tmp) {
            Clear();
            m_Type = VALUE_OBJECT;
            m_Data = new Object(std::forward<Object>(tmp));
            return *this;
        }
        Value& operator=(const Array& tmp) {
            Clear();
            m_Type = VALUE_ARRAY;
            m_Data = new Array(tmp);
            return *this;
        }
        Value& operator=(Array&& tmp) {
            Clear();
            m_Type = VALUE_ARRAY;
            m_Data = new Array(std::forward<Array>(tmp));
            return *this;
        }
        Value& operator=(const std::string& tmp) {
            Clear();
            m_Type = VALUE_STRING;
            m_Data = new std::string(tmp);
            return *this;
        }
        Value& operator=(std::string&& tmp) {
            Clear();
            m_Type = VALUE_STRING;
            m_Data = new std::string(std::forward<std::string>(tmp));
            return *this;
        }

        Value& operator[](const std::string& tmp) {
            return m_Type == VALUE_OBJECT ? (*(Object*)m_Data)[tmp] : (*this = Object(), (*(Object*)m_Data)[tmp]);
        }
        Value& operator[](std::string&& tmp) {
            return (*this)[tmp];
        }
        Value& operator[](size_t tmp) {
            if (m_Type != VALUE_ARRAY)
                *this = Array();
            while (tmp >= ((Array*)m_Data)->size())
                ((Array*)m_Data)->emplace_back();
            return (*(Array*)m_Data)[tmp];
        }

        operator Object() const {
            return m_Type == VALUE_OBJECT ? *(Object*)m_Data : Object();
        }
        operator Array() const {
            return m_Type == VALUE_ARRAY ? *(Array*)m_Data : Array();
        }
        operator std::string() const {
            return m_Type == VALUE_STRING ? *(std::string*)m_Data : std::string();
        }

        const Object& AsObject() const {
            static Object s_Empty;
            return m_Type == VALUE_OBJECT ? *(Object*)m_Data : s_Empty;
        }
        const Array& AsArray() const {
            static Array s_Empty;
            return m_Type == VALUE_ARRAY ? *(Array*)m_Data : s_Empty;
        }
        const std::string& AsString() const {
            static std::string s_Empty;
            return m_Type == VALUE_STRING ? *(std::string*)m_Data : s_Empty;
        }

        void Clear() {
            switch (m_Type) {
                case (VALUE_NULL):
                    break;
                case (VALUE_OBJECT):
                    delete (Object*)m_Data;
                    m_Type = VALUE_NULL;
                    m_Data = nullptr;
                    break;
                case (VALUE_ARRAY):
                    delete (Array*)m_Data;
                    m_Type = VALUE_NULL;
                    m_Data = nullptr;
                    break;
                case (VALUE_STRING):
                    delete (std::string*)m_Data;
                    m_Type = VALUE_NULL;
                    m_Data = nullptr;
                    break;
            }
        }
        ValueType GetType() const {
            return m_Type;
        }

    private:
        ValueType m_Type = VALUE_NULL;
        void *m_Data = nullptr;
};

class Parser {
    public:
        Parser() = default;
        Parser(const Parser&) = delete;
        ~Parser() = default;

        Parser& operator=(const Parser&) = delete;

        const std::string& GetError() const {
            return m_Error;
        }

        bool ParseFromFile(const std::string& path) {
            m_Error = "";
            if (!Open(path)) {
                m_Error = "Cannot open file";
                return false;
            }
            NextToken();
            if (m_Type != TOKEN_LCBRACKET) {
                m_Error = std::string("Invalid object -> ") + GetPosition();
                return false;
            }
            m_Exception = false;
            m_Result = GetObject();
            return !m_Exception;
        }
        bool ParseFromFile(std::string&& path) {
            return ParseFromFile(path);
        }
        bool ParseFromMemory(const std::string& mem) {
            m_Error = "";
            Open(mem, false);
            NextToken();
            if (m_Type != TOKEN_LCBRACKET) {
                m_Error = std::string("Invalid object -> ") + GetPosition();
                return false;
            }
            m_Exception = false;
            m_Result = GetObject();
            return !m_Exception;
        }
        bool ParseFromMemory(std::string&& mem) {
            return ParseFromMemory(mem);
        }
        Object GetResult() const {
            return m_Result;
        }

        bool ParseToFile(const std::string& path, const Object& obj, bool flag = false) {
            std::ofstream fout;
            fout.open(path);
            if (!fout.is_open())
                return false;
            fout << ParseToString(obj, flag);
            fout.flush();
            fout.close();
            return true;
        }
        bool ParseToFile(std::string&& path, const Object& obj, bool flag = false) {
            return ParseToFile(path, obj, flag);
        }
        std::string ParseToString(const Object& obj, bool flag = false) {
            return GenObject(obj, flag ? ~0ull : 0);
        }

    private:
        bool m_Flag;
        std::ifstream m_Fin;
        std::istringstream m_Sin;

        enum TokenType {
            TOKEN_EOF,
            TOKEN_ERROR,
            TOKEN_LCBRACKET,
            TOKEN_LSBRACKET,
            TOKEN_RCBRACKET,
            TOKEN_RSBRACKET,
            TOKEN_EQUAL,
            TOKEN_COMMA,
            TOKEN_KEY,
            TOKEN_VALUE
        } m_Type;
        std::string m_Value;
        int m_Line, m_Column;

        bool Open(const std::string& tmp, bool type = true) {
            m_Flag = type;
            m_Type = TOKEN_EOF;
            m_Value = "EOF";
            m_Line = m_Column = 1;
            if (type) {
                if (m_Fin.is_open())
                    m_Fin.close();
                m_Fin.open(tmp);
                return m_Fin.is_open();
            }
            else {
                m_Sin.str(tmp);
                return true;
            }
        }
        bool IsEOF() const {
            return m_Flag ? m_Fin.eof() : m_Sin.eof();
        }
        char PeekChar() {
            return m_Flag ? m_Fin.peek() : m_Sin.peek();
        }
        char GetChar() {
            return m_Flag ? m_Fin.get() : m_Sin.get();
        }
        void SkipChar() {
            char c = PeekChar();
            while (!isgraph(c) && c >= 0) {
                if (c == '\n') {
                    ++m_Line;
                    m_Column = -1;
                }
                ++m_Column;
                GetChar();
                c = PeekChar();
            }
        }
        std::string GetUnescape() {
            char c;
            std::string t;
            unsigned int b = 0;
            for (int i = 0; i < 4; ++i) {
                ++m_Column;
                GetChar();
                c = PeekChar();
                if (!isxdigit(c))
                    return t.insert(0, "u");
                t += c;
                b = (b << 4) | (isdigit(c) ? c - '0' : toupper(c) - 'A' + 10);
            }
            if (b <= 0x7F)
                return std::string(1, b);
            else if (b <= 0x7FF) {
                t = "";
                t[0] = 0xC0 | (b >> 6);
                t[1] = 0x80 | (b & 0x3F);
                return t;
            }
            else {
                t = "";
                t[0] = 0xE0 | (b >> 12);
                t[1] = 0x80 | ((b >> 6) & 0x3F);
                t[2] = 0x80 | (b & 0x3F);
                return t;
            }
        }
        void GetValue() {
            char c = PeekChar();
            std::string buf;
            while (c != EOF) {
                switch (c) {
                    case ('"'):
                        m_Type = TOKEN_VALUE;
                        m_Value = buf;
                        ++m_Column;
                        GetChar();
                        return;
                    case ('\\'): {
                        ++m_Column;
                        GetChar();
                        c = PeekChar();
                        if (c == EOF) {
                            m_Type = TOKEN_ERROR;
                            m_Value = std::string("Invalid value -> ") + GetPosition();
                            return;
                        }
                        switch (c) {
                            case ('0'):
                                buf += '\0';
                                break;
                            case ('t'):
                                buf += '\t';
                                break;
                            case ('n'):
                                buf += '\n';
                                break;
                            case ('r'):
                                buf += '\r';
                                break;
                            case ('"'):
                                buf += '"';
                                break;
                            case ('\\'):
                                buf += '\\';
                                break;
                            case ('u'):
                                buf += GetUnescape();
                                break;
                            default:
                                if (iscntrl(c)) {
                                    m_Type = TOKEN_ERROR;
                                    m_Value = std::string("Invalid value -> ") + GetPosition();
                                    return;
                                }
                                buf += c;
                                break;
                        }
                        break;
                    }
                    default:
                        if (iscntrl(c)) {
                            m_Type = TOKEN_ERROR;
                            m_Value = std::string("Invalid value -> ") + GetPosition();
                            return;
                        }
                        buf += c;
                        break;
                }
                ++m_Column;
                GetChar();
                c = PeekChar();
            }
            m_Type = TOKEN_ERROR;
            m_Value = std::string("Invalid value -> ") + GetPosition();
        }
        void GetKey(char tmp) {
            if (!isalpha(tmp) && tmp != '_') {
                m_Type = TOKEN_ERROR;
                m_Value = std::string("Invalid key -> ") + GetPosition();
                return;
            }
            std::string buf(1, tmp);
            char c = PeekChar();
            while (isalpha(c) || isdigit(c) || c == '_') {
                buf += c;
                ++m_Column;
                GetChar();
                c = PeekChar();
            }
            m_Type = TOKEN_KEY;
            m_Value = buf;
        }
        void NextToken() {
            SkipChar();
            if (IsEOF()) {
                m_Type = TOKEN_EOF;
                m_Value = "EOF";
                return;
            }
            char c = GetChar();
            ++m_Column;
            switch (c) {
                case ('{'):
                    m_Type = TOKEN_LCBRACKET;
                    m_Value = "{";
                    break;
                case ('['):
                    m_Type = TOKEN_LSBRACKET;
                    m_Value = "[";
                    break;
                case ('}'):
                    m_Type = TOKEN_RCBRACKET;
                    m_Value = "}";
                    break;
                case (']'):
                    m_Type = TOKEN_RSBRACKET;
                    m_Value = "]";
                    break;
                case ('='):
                    m_Type = TOKEN_EQUAL;
                    m_Value = "=";
                    break;
                case (','):
                    m_Type = TOKEN_COMMA;
                    m_Value = ",";
                    break;
                case ('"'):
                    GetValue();
                    break;
                default:
                    GetKey(c);
                    break;
            }
        }

    Object m_Result;
    bool m_Exception;
    std::string m_Error;

    std::string GetPosition() const {
        return std::string("Line: ") + std::to_string(m_Line) + std::string(" Column: ") + std::to_string(m_Column);
    }
    Object GetObject() {
        Object obj;
        NextToken();
        while (true) {
            if (m_Type == TOKEN_RCBRACKET)
                return obj;
            std::pair<std::string, Value> pr = GetPair();
            if (m_Exception)
                return Object();
            NextToken();
            if (m_Type == TOKEN_COMMA) {
                NextToken();
                if (m_Type != TOKEN_KEY) {
                    m_Error = std::string("Invalid key -> ") + GetPosition();
                    m_Exception = true;
                    return Object();
                }
            }
            else if (m_Type != TOKEN_RCBRACKET) {
                if (m_Type == TOKEN_ERROR)
                    m_Error = m_Value;
                else 
                    m_Error = std::string("Object syntax error -> ") + GetPosition();
                m_Exception = true;
                return Object();
            }
            obj[pr.first] = pr.second;
        }
    }
    Array GetArray() {
        Array arr;
        NextToken();
        while (true) {
            if (m_Type == TOKEN_RSBRACKET)
                return arr;
            Value vl = GetValueEx();
            if (m_Exception)
                return Array();
            NextToken();
            if (m_Type == TOKEN_COMMA) {
                NextToken();
                if (m_Type != TOKEN_LCBRACKET && m_Type != TOKEN_LSBRACKET && m_Type != TOKEN_VALUE) {
                    m_Error = std::string("Invalid value -> ") + GetPosition();
                    m_Exception = true;
                    return Array();
                }
            }
            else if (m_Type != TOKEN_RSBRACKET) {
                if (m_Type == TOKEN_ERROR)
                    m_Error = m_Value;
                else 
                    m_Error = std::string("Array syntax error -> ") + GetPosition();
                m_Exception = true;
                return Array();
            }
            arr.emplace_back(vl);
        }
    }
    Value GetValueEx() {
        Value vl;
        switch (m_Type) {
            case (TOKEN_LCBRACKET):
                vl = GetObject();
                break;
            case (TOKEN_LSBRACKET):
                vl = GetArray();
                break;
            case (TOKEN_VALUE):
                return m_Value;
            default:
                if (m_Type == TOKEN_ERROR)
                    m_Error = m_Value;
                else 
                    m_Error = std::string("Invalid value -> ") + GetPosition();
                m_Exception = true;
                break;
        }
        return m_Exception ? Value() : vl;
    }
    std::pair<std::string, Value> GetPair() {
        if (m_Type != TOKEN_KEY) {
            if (m_Type == TOKEN_ERROR)
                m_Error = m_Value;
            else 
                m_Error = std::string("Invalid key -> ") + GetPosition();
            m_Exception = true;
            return std::pair<std::string, Value>();
        }
        std::string key = m_Value;
        NextToken();
        if (m_Type != TOKEN_EQUAL) {
            if (m_Type == TOKEN_ERROR)
                m_Error = m_Value;
            else 
                m_Error = std::string("KVP syntax error -> ") + GetPosition();
            m_Exception = true;
            return std::pair<std::string, Value>();
        }
        NextToken();
        Value vl = GetValueEx();
        return m_Exception ? std::pair<std::string, Value>() : std::make_pair(key, vl);
    }

    std::string GenObject(const Object& obj, size_t off) {
        std::string tmp(1, '{');
        size_t cnt = 0;
        if (off == ~0ull)
            for (auto& i : obj) {
                if (cnt++)
                    tmp += ',';
                tmp += GenPair(i, off);
            }
        else {
            tmp += '\n';
            for (auto& i : obj) {
                if (cnt++)
                    tmp += ",\n";
                for (size_t i = 0; i < off + 1; ++i)
                    tmp += '\t';
                tmp += GenPair(i, off);
            }
            tmp += '\n';
            for (size_t i = 0; i < off; ++i)
                tmp += '\t';
        }
        return tmp + '}';
    }
    std::string GenArray(const Array& arr, size_t off) {
        std::string tmp(1, '[');
        size_t cnt = 0;
        if (off == ~0ull)
            for (auto& i : arr) {
                if (cnt++)
                    tmp += ',';
                tmp += GenValue(i, off);
            }
        else {
            tmp += '\n';
            for (auto& i : arr) {
                if (cnt++)
                    tmp += ",\n";
                for (size_t i = 0; i < off + 1; ++i)
                    tmp += '\t';
                tmp += GenValue(i, off);
            }
            tmp += '\n';
            for (size_t i = 0; i < off; ++i)
                tmp += '\t';
        }
        return tmp + ']';
    }
    std::string GetEscape(const std::string& str, size_t& pos) {
        std::string tmp;
        if (str[pos] < 0x7F) {
            tmp = "\\u00";
            tmp += (str[pos] >> 4) >= 10 ? (str[pos] >> 4) + 'a' : (str[pos] >> 4) + '0';
            tmp += (str[pos] & 0xF) >= 10 ? (str[pos] & 0xF) + 'a' : (str[pos] & 0xF) + '0';
        }
        else if (str[pos] & 0xC0) {
            tmp = "\\u0";
            if (pos + 1 >= str.length())
                return "?";
            unsigned int p = ((str[pos] & 0x1F) << 6) | (str[pos + 1] & 0x3F);
            tmp += (p >> 8) >= 10 ? (p >> 8) + 'a' : (p >> 8) + '0';
            tmp += ((p >> 4) & 0xF) >= 10 ? ((p >> 4) & 0xF) + 'a' : ((p >> 4) & 0xF) + '0';
            tmp += (p & 0xF) >= 10 ? (p & 0xF) + 'a' : (p & 0xF) + '0';
            ++pos;
        }
        else {
            tmp = "\\u";
            if (pos + 2 >= str.length())
                return "?";
            unsigned int p = ((str[pos] & 0xF) << 12) | ((str[pos + 1] & 0x3F) << 6) | (str[pos + 2] & 0x3F);
            tmp += (p >> 12) >= 10 ? (p >> 12) + 'a' : (p >> 12) + '0';
            tmp += ((p >> 8) & 0xF) >= 10 ? ((p >> 8) & 0xF) : ((p >> 8) & 0xF) + '0';
            tmp += ((p >> 4) & 0xF) >= 10 ? ((p >> 4) & 0xF) + 'a' : ((p >> 4) & 0xF) + '0';
            tmp += (p & 0xF) >= 10 ? (p & 0xF) + 'a' : (p & 0xF) + '0';
            pos += 2;
        }
        return tmp;
    }
    std::string GenString(const std::string& str) {
        std::string tmp(1, '"');
        for (size_t i = 0; i < str.length(); ++i) {
            if (iscntrl(str[i])) {
                switch (str[i]) {
                    case ('\0'):
                        tmp += "\\0";
                        break;
                    case ('\t'):
                        tmp += "\\t";
                        break;
                    case ('\r'):
                        tmp += "\\r";
                        break;
                    case ('\n'):
                        tmp += "\\n";
                        break;
                    default:
                        tmp += GetEscape(str, i);
                        break;
                }
            }
            else if (str[i] == '"')
                tmp += "\\\"";
            else if (str[i] == '\\')
                tmp += "\\\\";
            else
                tmp += str[i];
        }
        return tmp + '"';
    }
    std::string GenValue(const Value& vl, size_t off) {
        switch (vl.GetType()) {
            case (VALUE_OBJECT):
                return GenObject(vl.AsObject(), off == ~0ull ? off : off + 1);
            case (VALUE_ARRAY):
                return GenArray(vl.AsArray(), off == ~0ull ? off : off + 1);
            case (VALUE_STRING):
                return GenString(vl.AsString());
            default:
                break;
        }
        return "";
    }
    std::string GenPair(const std::pair<std::string, Value>& pr, size_t off) {
        return off == ~0ull ? pr.first + '=' + GenValue(pr.second, off) : pr.first + " = " + GenValue(pr.second, off) ;
    }
};

}

#endif