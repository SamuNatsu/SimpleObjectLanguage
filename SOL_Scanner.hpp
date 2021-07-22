#ifndef SOL_SCANNER_HPP_INCLUDED
#define SOL_SCANNER_HPP_INCLUDED

#include <string>
#include <fstream>
#include <sstream>

#include "SOL_Token.hpp"

namespace sol {
namespace internal {

using ScanType = enum __ {
    SCAN_FILE,
    SCAN_STRING
};

class Scanner {
    public:
        Scanner(ScanType type, const std::string& str): p_type(type) {
            if (type == SCAN_FILE)
                p_fin.open(str);
            else 
                p_sin.str(str);
        }
        Scanner(ScanType type, std::string&& str): p_type(type) {
            if (type == SCAN_FILE)
                p_fin.open(str);
            else 
                p_sin.str(str);
        }
        Scanner(const Scanner&) = delete;
        ~Scanner() {
            if (p_type == SCAN_FILE && p_fin.is_open())
                p_fin.close();
        }

        Scanner& operator=(const Scanner&) = delete;

        bool available() const {
            return p_type == SCAN_FILE ? p_fin.is_open() : true;
        }

        const Token& token() const {
            return p_token;
        }
        void next() {
            p_skip();
            switch (p_peek()) {
                case (EOF):
                    p_token = Token();
                    break;
                case ('{'):
                    p_token = Token(TOKEN_LCBRACKET, p_line, p_column, "{");
                    p_get();
                    break;
                case ('['):
                    p_token = Token(TOKEN_LSBRACKET, p_line, p_column, "[");
                    p_get();
                    break;
                case ('}'):
                    p_token = Token(TOKEN_RCBRACKET, p_line, p_column, "}");
                    p_get();
                    break;
                case (']'):
                    p_token = Token(TOKEN_RSBRACKET, p_line, p_column, "]");
                    p_get();
                    break;
                case ('='):
                    p_token = Token(TOKEN_EQUAL, p_line, p_column, "=");
                    p_get();
                    break;
                case (','):
                    p_token = Token(TOKEN_COMMA, p_line, p_column, ",");
                    p_get();
                    break;
                case ('"'):
                    p_token = p_getValue();
                    p_get();
                    break;
                default:
                    if (isalpha(p_peek()) || p_peek() == '_')
                        p_token = p_getKey();
                    else 
                        p_token = Token(TOKEN_ERROR, p_line, p_column, "Invalid key");
                    break;
            }
        }

    private:
        Token p_token;
        ScanType p_type;
        std::ifstream p_fin;
        std::istringstream p_sin;
        size_t p_line = 1, p_column = 1;

    private:
        bool p_eof() const {
            return p_type == SCAN_FILE ? p_fin.eof() : p_sin.eof();
        }
        char p_peek() {
            return p_type == SCAN_FILE ? p_fin.peek() : p_sin.peek();
        }
        char p_get() {
            return p_type == SCAN_FILE ? p_fin.get() : p_sin.get();
        }
        void p_skip() {
            char c = p_peek();
            while (isspace(c)) {
                if (c == '\n') {
                    ++p_line;
                    p_column = 1;
                }
                if (isblank(c))
                    ++p_column;
                p_get();
                c = p_peek();
            }
        }
        unsigned int p_x2d(char c) {
            if (isdigit(c))
                return c - '0';
            else if (isupper(c))
                return c - 'A' + 10;
            else 
                return c - 'a' + 10;
        }
        std::string p_unescape() {
            p_get();
            ++p_column;
            char c = p_peek();
            switch (c) {
                case ('t'):
                    return "\t";
                case ('n'):
                    return "\n";
                case ('r'):
                    return "\r";
                case ('"'):
                    return "\"";
                case ('\\'):
                    return "\\";
                case ('u'): {
                    unsigned int xnum = 0;
                    std::string xs;
                    p_get();
                    ++p_column;
                    c = p_peek();
                    if (!isxdigit(c))
                        return std::string("\\u") + xs;
                    xs += c;
                    xnum = (xnum << 4) + p_x2d(c);
                    p_get();
                    ++p_column;
                    c = p_peek();
                    if (!isxdigit(c))
                        return std::string("\\u") + xs;
                    xs += c;
                    xnum = (xnum << 4) + p_x2d(c);
                    p_get();
                    ++p_column;
                    c = p_peek();
                    if (!isxdigit(c))
                        return std::string("\\u") + xs;
                    xs += c;
                    xnum = (xnum << 4) + p_x2d(c);
                    p_get();
                    ++p_column;
                    c = p_peek();
                    if (!isxdigit(c))
                        return std::string("\\u") + xs;
                    xnum = (xnum << 4) + p_x2d(c);
                    if (xnum <= 0x7F)
                        return std::string(1, xnum);
                    else if (xnum <= 0x7FF){
                        xs = "";
                        xs += char(0xC0 | (xnum >> 6));
                        xs += char(0x80 | (xnum & 0x3F));
                        return xs;
                    }
                    else {
                        xs = "";
                        xs += char(0xE0 | (xnum >> 12));
                        xs += char(0x80 | ((xnum >> 6) & 0x3F));
                        xs += char(0x80 | (xnum & 0x3F));
                        return xs;
                    }
                }
                default:
                    return std::string("\\") + c;
            }
        }

        Token p_getValue() {
            size_t line = p_line, column = p_column;
            p_get();
            ++p_column;
            char c = p_peek();
            std::string s;
            while (c != EOF) {
                if (c == '"')
                    return Token(TOKEN_VALUE, line, column, s);
                if (iscntrl(c))
                    return Token(TOKEN_ERROR, line, column, "Invalid value character");
                if (c == '\\')
                    s += p_unescape();
                else 
                    s += c;
                p_get();
                c = p_peek();
                ++p_column;
            }
            return Token(TOKEN_ERROR, line, column, "Incomplete value");
        }
        Token p_getKey() {
            size_t line = p_line, column = p_column;
            std::string k(1, p_peek());
            p_get();
            ++p_column;
            char c = p_peek();
            while (isalpha(c) || isdigit(c) || c == '_') {
                k += c;
                p_get();
                c = p_peek();
            }
            return Token(TOKEN_KEY, line, column, k);
        }
};

}
}

#endif