/* $Id: family.C,v 1.7 2001/04/08 14:04:59 mostad Exp $ */

#include <cstdlib>
#include <cstring>

#include "table.h"
#include "person.h"
#include "family.h"
#include "odds.h"
#include "special.h"

person* family::get_person(char* name) {
  person* q = p;
  while (q) {
    if (strcmp(name, q->name())==0) return q;
    q = q->get_next();
  }
  return 0;
} 

int family::totalFamilySize()
{
   int result = 0; 
   person* q = p; 
   while (q)
   {
      result++;
      q = q->get_next(); 
   }
   return result; 
}

family::~family() {
  if (p) {
    person* q;
    while ((q = p->get_next())) {
      delete p;
      p = q;
    }
    delete p;
  }
  delete oddsobj;
}

int family::oddsOK() {
  if (oddsobj) {
    person* p1;
    person* p2;
    if ((p1 =  oddsobj->pers1->get_father()) && 
	(p2 =  oddsobj->pers2->get_father()) &&
	(p1 != p2)) return 0;
    if ((p1 =  oddsobj->pers1->get_mother()) && 
	(p2 =  oddsobj->pers2->get_mother()) && 
	(p1 != p2)) return 0;
    if (oddsobj->pers1->has_ancestor(oddsobj->pers2)) return 0;
    if (oddsobj->pers2->has_ancestor(oddsobj->pers1)) return 0;
  } 
  return 1;
}

int family::add_person(int male, char* name, int info, int& error) 
{
   person* q = get_person(name);
   if (q) 
   {
      if (male==q->is_male()) 
      {
// Removed 2012-03-08
//	 if (info>1) 
//	    if (male) 
//	       cout<<"The male \""<<name<<"\" is already defined.\n";
//	    else cout<<"The female \""<<name<<"\" is already defined.\n";
      } 
      else 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    if (male) cout<<"ERROR: \""<<name
//			  <<"\" already exists as a female!\n";
//	    else cout<<"ERROR: \""<<name
//		     <<"\" already exists as a male!\n";
	 error = 1; 
      }
      delete[] name;
      return 0;
   }
   if (p) p->add_at_end(new person(name, male));
   else p = new person(name, male);
// Removed 2012-03-08
//   if (info>1) 
//      if (male) cout<<"The male \""<<name<<"\" added to the data.\n";
//      else cout<<"The female \""<<name<<"\" added to the data.\n";
   return 1;
}

int family::isMale(char* name)
{
   person* q = get_person(name); 
   if (q) 
   {
      delete[] name; 
      return q->is_male(); 
   }
   delete[] name; 
   return 0; 
}

int family::inFamily(char* name)
{
   person* q = get_person(name); 
   delete[] name; 
   return q!=0; 
}

int family::remove_person(int male, char* name, person*& q, 
			  int info, int& error) 
{
   q = get_person(name);
   if (q) 
      if (male==q->is_male()) 
      {
	 if (q==p) p = p->get_next();
	 else p->remove_next(q);
	 //Extract q from all family relation lists:
	 if (q->get_mother()) q->remove_mother();
	 if (q->get_father()) q->remove_father();
	 q->discard_children();
	 if (oddsobj && (oddsobj->pers1==q || oddsobj->pers2==q)) 
	 {
	    delete oddsobj;
	    oddsobj = 0;
	 }
// Removed 2012-03-08
//	 if (info>1) 
//	    if (male) cout<<"Removing the male \""<<name
//			  <<"\" from the data.\n";
//	    else cout<<"Removing the female \""
//		     <<name<<"\" from the data.\n";
	 delete[] name;
	 return 1;
      } 
      else 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    if (q->is_male()) cout<<"ERROR: \""<<name<<"\" is a male!\n";
//	    else cout<<"ERROR: \""<<name<<"\" is a female!\n";
	 error = 1; 
	 q = 0;
	 delete[] name;
	 return 0;
      }
// Removed 2012-03-08
//   if (info>0)
//      cout<<"ERROR: The person \""<<name<<"\" could not be found.\n";
   error = 1; 
   delete[] name;
   return 0;
}

void family::write_persons(int male, ostream& out) {
   if (male) out<<"males    "; else out<<"females  ";
   person* q = p;
   int done_one = 0;
   while (q) {
      if (male==q->is_male()) {
	 if (done_one) out<<", "; else done_one = 1;
	 out<<q->name();
      }
      q = q->get_next();
   }
   out<<'\n';
}

int family::add_relation(char* parent, char* child, int info, int& error) 
{
   person* par = get_person(parent);
   if (!par) { 
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The parent \""<<parent<<"\" could not be found.\n";
      error = 1; 
      delete[] parent; 
      delete[] child;
      return 0;
   }
   person* ch = get_person(child);
   if (!ch) {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The child \""<<child<<"\" could not be found.\n";
      error = 1; 
      delete[] parent; 
      delete[] child;
      return 0;
   }
   if (par==ch) {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The child \""<<child<<"\" cannot be its own parent!\n";
      error = 3; 
      delete[] parent; 
      delete[] child;
      return 0;
   }
   if (par->has_ancestor(ch)) {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The child \""<<child
//	     <<"\" is an ancestor of the parent \""<<parent<<"\"!\n";
      error = 4; 
      delete[] parent; 
      delete[] child;
      return 0;
   }
   if (par->is_male()) {
      if (ch->get_father() == par) 
      {
	 delete[] parent; 
	 delete[] child;
	 return 0;
      }
      else if (ch->get_father()) 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    cout<<"ERROR: The child \""<<child<<"\" already has a father.\n";
	 error = 5; 
	 delete[] parent; 
	 delete[] child;
	 return 0;
      }
   }
   else 
   {
      if (ch->get_mother() == par) 
      {
	 delete[] parent; 
	 delete[] child;
	 return 0;
      }
      else if (ch->get_mother()) {
// Removed 2012-03-08
//	 if (info>0)
//	    cout<<"ERROR: The child \""<<child<<"\" already has a mother.\n";
	 error = 5; 
	 delete[] parent; 
	 delete[] child;
	 return 0;
      }
   }
   //We know ch does not have a parent of the relevant type, and that 
   //creation of the relation does not lead to a loop.
   ch->add_parent(par);
   if (!oddsOK()) 
   {
      if (par->is_male()) 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    cout<<"ERROR: Adding \""<<parent<<"\" as the father of \""
//		<<child<<"\" is in conflict with the current odds question.\n";
	 error = 1; 
	 ch->remove_father();
      } 
      else 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    cout<<"ERROR: Adding \""<<parent<<"\" as the mother of \""
//		<<child<<"\" is in conflict with the current odds question.\n";
	 error = 1; 
	 ch->remove_mother();
      }
      delete[] parent; 
      delete[] child;
      return 0;
   }
// Removed 2012-03-08
//   if (info>1) 
//      if (par->is_male())
//	 cout<<"Added to data: \""<<parent<<"\" is the father of \""
//	     <<child<<"\".\n";
//       else
//      cout<<"Added to data: \""<<parent
//	  <<"\" is the mother of \""<<child<<"\".\n";
   delete[] parent; 
   delete[] child;
   return 1;
}

int family::remove_relation(char* parent, char* child, 
			    int info, int& error) 
{
   person* par = get_person(parent);
   if (!par) 
   { 
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The parent \""<<parent<<"\" could not be found.\n";
      error = 1; 
      delete[] parent; 
      delete[] child;
      return 0;
   }
   person* ch = get_person(child);
   if (!ch) 
   {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The child \""<<child<<"\" could not be found.\n";
      error = 1; 
      delete[] parent; 
      delete[] child;
      return 0;
   }
   if (par->is_male()) 
   {
      if (ch->get_father() == par) 
      {
	 ch->remove_father(); 
// Removed 2012-03-08
//	 if (info>1) 
//	    cout<<"Removed from data: \""<<parent
//		<<"\" is no longer the father of \""<<child<<"\".\n";
	 delete[] parent; 
	 delete[] child;
	 return 1;
      } 
      else 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    cout<<"ERROR: \""<<parent<<"\" is not the father of \""
//		<<child<<"\".\n";
	 error = 1; 
	 delete[] parent; 
	 delete[] child;
	 return 0;
      }
   } 
   else 
   {
      if (ch->get_mother() == par) {
	 ch->remove_mother();
// Removed 2012-03-08
//	 if (info>1)
//	    cout<<"Removed from data: \""<<parent
//		<<"\" is no longer the mother of \""<<child<<"\".\n";
	 delete[] parent; 
	 delete[] child;
	 return 1;
      } 
      else 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    cout<<"ERROR: \""<<parent<<"\" is not the mother of \""
//		<<child<<"\".\n";
	 error = 1;
	 delete[] parent; 
	 delete[] child;
	 return 0;
      }
   }
}

int family::remove_possible_relation(char* parent, char* child) {
  person* par = get_person(parent);
  if (!par) { 
    delete[] parent; delete[] child;
    return 0;
  }
  person* ch = get_person(child);
  if (!ch) {
    delete[] parent; delete[] child;
    return 0;
  }
  if (par->is_male()) {
    if (ch->get_father() == par) {
      ch->remove_father(); 
      delete[] parent; delete[] child;
      return 1;
    } else {
      delete[] parent; delete[] child;
      return 0;
    }
  } else {
    if (ch->get_mother() == par) {
      ch->remove_mother();
      delete[] parent; delete[] child;
      return 1;
    } else {
      delete[] parent; delete[] child;
      return 0;
    }
  }
}

int family::add_odds(char* person1, char* person2, 
		     int info, int& error) 
{
   person* p1 = get_person(person1);
   if (!p1) 
   { 
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: Could not find person \""<<person1<<"\".\n";
      error = 1; 
      delete[] person1; 
      delete[] person2;
      return 0;
   }
   person* p2 = get_person(person2);
   if (!p2) 
   { 
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: Could not find person \""<<person2<<"\".\n";
      error = 1; 
      delete[] person1; 
      delete[] person2;
      return 0;
   }
   if (p1==p2) 
   {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The two persons in the odds command are identical.\n";
      error = 1; 
      delete[] person1; 
      delete[] person2;
      return 0;
   }
   if (p1->is_male()!=p2->is_male()) 
   {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: \""<<person1<<"\" and \""<<person2
//	     <<"\" have opposite sex!\n";
      error = 1; 
      delete[] person1; 
      delete[] person2;
      return 0;
   }
   oddsobject* old_odds = oddsobj;
   oddsobj = new oddsobject;
   oddsobj->pers1 = p1;
   oddsobj->pers2 = p2;
   if (oddsOK()) 
   {
// Removed 2012-03-08
//      if (info>1)
//	 cout<<"The odds question \""<<person1<<"\" = \""<<person2
//	     <<"\" has been added to the data.\n";
      delete old_odds;
      delete[] person1; 
      delete[] person2;
      return 1;
   } 
   else 
   {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The attempted odds command is not compatible with "
//	     <<"the family relations.\n";
      error = 1; 
      delete oddsobj;
      oddsobj = old_odds;
      delete[] person1; 
      delete[] person2;
      return 0;
   }
}

int family::remove_odds(int info, int&) 
{
   if (oddsobj) 
   {
// Removed 2012-03-08
//      if (info>1) 
//	 cout<<"The odds question \""<<oddsobj->pers1->name()
//	     <<"\" = \""<<oddsobj->pers2->name()<<"\" has been removed.\n";
      delete oddsobj;
      oddsobj = 0;
      return 1;
   }
   return 0;
}

void family::write(ostream& out) {
  if (p) {
    out<<separator<<"FAMILY STRUCTURE\n"<<separator<<'\n';
    write_persons(0,out);
    write_persons(1,out);
    
    table tab("parents","children");
    person* q = p;
    while (q) {
      q->write_parents(tab);
      q = q->get_next();
    }
    if (p) p->clear_parents();
    
    if (tab.contains_data()) {
      out<<"\nFamily relations:\n";
      tab.printout(out);
    } else out<<"\nNo family relations registered.\n";
    
    if (oddsobj) 
      out<<"\nQuestion for odds computation: is "
	 <<oddsobj->pers1->name()<<" = "
	 <<oddsobj->pers2->name()<<"?\n";
  }
}

int family::add_person_to_cutset(char* name, int info, int& error) 
{
   if (od==0) 
   {
      od = new odds(get_first(), 1);
      if (oddsobj) collapse_od = new odds(get_first(), 
					  oddsobj->pers1,
					  oddsobj->pers2);
   }
   person* prs = get_person(name);
   if (prs==0) 
   {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The person \""<<name<<"\" not found.\n";
      error = 1; 
      delete[] name;
      return 1;
   }
   if (od->add_person_to_cutset(name, prs->get_alias(), info, error)) {
      delete[] name;
      return 1;
   }
   if (oddsobj && collapse_od->add_person_to_cutset
       (name, prs->get_collapsed_alias(), info, error)) 
   {
      delete[] name;
      return 1;
   }
   delete[] name;
   return 0;
}

void family::end_cutset(int info, int& error) 
{
   if (od==0) 
   {
// Removed 2012-03-08
     //      if (info>0)
     //	 cout<<"INTERNAL ERROR in family::end_cutset.\n";
      error = 1; 
// Removed 2012-03-08
      //      exit(1);
   }
   if (oddsobj) 
   {
      //A cutset should be warned to be bad only if it is bad in the 
      //collapsed case. 
      int tmpError = 0; 
      od->end_cutset(0, tmpError);
      collapse_od->end_cutset(info, error);
   } else od->end_cutset(info, error);
}

int family::add_data(systemdata& sd, person* prs, 
		     int allele1, int allele2, 
		     int info, int& error) 
{
  if (od==0) {
    od = new odds(get_first(), sd.useCutsets());
    if (oddsobj) collapse_od = new odds(get_first(), 
					oddsobj->pers1,
					oddsobj->pers2);
  }
  //Data will never be added twice to one person in the uncollapsed case:
  od->add_data(sd, prs->get_alias(), allele1, allele2, 
	       info, error);
  if (oddsobj) 
    return collapse_od->add_data(sd, prs->get_collapsed_alias(), 
				 allele1, allele2, 
				 info, error);
  return 0;
}

double family::execute(systemdata& sd, int& err) {
  if (od==0) {
    od = new odds(get_first(), sd.useCutsets());
    if (oddsobj) collapse_od = new odds(get_first(), 
					oddsobj->pers1,
					oddsobj->pers2);
  }
  if (oddsobj) return collapse_od->execute(sd, err)/od->execute(sd, err);
  else return od->execute(sd, err);
}

void family::remove_data() {
  if (od) od->remove_data();
  if (collapse_od) collapse_od->remove_data();
}

void family::remove_cutsets() {
  if (od) {
    person* q = p;
    while (q) {
      q->clear_alias();
      q->clear_collapsed_alias();
      q = q->get_next();
    }
    delete od;
    od = 0;
    delete collapse_od;
    collapse_od = 0;
  }
}





