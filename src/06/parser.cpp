#include <cctype>
#include "./parser.hpp"
#include "./c_command.hpp"

using namespace nand2tetris::assembler;
using namespace std;

Parser::~Parser() {
    in_.close();
}

void Parser::SkipWhiteSpace() {
    while (isspace(current_line_[pos_])) {
        pos_++;
    }
}

string Parser::ExtractSymbol() {
    string sym;
    char c;

    while(pos_ < current_line_.size()) {
        c = current_line_[pos_];
        if (!isalnum(c) && c != '_' && c != '.' && c !='$' && c !=':') {
            break;
        }
        sym += c;
        pos_++;
    }
    return sym;
}

void Parser::HandleComment() {
    pos_++;

    if(current_line_[pos_] != '/') {
        string message_ = "unexpected: ";
        message_ += current_line_[pos_];
        throw SyntaxErrorException(line_num_, pos_, current_line_, message_);
    }

    pos_ = current_line_.size();
}

void Parser::HandleLCommand() {
    // (LOOP)とか
    pos_++;
    SkipWhiteSpace();

    string symbol = ExtractSymbol();
    if (current_line_[pos_] != ')') {
        throw SyntaxErrorException(line_num_, pos_, current_line_, string(") expected"));
    }
    pos_++;
    SkipWhiteSpace();

    if (pos_ != current_line_.size()) {
        throw SyntaxErrorException(line_num_, pos_, current_line_, string("unexpected "));
    }

    symbol_ = symbol;
    command_type_ = L_COMMAND;
    pos_ = current_line_.size();

}

void Parser::HandleACommand() {
    // @SYMBOL
    pos_++;
    SkipWhiteSpace();

    string sym;
    if(isdigit(current_line_[pos_])) {
        while(isdigit(current_line_[pos_])) {
            sym += current_line_[pos_];
        }
    }
    sym = ExtractSymbol();

    SkipWhiteSpace();

    if (pos_ != current_line_.size()) {
        string message = "unexpected" + current_line_[pos_];
        throw SyntaxErrorException(line_num_, pos_, current_line_, message);
    }

    pos_ = current_line_.size();
    symbol_ = sym;
    command_type_ = A_COMMAND;
}

void Parser::HandleCCommand() {
    // dest=comp;jump
    SkipWhiteSpace();
    int state = 0;
    char c;
    string s;

    int pos_dest = pos_;
    int pos_comp = pos_;
    int pos_jump = pos_;

    while (state < 3) {
        if (pos_ >= current_line_.size()) {
            c = 0;
        } else {
            c = current_line_[pos_];
        }

        if (isspace(c)) {
            pos_++;
            continue;
        }

        if (state == 0) {
            switch (c) {
                case '=':
                    dest_ = s;
                    s = "";
                    pos_comp = pos_;
                    pos_jump = pos_;
                    state = 1;
                    pos_++;
                    continue;
                case ';':
                case '/':
                case 0:
                    dest_ = "null";
                    comp_ = s;
                    s = "";
                    pos_jump = pos_;
                    state = 2;
                    pos_++;
                    continue;
            }
            
        }

        if (state == 1) {
            switch(c) {
                case ';':
                case '/':
                case 0:
                    comp_ = s;
                    s = "";
                    pos_jump = pos_;
                    state = 2;
                    pos_++;
                    continue;
            }    
        }

        if (state == 2) {
            switch (c) {
                case '/':
                case 0:
                    if (s == "") {
                        jump_ = "null";
                    } else {
                        jump_ = s;
                    }
                    state = 3;
            }
        }

        s += c;
        pos_++;
    }
    command_type_ = C_COMMAND;
    pos_ = current_line_.size();

}

bool Parser::HasMoreCommands() {
    if (!in_) return false;

    SkipWhiteSpace();
    if (pos_ == current_line_.size()) {
        getline(in_, current_line_);
    } else {
        return true;
    }

    // newline
    pos_ = 0;
    SkipWhiteSpace();
    if (pos_ == current_line_.size()) {
        return HasMoreCommands();
    }

    if (current_line_[pos_] == '/') {
        HandleComment();
        return HasMoreCommands();
    }

    return true;
}

void Parser::Advance() {
    if (!HasMoreCommands()) {
        throw IllegalCallException("error");
    }

    switch(current_line_[pos_]) {
        case '(':
            HandleLCommand();
            break;
        case '@':
            HandleACommand();
            break;
        default:
            HandleCCommand();
            break;
    }

    line_num_++;

}

CommandType Parser::CommandType() { return command_type_; }
string Parser::Symbol() { return symbol_; }
string Parser::Dest() { return dest_; }
string Parser::Comp() { return comp_; }
string Parser::Jump() { return jump_; }
