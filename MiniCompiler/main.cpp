#include "codegen.h"
#include "lexer.h"
#include "parser.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>

using namespace minicompiler;

static std::string readFile(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Unable to open input file: " + path);
    }
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

int main(int argc, char** argv) {
    try {
        std::string inputPath = argc > 1 ? argv[1] : "test_input.txt";
        std::string source = readFile(inputPath);

        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();

        Parser parser(std::move(tokens));
        std::vector<StmtPtr> program = parser.parseProgram();

        CodeGenerator generator;
        std::vector<std::string> instructions = generator.generate(program);

        for (const auto& instruction : instructions) {
            std::cout << instruction << '\n';
        }

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
}
