#pragma once

#include "ast.h"
#include "token.h"

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

namespace minicompiler {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    std::vector<StmtPtr> parseProgram();

private:
    const Token& peek(std::size_t offset = 0) const;
    bool isAtEnd() const;
    const Token& advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    const Token& consume(TokenType type, const std::string& message);
    void skipNewlines();

    StmtPtr parseStatement();
    StmtPtr parseAssignment();
    StmtPtr parsePrint();
    StmtPtr parseIf();
    std::vector<StmtPtr> parseBlock();

    ExprPtr parseExpression();
    ExprPtr parseComparison();
    ExprPtr parseTerm();
    ExprPtr parseFactor();
    ExprPtr parseUnary();
    ExprPtr parsePrimary();

    void markVariableDefined(const std::string& name);
    bool isVariableDefined(const std::string& name) const;

    std::vector<Token> tokens;
    std::size_t current;
    std::unordered_set<std::string> definedVariables;
};

} // namespace minicompiler
