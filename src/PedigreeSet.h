/* $Id: PedigreeSet.h,v 1.7 1999/12/03 14:07:51 mostad Exp $ */
#ifndef __PEDIGREESET_H_
#define __PEDIGREESET_H_

#include "PedigreeList.h"

class GroupOfPersons; 
class Pedigree; 

class PedigreeSet 
{
   public:
      PedigreeSet(const char* filename, int& OK); 

      PedigreeSet(GroupOfPersons* gp); 

      ~PedigreeSet(); 

      void addCutset(const char* cutsetfile); 

      // Return 1 if successful; 0 otherwise
      //      int updateDNA(const char* datafile, 
      //		    int mutationModel, 
      //		    double mutationRange, 
      //		    double kinship); 

      // Return 1 if successful; 0 otherwise
      int updateInbreeding(char* multiplier); 

      // Return 1 if successful; 0 otherwise
      int updatePromiscuity(char* multiplier); 

      // Return 1 if successful; 0 otherwise
      int updateGenerations(char* multiplier, 
			    char* maxGenerations); 

      void write(const char* outputfile); 

   private:

      GroupOfPersons* gp; 

      PedigreeList list; 

      double* weight; 

};


#endif

