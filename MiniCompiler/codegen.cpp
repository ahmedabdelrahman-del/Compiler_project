#include "codegen.h"

#include <stdexcept>
#include <type_traits>

namespace minicompiler {

std::vector<std::string> CodeGenerator::generate(const std::vector<StmtPtr>& statements) {
    instructions.clear();
    labelCounter = 0;
    constants.clear();

    for (const auto& statement : statements) {
        emitStatement(*statement);
    }

    return instructions;
}

void CodeGenerator::emitLine(const std::string& line) {
    instructions.push_back(line);
}

std::string CodeGenerator::newLabel() {
    return "L" + std::to_string(++labelCounter);
}

std::string CodeGenerator::mapBinaryOperator(TokenType type) const {
    switch (type) {
        case TokenType::Plus: return "ADD";
        case TokenType::Minus: return "SUB";
        case TokenType::Star: return "MUL";
        case TokenType::Slash: return "DIV";
        case TokenType::Greater: return "GT";
        case TokenType::GreaterEqual: return "GE";
        case TokenType::Less: return "LT";
        case TokenType::LessEqual: return "LE";
        case TokenType::EqualEqual: return "EQ";
        case TokenType::BangEqual: return "NE";
        default:
            throw std::runtime_error("Unsupported binary operator");
    }
}

void CodeGenerator::emitExpression(const Expr& expr) {
    if (const auto* number = dynamic_cast<const NumberExpr*>(&expr)) {
        emitLine("PUSH " + std::to_string(number->value));
        return;
    }
    if (const auto* variable = dynamic_cast<const VariableExpr*>(&expr)) {
        emitLine("LOAD " + variable->name);
        return;
    }
    if (const auto* binary = dynamic_cast<const BinaryExpr*>(&expr)) {
        emitExpression(*binary->left);
        emitExpression(*binary->right);
        emitLine(mapBinaryOperator(binary->op));
        return;
    }
    throw std::runtime_error("Unsupported expression node");
}

void CodeGenerator::emitStatement(const Stmt& statement) {
    if (const auto* assign = dynamic_cast<const AssignStmt*>(&statement)) {
        emitExpression(*assign->value);
        emitLine("STORE " + assign->name);
        return;
    }
    if (const auto* printStmt = dynamic_cast<const PrintStmt*>(&statement)) {
        emitExpression(*printStmt->value);
        emitLine("PRINT");
        return;
    }
    if (const auto* ifStmt = dynamic_cast<const IfStmt*>(&statement)) {
        std::string endLabel = newLabel();
        emitExpression(*ifStmt->condition);
        emitLine("JMP_IF_FALSE " + endLabel);
        for (const auto& bodyStatement : ifStmt->body) {
            emitStatement(*bodyStatement);
        }
        emitLine(endLabel + ":");
        return;
    }
    throw std::runtime_error("Unsupported statement node");
}

} // namespace minicompiler
