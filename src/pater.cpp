/* $Id: pater.C,v 1.10 2001/04/12 16:00:36 mostad Exp $ */

#include "pater.h" 
#include "special.h"

void pater::add_information(const char* Information, int info, int& error) 
{
   if (!Information) 
   {
// Removed 2012-03-08
//      if (info>1) cout<<"No information added.\n";
      return;
   }
   if (information) 
   {
// Removed 2012-03-08
//      if (info>0)
//      {
//	 cout<<"ERROR: Information must first be removed before\n"
//	    "new information can be added.\n";
//	 error = 1; 
//      }
   } 
   else 
   {
      information = newcopy(Information);
      rewrite = 1;
// Removed 2012-03-08
//      if (info>1) cout<<"New information added.\n";
   }
}

void pater::remove_information(int info, int& error) 
{
   if (information) 
   {
      delete[] information;
      information = 0;
      rewrite = 1;
// Removed 2012-03-08
//      if (info>1) cout<<"Information removed.\n";
   } 
// Removed 2012-03-08
//   if (info>0) cout<<"ERROR: No information present.\n";
   error = 1; 
}

void pater::add_person(int male, char* name, int info, int& error) { 
    if (fam.add_person(male, name, info, error)) 
       rewrite = recalculate = 1;
}

int pater::isMale(char* name)
{
   return fam.isMale(name); 
}

int pater::inFamily(char* name)
{
   return fam.inFamily(name); 
}

void pater::remove_person(int male, char* name, int info, int& error) { 
    person* p;
    if (fam.remove_person(male, name, p, info, error)) 
       rewrite = recalculate = 1;
    if (p) alldat.remove_person(p, info, error);
}

void pater::add_parent(char* parent, char* child, int info, int& error) {
    if (fam.add_relation(parent, child, info, error)) 
       rewrite = recalculate = 1;
}

void pater::remove_parent(char* parent, char* child, int info, int& error) {
    if (fam.remove_relation(parent, child, info, error)) 
       rewrite = recalculate = 1;
}

void pater::remove_possible_parent(char* parent, char* child) {
   if (fam.remove_possible_relation(parent, child)) rewrite = recalculate = 1;
}

void pater::add_odds(char* person1, char* person2, int info, int& error) {
    if (fam.add_odds(person1, person2, info, error)) 
       rewrite = recalculate = 1;
}

void pater::remove_odds(int info, int& error) {
    if (fam.remove_odds(info, error)) rewrite = recalculate = 1;
}

void pater::add_system(char* systemname, 
		       int lOfVector, 
		       double* mMatrixFemale, 
		       double* mMatrixMale, 
		       int sMutationMatrix, 
		       int n_possibilities, 
		       int info, 
		       int& error) {
    if (alldat.add_system(systemname, 
			  lOfVector, 
			  mMatrixFemale, 
			  mMatrixMale, 
			  sMutationMatrix, 
			  n_possibilities, 
			  info, 
			  error)) 
       rewrite = recalculate = 1;
}

void pater::remove_system(char* systemname, int info, int& error) {
    if (alldat.remove_system(systemname, info, error)) 
       rewrite = recalculate = 1;
}

int pater::get_number_of_systems()
{
   return alldat.get_number_of_systems(); 
}

/*
void pater::setNewParameters(int mutationModel, 
			     double mutationRange, 
			     double kinship)
{
   alldat.setMutation(mutationModel, 
		      mutationRange); 
   alldat.setKinship(kinship); 
//////////////////////////////////////////////////////////
// 2004-08-31: This function seems to be unused at the moment, 
// but I still add the line below, as it would seem to be 
// an error if this function was used without this line: 

rewrite = recalculate = 1; 

//////////////////////////////////////////////////////////
}
*/

void pater::setKinship(double kinship)
{


///////////////////////////////////////////////////////////
// 2004-08-31: The parameter previouslyUsedKinship, and the code
// below, was added to remove a bug: 

	if (kinship != previouslyUsedKinship) {
		previouslyUsedKinship = kinship; 
		rewrite = recalculate = 1; 
		alldat.setKinship(kinship); 
	}

// OLD CODE: 
//
//   alldat.setKinship(kinship); 
//
///////////////////////////////////////////////////////////
}

void pater::add_allele(char* systemname, char* allele, 
		       double probability, int info, int& error) {
   if (alldat.add_allele(systemname, allele, probability, info, error))
      rewrite = recalculate = 1;
}

void pater::set_allele_as_silent(char* systemname, char* allele, int& error) {
	if (alldat.set_allele_as_silent(systemname, allele, error))
		rewrite = recalculate = 1;       
}

void pater::remove_allele(char* systemname, char* allele, 
			  int info, int& error) 
{
   if (alldat.remove_allele(systemname, allele, info, error))
	rewrite = recalculate = 1;
}

void pater::add_data(char* systemname, char* pers, char* allele1, 
		     char* allele2, int info, int& error) 
{
   person* p = fam.get_person(pers);
   if (p) {
      if (alldat.add_data(systemname, *p, allele1, allele2, info, error))
	 rewrite = recalculate = 1;
      delete[] pers;
   }
   else 
   {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The person \""<<pers<<"\" could not be found.\n";
      error = 1; 
      delete[] pers;
      delete[] systemname;
      delete[] allele1;
      delete[] allele2;
   }
}

void pater::remove_data(char* systemname, char* pers, int info, int& error) 
{
   person* p = fam.get_person(pers);
   if (p) {
      if (alldat.remove_data(systemname, *p, info, error))
	 rewrite = recalculate = 1;
      delete[] pers;
   }
   else 
   {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The person \""<<pers<<"\" could not be found.\n";
      error = 1; 
      delete[] pers;
      delete[] systemname;
   }
}

int pater::add_person_to_cutset(char* name, int info, int& error) {
  cuts_added = 1;
  return fam.add_person_to_cutset(name, info, error);
}

void pater::end_cutset(int info, int& error) {
  fam.end_cutset(info, error);
}

void pater::execute(int info, int& error) 
{
   if (recalculate || cuts_added) 
   {
      if (fam.get_first()) 
      {
	 ofstream file(dump_file_name);
// Removed 2012-03-08
//	 if (!file.good() && info>0) 
//	    cout<<"WARNING: Cannot open file \""<<dump_file_name
//		<<"\" for storage of data structure during execution.\n";
//	 else 
//	 {
	    write_top(file);
	    write(file);
	    file<<separator;
//	 }      
	 file.close();
	 alldat.execute(fam, info, error);
	 system_delete_file(dump_file_name);
	 if (error) return;
      }
      else 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    cout<<"ERROR: Probabilities may not be calculated "
//	       "before any persons are added.\n";
	 error = 1; 
	 return;
      }
   }
   recalculate = 0;
// Removed 2012-03-08
//   if (info>0) alldat.write_results(cout, fam.get_odds());
}

void pater::remove_cutsets() {
  cuts_added = 0;
  fam.remove_cutsets();
}

void pater::write_top(ostream& out) {
    out<<separator<<"DNA PROBABILITY COMPUTATIONS REPORT\n"
       <<"Written by the program familias, version "<<version<<'\n'
       <<"Date: ";
    writedate(out);
    out<<'\n'<<separator;
}

void pater::write(ostream& out) {
    if (information) out<<information; 
    if (recalculate==0) alldat.write_results(out, fam.get_odds()); 
    fam.write(out); 
    alldat.write(out, recalculate==0, fam.get_odds());
}







