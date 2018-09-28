/* $Id: FamInterface.h,v 1.11 2001/04/12 16:00:33 mostad Exp $ */

#ifndef __FAMINTERFACE_H_
#define __FAMINTERFACE_H_

#include "pater.h"
#include "PedigreeList.h"

class FamInterface
{
   public:
//	   void GetNumberOfSystems(int* number);

      FamInterface(); 
      ~FamInterface(); 

//      void GetVersion(/*[out]*/ double* version); 

      //In general for errors: 
      //error==0 no error
      //error==1 data error: input parameters not as specified
      //error>1: method specific
      
      //The function recalculates all probabilities. 
	  //Note that the output in the probabilities array is the prior; it must 
	  //be combined with the likelihoods by the calling routine to produce the 
	  //posterior. 
      //First, it is checked if any of the current pedigrees are equivalent. 
      //If so, the redundant ones are marked with redundant[index]==1, 
      //the others are marked with redundant[index]==0. 
      //Pedigrees marked redundant are removed from the lists, and the 
      //indices of other arrays adjusted accordingly. 
      //The "probabilities" are adjusted so that they sum to one. 
      //However, "likelihoods" represent the likelihoods computed.
      //usingDNAObservations: 1 or 0
      //kinship: Must be between 0 (inclusive) and 1. 
      //redundant: array of length numberOfPedigrees, 
      //before removing redundants
      //probabilities: array of length numberOfPedigrees, 
      //after removing redundants, 
      //likelihoods: array of length numberOfPedigrees TIMES the number 
	  //of allele systems (with the allele systems cycling fastest)
	  //(this means that the VB shell can display likelihoods and compute
	  //posterior probabilities for single systems, and subsets of systems)
      //after removing redundants. 
      //error = 2: All pedigrees get zero probability. 
      //Note that pedigrees with zero probability are not removed
      //from the list. This could be changed in the future, if desireable. 
      //Note that the calling routine is responsible for allocating and
      //deallocating all arrays. 
      void GetProbabilities(/*[in]*/ double generationsParameter, 
			    /*[in]*/ int maxGenerations, 
			    /*[in]*/ double inbreedingParameter, 
			    /*[in]*/ double promiscuityParameter, 
			    /*[in]*/ int usingDNAObservations, 
			    /*[in]*/ double kinship, 
			    /*[out]*/ int* redundant, 
			    /*[out]*/ double* probabilities, 
			    /*[out]*/ double* likelihoods, 
			    /*[out]*/ int* error); 
//      void RemoveDNAObservation(/*[in]*/ int indexperson, 
//				/*[in]*/ int indexAlleleSystem, 
//				/*[out]*/ int* error);
      void AddDNAObservation(/*[in]*/ int indexperson, 
			     /*[in]*/ int indexAlleleSystem, 
			     /*[in]*/ int indexAllele1, 
			     /*[in]*/ int indexAllele2, 
			     /*[out]*/ int* error);
      
      //It may be better to use "Edit" instead of "Remove" and then "Add", 
      //as the former operation preserves DNA observations of 
      //alleles that may be registered for this system. 
      //nAlelles must be at least 1. 
      //mutationModel: An integer which should be 0, 1, 2 or 3:
      //Once it is determined that a mutation takes place, 
      //0 means the probability is equal for all other alleles, 
      //1 means the probability is proportional to their frequencies, 
      //2 means the probabilities form a geometric series, depending
      //on distance in the list of alleles,
	  //3 is like 2, but with a stable mutation matrix. 
      //
      // REQUIREMENTS FOR PARAMETERS: 
      // nAlleles >= 1
      // mutationRate >= 0 and mutationRate < 1
      // mutationModel = 0, 1, 2 or 3. 
	  // nPossibilities IS CURRENTLY UNUSED
      // //nPossibilities >= nAlleles (for all mutation models)
      // //nPossibilities > 1 (for all mutation models)
      // mutationRange > 0 (for all mutation models)
      // mutationRange < 1 (for all mutation models)
      // each freqency must be positive
	  // the sum of the frequencies is assumed to be 1 (not checked)
      // //the sum of frequencies <=1
      // //We must have nAlleles == nPossibilities if and only if sum == 1
      // We must also have hasSilentAllele equal to 0 or 1
      // 
      //nPossibilities: a parameter that carries meaning only for 
      //mutationModels 0 and 1. CURRENTLY UNUSED!
      //It then specifies the possible number of alleles, 
      //and must be greater than or equal to nAlleles. 
      //mutationRange: a parameter that carries meaning only for
      //mutationModels 2 and 3. As a dummy, one should input 0.1 here. 
      //frequencies: array of length nAlleles
      //corresponcence: array of length OLD VALUE of nAlleles
      //for this system. It indicates the indices
      //in the new system of the alleles of the
      //old system. If they are not in the new system
      //the value of the array at that index should be -1.
	  //If hasSilentAllele is 1, then the LAST allele in the list
	  //is the silent allele. 
      //Note that the calling routine is responsible for allocation
      //and deallocation of all arrays. 
//      void EditAlleleSystem(/*[in]*/ int indexSystem, 
//			    /*[in]*/ int nAlleles, 
//			    /*[in]*/ double mutationRateFemale,
//			    /*[in]*/ double mutationRateMale,
//			    /*[in]*/ int mutationModelFemale, 
//			    /*[in]*/ int mutationModelMale, 
//			    /*[in]*/ int nPossibilities, 
//			    /*[in]*/ double mutationRangeFemale, 
//			    /*[in]*/ double mutationRangeMale, 
//			    /*[in]*/ double* frequencies, 
//			    /*[in]*/ int* correspondence, 
//				/*[in]*/ int hasSilentAllele, 
//			    /*[out]*/ int* error);
//
//      void RemoveAlleleSystem(/*[in]*/ int index, 
//			      /*[out ]*/ int* error);

      //frequencies: array of length nAlleles
      void AddAlleleSystem(/*[in]*/ int nAlleles, 
			   /*[in]*/ int lOfVector, 
			   /*[in]*/ double* mMatrixFemale, 
			   /*[in]*/ double* mMatrixMale, 
			   /*[in]*/ int sMutationMatrix, 
			   /*[in]*/ int nPossibilities, 
			   /*[in]*/ double* frequencies, 
			   /*[in]*/ int hasSilentAllele, 
			   /*[out]*/ int* index, 
			   /*[out]*/ int* error);

//      //error==2: trying to remove a fixed relation
//      void RemoveRelation(/*[in]*/ int parentindex, 
//			  /*[in]*/ int childindex, 
//			  /*[in]*/ int pedigree, 
//			  /*[out]*/ int* error);

      //error==2: illegal relation, based on YOB or isChild data. 
      //error==3: illegal relation: cycle in pedigree, or duplicate parent. 
      void AddRelation(/*[in]*/ int parentindex, 
		       /*[in]*/ int childindex, 
		       /*[in]*/ int pedigree, 
		       /*[out]*/ int* error);

      //add an extra person to the given pedigree: 
//      void AddExtraPerson(/*[in]*/ int male, 
//			  /*[in]*/ int pedigree, 
//			  /*[out]*/ int* error); 
//
//      //remove the extra person with the given index from the given pedigree: 
//      void RemoveExtraPerson(/*[in]*/ int person, 
//			     /*[in]*/ int pedigree, 
//			     /*[out]*/ int* error); 
//
//      //"matrix" is an array of size n*n, where n
//      //is the total number of persons in this pedigree. 
//      //If person i is the parent of person j, then 
//      //matrix[i+n*j]==1; otherwise matrix[i+n*j]==0. 
//      //Note that the calling function must allocate/deallocate 
//      //the array!
//      void GetPedigree(/*[in]*/ int index, 
//		       /*[out]*/ int* matrix, 
//		       /*[out]*/ int* error);
//
//      //mother and father are arrays of length n, where n
//      //is the total nubmer of persons in the pedigree nr. index. 
//      //The indices of the relevant parents are returned in these
//      //arrays, if no relevant parent is registered, -1 is returned. 
//      //Note that the calling function must allocate/deallocate 
//      //the arrays!
//      void GetParents(/*[in]*/ int index, 
//		      /*[out]*/ int* mother, 
//		      /*[out]*/ int* father, 
//		      /*[out]*/ int* error); 
//
//      //Get the total number of persons in this pedigree: 
//      void GetSizeOfPedigree(/*[in]*/ int index, 
//			     /*[out]*/ int* size, 
//			     /*[out]*/ int* error);
//
//      //Note that even when pedigrees are generated with a 
//      //certain number of extra females, some of the generated
//      //pedigrees may have fewer extra females (as some of the
//      //extras then turned out to be redundant).  
//      void GetNumberOfExtraFemales(/*[in]*/ int pedigree, 
//				   /*[out]*/ int* number, 
//				   /*[out]*/ int* error);
//
//      //Note that even when pedigrees are generated with a 
//      //certain number of extra males, some of the generated
//      //pedigrees may have fewer extra males (as some of the
//      //extras then turned out to be redundant).  
//      void GetNumberOfExtraMales(/*[in]*/ int pedigree, 
//				 /*[out]*/ int* number, 
//				 /*[out]*/ int* error);
//
//      //Functions for specifying pedigrees:  
//      //Each pedigree may have, in addition to the fixed persons, 
//      //a number of anonymous males and a number of 
//      //anonymous females. The extra females are given the first
//      //indices after the fixed persons; then follow the extra
//      //males. 
//
//      void RemovePedigree(/*[in]*/ int index, 
//			  /*[out]*/ int* error);

      //Adding a pedigree with only the fixed relations: 
      void AddPedigree(/*[in]*/ int nExtraFemales, 
		       /*[in]*/ int nExtraMales, 
		       /*[out]*/ int* index, 
		       /*[out]*/ int* error);

//      void GetNumberOfPedigrees(/*[out]*/ int* number);
//
//      //Generating a set of pedigrees. 
//      //The equivalent pedigrees among the existing set are first
//      //removed, and the removed ones are indicated in the "removed" array. 
//      //Then, new pedigrees are added to the end of the list. 
//      void GeneratePedigrees(/*[in]*/ int nExtraFemales, 
//			     /*[in]*/ int nExtraMales, 
//			     /*[out]*/ int* removed, 
//			     /*[out]*/ int* error);
//
//      //If the relation did not exist, the function does nothing. 
//      void RemoveFixedRelation(/*[in]*/ int parentindex, 
//			       /*[in]*/ int childindex, 
//			       /*[out]*/ int* error);
//
//      //There is a set of "fixed relations" which all pedigrees have in 
//      //common. Then, each specific pedigree may have additional relations. 
//      //error==2: illegal relation, based on YOB or isChild data. 
//      //error==3: illegal relation: child cannot be its own parent!
//      //error==4: illegal relation: child is ancestor of parent
//      //error==5: illegal relation: child already has such parent. 
//      //The relation is added to all present pedigrees: For some, the
//      //relation is illegal in that particular pedigree. Then, that 
//      //pedigree is removed from the list of pedigrees. A list of 
//      //indicators is returned, indicating which pedigrees are removed. 
//      //NOTE: The array removed must be allocated and deallocated by
//      //the calling routine. Its length should equal to the number of 
//      //pedigrees. 
//      void AddFixedRelation(/*[in]*/ int parentindex, 
//			    /*[in]*/ int childindex, 
//			    /*[out]*/ int* removed, 
//			    /*[out]*/ int* error);
//
//      //Removing a person
//      void RemovePerson(/*[in]*/ int index, 
//			/*[out]*/ int* error);
//
      //Adding a single person to the set of specified persons. 
      //male: 1 if male, 0 if female. For now, we assume that all
      //persons have a known sex. 
      //YOB: Year Of Birth. Must be a positive number, or -1, which 
      //designates missing data. 
	  //isChild: 1 means child (i.e., has no own children), 0 means not. 
      //index: The index of the new person in the array of persons. 
      //Used only for control. 
      // VCFamilias keeps a list of persons: The females are listed first,then 
      // the males. If for example a female is added after males have been
      // added, the indices of all males are increased by one, and the 
      // returned index is at the end of the list of females. 
      void AddPerson(/*[in]*/ int male, 
		     /*[in]*/ int YOB, 
			 /*[in]*/ int isChild, 
		     /*[out]*/ int* index, 
		     /*[out]*/ int* error);
   private:

      //Contains the set of named persons, and all DNA-related info. 
      //Also contains the fixed family relations. 
      pater pat; 

      int nPersons; 
      int* male; //array of length nPersons
	  int* isChild; //array of length nPersons
      char** internalPersonName; //array of length nPersons
      int counter; //used to give internal names
      int* YOB; //array of length nPersons

      int nSystems; 
      char** internalSystemName; //array of length nSystems
      int* nAlleles; //array of length nSystems
      char*** internalAlleleName; //array of length nSystems; 
      //of arrays of lengths nAlleles[i]. 
      
      //All pedigrees:
      PedigreeList pedset;  
};

#endif 
