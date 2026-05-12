# Compiler_project

Mini Conditional Language Compiler in C++.

## Layout

The compiler lives in [MiniCompiler/](MiniCompiler) and is split into the following files:

- [main.cpp](MiniCompiler/main.cpp)
- [token.h](MiniCompiler/token.h)
- [lexer.h](MiniCompiler/lexer.h)
- [lexer.cpp](MiniCompiler/lexer.cpp)
- [ast.h](MiniCompiler/ast.h)
- [parser.h](MiniCompiler/parser.h)
- [parser.cpp](MiniCompiler/parser.cpp)
- [codegen.h](MiniCompiler/codegen.h)
- [codegen.cpp](MiniCompiler/codegen.cpp)
- [test_input.txt](MiniCompiler/test_input.txt)
- [error_input.txt](MiniCompiler/error_input.txt)
- [missing_expression.txt](MiniCompiler/missing_expression.txt)
- [invalid_operator.txt](MiniCompiler/invalid_operator.txt)
- [missing_paren.txt](MiniCompiler/missing_paren.txt)
- [invalid_token.txt](MiniCompiler/invalid_token.txt)
- [bad_if_condition.txt](MiniCompiler/bad_if_condition.txt)
- [missing_colon.txt](MiniCompiler/missing_colon.txt)
- [undefined_variable.txt](MiniCompiler/undefined_variable.txt)

## Build

From the repository root:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic MiniCompiler/main.cpp MiniCompiler/lexer.cpp MiniCompiler/parser.cpp MiniCompiler/codegen.cpp -o MiniCompiler/minicompiler
```

## Run

```bash
./MiniCompiler/minicompiler MiniCompiler/test_input.txt
```

Or run the helper script from the repository root:

```bash
bash run_minicompiler.sh
```

If your terminal is already inside `MiniCompiler/`, run:

```bash
bash run_minicompiler.sh
```

To test the rubric error case, run the compiler on [MiniCompiler/error_input.txt](MiniCompiler/error_input.txt). The expected message is:

```text
Line 2: Expected number, identifier, or '(' after '*'
```

Additional fixtures:

- [MiniCompiler/missing_expression.txt](MiniCompiler/missing_expression.txt) -> `Line 1: Expected expression after '='`
- [MiniCompiler/invalid_operator.txt](MiniCompiler/invalid_operator.txt) -> `Line 1: Unexpected token '*'`
- [MiniCompiler/missing_paren.txt](MiniCompiler/missing_paren.txt) -> `Line 1: Expected ')'`
- [MiniCompiler/invalid_token.txt](MiniCompiler/invalid_token.txt) -> `Line 1: Unexpected character '$'`
- [MiniCompiler/bad_if_condition.txt](MiniCompiler/bad_if_condition.txt) -> `Line 1: Expected expression after '>'`
- [MiniCompiler/missing_colon.txt](MiniCompiler/missing_colon.txt) -> `Line 1: Expected ':' after condition`
- [MiniCompiler/undefined_variable.txt](MiniCompiler/undefined_variable.txt) -> `Line 1: Undefined variable 'z'`

The sample program lowers to stack-machine style instructions such as `PUSH`, `LOAD`, `ADD`, `STORE`, `JMP_IF_FALSE`, and `PRINT`.

## Presentation (Reveal.js)

A Reveal.js HTML wrapper is included at `reveal.html` and reads the Markdown slides from `slides.md`.

To serve and view the presentation locally run (from the repository root):

```bash
python3 -m http.server 8000
# then open http://localhost:8000/reveal.html in your browser
```

Or open `reveal.html` directly in a browser that allows loading local files (some browsers restrict the Markdown plugin when opened via the `file://` protocol).

## Next.js Presentation

For a formal, professional presentation with speaker notes we added a Next.js app under `presentation-next/`.

Run from the repository root:

```bash
cd presentation-next
npm install
npm run dev
```

The app serves on http://localhost:3000 by default and shows speaker notes alongside each slide for easy presentation.

