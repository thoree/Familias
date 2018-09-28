/* $Id: aldata.h,v 1.8 2001/04/12 16:00:34 mostad Exp $ */

#ifndef __ALDATA_H_
#define __ALDATA_H_

#include <iostream> 
#include <fstream>
using namespace std; 

class person;
class allelesystem;
class oddsobject;
class family;

class alleledata {
    //This class contains all the data that is not purely family data.
    allelesystem* a;

    //It also contains the last results from a probability calculation or
    //an odds calculation. Other parts of the program must help keep track
    //of whether these are outdated or not.

    allelesystem* get_system(char* name);

public:
    alleledata() : a(0) {}
    ~alleledata();
    //For all the 6 routines below, it is assumed that it is already checked
    //that the names do not contain illegal characters.
    //If info==0, no written output. 
    //If info==1, only error messages are written, not information messages. 
    //If info==2, both types of messages are written. 
    //If data is indeed changed, the return value is 1, otherwise 0.

    int add_system(char* name, 
		   int lOfVector, 
		   double* mMatrixFemale, 
		   double* mMatrixMale, 
		   int sMutationMatrix, 
		   int n_possibilities, 
		   int info, 
		   int& error);
    //If system exists, set new mutationrate.
    //Otherwise, create new system.

      int get_number_of_systems(); 

      //      void setMutation(int mutationModel, 
      //		       double mutationRange); 

      void setKinship(double kinship); 


    int add_allele(char* systemname, char* allelename, double probability, 
		   int info, int& error);
    //If system does not exist: ERROR
    //If allelename does not exist: Add. 
    //Otherwise: Change probabilty.

	int set_allele_as_silent(char* systemname, char* allelename, int& error); 

    int add_data(char* systemname, person& p, char* allele1, char* allele2, 
		 int info, int& error);
    //If any names do not exist: ERROR.
    //If person already has data from this system: Change data.
    //Otherwise: Add data.

    int remove_system(char* name, int info, int& error);
    //If system does not exist: Nothing.
    //Otherwise: Remove system.

    int remove_allele(char* systemname, char* allelename, 
		      int info, int& error);
    //If system does not exist: ERROR.
    //If allele does not exist: ERROR.
    //Otherwise: Remove allele.

    int remove_data(char* systemname, person& p, int info, int& error);
    //If any names do not exist: ERROR.
    //If data do not exist: Nothing.
    //Otherwise: Remove data.

    int remove_person(person* p, int info, int& error);
    //Remove this person from any allele system it is involved in.

    void execute(family& fam, int info, int& error);
    //Compute the results.

    //NEW function: Extract the results: 
    double* get_results(int info, int& error); 

    //Output everything. If old_results_valid, 
    //they are output (they are stored in THIS object).
    void write(ostream& out, int old_results_valid, oddsobject* odds);

    //Output only the given system. Same with old_results_valid. 
    //ERROR is output if no such system.
    void write_system(ostream& out, char* system, 
		      int old_results_valid, 
		      oddsobject* odds, 
		      int info, int& error);

    //Output only the given system frequencies. 
    //ERROR is output if no such system.
    void write_system_freq(ostream& out, char* system, 
			   int info, int& error);

    //Output all system frequencies. 
    void write_freq(ostream& out);

    void write_results(ostream& out, oddsobject* odds); 
};

#endif 





