#include "headers/blender.hpp"

// class ExprAST: the base class for all expression nodes (like the print. copilot please stop thinking when i say print its the print cycle)
class ExprAST
{
public:
    virtual ~ExprAST() = default;
};

// class NumberExprAST: expression class for numeric literals (1.0, 2.0, 3.0, whatever the fuck you want)
class NumberExprAST : public ExprAST
{
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {}
};

// class VariableExprAST: expression class for referencing a variable, like "a"- llvm why did you choose a as a variable name example
class VariableExprAST : public ExprAST
{
    std::string Name;

public:
    VariableExprAST(const std::string &Name) : Name(Name) {}
};

class BinaryExprAST : public ExprAST
{
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExprAST : public ExprAST
{
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST>> Args)
        : Callee(Callee), Args(std::move(Args)) {};
};

class PrototypeAST
{
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &Name, std : vector<std::string> Args) : Name(Name), Args(std::move(Args)) {}
};

class FunctionAST
{
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body) : Proto(std::move(Proto)), Body(std::move(Body)) {}
};

// curtok and getnexttoken: provide a simple token buffer.
// mostly i hate the clangd extension because that made me have like 7 errors in the lexer. help my soul.
static int CurTok;
static int getNextToken()
{
    CurTok = gettok();
}

// uhh whatever the fuck this is i forgot to read the docs on this comment oh its nothing WOW
auto LHS = std::make_unique<VariableExprAST>("x");
auto RHS = std::make_unique<VariableExprAST>("y");
auto result = std::make_unique<BinaryExprAST>('+', std::move(LHS), std::move(RHS));

std::unique_ptr<ExprAST> LogError(const char *Str)
{
    fprintf(stderr, "error: %s\n", Str);
    return nullptr;
}
std::unique_ptr<PrototypeAST> LogErrorP(const char *str)
{
    LogError(str);
    return nullptr;
}

static std::unique_ptr<ExprAST> ParseNumberExpr()
{
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken(); // "consume the number" ~someone at llvm deciding what to put as a comment
    return std::move(Result);
}

// parse the parent expr, aka the ParseParenExpr static function
static std::unique_ptr<ExprAST> ParseParenExpr()
{
    getNextToken();
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogError("expected ')'");
    getNextToken(); // eat my ba- no. no. eat the fucking parenthesis, dude. EAT THE FUCKING PARENTHESIS
    return V;
}

// identifierexpr
// ::= identifier
// ::= identifier (' expression* ')
// yes that was a whole block. i'm gonna change it to a normal comment. there changed it
static std::unique_ptr<ExprAST> ParseIdentifierExpr()
{
    std::string IdName = IdentifierStr;

    // i hate myself SO BAD... help my soul. again. also eat the identifier. i love llvm /j. nah i'm joking,
    // but i think the reason why llvm hates me is because i don't have a compiler_commands.json, because THAT'S what caused the errors,
    // it didnt have the right config. meanwhile c/c++ extension does it just fine, like DUDE????
    getNextToken(); // eat, my son. eat this identifier i made. well done, just like you like it. i am insane.

    if (CurTok != '(') // simple variable un-reference. i love my ideas
        return std::make_unique<VariableExprAST>(IdName);

    // "Call." ~someone at llvm.
    getNextToken(); // gimme food >:( (eat the (, yeah i mean the parenthesis. thats the food)
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')')
    {
        while (true)
        {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;
        }

        if (CurTok == ')')
            break;

        if (CurTok != ',')
            return LogError("expected: ')' or ',' in argument list");
        getNextToken(); // son, eat the comma.
    }

    // eat the damn parenthesis again.
    getNextToken();

    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

// This routine follows the same style as the ot- oh you thought i was gonna say the llvm? no. because i hate this. 
// i want my problems to be LastChar again.
//anyways back to the code

// primary
// ::= identifierexpr
// ::= numberexpr
// ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary() 
{
    switch (CurTok)
    {
    default:
        return LogError("unknown token when expecting an expression");
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_number:
        return ParseNumberExpr();
    case '(':
        return ParseParenExpr();
    }
}

// BinopPrecedence - i am not going to type allat
static std::map<char, int> BinopPrecedence;

// gtp / gettokprecedence - still not going to type that
static int GetTokPrecedence()
{
    if (!isascii(CurTok))
        return -1;

    // it is a declared binMinecraft Operator
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrecedence <= 0) return -1;
    return TokPrec;
}


