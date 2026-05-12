#pragma once

#include "token.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace minicompiler {

struct Expr {
    virtual ~Expr() = default;
};

using ExprPtr = std::unique_ptr<Expr>;

struct NumberExpr final : Expr {
    explicit NumberExpr(long long value) : value(value) {}
    long long value;
};

struct VariableExpr final : Expr {
    explicit VariableExpr(std::string name) : name(std::move(name)) {}
    std::string name;
};

struct BinaryExpr final : Expr {
    BinaryExpr(ExprPtr left, TokenType op, ExprPtr right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    ExprPtr left;
    TokenType op;
    ExprPtr right;
};

struct Stmt {
    virtual ~Stmt() = default;
};

using StmtPtr = std::unique_ptr<Stmt>;

struct AssignStmt final : Stmt {
    AssignStmt(std::string name, ExprPtr value)
        : name(std::move(name)), value(std::move(value)) {}

    std::string name;
    ExprPtr value;
};

struct PrintStmt final : Stmt {
    explicit PrintStmt(ExprPtr value) : value(std::move(value)) {}
    ExprPtr value;
};

struct IfStmt final : Stmt {
    IfStmt(ExprPtr condition, std::vector<StmtPtr> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    ExprPtr condition;
    std::vector<StmtPtr> body;
};

} // namespace minicompiler
