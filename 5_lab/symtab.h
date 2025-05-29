#ifndef SYMTAB_H
#define SYMTAB_H

#include <list>
#include <map>
#include <vector>

#include "cool-tree.h"

template <typename Key, typename Value>
class SymbolTable {
 private:
  std::list<std::map<Key, Value *>> scopes;

 public:
  SymbolTable() {
    enterscope();  // Start with one scope
  }

  void enterscope() { scopes.push_front(std::map<Key, Value *>()); }

  void exitscope() {
    if (!scopes.empty()) {
      scopes.pop_front();
    }
  }

  void addid(Key key, Value *value) {
    if (!scopes.empty()) {
      scopes.front()[key] = value;
    }
  }

  Value *lookup(Key key) {
    for (auto &scope : scopes) {
      auto it = scope.find(key);
      if (it != scope.end()) {
        return it->second;
      }
    }
    return nullptr;
  }
};

#endif