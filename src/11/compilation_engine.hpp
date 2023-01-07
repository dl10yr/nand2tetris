#ifndef COMPILATION_ENGINE
#define COMPILATION_ENGINE 1

#include "jack_tokenizer.hpp"
#include <fstream>
#include <iostream>
using namespace nand2tetris::jack_tokenizer;

namespace nand2tetris
{
  namespace compilation_engine
  {

    class CompilationEngine
    {
    private:
      JackTokenizer *jack_tokenizer_;
      VMWriter *vm_writer_;
      SymbolTable *symbol_table_;
      string current_function_name_;
      JackKeyWord subroutine_type_;
      string class_name_;
      int num_of_args_;
      int label_num_;

    public:
      CompilationEngine(JackTokenizer *jack_tokenizer, VMWriter *vm_writer, SymbolTable *symbol_table) : {};
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
      void CompileTerm();
    };

  }
}

#endif