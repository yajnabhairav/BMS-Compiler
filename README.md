# BMS Compiler

Welcome to my **BMS Compiler** project!  This project is designed to convert high-level code into low-level assembly code, showcasing various compiler components.

## 📁 Project Structure
Here's a quick breakdown of the files included in this repository:

- **Lexer**: Handles lexical analysis, breaking code into tokens.
- **Parser**: Analyzes syntax and builds a parse tree.
- **Token**: Defines token structures used in lexical analysis.
- **CodeGenerator**: Transforms parsed data into assembly code.
- **Assembly**: Contains assembly-related processing.
- **Error**: Manages error handling for lexical and syntax errors.
- **Main**: The entry point to compile input code.
- **LICENSE**: MIT License for open-source distribution.

## ⚙️ How to Use
1. Clone this repository:
   ```sh
   git clone https://github.com/yajnabhairav/BMS-Compiler.git
   ```
2. Navigate to the project folder:
   ```sh
   cd BMS-Compiler
   ```
3. Compile the code:
   ```sh
   gcc -o compiler main.c lexer.c parser.c codegenerator.c -Wall
   ```
4. Run the compiler with an input file:
   ```sh
   ./compiler input.bms
   ```

## ✨ Features
- Tokenization and Lexical Analysis
- Syntax Parsing
- Error Handling
- Code Generation

## 📌 Future Improvements
- Semantic Analysis
- Optimization Techniques
- Support for More Programming Constructs

## 🤝 Contributing
Feel free to fork, modify, and contribute! Open an issue if you find bugs or have suggestions.

## 📜 License
This project is licensed under the **MIT License**.

---
🚀 **Happy Coding!**

