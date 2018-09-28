/* $Id: PedigreeList.C,v 1.9 2001/04/08 14:04:57 mostad Exp $ */

#include <fstream>
#include <cstdlib>

#include "PedigreeList.h"
#include "Pedigree.h"
#include "GroupOfPersons.h"
#include "read.h"
#include "pater.h"
#include "special.h"

PedigreeList::PedigreeList(int Info)
{
   nNamedPersons = 0; 
   male = 0; 
   fixedParent = 0; 
   nPedigrees = 0; 
   first = 0; 
   last  = 0; 
   info = Info;
}

PedigreeList::~PedigreeList()
{
   delete[] male; 
   delete[] fixedParent; 
   while (first)
   {
      Pedigree* p = first; 
      first = (Pedigree*) first->getNext(); 
      delete p; 
   }
}

int PedigreeList::getNumberOfPedigrees()
{
   return nPedigrees; 
}

Pedigree* PedigreeList::getPedigree(int index)
{
   Pedigree* result = first; 
   for (int i=0; i<index; i++)
      result = (Pedigree*) result->getNext(); 
   return result; 
}

void PedigreeList::addPerson(int mal)
{
   nNamedPersons++; 
   int* newMale = new int[nNamedPersons]; 
   int* newFixedParent = new int[nNamedPersons*nNamedPersons]; 
   int i,j; 
   for (i=0; i<nNamedPersons-1; i++)
   {
      newMale[i] = male[i]; 
      for (j=0; j<nNamedPersons-1; j++)
	 newFixedParent[i+j*nNamedPersons] = 
	    fixedParent[i+j*(nNamedPersons-1)]; 
      newFixedParent[i+(nNamedPersons-1)*nNamedPersons] = 0; 
   }
   for (j=0; j<nNamedPersons; j++)
      newFixedParent[nNamedPersons-1+j*nNamedPersons] = 0 ; 
   newMale[nNamedPersons-1] = mal; 

   delete[] fixedParent; 
   delete[] male; 
   fixedParent = newFixedParent; 
   male = newMale; 

   Pedigree* p = first; 
   while (p)
   {
      p->addPerson(mal); 
      p = (Pedigree*) p->getNext(); 
   }
}

void PedigreeList::removePerson(int index)
{
   nNamedPersons--; 
   int* newFixedParent = new int[nNamedPersons*nNamedPersons]; 
   int i, j; 
   for (i=0; i<index; i++)
      for (j=0; j<index; j++)
	 newFixedParent[i+j*nNamedPersons] = 
	    fixedParent[i+j*(nNamedPersons+1)]; 
   for (i=index+1; i<=nNamedPersons; i++)
      for (j=0; j<index; j++)
	 newFixedParent[i-1+j*nNamedPersons] = 
	    fixedParent[i+j*(nNamedPersons+1)]; 
   for (i=0; i<index; i++)
      for (j=index+1; j<=nNamedPersons; j++)
	 newFixedParent[i+(j-1)*nNamedPersons] =
	    fixedParent[i+j*(nNamedPersons+1)]; 
   for (i=index+1; i<=nNamedPersons; i++)
      for (j=index+1; j<=nNamedPersons; j++)
	 newFixedParent[i-1+(j-1)*nNamedPersons] = 
	    fixedParent[i+j*(nNamedPersons+1)]; 
   delete[] fixedParent; 
   fixedParent = newFixedParent; 
   
   for (i=index+1; i<=nNamedPersons; i++)
      male[i-1] = male[i]; 

   Pedigree* p = first; 
   while (p)
   {
      p->removePerson(index); 
      p = (Pedigree*) p->getNext(); 
   }
}

void PedigreeList::addFixedRelation(int parentindex, 
				    int childindex, 
				    int* removed)
{
   int i; 
   if (fixedParent[parentindex+childindex*nNamedPersons]==0)
   {
      fixedParent[parentindex+childindex*nNamedPersons] = 1; 
      Pedigree* p = first; 
      for (i=0; i<nPedigrees; i++)
      {
	 p->addRelation(parentindex, childindex, removed[i]); 
	 p = (Pedigree*) p->getNext(); 
      }
      removePedigrees(removed); 
   }
   else
      for (i=0; i<nPedigrees; i++)
	 removed[i] = 0; 
}

void PedigreeList::removeFixedRelation(int parentindex, 
				       int childindex)
{
   if (fixedParent[parentindex+childindex*nNamedPersons])
   {
      fixedParent[parentindex+childindex*nNamedPersons] = 0; 
      Pedigree* p = first; 
      while (p)
      {
	 p->removeRelation(parentindex, childindex); 
	 p = (Pedigree*) p->getNext(); 
      }
   }
}

int PedigreeList::isFixedParent(int parentindex, int childindex) const
{
   return fixedParent[parentindex+childindex*nNamedPersons]; 
}

void PedigreeList::addPedigree(int nExtraFemales, 
			       int nExtraMales)
{
   int nTotal = nNamedPersons+nExtraFemales+nExtraMales; 
   int* fullMatrix = new int[nTotal*nTotal]; 
   for (int i=0; i<nTotal; i++)
      for (int j=0; j<nTotal; j++)
	 if (i<nNamedPersons && j<nNamedPersons)
	    fullMatrix[i+j*nTotal] =
	       fixedParent[i+j*nNamedPersons]; 
	 else
	    fullMatrix[i+j*nTotal] = 0; 
   addPedigree(new Pedigree(nNamedPersons, 
			    nExtraFemales, 
			    nExtraMales, 
			    male, 
			    fullMatrix)); 
   delete[] fullMatrix; 
}

void PedigreeList::addPedigree(Pedigree* p)
{
   if (last)
      last->append(p); 
   else
      first = p; 
   last = p; 
   nPedigrees++; 
}

void PedigreeList::removePedigree(int index)
{
   Pedigree* p = first; 
   if (index==0)
   {
      if (first==last)
      {
	 delete first; 
	 first = last = 0; 
      }
      else
      {
	 first = (Pedigree*) p->getNext(); 
	 delete p; 
      }
   }
   else
   {
      while (--index > 0)
	 p = (Pedigree*) p->getNext(); 
      if (((Pedigree*)p->getNext())==last)
	 last = p; 
      p->deleteNext(); 
   }
   nPedigrees--; 
}

void PedigreeList::removeEquivalentPedigrees(int* removed)
{
   int i;
   int counter = 0; 
   Pedigree* p = first; 
   Pedigree* pred = 0; 
   for (i=0; i<nPedigrees; i++)
   {
      p->pruneAndRemove(); 
      p->changeToStandardForm(); 
      Pedigree* q = first; 
      while (q != p)
      {
	 if (p->isEqualTo(q))
	    break; 
	 q = (Pedigree*) q->getNext(); 
      }
      if (p==q) 
      {
	 pred = p; 
	 counter++; 
	 removed[i] = 0; 
      }
      else
      {
	 if (((Pedigree*) pred->getNext())==last)
	    last = pred; 
	 pred->deleteNext(); 
	 removed[i] = 1; 
      }
      p = (Pedigree*) pred->getNext(); 
   }
   nPedigrees = counter; 
}

void PedigreeList::generatePedigrees(int nExtraFemales, 
				     int nExtraMales, 
				     int* possibleParent)
{
   int nTotal = nNamedPersons + nExtraFemales + nExtraMales; 
   int* parent = new int[nTotal*nTotal]; 
   for (int i=0; i<nTotal; i++)
      for (int j= 0; j<nTotal; j++)
	 if (i<nNamedPersons && j<nNamedPersons)
	    parent[i+j*nTotal] = 
	       fixedParent[i+j*nNamedPersons]; 
	 else
	    parent[i+j*nTotal] = 0; 
   generateParentsForPerson(0, 
			    parent, 
			    nTotal, 
			    nExtraFemales, 0, 
			    nExtraMales, 0, 
			    possibleParent); 
   delete[] parent; 
}

int PedigreeList::computePrior(double generationsParameter, 
			       int maxGenerations, 
			       double inbreedingParameter, 
			       double promiscuityParameter, 
			       int* isChild, 
			       double* probabilities)
{
   int i = 0; 
   double sum = 0; 
   for (i=0; i<nPedigrees; i++)
      probabilities[i] = 1.0; 
   Pedigree* p = first; 
   i = 0; 
   while (p)
   {
      int nGen = p->computeGenerations(isChild); 
      if (maxGenerations != -1 && nGen > maxGenerations) 
	 probabilities[i] = 0; 
      else
      {
	 if (generationsParameter != 1) 
	    probabilities[i] *= mypow(generationsParameter, nGen); 
	 if (inbreedingParameter != 1)
	    probabilities[i] *= mypow(inbreedingParameter, 
				    p->computeInbreeding()); 
	 if (promiscuityParameter != 1)
	    probabilities[i] *= mypow(promiscuityParameter, 
				    p->computePromiscuity()); 
      }
      sum += probabilities[i]; 
      //removed[i] = (probabilities[i]==0.0); 
      i++; 
      p = (Pedigree*) p->getNext(); 
   }
   if (sum == 0.0) return 0; 
   //removePedigrees(removed); 
   for (i=0; i<nPedigrees; i++)
      probabilities[i] /= sum; 
   return 1; 
}

int PedigreeList::computePosterior(pater& pat, 
				   int makeCutsets, 
				   char** names, 
				   double* probabilities, 
				   double* likelihoods)
{
   char* namePrefix = getNamePrefix(nNamedPersons, names); 
//   int i = 0; 
   Pedigree* p = first; 
//   double sum = 0; 
   int counter = 0; 
   while (p)
   {
	   double* result = p->computeProbability(pat, 
						fixedParent, 
						names, 
						namePrefix, 
						makeCutsets, 
						0); 
		int nSystems = pat.get_number_of_systems(); 
		int j; 
		for (j=0; j<nSystems; j++) {
			likelihoods[counter] = result[j]; 
			counter++; 
		//	probabilities[i] *= result[j]; 
		}
		delete[] result; 
	  //probabilities[i] *= likelihoods[i]; 
//      sum += probabilities[i]; 
      //removed[i] = (probabilities[i]==0.0); 
//      i++; 
      p = (Pedigree*) p->getNext(); 
   }
   delete[] namePrefix; 
//   if (sum ==0.0) return 0; 
   //removePedigrees(removed); 
//   for (i=0; i<nPedigrees; i++)
//      probabilities[i] /= sum; 
   return 1; 
}

//RECURSIVE ALGORITHM GENERATING PEDIGREES: 
void PedigreeList::generateParentsForPerson(int i, 
					    int* parent, 
					    int nTotal, 
					    int nExtraFemales, 
					    int nExtraFemalesUsed, 
					    int nExtraMales, 
					    int nExtraMalesUsed, 
					    int* possibleParent)
{
   if (i==nTotal)
   {
      //Generate pedigree on standard form: 
      Pedigree* p = new Pedigree(nNamedPersons, 
				 nExtraFemales, 
				 nExtraMales, 
				 male, 
				 parent); 

      p->pruneAndRemove(); 
      p->changeToStandardForm(); 

      Pedigree* q = first; 
      while (q)
      {
	 if (p->isEqualTo(q))
	 {
	    delete p; 
	    return; 
	 }
	 q = (Pedigree*) q->getNext(); 
      }
      nPedigrees++; 
      if (last)
      {
	 last->append(p); 
	 last = p; 
      }
      else
	 first = last = p; 
      if (info && nPedigrees%500==0)
      {
// Removed 2012-03-08
//	 cout<<"Adding pedigree nr. "<<nPedigrees<<'\n';
	 p->print(); 
// Removed 2012-03-08
//	 cout.flush(); 
      }
      return; 
   }

   //Try with no (new) mother: 
   generateFatherForPerson(i, 
			   parent, 
			   nTotal, 
			   nExtraFemales, 
			   nExtraFemalesUsed, 
			   nExtraMales, 
			   nExtraMalesUsed, 
			   possibleParent); 


   int j; 
   if (i<nNamedPersons) //May already have a mother: 
      for (j=0; j<nNamedPersons; j++)
	 if (parent[j+i*nTotal] && !male[j]) 
	    return; //finished
   

   //Try with a previously mentioned mother: 
   for (j=0; j<nNamedPersons+nExtraFemalesUsed; j++)
      if ((j>=nNamedPersons || !male[j]) && 
	  isPossibleParent(j, i, nTotal, parent, possibleParent))
      {
	 parent[j+i*nTotal] = 1; 
	 generateFatherForPerson(i, 
				 parent, 
				 nTotal, 
				 nExtraFemales, 
				 nExtraFemalesUsed, 
				 nExtraMales, 
				 nExtraMalesUsed, 
				 possibleParent); 
	 parent[j+i*nTotal] = 0; 
      }

   //Try with a new, "extra", mother:
   if (nExtraFemalesUsed < nExtraFemales)
      if (isPossibleParent(nNamedPersons + nExtraFemalesUsed, 
			   i, nTotal, parent, possibleParent))
      {
	 parent[nNamedPersons + nExtraFemalesUsed + i*nTotal] = 1; 
	 generateFatherForPerson(i, 
				 parent, 
				 nTotal, 
				 nExtraFemales, 
				 nExtraFemalesUsed + 1, 
				 nExtraMales, 
				 nExtraMalesUsed, 
				 possibleParent); 
	 parent[nNamedPersons + nExtraFemalesUsed + i*nTotal] = 0; 
      }
}
   
 
void PedigreeList::generateFatherForPerson(int i, 
					   int* parent,
					   int nTotal, 
					   int nExtraFemales, 
					   int nExtraFemalesUsed, 
					   int nExtraMales, 
					   int nExtraMalesUsed, 
					   int* possibleParent)
{
   //Try with no (new) father: 
   generateParentsForPerson(i + 1, 
			    parent, 
			    nTotal, 
			    nExtraFemales, 
			    nExtraFemalesUsed, 
			    nExtraMales, 
			    nExtraMalesUsed, 
			    possibleParent); 
   
   int j; 
   if (i<nNamedPersons) //May already have a father: 
      for (j=0; j<nNamedPersons; j++)
	 if (parent[j+i*nTotal] && male[j]) 
	    return; //finished

   //Try with a previously mentioned father: 
   for (j=0; j<nNamedPersons + nExtraFemales + nExtraMalesUsed; j++)
      if ((j<nNamedPersons && male[j]) || j >= nNamedPersons + nExtraFemales)
	 if (isPossibleParent(j, i, nTotal, parent, possibleParent))
	 {
	    parent[j+i*nTotal] = 1; 
	    generateParentsForPerson(i + 1, 
				     parent, 
				     nTotal, 
				     nExtraFemales, 
				     nExtraFemalesUsed, 
				     nExtraMales, 
				     nExtraMalesUsed, 
				     possibleParent); 
	    parent[j+i*nTotal] = 0; 
	 }
   
   //Try with a new, "extra", father:
   if (nExtraMalesUsed < nExtraMales)
      if (isPossibleParent(nNamedPersons + nExtraFemales + nExtraMalesUsed, 
			   i, nTotal, parent, possibleParent))
      {
	 parent[nNamedPersons + nExtraFemales + nExtraMalesUsed + i*nTotal] = 1; 
	 generateParentsForPerson(i + 1, 
				  parent, 
				  nTotal, 
				  nExtraFemales, 
				  nExtraFemalesUsed, 
				  nExtraMales, 
				  nExtraMalesUsed + 1, 
				  possibleParent); 
	 parent[nNamedPersons + nExtraFemales + nExtraMalesUsed + i*nTotal] = 0; 
      }
}

int PedigreeList::isAncestor(int childindex, 
                             int parentindex, 
                             int* isParent, 
                             int nTotal)
{
   if (childindex==parentindex) return 1; 
   for (int k=0; k<nTotal; k++)
      if (isParent[k+parentindex*nTotal] && 
	  isAncestor(childindex, k, isParent, nTotal))
	 return 1; 
   return 0; 
}

int PedigreeList::isPossibleParent(int j, 
				   int i, 
				   int nTotal, 
				   int* parent, 
				   int* possibleParent)
{
   if (possibleParent[j+i*nTotal]<1) return 0; 
   if (!checkParents(j, i, 2, nTotal, parent, possibleParent)) return 0; 
   if (!checkChildren(j, i, 2, nTotal, parent, possibleParent)) return 0; 
   return !isAncestor(i, j, parent, nTotal); 
}

int PedigreeList::checkParents(int j, 
			       int i, 
			       int nGenerations, 
			       int nTotal, 
			       int* parent, 
			       int* possibleParent)
{
   for (int k=0; k<nTotal; k++)
      if (parent[k+j*nTotal])
      {
	 if (possibleParent[k+i*nTotal]<nGenerations) return 0; 
	 if (!checkParents(k, i, nGenerations+1, nTotal, 
			   parent, possibleParent)) return 0; 
      }
   return 1; 
}

int PedigreeList::checkChildren(int j, 
				int i, 
				int nGenerations, 
				int nTotal, 
				int* parent, 
				int* possibleParent)
{
   for (int k=0; k<nTotal; k++)
      if (parent[i+k*nTotal])
      {
	 if (possibleParent[j+k*nTotal]<nGenerations) return 0; 
	 if (!checkChildren(j, k, nGenerations+1, nTotal, 
			    parent, possibleParent)) return 0; 
      }
   return 1; 
}

void PedigreeList::removePedigrees(int* removed)
{
   int i; 
   Pedigree* p; 
   int counter = nPedigrees; 
   for (i=0; i<nPedigrees; i++)
      if (removed[i])
      {
	 if (first==last)
	 {
	    delete first; 
	    counter--; 
	    first = 0; 
	    last = 0; 
	 }
	 else
	 {
	    p = first; 
	    first = (Pedigree*) p->getNext(); 
	    delete p; 
	    counter--; 
	 }
      }
      else break; 

   p = first; 
   while (i < nPedigrees-1)
      if (removed[++i])
      {
	 if (((Pedigree*) p->getNext())==last)
	    last = p; 
	 p->deleteNext(); 
	 counter--; 
      }
      else
	 p = (Pedigree*) p->getNext(); 
   nPedigrees = counter; 
}
