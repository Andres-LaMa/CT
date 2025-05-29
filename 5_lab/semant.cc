#include "semant.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <cstring>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "utilities.h"

static bool TESTING = false;
static std::ostringstream nop_sstream;
static std::ostream &log = TESTING ? std::cout : nop_sstream;

extern int semant_debug;
extern char *curr_filename;

typedef SymbolTable<Symbol, method_class> MethodTable;
static std::map<Symbol, MethodTable> methodtables;
static void initialize_constants(void) {
  arg = idtable.add_string("arg");
  arg2 = idtable.add_string("arg2");
  Bool = idtable.add_string("Bool");
  concat = idtable.add_string("concat");
  cool_abort = idtable.add_string("abort");
  copy = idtable.add_string("copy");
  Int = idtable.add_string("Int");
  in_int = idtable.add_string("in_int");
  in_string = idtable.add_string("in_string");
  IO = idtable.add_string("IO");
  length = idtable.add_string("length");
  Main = idtable.add_string("Main");
  main_meth = idtable.add_string("main");
  No_class = idtable.add_string("_no_class");
  No_type = idtable.add_string("_no_type");
  Object = idtable.add_string("Object");
  out_int = idtable.add_string("out_int");
  out_string = idtable.add_string("out_string");
  prim_slot = idtable.add_string("_prim_slot");
  self = idtable.add_string("self");
  SELF_TYPE = idtable.add_string("SELF_TYPE");
  Str = idtable.add_string("String");
  str_field = idtable.add_string("_str_field");
  substr = idtable.add_string("substr");
  type_name = idtable.add_string("type_name");
  val = idtable.add_string("_val");
}
ClassTable::ClassTable(Classes classes)
    : semant_errors(0), error_stream(std::cerr) {
  install_basic_classes();
  log << "Now building the inheritance graph:" << "\n";
  for (int i = classes->first(); classes->more(i); i = classes->next(i)) {
    if (classes->nth(i)->GetName() == SELF_TYPE) {
      semant_error(classes->nth(i)) << "Error! SELF_TYPE redeclared!" << "\n";
    }
    if (m_classes.find(classes->nth(i)->GetName()) == m_classes.end()) {
      m_classes.insert(
          std::make_pair(classes->nth(i)->GetName(), classes->nth(i)));
    } else {
      semant_error(classes->nth(i))
          << "Error! Class " << classes->nth(i)->GetName()
          << " has been defined!" << "\n";
      return;
    }
  }

  if (m_classes.find(Main) == m_classes.end()) {
    semant_error() << "Class Main is not defined." << "\n";
  } else {
    bool has_main = false;
    Features features = m_classes[Main]->GetFeatures();
    for (int j = features->first(); features->more(j); j = features->next(j)) {
      Feature feature = features->nth(j);
      if (feature->IsMethod() && feature->GetName() == main_meth) {
        has_main = true;
        break;
      }
    }
    if (!has_main) {
      semant_error(m_classes[Main])
          << "Class Main must have a method 'main'" << "\n";
    }
  }
  for (int i = classes->first(); classes->more(i); i = classes->next(i)) {
    curr_class = classes->nth(i);

    Symbol parent_name = curr_class->GetParent();
    while (parent_name != Object && parent_name != classes->nth(i)->GetName()) {
      if (m_classes.find(parent_name) == m_classes.end()) {
        semant_error(curr_class)
            << "Error! Cannot find class " << parent_name << "\n";
        return;
      }
      if (parent_name == Int || parent_name == Str ||
          parent_name == SELF_TYPE || parent_name == Bool) {
        semant_error(curr_class)
            << "Error! Class " << curr_class->GetName()
            << " cannot inherit from " << parent_name << "\n";
        return;
      }

      log << " <- " << parent_name;
      curr_class = m_classes[parent_name];
      parent_name = curr_class->GetParent();
    }

    if (parent_name == Object) {
      log << " <- " << parent_name << "\n";
    } else {
      semant_error(curr_class) << "Error! Cycle inheritance!" << "\n";
      return;
    }
  }

  log << "\n";
}
bool ClassTable::IsParent(Symbol ancestor, Symbol child) {
  if (ancestor == Int || ancestor == Bool || ancestor == Str) {
    return false;
  }

  if (ancestor == SELF_TYPE) {
    return child == SELF_TYPE;
  }

  if (child == SELF_TYPE) {
    child = curr_class->GetName();
  }

  for (; child != No_class;
       child = m_classes.find(child)->second->GetParent()) {
    if (child == ancestor) {
      return true;
    }
  }
  return false;
}
std::list<Symbol> ClassTable::GetInheritancePath(Symbol type) {
  if (type == SELF_TYPE) {
    type = curr_class->GetName();
  }

  std::list<Symbol> path;
  for (; type != No_class; type = m_classes[type]->GetParent()) {
    path.push_front(type);
  }

  return path;
}
Symbol ClassTable::FindLCA(Symbol type1, Symbol type2) {
  std::list<Symbol> path1 = GetInheritancePath(type1);
  std::list<Symbol> path2 = GetInheritancePath(type2);

  Symbol ret;
  std::list<Symbol>::iterator iter1 = path1.begin(), iter2 = path2.begin();

  while (iter1 != path1.end() && iter2 != path2.end()) {
    if (*iter1 == *iter2) {
      ret = *iter1;
    } else {
      break;
    }

    iter1++;
    iter2++;
  }

  return ret;
}
void ClassTable::install_basic_classes() {
  Symbol filename = stringtable.add_string("<basic class>");

  Class_ Object_class = class_(
      Object, No_class,
      append_Features(
          append_Features(single_Features(method(cool_abort, nil_Formals(),
                                                 Object, no_expr())),
                          single_Features(method(type_name, nil_Formals(), Str,
                                                 no_expr()))),
          single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
      filename);
  Class_ IO_class = class_(
      IO, Object,
      append_Features(
          append_Features(
              append_Features(single_Features(method(
                                  out_string, single_Formals(formal(arg, Str)),
                                  SELF_TYPE, no_expr())),
                              single_Features(method(
                                  out_int, single_Formals(formal(arg, Int)),
                                  SELF_TYPE, no_expr()))),
              single_Features(
                  method(in_string, nil_Formals(), Str, no_expr()))),
          single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
      filename);
  Class_ Int_class = class_(
      Int, Object, single_Features(attr(val, prim_slot, no_expr())), filename);
  Class_ Bool_class = class_(
      Bool, Object, single_Features(attr(val, prim_slot, no_expr())), filename);
  Class_ Str_class = class_(
      Str, Object,
      append_Features(
          append_Features(
              append_Features(
                  append_Features(
                      single_Features(attr(val, Int, no_expr())),
                      single_Features(attr(str_field, prim_slot, no_expr()))),
                  single_Features(
                      method(length, nil_Formals(), Int, no_expr()))),
              single_Features(method(concat, single_Formals(formal(arg, Str)),
                                     Str, no_expr()))),
          single_Features(
              method(substr,
                     append_Formals(single_Formals(formal(arg, Int)),
                                    single_Formals(formal(arg2, Int))),
                     Str, no_expr()))),
      filename);

  m_classes.insert(std::make_pair(Object, Object_class));
  m_classes.insert(std::make_pair(IO, IO_class));
  m_classes.insert(std::make_pair(Int, Int_class));
  m_classes.insert(std::make_pair(Bool, Bool_class));
  m_classes.insert(std::make_pair(Str, Str_class));
}

bool ClassTable::isCyclic(Symbol cls, std::set<Symbol> &visited,
                          std::set<Symbol> &recursion_stack) {
  if (visited.find(cls) != visited.end()) return false;

  visited.insert(cls);
  recursion_stack.insert(cls);

  Symbol parent = m_classes[cls]->GetParent();
  if (parent != No_class && isCyclic(parent, visited, recursion_stack))
    return true;

  recursion_stack.erase(cls);
  return false;
}

bool ClassTable::check_inheritance_cycles() {
  std::set<Symbol> visited;
  std::set<Symbol> recursion_stack;

  for (auto &cls : m_classes) {
    if (isCyclic(cls.first, visited, recursion_stack)) {
      return true;
    }
  }
  return false;
}

std::ostream &ClassTable::semant_error() {
  semant_errors++;
  return error_stream;
}

std::ostream &ClassTable::semant_error(Class_ c) {
  return semant_error(c->get_filename(), c);
}

std::ostream &ClassTable::semant_error(Symbol filename, tree_node *t) {
  error_stream << filename << ":" << t->get_line_number() << ": ";
  return semant_error();
}

void method_class::AddMethodToTable(Symbol class_name) {
  log << "    Adding method " << name << "\n";
  methodtables[class_name].addid(
      name,
      new method_class(copy_Symbol(name), formals->copy_list(),
                       copy_Symbol(return_type), expr->copy_Expression()));
}

void method_class::AddAttribToTable(Symbol class_name) {}

void attr_class::AddMethodToTable(Symbol class_name) {}

void attr_class::AddAttribToTable(Symbol class_name) {
  log << "Adding attrib " << name << "\n";

  if (name == self) {
    classtable->semant_error(curr_class)
        << "Error! 'self' cannot be the name of an attribute in class "
        << curr_class->GetName() << "\n";
  }
  if (attribtable.lookup(name) != NULL) {
    classtable->semant_error(curr_class)
        << "Error! attribute '" << name << "' already exists!" << "\n";
    return;
  }

  attribtable.addid(name, new Symbol(type_decl));
}
void method_class::CheckFeatureType() {
  log << "    Checking method \"" << name << "\"" << "\n";

  if (curr_class && curr_class->GetName() == Main && name == main_meth) {
    if (return_type != Object) {
      classtable->semant_error(curr_class)
          << "Method 'main' in class 'Main' must have return type 'Object'"
          << "\n";
    }
  }

  if (classtable->m_classes.find(return_type) == classtable->m_classes.end() &&
      return_type != SELF_TYPE) {
    classtable->semant_error(curr_class)
        << "Error! return type " << return_type << " doesn't exist." << "\n";
  }
  attribtable.enterscope();
  std::set<Symbol> used_names;
  for (int i = formals->first(); formals->more(i); i = formals->next(i)) {
    Symbol name = formals->nth(i)->GetName();
    if (used_names.find(name) != used_names.end()) {
      classtable->semant_error(curr_class)
          << "Error! formal name duplicated. " << "\n";
    } else {
      used_names.insert(name);
    }

    Symbol type = formals->nth(i)->GetType();
    if (classtable->m_classes.find(type) == classtable->m_classes.end()) {
      classtable->semant_error(curr_class)
          << "Error! Cannot find class " << type << "\n";
    }
    if (formals->nth(i)->GetName() == self) {
      classtable->semant_error(curr_class) << "Error! self in formal " << "\n";
    }
    attribtable.addid(formals->nth(i)->GetName(),
                      new Symbol(formals->nth(i)->GetType()));
  }

  Symbol expr_type = expr->CheckExprType();
  if (classtable->IsParent(return_type, expr_type) == false) {
    classtable->semant_error(curr_class)
        << "Error! return type is not ancestor of expr type. " << "\n";
  }
  attribtable.exitscope();
}

void attr_class::CheckFeatureType() {
  log << "    Checking attribute \"" << name << "\"" << "\n";

  if (init->CheckExprType() != No_type) {
    Symbol init_type = init->CheckExprType();
    if (!classtable->IsParent(type_decl, init_type)) {
      classtable->semant_error(curr_class)
          << "Error! Initialization type doesn't match attribute type." << "\n";
    }
  }
}

Symbol assign_class::CheckExprType() {
  Symbol *lvalue_type = attribtable.lookup(name);
  Symbol rvalue_type = expr->CheckExprType();
  if (lvalue_type == NULL) {
    classtable->semant_error(curr_class)
        << "Error! Cannot find lvalue " << name << "\n";
    type = Object;
    return type;
  }
  if (classtable->IsParent(*lvalue_type, rvalue_type) == false) {
    classtable->semant_error(curr_class)
        << "Error! lvalue is not an ancestor of rvalue. " << "\n";
    type = Object;
    return type;
  }
  type = rvalue_type;
  return type;
}

Symbol static_dispatch_class::CheckExprType() {
  bool error = false;

  Symbol expr_class = expr->CheckExprType();

  if (classtable->IsParent(type_name, expr_class) == false) {
    error = true;
    classtable->semant_error(curr_class)
        << "Error! Static dispatch class is not an ancestor." << "\n";
  }

  log << "Static dispatch: class = " << type_name << "\n";
  std::list<Symbol> path = classtable->GetInheritancePath(type_name);
  method_class *method = NULL;
  for (std::list<Symbol>::iterator iter = path.begin(); iter != path.end();
       ++iter) {
    log << "Looking for method in class " << *iter << "\n";
    if ((method = methodtables[*iter].lookup(name)) != NULL) {
      break;
    }
  }

  if (method == NULL) {
    error = true;
    classtable->semant_error(curr_class)
        << "Error! Cannot find method '" << name << "'" << "\n";
  }
  for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
    Symbol actual_type = actual->nth(i)->CheckExprType();
    if (method != NULL) {
      Symbol formal_type = method->GetFormals()->nth(i)->GetType();
      if (classtable->IsParent(formal_type, actual_type) == false) {
        classtable->semant_error(curr_class)
            << "Error! Actual type " << actual_type
            << " doesn't suit formal type " << formal_type << "\n";
        error = true;
      }
    }
  }

  if (error) {
    type = Object;
  } else {
    type = method->GetType();
    if (type == SELF_TYPE) {
      type = type_name;
    }
  }

  return type;
}

Symbol dispatch_class::CheckExprType() {
  bool error = false;

  Symbol expr_type = expr->CheckExprType();

  if (expr_type == SELF_TYPE) {
    log << "Dispatch: class = " << SELF_TYPE << "_" << curr_class->GetName()
        << "\n";
  } else {
    log << "Dispatch: class = " << expr_type << "\n";
  }
  std::list<Symbol> path = classtable->GetInheritancePath(expr_type);
  method_class *method = NULL;
  for (std::list<Symbol>::iterator iter = path.begin(); iter != path.end();
       ++iter) {
    log << "Looking for method in class " << *iter << "\n";
    if ((method = methodtables[*iter].lookup(name)) != NULL) {
      break;
    }
  }

  if (method == NULL) {
    error = true;
    classtable->semant_error(curr_class)
        << "Error! Cannot find method '" << name << "'" << "\n";
  }
  for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
    Symbol actual_type = actual->nth(i)->CheckExprType();
    if (method != NULL) {
      Symbol formal_type = method->GetFormals()->nth(i)->GetType();
      if (classtable->IsParent(formal_type, actual_type) == false) {
        classtable->semant_error(curr_class)
            << "Error! Actual type " << actual_type
            << " doesn't suit formal type " << formal_type << "\n";
        error = true;
      }
    }
  }

  if (error) {
    type = Object;
  } else {
    type = method->GetType();
    if (type == SELF_TYPE) {
      type = expr_type;
    }
  }

  return type;
}
Symbol cond_class::CheckExprType() {
  if (pred->CheckExprType() != Bool) {
    classtable->semant_error(curr_class)
        << "Error! Type of pred is not Bool." << "\n";
  }

  Symbol then_type = then_exp->CheckExprType();
  Symbol else_type = else_exp->CheckExprType();

  if (else_type == No_type) {
    type = then_type;
  } else {
    type = classtable->FindLCA(then_type, else_type);
  }
  return type;
}

Symbol loop_class::CheckExprType() {
  if (pred->CheckExprType() != Bool) {
    classtable->semant_error(curr_class)
        << "Error! Type of pred is not Bool." << "\n";
  }
  body->CheckExprType();
  type = Object;
  return type;
}
Symbol typcase_class::CheckExprType() {
  Symbol expr_type = expr->CheckExprType();

  Case branch;
  std::vector<Symbol> branch_types;
  std::vector<Symbol> branch_type_decls;

  for (int i = cases->first(); cases->more(i); i = cases->next(i)) {
    branch = cases->nth(i);
    Symbol branch_type = branch->CheckBranchType();
    branch_types.push_back(branch_type);
    branch_type_decls.push_back(((branch_class *)branch)->GetTypeDecl());
  }

  for (size_t i = 0; i < branch_types.size() - 1; ++i) {
    for (size_t j = i + 1; j < branch_types.size(); ++j) {
      if (branch_type_decls[i] == branch_type_decls[j]) {
        classtable->semant_error(curr_class)
            << "Error! Two branches have same type." << "\n";
      }
    }
  }

  type = branch_types[0];
  for (size_t i = 1; i < branch_types.size(); ++i) {
    type = classtable->FindLCA(type, branch_types[i]);
  }
  return type;
}
Symbol branch_class::CheckBranchType() {
  attribtable.enterscope();

  attribtable.addid(name, new Symbol(type_decl));
  Symbol type = expr->CheckExprType();

  attribtable.exitscope();

  return type;
}

Symbol block_class::CheckExprType() {
  for (int i = body->first(); body->more(i); i = body->next(i)) {
    type = body->nth(i)->CheckExprType();
  }
  return type;
}
Symbol let_class::CheckExprType() {
  if (identifier == self) {
    classtable->semant_error(curr_class)
        << "Error! self in let binding." << "\n";
  }
  attribtable.enterscope();
  attribtable.addid(identifier, new Symbol(type_decl));

  Symbol init_type = init->CheckExprType();
  if (init_type != No_type) {
    if (classtable->IsParent(type_decl, init_type) == false) {
      classtable->semant_error(curr_class)
          << "Error! init value is not child." << "\n";
    }
  }

  type = body->CheckExprType();
  attribtable.exitscope();
  return type;
}

Symbol plus_class::CheckExprType() {
  Symbol e1_type = e1->CheckExprType();
  Symbol e2_type = e2->CheckExprType();
  if (e1_type != Int || e2_type != Int) {
    classtable->semant_error(curr_class)
        << "Error! '+' meets non-Int value." << "\n";
    type = Object;
  } else {
    type = Int;
  }
  return type;
}

Symbol sub_class::CheckExprType() {
  Symbol e1_type = e1->CheckExprType();
  Symbol e2_type = e2->CheckExprType();
  if (e1_type != Int || e2_type != Int) {
    classtable->semant_error(curr_class)
        << "Error! '-' meets non-Int value." << "\n";
    type = Object;
  } else {
    type = Int;
  }
  return type;
}

Symbol mul_class::CheckExprType() {
  Symbol e1_type = e1->CheckExprType();
  Symbol e2_type = e2->CheckExprType();
  if (e1_type != Int || e2_type != Int) {
    classtable->semant_error(curr_class)
        << "Error! '*' meets non-Int value." << "\n";
    type = Object;
  } else {
    type = Int;
  }
  return type;
}

Symbol divide_class::CheckExprType() {
  Symbol e1_type = e1->CheckExprType();
  Symbol e2_type = e2->CheckExprType();
  if (e1_type != Int || e2_type != Int) {
    classtable->semant_error(curr_class)
        << "Error! '/' meets non-Int value." << "\n";
    type = Object;
  } else {
    type = Int;
  }
  return type;
}

Symbol neg_class::CheckExprType() {
  if (e1->CheckExprType() != Int) {
    classtable->semant_error(curr_class)
        << "Error! '~' meets non-Int value." << "\n";
    type = Object;
  } else {
    type = Int;
  }
  return type;
}

Symbol lt_class::CheckExprType() {
  Symbol e1_type = e1->CheckExprType();
  Symbol e2_type = e2->CheckExprType();
  if (e1_type != Int || e2_type != Int) {
    classtable->semant_error(curr_class)
        << "Error! '<' meets non-Int value." << "\n";
    type = Object;
  } else {
    type = Bool;
  }
  return type;
}
Symbol eq_class::CheckExprType() {
  Symbol e1_type = e1->CheckExprType();
  Symbol e2_type = e2->CheckExprType();
  if (e1_type == Int || e2_type == Int || e1_type == Bool || e2_type == Bool ||
      e1_type == Str || e2_type == Str) {
    if (e1_type != e2_type) {
      classtable->semant_error(curr_class)
          << "Error! '=' meets different types." << "\n";
      type = Object;
    } else {
      type = Bool;
    }
  } else {
    type = Bool;
  }
  return type;
}

Symbol leq_class::CheckExprType() {
  Symbol e1_type = e1->CheckExprType();
  Symbol e2_type = e2->CheckExprType();
  if (e1_type != Int || e2_type != Int) {
    classtable->semant_error(curr_class)
        << "Error! '<=' meets non-Int value." << "\n";
    type = Object;
  } else {
    type = Bool;
  }
  return type;
}

Symbol comp_class::CheckExprType() {
  if (e1->CheckExprType() != Bool) {
    classtable->semant_error(curr_class)
        << "Error! 'not' meets non-Bool value." << "\n";
    type = Object;
  } else {
    type = Bool;
  }
  return type;
}

Symbol int_const_class::CheckExprType() {
  type = Int;
  return type;
}

Symbol bool_const_class::CheckExprType() {
  type = Bool;
  return type;
}

Symbol string_const_class::CheckExprType() {
  type = Str;
  return type;
}

Symbol new__class::CheckExprType() {
  if (type_name == SELF_TYPE) {
    type = SELF_TYPE;
    return type;
  }
  if (classtable->m_classes.find(type_name) == classtable->m_classes.end()) {
    classtable->semant_error(curr_class)
        << "Error! type " << type_name << " doesn't exist." << "\n";
    type = Object;
  } else {
    type = type_name;
  }
  return type;
}

Symbol isvoid_class::CheckExprType() {
  e1->CheckExprType();
  type = Bool;
  return type;
}

Symbol no_expr_class::CheckExprType() { return No_type; }

Symbol object_class::CheckExprType() {
  if (name == self) {
    type = SELF_TYPE;
    return type;
  }

  Symbol *found_type = attribtable.lookup(name);
  if (found_type == NULL) {
    classtable->semant_error(curr_class)
        << "Cannot find object " << name << "\n";
    type = Object;
  } else {
    type = *found_type;
  }

  return type;
}
void program_class::semant() {
  initialize_constants();
  classtable = new ClassTable(classes);

  if (classtable->check_inheritance_cycles()) {
    std::cerr << "Compilation halted due to inheritance cycles." << "\n";
    exit(1);
  }

  if (classtable->errors()) {
    std::cerr << "Compilation halted due to static semantic errors." << "\n";
    exit(1);
  }
  log << "Now constructing the methodtables:" << "\n";
  for (std::map<Symbol, Class_>::iterator iter = classtable->m_classes.begin();
       iter != classtable->m_classes.end(); ++iter) {
    log << "class " << iter->first << ":" << "\n";

    Symbol class_name = iter->first;
    methodtables[class_name].enterscope();
    Features curr_features = classtable->m_classes[class_name]->GetFeatures();
    for (int j = curr_features->first(); curr_features->more(j);
         j = curr_features->next(j)) {
      Feature curr_feature = curr_features->nth(j);
      curr_feature->AddMethodToTable(class_name);
    }
  }
  log << "Now searching for illegal method overriding:" << "\n";
  for (std::map<Symbol, Class_>::iterator iter = classtable->m_classes.begin();
       iter != classtable->m_classes.end(); ++iter) {
    Symbol class_name = iter->first;
    curr_class = classtable->m_classes[class_name];
    log << "    Consider class " << class_name << ":" << "\n";

    Features curr_features = classtable->m_classes[class_name]->GetFeatures();

    for (int j = curr_features->first(); curr_features->more(j);
         j = curr_features->next(j)) {
      Feature curr_method = curr_features->nth(j);

      if (curr_method->IsMethod() == false) {
        continue;
      }

      log << "        method " << curr_method->GetName() << "\n";

      Formals curr_formals = ((method_class *)(curr_method))->GetFormals();

      std::list<Symbol> path = classtable->GetInheritancePath(class_name);
      for (std::list<Symbol>::reverse_iterator iter = path.rbegin();
           iter != path.rend(); ++iter) {
        Symbol ancestor_name = *iter;
        log << "            ancestor " << ancestor_name << "\n";
        method_class *method =
            methodtables[ancestor_name].lookup(curr_method->GetName());

        if (method != NULL) {
          Formals formals = method->GetFormals();

          int k1 = formals->first(), k2 = curr_formals->first();
          for (; formals->more(k1) && curr_formals->more(k2);
               k1 = formals->next(k1), k2 = formals->next(k2)) {
            if (formals->nth(k1)->GetType() !=
                curr_formals->nth(k2)->GetType()) {
              log << "error" << "\n";
              classtable->semant_error(classtable->m_classes[class_name])
                  << "Method override error: formal type not match." << "\n";
            }
          }

          if (formals->more(k1) || curr_formals->more(k2)) {
            log << "error" << "\n";
            classtable->semant_error(classtable->m_classes[class_name])
                << "Method override error: length of formals not match."
                << "\n";
          }
        }
      }
    }
  }

  log << "\n";
  log << "Now checking all the types:" << "\n";

  for (int i = classes->first(); classes->more(i); i = classes->next(i)) {
    curr_class = classes->nth(i);

    log << "Checking class " << curr_class->GetName() << ":" << "\n";
    std::list<Symbol> path =
        classtable->GetInheritancePath(curr_class->GetName());
    for (std::list<Symbol>::iterator iter = path.begin(); iter != path.end();
         iter++) {
      curr_class = classtable->m_classes[*iter];
      Features curr_features = curr_class->GetFeatures();
      attribtable.enterscope();
      for (int j = curr_features->first(); curr_features->more(j);
           j = curr_features->next(j)) {
        Feature curr_feature = curr_features->nth(j);
        curr_feature->AddAttribToTable(curr_class->GetName());
      }
    }

    curr_class = classes->nth(i);
    Features curr_features = curr_class->GetFeatures();
    for (int j = curr_features->first(); curr_features->more(j);
         j = curr_features->next(j)) {
      Feature curr_feature = curr_features->nth(j);
      curr_feature->CheckFeatureType();
    }

    for (size_t j = 0; j < path.size(); ++j) {
      attribtable.exitscope();
    }

    log << "\n";
  }

  if (classtable->errors()) {
    std::cerr << "Compilation halted due to static semantic errors." << "\n";
    exit(1);
  }
}
