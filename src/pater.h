/* $Id: pater.h,v 1.9 2001/04/12 16:00:37 mostad Exp $ */

#ifndef __PATER_H_
#define __PATER_H_

#include "aldata.h"
#include "family.h"
#include "special.h"

class person;

class pater {
    //This object contains all data and does all reading and 
    //execution of commands.
    family fam;
    alleledata alldat;
    int recalculate; //is 1 if probabilities/odds must be recalculated
    int cuts_added;  //is 1 if probabilities/odds shuld be recalculated 
                     //because a cutset is added.
    int rewrite;     //is 1 if user should be prompted to write out data
					 //before quitting
    char* information;
    double previouslyUsedKinship; //used to detect whether one needs to recalculate
	                 //when the kinship is set. 

public:
    //In all the routines below, this object takes over the responsibility
    //for the deallocation of the strings submitted.
    pater() : recalculate(1), cuts_added(0), 
    rewrite(0), information(0), previouslyUsedKinship(-1.0) {}
    ~pater() { delete[] information; }
    void add_information(const char* Information, int info, int& error);
    void remove_information(int info, int& error);
    int has_information() { return !(information==0); }
    void add_person(int male, char* name, int info, int& error);
    void remove_person(int male, char* name, int info, int& error);
    void add_parent(char* parent, char* child, int info, int& error);
    void remove_parent(char* parent, char* child, int info, int& error);
    void add_odds(char* person1, char* person2, int info, int& error);
    void remove_odds(int info, int& error);
    void add_system(char* systemname, 
		    int lOfVector, 
		    double* mMatrixFemale, 
		    double* mMatrixMale, 
		    int sMutationMatrix, 
		    int n_possibilities, 
		    int info, 
		    int& error);
    void remove_system(char* systemname, int info, int& error);
      
      int get_number_of_systems(); 
      
      //      //Midlertidig: 
      //      void setNewParameters(int mutationModel, 
      //			    double mutationRange, 
      //			    double kinship); 

    void setKinship(double kinship); 

    void add_allele(char* systemname, char* allele, 
		    double probability, int info, int& error); 
	void set_allele_as_silent(char* systemname, char* allele, int& error); 
    void remove_allele(char* systemname, char* allele, int info, int& error);
    void add_data(char* systemname, char* pers, char* allele1, 
		  char* allele2, int info, int& error);
    void remove_data(char* systemname, char* pers, int info, int& error);

    //The following 2 routines must be followed immediately by the execute
    //routine, witout other pater routines in between. They return 1 if they
    //result in an error, 0 otherwise:
    int add_person_to_cutset(char* name, int info, int& error);
    void end_cutset(int info, int& error);
    void execute(int info, int& error);
    void remove_cutsets();
      
      //NEW functions:
      double* getResults(int info, int& error) 
      {
	 return alldat.get_results(info, error); 
      }
      int totalFamilySize()
      {
	 return fam.totalFamilySize();
      }
      void remove_possible_parent(char* parent, char* child); 
      int isMale(char* name); 
      int inFamily(char* name); 

    void write_top(ostream& out);
    void write(ostream& out);
    void write_info(ostream& out) {
	if (information) out<<information;
    }
    void write_family(ostream& out) { 
	fam.write(out); 
    }
    void write_alleles(ostream& out) { 
	alldat.write(out, recalculate==0, fam.get_odds()); 
    }
    void write_persons(int male, ostream& out) { 
	fam.write_persons(male, out); 
    }
      void write_system(ostream& out, char* system, 
			int info, int& error) 
      {
	 alldat.write_system(out, system, recalculate==0, fam.get_odds(), 
			     info, error); 
      }
      void write_system_freq(ostream& out, char* system, int info, int& error) 
      { 
	 alldat.write_system_freq(out, system, info, error); 
      }
    void write_freq(ostream& out) { 
	alldat.write_freq(out); 
    }
    void has_written() {rewrite = 0; }
    int must_write() { return rewrite; }
};

#endif



