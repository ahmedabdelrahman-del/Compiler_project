#include "lexer.h"

#include <cctype>
#include <stdexcept>

namespace minicompiler {

Lexer::Lexer(std::string source)
    : source(std::move(source)), start(0), current(0), line(1), column(1), atLineStart(true), indentationStack{0} {}

char Lexer::peek(std::size_t offset) const {
    if (current + offset >= source.size()) {
        return '\0';
    }
    return source[current + offset];
}

char Lexer::advance() {
    if (isAtEnd()) {
        return '\0';
    }

    char ch = source[current++];
    if (ch == '\n') {
        ++line;
        column = 1;
        atLineStart = true;
    } else {
        ++column;
    }
    return ch;
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source[current] != expected) {
        return false;
    }
    advance();
    return true;
}

bool Lexer::isAtEnd() const {
    return current >= source.size();
}

void Lexer::emitIndentation(std::vector<Token>& tokens) {
    std::size_t spaces = 0;
    while (!isAtEnd() && peek() == ' ') {
        advance();
        ++spaces;
    }

    if (!isAtEnd() && peek() == '\n') {
        advance();
        return;
    }

    std::size_t tokenLine = line;
    std::size_t tokenColumn = column;

    if (spaces > indentationStack.back()) {
        indentationStack.push_back(spaces);
        tokens.push_back(makeToken(TokenType::Indent, "<indent>", tokenLine, tokenColumn));
    } else {
        emitDedents(tokens, tokenLine, tokenColumn);
        if (spaces != indentationStack.back()) {
            throw std::runtime_error("Inconsistent indentation at line " + std::to_string(tokenLine));
        }
    }

    atLineStart = false;
}

void Lexer::emitDedents(std::vector<Token>& tokens, std::size_t lineValue, std::size_t columnValue) {
    while (indentationStack.size() > 1 && indentationStack.back() > 0) {
        indentationStack.pop_back();
        tokens.push_back(makeToken(TokenType::Dedent, "<dedent>", lineValue, columnValue));
    }
}

void Lexer::skipComment() {
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        start = current;

        if (atLineStart) {
            emitIndentation(tokens);
            if (atLineStart) {
                continue;
            }
        }

        char ch = peek();
        if (ch == ' ' || ch == '\r' || ch == '\t') {
            advance();
            continue;
        }

        if (ch == '\n') {
            std::size_t tokenLine = line;
            std::size_t tokenColumn = column;
            advance();
            tokens.push_back(makeToken(TokenType::Newline, "\\n", tokenLine, tokenColumn));
            continue;
        }

        if (ch == '#') {
            skipComment();
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(ch))) {
            std::size_t tokenLine = line;
            std::size_t tokenColumn = column;
            std::string lexeme;
            while (std::isdigit(static_cast<unsigned char>(peek()))) {
                lexeme.push_back(advance());
            }
            long long value = std::stoll(lexeme);
            tokens.push_back(makeNumberToken(lexeme, tokenLine, tokenColumn, value));
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(ch)) || ch == '_') {
            std::size_t tokenLine = line;
            std::size_t tokenColumn = column;
            std::string lexeme;
            while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
                lexeme.push_back(advance());
            }

            if (lexeme == "if") {
                tokens.push_back(makeToken(TokenType::If, lexeme, tokenLine, tokenColumn));
            } else if (lexeme == "print") {
                tokens.push_back(makeToken(TokenType::Print, lexeme, tokenLine, tokenColumn));
            } else {
                tokens.push_back(makeToken(TokenType::Identifier, lexeme, tokenLine, tokenColumn));
            }
            continue;
        }

        std::size_t tokenLine = line;
        std::size_t tokenColumn = column;
        switch (advance()) {
            case '+': tokens.push_back(makeToken(TokenType::Plus, "+", tokenLine, tokenColumn)); break;
            case '-': tokens.push_back(makeToken(TokenType::Minus, "-", tokenLine, tokenColumn)); break;
            case '*': tokens.push_back(makeToken(TokenType::Star, "*", tokenLine, tokenColumn)); break;
            case '/': tokens.push_back(makeToken(TokenType::Slash, "/", tokenLine, tokenColumn)); break;
            case '(': tokens.push_back(makeToken(TokenType::LParen, "(", tokenLine, tokenColumn)); break;
            case ')': tokens.push_back(makeToken(TokenType::RParen, ")", tokenLine, tokenColumn)); break;
            case ':': tokens.push_back(makeToken(TokenType::Colon, ":", tokenLine, tokenColumn)); break;
            case ',': tokens.push_back(makeToken(TokenType::Comma, ",", tokenLine, tokenColumn)); break;
            case '=':
                if (match('=')) {
                    tokens.push_back(makeToken(TokenType::EqualEqual, "==", tokenLine, tokenColumn));
                } else {
                    tokens.push_back(makeToken(TokenType::Equal, "=", tokenLine, tokenColumn));
                }
                break;
            case '!':
                if (match('=')) {
                    tokens.push_back(makeToken(TokenType::BangEqual, "!=", tokenLine, tokenColumn));
                    break;
                }
                throw std::runtime_error("Unexpected '!' at line " + std::to_string(tokenLine));
            case '>':
                if (match('=')) {
                    tokens.push_back(makeToken(TokenType::GreaterEqual, ">=", tokenLine, tokenColumn));
                } else {
                    tokens.push_back(makeToken(TokenType::Greater, ">", tokenLine, tokenColumn));
                }
                break;
            case '<':
                if (match('=')) {
                    tokens.push_back(makeToken(TokenType::LessEqual, "<=", tokenLine, tokenColumn));
                } else {
                    tokens.push_back(makeToken(TokenType::Less, "<", tokenLine, tokenColumn));
                }
                break;
            default:
                throw std::runtime_error("Line " + std::to_string(tokenLine) + ": Unexpected character '" + std::string(1, source[current - 1]) + "'");
        }
    }

    emitDedents(tokens, line, column);
    tokens.push_back(makeToken(TokenType::EndOfFile, "", line, column));
    return tokens;
}

} // namespace minicompiler
