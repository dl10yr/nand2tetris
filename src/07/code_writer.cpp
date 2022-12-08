#include "parser.hpp"
#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

using namespace nand2tetris::code_writer;
using namespace std;

CodeWriter::CodeWriter() {
  labelnum_ = 0;
  start_ = 0;
}

CodeWriter::~CodeWriter() {
  out_.close();
}

void CodeWriter::Close() {
  out_.close();
}

void CodeWriter::SetFileName(string filename) {
  filename_ = filename;
  string f = filename + ".asm";
  out_.open(f.c_str());
  if (!out_.is_open()) {
    cerr << "could not open output file" << filename_ << "\n";
  }
  Init();
}

void CodeWriter::PopD() {
  out_ << "@SP" << endl // SP: 最上位は何も入ってない
    << "AM=M-1" << endl // 最後の要素はSPの1個前
    << "D=M" << endl; // DレジスタにM入れる
}

void CodeWriter::DecSP() {
  out_ << "@SP" << endl
    << "AM=M-1" << endl;
}

void CodeWriter::IncSP() {
  out_ << "@SP" << endl
    << "AM=M+1" << endl;
}

void CodeWriter::SetReturn() {
  out_ << "@RETURN" << labelnum_ << endl
    << "D=A" << endl
    << "@R15" << endl
    << "M=D" << endl;
}

void CodeWriter::GetReturn() {
  out_ << "@R15" << endl
    << "A=M" << endl
    << "0;JMP" << endl;
}

void CodeWriter::init() {
  out_ << "@START" << start_ << endl
    << "0;JMP" << endl
    << "(MAKETRUE)" << endl;
  DecSP();
  out_ << "M=-1" << endl;
  IncSP();
  GetReturn();
  out_ << "(START" << start_ << ")" << endl;
  start_++;
  
}

void CodeWriter::Push(string location, int index) {
  if (location == "3" || location == "5") {
    out_ << "@" << location << endl
      << "D=A" << endl
      << "@" << index << endl
      << "A=D+A" << endl
      << "D=M" << endl
      << "@SP" << endl
      << "A=M" << endl
      << "M=D" << endl;
  } else {
    out_ << "@" << location << endl
      << "D=M" << endl
      << "@" << index << endl
      << "A=D+A" << endl
      << "D=M" << endl
      << "@SP" << endl
      << "A=M" << endl
      << "M=D" << endl;
  }

  IncSP();
}

void CodeWriter::PopGPR(int num) {
  // 13から15は汎用的なレジスタとして使うことができる
  assert(num >= 13 && num <= 15);
  out_ << "@" << num << endl
    << "M=D" << endl;
}

void CodeWriter::Pop(string location, int index) {
  // スタックからpopしてsegmentのindexにわたす

  if (location == "3" || location == "5") {
    out_ << "@" << location << endl
      << "D=A" << endl
      << "@" << index << endl
      << "D=D+A" << endl; //popしたあとに格納されるメモリのアドレス
    PopGPR(13); // M[R13]にpopしたあとに格納されるメモリのアドレスが入ってる
    DecSP();
    out_ << "D=M" << endl
      << "@R13" << endl
      << "A=M" << endl
      << "M=D" << endl;
  } else {
    out_ << "@" << location << endl
      << "D=M" << endl
      << "@" << index << endl
      << "D=D+A" << endl;
    PopGPR(13);
    DecSP();
    out_ << "D=M" << endl
      << "@R13" << endl
      << "A=M" << endl
      << "M=D" << endl;
  }

  IncSP();
}

void CodeWriter::WriteArithmetic(string command) {
  if (command == "add") {
    PopD();
    DecSP();
    out_ << "M=D+M" << endl;
    IncSP();
    return;
  } else if (command == "sub") {
    PopD();
    DecSP();
    out_ << "M=M-D" << endl;
    IncSP();
  } else if (command == "neg") {
    DecSP();
    out_ << "M=-M" << endl;
    IncSP();
  } else if (command == "not") {
    DecSP();
    out_ << "M=!M" << endl;
    IncSP();
  } else if (command == "and") {
    PopD();
    DecSP();
    out_ << "M=D&M" << endl;
    IncSP();
  } else if (command == "or") {
    PopD();
    DecSP();
    out_ << "M=D|M" << endl;
  } else if (command == "eq") {
    SetReturn();
    PopD();
    DecSP();
    out_ << "D=D-M" << endl
      << "M=0" << endl;
    IncSP();
    out_ << "@MAKETRUE" << endl
      << "D;JEQ" << endl
      << "(RETURN" << labelnum_ << ")" << endl;
    labelnum_++;
  } else if (command == "gt") {
    SetReturn();
    PopD();
    DecSP();
    out_ << "D=D-M" << endl
      << "M=0" << endl;
    IncSP();
    out_ << "@MAKETRUE" << endl
      << "D;JLT" << "(RETURN" << labelnum_ << ")" << endl;
    labelnum_++;
  } else if (command == "lt") {
    SetReturn();
    PopD();
    DecSP();
    out_ << "D=D-M" << endl
      << "M=0" << endl;
    IncSP();
    out_ << "@MAKETRUE" << endl
      << "D;JGT" << endl
      << "(RETURN" << labelnum_ << ")" << endl;
    labelnum_++;
  }
}

void CodeWriter::WritePushPop(CommandType command, string segment, int index) {
  if (command == C_PUSH) {
    if (segment == "constant") {
      out_ << "@" << index << endl
        << "D=A" << endl
        << "@SP" << endl // SPのアドレスが入っている場所
        << "A=M" << endl // AにSPのアドレスが入っている場所を代入
        << "M=D" << endl;
      IncSP();
    } else if (segment == "local") {
      Push("LCL", index);
    } else if (segment == "argument") {
      Push("ARG", index);
    } else if (segment == "this") {
      Push("THIS", index);
    } else if (segment == "that") {
      Push("THAT", index);
    } else if (segment == "pointer") {
      Push("3", index);
    } else if (segment == "temp") {
      Push("5", index);
    } else if (segment == "static") {
      out_ << "@" filename_ << "." << index << endl
        << "D=M" << endl
        << "@SP" << endl
        << "A=M" << endl
        << "M=D" << endl
        << "@SP" << endl
        << "M=M+1" << endl;
    }
  } else if (command == C_POP) {
    if (segment == "local") {
      Pop("LCL", index);
    } else if (segment == "argument") {
      Pop("ARG", index);
    } else if (segment == "this") {
      Pop("THIS", index);
    } else if (segment == "that") {
      Pop("THAT", index);
    } else if (segment == "pointer") {
      Pop("3", index);
    } else if (segment == "temp") {
      Pop("5", index);
    } else if (segment == "static") {
      out_ << "@" << filename_ << "." << index << endl
        << "D=A" << endl
        << "@R13" << endl
        << "M=D" << endl
        << "@SP" << endl
        << "M=M-1" << endl
        << "A=M" << endl
        << "D=M" << endl
        << "@R13" << endl
        << "A=M" << endl
        << "M=D" << endl;
    }
  }
}