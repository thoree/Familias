/* $Id: GroupOfPersons.C,v 1.10 2000/08/07 07:44:07 mostad Exp $ */

#include <cstdio>

#include "special.h"
#include "GroupOfPersons.h"

GroupOfPersons::GroupOfPersons(ifstream& is)
{
   readData(is); 
}

GroupOfPersons::GroupOfPersons(const char* filename, int& OK)
{
   nWomen = 0; 
   namesWomen = 0; 
   nExtraWomen = 0; 
   nMen = 0; 
   namesMen = 0; 
   nExtraMen = 0; 
   nChildren = 0; 
   namesChildren = 0; 
   nTotal = 0; 
   allNames = 0; 
   male = 0; 
   adult = 0; 

   //#ifdef UNIXVERSION
   ifstream is(filename);
   //#else
   //ifstream is(filename, ios::nocreate);
   //#endif
   if (!is)
   {
// Removed 2012-03-08
//      cout<<"Cannot open file \""<<filename<<"\".\n";
      OK = 0; 
      return; 
   }
// Removed 2012-03-08
//   cout<<"Reading the header of the pedigree file \""<<filename<<"\".\n";
   readData(is); 
   if (!is)
   {
// Removed 2012-03-08
//      cout<<"Problems reading file \""<<filename<<"\".\n";
      OK = 0; 
   }

}

void GroupOfPersons::readData(ifstream& is)
{
   int i; 
   is>>nWomen; 
   namesWomen = new char*[nWomen]; 
   for (i=0; i<nWomen; i++)
   {
      namesWomen[i] = new char[100]; 
      is>>namesWomen[i];    
   }
   is>>nMen; 
   namesMen = new char*[nMen]; 
   for (i=0; i<nMen; i++)
   {
      namesMen[i] = new char[100]; 
      is>>namesMen[i]; 
   }
   is>>nChildren; 
   namesChildren = new char*[nChildren]; 
   for (i=0; i<nChildren; i++)
   {
      namesChildren[i] = new char[100]; 
      is>>namesChildren[i]; 
   }
   is>>nExtraWomen; 
   is>>nExtraMen; 
   nTotal = nWomen + nExtraWomen + nMen + nExtraMen + nChildren; 
   allNames = new char*[nTotal]; 
   for (i=0; i<nTotal; i++)
   {
      if (i<nWomen) 
	 allNames[i] = namesWomen[i]; 
      else if (i<nWomen+nMen)
	 allNames[i] = namesMen[i-nWomen]; 
      else if (i<nWomen+nMen+nChildren)
	 allNames[i] = namesChildren[i-nWomen-nMen]; 
      else
      {
	 allNames[i] = new char[100];
	 snprintf(allNames[i], 100, "EXTRA%d", i+1); 
      }
   }
   male = new int[nTotal]; 
   adult = new int[nTotal]; 
   for (i=0; i<nTotal; i++)
   {
      male[i] = ((i>=nWomen && i<nWomen+nMen) || (i>=nTotal-nExtraMen)); 
      adult[i] = (i<nWomen+nMen || i>=nWomen+nMen+nChildren); 
   }
}

GroupOfPersons::~GroupOfPersons()
{
   for (int i=0; i<nTotal; i++)
      delete[] allNames[i]; 
   delete[] namesWomen; 
   delete[] namesMen; 
   delete[] namesChildren; 
   delete[] allNames; 
   delete[] male; 
   delete[] adult; 
}

void GroupOfPersons::write(ofstream& os)
{
   int i; 
   os<<nWomen<<'\n'; 
   for (i=0; i<nWomen; i++)
      os<<namesWomen[i]<<"  ";   
   os<<'\n';
   os<<nMen<<'\n'; 
   for (i=0; i<nMen; i++)
      os<<namesMen[i]<<"  "; 
   os<<'\n';
   os<<nChildren<<'\n'; 
   for (i=0; i<nChildren; i++)
      os<<namesChildren[i]<<"  "; 
   os<<'\n';
   os<<nExtraWomen<<'\n'; 
   os<<nExtraMen<<'\n'; 
   os<<"\n";
}

int         GroupOfPersons::getTotalMen() const { return nMen+nExtraMen; }
int         GroupOfPersons::getNamedMen() const { return nMen; }
int         GroupOfPersons::getExtraMen() const { return nExtraMen; }
int         GroupOfPersons::getTotalWomen() const { return nWomen+nExtraWomen; } 
int         GroupOfPersons::getNamedWomen() const { return nWomen; }
int         GroupOfPersons::getExtraWomen() const { return nExtraWomen; }
int         GroupOfPersons::getNamedAdults() const { return nWomen+nMen; }
int         GroupOfPersons::getTotalChildren() const { return nChildren; }
int         GroupOfPersons::getNamedChildren() const { return nChildren; }
int         GroupOfPersons::getNamedPersons() const { return nWomen+nMen+nChildren; }
int         GroupOfPersons::getTotalSize() const { return nTotal; } 
const char* GroupOfPersons::getNameMan(int i) const { return namesMen[i]; }
const char* GroupOfPersons::getNameWoman(int i) const { return namesWomen[i]; }
const char* GroupOfPersons::getNameChild(int i) const { return namesChildren[i]; }
const char* GroupOfPersons::getName(int i) const { return allNames[i]; }
int         GroupOfPersons::isExtra(int i) const { return (i>=nWomen+nMen+nChildren); }
int         GroupOfPersons::isMale(int i) const { return male[i]; }
int         GroupOfPersons::isAdult(int i) const { return adult[i]; }
