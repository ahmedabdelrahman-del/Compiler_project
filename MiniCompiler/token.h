#pragma once

#include <cstddef>
#include <string>
#include <utility>

namespace minicompiler {

enum class TokenType {
    EndOfFile,
    Newline,
    Indent,
    Dedent,
    Identifier,
    Number,
    If,
    Print,
    Plus,
    Minus,
    Star,
    Slash,
    Equal,
    EqualEqual,
    BangEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,
    LParen,
    RParen,
    Colon,
    Comma
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::size_t line;
    std::size_t column;
    long long numberValue;
    bool hasNumberValue;
};

inline Token makeToken(TokenType type, std::string lexeme, std::size_t line, std::size_t column) {
    return Token{type, std::move(lexeme), line, column, 0, false};
}

inline Token makeNumberToken(std::string lexeme, std::size_t line, std::size_t column, long long value) {
    return Token{TokenType::Number, std::move(lexeme), line, column, value, true};
}

inline bool isComparisonOperator(TokenType type) {
    return type == TokenType::Greater || type == TokenType::GreaterEqual ||
           type == TokenType::Less || type == TokenType::LessEqual ||
           type == TokenType::EqualEqual || type == TokenType::BangEqual;
}

} // namespace minicompiler
