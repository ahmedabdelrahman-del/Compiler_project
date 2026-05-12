#pragma once

#include "token.h"

#include <string>
#include <vector>

namespace minicompiler {

class Lexer {
public:
    explicit Lexer(std::string source);

    std::vector<Token> tokenize();

private:
    char peek(std::size_t offset = 0) const;
    char advance();
    bool match(char expected);
    bool isAtEnd() const;
    void emitIndentation(std::vector<Token>& tokens);
    void emitDedents(std::vector<Token>& tokens, std::size_t line, std::size_t column);
    void skipComment();

    std::string source;
    std::size_t start;
    std::size_t current;
    std::size_t line;
    std::size_t column;
    bool atLineStart;
    std::vector<std::size_t> indentationStack;
};

} // namespace minicompiler
