#ifndef SOL_TOKEN_HPP_INCLUDED
#define SOL_TOKEN_HPP_INCLUDED

#include <string>

namespace sol {
namespace internal {

using TokenType = enum _ {
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
};

class Token {
    public:
        Token(TokenType type = TOKEN_EOF, size_t line = 1, size_t column = 1, const std::string& value = std::string()):
            p_type(type), p_line(line), p_column(column), p_value(value) {}
        Token(TokenType type, size_t line = 1, size_t column = 1, std::string&& value = ""):
            p_type(type), p_line(line), p_column(column) {
            p_value.swap(value);
        }
        Token(const Token& t):
            p_type(t.p_type), p_line(t.p_line), p_column(t.p_column), p_value(t.p_value) {}
        Token(Token&& t): p_type(t.p_type), p_line(t.p_line), p_column(t.p_column) {
            p_value.swap(t.p_value);
        }
        ~Token() = default;

        Token& operator=(const Token& t) {
            p_type = t.p_type;
            p_line = t.p_line;
            p_column = t.p_column;
            p_value = t.p_value;
            return *this;
        }
        Token& operator=(Token&& t) {
            p_type = t.p_type;
            p_line = t.p_line;
            p_column = t.p_column;
            p_value.swap(t.p_value);
            return *this;
        }

        TokenType type() const {
            return p_type;
        }
        std::string pos() const {
            return std::string("Line: ") + std::to_string(p_line) + " Column: " + std::to_string(p_column);
        }
        const std::string& value() const {
            return p_value;
        }

    private:
        TokenType p_type;
        size_t p_line, p_column;
        std::string p_value;
};

}
}

#endif