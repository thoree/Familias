/* $Id: family.h,v 1.5 2000/08/07 07:44:09 mostad Exp $ */

#ifndef __FAMILY_H_
#define __FAMILY_H_

#include <fstream>
using namespace std; 

class person;
class systemdata;
class odds;

class oddsobject {
public:
  person* pers1;
  person* pers2;
};

class family {
  person* p;
  oddsobject* oddsobj;
  odds* od;
  odds* collapse_od;
public:
  family() : p(0), oddsobj(0), od(0), collapse_od(0) {}
  ~family();
  oddsobject* get_odds() { return oddsobj; }
  
  //The following routine tests whether the current odds
  //object is compatible with the rest of the family data:
  //Test that if both have a mother, it is the same mother,
  //test that if both have a father, it is the same father,
  //test that neither is an ancestor of the other.
  int oddsOK();
  
  //In the following 5 routines, we always assume that the names
  //given contain no illegal characters. 
  //If info==0, no written output. 
  //If info==1, only error messages are written, not information messages. 
  //If info==2, both types of messages are written. 
  //If data is indeed changed, the return value is 1, otherwise 0.
  
  int add_person(int male, char* name, int info, int& error);
  int remove_person(int male, char* name, person*& q, int info, int& error);
      
      int isMale(char* name); 
      int inFamily(char* name); 

  void write_persons(int male, ostream& out);
  int add_relation(char* parent, char* child, int info, int& error);
  int remove_relation(char* parent, char* child, int info, int& error);
      
      int remove_possible_relation(char* parent, char* child); 

  int add_odds(char* person1, char* person2, int info, int& error);
  int remove_odds(int info, int& error);
  void write(ostream& out);
  person* get_first() {return p; }
  person* get_person(char* name);

      int totalFamilySize(); 

  int add_person_to_cutset(char* name, int info, int& error);
  void end_cutset(int info, int& error);
  int add_data(systemdata& sd, person* p, 
	       int allele1, int allele2, 
	       int info, int& error);
  double execute(systemdata& sd, int& err);
  void remove_data();
  void remove_cutsets();
};

#endif



