/* $Id: PedigreeList.h,v 1.5 2001/04/08 14:04:57 mostad Exp $ */
#ifndef __PEDIGREELIST_H_
#define __PEDIGREELIST_H_

class GroupOfPersons; 
class Pedigree; 
class pater; 

class PedigreeList 
{
   public:
      PedigreeList(int info = 0); 

      ~PedigreeList(); 

      int getNumberOfPedigrees(); 

      Pedigree* getPedigree(int index); 

      //Add a person at the end of the list of named persons. 
      //Add it also to all pedigrees. 
      void addPerson(int male); 

      //A person is removed; also in all pedigrees in the list. 
      void removePerson(int index); 

      //Add a relation to all pedigrees. Pedigrees where this creates
      //a conflict are removed. A list of indices of removed pedigrees
      //is returned. 
      void addFixedRelation(int parentindex, 
			    int childindex, 
			    int* removed); 

      //Remove a relation from all pedigrees. 
      void removeFixedRelation(int parentindex, 
			       int childindex); 

      int isFixedParent(int parentindex, int childindex) const; 
      
      //Add a pedigree at the end of the list. 
      //The new pedigree gets exactly the fixed relations. 
      void addPedigree(int nExtraFemales, 
		       int nExtraMales); 

      //Add the given pedigree at the end of the list
      //NOTE: It is not checked, but simply assumed, that
      //the pedigree contains any fixed relations
      void addPedigree(Pedigree* p); 
		      
      //Remove the pedigree with the given index
      void removePedigree(int index); 
      
      //Remove from the list all pedigrees that are equivalent
      //to previous pedigrees on the list. 
      void removeEquivalentPedigrees(int* removed); 
      
      //Add to the end of the list all pedigrees that may be constructed
      //with the extra persons, provided it is not equivalent with
      //any pedigree previously on the list. 
      //possibleParent is a matrix indicating parent possibility
      //based on YOB data and fertility Age; 
      void generatePedigrees(int nExtraFemales, 
			     int nExtraMales, 
			     int* possibleParent); 

      int computePrior(double generationsParameter, 
		       int maxGenerations, 
		       double inbreedingParameter,
		       double promiscuityParameter, 
		       int* isChild, 
		       double* probabilities); 

      int computePosterior(pater& pat, 
			   int makeCutsets, 
			   char** names, 
			   double* probabilities, 
			   double* likelihoods); 
      
   private:

      int nNamedPersons; 
      int* male; 
      int* fixedParent; 
      //matrix: fixedParent[i+j*nNamedPersons]==1 iff i is a parent of j. 

      int nPedigrees; 
      Pedigree* first; //linked list. 
      Pedigree* last; 

      //Used when generating pedigrees with "familias"
      int info; 

      //Recursive procedure: All persons below i
      //have been given suggested parents; do so also for 
      //i and those above. Return finished pedigrees in list. 
      void generateParentsForPerson(int i, 
				    int* parent,
				    int nTotal, 
				    int nExtraFemales, 
				    int nExtraFemalesUsed, 
				    int nExtraMales, 
				    int nExtraMalesUsed, 
				    int* possibleParent); 

      //Used in conjunction with procedure above: 
      void generateFatherForPerson(int i, 
				   int* parent,
				   int nTotal, 
				   int nExtraFemales, 
				   int nExtraFemalesUsed, 
				   int nExtraMales, 
				   int nExtraMalesUsed, 
				   int* possibleParent); 

      void removePedigrees(int* removed); 

      static int isAncestor(int childindex, 
			    int parentindex, 
			    int* isParent, 
			    int nTotal); 

      static int isPossibleParent(int j, 
				  int i, 
				  int nTotal, 
				  int* parent, 
				  int* possibleParent); 

      static int checkParents(int j, 
			      int i, 
			      int nGenerations, 
			      int nTotal, 
			      int* parent, 
			      int* possibleParent); 
      
      static int checkChildren(int j, 
			       int i, 
			       int nGenerations, 
			       int nTotal, 
			       int* parent, 
			       int* possibleParent); 

};


#endif

