/* $Id: aldata.C,v 1.9 2001/04/12 16:00:34 mostad Exp $ */

#include <cstring>

#include "family.h"
#include "alsys.h"
#include "aldata.h" 
#include "table.h"
#include "person.h"
#include "special.h"
#include "assert.h"

alleledata::~alleledata() {
    if (a) {
	allelesystem* s;
	while ((s = a->get_next())) {
	    delete a;
	    a = s;
	}
	delete a;
    }
}

void alleledata::write(ostream& out, int old_results_valid,
		       oddsobject* odds) {
    allelesystem* p = a;
    while (p) {
	p->write(out, old_results_valid, odds);
	p = p -> get_next();
    }
}

void alleledata::write_system(ostream& out, char* system, 
			      int old_results_valid,
			      oddsobject* odds, 
			      int info, 
			      int& error) 
{
   allelesystem* p = get_system(system);
   if (p) p->write(out, old_results_valid, odds);
   else 
   {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The system \""<<system<<"\" not found.\n";
      error = 1; 
   }
   delete[] system;
}

void alleledata::write_system_freq(ostream& out, char* system, 
				   int info, int& error) {
   allelesystem* p = get_system(system);
   if (p) p->write_freq(out);
   else 
   {
// Removed 2012-03-08
//      if (info>0)
//	 cout<<"ERROR: The system \""<<system<<"\" not found.\n";
      error = 1; 
   }
   delete[] system;
}

void alleledata::write_freq(ostream& out) {
    allelesystem* p = a;
    while (p) {
	p->write_freq(out);
	p = p -> get_next();
    }
}

void alleledata::write_results(ostream& out, oddsobject* odds) {
    out<<separator<<"RESULTS\n"<<separator;
    if (a) {
	allelesystem* p = a;
	double product = 1;
	out<<"\nResults for each system:\n";
	table tab("allele system","result");
	while (p) {
	    tab.put(p->Name());
	    tab.endcolumn();
	    tab.put(p->Result());
	    tab.endcolumn();
	    product *= p->Result();
	    p = p ->get_next();
	}
	tab.printout(out);
	if (odds) 
	    out<<"\nThe total odds that "<<odds->pers1->name()
	       <<" = "<<odds->pers2->name()<<": "<<product<<"\n";
	else
	    out<<"\nThe total probability of the data given the family "
	       <<"structure: "<<product<<'\n';
    } else
	out<<"\nNo allele systems in the data.\n";
}

double* alleledata::get_results(int info, int& error)
{
	double* result = new double[get_number_of_systems()]; 
	int counter = 0; 
   if (a)
   {
      //double result = 1; 
      allelesystem* p = a; 
      while (p)
      {
		  result[counter] = p->Result(); 
		  counter++; 
	 //result *= p->Result(); 
	 p = p->get_next();
      }
      return result; 
   }
// Removed 2012-03-08
//   if (info>0)
//      cout<<"ERROR: No allele systems in the data.\n";
   error = 1; 
   return 0;
}

void alleledata::execute(family& fam, int info, int& error) {
    allelesystem* p = a;
    while (p && !error) {
	p->execute(fam, info, error);
	p = p -> get_next();
    }
}

int alleledata::remove_person(person* pers, int info, int& error) {
    int sum = 0;
    allelesystem* p = a;
    while (p) {
	sum += p -> remove_data(*pers, info, error);
	p = p ->get_next();
    }
    delete pers;
    return sum;
}

//NOTE: name is not deleted!
allelesystem* alleledata::get_system(char* name) {
    allelesystem* p = a;
    while (p) {
	if (strcmp(name, p->Name())==0) return p;
	p = p->get_next();
    }
    return 0;
} 

int alleledata::add_system(char* name, 
			   int lOfVector, 
			   double* mMatrixFemale, 
			   double* mMatrixMale, 
			   int sMutationMatrix, 
			   int n_possibilities, 
			   int info, 
			   int& error) {


   if (a) a->add_at_end(new allelesystem(name, 
					 lOfVector, 
					 mMatrixFemale, 
					 mMatrixMale, 
					 sMutationMatrix, 
					 n_possibilities));
   else a = new allelesystem(name, 
			     lOfVector, 
			     mMatrixFemale, 
			     mMatrixMale, 
			     sMutationMatrix, 
			     n_possibilities);
   return 1;
}

int alleledata::get_number_of_systems()
{
   if (!a) return 0; 
   return a->get_number_of_systems(); 
}

//void alleledata::setMutation(int mutationModel, 
//			     double mutationRange)
//{
//   allelesystem* p = a; 
//   while (p) 
//   {
//      p->setMutation(mutationModel, 
//		     mutationRange); 
//      p = p->get_next(); 
//   }
//}

void alleledata::setKinship(double kinship)
{
   allelesystem* p = a; 
   while (p) 
   {
      p->setKinship(kinship); 
      p = p->get_next(); 
   }
}

int alleledata::add_allele(char* systemname, char* allelename, 
			   double probability, int info, int& error) 
{
   allelesystem* p = get_system(systemname);
   if (p) {
      delete[] systemname;
      return p->add_allele(allelename, probability, info, error);
   }
// Removed 2012-03-08
//   if (info > 0)
//      cout<<"ERROR: The allele system \""<<systemname
//	  <<"\" could not be found.\n";
   error = 1; 
   delete[] allelename;
   delete[] systemname;
   return 0;
}

int alleledata::set_allele_as_silent(char* systemname, char* allelename, int& error)
{
   allelesystem* p = get_system(systemname);
   if (p) {
      delete[] systemname;
      return p->set_allele_as_silent(allelename, error);
   }
   error = 1; 
   delete[] allelename;
   delete[] systemname;
   return 0;
}

int alleledata::add_data(char* systemname, person& p, 
			 char* allele1, char* allele2,
			 int info, int& error) 
{
   allelesystem* asys = get_system(systemname);
   if (asys) {
      delete[] systemname;
      return asys->add_data(p, allele1, allele2, info, error);
   }
// Removed 2012-03-08
//   if (info>0)
//      cout<<"ERROR: The allele system \""<<systemname
//	  <<"\" could not be found.\n";
   error = 1; 
   delete[] allele1;
   delete[] allele2;
   delete[] systemname;
   return 0;
}

int alleledata::remove_system(char* name, int info, int& error) 
{
   allelesystem* p = get_system(name);
   if (p) 
   {
      if (p==a) a = a->get_next();
      else a->remove_next(p);
// Removed 2012-03-08
//      if (info>1) 
//	 cout<<"Removing the system \""<<name<<"\" from the data.\n";
      delete p;
      delete[] name;
      return 1;
   } 
// Removed 2012-03-08
//   if (info>0)
//      cout<<"ERROR: The allele system \""
//	  <<name<<"\" could not be found.\n";
   error = 1; 
   delete[] name;
   return 0;
}

int alleledata::remove_allele(char* systemname, char* allelename, 
			      int info, int& error) 
{
   allelesystem* p = get_system(systemname);
   if (p) 
   {
      delete[] systemname;
      return p->remove_allele(allelename, info, error);
   }
// Removed 2012-03-08
//   if (info>0)
//      cout<<"ERROR: The allele system \""<<systemname
//	  <<"\" could not be found.\n";
   error = 1; 
   delete[] systemname;
   delete[] allelename;
   return 0;
}

int alleledata::remove_data(char* systemname, person& p, 
			    int info, int& error) 
{
   allelesystem* asys = get_system(systemname);
   if (asys) {
      delete[] systemname;
      return asys->remove_data(p, info, error);
   }
// Removed 2012-03-08
//   if (info>0)
//      cout<<"ERROR: The allele system \""<<systemname
//	  <<"\" could not be found.\n";
   error = 1; 
   delete[] systemname;
   return 0;
}



