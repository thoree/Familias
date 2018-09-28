/* $Id: Pedigree.h,v 1.9 2001/04/08 14:04:56 mostad Exp $ */
#ifndef __PEDIGREE_H_
#define __PEDIGREE_H_

#include <fstream>

#include "Element.h"
#include "special.h"

class pater; 

class Pedigree : public Element
{
   public:

      Pedigree(int nNamedPersons, 
	       int nExtraFemales, 
	       int nExtraMales, 
	       int* male, //dimension nNamedPersons
	       int* parent); //dimension nTotal*nTotal

      Pedigree(int nNamedPersons, 
	       int* male, //dimension nNamedPersons
	       int* isChild, //dimension nNamedPersons
	       ifstream& is, 
	       int& OK); 
	       
      ~Pedigree(); 

      void write(ofstream& os) const; //OK

      void print() const; //OK

      int isEqualTo(Pedigree* q) const; //OK

      int unsafeEquals(Pedigree* pg) const; 

      int computeInbreeding() const; //OK
      
      int computePromiscuity() const;  //OK

      int computeGenerations(int* isChild) const; //OK

      //When calling this, we assume that all prunable persons
      //have already been removed. 
      //fixedParent is a matrix of dim. nNamedPersons*nNamedPersons, and
      //names is an array of length nNamedPersons. 
      //The pat-object only contains these persons, and exactly those
      //relations indicated in fixedParent. 
      //namePrefix is a string such that all names starting with this 
      //string and continuing with an integer is not in conflict with
      //the names of named persons. 
      //if info==1, details of the computation is written to screen
	  //AN ARRAY WITH RESULTS FOR INDIVIDUAL SYSTEMS IS RETURNED
      double* computeProbability(pater& pat, 
				int* fixedParent, 
				char** names, 
				char* namePrefix, 
				int makeCutsets, 
				int info) const; 

//       double computeProbability(pater* pat, 
// 				char** names, //length nTotal
// 				int* isChild, //length nTotal
// 				int info, 
// 				int& error) const; 

      void getPedigree(int* matrix) const; 

      void getParents(int* moth, int* fath) const; 

      //The array setOfPersons, of length nNamedPersons, 
      //indicates a subset. The routine returns 1 if any of 
      //these people have children
      int hasChildren(int* setOfPersons) const; 

      int getPedigreeSize() const; 

      int getNumberOfExtraFemales() const; 

      int getNumberOfExtraMales() const; 

      int onStandardForm() const; 

      //The routine permutes the extra persons to 
      //get a pedigree which is on standard form 
      //according to the routine above. 
      //NOTE: When either of "prune()" or "pruneAndRemove()"
      //is run, and then the routine below, the pedigree
      //has the property that no further non-trivial permutation of 
      //its extra persons will yield a pedigree which is
      //"onStandardForm". 
      void changeToStandardForm(); 

      void addPerson(int male); 

      void addExtraPerson(int male); 

      void removePerson(int index); 

      void addRelation(int parentindex,
		       int childindex, 
		       int& error); 

      void removeRelation(int parentindex, 
			  int childindex); 

      //Remove unnecessary relations, but not unnecessary
      //extra persons. 
//      void prune(); //OK

      //Remove unnecessary extra persons also. 
      void pruneAndRemove(); //OK

   private:

      //The persons in the pedigree are listed first
      //with "named" persons, then with the extra females, 
      //and then with the extra males. 
      int nTotal; 
      int nNamedPersons; //included in nTotal

      int* male; //array of length nTotal

      //An array of length nTotal, of arrays of length nTotal. 
      // parent[i][j]==1 iff i is a parent of j: 
//      int** parent;

      //Two arrays of lenghts nTotal. 
      //Each gives the index of the father, resp. mother, 
      //if it is in the pedigree. If not, the index is -1. 
      int* father; 
      int* mother; 

//Private methods: 
      
      //Return a new array with indicators for pruing. 
      //The array is 1 for persons who are "extra", and who
      //are not necessary in the pedigree. 
      int* getPruning() const; //OK

      //Get a pruning array with indicators for cutsets. 
      int* getCutsets() const; //OK

      //Mark this node (i), and all of its non-pruned and
      //non-marked connected neighbours
      //in the undirected graph, with 1. 
      void mark(int i, int* pruned, int* marks) const; //OK

      int hasCommonAncestor(int person1, int person2) const; //OK

      int isAncestor(int person1, int person2) const; //OK
      
      //Return the maximal number of generations one may go back
      //from this person, to get a person who is not "Extra". 
      //Return -1 if there are no such persons, AND person i 
      //is also an "Extra". 
      int getMaxGenerations(int i) const; //OK
};


#endif

