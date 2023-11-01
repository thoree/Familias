/* $Id: Pedigree.C,v 1.14 2001/04/08 14:04:56 mostad Exp $ */

#include <cassert>
#include <cstdio>

#include "Pedigree.h"
#include "pater.h"
#include "special.h"

Pedigree::Pedigree(int nNamedPers, 
		   int nExtraFemales, 
		   int nExtraMales, 
		   int* mal, 
		   int* par)
{
   nNamedPersons = nNamedPers; 
   nTotal = nNamedPersons + nExtraMales + nExtraFemales; 

   male = new int[nTotal]; 
   int i, j; 
   for (i=0; i<nNamedPersons; i++)
      male[i] = mal[i]; 
   for (i=0; i<nExtraFemales; i++)
      male[nNamedPersons+i] = 0; 
   for (i=0; i<nExtraMales; i++)
      male[nNamedPersons+nExtraFemales+i] = 1; 
   
//    parent = new int*[nTotal]; 
//    for (i=0; i<nTotal; i++)
//    {
//       parent[i] = new int[nTotal]; 
//       for (j=0; j<nTotal; j++)
// 	 parent[i][j] = par[i+j*nTotal]; 
//    }
   father = new int[nTotal]; 
   mother = new int[nTotal]; 
   for (i=0; i<nTotal; i++)
   {
      father[i] = -1; 
      mother[i] = -1; 
      for (j=0; j<nTotal; j++)
	 if (par[j+i*nTotal])
	    if (male[j]) 
	       father[i] = j; 
	    else 
	       mother[i] = j; 
   }
}

Pedigree::Pedigree(int nNamedPers, 
		   int* mal, //dimension nNamedPersons
		   int* isChild, //dimesion nNamedPersons
		   ifstream& is, 
		   int& OK)
{
   OK = 1; 
   nNamedPersons = nNamedPers; 
   
   int nExtraMal = 0, nExtraFem = 0; 
   is>>nExtraFem>>nExtraMal; 
   if (nExtraFem<0 || nExtraMal<0)
   {
      OK = 0;
      return; 
   }

   nTotal = nNamedPersons + nExtraMal + nExtraFem; 
   male = new int[nTotal]; 
   int i; 
   for (i=0; i<nNamedPersons; i++)
      male[i] = mal[i]; 
   for (i=0; i<nExtraFem; i++)
      male[nNamedPersons+i] = 0; 
   for (i=0; i<nExtraMal; i++)
      male[nNamedPersons+nExtraFem+i] = 1; 
   
   father = new int[nTotal]; 
   mother = new int[nTotal]; 
//   parent = new int*[nTotal]; 

//    for (i=0; i<nTotal; i++)
//    {
//       parent[i] = new int[nTotal]; 
//       for (j=0; j<nTotal; j++)
// 	 parent[i][j] = 0; 
//    }
   for (i=0; i<nTotal; i++)
   {
      is>>mother[i]>>father[i]; 
      if (mother[i]<-1 || father[i]<-1 ||
	  mother[i]>=nTotal || father[i]>=nTotal ||
	  (mother[i]>=0 && mother[i]<nNamedPersons && 
	   isChild[mother[i]]) ||
	  (father[i]>=0 && father[i]<nNamedPersons && 
	   isChild[father[i]]) ||
	  (mother[i]>=0 && male[mother[i]]) ||
	  (father[i]>=0 && !male[father[i]]))
      {
	 OK = 0; 
	 return; 
      }
//       if (mother[i]>-1) parent[mother[i]][i] = 1; 
//       if (father[i]>-1) parent[father[i]][i] = 1; 
   }
}

// Pedigree::Pedigree(Element* nx, GroupOfPersons* gp, int* par) :
//    Element(nx)
// {
//    nNamedPersons = gp->getNamedPersons(); 
//    nTotal        = gp->getTotalSize();
   
//    male   = new int[nTotal]; 
//    father = new int[nTotal]; 
//    mother = new int[nTotal]; 
//    parent = new int*[nTotal]; 
//    int i, j; 
//    for (i=0; i<gp->getNamedWomen(); i++)
//       male[i] = 0; 
//    for (i=0; i<gp->getNamedMen(); i++)
//       male[gp->getNamedWomen()+i] = 1; 
//    for (i=0; i<gp->getNamedChildren(); i++)
//       male[gp->getNamedAdults()+i] = 1; 
//    for (i=0; i<gp->getExtraWomen(); i++)
//       male[gp->getNamedPersons()+i] = 0; 
//    for (i=0; i<gp->getExtraMen(); i++)
//       male[gp->getNamedPersons()+gp->getExtraWomen()+i] = 1; 

//    for (i=0; i<nTotal; i++)
//    {
//       parent[i] = new int[nTotal]; 
//       for (j=0; j<nTotal; j++)
// 	 parent[i][j] = par[i+j*nTotal]; 
//    }
//    for (i=0; i<nTotal; i++)
//    {
//       father[i] = -1; 
//       mother[i] = -1; 
//    }
//    for (i=0; i<nTotal; i++)
//       for (j=0; j<nTotal; j++)
// 	 if (parent[i][j])
// 	    if (male[i]) 
// 	       father[j] = i; 
// 	    else 
// 	       mother[j] = i; 
// }

Pedigree::~Pedigree()
{
//   for (int i=0; i<nTotal; i++) 
//     delete[] parent[i]; 
//   delete[] parent; 
   delete[] male; 
   delete[] father; 
   delete[] mother; 
}

void Pedigree::write(ofstream& os) const
{
   os.width(2); 
   os<<getNumberOfExtraFemales()<<' '; 
   os.width(2); 
   os<<getNumberOfExtraMales()<<'\n'; 
   for (int j=0; j<nTotal; j++)
   {
      os.width(2); 
      os<<mother[j]<<' ';
      os.width(2); 
      os<<father[j]<<'\n'; 
   }
   os<<'\n';
}

void Pedigree::print() const
{
// Removed 2012-03-08
/*
   cout.width(2); 
   cout<<getNumberOfExtraFemales()<<' '; 
   cout.width(2);    
   cout<<getNumberOfExtraMales()<<'\n'; 
   for (int j=0; j<nTotal; j++)
   {
      cout.width(2); 
      cout<<mother[j]<<' '; 
      cout.width(2); 
      cout<<father[j]<<'\n';
   }
   cout<<'\n';
*/
}

int Pedigree::isEqualTo(Pedigree* q) const
{
   if (nTotal != q->nTotal) return 0; 
   if (nNamedPersons != q->nNamedPersons) return 0; 
   int i; 
   for (i=0; i<nTotal; i++)
   {
      if (mother[i] != q->mother[i]) return 0; 
      if (father[i] != q->father[i]) return 0; 
      if (male[i]   != q->male[i]  ) return 0; 
   }
   return 1; 
}

// int Pedigree::equals(Pedigree* pg)
// {
//    if (nMenTotal != pg->nMenTotal) return 0; 
//    if (nExtraMen != pg->nExtraMen) return 0; 
//    if (nWomenTotal != pg->nWomenTotal) return 0; 
//    if (nExtraWomen != pg->nExtraWomen) return 0; 
////    for (int i=0; i<nTotal*nTotal; i++)
////       if (parent[i] != pg->parent[i]) return 0; 
//    return 1;
// }

int Pedigree::unsafeEquals(Pedigree* pg) const
{
   for (int i=0; i<nTotal; i++)
   {
      if (mother[i] != pg->mother[i]) return 0; 
      if (father[i] != pg->father[i]) return 0; 
   }
   return 1; 
}

int Pedigree::computeInbreeding() const
{
   int nInbreedings = 0; 
   for (int i=0; i<nTotal; i++)
      if (father[i]!=-1 && mother[i]!=-1)
	 nInbreedings += hasCommonAncestor(father[i], mother[i]); 
   return nInbreedings; 
}

//Measures the number of pairs of persons p1 and p2 such that: 
//p1 and p2 have one common parent, but the other parent is either
//different for the two, or not in the pedigree for one or both of them.   
int Pedigree::computePromiscuity() const
{
   int nPairs = 0; 
   for (int i=0; i<nTotal; i++)
      for (int j=0; j<i; j++) //We use j<i to avoid counting pairs twice. 
	 if (mother[i]>=0 && mother[i]==mother[j])
	    nPairs += (father[i] != father[j] || (father[i]==-1 && father[j]==-1)); 
	 else if (father[i]>=0 && father[i]==father[j])
	    nPairs += (mother[i] != mother[j] || (mother[i]==-1 && mother[j]==-1)); 
   return nPairs; 
}

//compute the maximal number of parent-child links connecting two 
//persons who are both adults and both non-extras. 
int Pedigree::computeGenerations(int* isChild) const
{
   int maxGen = 0; 
   int thisGen; 
   for (int i=0; i<nNamedPersons; i++)
      if (!isChild[i])
      {
	 thisGen = getMaxGenerations(i); 
	 if (thisGen>maxGen)
	    maxGen = thisGen; 
      }
   return maxGen; 
}

double* Pedigree::computeProbability(pater& pat, 
				    int* fixedParent, 
				    char** nams, 
				    char* namePrefix, 
				    int makeCutsets, 
				    int info) const
{
   //make names for extras: 
   char** names = new char*[nTotal]; 
   int i; 
   int error = 0; 
   for (i=0; i < nNamedPersons; i++)
      names[i] = newcopy(nams[i]); 
   for (i=nNamedPersons; i<nTotal; i++)
   {
      names[i] = new char[100]; 
      snprintf(names[i], 100, "%s%d", namePrefix, i);
      pat.add_person(male[i], newcopy(names[i]), 0, error); 
   }
   //removed 2012-03-16
   //   assert(!error); 
   for (i=0; i<nTotal; i++)
   {
      if (mother[i] >= 0 && 
	  !(mother[i]<nNamedPersons && 
	    i        <nNamedPersons &&
	    fixedParent[mother[i]+i*nNamedPersons]))
	 pat.add_parent(newcopy(names[mother[i]]), newcopy(names[i]), 0, error); 
      if (father[i] >= 0 && 
	  !(father[i]<nNamedPersons && 
	    i        <nNamedPersons &&
	    fixedParent[father[i]+i*nNamedPersons]))
	 pat.add_parent(newcopy(names[father[i]]), newcopy(names[i]), 0, error); 
// Removed 2012-03-08
//      if (error && info)
//      {
//	 cout<<"ERROR: In family structure of pedigree.\n";
//	 return 0; 
//      }
   }
   //removed 2012-03-16
   //   assert(!error); 
   if (makeCutsets)
   {
      int* prune = getCutsets(); 
      for (i=0; i<nTotal; i++)
      {
   //removed 2012-03-16
	//	 assert(prune[i]!=1); 
	 if (prune[i]==2)
	 {
	    pat.add_person_to_cutset(newcopy(names[i]), 0, error); 
	    pat.end_cutset(0, error); 
	 }
      }
      delete[] prune; 
   }
   //removed 2012-03-16
   //   assert(!error); 
// Removed 2012-03-08
//   if (info) pat.write_family(cout); 

   pat.execute(info, error); 
   //removed 2012-03-16
   //   assert(!error); 
   double* result = pat.getResults(0, error); 
   //removed 2012-03-16
   //   assert(!error); 
   pat.remove_cutsets(); 
   for (i=0; i<nTotal; i++)
   {
      if (mother[i] >= 0 && 
	  !(mother[i]<nNamedPersons && 
	    i        <nNamedPersons &&
	    fixedParent[mother[i]+i*nNamedPersons]))
	 pat.remove_possible_parent(newcopy(names[mother[i]]), 
				    newcopy(names[i])); 
      if (father[i] >= 0 && 
	  !(father[i]<nNamedPersons && 
	    i        <nNamedPersons &&
	    fixedParent[father[i]+i*nNamedPersons]))
	 pat.remove_possible_parent(newcopy(names[father[i]]), 
				    newcopy(names[i])); 
   }  

   //removed 2012-03-16
   //   assert(!error); 
   for (i=nNamedPersons; i<nTotal; i++)
      pat.remove_person(male[i], newcopy(names[i]), 0, error); 
   //removed 2012-03-16
   //   assert(!error); 
   for (i=0; i<nTotal; i++)
      delete[] names[i]; 
   delete[] names; 
   return result; 
}

// double Pedigree::computeProbability(pater* pat, 
// 				    char** names, //length nTotal
// 				    int info, 
// 				    int& error) const
// {
//    int i,j; 
//    for (i=0; i<nTotal; i++)
//       for (j=0; j<nTotal; j++)
// 	 if (parent[i][j])
// 	 {
// 	    int error; 
// 	    pat->add_parent(newcopy(names[i]), 
// 			    newcopy(names[j]), info, error); 
// 	 }
//    if (error) return 0; 
//    int* prune = getCutsets(); 
//    for (i=0; i<nTotal; i++)
//       if (prune[i]==1) pat->remove_person(male[i], 
// 					  newcopy(names[i]), info, error); 
//    if (error)
//    {
//       delete[] prune; 
//       return 0; 
//    }
//    for (i=0; i<nTotal; i++)
//       if (prune[i]==2)
//       {
// 	 pat->add_person_to_cutset(newcopy(names[i]), info, error); 
// 	 pat->end_cutset(info, error); 
//       }
//    if (error) 
//    {
//       delete[] prune; 
//       return 0; 
//    }
//    pat->write_family(cout); 
//    pat->execute(info, error); 
//    double result = pat->getResults(info, error); 
//    if (error)
//    {
//       delete[] prune; 
//       return 0; 
//    }
//    pat->remove_cutsets(); 
//    for (i=0; i<nTotal; i++)
//       if (prune[i]==1) pat->add_person(male[i], 
// 				       newcopy(names[i]), info, error); 
//    for (i=0; i<nTotal; i++)
//       for (j=0; j<nTotal; j++)
// 	 if (parent[i][j])
// 	    pat->remove_possible_parent(newcopy(names[i]), 
// 					newcopy(names[j])); 
//    if (error)
//    {
//       delete[] prune; 
//       return 0; 
//    }

//    delete[] prune; 
//    return result; 
// }

void Pedigree::getPedigree(int* matrix) const
{
   int i; 
   for (i=0; i<nTotal*nTotal; i++)
      matrix[i] = 0; 
   for (i=0; i<nTotal; i++)
   {
      if (mother[i]>=0)
	 matrix[mother[i]+i*nTotal] = 1; 
      if (father[i]>=0)
	 matrix[father[i]+i*nTotal] = 1; 
   }
}

void Pedigree::getParents(int* moth, int* fath) const
{
   for (int i=0; i<nTotal; i++)
   {
      moth[i] = mother[i]; 
      fath[i] = father[i]; 
   }
}

int Pedigree::getPedigreeSize() const
{
   return nTotal; 
}

int Pedigree::getNumberOfExtraFemales() const
{
   int result = 0; 
   for (int i=nNamedPersons; i<nTotal; i++)
      result += !male[i]; 
   return result; 
}

int Pedigree::getNumberOfExtraMales() const
{
   int result = 0; 
   for (int i=nNamedPersons; i<nTotal; i++)
      result += male[i]; 
   return result; 
}

int Pedigree::onStandardForm() const
{
   int counterExtraFemales = nNamedPersons; 
   int counterExtraMales   = nNamedPersons + getNumberOfExtraFemales(); 
   for (int i=0; i<nTotal; i++)
   {
      if (mother[i] >= counterExtraFemales && 
	  i < counterExtraFemales &&
	  mother[i] > counterExtraFemales++) 
	 return 0; 
      if (father[i] >= counterExtraMales && 
	  i < counterExtraMales &&
	  father[i] > counterExtraMales++) 
	 return 0; 
   }
   return 1; 
}

void Pedigree::changeToStandardForm()
{
   int* permutation = new int[nTotal+1]; 
   int* invPerm     = new int[nTotal+1]; 
   int i;
   int madeChange = 0; 

   for (i=0; i<nTotal+1; i++)
      invPerm[i] = permutation[i] = i-1; 
   permutation++; 
   invPerm++; 
   int counterExtraFemales = nNamedPersons; 
   int counterExtraMales   = nNamedPersons + getNumberOfExtraFemales(); 
   for (i=0; i<nTotal; i++)
   {
      if (invPerm[mother[permutation[i]]] >= counterExtraFemales &&
	  i < counterExtraFemales &&
	  invPerm[mother[permutation[i]]] > counterExtraFemales++)
      {
	 madeChange = 1; 
	 int a = mother[permutation[i]];
	 int b = invPerm[a]; 
	 int c = counterExtraFemales-1; 
	 int d = permutation[c]; 

	 invPerm[a] = c; 
	 invPerm[d] = b; 
	 permutation[c] = a; 
	 permutation[b] = d; 
      }
      if (invPerm[father[permutation[i]]] >= counterExtraMales &&
	  i < counterExtraMales &&
	  invPerm[father[permutation[i]]] > counterExtraMales++)
      {
	 madeChange = 1; 
	 int a = father[permutation[i]];
	 int b = invPerm[a]; 
	 int c = counterExtraMales-1; 
	 int d = permutation[c]; 

	 invPerm[a] = c; 
	 invPerm[d] = b; 
	 permutation[c] = a; 
	 permutation[b] = d; 
      }
   }
   
   if (madeChange)
   {
      //Apply permutation to data structure: 
      int* newMother = new int[nTotal]; 
      int* newFather = new int[nTotal]; 
      for (i=0; i<nTotal; i++)
      {
	 newMother[i] = invPerm[mother[permutation[i]]]; 
	 newFather[i] = invPerm[father[permutation[i]]]; 
      }
      delete[] father; 
      delete[] mother; 
      father = newFather; 
      mother = newMother; 
//       for (i=0; i<nTotal; i++)
//       {
// 	 for (j=0; i<nTotal; j++)
// 	    parent[j][i] = 0; 
// 	 if (mother[i]>-1) parent[mother[i]][i] = 1; 
// 	 if (father[i]>-1) parent[father[i]][i] = 1; 
//       }
    }

   permutation--; 
   invPerm--; 
   delete[] permutation; 
   delete[] invPerm; 
}

void Pedigree::addPerson(int mal)
{
   int* newMale = new int[nTotal+1]; 
   int* newMother = new int[nTotal+1]; 
   int* newFather = new int[nTotal+1]; 
   int i; 
   for (i=0; i<nTotal; i++)
   {
      if (mother[i]>=nNamedPersons) mother[i]++; 
      if (father[i]>=nNamedPersons) father[i]++; 
   }
   for (i=0; i<nNamedPersons; i++)
   {
      newMale[i] = male[i];
      newMother[i] = mother[i]; 
      newFather[i] = father[i]; 
   }
   for (i=nNamedPersons; i<nTotal; i++)
   {
      newMale[i+1] = male[i]; 
      newMother[i+1] = mother[i]; 
      newFather[i+1] = father[i]; 
   }
   newMale[nNamedPersons] = mal; 
   newMother[nNamedPersons] = -1; 
   newFather[nNamedPersons] = -1; 
   delete[] male; 
   delete[] mother; 
   delete[] father; 
   male = newMale; 
   mother = newMother; 
   father = newFather;
   
   nTotal++; 
   nNamedPersons++; 
}

void Pedigree::addExtraPerson(int mal)
{
   int* newMale = new int[nTotal+1]; 
   int* newMother = new int[nTotal+1]; 
   int* newFather = new int[nTotal+1]; 
   int i; 
   int indexNew; 
   if (mal)
      indexNew = nTotal; 
   else
   {
      indexNew = nNamedPersons; 
      while (indexNew<nTotal && !male[indexNew]) indexNew++; 
      for (i=0; i<nTotal; i++)
      {
	 if (mother[i]>=indexNew) mother[i]++; 
	 if (father[i]>=indexNew) father[i]++; 
      }
   }

   for (i=0; i<indexNew; i++)
   {
      newMale[i] = male[i];
      newMother[i] = mother[i]; 
      newFather[i] = father[i]; 
   }
   newMale[indexNew] = mal; 
   newMother[indexNew] = -1; 
   newFather[indexNew] = -1; 
   for (i=indexNew; i<nTotal; i++)
   {
      newMale[i+1] = male[i];
      newMother[i+1] = mother[i]; 
      newFather[i+1] = father[i]; 
   }

   delete[] male; 
   delete[] mother; 
   delete[] father; 
   male = newMale; 
   mother = newMother; 
   father = newFather;
   nTotal++; 
}

void Pedigree::removePerson(int index)
{
   int i; 
   for (i=0; i<nTotal; i++)
   {
      if (father[i]>index) father[i]--; 
      else if (father[i]==index) father[i] = -1; 
      if (mother[i]>index) mother[i]--; 
      else if (mother[i]==index) mother[i] = -1; 
   }
   for (i=index+1; i<nTotal; i++)
   {
      male[i-1] = male[i]; 
      father[i-1] = father[i]; 
      mother[i-1] = mother[i]; 
   }
   if (index < nNamedPersons) nNamedPersons--; 
   nTotal--; 
}

int* Pedigree::getPruning() const
{
   int* result = new int[nTotal]; 
   int i, j; 
   for (i=0; i<nTotal; i++)
      result[i] = 0; 
 
   int finished; 
   do
   {
      finished = 1; 
      //remove extra persons who have no children: 
      for (i = nNamedPersons; i<nTotal; i++)
	 if (result[i]==0)
	 {
	    for (j=0; j<nTotal; j++)
	       if ((mother[j]==i || father[j]==i) && result[j]==0) break; 
	    if (j==nTotal)
	    {
	       result[i] = 1;
	       finished = 0; 
	    }
	 }
      //remove extra persons who have no parents, and at most one child: 
      for (i = nNamedPersons; i<nTotal; i++)
	 if (result[i]==0)
	 {
	    if (father[i]>=0 && result[father[i]]==0) continue; 
	    if (mother[i]>=0 && result[mother[i]]==0) continue; 
	    int ownChildren = 0; 
	    for (j=0; j<nTotal; j++)
	       if ((mother[j]==i || father[j]==i) && result[j]==0)
		  ownChildren++; 
	    if (ownChildren>1) continue; 
	    result[i] = 1;
	    finished = 0; 
	 }
   }
   while (!finished); 
   return result; 
}

// void Pedigree::prune()
// {
//    int* pr = getPruning(); 
//    int i, j; 
//    for (i=0; i<nTotal; i++)
//       for (j=0; j<nTotal; j++)
// 	 if (pr[i] || pr[j]) 
// 	 {
// 	    parent[i][j] = 0; 
// 	    if (male[i])
// 	       father[j] = -1; 
// 	    else
// 	       mother[j] = -1; 
// 	 }
//    delete[] pr; 
// }

void Pedigree::pruneAndRemove()
{
   int* pr = getPruning(); 
   int i = nNamedPersons; 
   while (i<nTotal)
      if (pr[i])
      {
	 int j;  
	 for (j=0; j<nTotal; j++)
	 {
	    if (mother[j]==i) mother[j] = -1; 
	    if (father[j]==i) father[j] = -1; 
	    if (mother[j] >i) mother[j]--; 
	    if (father[j] >i) father[j]--; 
	 }
	 for (j=i+1; j<nTotal; j++)
	 {
	    mother[j-1] = mother[j]; 
	    father[j-1] = father[j]; 
	    male[j-1]   = male[j]; 
		pr[j-1]     = pr[j]; 
	 }
	 nTotal--; 
      }
      else i++; 
   delete[] pr; 
}

void Pedigree::addRelation(int parentindex, 
			   int childindex, 
			   int& error)
{
   error = 1; 
   if (isAncestor(childindex, parentindex)) return; 
   if (male[parentindex])
   {
      if (father[childindex]>=0 && father[childindex]!=parentindex) return;
      father[childindex] = parentindex; 
   }
   else
   {
      if (mother[childindex]>=0 && mother[childindex]!=parentindex) return;
      mother[childindex] = parentindex; 
   }
   error = 0; 
}

void Pedigree::removeRelation(int parentindex, 
			      int childindex)
{
   if (male[parentindex])
      father[childindex] = -1; 
   else
      mother[childindex] = -1; 
}

int* Pedigree::getCutsets() const
{
   int* pruned = getPruning(); 
   int i, j; 
   int* marks = new int[nTotal]; 
   for (i=0; i<nTotal; i++)
      if (pruned[i]!=1)
      {
	 for (j=0; j<nTotal; j++)
	    marks[j] = 0; 
	 marks[i] = 1; 
	 for (j=0; j<nTotal; j++)
	    if ((mother[j]==i || father[j]==i) && pruned[j]!=1)
	       break; 
	 if (j==nTotal)
	 {
	    if (father[i]!=-1 && pruned[father[i]]!=1)
	       j = father[i]; 
	    else if (mother[i]!=-1 && pruned[mother[i]]!=1)
	       j = mother[i]; 
	 }
	 if (j==nTotal) continue; //i is unconnected
	 mark(j, pruned, marks); 
	 for (j=0; j<nTotal; j++)
	    if (marks[j]==0)
	    {
	       pruned[i]=2; 
	       break; 
	    }
      }
   delete[] marks; 
   return pruned; 
}

void Pedigree::mark(int i, int* pruned, int* marks) const
{
   marks[i] = 1; 
   int j; 
   for (j=0; j<nTotal; j++)
      if ((mother[j]==i || father[j]==i) && pruned[j]!=1 && marks[j]==0)
	 mark(j, pruned, marks); 
   if (father[i]!=-1 && pruned[father[i]]!=1 && marks[father[i]]==0)
      mark(father[i], pruned, marks); 
   if (mother[i]!=-1 && pruned[mother[i]]!=1 && marks[mother[i]]==0)
      mark(mother[i], pruned, marks); 
}

int Pedigree::hasCommonAncestor(int person1, int person2) const
{
   if (isAncestor(person1, person2)) return 1; 
   if (father[person1]!=-1 && hasCommonAncestor(father[person1], person2))
      return 1; 
   if (mother[person1]!=-1 && hasCommonAncestor(mother[person1], person2))
      return 1; 
   return 0; 
}

int Pedigree::isAncestor(int person1, int person2) const
{
   if (person1==person2) return 1; 
   if (father[person2]!=-1 && isAncestor(person1, father[person2])) return 1; 
   if (mother[person2]!=-1 && isAncestor(person1, mother[person2])) return 1; 
   return 0; 
}

//Return the maximal number of generations we can go up from 
//this person, reaching a non-extra person in the pedigree: 
int Pedigree::getMaxGenerations(int i) const
{
   int result = 0; 
   int tmp; 
   if (father[i]!=-1)
   {
      tmp = getMaxGenerations(father[i]); 
      if (tmp>0 || father[i]<nNamedPersons) 
	 result = tmp+1; 
   }
   if (mother[i]!=-1)
   {
      tmp = getMaxGenerations(mother[i]);
      if (tmp>0 || mother[i]<nNamedPersons)
	 if (tmp+1>result) result = tmp+1; 
   }
   return result; 
}


