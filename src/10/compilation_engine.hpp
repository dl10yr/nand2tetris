#ifndef COMPILATION_ENGINE
#define COMPILATION_ENGINE 1

#include "jack_tokenizer.hpp"
#include <fstream>
#include <iostream>
using namespace nand2tetris::jack_tokenizer;

namespace nand2tetris {
namespace compilation_engine {


class CompilationEngine {
  private:
    JackTokenizer * tokenizer_;
    ofstream out_;
    void CompileTerm();

  public:
    CompilationEngine(string input_file, string output_file): {};
    ~CompilationEngine();

    void CompileClass();
    void CompileClassVarDec();
    void CompileSubroutine();
    void CompileParameterList();
    void CompileSubroutineBody();
    void CompileVarDec();
    void CompileStatements();
    void CompileDo();
    void CompileLet();
    void CompileWhile();
    void CompileReturn();
    void CompileIf();
    void CompileExpression();
    void CompileExpressionList();
};

}
}

#endif