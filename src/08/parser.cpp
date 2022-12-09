#include "parser.hpp"
#include <fstream>
#include <string>
#include <cctype>

using namespace nand2tetris::vm_translator2;
using namespace std;

Parser::Parser(string filename) {
  filename_ = filename;
  string f = filename + ".vm";
  in_.open(filename.c_str());
  if (!in_.is_open()) {
    cerr << "Error: could not open\n";
  }
}

Parser::~Parser() {
  in_.close();
}

void Parser::SkipWhiteSpace() {
  while (isspace(current_line_[pos_])) {
    pos_++;
  }
}

void Parser::HasMoreCommands() {
  return !in_.eof();
}

void Parser::Advance() {
  if (HasMoreCommands()) {
    getline(in_, current_command_);
    while (((current_command_.substr(0, 2) == "//") || (current_command_.substr(0, 1) == ""))
      && HasMoreCommands()) {
        getline(in_, current_command_);
    }
  }
}

VMCommandType Parser::CommandType() {
  if ((current_command_.substr(0, 3) == "add")
    || (current_command_.substr(0, 3) == "sub")
    || (current_command_.substr(0, 3) == "neg")
    || (current_command_.substr(0, 3) == "and")
    || (current_command_.substr(0, 3) == "not")
    || (current_command_.substr(0, 2) == "eq")
    || (current_command_.substr(0, 2) == "gt")
    || (current_command_.substr(0, 2) == "lt")
    || (current_command_.substr(0, 2) == "or")
  ) {
    return C_ARITHMETIC;
  } else if (current_command_.substr(0, 4) == "push") {
    return C_PUSH;
  } else if (current_command_.substr(0, 3) == "pop") {
    return C_POP;
  } else if (current_command_.substr(0, 5) == "label") {
    return C_LABEL;
  } else if (current_command_.substr(0, 4) == "goto") {
    return C_GOTO;
  } else if (current_command_.substr(0, 2) == "if") {
    return C_IF;
  } else if (current_command_.substr(0, 8) == "function") {
    return C_FUNCTION;
  } else if (current_command_.substr(0, 6) == "return") {
    return C_RETURN;
  } else if (current_command_.substr(0, 4) == "call") {
    return C_CALL;
  }
}

string Parser::Arg1() {
  VMCommandType ct = CommandType();
  if (ct != C_RETURN) {
    int first_space = current_command_.find(" ", 0);
    int second_space = current_command_.find(" ", first_space);
  
    string a1 = current_command_.substr(first_space + 1, second_space - first_space - 1);
    return a1;
  }
}

string Parser::Arg2() {
  VMCommandType ct = CommandType();
  if (ct == C_PUSH || ct == C_POP || ct == C_FUNCTION || ct == C_CALL) {
    int first_space = current_command_.find(" ", 0);
    int second_space = current_command_.find(" ", first_space + 1);
    int third_space = current_command_.find(" ", second_space + 1);
    string a2 = current_command_.substr(second_space + 1, third_space - second_space - 1);
    int a2i = stoi(a2);
    return a2i;
  }
}

