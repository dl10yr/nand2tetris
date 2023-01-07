#include "symbol_table.hpp"
#include <map>
#include <string>
using namespace std;
using namespace nand2tetris::symbol_table;

bool IsKey(map<string, symbol_table::ST> &scope, string key)
{
  return !(scope.find(key) == scope.end());
}

SymbolTable::SymbolTable()
{
  class_scope_.clear();
  subroutine_scope_.clear();
  static_count_ = 0;
  field_count_ = 0;
  arg_count_ = 0;
  var_count_ = 0;
}

void SymbolTable::StartSubRoutine()
{
  subroutine_scope_.clear();
  arg_count_ = 0;
  var_count_ = 0;
}

void SymbolTable::Define(string name, string type, symbol_table::Kind kind)
{
  if (kind == symbol_table::STATIC)
  {
    class_scope_[name].type = type;
    class_scope_[name].kind = kind;
    class_scope_[name].index = static_count_;
    static_count_++;
  }
  else if (kind == symbol_table::FIELD)
  {
    class_scope_[name].type = type;
    class_scope_[name].kind = kind;
    class_scope_[name].index = field_count_;
    field_count_++;
  }
  else if (kind == symbol_table::ARG)
  {
    subroutine_scope_[name].type = type;
    subroutine_scope_[name].kind = kind;
    subroutine_scope_[name].index = var_count_;
    var_count_++;
  }
}

int SymbolTable::VarCount(symbol_table::Kind kind)
{
  switch (kind)
  {
  case STATIC:
    return static_count_;
  case symbol_table::FIELD:
    return field_count_;
  case symbol_table::ARG:
    return arg_count_;
  case symbol_table::VAR:
    return var_count_;
  default:
    return -1;
  }
}

Kind SymbolTable::KindOf(string name)
{
  if (IsKey(class_scope_, name))
    return class_scope_[name].kind;
  else if (IsKey(subroutine_scope_, name))
    return subroutine_scope_[name].kind else return NONE;
}

string SymbolTable::TypeOf(string name)
{
  if (IsKey(class_scope_, name))
  {
    return class_scope_[name].type;
  }
  else if (IsKey(subroutine_scope_, name))
  {
    return subroutine_scope_[name].type;
  }
  else
  {
    return "";
  }
}

int SymbolTable::IndexOf(string name)
{
  if (IsKey(class_scope_, name))
  {
    return class_scope_[name].index;
  }
  else if (IsKey(subroutine_scope_, name))
  {
    return subroutine_scope_[name].index;
  }
  else
  {
    return -1;
  }
}
