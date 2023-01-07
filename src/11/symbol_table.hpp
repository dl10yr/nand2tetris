#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE 1

#include <string>
#include <map>
using namespace std;

namespace nand2tetris
{
  namespace symbol_table
  {

    enum Kind
    {
      STATIC,
      FIELD,
      ARG,
      VAR,
      NONE
    };

    struct ST
    {
      string type;
      Kind kind;
      int index;
    };

    class SymbolTable
    {
    private:
      map<string, ST> class_scope_;
      map<string, ST> subroutine_scope_;
      int static_count_;
      int field_count_;
      int arg_count_;
      int var_count_;

    public:
      SymbolTable();

      void StartSubRoutine();
      void Define(string name, string type, Kind kind);
      int VarCount(Kind kind);
      Kind KindOf(string name);
      string TypeOf(string name);
      int IndexOf(string name);
    };

  }
}

#endif