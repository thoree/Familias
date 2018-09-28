/* $Id: FamInterface.C,v 1.15 2001/04/12 16:00:32 mostad Exp $ */

#include "FamInterface.h"
#include "special.h"
#include "Pedigree.h"
#include "special.h"

#include <cassert>
#include <cstdio>

const int fertilityAge = 12; 

FamInterface::FamInterface()
{
   nPersons = 0; 
   male = 0; 
   isChild = 0; 
   internalPersonName = 0; 
   counter = 0; 
   YOB = 0; 
   nSystems = 0; 
   internalSystemName = 0; 
   nAlleles = 0; 
   internalAlleleName = 0; 
}

FamInterface::~FamInterface()
{
   int i; 
   for (i=0; i<nPersons; i++)
      delete[] internalPersonName[i]; 
   delete[] male; 
   delete[] isChild; 
   delete[] internalPersonName; 
   delete[] YOB; 
   for (i=0; i<nSystems; i++)
   {
      for (int j=0; j<nAlleles[i]; j++)
	 delete[] internalAlleleName[i][j]; 
      delete[] internalSystemName[i]; 
      delete[] internalAlleleName[i]; 
   }
   delete[] nAlleles; 
   delete[] internalSystemName; 
   delete[] internalAlleleName; 
}
/*
void FamInterface::GetVersion(double* vers)
{
   *vers = numericVersion; 
}
*/
void FamInterface::AddPerson(int mal, int yob, int ischi, int *index, int *error)
{
   if ((mal != 0 && mal != 1) || (ischi !=0 && ischi != 1))
   {
      *error = 1; 
      return; 
   }
   *error = 0; 
   
   int* newMale = new int[nPersons+1]; 
   int* newIsChild = new int[nPersons+1]; 
   char** newNames = new char*[nPersons+1]; 
   int* newYOB  = new int[nPersons+1]; 
   for (int i=0; i<nPersons; i++)
   {
      newMale[i]    = male[i];
	  newIsChild[i] = isChild[i]; 
      newNames[i]   = internalPersonName[i]; 
      newYOB[i]     = YOB[i]; 
   }
   delete[] male; 
   delete[] isChild; 
   delete[] internalPersonName; 
   delete[] YOB; 
   male         = newMale;
   isChild      = newIsChild; 
   internalPersonName = newNames; 
   YOB          = newYOB; 
   male[nPersons]         = mal;
   isChild[nPersons]      = ischi; 
   internalPersonName[nPersons] = new char[100]; 
   sprintf(internalPersonName[nPersons], "%d", ++counter); 
   YOB[nPersons]          = yob; 
   *index = nPersons; 
   int internalError = 0; 
   pat.add_person(mal, 
		  newcopy(internalPersonName[nPersons]), 
		  0, internalError); 
   //removed 2012-03-16
   //   assert(internalError==0); 
   pedset.addPerson(mal); 
   nPersons++; 
}
/*
void FamInterface::RemovePerson(int index, int *error)
{
   if (index<0 || index>=nPersons)
   {
      *error = 1; 
      return; 
   }
   *error = 0; 

   int internalError = 0; 
   pat.remove_person(male[index], 
		     newcopy(internalPersonName[index]), 
		     0, internalError); 
   pedset.removePerson(index); 
   //removed 2012-03-16
   //   assert(!internalError); 
   delete[] internalPersonName[index]; 
   for (int i=index; i<nPersons-1; i++)
   {
      male[i]         = male[i+1]; 
	  isChild[i]      = isChild[i+1]; 
      internalPersonName[i] = internalPersonName[i+1]; 
      YOB[i]          = YOB[i+1]; 
   }
   nPersons--; 
}

void FamInterface::AddFixedRelation(int parentindex, 
				    int childindex, 
				    int* removed, 
				    int *error)
{
   if (parentindex<0 || parentindex>=nPersons ||
       childindex<0 || childindex>=nPersons)
   {
      *error = 1; 
      return; 
   }
   if ((YOB[parentindex] != -1 &&
		YOB[childindex] != -1 &&
		YOB[parentindex] >= YOB[childindex])||
		isChild[parentindex])
   {
      *error = 2; 
      return; 
   }
   *error = 0; 
   pat.add_parent(newcopy(internalPersonName[parentindex]), 
		  newcopy(internalPersonName[childindex]), 
		  0, 
		  *error); 
   //removed 2012-03-16
   //   assert(*error!=1); //internal error
   if (*error==0)
      pedset.addFixedRelation(parentindex, 
			      childindex, 
			      removed); 
}

void FamInterface::RemoveFixedRelation(int parentindex, 
				       int childindex, 
				       int *error)
{
   if (parentindex<0 || parentindex>=nPersons ||
       childindex<0 || childindex>=nPersons)
   {
      *error = 1; 
      return; 
   }
   *error = 0; 
   int paterError; 
   pat.remove_parent(newcopy(internalPersonName[parentindex]), 
		     newcopy(internalPersonName[childindex]), 
		     0, 
		     paterError); 
   //paterError==1 iff the relation did not exist; we ignore this
   pedset.removeFixedRelation(parentindex, 
			      childindex); 
}

void FamInterface::GeneratePedigrees(int nExtraFemales, 
				     int nExtraMales, 
				     int *removed, 
				     int *error)
{
   if (nExtraFemales<0 || nExtraMales<0)
   {
      *error = 1; 
      return; 
   }

   int nTotal = nPersons + nExtraFemales + nExtraMales; 
   //possibleParent is an array such that 
   //possibleParent[i+j*nTotal] is the number of generations going
   //in positive time from i to j that is poissible according to 
   //the YOB and isChild data. 
   //So if possibleParent[..] is 0, i cannot be an ancestor of j. 
   //If possibleParent[..] is 1, i can be a parent, but not a grandparent. 
   //And so on...
   int* possibleParent = new int[nTotal*nTotal]; 
   int i, j; 
   for (i=0; i<nTotal; i++) {
	   if (i>=nPersons)
	       for (j=0; j<nTotal; j++)
	           possibleParent[i+j*nTotal] = nTotal;
	   else if (isChild[i])
		   for (j=0; j<nTotal; j++)
		       possibleParent[i+j*nTotal] = 0; 
	   else if (YOB[i] == -1)
	       for (j=0; j<nTotal; j++)
	           possibleParent[i+j*nTotal] = nTotal;
       else 
	       for (j=0; j<nTotal; j++)
	           if (j>=nPersons || YOB[j] == -1)
	               possibleParent[i+j*nTotal] = nTotal; 
	           else
			   {
	               int nG = (YOB[j] - YOB[i])/fertilityAge; 
	               if (nG>0)
		               possibleParent[i+j*nTotal] = nG; 
	               else
		               possibleParent[i+j*nTotal] = 0; 
			   }
   }


//	*error = isChild[0]*1000000 + isChild[1]*1000+isChild[2]; 
//	return; 

   pedset.generatePedigrees(nExtraFemales, 
			    nExtraMales, 
			    possibleParent); 

    delete[] possibleParent; 
   *error = 0; 
}

void FamInterface::GetNumberOfPedigrees(int *number)
{
   *number = pedset.getNumberOfPedigrees(); 
}
*/
void FamInterface::AddPedigree(int nExtraFemales, 
			       int nExtraMales, 
			       int *index, 
			       int *error)
{
   if (nExtraFemales<0 || nExtraMales<0)
   {
      *error = 1; 
      return; 
   }
   *error = 0;
   pedset.addPedigree(nExtraFemales, 
		      nExtraMales); 
   *index = pedset.getNumberOfPedigrees()-1; 
}
/*
void FamInterface::RemovePedigree(int index, 
				  int *error)
{
   if (index<0 || index>=pedset.getNumberOfPedigrees())
   {
      *error = 1; 
      return; 
   }
   *error = 0; 
   pedset.removePedigree(index); 
}

void FamInterface::GetNumberOfExtraMales(int index, 
					 int *number, 
					 int *error)
{
   if (index<0 || index>=pedset.getNumberOfPedigrees())
   {
      *error = 1; 
      return; 
   }
   *error = 0; 
   *number = pedset.getPedigree(index)->getNumberOfExtraMales(); 
}

void FamInterface::GetNumberOfExtraFemales(int index, 
					   int *number, 
					   int *error)
{
   if (index<0 || index>=pedset.getNumberOfPedigrees())
   {
      *error = 1; 
      return; 
   }
   *error = 0; 
   *number = pedset.getPedigree(index)->getNumberOfExtraFemales(); 
}

void FamInterface::GetSizeOfPedigree(int index, 
				     int *size, 
				     int *error)
{
   if (index<0 || index>=pedset.getNumberOfPedigrees())
   {
      *error = 1; 
      return; 
   }
   *error = 0; 
   *size = pedset.getPedigree(index)->getPedigreeSize(); 
}

void FamInterface::GetPedigree(int index, 
			       int *matrix, 
			       int *error)
{
   if (index<0 || index>=pedset.getNumberOfPedigrees())
   {
      *error = 1; 
      return; 
   }
   *error = 0; 
   pedset.getPedigree(index)->getPedigree(matrix); 
}

void FamInterface::GetParents(int index, 
			      int* mother, 
			      int* father, 
			      int* error)
{
   if (index<0 || index>=pedset.getNumberOfPedigrees())
   {
      *error = 1; 
      return; 
   }
   *error = 0; 
   pedset.getPedigree(index)->getParents(mother, father); 
}
*/
void FamInterface::AddRelation(int parentindex, 
			       int childindex, 
			       int index, 
			       int *error)
{
   if (index<0 || index>=pedset.getNumberOfPedigrees())
   {
      *error = 1; 
      return; 
   }
   Pedigree* ped = pedset.getPedigree(index); 
   int size = ped->getPedigreeSize(); 
   if (parentindex<0 || parentindex>=size ||
       childindex<0 || childindex>=size)
   {
      *error = 1; 
      return; 
   }
//   if (parentindex == nPersons) 
//   {
//	   *error = 10000000; 
//	if (parentindex < nPersons) *error += 10; 
//	if (parentindex < nPersons && 
//       YOB[parentindex] != -1) *error += 100; 
//	if (parentindex < nPersons &&
//		YOB[parentindex] != -1 &&
//		childindex < nPersons && 
//		YOB[childindex] != -1) *error += 1000; 
//	if (parentindex < nPersons &&
//		YOB[parentindex] != -1 &&
//		childindex < nPersons && 
//		YOB[childindex] != -1 &&
//		YOB[parentindex] >= YOB[childindex]) *error += 1000; 
//	if (isChild[parentindex]) *error += 10000; 
//
//	return; 
//   }
   if ((parentindex < nPersons && 
       YOB[parentindex] != -1 &&
       childindex < nPersons &&
       YOB[childindex] != -1 &&
       YOB[parentindex] >= YOB[childindex])
	   || (parentindex < nPersons && isChild[parentindex]))
   {
      *error = 2; 
      return; 
   }
   ped->addRelation(parentindex, 
		    childindex, 
		    *error); 
   if (*error>0) *error = 3; 
}
/*
void FamInterface::AddExtraPerson(int mal, 
				  int index, 
				  int *error)
{
   if (index<0 || index>=pedset.getNumberOfPedigrees() ||
      mal<0 || mal>1)
   {
      *error = 1; 
      return; 
   }
   pedset.getPedigree(index)->addExtraPerson(mal); 
}

void FamInterface::RemoveExtraPerson(int person, 
				     int index, 
				     int *error)
{
   if (index<0 || index>=pedset.getNumberOfPedigrees())
   {
      *error = 1; 
      return; 
   }
   Pedigree* ped = pedset.getPedigree(index); 
   if (person >= ped->getPedigreeSize() || person < nPersons)
   {
      *error = 1; 
      return; 
   }
   ped->removePerson(person); 
}

void FamInterface::RemoveRelation(int parentindex, 
				  int childindex, 
				  int index, 
				  int *error)
{
   if (index<0 || index>=pedset.getNumberOfPedigrees())
   {
      *error = 1; 
      return; 
   }
   Pedigree* ped = pedset.getPedigree(index); 
   int size = ped->getPedigreeSize(); 
   if (parentindex<0 || parentindex>=size ||
       childindex<0 || childindex>=size)
   {
      *error = 1; 
      return; 
   }
   if (parentindex < nPersons && childindex < nPersons &&
       pedset.isFixedParent(parentindex, childindex))
   {
      *error = 2; 
      return; 
   }
   *error = 0; 
   ped->removeRelation(parentindex, 
		       childindex); 
}
*/
void FamInterface::AddAlleleSystem(int nAll, 
				   int lOfVector, 
				   double* mMatrixFemale, 
				   double* mMatrixMale, 
				   int sMutationMatrix, 
				   int nPossibilities, 
				   double *frequencies,
				   int hasSilentAllele,  
				   int *index, 
				   int *error)
{
   if (nAll < 1 ||
	   hasSilentAllele         < 0 ||
	   hasSilentAllele         > 1)
   {
      *error = 1; 
      return; 
   }
   //double sum = 0; 
   int i; 
   for (i=0; i<nAll; i++)
   {
      if (frequencies[i]<=0)
      {
	 *error = 1; 
	 return; 
      }
      //sum += frequencies[i]; 
   }
   //if (sum > 1.0000001)
   //{
   //   *error = 1; 
   //   return; 
   //}
   //if (sum > 0.99999999)
   //{
   //   if (nAll != nPossibilities) 
   //   {
   // *error = 1; 
	// return; 
   //   }
   //}
   //else 
   //   if (nAll == nPossibilities)
   //   {
	// *error = 1; 
	// return; 
   //   }
   
   char** newSystems = new char*[nSystems+1]; 
   int*  newAlleles  = new int[nSystems+1]; 
   char*** newintAll = new char**[nSystems+1]; 
   for (i=0; i<nSystems; i++)
   {
      newSystems[i] = internalSystemName[i]; 
      newAlleles[i] = nAlleles[i]; 
      newintAll[i] = internalAlleleName[i]; 
   }
   delete[] internalSystemName; 
   delete[] nAlleles; 
   delete[] internalAlleleName; 
   internalSystemName = newSystems; 
   nAlleles = newAlleles; 
   internalAlleleName = newintAll; 
   internalSystemName[nSystems] = new char[100]; 
   sprintf(internalSystemName[nSystems], "%d", ++counter); 
   nAlleles[nSystems] = nAll; 
   internalAlleleName[nSystems] = new char*[nAll]; 
   for (i=0; i<nAll; i++)
   {
      internalAlleleName[nSystems][i] = new char[100]; 
      sprintf(internalAlleleName[nSystems][i], "%d", ++counter); 
   }
   
   int paterError = 0; 
   pat.add_system(newcopy(internalSystemName[nSystems]), 
		  lOfVector, 
		  mMatrixFemale, 
		  mMatrixMale, 
		  sMutationMatrix, 
		  nPossibilities, 
		  0, 
		  paterError); 
   //removed 2012-03-16
   //   assert(paterError==0); //internal Error
   for (i=0; i<nAll; i++)
      pat.add_allele(newcopy(internalSystemName[nSystems]), 
		     newcopy(internalAlleleName[nSystems][i]), 
		     frequencies[i], 0, paterError);
   if (hasSilentAllele) 
	   pat.set_allele_as_silent(newcopy(internalSystemName[nSystems]), 
			newcopy(internalAlleleName[nSystems][nAll-1]), paterError); 
   //removed 2012-03-16
   //   assert(paterError==0); //internal Error
   *index = nSystems; 
   nSystems++; 
   *error = 0;
}
/*
void FamInterface::RemoveAlleleSystem(int index, int *error)
{
   if (index<0 || index>=nSystems)
   {
      *error = 1; 
      return; 
   }
   *error = 0;
   int paterError = 0; 
   pat.remove_system(newcopy(internalSystemName[index]), 
		     0, paterError); 
   //removed 2012-03-16
   //   assert(paterError==0); //internal error

   int i; 
   delete[] internalSystemName[index]; 
   for (i=0; i<nAlleles[index]; i++)
      delete[] internalAlleleName[index][i]; 
   delete[] internalAlleleName[index]; 
   for (i=index+1; i<nSystems; i++)
   {
      internalSystemName[i-1] = internalSystemName[i]; 
      nAlleles[i-1]           = nAlleles[i]; 
      internalAlleleName[i-1]  = internalAlleleName[i]; 
   }
   nSystems--; 
}

void FamInterface::EditAlleleSystem(int indexSystem, 
				    int nAll, 
				    double mutationRateFemale, 
				    double mutationRateMale, 
				    int mutationModelFemale, 
				    int mutationModelMale, 
				    int nPossibilities, 
				    double mutationRangeFemale, 
				    double mutationRangeMale, 
				    double *frequencies, 
				    int *correspondence, 
					int hasSilentAllele, 
				    int *error)
{
   if (indexSystem<0 || 
       indexSystem > nSystems ||
       nAll < 1 || 
       mutationRateFemale   < 0 || 
       mutationRateMale     < 0 || 
       mutationRateFemale  >= 1 ||
       mutationRateMale    >= 1 ||
       mutationModelFemale  < 0 ||
       mutationModelMale    < 0 ||
       mutationModelFemale  > 3 ||
       mutationModelMale    > 3 ||
     //  nPossibilities    < nAll ||
     //  nPossibilities       < 2 ||
       mutationRangeFemale <= 0 ||
       mutationRangeMale   <= 0 ||
       mutationRangeFemale >= 1 ||
       mutationRangeMale   >= 1 ||
	   hasSilentAllele      < 0 ||
	   hasSilentAllele      > 1)
   {
      *error = 1; 
      return; 
   }
	  
   //double sum = 0; 
   int i; 
   for (i=0; i<nAll; i++)
   {
      if (frequencies[i]<=0)
      {
	 *error = 1; 
	 return; 
      }
     // sum += frequencies[i]; 
   }
   //if (sum > 1.0000001)
   //{
   //   *error = 1; 
   //   return; 
   //}
   //if (sum > 0.99999999)
   //{
   //   if (nAll != nPossibilities) 
   //   {
	// *error = 1; 
	// return; 
   //   }
   //}
   //else 
   //   if (nAll == nPossibilities)
   //   {
	// *error = 1; 
	// return; 
   //   }

   int*  newAlls = new int[nAll]; 
   for (i=0; i<nAll; i++) newAlls[i] = 1; 
   for (i=0; i<nAlleles[indexSystem]; i++)
   {
      if (correspondence[i]<-1 || correspondence[i]>=nAll)
      {
	 *error = 1; 
	 delete[] newAlls; 
	 return; 
      }
	  if (correspondence[i]==-1) continue; 
      if (newAlls[correspondence[i]]==0)
      {
	 *error = 1; 
	 delete[] newAlls; 
	 return; 
      }
      newAlls[correspondence[i]] = 0; 
   }
   *error = 0;

   //Update pater data structure: 
   int paterError = 0; 
   for (i=0; i<nAlleles[indexSystem]; i++)
      if (correspondence[i]==-1) 
	 pat.remove_allele(newcopy(internalSystemName[indexSystem]),
			   newcopy(internalAlleleName[indexSystem][i]),
			   0, paterError); 
      else
	 //Reset frequency to temporary value, to avoid transition errors
	 pat.add_allele(newcopy(internalSystemName[indexSystem]), 
			newcopy(internalAlleleName[indexSystem][i]), 
			1e-30, 0, paterError); 
   //removed 2012-03-16
   //   assert(paterError==0);  //internal error
   //Adjust mutation rate, and n_alleles: 
   pat.add_system(newcopy(internalSystemName[indexSystem]), 
		  mutationRateFemale, 
		  mutationRateMale, 
		  mutationModelFemale, 
		  mutationModelMale, 
		  nPossibilities,
		  mutationRangeFemale, 
		  mutationRangeMale, 
		  0, paterError); 
   //removed 2012-03-16
   //   assert(paterError == 0); //internalError; 
   //finish pater update below...
   
   //Update data structure of this class:
   char** newAllName = new char*[nAll]; 
   for (i=0; i<nAlleles[indexSystem]; i++)
      if (correspondence[i]>=0)
	 newAllName[correspondence[i]] = 
	    internalAlleleName[indexSystem][i]; 
      else delete[] internalAlleleName[indexSystem][i]; 
   delete[] internalAlleleName[indexSystem]; 
   internalAlleleName[indexSystem] = newAllName; 
   for (i=0; i<nAll; i++)
      if (newAlls[i])
      {
	 internalAlleleName[indexSystem][i] = new char[100]; 
	 sprintf(internalAlleleName[indexSystem][i], "%d", ++counter); 
      }
   nAlleles[indexSystem] = nAll; 
   delete[] newAlls; 

   //finish updating pater data structure: 
   for (i=0; i<nAll; i++)
   {
	 pat.add_allele(newcopy(internalSystemName[indexSystem]),
			newcopy(internalAlleleName[indexSystem][i]), 
			frequencies[i], 
			0, paterError);
   }
   if (hasSilentAllele) 
	   pat.set_allele_as_silent(newcopy(internalSystemName[indexSystem]), 
			newcopy(internalAlleleName[indexSystem][nAll-1]), paterError); 
   //removed 2012-03-16
   //   assert(paterError == 0); //internal error. 
}
*/
void FamInterface::AddDNAObservation(int indexperson, 
				     int indexAlleleSystem, 
				     int indexAllele1, 
				     int indexAllele2, 
				     int *error)
{
   if (indexperson<0 || indexperson>=nPersons ||
       indexAlleleSystem<0 || indexAlleleSystem>=nSystems ||
       indexAllele1<0 || indexAllele1>=nAlleles[indexAlleleSystem] ||
       indexAllele2<0 || indexAllele2>=nAlleles[indexAlleleSystem])
   {
      *error = 1; 
      return; 
   }
   *error = 0;
   int paterError = 0; 
   pat.add_data(newcopy(internalSystemName[indexAlleleSystem]), 
		newcopy(internalPersonName[indexperson]), 
		newcopy(internalAlleleName[indexAlleleSystem][indexAllele1]), 
		newcopy(internalAlleleName[indexAlleleSystem][indexAllele2]), 
		0, paterError); 
   //removed 2012-03-16
   //   assert(paterError==0); //internal error
}
/*
void FamInterface::RemoveDNAObservation(int indexperson, 
					int indexAlleleSystem, 
					int *error)
{
   if (indexperson<0 || indexperson>=nPersons ||
       indexAlleleSystem<0 || indexAlleleSystem>=nSystems)
   {
      *error = 1; 
      return; 
   }
   *error = 0;
   int paterError = 0; 
   pat.remove_data(newcopy(internalSystemName[indexAlleleSystem]), 
		   newcopy(internalPersonName[indexperson]), 
		   0, paterError); 
   //removed 2012-03-16
   //   assert(paterError==0); //internal error
}
*/
void FamInterface::GetProbabilities(double generationsParameter, 
				    int maxGenerations, 
				    double inbreedingParameter, 
				    double promiscuityParameter, 
				    int usingDNAObservations, 
				    double kinship, 
				    int *redundant, 
				    double *probabilities, 
				    double *likelihoods, 
				    int *error)
{
   if (generationsParameter<0 ||
       maxGenerations < -1 ||
       inbreedingParameter<0 ||
       promiscuityParameter<0 ||
       kinship<0 ||
       kinship>=1)
   {
      *error = 1; 
      return; 
   }

   pedset.removeEquivalentPedigrees(redundant); 
   //int* isChild = new int[nPersons]; 
   //for (int i=0; i<nPersons; i++)
   //{
   //   if (YOB[i] != -1 && YOB[i] > referenceYear)
   //   {
//	 *error = 100000; 
//   delete[] isChild; 
//	 return; 
//      }
//      if (YOB[i] != -1 && YOB[i] > referenceYear - fertilityAge)
//	 isChild[i] = 1; 
//      else
//	 isChild[i] = 0; 
//   }
   *error = 0; 
   if (!pedset.computePrior(generationsParameter, 
			    maxGenerations, 
			    inbreedingParameter, 
			    promiscuityParameter, 
			    isChild, 
			    probabilities))
      *error = 2; 
   else if (usingDNAObservations && pat.get_number_of_systems()>0)
   {
      pat.setKinship(kinship); 
      if (!pedset.computePosterior(pat, 
				   kinship == 0, 
				   internalPersonName, 
				   probabilities, 
				   likelihoods))
	 *error = 2; 
   }
//   delete[] isChild; 
}
/*
void FamInterface::GetNumberOfSystems(int* number)
{
	*number = nSystems; 
}
*/
