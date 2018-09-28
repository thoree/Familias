/* $Id: GroupOfPersons.h,v 1.7 2000/08/07 07:44:07 mostad Exp $ */
#ifndef __GROUPOFPERSONS_H_
#define __GROUPOFPERSONS_H_

#include <fstream>

class GroupOfPersons 
{
   public:
      GroupOfPersons(ifstream& is); 
      GroupOfPersons(const char* filename, int& OK); 

      ~GroupOfPersons(); 

      void write(ofstream& os); 

      int         getTotalMen() const; 
      int         getNamedMen() const; 
      int         getExtraMen() const; 
      int         getTotalWomen() const; 
      int         getNamedWomen() const; 
      int         getExtraWomen() const;
      int         getNamedAdults() const; 
      int         getTotalChildren() const; 
      int         getNamedChildren() const; 
      int         getNamedPersons() const; 
      int         getTotalSize() const; 
      int         isExtra(int i) const; 
      int         isMale(int i) const; 
      int         isAdult(int i) const;
      const char* getNameMan(int i) const; 
      const char* getNameWoman(int i) const; 
      const char* getNameChild(int i) const; 
      const char* getName(int i) const; 

   private:

      int nWomen; 
      char** namesWomen; 
      int nExtraWomen; 
      int nMen; 
      char** namesMen; 
      int nExtraMen; 
      int nChildren; 
      char** namesChildren; 
      int nTotal; 
      char** allNames; 

      int* male; 
      int* adult; 
      
      void readData(ifstream& is); 

};


#endif

