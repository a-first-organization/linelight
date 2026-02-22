#include "headers/blender.hpp"

int main()  {   
    // class ExprAST: the base class for all expression nodes (like the print. copilot please stop thinking when i say print its the print cycle)
    class ExprAST {
        public:
            virtual ~ExprAST() = default;
    };

    // class NumberExprAST: expression class for numeric literals (1.0, 2.0, 3.0, whatever the fuck you want)
    class NumberExprAST : public ExprAST {
        double Val;

    public:
        NumberExprAST(double Val) : Val(Val) {}
    };

    // class VariableExprAST: expression class for referencing a variable, like "a"- llvm why did you choose a as a variable name example
    class VariableExprAST : public ExprAST {
        std::string Name;

    public:
        VariableExprAST(const std::string& Name) : Name(Name) {}
    };
}