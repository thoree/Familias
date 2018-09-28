/* $Id: person.h,v 1.3 2000/03/03 09:45:04 mostad Exp $ */

#ifndef __PERSON_H_
#define __PERSON_H_

class table;
class allelesystem;
class pers;

class person {

  /////////////////
  //The pcopy class sets and removes thes pointers:
  friend class pcopy;
  pers* alias;
  pers* collapsed_alias;
  /////////////////

  char* Name;
  int male;
  
  person* mother;
  person* father;
  person* child;
  person* paternal_sibling;
  person* maternal_sibling;
  person* next;

  int is_processed;
public:
  person(char* name, int ml);
  ~person() {	delete[] Name; }

  pers* get_alias() { return alias;}
  void clear_alias() { alias = 0;}
  pers* get_collapsed_alias() { return collapsed_alias;}
  void clear_collapsed_alias() { collapsed_alias = 0;}

  char* name() { return Name; }
  int is_male() { return male; }
  person* get_mother() { return mother; }
  person* get_father() { return father; }
  person* get_next() { return next; }
  void write_parents(table& tab);
  void clear_parents();
  
  //We may assume the relevant parent does not exist.
  //We assume that p != 0.
  //Thus there are also no siblings of the relevant type.
  void add_parent(person* p);
  
  //We assume that there is a mother.
  void remove_mother();
  
  //We assume that there is a father.
  void remove_father();
  
  void discard_children();
  person* remove_next(person* p);
  void add_next(person* p);
  void add_at_end(person* p);
  int has_ancestor(person* p);
};

#endif
