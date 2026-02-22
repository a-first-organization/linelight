#include "headers/blender.hpp"

// the lexer makes tokens from 0-255, incase it's an unknown character,
// otherwise, one of the tokens returns 1 of these known bitches. this makes
// sense for me, i'm a rookie lol
enum Token {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary shit
  tok_identifier = -4,
  tok_number = -5
};

static std::string IdentifierStr; // fills in identifier thing
static double NumVal;             // fills in the shit about the numbers

// gettok: gets the token. in LLVM's words, it returns the next token from the
// standard input.
static int gettok() {
  static int LastChar = ' ';

  // remove any whitespace.
  while (isspace(LastChar))
    LastChar = getchar();

  // the identifier
  if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (isalnum(LastChar = getchar()))
      IdentifierStr += LastChar;

    if (IdentifierStr == "def")
      return tok_def;
    if (IdentifierStr == "extern")
      return tok_extern;
    return tok_identifier;

    if (isdigit(LastChar) || LastChar == '.') {
      std::string NumStr;
      do {
        NumStr += LastChar;
        LastChar = getchar();
      } while (isdigit(LastChar) || LastChar == '.');

      NumVal = strtod(NumStr.c_str(), 0);
      return tok_number;
    }

    if (LastChar == "#") {
      // comment until it reaches EOL.
      do
        LastChar = getchar();
      while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

      if (LastChar != EOF)
        return gettok();
    }
    // "Do not eat the EOF.", a wise man said. i mean he is right i'm just
    // here doing my own thing
    if (LastChar == EOF) {
      return tok_eof;

      // otherwise, just return the ascii value of the character, aka the DAMN
      // VALUE OF IT. HOW DO ROOKIE CODERS MESS THIS UP? I'M A ROOKIE CODER
      // MYSELF. I UNDERSTAND THIS.
      int ThisChar = LastChar;
      LastChar = getchar();
      return ThisChar;
    }
  }
}