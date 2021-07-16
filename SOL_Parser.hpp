#ifndef SOL_PARSER_HPP_INCLUDED
#define SOL_PARSER_HPP_INCLUDED

#include <cstdio>

#include <string>
#include <algorithm>

#include "SOL_Token.hpp"
#include "SOL_Value.hpp"
#include "SOL_Scanner.hpp"

namespace sol {

class Parser {
    public:
        static bool outputEscapeUnicode;

    public:
        static const std::string& error() {
            return p_error;
        }
        static Value result() {
            return p_result;
        }

        static bool fromFile(const std::string& path) {
            internal::Scanner sc(internal::SCAN_FILE, path);
            if (!sc.available()) {
                p_error = "Fail to open file";
                return false;
            }
            p_flag = true;
            sc.next();
            if (sc.token().type() == internal::TOKEN_LSBRACKET)
                p_result = p_getArray(sc);
            else if (sc.token().type() == internal::TOKEN_LCBRACKET)
                p_result = p_getObject(sc);
            else {
                p_error = "Invalid file";
                return false;
            }
            return p_flag;
        }
        static bool fromFile(std::string&& path) {
            return fromFile(path);
        }
        static bool fromString(const std::string& str) {
            internal::Scanner sc(internal::SCAN_STRING, str);
            p_flag = true;
            sc.next();
            if (sc.token().type() == internal::TOKEN_LSBRACKET)
                p_result = p_getArray(sc);
            else if (sc.token().type() == internal::TOKEN_LCBRACKET)
                p_result = p_getObject(sc);
            else {
                p_error = "Invalid string";
                return false;
            }
            return p_flag;
        }
        static bool fromString(std::string&& str) {
            return fromString(str);
        }

        static bool toFile(const std::string& path, const Value& v) {
            FILE* fout = fopen(path.c_str(), "w");
            if (fout == nullptr) {
                p_error = "Fail to create file";
                return false;
            }
            std::string s = toString(v);
            if (fwrite(s.c_str(), 1, s.length(), fout) != s.length()) {
                p_error = "Incomplete output";
                return false;
            }
            fclose(fout);
            return true;
        }
        static bool toFile(const std::string& path, Value&& v) {
            return toFile(path, v);
        }
        static bool toFile(std::string&& path, const Value& v) {
            return toFile(path, v);
        }
        static bool toFile(std::string&& path, Value&& v) {
            return toFile(path, v);
        }
        static std::string toString(const Value& v) {
            std::string rtn;
            if (v.isArray()) {
                rtn += '[';
                size_t cnt = 0;
                for (auto& i : const_cast<Value&>(v).array()) {
                    if (cnt++)
                        rtn += ',';
                    rtn += toString(i);
                }
                rtn += ']';
            }
            else if (v.isObject()) {
                rtn += '{';
                size_t cnt = 0;
                for (auto& i : const_cast<Value&>(v).object()) {
                    if (cnt++)
                        rtn += ',';
                    rtn += i.first + '=' + toString(i.second);
                }
                rtn += '}';
            }
            else if (v.isString())
                rtn = std::string("\"") + p_escape(v) + '"';
            return rtn;
        }
        static std::string toString(Value&& v) {
            return toString(v);
        }

        static bool toFile(const std::string& path, const Value& v, size_t n, size_t off = 0) {
            FILE* fout = fopen(path.c_str(), "w");
            if (fout == nullptr) {
                p_error = "Fail to create file";
                return false;
            }
            std::string s = toString(v, n, off);
            if (fwrite(s.c_str(), 1, s.length(), fout) != s.length()) {
                p_error = "Incomplete output";
                return false;
            }
            fclose(fout);
            return true;
        }
        static bool toFile(const std::string& path, Value&& v, size_t n, size_t off = 0) {
            return toFile(path, v, n, off);
        }
        static bool toFile(std::string&& path, const Value& v, size_t n, size_t off = 0) {
            return toFile(path, v, n, off);
        }
        static bool toFile(std::string&& path, Value&& v, size_t n, size_t off = 0) {
            return toFile(path, v, n, off);
        }
        static std::string toString(const Value& v, size_t n, size_t off = 0) {
            std::string rtn;
            off += n;
            if (v.isArray()) {
                rtn += "[\n";
                size_t cnt = 0;
                for (auto& i : const_cast<Value&>(v).array()) {
                    if (cnt++)
                        rtn += ",\n";
                    rtn += std::string(off, ' ') + toString(i, n, off);
                }
                rtn += std::string("\n") + std::string(off - n, ' ') + ']';
            }
            else if (v.isObject()) {
                rtn += "{\n";
                size_t cnt = 0;
                for (auto& i : const_cast<Value&>(v).object()) {
                    if (cnt++)
                        rtn += ",\n";
                    rtn += std::string(off, ' ') + i.first + " = " + toString(i.second, n, off);
                }
                rtn += std::string("\n") + std::string(off - n, ' ') + "}";
            }
            else if (v.isString())
                rtn = std::string("\"") + p_escape(v) + '"';
            return rtn;
        }
        static std::string toString(Value&& v, size_t n, size_t off = 0) {
            return toString(v, n, off);
        }
    
    private:
        static bool p_flag;
        static std::string p_error;
        static Value p_result;

    private:
        Parser() = default;
        Parser(const Parser&) = delete;
        ~Parser() = default;
        Parser& operator=(const Parser&) = delete;

        static Value p_getArray(internal::Scanner& sc) {
            Array rtn;
            sc.next();
            while (sc.token().type() != internal::TOKEN_RSBRACKET) {
                Value t;
                if (sc.token().type() == internal::TOKEN_LSBRACKET)
                    t = p_getArray(sc);
                else if (sc.token().type() == internal::TOKEN_LCBRACKET)
                    t = p_getObject(sc);
                else if (sc.token().type() == internal::TOKEN_VALUE)
                    t = sc.token().value();
                else if (sc.token().type() == internal::TOKEN_ERROR) {
                    p_flag = false;
                    p_error = sc.token().value() + sc.token().pos();
                }
                else {
                    p_flag = false;
                    p_error = std::string("Invalid array@") + sc.token().pos();
                }
                if (!p_flag)
                    break;
                rtn.emplace_back(t);
                sc.next();
                if (sc.token().type() == internal::TOKEN_COMMA)
                    sc.next();
                else if (sc.token().type() != internal::TOKEN_RSBRACKET) {
                    p_flag = false;
                    p_error = std::string("Invalid array@") + sc.token().pos();
                    break;
                }
            }
            return p_flag ? rtn : Array();
        }
        static Value p_getObject(internal::Scanner& sc) {
            Object rtn;
            sc.next();
            while (sc.token().type() != internal::TOKEN_RCBRACKET) {
                if (sc.token().type() != internal::TOKEN_KEY) {
                    p_flag = false;
                    if (sc.token().type() == internal::TOKEN_ERROR)
                        p_error = sc.token().value() + sc.token().pos();
                    else
                        p_error = std::string("Invalid object@") + sc.token().pos();
                    break;
                }
                std::string k = sc.token().value();
                sc.next();
                if (sc.token().type() != internal::TOKEN_EQUAL) {
                    p_flag = false;
                    if (sc.token().type() == internal::TOKEN_ERROR)
                        p_error = sc.token().value() + sc.token().pos();
                    else
                        p_error = std::string("Invalid object@") + sc.token().pos();
                    break;
                }
                Value t;
                sc.next();
                if (sc.token().type() == internal::TOKEN_LSBRACKET)
                    t = p_getArray(sc);
                else if (sc.token().type() == internal::TOKEN_LCBRACKET)
                    t = p_getObject(sc);
                else if (sc.token().type() == internal::TOKEN_VALUE)
                    t = sc.token().value();
                else if (sc.token().type() == internal::TOKEN_ERROR) {
                    p_flag = false;
                    p_error = sc.token().value() + sc.token().pos();
                }
                else {
                    p_flag = false;
                    p_error = std::string("Invalid object@") + sc.token().pos();
                }
                if (!p_flag)
                    break;
                rtn[k] = t;
                sc.next();
                if (sc.token().type() == internal::TOKEN_COMMA)
                    sc.next();
                else if (sc.token().type() != internal::TOKEN_RCBRACKET) {
                    p_flag = false;
                    p_error = std::string("Invalid object@") + sc.token().pos();
                    break;
                }
            }
            return p_flag ? rtn : Object();
        }

        static std::string p_u2x(unsigned int u, size_t len) {
            std::string rtn;
            if (!u)
                rtn += '0';
            while (u) {
                if (u % 16 < 10)
                    rtn += u % 16 + '0';
                else
                    rtn += u % 16 - 10 + 'A';
                u /= 16;
            }
            for (size_t i = 0; i < (rtn.length() >> 1); ++i)
                std::swap(rtn[i], rtn[rtn.length() - i - 1]);
            while (rtn.length() < len)
                rtn = std::string("0") + rtn;
            return rtn;
        }
        static std::string p_escape(const Value& v) {
            std::string rtn;
            for (size_t i = 0; i < const_cast<Value&>(v).string().length(); ++i) {
                unsigned char c = const_cast<Value&>(v).string()[i];
                if (c <= 0x7F || !outputEscapeUnicode) {
                    switch (c) {
                        case ('\t'):
                            rtn += "\\t";
                            break;
                        case ('\n'):
                            rtn += "\\n";
                            break;
                        case ('\r'):
                            rtn += "\\r";
                            break;
                        case ('"'):
                            rtn += "\\\"";
                            break;
                        case ('\\'):
                            rtn += "\\\\";
                            break;
                        default:
                            rtn += c;
                    }
                }
                else if ((c >> 5) == 0x6) {
                    unsigned int xc = (c & 0x1F) << 6;
                    if (++i >= const_cast<Value&>(v).string().length())
                        break;
                    c = const_cast<Value&>(v).string()[i];
                    xc |= c & 0x3F;
                    rtn += std::string("\\u") + p_u2x(xc, 4);
                }
                else if ((c >> 4) == 0xE) {
                    unsigned int xc = (c & 0xF) << 12;
                    if (++i >= const_cast<Value&>(v).string().length())
                        break;
                    c = const_cast<Value&>(v).string()[i];
                    xc |= (c & 0x3F) << 6;
                    if (++i >= const_cast<Value&>(v).string().length())
                        break;
                    c = const_cast<Value&>(v).string()[i];
                    xc |= c & 0x3F;
                    rtn += std::string("\\u") + p_u2x(xc, 4);
                }
            }
            return rtn;
        }
};

bool Parser::outputEscapeUnicode = true;
bool Parser::p_flag;
std::string Parser::p_error;
Value Parser::p_result;

}

#endif