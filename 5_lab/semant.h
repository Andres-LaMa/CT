#pragma once
#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>

#include <iostream>
#include <list>
#include <map>
#include <set>

#include "cool-tree.h"
#include "list.h"
#include "stringtab.h"
#include "symtab.h"

#define TRUE 1
#define FALSE 0

class ClassTable;
typedef ClassTable *ClassTableP;

// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.  You may use it or not as
// you like: it is only here to provide a container for the supplied
// methods.

static Symbol arg, arg2, Bool, concat, cool_abort, copy, Int, in_int, in_string,
    IO, length, Main, main_meth, No_class, No_type, Object, out_int, out_string,
    prim_slot, self, SELF_TYPE, Str, str_field, substr, type_name, val;

static Class_ curr_class = NULL;
static ClassTable *classtable;
static SymbolTable<Symbol, Symbol> attribtable;

class ClassTable {
 private:
  int semant_errors;
  void install_basic_classes();
  std::ostream &error_stream;

 public:
  std::map<Symbol, Class_> m_classes;
  ClassTable(Classes);
  int errors() { return semant_errors; }
  std::ostream &semant_error();
  std::ostream &semant_error(Symbol filename, tree_node *t);
  std::ostream &semant_error(Class_ c);

  bool IsParent(Symbol ancestor, Symbol child);
  std::list<Symbol> GetInheritancePath(Symbol type);
  Symbol FindLCA(Symbol type1, Symbol type2);

  bool isCyclic(Symbol cls, std::set<Symbol> &visited,
                std::set<Symbol> &recursion_stack);
  bool check_inheritance_cycles();
};

#endif