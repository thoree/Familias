/* $Id: odds.C,v 1.8 2001/04/08 14:04:59 mostad Exp $ */

#include "family.h"
#include "odds.h"
#include "person.h"
#include "cutset.h"
#include "special.h"

odds::odds(person* p, int separateComponents) {
  primcut = new cutset;
  currcut = 0;
  currbranch = 0;
  cutset_must_end = 0;
  branch* br = new branch;
  primcut->branch_add(br);
  
  while (p) {
    br->add(new pers(p, 0));
    p = p->get_next();
  }

  pers* q = (pers*) br->get_first();
  while (q) {
    q->set_relatives();
    q = (pers*) br->get_next(q);
  }

  collapsed_pers = 0;

  if (separateComponents) primcut->separate_branches();


//  q = (pers*) br->get_first();
//  while (q) {
//     q->initializeNode();
//     q = (pers*) br->get_next(q);
//  }
//  primcut->makeCutsets();
}

odds::odds(person* p, person* coll1, person* coll2) {
  primcut = new cutset;
  currcut = 0;
  currbranch = 0;
  cutset_must_end = 0;
  branch* br = new branch;
  primcut->branch_add(br);

  while (p) {
    br->add(new pers(p, 1));
    p = p->get_next();
  }
  
  pers* q = (pers*) br->get_first();
  while (q) {
    q->set_collapsed_relatives();
    q = (pers*) br->get_next(q);
  }

  collapsed_pers = coll1->get_collapsed_alias();
  pers* p2 = coll2->get_collapsed_alias();
  br->remove(p2);
  //The following routine also moves the collapsed_alias 
  //pointer of p2->alias:
  collapsed_pers->add_relatives_from(p2);
  delete p2;

  primcut->separate_branches();
//  q = (pers*) br->get_first();
//  while (q) {
//     q->initializeNode();
//     q = (pers*) br->get_next(q);
//  }
//  primcut->makeCutsets();
}
  
odds::~odds() {
  delete currcut;
  delete primcut;
}

int odds::add_data(systemdata& sd, pers* p, 
		   int allele1, int allele2, 
		   int info, int& error) 
{
   return p->add_data(sd, allele1, allele2, info, error);
}

void odds::remove_data() { primcut->remove_data();}

int odds::add_person_to_cutset(char* name, pers* pe, 
			       int info, int& error) 
{
   if (cutset_must_end) 
   { //(see below)
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The persons identified in the odds statement\n"
//	    "cannot belong to two different cutsets, unless the last\n"
//	    "of these cutsets contains only one person.\n";
      error = 1; 
      return 1;
   }
   if (pe->container_cutset()) 
   {
      if (pe->container_cutset()==currcut) 
      {
	 return 0;
      }
      //We silently accept several occurences of the same person in one cutset.
      //(This may be convenient if the person is the collapsed person, 
      //appearing under his/her two names.)
      //At this point, we know that the previous occurrence of "pe" 
      //was in a different cutset. This is always illegal UNLESS: This person
      //is the collapsed person, and the current cutset contains only this
      //person. In this case, the current cutset is silently ignored.
      if (pe==collapsed_pers) 
      {
	 if (currcut) 
	 {
// Removed 2012-03-08
//	    if (info>0)
//	       cout<<"ERROR: The persons identified in the odds statement\n"
//		  "cannot belong to two different cutsets, unless the last\n"
//		  "of these cutsets contains only one person.\n";
	    error = 1; 
	    return 1;
	 }
	 //We provisionally accept the cutset (and ignore it), pending 
	 //confirmation that the cutset contains only this person:
	 cutset_must_end = 1; 
	 return 0;
      } 
      else 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    cout<<"ERROR: The person \""<<name
//		<<"\" cannot belong to two different cutsets.\n";
	 error = 1; 
	 return 1;
      }
   }
   if (currcut==0) 
   {
      currcut = new cutset;
      currbranch = pe->container_branch();
   } 
   else 
      if (pe->container_branch() != currbranch) 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    cout<<"ERROR: The person \""<<name
//		<<"\" is not in the same component\n"
//		<<"of the pedigree as the rest of the cutset.\n";
	 error = 1; 
	 return 1;
      }
   currbranch->remove(pe);
   currcut->pers_add(pe);
   return 0;
}

void odds::end_cutset(int info, int&) 
{
   Link* lk;
   pers* pr;
   if (cutset_must_end) 
   {
      //We merely confirm that the cutset is discarded:
// Removed 2012-03-08
//      if (info>1)
//	 cout<<"Cutset with odds person removed.\n";
      cutset_must_end = 0;
      return;
   }

   if (currbranch->empty()) 
   {
      //Unnecessary cutset.
// Removed 2012-03-08
//      if (info>1) 
//      {
//	 cout<<"Ineffectual cutset removed: ";
//	 currcut->print(cout);
//      }
      pr = currcut->get_first_pers();
      while (pr) 
      {
	 currcut->pers_remove(pr);
	 currbranch->add(pr);
	 pr = currcut->get_first_pers();
      }
      delete currcut;
   } 
   else 
   {
      cutset* oldcutset = currbranch->container_cutset();
      oldcutset->branch_remove(currbranch);
      branch* newbranch = new branch;
      oldcutset->branch_add(newbranch);
      newbranch->add(currcut);
      
      if (oldcutset==primcut) lk = currbranch->get_first();
      else lk = oldcutset->find_relative_in_branch(currbranch);
      if (!lk) 
      {
// Removed 2012-03-08
//	 if (info>1) 
//	 {
//	    cout<<"Cutset OK: ";
//	    currcut->print(cout);
//	 }
	 currcut->branch_add(currbranch);
	 currcut->separate_branches();
      } 
      else 
      {
	 do
	 {
	    currbranch->remove(lk);
	    newbranch->add(lk);
	    lk->collect_from(currbranch);
	    
	    if (oldcutset==primcut) break; 
	    else lk = oldcutset->find_relative_in_branch(currbranch); 
	 } while (lk); 
	 if (currbranch->empty()) 
	 {
// Removed 2012-03-08
//	    if (info>1) 
//	    {
//	       cout<<"Ineffectual cutset removed: ";
//	       currcut->print(cout);
//	    }
	    pr = currcut->get_first_pers();
	    while (pr) 
	    {
	       currcut->pers_remove(pr);
	       newbranch->add(pr);
	       pr = currcut->get_first_pers();
	    }
	    newbranch->remove(currcut);
	    delete currcut;
	    delete currbranch;
	 } 
	 else 
	 {
// Removed 2012-03-08
//	    if (info>1) {
//	       cout<<"Cutset OK: ";
//	       currcut->print(cout);
//	    }
	    currcut->branch_add(currbranch);
	    currcut->separate_branches();
	 }
      }
   }
   currcut = 0;
   currbranch = 0;
}

double odds::execute(systemdata& sd, int& err) {
  if (err) return 1;
  if (primcut->add_tables(sd.number_of_alleles())) {
    primcut->remove_tables();
    err = 1;
    return 1;
  }
  primcut->sort();
 
  //cout<<"DATA STRUCTURE:\n";
  //primcut->print(4);
  //cout<<"***************\n";

  double result = 1;
  branch* br = primcut->get_first_branch();
  while(br) {
    result *= br->get_first()->execute(sd);
    br = primcut->get_next_branch(br);
  }

  primcut->remove_tables();
  return result;
}




