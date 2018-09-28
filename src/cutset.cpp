/* $Id: cutset.C,v 1.9 2001/04/12 16:00:36 mostad Exp $ */

#include "cutset.h"
#include "person.h"
#include "odds.h"
#include "special.h"

void Linked_list::print(int indent) {
  Link* lk = first;
  while (lk) {
    lk->print(indent+2);
    lk = lk->next;
    if (lk==first) return;
  }
}

Linked_list::~Linked_list() {
  if (first) {
    Link* lk = first;
    lk->prev->next = 0;
    while(lk) {
      first = first->next;
      delete lk;
      lk = first;
    }
  }
}

void Linked_list::add(Link* lk) {
  if (first) {
    lk->prev = first->prev;
    lk->next = first;
    first->prev->next = lk;
    first->prev = lk;
    first = lk;
  } else lk->next = lk->prev = first = lk;
  lk->belongs_to = this;
}

void Linked_list::remove(Link* lk) {
  if (lk->next == lk) first = 0;
  else {
    if (first == lk) first = lk->next;
    lk->next->prev = lk->prev;
    lk->prev->next = lk->next;
  }
  lk->belongs_to = 0;
}

Link* Linked_list::get_next(Link* lk) {
  if (lk->next==first) return 0;
  else return lk->next;
}

int Linked_list::n_elements() {
  if (!first) return 0;
  Link* lk = first->next;
  int result = 1;
  while (lk!=first) {
    result++;
    lk = lk->next;
  }
  return result;
}

pcopy::pcopy(person* p, int is_collapsed) : 
alias(p), male(p->is_male()), mother(0), father(0), 
child(0), paternal_sibling(0), maternal_sibling(0) {
  if (is_collapsed) p->collapsed_alias = (pers*) this;
  else p->alias = (pers*) this;
}

void pcopy::set_relatives() {
  if (alias->mother) mother = alias->mother->alias;
  if (alias->father) father = alias->father->alias;
  if (alias->child)  child  = alias->child->alias;
  if (alias->paternal_sibling) 
    paternal_sibling = alias->paternal_sibling->alias;
  if (alias->maternal_sibling) 
    maternal_sibling = alias->maternal_sibling->alias;
}

void pcopy::set_collapsed_relatives() {
  if (alias->mother) mother = alias->mother->collapsed_alias;
  if (alias->father) father = alias->father->collapsed_alias;
  if (alias->child)  child  = alias->child->collapsed_alias;
  if (alias->paternal_sibling) 
    paternal_sibling = alias->paternal_sibling->collapsed_alias;
  if (alias->maternal_sibling) 
    maternal_sibling = alias->maternal_sibling->collapsed_alias;
}
/*
void pcopy::initializeNode()
{
   if (mother) addNeighbour(mother);
   if (father) addNeighbour(father);
   pcopy* p = child;
   while (p)
   {
      addNeighbour(p);
      if (male) p = p->paternal_sibling;
      else      p = p->maternal_sibling;
   }
}
*/
void pcopy::add_relatives_from(pcopy* p) {
  pcopy* p1;
  pcopy* p2;
  if (p->father) {
    //Remove p from the childred of p->father:
    if (p->father->child==p) p->father->child = p->paternal_sibling;
    else {
      p1 = p->father->child;
      while (p1->paternal_sibling != p) p1 = p1->paternal_sibling;
      p1->paternal_sibling = p->paternal_sibling;
    }
    //Add father to this, if necessary:
    if (!father) { 
      father = p->father;
      paternal_sibling = father->child;
      father->child = this;
    }
  }
  if (p->mother) {
    //Remove p from the childred of p->mother:
    if (p->mother->child==p) p->mother->child = p->maternal_sibling;
    else {
      p1 = p->mother->child;
      while (p1->maternal_sibling != p) p1 = p1->maternal_sibling;
      p1->maternal_sibling = p->maternal_sibling;
    }
    //Add father to this, if necessary:
    if (!mother) { 
      mother = p->mother;
      maternal_sibling = mother->child;
      mother->child = this;
    }
  }
  p1 = p->child;
  if (male) {
    while (p1) {
      p2 = p1;
      p1 = p1->paternal_sibling;
      p2->father = this;
      p2->paternal_sibling = child;
      child = p2;
    }
  } else {
    while (p1) {
      p2 = p1;
      p1 = p1->maternal_sibling;
      p2->mother = this;
      p2->maternal_sibling = child;
      child = p2;
    }
  }
  p->alias->collapsed_alias = (pers*) this;
}
      
pcopy* pcopy::get_next_relative(pcopy* p) {
  if (p==0) {
    if (mother) return mother;
    else if (father) return father;
    else if (child) return child;
    else return 0;
  } else if (p==mother) {
    if (father) return father;
    else if (child) return child;
    else return 0;
  } else if (p==father) {
    if (child) return child;
    else return 0;
  } else if (male) {
    if (p->paternal_sibling) return p->paternal_sibling;
    else return 0;
  } else {
    if (p->maternal_sibling) return p->maternal_sibling;
    else return 0;
  }
}

void pcopy::printName() { 
// Removed 2012-03-08
//cout<<alias->name()<<'\n';
}

void pcopy::print(ostream& s) { 
  s<<alias->name(); 
}

int branch::add_tables(int n_alleles) {
  Link* lk = get_first();
  while (lk) {
    if (lk->add_tables(n_alleles)) return 1;
    lk = get_next(lk);
  }
  return 0;
}

void branch::remove_tables() {
  Link* lk = get_first();
  while (lk) {
    lk->remove_tables();
    lk = get_next(lk);
  }
}

void branch::remove_data() {
  Link* lk = get_first();
  while (lk) {
    lk->remove_data();
    lk = get_next(lk);
  }
}

void branch::print(int indent) {
// Removed 2012-03-08
//  for (int i=0; i<indent; i++) cout<<' ';
//  cout<<"This branch contains:\n";
  Linked_list::print(indent);
}    


branch* pers::get_owner_branch() { 
  if (container_branch()) return container_branch();
  else return container_cutset()->container_branch();
}

cutset* pers::get_owner_cutset() {
  if (container_cutset()) return container_cutset();
  else return container_branch()->container_cutset();
}

void pers::collect_from(branch* oldbranch) {
  branch* thisbranch = get_owner_branch();
  pers* p = 0;
  Link* lk;
  while ((p = (pers*) get_next_relative(p))) {
    if (p->container_cutset()==thisbranch->container_cutset()) continue;
    if (p->get_owner_branch()!=oldbranch) continue;

    //Move the object from one list to the other:
    lk = p->container_cutset();
    if (!lk) lk = p;
    oldbranch->remove(lk);
    thisbranch->add(lk);
    //Continue recursion:
    lk->collect_from(oldbranch);
  }
}

int pers::add_data(systemdata& sd, int all1, int all2, 
		   int info, int& error) 
{
   if (has_data) 
   {
      if ((all1 != allele1 || all2 != allele2) &&
	  (all1 != allele2 || all2 != allele1)) 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    cout<<"ERROR: In system \""<<sd.sysname()<<"\" the odds "
//	       "computation\nis incompatible with the given data.\n";
	 error = 1; 
	 return 1;
      }
   } 
   else 
   {
      has_data = 1;
      allele1 = all1;
      allele2 = all2;
   }
   return 0;
}

double pers::execute(systemdata& sd) {
  is_processed = 1;
  double sum = 0;
  double prob;
  int set_paternal_allele;
  int set_maternal_allele; 
  pers* p;
  for (paternal_allele=0; paternal_allele<sd.number_of_alleles(); paternal_allele++)
    for (maternal_allele=0; maternal_allele<sd.number_of_alleles(); maternal_allele++) {
		if (has_data && 
	  ((maternal_allele != allele1 ||
	    paternal_allele != allele2) &&
	   (paternal_allele != allele1 ||
	    maternal_allele != allele2)))
		{
			if (!sd.has_silent_allele()) continue; 
			if ((paternal_allele != sd.silent_allele() && 
				 maternal_allele != sd.silent_allele()) ||
				 allele1 != allele2) continue; 
			if (paternal_allele != allele1 && 
				maternal_allele != allele1) continue; 
		}
      prob = 1;
      pers* fath = (pers*) get_father();
      pers* moth = (pers*) get_mother();
      
      if (fath)
      {
	 set_paternal_allele = 0; 
	 if (fath->is_processed)
	    prob *= sd.inherit_prob_male(fath->paternal_allele,
					 fath->maternal_allele,
					 paternal_allele);
      }
      else 
      {
	 prob *= sd.set_allele(paternal_allele);
	 set_paternal_allele = 1; 
      }

      if (moth)
      {
	 set_maternal_allele = 0; 
	 if (moth->is_processed)
	    prob *= sd.inherit_prob_female(moth->paternal_allele,
					   moth->maternal_allele,
					   maternal_allele);
      }
      else 
      {
	 prob *= sd.set_allele(maternal_allele);
	 set_maternal_allele = 1; 
      }
      p = (pers*) get_child();
      if (is_male()) 
	while (p) {
	  if (p->is_processed) 
	    prob *= sd.inherit_prob_male(paternal_allele,
					 maternal_allele, 
					 p->paternal_allele);
	  p = (pers*) p->get_paternal_sibling();
	}
      else 
	while (p) {
	  if (p->is_processed)
	    prob *= sd.inherit_prob_female(paternal_allele,
					   maternal_allele, 
					   p->maternal_allele);
	  p = (pers*) p->get_maternal_sibling();
	}
      if (prob > 0) {
	Link* lk = container_branch()->get_next(this);
	if (lk) prob *= lk->execute(sd);
	sum += prob;
      }
      if (set_paternal_allele) sd.unset_allele(paternal_allele); 
      if (set_maternal_allele) sd.unset_allele(maternal_allele); 
    }
  is_processed = 0;
  return sum;
}

double pers::execute_cutset_part(systemdata& sd, int index) {
  is_processed = 1;
  double sum = 0;
  double prob;
  int set_paternal_allele;
  int set_maternal_allele; 
  pers* p;
  index *= sd.number_of_alleles()*sd.number_of_alleles();

  for (paternal_allele=0; paternal_allele<sd.number_of_alleles(); paternal_allele++)
    for (maternal_allele=0; maternal_allele<sd.number_of_alleles(); maternal_allele++) {
 		if (has_data && 
	  ((maternal_allele != allele1 ||
	    paternal_allele != allele2) &&
	   (paternal_allele != allele1 ||
	    maternal_allele != allele2)))
		{
			if (!sd.has_silent_allele()) continue; 
			if ((paternal_allele != sd.silent_allele() && 
				 maternal_allele != sd.silent_allele()) ||
				 allele1 != allele2) continue; 
			if (paternal_allele != allele1 && 
				maternal_allele != allele1) continue; 
		}
      
      prob = 1;
      pers* fath = (pers*) get_father();
      pers* moth = (pers*) get_mother();
      
      if (fath)
      {
	 set_paternal_allele = 0; 
	 if (fath->is_processed)
	    prob *= sd.inherit_prob_male(fath->paternal_allele,
					 fath->maternal_allele,
					 paternal_allele);
      }
      else 
      {
	 prob *= sd.set_allele(paternal_allele);
	 set_paternal_allele = 1; 
      }

      if (moth)
      {
	 set_maternal_allele = 0; 
	 if (moth->is_processed)
	    prob *= sd.inherit_prob_female(moth->paternal_allele,
					   moth->maternal_allele,
					   maternal_allele);
      }
      else 
      {
	 prob *= sd.set_allele(maternal_allele);
	 set_maternal_allele = 1; 
      }

      p = (pers*) get_child();
      if (is_male()) 
	while (p) {
	  if (p->is_processed) 
	    prob *= sd.inherit_prob_male(paternal_allele,
					 maternal_allele, 
					 p->paternal_allele);
	  p = (pers*) p->get_paternal_sibling();
	}
      else 
	while (p) {
	  if (p->is_processed)
	    prob *= sd.inherit_prob_female(paternal_allele,
					   maternal_allele, 
					   p->maternal_allele);
	  p = (pers*) p->get_maternal_sibling();
	}
      if (prob > 0) {
	pers* pr = container_cutset()->get_next_pers(this);
	if (pr) prob *= pr->execute_cutset_part
	  (sd, index +
	   sd.number_of_alleles()*paternal_allele +
	   maternal_allele);
	else prob *= container_cutset()->execute_cutset
	  (sd, index +
	   sd.number_of_alleles()*paternal_allele +
	   maternal_allele);
	sum += prob;
      }
      if (set_paternal_allele) sd.unset_allele(paternal_allele); 
      if (set_maternal_allele) sd.unset_allele(maternal_allele); 
    }
  is_processed = 0;
  return sum;
}

void pers::print(int indent) {
// Removed 2012-03-08
//  for (int i=0; i<indent; i++) cout<<' ';
//  cout<<"Pers: ";
  printName();
}

void pers::print(ostream& s) {
  pcopy::print(s);
}

void cutset::separate_branches() {
  // We assume this cutset has exactly one branch.
  // We want to separate it into several branches so that 
  // the invariants above are fulfilled. 
  branch* br;
  Link* lk;
  branch* oldbranch = get_first_branch();
  while (!oldbranch->empty()) {
    br = new branch;
    branch_add(br);
    lk = oldbranch->get_first();
    oldbranch->remove(lk);
    br->add(lk);
    lk->collect_from(oldbranch);
  }
  branch_remove(oldbranch);
  delete oldbranch;
}
/*
void cutset::makeCutsets()
{
   separate_branches();

   //We collect one person from each connected component of
   //the graph, as the branching system may be changed by 
   //the establishCutset calls via makeCutsets.
   int n = branch_list::n_elements();
   int i; 
   pers** arr = new pers*[n];
   branch* br = get_first_branch();
   for (i=0; i<n; i++)
   {
      arr[i] = (pers*) br->get_first();
      br = get_next_branch(br);
   }
   for (i=0; i<n; i++)
      arr[i]->makeCutsets();
   delete[] arr;
}
*/
Link* cutset::find_relative_in_branch(branch* br) {
  pers* p = get_first_pers();
  pers* q;
  while (p) {
    q = 0;
    while ((q = (pers*) p->get_next_relative(q))) {
      if (q->get_owner_branch()==br) {
	if (q->container_cutset()) return q->container_cutset();
	else return q;
      }
    }
    p = get_next_pers(p);
  }
  return 0;
}

void cutset::collect_from(branch* oldbranch) {
  pers* p = get_first_pers();
  while (p) {
    p->collect_from(oldbranch);
    p = get_next_pers(p);
  }
}

int cutset::add_tables(int n_alleles) {
  int tablesize = 1;
  for (int i = 2*pers_list::n_elements(); i>0; i--) {
    if (tablesize > MAX_TABLE_SIZE/n_alleles) return 1;
    tablesize *= n_alleles;
  }
  tab = (double*) calloc(tablesize, sizeof(double));
  if (tab==0) return 1;
  for (tablesize--; tablesize>=0; tablesize--) tab[tablesize] = -1;
  branch* br = get_first_branch();
  while (br) {
    if (br->add_tables(n_alleles)) return 1;
    br = get_next_branch(br);
  }
  return 0;
}

void cutset::remove_tables() {

  ////////////////////////////////////////////////////////
  // THE COMMAND BELOW HAS BEEN CHANGED IN ORDER TO REMOVE 
  // A BUG CONCERNING MEMORY MANAGEMENT. 
  // The change is according to a suggestion by Prof. Brian Ripley. 
  // He writes: 
  // You need to use
  // either C or C++ methods of (de)allocation, not mix them.  
  //
  // changed 2014-08-01 by Petter Mostad
  //////////////////////////////////////////////////

  free(tab); //delete[] tab;

  ///////////////////////////////////////////////
  ///////////////////////////////////////////////


  branch* br = get_first_branch();
  while (br) {
    br->remove_tables();
    br = get_next_branch(br);
  }
}

void cutset::remove_data() {
  pers* pr = get_first_pers();
  while (pr) {
    pr->remove_data();
    pr = get_next_pers(pr);
  }
  branch* br = get_first_branch();
  while (br) {
    br->remove_data();
    br = get_next_branch(br);
  }
}

double cutset::execute_cutset(systemdata& sd, int index) {
  if (tab[index]<0) {//It is the first time the members of this cutset
    //has been set to the values indicated by the index.
    //Thus we must now compute the conditional probability of the branches.
    double result = 1;
    branch* br = get_first_branch();
    while (br) {
      result *= br->get_first()->execute(sd);
      br = get_next_branch(br);
    }
    tab[index] = result;
  }
  Link* lk = container_branch()->get_next(this);
  if (lk) return tab[index] * lk->execute(sd);
  else return tab[index];
}

void cutset::print(int indent) {
// Removed 2012-03-08
//  int i;
//  for (i=0; i<indent; i++) cout<<' ';
//  cout<<"CUTSET\n";
//  indent += 2;
//  for (i=0; i<indent; i++) cout<<' ';
//  cout<<"PERS in CUTSET:\n";
//  pers_list::print(indent);
//  for (i=0; i<indent; i++) cout<<' ';
//  cout<<"BRANCHES in CUTSET:\n";
  branch_list::print(indent);
}

void cutset::print(ostream& s) {
  pers* pr = get_first_pers();
  if (pr) {
    pr->print(s);
    pr = get_next_pers(pr);
    while (pr) {
      s<<", ";
      pr->print(s);
      pr = get_next_pers(pr);
    }
    s<<'\n';
  }
}

/*
void pers::establishCutset()
{
   cutset* currcut = new cutset;
   branch* currbranch = container_branch();
   
   pers* pr = this;
   do {
      currbranch->remove(pr);
      currcut->pers_add(pr);
      pr = (pers*) pr->getNext();
   } while (pr!=this);
   
   cutset* oldcutset = currbranch->container_cutset();
   oldcutset->branch_remove(currbranch);
   branch* newbranch = new branch;
   oldcutset->branch_add(newbranch);
   newbranch->add(currcut);
   Link* lk;
   if (oldcutset->pers_list::n_elements()==0) 
      lk = currbranch->get_first();
   else 
      lk = oldcutset->find_relative_in_branch(currbranch);
   if (lk) {
      currbranch->remove(lk);
      newbranch->add(lk);
      lk->collect_from(currbranch);
   }
// Removed 2012-03-08
//   cerr<<"Cutset: ";
//   currcut->print(cerr);
   currcut->branch_add(currbranch);
   currcut->separate_branches();
}
*/




