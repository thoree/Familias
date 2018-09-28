/* $Id: odds.h,v 1.7 2001/04/12 16:00:36 mostad Exp $ */

#ifndef __ODDS_H_
#define __ODDS_H_

class person;
class family;
class oddsobject;
class pers;
class cutset;
class branch;
class systemdata;

//This class does the actual computation of probabilities or odds,
//given a family, allele system data, and after being given the 
//relevant allele data for persons through the "add_data" routine.
class odds {
  //The following object contains no pers objects, but a number of branches
  //corresponding to the connected components of the pedigree:
  cutset* primcut;

  //The following points to the result of a possible collapse:
  pers* collapsed_pers;

  //The next three items are used in the interplay between routines
  //add_person_to_cutset and end_cutset:
  cutset* currcut;
  branch* currbranch;
  int cutset_must_end;
public:
  odds(person* p, int separateComponents);
  //The second constructor is used if the two last persons should be
  //"collapsed" into one person in this object.
  odds(person* p, person* coll1, person* coll2);
  ~odds();

  //The following function adds one person to the most recently made cutset,
  //or makes a cutset, if necessary. It returns 1 if error, 0 otherwise:
  int add_person_to_cutset(char* name, pers* pr, 
			   int info, int& error);
  //The following function completes and stores a cutset.
  void end_cutset(int info, int& error);

  int add_data(systemdata& sd, pers* p, 
	       int allele1, int allele2, 
	       int info, int& error);
  double execute(systemdata& sys, int& err);
  void remove_data();
};

class systemdata 
{
      char* systemname;
      int n_dataalleles;
      
      //The frequencies of alleles: 
      double* dataprobability;

      //The number "used" of each allele: 
      int* used;
      int totalUsed; 

      //The transition matrix (the identity if no mutations)
      double** dataprobmatrixFemale;
      double** dataprobmatrixMale;

      //The kinship parameter: 
      double kinship; 

	  int hasSilentAllele; 
	  int silentAllele; 

   public:
      
      systemdata(char* Systemname, 
		 int N_dataalleles,
		 double* Dataprobability, 
		 double** DataprobmatrixFemale, 
		 double** DataprobmatrixMale, 
		 double kship,
		 int hasSilent, 
		 int silent) :
	 systemname(Systemname), 
	 n_dataalleles(N_dataalleles),
	 dataprobability(Dataprobability), 
	 dataprobmatrixFemale(DataprobmatrixFemale), 
	 dataprobmatrixMale(DataprobmatrixMale), 
	 kinship(kship), 
	 hasSilentAllele(hasSilent), 
	 silentAllele(silent)
      {
	 used = new int[n_dataalleles]; 
	 for (int i=0; i<n_dataalleles; i++)
	    used[i] = 0; 
	 totalUsed = 0; 
      }

      ~systemdata()
      {
	 delete[] used; 
      }

      int number_of_alleles() 
      {
	 return n_dataalleles;
      }

      double prob(int allele) 
      {
	 return dataprobability[allele];
      }

      double set_allele(int allele)
      {
	 return ((used[allele]++)*kinship + 
	    dataprobability[allele]*(1-kinship))/
	    (1+((totalUsed++)-1)*kinship); 
      }

      void unset_allele(int allele)
      {
	 used[allele]--; 
	 totalUsed--; 
      }

      int useCutsets()
      {
	 return kinship==0; 
      }

      double inherit_prob_female(int paternalallele, 
				 int maternalallele, 
				 int resultallele) 
      {
	 return 0.5 * dataprobmatrixFemale[paternalallele][resultallele]
	    + 0.5 * dataprobmatrixFemale[maternalallele][resultallele];
      }

      double inherit_prob_male(int paternalallele, 
			       int maternalallele, 
			       int resultallele) 
      {
	 return 0.5 * dataprobmatrixMale[paternalallele][resultallele]
	    + 0.5 * dataprobmatrixMale[maternalallele][resultallele];
      }

      char* sysname() 
      { 
	 return systemname;
      }

	  int has_silent_allele()
	  {
		  return hasSilentAllele; 
	  }

	  int silent_allele()
	  {
		  return silentAllele; 
	  }
};


#endif









