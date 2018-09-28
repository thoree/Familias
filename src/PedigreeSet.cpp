/* $Id: PedigreeSet.C,v 1.13 2001/04/08 14:04:57 mostad Exp $ */

#include <fstream>
#include <cstdlib>

#include "PedigreeSet.h"
#include "Pedigree.h"
#include "GroupOfPersons.h"
#include "read.h"
#include "pater.h"
#include "special.h"

PedigreeSet::PedigreeSet(const char* filename, int& OK) : list(1)
{
  //#ifdef UNIXVERSION
   ifstream is(filename);
   //#else
   //ifstream is(filename, ios::nocreate);
   //#endif
   if (!is)
   {
// Removed 2012-03-08
//      cout<<"ERROR: Cannot open file \""<<filename<<"\".\n";
      OK = 0; 
      gp = 0; 
      weight = 0;
      return; 
   }
// Removed 2012-03-08
//   cout<<"Reading pedigrees from file \""<<filename<<"\".\n";
   gp = new GroupOfPersons(is); 
   int nPedigrees = 0; 
   is>>nPedigrees; 
   if (!is || nPedigrees<1)
   {
// Removed 2012-03-08
//      cout<<"ERROR: No pedigrees in file \""<<filename<<"\".\n";
      OK = 0; 
      gp = 0; 
      weight = 0;
      return; 
   }
   weight = new double[nPedigrees];

   int nNamedPersons = gp->getNamedPersons(); 
   int* male = new int[nNamedPersons]; 
   int* isChild = new int[nNamedPersons]; 
   int i; 
   for (i=0; i<nNamedPersons; i++)
   {
      male[i] = gp->isMale(i); 
      list.addPerson(male[i]); 
      isChild[i] = ! gp->isAdult(i); 
   }

   for (i=0; i<nPedigrees; i++)
   {
      Pedigree* p = new Pedigree(nNamedPersons, male, isChild, is, OK); 
      if (!OK)
      {
// Removed 2012-03-08
//	 cout<<"ERROR: Problems reading pedigree number "<<i+1<<".\n";
	 delete p; 
	 delete gp; 
	 delete[] weight; 
	 delete[] male;  
	 gp = 0; 
	 weight = 0; 
	 return; 
      }
      list.addPedigree(p); 
      is>>weight[i]; 
      if (!is || weight[i]<=0)
      {
// Removed 2012-03-08
//	 cout<<"ERROR: Problems reading weight of pedigre number "<<i+1<<"\".\n";
	 delete gp; 
	 delete[] weight; 
	 delete[] male; 
	 OK = 0; 
	 gp = 0; 
	 weight = 0; 
	 return; 
      }
   }
   delete[] male; 
// Removed 2012-03-08
//   cout<<nPedigrees<<" pedigrees read.\n";
}

PedigreeSet::PedigreeSet(GroupOfPersons* GP) : list(1)
{
   gp = GP;
   int i, j; 
   int nTotal = gp->getTotalSize(); 
   int* parent = new int[nTotal*nTotal]; 
   for (i=0; i<nTotal; i++)
      if (gp->isAdult(i))
	 for (j=0; j<nTotal; j++)
	    parent[i+j*nTotal] = nTotal; 
      else
	 for (j=0; j<nTotal; j++)
	    parent[i+j*nTotal] = 0; 

   int nNamedPersons = gp->getNamedPersons(); 
   for (i=0; i<nNamedPersons; i++)
      list.addPerson(gp->isMale(i)); 
   list.generatePedigrees(gp->getExtraWomen(), 
			  gp->getExtraMen(), 
			  parent); 
   int nPedigrees = list.getNumberOfPedigrees(); 
   if (nPedigrees==0)
   {
// Removed 2012-03-08
//      cout<<"No pedigrees generated!\n";
      weight = 0; 
   }
   else
   {
// Removed 2012-03-08
//      cout<<"Number of pedigrees generated: "<<nPedigrees<<".\n";
      weight = new double[nPedigrees]; 
      for (i=0; i<nPedigrees; i++)
	 weight[i] = 1.0/nPedigrees; 
   }
   delete[] parent; 
}

PedigreeSet::~PedigreeSet()
{
   delete gp; 
   delete[] weight; 
}

void PedigreeSet::addCutset(const char* cutsetfile)
{
  //#ifdef UNIXVERSION
   ifstream is(cutsetfile);
   //#else
   //ifstream is(cutsetfile, ios::nocreate);
   //#endif
   if (!is)
   {
// Removed 2012-03-08
//      cout<<"ERROR: Cannot open file \""<<cutsetfile<<"\".\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return; 
    }
// Removed 2012-03-08
//   cout<<"Reading cutsets from file \""<<cutsetfile
//      <<"\" not implemented yet.\n";
   //for (int i=0; i<nPedigrees; i++)
   //   ped[i]->addCutset(gp, is); 
   if (!is)
   {
// Removed 2012-03-08
//      cout<<"ERROR: Problems reading file \""<<cutsetfile<<"\".\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return; 
   }
}

/*
int PedigreeSet::updateDNA(const char* datafile, 
			   int mutationModel, 
			   double mutationRange, 
			   double kinship)
{
  //#ifdef UNIXVERSION
   ifstream is(datafile);
   //#else
   //ifstream is(datafile, ios::nocreate);
   //#endif
   if (!is)
   {
// Removed 2012-03-08
//      cout<<"ERROR: Problems opening file \""<<datafile<<"\".\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return 0; 
   }
// Removed 2012-03-08
//   cout<<"Reading file \""<<datafile<<"\".\n";
   int error = 0; 
   pater* pat = new pater; 
   do_commands(is, 0, pat, error); 
   if (error) 
   {
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return 0; 
   }
   int i,j;
   int nNamedPersons = gp->getNamedPersons();


   if (mutationModel!=0 || kinship > 0)
      pat->setNewParameters(mutationModel, mutationRange, kinship); 


   //Test that the same people appear in the pater data and 
   //in the pedigree file: 
   for (i=0; i<nNamedPersons; i++)
   {
      if (!pat->inFamily(newcopy(gp->getName(i))))
      {
// Removed 2012-03-08
//	 cout<<"ERROR: The person \""<<gp->getName(i)
//	     <<"\" appears in the pedigree file,\n"
//	     <<"but not in the DNA data file.\n"; 
	 delete gp; 
	 delete[] weight; 
	 gp = 0; 
	 weight = 0; 
	 return 0; 
      }
      if (gp->isAdult(i))
      {
	 if (pat->isMale(newcopy(gp->getName(i))) != gp->isMale(i))
	 {
// Removed 2012-03-08
//	    cout<<"ERROR: the person \""<<gp->getName(i)
//		<<"\" has different sex\n"
//		<<"in the pedigree file and in the DNA data file.\n";
	    
	    delete gp; 
	    delete[] weight; 
	    gp = 0; 
	    weight = 0; 
	    return 0; 
	 }
      }
   }
   
   if (pat->totalFamilySize() != nNamedPersons)
   {
//      cout<<"ERROR: There are persons listed in the DNA data files\n"
//	  <<"that do not appear in the pedigree file.\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return 0; 
   }

   //Removing old family relations:
   for (i=0; i< nNamedPersons; i++)
      for (j=0; j < nNamedPersons; j++)
	 pat->remove_possible_parent(newcopy(gp->getName(i)), 
				     newcopy(gp->getName(j))); 


   int* parnt   = new int[nNamedPersons*nNamedPersons]; 
   for (i=0; i<nNamedPersons*nNamedPersons; i++)
      parnt[i] = 0; 

   char** names = new char*[nNamedPersons]; 
   for (i=0; i<nNamedPersons; i++)
      names[i] = newcopy(gp->getName(i)); 

   char* namePrefix = getNamePrefix(nNamedPersons, names);
   int counter = 0; 
   Pedigree* p = list.getPedigree(0); 
   double sum = 0; 
   while (p)
   { 
//      cout<<"=======================================================================\n\n"; 
// Removed 2012-03-08
//      cout<<"Computing probability for pedigree "<<counter+1<<"\n";
//      cout<<"=======================================================================\n"; 
      p->pruneAndRemove(); 
		
	  double* result = p->computeProbability(*pat, parnt, names, namePrefix, 
					       1, counter%100==0); 
	  int nSystems = pat->get_number_of_systems();
	  int j; 
	  for (j=0; j<nSystems; j++)
		  weight[counter] *= result[j];  
	  delete[] result; 
//      weight[counter] *= p->computeProbability(*pat, parnt, names, namePrefix, 
//					       1, counter%100==0); 
      sum += weight[counter]; 
      counter++; 
      p = (Pedigree*) p->getNext(); 
   }


   for (i=0; i<nNamedPersons; i++)
      delete[] names[i]; 
   delete[] names; 
   delete[] parnt; 

   if (sum==0)
   {
// Removed 2012-03-08
//      cout<<"ERROR: The updating gives all pedigrees probability zero.\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return 0; 
   }
   for (i=0; i<counter; i++)
      weight[i] = weight[i]/sum; 
   delete pat;
   return 1; 
}
*/


void PedigreeSet::write(const char* outputfile)
{
   int nPedigrees = list.getNumberOfPedigrees(); 
   if (nPedigrees==0) return; 
   //Count number of pedigrees with nonzero weight: 
   int i; 
   int number = 0; 
   for (i=0; i<nPedigrees; i++)
      if (weight[i]>0) number++; 

   ofstream os(outputfile);
   if (!os.good())
   {
// Removed 2012-03-08
//      cout<<"ERROR: Cannot open file \""<<outputfile<<"\".\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return; 
   }
// Removed 2012-03-08
//   cout<<"Writing "<<number<<" pedigrees to file \""<<outputfile<<"\".\n";
   gp->write(os); 
   os<<number<<"\n\n"; 
   if (!os.good())
   {
// Removed 2012-03-08
//      cout<<"ERROR: Problems writing file \""<<outputfile<<"\".\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return; 
   }
   Pedigree* p = list.getPedigree(0); 
   for (i=0; i<nPedigrees; i++)
   {
      if (weight[i]>0)
      {
	 p->write(os); 
	 os<<weight[i]<<"\n\n"; 
      }
      p = (Pedigree*) p->getNext(); 
   }
   if (!os.good())
   {
// Removed 2012-03-08
//      cout<<"ERROR: Problems writing file \""<<outputfile<<"\".\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return; 
   }
}

//Note: Any legal (in terms of cycles AND pruning) pedigree may be transformed
//by permutation of extra women to one where, when going down
//the ordered list of persons, the extra women appear as MOTHERS in the
//same order as they appear on the list. (Proof: Starting at the top 
//of the ordered list, number the extra women as they appear as mothers. 
//Continue with this also when getting to the part of the list
//with extra women who have now been numbered. This works fine until 
//we may get to an extra women who has not yet been ordering. If so, 
//do the same process, but starting at the bottom of the ordered list, 
//and find the reverse ordering. This procedure will now order all 
//the extra women, as because of pruning, all extra women are mothers, 
//and because of acyclicity, they all have ancestors who are not extra women.)
//As we are in the end only
//interested in equivalence classes of pedigrees under permutations
//of the above type, we generate all such classes by using the
//algorithm below. The same goes for extra men. 
// void PedigreeSet::generateParentsForPerson(int i, 
// 					   int* parent, 
// 					   int nTotal, 
// 					   int nExtraWomenUsed, 
// 					   int nExtraMenUsed, 
// 					   Pedigree*& list)
// {
//    if (i==nTotal)
//    {
//       int* male = new int[nTotal]; 
//       for (int j=0; j<nTotal; j++)
// 	 male[j] = gp->isMale(j); 
//       Pedigree* p = new Pedigree(gp->getNamedPersons(), 
// 				 gp->getExtraWomen(), 
// 				 gp->getExtraMen(), 
// 				 male, 
// 				 parent); 
//       delete[] male; 
//       p->append(list); 
//       list = p; 
//       removeEquivalences(list); 
//       return; 
//    }
//    int j,k; 
//    int usingExtraWoman, usingExtraMan; 
//    for (j=-1; j<=gp->getNamedWomen(); j++)
//    {
//       usingExtraWoman = 0; 
//       if (j==gp->getNamedWomen())
//       {
// 	 usingExtraWoman = 1; 
// 	 j = gp->getNamedPersons() + nExtraWomenUsed; 
// 	 if (nExtraWomenUsed+1 > gp->getExtraWomen()) break; 
//       }
//       if (j>=0)
// 	 parent[j+i*nTotal] = 1; 
//       if (j==-1 || !isAncestor(i, j, parent, nTotal))
//       {
// 	 for (k=-1; k<=gp->getNamedMen(); k++)
// 	 {
// 	    usingExtraMan = 0; 
// 	    if (k==gp->getNamedMen())
// 	    {
// 	       usingExtraMan = 1; 
// 	       k = gp->getNamedMen() + 
// 		  gp->getNamedChildren() + 
// 		  gp->getExtraWomen() + nExtraMenUsed; 
// 	       if (nExtraMenUsed+1 > gp->getExtraMen()) break; 
// 	    }
// 	    if (k>=0)
// 	       parent[k+gp->getNamedWomen()+i*nTotal] = 1; 
// 	    if (k==-1 || !isAncestor(i, k+gp->getNamedWomen(), parent, nTotal))
// 	       generateParentsForPerson(i+1, 
// 					parent, 
// 					nTotal, 
// 					nExtraWomenUsed+usingExtraWoman, 
// 					nExtraMenUsed+usingExtraMan, 
// 					list); 
// 	    if (k>=0)
// 	       parent[k+gp->getNamedWomen()+i*nTotal] = 0; 
// 	 }
//       }
//       if (j>=0)
// 	 parent[j+i*nTotal] = 0; 
//    }
// }

// int PedigreeSet::isAncestor(int i, int j, int* parent, int nTotal)
// {
//    if (i==j) return 1; 
//    for (int k=0; k<nTotal; k++)
//       if (parent[k+j*nTotal] && isAncestor(i, k, parent, nTotal))
// 	 return 1; 
//    return 0; 
// }

// int nPeds = 0; 

// void PedigreeSet::removeEquivalences(Pedigree*& list)
// {
//    list->prune(); 
//    Pedigree* p; 
//    if (!list->onStandardForm())
//    {
//       p = list; 
//       list = (Pedigree*) p->getNext(); 
//       delete p; 
//       return ;
//    }      
//    p = (Pedigree*) list->getNext(); 
//    while (p)
//    {
//       if (list->unsafeEquals(p))
//       {
// 	 p = list; 
// 	 list = (Pedigree*) p->getNext(); 
// 	 delete p; 
// 	 return; 
//       }
//       p = (Pedigree*) p->getNext(); 
//    }
//    if ((++nPeds)%500==0)
//    {
//       cout<<"Adding pedigree nr. "<<nPeds<<'\n';
//       list->print(); 
//    }
// }

int PedigreeSet::updateInbreeding(char* multiplier)
{
   int OK; 
   double mult = getDouble(multiplier, OK); 
   if (!OK)
   {
// Removed 2012-03-08
//      cout<<"ERROR: Problems reading a double from the string \""
//	  <<multiplier<<"\".\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return 0; 
   }
   if (mult==1) return 1; 
   double sum = 0; 
   int i = 0; 
   Pedigree* p = list.getPedigree(0); 
   while (p)
   {
      weight[i] *= mypow(mult, p->computeInbreeding()); 
      sum += weight[i]; 
      i++; 
      p = (Pedigree*) p->getNext(); 
   }
   if (sum==0)
   {
// Removed 2012-03-08
//      cout<<"ERROR: The updating gives all pedigrees probability zero.\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return 0; 
   }
   p = list.getPedigree(0); 
   i = 0; 
   while (p)
   {
      weight[i] = weight[i]/sum; 
      i++; 
      p = (Pedigree*) p->getNext(); 
   }
   return 1; 
}

int PedigreeSet::updatePromiscuity(char* multiplier)
{
   int OK; 
   double mult = getDouble(multiplier, OK); 
   if (!OK)
   {
// Removed 2012-03-08
//      cout<<"ERROR: Problems reading a double from the string \""
//	  <<multiplier<<"\".\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return 0; 
   }
   double sum = 0; 
   int i = 0; 
   Pedigree* p = list.getPedigree(0); 
   while (p)
   {
      weight[i] *= mypow(mult, p->computePromiscuity()); 
      sum += weight[i]; 
      i++; 
      p = (Pedigree*) p->getNext(); 
   }
   if (sum==0)
   {
// Removed 2012-03-08
//      cout<<"ERROR: The updating gives all pedigrees probability zero.\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return 0; 
   }
   p = list.getPedigree(0);
   i = 0; 
   while (p)
   {
      weight[i] = weight[i]/sum; 
      i++; 
      p = (Pedigree*) p->getNext(); 
   }
   return 1; 
}

int PedigreeSet::updateGenerations(char* multiplier, 
				   char* maxGenerations)
{
   int OK; 
   double mult = getDouble(multiplier, OK); 
   if (!OK)
   {
// Removed 2012-03-08
//      cout<<"ERROR: Problems reading a double from the string \""
//	  <<multiplier<<"\".\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return 0; 
   }
   int maxGen = getInteger(maxGenerations, OK); 
   if (!OK || maxGen<1) 
   {
// Removed 2012-03-08
//      cout<<"ERROR: Problems reading a positive integer from the string \""
//	  <<maxGenerations<<"\".\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return 0; 
   }

   int* isChild = new int[gp->getNamedPersons()]; 
   int i; 
   for (i=0; i<gp->getNamedPersons(); i++)
      isChild[i] = !gp->isAdult(i); 
   double sum = 0; 
   i = 0; 
   Pedigree* p = list.getPedigree(0); 
   while (p)
   {
      int nGen =  p->computeGenerations(isChild); 
      if (nGen > maxGen)
	 weight[i] = 0; 
      else
	 weight[i] *= mypow(mult, nGen); 
      sum += weight[i]; 
      i++; 
      p = (Pedigree*) p->getNext(); 
   }
   delete[] isChild; 
   if (sum==0)
   {
// Removed 2012-03-08
//      cout<<"ERROR: The updating gives all pedigrees probability zero.\n";
      delete gp; 
      delete[] weight; 
      gp = 0; 
      weight = 0; 
      return 0; 
   }
   p = list.getPedigree(0); 
   i = 0; 
   while (p)
   {
      weight[i] = weight[i]/sum; 
      i++; 
      p = (Pedigree*) p->getNext(); 
   }
   return 1; 
}

