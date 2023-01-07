#include "compilation_engine.hpp"
#include "jack_tokenizer.hpp"
#include "symbol_table.hpp"
#include "vm_writer.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

using namespace std;
using namespace nand2tetris::compilation_engine;
using namespace nand2tetris::jack_tokenizer;
using namespace nand2tetris::symbol_table;
using namespace nand2tetris::vm_writer;

namespace CEhelper
{
  string KeyWord2String(JackKeyWord key)
  {
    switch (key)
    {
    case CLASS:
      return "class";
    case METHOD:
      return "method";
    case FUNCTION:
      return "function";
    case CONSTRUCTOR:
      return "constructor";
    case INT:
      return "int";
    case BOOLEAN:
      return "boolean";
    case CHAR:
      return "char";
    case VOID:
      return "void";
    case VAR:
      return "var";
    case STATIC:
      return "static";
    case FIELD:
      return "field";
    case LET:
      return "let";
    case DO:
      return "do";
    case IF:
      return "if";
    case ELSE:
      return "else";
    case WHILE:
      return "while";
    case RETURN:
      return "return";
    case TRUE:
      return "true";
    case FALSE:
      return "false";
    case null:
      return "null";
    case THIS:
      return "this";
    }
  }

  Kind KeyWord2Kind(JackKeyWord key)
  {
    switch (key)
    {
    case STATIC:
      return symbol_table::Kind::STATIC;
    case FIELD:
      return symbol_table::Kind::FIELD;
    case VAR:
      return symbol_table::Kind::VAR;
    }
  }

  Segment Kind2Segment(Kind kind)
  {
    switch (kind)
    {
    case symbol_table::Kind::STATIC:
      return Segment::STATIC;
    case symbol_table::Kind::FIELD:
      return Segment::THIS;
    case symbol_table::Kind::ARG:
      return Segment::ARG;
    case symbol_table::Kind::VAR:
      return Segment::LOCAL;
    }
  }
}

CompilationEngine::CompilationEngine(JackTokenizer *jack_tokenizer, VMWriter *vm_writer, SymbolTable *symbol_table)
{
  jack_tokenizer_ = jack_tokenizer;
  vm_writer_ = vm_writer;
  symbol_table_ = symbol_table;
  num_of_args_ = 0;
  label_num_ = 0;
  CompileClass();
}

CompilationEngine::~CompilationEngine()
{
  vm_writer_->close();
  cout << "compile complete" << endl;
}

void CompilationEngine::CompileClass()
{
  jack_tokenizer_->Advance();
  assert(jack_tokenizer_->KeyWord() == CLASS);
  jack_tokenizer_->Advance();
  assert(jack_tokenizer_->TokenType() == IDENTIFIER);
  class_name_ = jack_tokenizer_->Identifier();
  jack_tokenizer_->Advance();
  assert(jack_tokenizer_->Symbol() == '{');
  jack_tokenizer_->Advance();
  while ((jack_tokenizer_->TokenType() == KEYWORD) && (jack_tokenizer_->Keyword() == STATIC || jack_tokenizer_->KeyWord() == FIELD || jack_tokenizer_->KeyWord() == CONSTRUCTOR || jack_tokenizer_->KeyWord() == FUNCTION || jack_tokenizer_->KeyWord() == METHOD || jack_tokenizer_->KeyWord() == VOID))
  {
    switch (jack_tokenizer_->KeyWord())
    {
    case STATIC:
    case FIELD:
      CompileClassVarDec();
      break;
    case CONSTRUCTOR:
    case FUNCTION:
    case METHOD:
    case VOID:
      CompileSubroutine();
      break;
    default:
      break;
    }
  }
  assert(jack_tokenizer_->Symbol() == '}');
}

void CompilationEngine::CompileClassVarDec()
{
  string name, type;
  symbol_table::Kind kind;
  kind = CEhelper::KeyWord2String(jack_tokenizer_->KeyWord());
  jack_tokenizer_->Advance();
  if (jack_tokenizer_->TokenType() == KEYWORD)
  {
    type = CEhelper::KeyWord2String(jack_tokenizer_->KeyWord());
    jack_tokenizer_->Advance();
  }
  else if (jack_tokenizer_->TokenType() == IDENTIFIER)
  {
    type = jack_tokenizer_->Identifier();
    jack_tokenizer_->Advance();
  }
  while (jack_tokenizer_->GetCurrentToken() != ";")
  {
    switch (jack_tokenizer_->TokenType())
    {
    case IDENTIFIER:
      name = jack_tokenizer_->Identifier();
      symbol_table_->Define(name, type, kind);
      break;
    case SYMBOL:
      jack_tokenizer_->UpdateCurrentToken();
      break;
    }
    if (jack_tokenizer_->GetCurrentToken() == "")
    {
      jack_tokenizer_->Advance();
    }
  }
  assert(jack_tokenizer_->Symbol() == ';');
  jack_tokenizer_->Advance();
}

void CompilationEngine::CompileSubroutine()
{
  symbol_table_->StartSubRoutine();
  subroutine_type_ = jack_tokenizer_->KeyWord();
  jack_tokenizer_->Advance();
  if (subroutine_type_ == METHOD)
  {
    symbol_table_->Define("this", class_name_, symbol_table::ARG);
  }
  string return_type;
  if (jack_tokenizer_->TokenType() == KEYWORD)
  {
    return_type = CEhelper::KeyWord2String(jack_tokenizer_->KeyWord());
    jack_tokenizer_->Advance();
  }
  else if (jack_tokenizer_->TokenType() == IDENTIFIER)
  {
    return_type = jack_tokenizer_->Identifier();
    jack_tokenizer_->Advance();
  }

  if (jack_tokenizer_->TokenType() == IDENTIFIER)
  {
    current_function_name_ = class_name_ + "." + jack_tokenizer_->Identifier();
    if (jack_tokenizer_->GetCurrentToken() == "")
    {
      jack_tokenizer_->Advance();
    }
    else if (jack_tokenizer_->Symbol() == '(')
    {
      jack_tokenizer_->UpdateCurrentToken();
      if (jack_tokenizer_->GetCurrentToken() == "")
      {
        jack_tokenizer_->Advance();
      }
      CompileParameterList();
    }
    if (jack_tokenizer_->GetCurrentToken() == ")")
    {
      jack_tokenizer_->UpdateCurrentToken();
    }
    if (jack_tokenizer_->GetCurrentToken() == "")
    {
      jack_tokenizer_->Advance();
    }
    if (jack_tokenizer_->GetCurrentToken() == "{")
    {
      CompileSubroutineBody();
    }
  }
  jack_tokenizer_->Advance();
}

void CompilationEngine::CompileParameterList()
{
  string name, type;
  symbol_table::Kind kind = symbol_table::ARG;
  int sm = 0;
  while (jack_tokenizer_->GetCurrentToken() != ")")
  {
    switch (jack_tokenizer_->TokenType())
    {
    case KEYWORD:
      if (sm == 0)
      {
        type = CEhelper::KeyWord2String(jack_tokenizer_->KeyWord());
        jack_tokenizer_->Advance();
        sm = 1
      }
      break;
    case IDENTIFIER:
      if (sm == 1)
      {
        name = jack_tokenizer_->Identifier();
        symbol_table_->Define(name, type, kind);
        sm = 0;
      }
      else if (sm == 0)
      {
        type = jack_tokenizer_->Identifier();
        jack_tokenizer_->Advance();
        sm = 1;
      }
      break;
    case SYMBOL:
      jack_tokenizer_->UpdateCurrentToken();
      sm = 0;
      break;
    }
    if (jack_tokenizer_->GetCurrentToken() == "")
    {
      jack_tokenizer_->Advance();
    }
  }
}

void CompilationEngine::CompileSubroutineBody()
{
  jack_tokenizer_->UpdateCurrentToken();
  if (jack_tokenizer_->GetCurrentToken() == "")
  {
    jack_tokenizer_->Advance();
  }
  while (jack_tokenizer_->KeyWord() == VAR)
  {
    CompileVarDec();
  }
  int num_of_locals = symbol_table_->VarCount(symbol_table::VAR);
  if (subroutine_type_ == CONSTRUCTOR)
  {
    vm_writer_->WritePush(vm_writer::CONST, symbol_table_->VarCount(symbol_table::FIELD));
    vm_writer_->WriteCall("Memory.alloc", 1);
    vm_writer_->WritePop(vm_writer::POINTER, 0);
  }
  else if (subroutine_type_ == METHOD)
  {
    vm_writer_->WritePush(vm_writer::ARG, 0);
    vm_writer_->WritePop(vm_writer::POINTER, 0);
  }
  if (jack_tokenizer_->KeyWord() == LET || jack_tokenizer_->KeyWord() == IF || jack_tokenizer_->KeyWord() == ELSE || jack_tokenizer_->KeyWord() == WHILE || jack_tokenizer_->KeyWord() == DO || jack_tokenizer_->KeyWord() == RETURN)
  {
    CompileStatements();
  }
  assert(jack_tokenizer_->Symbol() == '}');
}

void CompilationEngine::CompileVarDec()
{
  string name, type;
  symbol_table::Kind kind;
  kind = CEhelper::(jack_tokenizer_->KeyWord());
  jack_tokenizer_->Advance();
  int sm = 0;
  while (jack_tokenizer_->GetCurrentToken() != ";")
  {
    switch (jack_tokenizer_->TokenType())
    {
    case KEYWORD:
      if (sm == 0)
      {
        type = CEhelper::KeyWord2String(jack_tokenizer_->KeyWord());
        jack_tokenizer_->Advance();
        sm = 1;
        break;
      }
    case IDENTIFIER:
      if (sm == 1)
      {
        name = jack_tokenizer_->Identifier();
        symbol_table_->Define(name, type, kind);
        break;
      }
      else if (sm == 0)
      {
        type = jack_tokenizer_->Identifier();
        jack_tokenizer_->Advance();
        sm = 1;
        break;
      }
    case SYMBOL:
      jack_tokenizer_->UpdateCurrentToken();
      break;
    }
    if (jack_tokenizer_->GetCurrentToken() == "")
    {
      jack_tokenizer_->Advance();
    }
  }
  if (jack_tokenizer_->GetCurrentToken() == ";")
  {
    jack_tokenizer_->Advance();
  }
}

void CompilationEngine::CompileStatements()
{
  while ((jack_tokenizer_->TokenType() == KEYWORD) && (jack_tokenizer_->KeyWord() == LET || jack_tokenizer_->KeyWord() == IF || jack_tokenizer_->KeyWord() == ELSE || jack_tokenizer_->KeyWord() == WHILE || jack_tokenizer_->KeyWord() == DO || jack_tokenizer_->KeyWord() == RETURN))
  {
    switch (jack_tokenizer_->KeyWord())
    {
    case LET:
      CompileLet();
      break;
    case DO:
      CompileDo();
      break;
    case WHILE:
      CompileWhile();
      break;
    case RETURN:
      CompileReturn();
      break;
    case IF:
      CompileIf();
      break;
    }
  }
}

void CompilationEngine::CompileDo()
{
  jack_tokenizer_->Advance();
  string name = jack_tokenizer_->Identifier();
  if (jack_tokenizer_->Symbol() == '(')
  {
    jack_tokenizer_->UpdateCurrentToken();
    if (jack_tokenizer_->GetCurrentToken() == "")
    {
      jack_tokenizer_->Advance();
    }
    vm_writer_->WritePush(vm_writer::POINTER, 0);
    CompileExpressionList();
    if (jack_tokenizer_->Symbol() == ')')
    {
      jack_tokenizer_->UpdateCurrentToken();
      if (jack_tokenizer_->GetCurrentToken() == "")
      {
        jack_tokenizer_->Advance();
      }
      if (num_of_args_ > 0)
      {
        vm_writer_->WriteCall(class_name_ + "." + name, num_of_args_ + 1);
      }
      else
      {
        vm_writer_->WriteCall(class_name_ + "." + name, 1);
      }
      num_of_args_ = 0;
    }
  }
  else if (jack_tokenizer_->Symbol() == '.')
  {
    jack_tokenizer_->UpdateCurrentToken();
    string subroutine_name = jack_tokenizer_->Identifier();
    if (symbol_table_->KindOf(name) != symbol_table::NONE)
    {
      vm_writer_->WritePush(CEhelper::Kind2Segment(symbol_table_->KindOf(name)), symbol_table_->IndexOf(name));
      if (jack_tokenizer_->Symbol() == '(')
      {
        jack_tokenizer_->UpdateCurrentToken();
      }
      if (jack_tokenizer_->GetCurrentToken() == "")
      {
        jack_tokenizer_->Advance();
      }
      CompileExpressionList();
      if (jack_tokenizer_->Symbol() == ')')
      {
        jack_tokenizer_->UpdateCurrentToken();
        if (jack_tokenizer_->GetCurrentToken() == "")
        {
          jack_tokenizer_->Advance();
        }
      }
      vm_writer_->WriteCall(symbol_table_->TypeOf(name) + "." + subroutine_name, num_of_args_ + 1);
    }
    else
    {
      if (jack_tokenizer_->Symbol() == '(')
      {
        jack_tokenizer_->UpdateCurrentToken();
      }
      if (jack_tokenizer_->GetCurrentToken() == "")
      {
        jack_tokenizer_->Advance();
      }
      CompileExpressionList();
      if (jack_tokenizer_->Symbol() == ')')
      {
        jack_tokenizer_->UpdateCurrentToken();
        if (jack_tokenizer_->GetCurrentToken() == "")
        {
          jack_tokenizer_->Advance();
        }
      }
      vm_writer_->WriteCall(symbol_table_->TypeOf(name) + "." + subroutine_name, num_of_args_);
    }
    num_of_args_ = 0;
  }
  assert(jack_tokenizer_->Symbol() == ';');
  jack_tokenizer_->Advance();
  vm_writer_->WritePop(vm_writer::TEMP, 0);
}

void CompilationEngine::CompileLet()
{
  jack_tokenizer_->Advance();
  string LHSvar = jack_tokenizer_->Identifier();
  int is_array = 0;
  if (jack_tokenizer_->GetCurrentToken() == "")
  {
    jack_tokenizer_->Advance();
  }
  if (jack_tokenizer_->Symbol() == '[')
  {
    jack_tokenizer_->UpdateCurrentToken();
    if (jack_tokenizer_->GetCurrentToken() == "")
    {
      jack_tokenizer_->Advance();
    }
    CompileExpression();
    vm_writer_->WritePush(CEhelper::Kind2Segment(symbol_table_->KindOf(LHSvar)), symbol_table_->IndexOf(LHSvar));
    vm_writer_->WriteArithmetic(vm_writer::ADD);
    if (jack_tokenizer_->Symbol() == ']')
    {
      jack_tokenizer_->UpdateCurrentToken();
      if (jack_tokenizer_->GetCurrentToken() == "")
      {
        jack_tokenizer_->Advance();
      }
      is_array = 1;
    }
    if (jack_tokenizer_->Symbol() == '=')
    {
      jack_tokenizer_->UpdateCurrentToken();
      if (jack_tokenizer_->GetCurrentToken() == "")
      {
        jack_tokenizer_->Advance();
      }
      CompileExpression();
      if (is_array == 1)
      {
        vm_writer_->WritePop(vm_writer::TEMP, 0);
      }
    }
    assert(jack_tokenizer_->Symbol() == ';');
    if (is_array == 0)
    {
      vm_writer_->WritePop(CEhelper::Kind2Segment(symbol_table_->KindOf(LHSvar)), symbol_table_->IndexOf(LHSvar));
      jack_tokenizer_->Advance();
    }
    else
    {
      vm_writer_->WritePop(vm_writer::POINTER, 1);
      vm_writer_->WritePush(vm_writer::TEMP, 0);
      vm_writer_->WritePop(vm_writer::THAT, 0);
      jack_tokenizer_->Advance();
    }
  }
}

void CompilationEngine::CompileWhile()
{
  stringstream label_true, label_false;
  label_true << "WHILETRUE" << label_num_;
  label_false << "WHILEFALSE" << label_num_;
  label_num_++;
  jack_tokenizer_->UpdateCurrentToken();
  if (jack_tokenizer_->GetCurrentToken().substr(0, 1) != "(")
  {
    jack_tokenizer_->Advance();
  }
  assert(jack_tokenizer_->Symbol() == '(');
  jack_tokenizer_->UpdateCurrentToken();
  if (jack_tokenizer_->GetCurrentToken() == "")
    jack_tokenizer_->Advance();
  vm_writer_->WriteLabel(label_true.str());
  CompileExpression();
  vm_writer_->WriteArithmetic(vm_writer::NOT);
  vm_writer_->WriteIf(label_false.str());
  assert(jack_tokenizer_->Symbol() == ')');
  jack_tokenizer_->UpdateCurrentToken();
  if (jack_tokenizer_->GetCurrentToken() == "")
    jack_tokenizer_->Advance();
  assert(jack_tokenizer_->Symbol() == '{');
  jack_tokenizer_->UpdateCurrentToken();
  if (jack_tokenizer_->GetCurrentToken() == "")
    jack_tokenizer_->Advance();
  CompileStatements();
  vm_writer_->WriteGoTo(label_true.str());
  vm_writer_->WriteLabel(label_false.str());
  assert(jack_tokenizer_->Symbol() == '}');
  jack_tokenizer_->UpdateCurrentToken();
  if (jack_tokenizer_->GetCurrentToken() == "")
    jack_tokenizer_->Advance();
}

void CompilationEngine::CompileReturn()
{
  if (jack_tokenizer_->GetCurrentToken() == "return")
  {
    jack_tokenizer_->Advance();
    CompileExpression();
    assert(jack_tokenizer_->Symbol() == ';');
    jack_tokenizer_->Advance();
  }
  else if (jack_tokenizer_->GetCurrentToken() == "return;")
  {
    vm_writer_->WritePush(vm_writer::CONST, 0);
    vm_writer_->WriteReturn();
    jack_tokenizer_->Advance();
  }
}

void CompilationEngine::CompileIf()
{
  stringstream label_else, label_continue;
  label_else << "IFFALSE" << label_num_;
  label_continue << "CONTINUE" << label_num_;
  label_num_++;
  jack_tokenizer_->UpdateCurrentToken();
  jack_tokenizer_->UpdateCurrentToken(); // done twice since "if" is 2 letters
  if (jack_tokenizer_->GetCurrentToken() == "")
    jack_tokenizer_->Advance();
  assert(jack_tokenizer_->Symbol() == '(');
  jack_tokenizer_->UpdateCurrentToken();
  if (jack_tokenizer_->GetCurrentToken() == "")
    jack_tokenizer_->Advance();
  CompileExpression();
  vm_writer_->WriteArithmetic(vm_writer::NOT);
  vm_writer_->WriteIf(label_else.str());
  assert(jack_tokenizer_->Symbol() == ')');
  jack_tokenizer_->UpdateCurrentToken();
  if (jack_tokenizer_->GetCurrentToken() == "")
    jack_tokenizer_->Advance();
  assert(jack_tokenizer_->Symbol() == '{');
  jack_tokenizer_->UpdateCurrentToken();
  if (jack_tokenizer_->GetCurrentToken() == "")
    jack_tokenizer_->Advance();
  CompileStatements();
  vm_writer_->WriteGoTo(label_continue.str());
  assert(jack_tokenizer_->Symbol() == '}');
  jack_tokenizer_->UpdateCurrentToken();
  if (jack_tokenizer_->GetCurrentToken() == "")
    jack_tokenizer_->Advance();
  vm_writer_->WriteLabel(label_else.str());
  if (jack_tokenizer_->KeyWord() == KEYWORD && jack_tokenizer_->KeyWord() == ELSE)
  {
    jack_tokenizer_->UpdateCurrentToken();
    jack_tokenizer_->UpdateCurrentToken();
    jack_tokenizer_->UpdateCurrentToken();
    jack_tokenizer_->UpdateCurrentToken();
    if (jack_tokenizer_->GetCurrentToken() == "")
      jack_tokenizer_->Advance();
    assert(jack_tokenizer_->Symbol() == '{');
    jack_tokenizer_->UpdateCurrentToken();
    if (jack_tokenizer_->GetCurrentToken() == "")
      jack_tokenizer_->Advance();
    CompileStatements();
    assert(jack_tokenizer_->Symbol() == '}');
    jack_tokenizer_->UpdateCurrentToken();
    if (jack_tokenizer_->GetCurrentToken() == "")
      jack_tokenizer_->Advance();
  }
  vm_writer_->WriteLabel(label_continue.str());
}

void CompilationEngine::CompileTerm()
{
  string id, str_const;
  switch (jack_tokenizer_->TokenType())
  {
  case IDENTIFIER:
    id = jack_tokenizer_->Identifier();
    if (jack_tokenizer_->GetCurrentToken() != "")
    {
      if (jack_tokenizer_->Symbol() == '(') // subroutine call
      {
        jack_tokenizer_->UpdateCurrentToken();
        if (jack_tokenizer_->GetCurrentToken() == "")
        {
          jack_tokenizer_->Advance();
        }
        vm_writer_->WritePush(vm_writer::POINTER, 0);
        CompileExpressionList();
        if (jack_tokenizer_->Symbol() == ')')
        {
          jack_tokenizer_->UpdateCurrentToken();
          if (jack_tokenizer_->GetCurrentToken() == "")
            jack_tokenizer_->Advance();
          if (num_of_args_ > 0)
          {
            vm_writer_->WriteCall(class_name_ + "." + id, num_of_args_ + 1);
          }
          else
          {
            vm_writer_->WriteCall(class_name_ + "." + id, 1);
            num_of_args_ = 0;
          }
        }
      }
      else if (jack_tokenizer_->Symbol() == '.')
      {
        jack_tokenizer_->UpdateCurrentToken();
        string subroutine_name = jack_tokenizer_->Identifier();
        if (symbol_table_->KindOf(id) != symbol_table::NONE)
        {
          if (jack_tokenizer_->Symbol() == '(')
          {
            jack_tokenizer_->UpdateCurrentToken();
          }
          if (jack_tokenizer_->GetCurrentToken() == "")
            jack_tokenizer_->Advance();
          CompileExpressionList();
          if (jack_tokenizer_->Symbol() == ')')
          {
            jack_tokenizer_->UpdateCurrentToken();
            if (jack_tokenizer_->GetCurrentToken() == "")
            {
              jack_tokenizer_->Advance();
            }
          }
          vm_writer_->WriteCall(symbol_table_->TypeOf(id) + "." + subroutine_name, num_of_args_ + 1);
        }
        else
        {
          if (jack_tokenizer_->Symbol() == '(')
          {
            jack_tokenizer_->UpdateCurrentToken();
          }
          if (jack_tokenizer_->GetCurrentToken() == "")
          {
            jack_tokenizer_->Advance();
          }
          CompileExpressionList();
          if (jack_tokenizer_->Symbol() == ")")
          {
            jack_tokenizer_->UpdateCurrentToken();
            if (jack_tokenizer_->GetCurrentToken() == "")
            {
              jack_tokenizer_->Advance();
            }
          }
          vm_writer_->WriteCall(id + "." + subroutine_name, num_of_args_);
        }
        num_of_args_ = 0;
      }
      else if (jack_tokenizer_->Symbol() == '[')
      {
        jack_tokenizer_->UpdateCurrentToken();
        if (jack_tokenizer_->GetCurrentToken() == "")
          jack_tokenizer_->Advance();
        CompileExpression();
        vm_writer_->WritePush(CEhelper::Kind2Segment(symbol_table_->KindOf(id)), symbol_table_->IndexOf(id));
        vm_writer_->WriteArithmetic(vm_writer::ADD);
        if (jack_tokenizer_->Symbol() == ']')
        {
          jack_tokenizer_->UpdateCurrentToken();
        }
        vm_writer_->WritePop(vm_writer::POINTER, 1);
        vm_writer_->WritePush(vm_writer::THAT, 0);
      }
      else
      {
        vm_writer_->WritePush(CEhelper::Kind2Segment(symbol_table_->KindOf(id)), symbol_table_->IndexOf(id));
      }
    }
    else
    {
      vm_writer_->WritePush(CEhelper::Kind2Segment(symbol_table_->KindOf(id)), symbol_table_->IndexOf(id));
    }
    break;
  case INT_CONST:
    vm_writer_->WritePush(vm_writer::CONST, jack_tokenizer_->IntVal());
    break;
  case STRING_CONST:
    str_const = jack_tokenizer_->StringVal();
    vm_writer_->WritePush(vm_writer::CONST, str_const.length());
    vm_writer_->WriteCall("String.new", 1);
    for (int i = 0; i < str_const.length(); i++)
    {
      vm_writer_->WritePush(vm_writer::CONST, (int)str_const[i]);
      vm_writer_->WriteCall("String.appendChar", 2);
    }
    break;
  case KEYWORD: // true, false, null, this will automatically update current token when calling keyWord()
    switch (jack_tokenizer_->KeyWord())
    {
    case TRUE:
      vm_writer_->WritePush(vm_writer::CONST, 0);
      vm_writer_->WriteArithmetic(vm_writer::NOT);
      break;
    case FALSE:
    case null:
      vm_writer_->WritePush(vm_writer::CONST, 0);
      break;
    case THIS:
      vm_writer_->WritePush(vm_writer::POINTER, 0);
      break;
    default:
      break;
    }
    break;
  case SYMBOL:
    if (jack_tokenizer_->Symbol() == '(')
    {
      jack_tokenizer_->UpdateCurrentToken();
      if (jack_tokenizer_->GetCurrentToken() == "")
        jack_tokenizer_->Advance();
      CompileExpression();
      assert(jack_tokenizer_->Symbol() == ')');
      jack_tokenizer_->UpdateCurrentToken();
    }
    else // unaryOp term
    {
      char uOp = jack_tokenizer_->Symbol();
      jack_tokenizer_->UpdateCurrentToken();
      if (jack_tokenizer_->GetCurrentToken() == "")
        jack_tokenizer_->Advance();
      CompileTerm();
      switch (uOp)
      {
      case '-':
        vm_writer_->WriteArithmetic(vm_writer::NEG);
        break;
      case '~':
        vm_writer_->WriteArithmetic(vm_writer::NOT);
        break;
      default:
        cerr << "not a uop" << endl;
        break;
      }
    }
    break;
  }
  if (jack_tokenizer_->GetCurrentToken() == "")
    jack_tokenizer_->Advance();
}

void CompilationEngine::CompileExpression()
{
  CompileTerm();
  while (jack_tokenizer_->GetCurrentToken().substr(0, 1) != ";" && jack_tokenizer_->GetCurrentToken().substr(0, 1) != "}" && jack_tokenizer_->GetCurrentToken().substr(0, 1) != "]" && jack_tokenizer_->GetCurrentToken().substr(0, 1) != ")" && jack_tokenizer_->GetCurrentToken().substr(0, 1) != ",")
  {
    switch (jack_tokenizer_->TokenType())
    {
      char op;
    case SYMBOL:
      op = jack_tokenizer_->Symbol();
      jack_tokenizer_->UpdateCurrentToken();
      if (jack_tokenizer_->GetCurrentToken() == "")
      {
        jack_tokenizer_->Advance();
      }
      CompileTerm();
      switch (op)
      {
      case '+':
        vm_writer_->WriteArithmetic(vm_writer::ADD);
        break;
      case '-':
        vm_writer_->WriteArithmetic(vm_writer::SUB);
        break;
      case '*':
        vm_writer_->WriteCall("Math.multiply", 2);
        break;
      case '/':
        vm_writer_->WriteCall("Math.divide", 2);
        break;
      case '&':
        vm_writer_->WriteArithmetic(vm_writer::AND);
        break;
      case '|':
        vm_writer_->WriteArithmetic(vm_writer::OR);
        break;
      case '<':
        vm_writer_->WriteArithmetic(vm_writer::LT);
        break;
      case '>':
        vm_writer_->WriteArithmetic(vm_writer::GT);
        break;
      case '=':
        vm_writer_->WriteArithmetic(vm_writer::EQ);
        break;
      case '~':
        vm_writer_->WriteArithmetic(vm_writer::NOT);
        break;
      }
      break;
    default:
      CompileTerm();
      break;
    }
    if (jack_tokenizer_->GetCurrentToken() == "")
      jack_tokenizer_->Advance();
  }
}

void CompilationEngine::CompileExpressionList()
{
  while (jack_tokenizer_->GetCurrentToken().substr(0, 1) != ")")
  {
    CompileExpression();
    num_of_args_++;
    if (jack_tokenizer_->GetCurrentToken().substr(0, 1) == ",")
    {
      jack_tokenizer_->UpdateCurrentToken();
      if (jack_tokenizer_->GetCurrentToken() == "")
        jack_tokenizer_->Advance();
    }
  }
}