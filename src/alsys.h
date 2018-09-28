/* $Id: alsys.h,v 1.8 2001/04/12 16:00:35 mostad Exp $ */

#ifndef __ALSYS_H_
#define __ALSYS_H_

class person;
class oddsobject;
class family;

class dataitem {
//The allele1 and allele2 variables will be indices in the 
//full allele system.
public:
    dataitem(person& pers, int all1, int all2) :
	next(0), p(&pers), allele1(all1), allele2(all2) {}
    ~dataitem() { if (next) delete next; }
    dataitem* next;
    person* p;
    int allele1;
    int allele2;
    void append(dataitem* dit) {
	if (next) next->append(dit);
	else next = dit;
    }
    void remove_next(dataitem* dit);
};

class allelesystem {
    char* Systemname;


    ////2014-07-23: The following 6 parameters were replaced with the 
    //// next 4 parameters: 
    //    double mutationrateFemale;
    //    double mutationrateMale;
    //    int mutationModelFemale; 
    //    int mutationModelMale; 
    //    //The following parameter is only meaningful when mutationModel==2: 
    //    double mutationRangeFemale; 
    //    double mutationRangeMale;
    //// WERE REPLACED BY: 
    int lengthOfVector; //Ugly way to make construction etc smooth. 
    double* mutationMatrixFemale; 
    double* mutationMatrixMale; 
    int simplifyMutationMatrix; 

    //The number below represents the total number of possible "alleles"
    //a mutation can mutate TO. We must always have n_possibilities>=2.
    //This parameter is only meaningful for mutationModel==0. 
    int n_possibilities;
    //Used to adjust probabilities: 
    double kinship; 

    allelesystem* next;

    //Full contents of system:
    int n_alleles;
    char** name;
    //The general population probability ( >=0) of each of the alleles 
    //listed in "name". The sum of the array numbers must be <=1. 
    double* probability;
	//We keep a flag indicating whether the system has a silent allele, 
	//and the index of that allele in the full system:
	int hasSilentAllele; 
	int silentAllele; 

    //The data stored about persons. In the data objects, the
    //information is relative to the full system.
    dataitem* data;

    //The result of probability calculations is stored:
    double result;

    //The data below is computed based on the data above by the 
    //routine compute_dataprob. The contents of the allele system is "pruned" 
    //to contain only the alleles appearing in the data, plus one "unknown", 
	//and the silent allele, if there is such an allele. 
    //n_dataalleles!=0 means that we have filled in data below.
    int n_dataalleles;
    //For an index i in the full system, index[i] gives the index
    //in the "pruned" system.
    int* index;
    //The sum of the dataprobability array is 1. The probabilities of all
    //but the "unknown" alleles are taken from "probability" above.
    double* dataprobability;
    //dataprobmatrix[i][j] represents the probability of starting with
    //an allele i, and, after a POSSIBLE mutation, end up with j.
    double** dataprobmatrixFemale;
    double** dataprobmatrixMale;

    //The following flag is set to 1 whenever the 4 variables above 
    //must be recalculated before use.
    int recalc_data;

public:
    allelesystem(char* name, 
		 int lOfVector, 
		 double* mMatrixFemale, 
		 double* mMatrixMale, 
		 int sMutationMatrix, 
		 int n_poss); 

    ~allelesystem();

    char* Name() { return Systemname; }
    allelesystem* get_next() { return next; }
    void add_at_end(allelesystem* p) {
	if (next) next->add_at_end(p);
	else next = p;
    }
    double Result() { return result; }
    void remove_next(allelesystem* s);

    //REMOVED 2014-07-23 as it is not needed when the code is used 
    //as an R interface: 
    //    int new_mutationrate(double mutationrateFemale, 
    //			 double mutationrateMale, 
    //			 int mutationModelFemale, 
    //			 int mutationModelMale, 
    //			 int n_possibilities, 
    //			 double mutationRangeFemale, 
    //			 double mutationRangeMale, 
    //			 int info, 
    //			 int& error);
      
    //      void setMutation(int mutationModel, 
    //		       double mutationRange); 

      void setKinship(double kinship); 

      int get_number_of_systems(); 

    int add_allele(char* allelename, double probability, 
		   int info, int& error); 
	int set_allele_as_silent(char* allelename, int& error); 
    int add_data(person& p, char* allele1, char* allele2, 
		 int info, int& error);
    int remove_allele(char* allelename, 
		      int info, int& error);
    int remove_data(person& p, int info, int& error);

    //Compute the last three of the data items above, and change the
    //indices in the data object to reflect this.
    void compute_dataprob();

    void execute(family& fam, int info, int& error);
    //Computes the probability (or the odds) 
    //for the data currently stored, given the family structure.

    void write(ostream& out, int old_results_valid,
	       oddsobject* odds);
    //Lists everything about this system.

    void write_freq(ostream& out);
    //Lists only allele frequencies.
};

#endif
