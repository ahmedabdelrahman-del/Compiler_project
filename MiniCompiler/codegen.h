#pragma once

#include "ast.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace minicompiler {

class CodeGenerator {
public:
    std::vector<std::string> generate(const std::vector<StmtPtr>& statements);

private:
    void emitStatement(const Stmt& statement);
    void emitExpression(const Expr& expr);
    std::string newLabel();
    void emitLine(const std::string& line);
    std::string mapBinaryOperator(TokenType type) const;

    std::vector<std::string> instructions;
    std::size_t labelCounter;
    std::unordered_map<std::string, long long> constants;
};

} // namespace minicompiler
