/* $Id: cutset.h,v 1.9 2000/08/07 07:44:08 mostad Exp $ */

#ifndef __CUTSET_H_
#define __CUTSET_H_

#include <fstream>
#include <cstdlib>

#include "Node.h"
#include "special.h"

class person;
class Link;
class branch;
class cutset;
class systemdata;

class Linked_list {
  Link* first;
public:
  Linked_list() : first(0) {}
  virtual ~Linked_list();
  void add(Link*);
  void remove(Link*);
  Link* get_first() {return first;}
  Link* get_next(Link* lk);
  int empty() {return first==0;}
  int n_elements();
  virtual branch* get_branch() {return 0;}
  virtual cutset* get_cutset() {return 0;}
  void print(int indent);
};

class Link {
friend class Linked_list;
  Link* next;
  Link* prev;
  Linked_list* belongs_to;
public:
  Link() : next(0), prev(0), belongs_to(0) {}
  virtual ~Link() {}
  cutset* container_cutset() { return belongs_to->get_cutset(); }
  branch* container_branch() { return belongs_to->get_branch(); }
  virtual void collect_from(branch*) = 0;
  virtual int add_tables(int) = 0;
  virtual void remove_tables() = 0;
  virtual void remove_data() = 0;
  virtual double execute(systemdata& sd) = 0;
  virtual void print(int indent) = 0;
  virtual void sort() = 0;
  virtual int hasdata() = 0;
};

//////////////////////////////////////////////////////
//SOME INVARIANTS for the data structure after creation by the odds object,
//or after odds::end_cutset:
//
//  All links, except primcut, are kept in linked lists.
//  All branches are non-empty.
//  All cutsets, except primcut, are non-empty as a pers_list.
//  I)  Let pers* A, pers* B, and A is a parent of B. Then:
//      (A->get_branch() != 0 && 
//      (A->get_branch() == B->get_branch() || 
//       A->get_branch()->get_cutset() == B->get_cutset() ||
//       A->get_branch() == B->get_cutset()->get_branch())   ||
//      (A->get_cutset() != 0 &&
//      (A->get_cutset() == B->get_cutset() ||
//       A->get_cutset() == B->get_branch()->get_cutset() ||
//       A->get_cutset()->get_branch() == B->get_branch())
//  II) Assume A and B satisfy the condition in I). Then there exists a
//      sequence A, X1, X2, ..., Xn, B, so that:
//      * All of X1, X2, ..., Xn are related to A and B as in the conditon 
//        in I).
//      * Each successive pair is related as parent/child, OR A and B are
//        members in the same pers_list.
//
/////////////////////////////////////////////////////

class pers;

//The following class is used for making simplified copies of the pedigree:
//class pcopy: public Node {
class pcopy {
  person* alias;
  int male;
  pcopy* mother;
  pcopy* father;
  pcopy* child;
  pcopy* paternal_sibling;
  pcopy* maternal_sibling;
public:
  pcopy(person* p, int is_collapsed);
  ~pcopy() {}
  void set_relatives();
  void set_collapsed_relatives();
  void add_relatives_from(pcopy* p);
//  void initializeNode();
  int is_male() { return male; }
  pcopy* get_next_relative(pcopy*);
  pcopy* get_mother() { return mother;}
  pcopy* get_father() { return father;}
  pcopy* get_child()  { return child;}
  pcopy* get_paternal_sibling() { return paternal_sibling;}
  pcopy* get_maternal_sibling() { return maternal_sibling;}
  void printName();
  void print(ostream& s);
};

//The following class is a link in a branch list, and is a linked list
//containing pers and cutset objects.
class branch: public Link, public Linked_list {
public:
  branch() {}
  ~branch() {}
  //Move pers objects with data to the front of this linked list:
  virtual void sort() {
    int must_move = 0;
    Link* lk = get_first();
    Link* next_lk;
    while (lk) {
      lk->sort();
      next_lk = get_next(lk);
      if (lk->hasdata()) {
	if (must_move) {
	  remove(lk);
	  add(lk);
	}
      } else must_move = 1;
      lk = next_lk;
    }
  }
  virtual int hasdata() { return 0; }  
  virtual branch* get_branch() {return this;}
  virtual cutset* get_cutset() {return 0;}
  virtual void collect_from(branch*) {
// Removed 2012-03-08
    //    cout<<"INTERNAL ERROR in branch::collect_from.\n";
    //    exit(1);
  }
  virtual double execute(systemdata&) {
// Removed 2012-03-08
    //    cout<<"INTERNAL ERROR in branch::execute.\n";
    //    exit(1);
    return 1;
  }
  virtual int add_tables(int n_alleles);
  virtual void remove_tables();
  virtual void remove_data();
  virtual void print(int indent);
};

//The following class is a Link in either a pers_list, or a branch.  
class pers : public Link , public pcopy {
  int has_data;
  int allele1;
  int allele2;
  int paternal_allele;
  int maternal_allele;
  int is_processed;
public:
  pers(person* p, int is_collapsed) :
  pcopy(p,is_collapsed), has_data(0), 
  is_processed(0) {}
  ~pers() {}
  branch* get_owner_branch();
  cutset* get_owner_cutset();
  int add_data(systemdata& sd, int all1, int all2, 
	       int info, int& error);
  virtual void remove_data() { has_data = 0;}
  virtual void collect_from(branch*);
  virtual int add_tables(int) { return 0;}
  virtual void remove_tables() {}
  virtual double execute(systemdata& sd);
  double execute_cutset_part(systemdata& sd, int index);
  virtual void print(int indent);
  void print(ostream& s);
  virtual void sort() {}
  virtual int hasdata() { return has_data; }

      //Called from the Node level:
//      void establishCutset();


};

class branch_list: public Linked_list {
public:
  branch_list() {}
  virtual ~branch_list() {}
  void branch_add(branch* br) { add(br);}
  void branch_remove(branch* br) { remove(br);}
  branch* get_first_branch() { return (branch*) get_first();}
  branch* get_next_branch(branch* br) { return (branch*) get_next(br);}
  int empty_branch() { return empty();}
  virtual branch* get_branch() {return 0;}
  virtual cutset* get_cutset() {return 0;}
};

class pers_list: public Linked_list {
public:
  pers_list() {}
  virtual ~pers_list() {}
  void pers_add(pers* pr) { add(pr);}
  void pers_remove(pers* pr) { remove(pr);}
  pers* get_first_pers() { return (pers*) get_first();}
  pers* get_next_pers(pers* p) { return (pers*) get_next(p);}
  int empty_pers() { return empty();}
  virtual branch* get_branch() {return 0;}
  virtual cutset* get_cutset() {return 0;}
};

//The following class is a link as part of a branch, and is a branch list
//containing branches, and a pers_list containing pers objects.
class cutset: public Link, public branch_list, public pers_list {
  double* tab;
public:
  cutset() : tab(0) {}
  ~cutset() {}
  virtual void sort() {
    branch* br = get_first_branch();
    while (br) {
      br->sort();
      br = get_next_branch(br);
    }
  }
  virtual int hasdata() { return 0;}
  //The following routine assumes this cutset has exactly one branch.
  //This branch is then possibly split into several, in order to 
  //re-establish the invariant (II).
  void separate_branches();

      /// Make cutsets:
//      void makeCutsets();

  Link* find_relative_in_branch(branch* br);
  //Collect from oldbranch into the branch of this cutset exactly those
  //links necessary for the (indirect) relatives of this cutset's persons
  //to be in this branch:
  virtual void collect_from(branch* oldbranch);
  virtual int add_tables(int n_alleles);
  virtual void remove_tables();
  virtual void remove_data();
  virtual double execute(systemdata& sd) {
    //We assume this function is not called for primcut; in that
    //case we know this object is non-empty as a pers list.
    return get_first_pers()->execute_cutset_part(sd, 0);
  }
  double execute_cutset(systemdata& sd, int index);
  virtual branch* get_branch() {return 0;}
  virtual cutset* get_cutset() {return this;}
  virtual void print(int indent);
  void print(ostream& s);
};
  
#endif


