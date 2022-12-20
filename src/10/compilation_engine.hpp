#ifndef COMPILATION_ENGINE
#define COMPILATION_ENGINE 1

#include "jack_tokenizer.hpp"
#include <fstream>
#include <iostream>

namespace nand2tetris {
namespace compilation_engine {


class CompilationEngine {
  private:
    ifstream in_;
    ofstream out_;

  public:
    CompilationEngine(string input_file, string output_file): {};
    ~CompilationEngine();

    void CompileClass();
    void CompileClassVarDec();
    void CompileSubroutine();
    void CompileParameterList();
    void CompileVarDec();
    void CompileStatements();
    void CompileDo();
    void CompileLet();
    void CompileWhile();
    void CompileReturn();
    void CompileIf();
    void CompileExpression();
};

}
}

#endif