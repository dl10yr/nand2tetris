
#ifndef PARSER
#define PARSER 1

#include <fstream>
#include <string>
#include <exception>
#include "./error.hpp"
#include "./c_command.hpp"

using namespace std;

namespace nand2tetris {
namespace assembler {

enum CommandType {
    A_COMMAND, C_COMMAND, L_COMMAND
};

class Parser {
    private:
        string current_line_;
        ifstream& in_;
        CommandType command_type_;
        string symbol_;
        string dest_;
        string comp_;
        string jump_;

        int pos_;
        int line_num_;

        DestTable& dest_table;
        CompTable& comp_table;
        JumpTable& jump_table;

        void SkipWhiteSpace();
        void HandleComment();
        void HandleLCommand();
        void HandleACommand();
        void HandleCCommand();
        string ExtractSymbol();


    public:
        Parser(ifstream& f, DestTable& dt, CompTable& ct, JumpTable& jt)
            :in_(f), dest_table(dt), comp_table(ct), jump_table(jt), pos_(0), line_num_(0) {}
        ~Parser(); 
        bool HasMoreCommands();
        void Advance();
        CommandType CommandType();
        string Symbol();
        string Dest();
        string Comp();
        string Jump();

        string GetCurrentLine() { return current_line_; }
};

}
}


#endif