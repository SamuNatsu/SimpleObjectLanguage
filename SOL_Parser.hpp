#ifndef SOL_PARSER_HPP_INCLUDED
#define SOL_PARSER_HPP_INCLUDED

#include <cstdio>

#include <string>
#include <algorithm>

#include "SOL_Token.hpp"
#include "SOL_Value.hpp"
#include "SOL_Scanner.hpp"

namespace {
    bool _outputEscapeUnicode;
    bool flag;
    std::string error;
    sol::Value result;
}

namespace sol {

class Parser {
    public:
        Parser() = delete;
        Parser(const Parser&) = delete;
        ~Parser() = delete;

        Parser& operator=(const Parser&) = delete;

        static const std::string& error() {
            return ::error;
        }
        static Value result() {
            return ::result;
        }

        static void outputEscapeUnicode(bool b) {
            ::_outputEscapeUnicode = b;
        }

        static bool fromFile(const std::string& path) {
            internal::Scanner sc(internal::SCAN_FILE, path);
            if (!sc.available()) {
                ::error = "Fail to open file";
                return false;
            }
            ::flag = true;
            sc.next();
            if (sc.token().type() == internal::TOKEN_LSBRACKET)
                ::result = p_getArray(sc);
            else if (sc.token().type() == internal::TOKEN_LCBRACKET)
                ::result = p_getObject(sc);
            else {
                ::error = "Invalid file";
                return false;
            }
            return ::flag;
        }
        static bool fromString(const std::string& str) {
            internal::Scanner sc(internal::SCAN_STRING, str);
            ::flag = true;
            sc.next();
            if (sc.token().type() == internal::TOKEN_LSBRACKET)
                ::result = p_getArray(sc);
            else if (sc.token().type() == internal::TOKEN_LCBRACKET)
                ::result = p_getObject(sc);
            else {
                ::error = "Invalid string";
                return false;
            }
            return ::flag;
        }

        static bool toFile(const std::string& path, const Value& v) {
            FILE* fout = fopen(path.c_str(), "w");
            if (fout == nullptr) {
                ::error = "Fail to create file";
                return false;
            }
            std::string s = toString(v);
            if (fwrite(s.c_str(), 1, s.length(), fout) != s.length()) {
                ::error = "Incomplete output";
                return false;
            }
            fclose(fout);
            return true;
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

        static bool toFile(const std::string& path, const Value& v, size_t n, size_t off = 0) {
            FILE* fout = fopen(path.c_str(), "w");
            if (fout == nullptr) {
                ::error = "Fail to create file";
                return false;
            }
            std::string s = toString(v, n, off);
            if (fwrite(s.c_str(), 1, s.length(), fout) != s.length()) {
                ::error = "Incomplete output";
                return false;
            }
            fclose(fout);
            return true;
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

    private:
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
                    ::flag = false;
                    ::error = sc.token().value() + sc.token().pos();
                }
                else {
                    ::flag = false;
                    ::error = std::string("Invalid array@") + sc.token().pos();
                }
                if (!::flag)
                    break;
                rtn.emplace_back(t);
                sc.next();
                if (sc.token().type() == internal::TOKEN_COMMA)
                    sc.next();
                else if (sc.token().type() != internal::TOKEN_RSBRACKET) {
                    ::flag = false;
                    ::error = std::string("Invalid array@") + sc.token().pos();
                    break;
                }
            }
            return ::flag ? rtn : Array();
        }
        static Value p_getObject(internal::Scanner& sc) {
            Object rtn;
            sc.next();
            while (sc.token().type() != internal::TOKEN_RCBRACKET) {
                if (sc.token().type() != internal::TOKEN_KEY) {
                    ::flag = false;
                    if (sc.token().type() == internal::TOKEN_ERROR)
                        ::error = sc.token().value() + sc.token().pos();
                    else
                        ::error = std::string("Invalid object@") + sc.token().pos();
                    break;
                }
                std::string k = sc.token().value();
                sc.next();
                if (sc.token().type() != internal::TOKEN_EQUAL) {
                    ::flag = false;
                    if (sc.token().type() == internal::TOKEN_ERROR)
                        ::error = sc.token().value() + sc.token().pos();
                    else
                        ::error = std::string("Invalid object@") + sc.token().pos();
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
                    ::flag = false;
                    ::error = sc.token().value() + sc.token().pos();
                }
                else {
                    ::flag = false;
                    ::error = std::string("Invalid object@") + sc.token().pos();
                }
                if (!::flag)
                    break;
                rtn[k] = t;
                sc.next();
                if (sc.token().type() == internal::TOKEN_COMMA)
                    sc.next();
                else if (sc.token().type() != internal::TOKEN_RCBRACKET) {
                    ::flag = false;
                    ::error = std::string("Invalid object@") + sc.token().pos();
                    break;
                }
            }
            return ::flag ? rtn : Object();
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
                if (c <= 0x7F || !::_outputEscapeUnicode) {
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

}

#endif