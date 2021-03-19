/**
 * @file SOL.hpp
 * @author SamuNatsu
 * @brief A simple toolkit for SOL, repo: https://github.com/SamuNatsu/SimpleObjectLanguage
 * @version 3.0.0
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021 SamuNatsu
 * 
 */

/**
 * MIT License
 * 
 * Copyright (c) 2021 SamuNatsu
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

#ifndef SOL_TOOLKIT_HPP_INCLUDED
#define SOL_TOOLKIT_HPP_INCLUDED

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#define SOL_VERSION         "3.0.0"
#define SOL_VERSION_MAJOR   3
#define SOL_VERSION_MINOR   0
#define SOL_VERSION_PATCH   0

namespace sol {

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

        #define _SOL_THIS_ASSIGN_   {*this = tmp;}
        #define _SOL_THIS_MOVE_     {*this = std::forward<decltype(tmp)>(tmp);}
            Value(const Value& tmp)         _SOL_THIS_ASSIGN_
            Value(Value&& tmp)              _SOL_THIS_MOVE_
            Value(const Object& tmp)        _SOL_THIS_ASSIGN_
            Value(Object&& tmp)             _SOL_THIS_MOVE_
            Value(const Array& tmp)         _SOL_THIS_ASSIGN_
            Value(Array&& tmp)              _SOL_THIS_MOVE_
            Value(const std::string& tmp)   _SOL_THIS_ASSIGN_
            Value(std::string&& tmp)        _SOL_THIS_MOVE_
        #undef _SOL_THIS_ASSIGN_
        #undef _SOL_THIS_MOVE_

        ~Value() {p_clear();}

        Value& operator=(const Value& tmp) {
            p_clear();
            m_Type = tmp.m_Type;
            switch (m_Type) {
                case (VALUE_NULL): break;
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
            p_clear();
            std::swap(m_Type, tmp.m_Type);
            std::swap(m_Data, tmp.m_Data);
            return *this;
        }

        #define _SOL_ASSIGN_(type, typename)    {p_clear(); m_Type = type; m_Data = new typename(tmp); return *this;}
        #define _SOL_MOVE_(type, typename)      {p_clear(); m_Type = type; m_Data = new typename(std::forward<typename>(tmp)); return *this;}
            Value& operator=(const Object& tmp)         _SOL_ASSIGN_(VALUE_OBJECT, Object)
            Value& operator=(Object&& tmp)              _SOL_MOVE_(VALUE_OBJECT, Object)
            Value& operator=(const Array& tmp)          _SOL_ASSIGN_(VALUE_ARRAY, Array)
            Value& operator=(Array&& tmp)               _SOL_MOVE_(VALUE_ARRAY, Array)
            Value& operator=(const std::string& tmp)    _SOL_ASSIGN_(VALUE_STRING, std::string)
            Value& operator=(std::string&& tmp)         _SOL_MOVE_(VALUE_STRING, std::string)
        #undef _SOL_ASSIGN_
        #undef _SOL_MOVE_

        Value& operator[](const std::string& tmp) {
            return m_Type == VALUE_OBJECT ? (*(Object*)m_Data)[tmp] : (*this = Object(), (*(Object*)m_Data)[tmp]);
        }
        Value& operator[](std::string&& tmp) {return (*this)[tmp];}
        Value& operator[](size_t tmp) {
            if (m_Type != VALUE_ARRAY)
                *this = Array();
            while (tmp >= ((Array*)m_Data)->size())
                ((Array*)m_Data)->emplace_back();
            return (*(Array*)m_Data)[tmp];
        }

        #define _SOL_CMP_AND_RET_(type, typename) {return m_Type == type ? *(typename*)m_Data : typename();}
            operator Object() const         _SOL_CMP_AND_RET_(VALUE_OBJECT, Object)
            operator Array() const          _SOL_CMP_AND_RET_(VALUE_ARRAY, Array)
            operator std::string() const    _SOL_CMP_AND_RET_(VALUE_STRING, std::string)
        #undef _SOL_CMP_AND_RET_

        #define _SOL_CMP_AND_RET_(type, typename) {static typename s_Empty; return m_Type == type ? *(typename*)m_Data : s_Empty;}
            const Object& asObject() const          _SOL_CMP_AND_RET_(VALUE_OBJECT, Object)
            const Array& asArray() const            _SOL_CMP_AND_RET_(VALUE_ARRAY, Array)
            const std::string& asString() const     _SOL_CMP_AND_RET_(VALUE_STRING, std::string)
        #undef _SOL_CMP_AND_RET_

        long long asInteger() const {
            static long long s_Empty = 0, rtn;
            static std::stringstream ss;
            if (m_Type != VALUE_STRING)
                return s_Empty;
            ss.str(*this);
            ss >> rtn;
            return rtn;
        }
        double asDouble() const {
            static double s_Empty = 0, rtn;
            static std::stringstream ss;
            if (m_Type != VALUE_STRING)
                return s_Empty;
            ss.str(*this);
            ss >> rtn;
            return rtn;
        }
        bool asBool() const {
            if (m_Type != VALUE_STRING)
                return false;
            return asString() == "true";
        }

        ValueType getType() const {return m_Type;}

    private:
        ValueType m_Type = VALUE_NULL;
        void *m_Data = nullptr;

        void p_clear() {
            switch (m_Type) {
                case (VALUE_NULL): break;
                case (VALUE_OBJECT):
                    delete (Object*)m_Data;
                    break;
                case (VALUE_ARRAY):
                    delete (Array*)m_Data;
                    break;
                case (VALUE_STRING):
                    delete (std::string*)m_Data;
                    break;
            }
            m_Type = VALUE_NULL;
            m_Data = nullptr;
        }
};

class getValue {
    public:
        Value operator()(const Object& obj, const std::string& key) const {
            std::string sub = key.substr(0, key.find_first_of('.'));
            if (obj.find(sub) == obj.end())
                return Value();
            if (sub == key)
                return obj.at(sub);
            if (obj.at(sub).getType() != VALUE_OBJECT)
                return Value();
            return getValue()(obj.at(sub).asObject(), key.substr(key.find_first_of('.') + 1));
        }
        Value operator()(const Object& obj, std::string&& key) const {
            return getValue()(obj, key);
        }
};

class Parser {
    public:
        Parser() = default;
        Parser(const Parser&) = delete;
        ~Parser() = default;

        Parser& operator=(const Parser&) = delete;

        const std::string& getError() const {return m_Error;}

        bool parseFromFile(const std::string& path) {
            m_Error = "";
            if (!p_open(path)) {
                m_Error = "Cannot open file";
                return false;
            }
            p_nextToken();
            if (m_Type != TOKEN_LCBRACKET) {
                m_Error = std::string("Invalid object -> ") + p_getPosition();
                return false;
            }
            m_Exception = false;
            m_Result = p_getObject();
            return !m_Exception;
        }
        bool parseFromFile(std::string&& path) {return parseFromFile(path);}

        bool parseFromMemory(const std::string& mem) {
            m_Error = "";
            p_open(mem, false);
            p_nextToken();
            if (m_Type != TOKEN_LCBRACKET) {
                m_Error = std::string("Invalid object -> ") + p_getPosition();
                return false;
            }
            m_Exception = false;
            m_Result = p_getObject();
            return !m_Exception;
        }
        bool parseFromMemory(std::string&& mem) {return parseFromMemory(mem);}

        Object getResult() const {return m_Result;}

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

        bool p_open(const std::string& tmp, bool type = true) {
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
            else
                return m_Sin.str(tmp), true;
        }
        bool p_isEOF() const {return m_Flag ? m_Fin.eof() : m_Sin.eof();}
        char p_peekChar() {return m_Flag ? m_Fin.peek() : m_Sin.peek();}
        char p_getChar() {return m_Flag ? m_Fin.get() : m_Sin.get();}
        void p_skipChar() {
            char c = p_peekChar();
            while (!isgraph(c) && c >= 0) {
                if (c == '\n') {
                    ++m_Line;
                    m_Column = -1;
                }
                ++m_Column;
                p_getChar();
                c = p_peekChar();
            }
        }

        std::string p_getUnescape() {
            char c;
            std::string t;
            unsigned int b = 0;
            for (int i = 0; i < 4; ++i) {
                ++m_Column;
                p_getChar();
                c = p_peekChar();
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
        void p_getValue() {
            char c = p_peekChar();
            std::string buf;
            while (c != EOF) {
                switch (c) {
                    case ('"'):
                        m_Type = TOKEN_VALUE;
                        m_Value = buf;
                        ++m_Column;
                        p_getChar();
                        return;
                    case ('\\'): {
                        ++m_Column;
                        p_getChar();
                        c = p_peekChar();
                        if (c == EOF) {
                            m_Type = TOKEN_ERROR;
                            m_Value = std::string("Invalid value -> ") + p_getPosition();
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
                                buf += p_getUnescape();
                                break;
                            default:
                                if (iscntrl(c)) {
                                    m_Type = TOKEN_ERROR;
                                    m_Value = std::string("Invalid value -> ") + p_getPosition();
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
                            m_Value = std::string("Invalid value -> ") + p_getPosition();
                            return;
                        }
                        buf += c;
                        break;
                }
                ++m_Column;
                p_getChar();
                c = p_peekChar();
            }
            m_Type = TOKEN_ERROR;
            m_Value = std::string("Invalid value -> ") + p_getPosition();
        }
        void p_getKey(char tmp) {
            if (!isalpha(tmp) && tmp != '_') {
                m_Type = TOKEN_ERROR;
                m_Value = std::string("Invalid key -> ") + p_getPosition();
                return;
            }
            std::string buf(1, tmp);
            char c = p_peekChar();
            while (isalpha(c) || isdigit(c) || c == '_') {
                buf += c;
                ++m_Column;
                p_getChar();
                c = p_peekChar();
            }
            m_Type = TOKEN_KEY;
            m_Value = buf;
        }
        void p_nextToken() {
            p_skipChar();
            if (p_isEOF()) {
                m_Type = TOKEN_EOF;
                m_Value = "EOF";
                return;
            }
            char c = p_getChar();
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
                    p_getValue();
                    break;
                default:
                    p_getKey(c);
                    break;
            }
        }

    Object m_Result;
    bool m_Exception;
    std::string m_Error;

    std::string p_getPosition() const {
        return std::string("Line: ") + std::to_string(m_Line) + std::string(" Column: ") + std::to_string(m_Column);
    }
    Object p_getObject() {
        Object obj;
        p_nextToken();
        while (true) {
            if (m_Type == TOKEN_RCBRACKET)
                return obj;
            std::pair<std::string, Value> pr = p_getPair();
            if (m_Exception)
                return Object();
            p_nextToken();
            if (m_Type == TOKEN_COMMA) {
                p_nextToken();
                if (m_Type != TOKEN_KEY) {
                    m_Error = std::string("Invalid key -> ") + p_getPosition();
                    m_Exception = true;
                    return Object();
                }
            }
            else if (m_Type != TOKEN_RCBRACKET) {
                if (m_Type == TOKEN_ERROR)
                    m_Error = m_Value;
                else 
                    m_Error = std::string("Object syntax error -> ") + p_getPosition();
                m_Exception = true;
                return Object();
            }
            obj[pr.first] = pr.second;
        }
    }
    Array p_getArray() {
        Array arr;
        p_nextToken();
        while (true) {
            if (m_Type == TOKEN_RSBRACKET)
                return arr;
            Value vl = p_getValueEx();
            if (m_Exception)
                return Array();
            p_nextToken();
            if (m_Type == TOKEN_COMMA) {
                p_nextToken();
                if (m_Type != TOKEN_LCBRACKET && m_Type != TOKEN_LSBRACKET && m_Type != TOKEN_VALUE) {
                    m_Error = std::string("Invalid value -> ") + p_getPosition();
                    m_Exception = true;
                    return Array();
                }
            }
            else if (m_Type != TOKEN_RSBRACKET) {
                if (m_Type == TOKEN_ERROR)
                    m_Error = m_Value;
                else 
                    m_Error = std::string("Array syntax error -> ") + p_getPosition();
                m_Exception = true;
                return Array();
            }
            arr.emplace_back(vl);
        }
    }
    Value p_getValueEx() {
        Value vl;
        switch (m_Type) {
            case (TOKEN_LCBRACKET):
                vl = p_getObject();
                break;
            case (TOKEN_LSBRACKET):
                vl = p_getArray();
                break;
            case (TOKEN_VALUE):
                return m_Value;
            default:
                if (m_Type == TOKEN_ERROR)
                    m_Error = m_Value;
                else 
                    m_Error = std::string("Invalid value -> ") + p_getPosition();
                m_Exception = true;
                break;
        }
        return m_Exception ? Value() : vl;
    }
    std::pair<std::string, Value> p_getPair() {
        if (m_Type != TOKEN_KEY) {
            if (m_Type == TOKEN_ERROR)
                m_Error = m_Value;
            else 
                m_Error = std::string("Invalid key -> ") + p_getPosition();
            m_Exception = true;
            return std::pair<std::string, Value>();
        }
        std::string key = m_Value;
        p_nextToken();
        if (m_Type != TOKEN_EQUAL) {
            if (m_Type == TOKEN_ERROR)
                m_Error = m_Value;
            else 
                m_Error = std::string("KVP syntax error -> ") + p_getPosition();
            m_Exception = true;
            return std::pair<std::string, Value>();
        }
        p_nextToken();
        Value vl = p_getValueEx();
        return m_Exception ? std::pair<std::string, Value>() : std::make_pair(key, vl);
    }
};

class convertToString {
    public:
        std::string operator()(const Object& obj, bool flag = false) const {
            return p_genObject(obj, flag ? ~0ull : 0);
        }

    private:
        std::string p_genObject(const Object& obj, size_t off) const {
            std::string tmp(1, '{');
            size_t cnt = 0;
            if (off == ~0ull)
                for (auto& i : obj) {
                    if (cnt++)
                        tmp += ',';
                    tmp += p_genPair(i, off);
                }
            else {
                tmp += '\n';
                for (auto& i : obj) {
                    if (cnt++)
                        tmp += ",\n";
                    for (size_t i = 0; i < off + 1; ++i)
                        tmp += '\t';
                    tmp += p_genPair(i, off);
                }
                tmp += '\n';
                for (size_t i = 0; i < off; ++i)
                    tmp += '\t';
            }
            return tmp + '}';
        }
        std::string p_genArray(const Array& arr, size_t off) const {
            std::string tmp(1, '[');
            size_t cnt = 0;
            if (off == ~0ull)
                for (auto& i : arr) {
                    if (cnt++)
                        tmp += ',';
                    tmp += p_genValue(i, off);
                }
            else {
                tmp += '\n';
                for (auto& i : arr) {
                    if (cnt++)
                        tmp += ",\n";
                    for (size_t i = 0; i < off + 1; ++i)
                        tmp += '\t';
                    tmp += p_genValue(i, off);
                }
                tmp += '\n';
                for (size_t i = 0; i < off; ++i)
                    tmp += '\t';
            }
            return tmp + ']';
        }
        std::string p_getEscape(const std::string& str, size_t& pos) const {
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
        std::string p_genString(const std::string& str) const {
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
                            tmp += p_getEscape(str, i);
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
        std::string p_genValue(const Value& vl, size_t off) const {
            switch (vl.getType()) {
                case (VALUE_OBJECT):
                    return p_genObject(vl.asObject(), off == ~0ull ? off : off + 1);
                case (VALUE_ARRAY):
                    return p_genArray(vl.asArray(), off == ~0ull ? off : off + 1);
                case (VALUE_STRING):
                    return p_genString(vl.asString());
                default:
                    break;
            }
            return "";
        }
        std::string p_genPair(const std::pair<std::string, Value>& pr, size_t off) const {
            return off == ~0ull ? pr.first + '=' + p_genValue(pr.second, off) : pr.first + " = " + p_genValue(pr.second, off) ;
        }
};

class saveToFile {
    public:
        bool operator()(const std::string& path, const Object& obj, bool flag = false) {
            std::ofstream fout;
            fout.open(path);
            if (!fout.is_open())
                return false;
            fout << convertToString()(obj, flag);
            fout.flush();
            fout.close();
            return true;
        }
        bool operator()(std::string&& path, const Object& obj, bool flag = false) {
            return saveToFile()(path, obj, flag);
        }
};

}

#endif
