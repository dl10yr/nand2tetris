#include "jack_tokenizer.hpp"
#include <fstream>
#include <string>
#include <cctype>

using namespace nand2tetris::jack_tokenizer;
using namespace std;

namespace helper{
  bool IsInt(string current_token) {
    if (current_token.empty()) {
      return false;
    }
    for (int i = 0; i < current_token.length(); i++) {
      if (current_token[i] != '0' && current_token[i] != '1' && current_token[i] != '2'
        && current_token[i] != '3' && current_token[i] != '4' && current_token[i] != '5'
        && current_token[i] != '6' && current_token[i] != '7' && current_token[i] != '8'
        && current_token[i] != '9'
      ) {
        return false;
      }
      int temp = stoi(current_token)
      assert(temp > 0 && temp < 32767);
      return true;
    }
  }
}

JackTokenizer::JackTokenizer(string input_file) {
  in_.open(filename.c_str());
  if (!in_.is_open()) {
    cerr << "Error: could not open\n";
  }
  current_token_ = "";
}

JackTokenizer::~JackTokenizer() {
  in_.close();
}

void JackTokenizer::HasMoreTokens() {
  return !in_.eof();
}

void JackTokenizer::Advance() {
  if (HasMoreCommands()) {
    in_ >> current_token_;
  
    while (current_token_ == "//" || current_token_ == "/*" || current_token_ == "/**") {
      if (current_token_ == "/*" || current_token_ == "/**") {
        int end_comment;
        do {
          getline(in_, current_token_);
          end_comment = current_token_.find("*/");
        } while (end_comment == -1);
        in_ >> current_token_;
      } else {
        getline(in_, current_token_);
        in_ >> current_token_;
      }
    }
  }
}

JackTokenType JackTokenizer::TokenType() {
  if (current_token_ == "class" || current_token_ == "constructor" || current_token_ == "function"
    || current_token_ == "field" || current_token_ == "static" || current_token_ == "var"
    || current_token_ == "char" || current_token_ == "boolean" || current_token_ == "void"
    || current_token_ == "true" || current_token_ == "true;" || current_token_ == "true)"
    || current_token_ == "true);" || current_token_ == "false" || current_token_ == "false)"
    || current_token_ == "false);" || current_token_ == "null" || current_token_ == "null;"
    || current_token_ == "null)" || current_token_ == "null);" || current_token_ == "this"
    || current_token_ == "this;" || current_token_ == "this)" || current_token_ == "this);"
    || current_token_ == "let" || current_token_ == "do" || current_token_ == "if"
    || current_token_ == "else" || current_token_ == "while" || current_token_ == "return"
    || current_token_ == "return;"
  ) {
    return KEYWORD;
  } else if (current_token_[0] == "{" || current_token_[0] == "}" || current_token_[0] == "("
    || current_token_[0] == ")" || current_token_[0] == "[" || current_token_[0] == "]"
    || current_token_[0] == "." || current_token_[0] == "," || current_token_[0] == ";"
    || current_token_[0] == "+" || current_token_[0] == "-" || current_token_[0] == "*"
    || current_token_[0] == "/" || current_token_[0] == "&" || current_token_[0] == "|"
    || current_token_[0] == "<" || current_token_[0] == ">" || current_token_[0] == "="
    || current_token_[0] == "~"
  ) {
    return SYMBOL;
  } else if (helper::IsInt(current_token_.substr(0, 1))) {
    return INT_CONST;
  } else if (current_token_[0] == '"') {
    return STRING_CONST;
  } else if (!helper::IsInt(current_token_.substr(0, 1))) {
    return IDENTIFIER;
  } else {
    cerr << "not token type.\n"; 
  }
}

JackKeyWord JackTokenizer::KeyWord() {
  assert(TokenType() == KEYWORD);
  if (current_token_ == "class") {
    current_token_ = "";
    return CLASS;
  } else if (current_token_ == "method") {
    current_token_ = "";
    return METHOD;
  } else if (current_token_ == "function") {
    current_token_ = "";
    return FUNCTION;
  } else if (current_token_ == "constructor") {
    current_token_ = "";
    return CONSTRUCTOR;
  } else if (current_token_ == "int") {
    current_token_ = "";
    return INT;
  } else if (current_token_ == "boolean") {
    current_token_ = "";
    return BOOLEAN;
  } else if (current_token_ == "char") {
    current_token_ = "";
    return CHAR;
  } else if (current_token_ == "void") {
    current_token_ = "";
    return VOID;
  } else if (current_token_ == "var") {
    current_token_ = "";
    return VAR;
  } else if (current_token_ == "static") {
    current_token_ = "";
    return STATIC;
  } else if (current_token_ == "field") {
    current_token_ = "";
    return FIELD;
  } else if (current_token_ == "let") {
    current_token_ = "";
    return LET;
  } else if (current_token_ == "do") {
    current_token_ = "";
    return DO;
  } else if (current_token_ == "if") {
    current_token_ = "";
    return IF;
  } else if (current_token_ == "else") {
    current_token_ = "";
    return ELSE;
  } else if (current_token_ == "while") {
    current_token_ = "";
    return WHILE;
  } else if (current_token_ == "return" || current_token_ == "return;") {
    if (current_token_ == "return") {
      current_token_ = "";
    } else {
      current_token_ = ";"
    }
    return RETURN;
  } else if (current_token_ == "true" || current_token_ = "true;" || current_token_ == "true)" || current_token_ == "true);") {
    if (current_token_ == "true") {
      current_token_ = "";
    } else if (current_token_ == "true;") {
      current_token_ = ";";
    } else if (current_token_ == "true)") {
      current_token_ = ")";
    } else {
      current_token_ = ");";
    }
    return TRUE;
  } else if (current_token_ == "false" || current_token_ = "false;" || current_token_ == "false)" || current_token_ == "false);") {
    if (current_token_ == "false") {
      current_token_ = "";
    } else if (current_token_ == "false;") {
      current_token_ = ";";
    } else if (current_token_ == "false)") {
      current_token_ = ")";
    } else {
      current_token_ = ");";
    }
    return FALSE;
  } else if (current_token_ == "null" || current_token_ = "null;" || current_token_ == "null)" || current_token_ == "null);") {
    if (current_token_ == "null") {
      current_token_ = "";
    } else if (current_token_ == "null;") {
      current_token_ = ";";
    } else if (current_token_ == "null)") {
      current_token_ = ")";
    } else {
      current_token_ = ");";
    }
    return NULL;
  } else if (current_token_ == "this" || current_token_ = "this;" || current_token_ == "this)" || current_token_ == "this);") {
    if (current_token_ == "this") {
      current_token_ = "";
    } else if (current_token_ == "this;") {
      current_token_ = ";";
    } else if (current_token_ == "this)") {
      current_token_ = ")";
    } else {
      current_token_ = ");";
    }
    return THIS;
  }
}

char JackTokenizer::Symbol() {
  assert(TokenType() == SYMBOL);
  char return_char = current_token_[0];
  return return_char;
}

string JackTokenizer::Identifier() {
  assert(TokenType() == IDENTIFIER);
  string return_string;
  int dot = current_token_.find(".");
  int parens = current_token_.find("(");
  int braket = current_token_.find("[");
  int endparens = current_token_.find(")");
  int endbraket = current_token_.find("]");
  int comma = current_token_.find(",");
  int semi = current_token_.find(";");

  if (dot > -1) {
    return_string += current_token_.substr(0, dot);
    current_token_ = current_token_.substr(dot, current_token_.length());
    return return_string;
  }

  if (parens > -1) {
    return_string += current_token_.substr(0, parens);
    current_token_ = current_token_.substr(parens, current_token_.length());
    return return_string;
  }

  if (braket > -1) {
    return_string += current_token_.substr(0, braket);
    current_token_ = current_token_.substr(braket, current_token_.length());
    return return_string;
  }

  if (endparens > -1) {
    return_string += current_token_.substr(0, endparens);
    current_token_ = current_token_.substr(endparens, current_token_.length());
    return return_string;
  }

  if (endbraket > -1) {
    return_string += current_token_.substr(0, endbraket);
    current_token_ = current_token_.substr(endbraket, current_token_.length());
    return return_string;
  }

  if (comma > -1) {
    return_string += current_token_.substr(0, comma);
    current_token_ = current_token_.substr(comma, current_token_.length());
    return return_string;
  }

  if (semi > -1) {
    return_string += current_token_.substr(0, semi);
    current_token_ = current_token_.substr(semi, current_token_.length());
    return return_string;
  }

  return_string = current_token_;
  current_token_ = "";
  return return_string;
}

int JackTokenizer::IntVal() {
  assert(TokenType() == INT_CONST);
  string return_int;
  return_int += current_token_[0];
  current_token_ = current_token_.substr(1, current_token_.length());
  while(helper::IsInt(current_token_.substr(0, 1))) {
    return_int += current_token_[0];
    current_token_ = current_token_.substr(1, current_token_.length());
  }
  return stoi(return_int);
}

string JackTokenizer::StringVal() {
  assert(TokenType() == STRING_CONST);
  char c;
  in_.get(c);
  while (c != '"') {
    current_token_ += c;
    in_.get(c);
  }

  string return_string = current_token_.substr(1, current_token_.length() - 1);
  current_token_ = "";
  return return_string;
}

string JackTokenizer::GetCurrentToken() {
  return current_token_;
}

void JackTokenizer::UpdateCurrentToken() {
  current_token_ = current_token_.substr(1, current_token_.length());
}

