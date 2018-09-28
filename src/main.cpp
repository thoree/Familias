#include <iostream>
#include "FamInterface.h"


static FamInterface* fint = new FamInterface; 

extern "C" {

  void NewFamilias() {
    delete fint; 
    fint = new FamInterface; 
  }

  /*
  void GetNumberOfSystems(int* result) 
  {
    fint->GetNumberOfSystems(result); 
  } 
  */

  void GetProbabilities(/*[in]*/ double* generationsParameter, 
			/*[in]*/ int* maxGenerations, 
			/*[in]*/ double* inbreedingParameter, 
			/*[in]*/ double* promiscuityParameter, 
			/*[in]*/ int* usingDNAObservations, 
			/*[in]*/ double* kinship, 
			/*[out]*/ int* redundant, 
			/*[out]*/ double* probabilities, 
			/*[out]*/ double* likelihoods, 
			/*[out]*/ int* error) {
    fint->GetProbabilities(/*[in]*/ *generationsParameter, 
			  /*[in]*/ *maxGenerations, 
			  /*[in]*/ *inbreedingParameter, 
			  /*[in]*/ *promiscuityParameter, 
			  /*[in]*/ *usingDNAObservations, 
			  /*[in]*/ *kinship, 
			  /*[out]*/ redundant, 
			  /*[out]*/ probabilities, 
			  /*[out]*/ likelihoods, 
			  /*[out]*/ error); 
  }
 

  //  void RemoveDNAObservation(/*[in]*/ int* indexperson, 
  //			    /*[in]*/ int* indexAlleleSystem, 
  //			    /*[out]*/ int* error) {
  //    fint->RemoveDNAObservation(/*[in]*/ *indexperson, 
  //			      /*[in]*/ *indexAlleleSystem, 
  //			      /*[out]*/ error); 
  //  } 


  void AddDNAObservation(/*[in]*/ int* indexperson, 
			 /*[in]*/ int* indexAlleleSystem, 
			 /*[in]*/ int* indexAllele1, 
			 /*[in]*/ int* indexAllele2, 
			 /*[out]*/ int* error) {
    fint->AddDNAObservation(/*[in]*/ *indexperson, 
			   /*[in]*/ *indexAlleleSystem, 
			   /*[in]*/ *indexAllele1, 
			   /*[in]*/ *indexAllele2, 
			   /*[out]*/ error); 
  } 
      
  //  void EditAlleleSystem(/*[in]*/ int* indexSystem, 
  //			/*[in]*/ int* nAlleles, 
  //			/*[in]*/ double* mutationRateFemale,
  //			/*[in]*/ double* mutationRateMale,
  //			/*[in]*/ int* mutationModelFemale, 
  //			/*[in]*/ int* mutationModelMale, 
  //			/*[in]*/ int* nPossibilities, 
  //			/*[in]*/ double* mutationRangeFemale, 
  //			/*[in]*/ double* mutationRangeMale, 
  //			/*[in]*/ double* frequencies, /*array*/
  //			/*[in]*/ int* correspondence, /*array*/
  //			/*[in]*/ int* hasSilentAllele, 
  //			/*[out]*/ int* error) {
  //    fint->EditAlleleSystem(/*[in]*/ *indexSystem, 
  //			  /*[in]*/ *nAlleles, 
  //			  /*[in]*/ *mutationRateFemale,
  //			  /*[in]*/ *mutationRateMale,
  //			  /*[in]*/ *mutationModelFemale, 
  //			  /*[in]*/ *mutationModelMale, 
  //			  /*[in]*/ *nPossibilities, 
  //			  /*[in]*/ *mutationRangeFemale, 
  //			  /*[in]*/ *mutationRangeMale, 
  //			  /*[in]*/ frequencies, 
  //			  /*[in]*/ correspondence, 
  //			  /*[in]*/ *hasSilentAllele, 
  //			  /*[out]*/ error); 
  //  }
  //
  //  void RemoveAlleleSystem(/*[in]*/ int* index, 
  //			  /*[out ]*/ int* error) {
  //    fint->RemoveAlleleSystem(/*[in]*/ *index, 
  //			    /*[out ]*/ error); 
  //  }

  void AddAlleleSystem(/*[in]*/ int* nAlleles, 
		       /*[in]*/ int* lOfVector, 
		       /*[in]*/ double* mMatrixFemale, /*array*/
		       /*[in]*/ double* mMatrixMale, /*array*/
		       /*[in]*/ int* sMutationMatrix, 
		       /*[in]*/ int* nPossibilities, 
		       /*[in]*/ double* frequencies, /*array*/ 
		       /*[in]*/ int* hasSilentAllele,
		       /*[out]*/ int* index, 
		       /*[out]*/ int* error) {
    fint->AddAlleleSystem(/*[in]*/ *nAlleles, 
			  /*[in]*/ *lOfVector, 
			  /*[in]*/ mMatrixFemale, 
			  /*[in]*/ mMatrixMale, 
			  /*[in]*/ *sMutationMatrix, 
			  /*[in]*/ *nPossibilities, 
			  /*[in]*/ frequencies, 
			  /*[in]*/ *hasSilentAllele,
			  /*[out]*/ index, 
			  /*[out]*/ error); 
  }

  //  void RemoveRelation(/*[in]*/ int* parentindex, 
  //		      /*[in]*/ int* childindex, 
  //		      /*[in]*/ int* pedigree, 
  //		      /*[out]*/ int* error) {
  //    fint->RemoveRelation(/*[in]*/ *parentindex, 
  //			/*[in]*/ *childindex, 
  //			/*[in]*/ *pedigree, 
  //			/*[out]*/ error); 
  //  }

  void AddRelation(/*[in]*/ int* parentindex, 
		   /*[in]*/ int* childindex, 
		   /*[in]*/ int* pedigree, 
		   /*[out]*/ int* error) {
    fint->AddRelation(/*[in]*/ *parentindex, 
		     /*[in]*/ *childindex, 
		     /*[in]*/ *pedigree, 
		     /*[out]*/ error); 
  }
  
  //  void AddExtraPerson(/*[in]*/ int* male, 
  //		      /*[in]*/ int* pedigree, 
  //		      /*[out]*/ int* error) {
  //fint->AddExtraPerson(/*[in]*/ *male, 
  //			/*[in]*/ *pedigree, 
  //			/*[out]*/ error); 
  //  }
  
  //  void RemoveExtraPerson(/*[in]*/ int* person, 
  //			 /*[in]*/ int* pedigree, 
  //			 /*[out]*/ int* error) {
  //    fint->RemoveExtraPerson(/*[in]*/ *person, 
  //			   /*[in]*/ *pedigree, 
  //			   /*[out]*/ error); 
  //  } 

  //  void GetPedigree(/*[in]*/ int* index, 
  //		   /*[out]*/ int* matrix, 
  //		   /*[out]*/ int* error) {
  //    fint->GetPedigree(/*[in]*/ *index, 
  //		     /*[out]*/ matrix, 
  //		     /*[out]*/ error); 
  //  }

  //  void GetParents(/*[in]*/ int* index, 
  //		  /*[out]*/ int* mother, 
  //		  /*[out]*/ int* father, 
  //		  /*[out]*/ int* error) {
  //    fint->GetParents(/*[in]*/ *index, 
  //		    /*[out]*/ mother, 
  //		    /*[out]*/ father, 
  //		    /*[out]*/ error); 
  //  }

  //  void GetSizeOfPedigree(/*[in]*/ int* index, 
  //			 /*[out]*/ int* size, 
  //			 /*[out]*/ int* error) {
  //    fint->GetSizeOfPedigree(/*[in]*/ *index, 
  //			   /*[out]*/ size, 
  //			   /*[out]*/ error);  
  //  }

  //  void GetNumberOfExtraFemales(/*[in]*/ int* pedigree, 
  //			       /*[out]*/ int* number, 
  //			       /*[out]*/ int* error) {
  //    fint->GetNumberOfExtraFemales(/*[in]*/ *pedigree, 
  //				 /*[out]*/ number, 
  //				 /*[out]*/ error); 
  //  }

  //  void GetNumberOfExtraMales(/*[in]*/ int* pedigree, 
  //			     /*[out]*/ int* number, 
  //			     /*[out]*/ int* error) {
  //    fint->GetNumberOfExtraMales(/*[in]*/ *pedigree, 
  //			       /*[out]*/ number, 
  //			       /*[out]*/ error); 
  //  }
  
  //  void RemovePedigree(/*[in]*/ int* index, 
  //		      /*[out]*/ int* error) {
  //    fint->RemovePedigree(/*[in]*/ *index, 
  //			/*[out]*/ error); 
  //  }

  void AddPedigree(/*[in]*/ int* nExtraFemales, 
		   /*[in]*/ int* nExtraMales, 
		   /*[out]*/ int* index, 
		   /*[out]*/ int* error) {
    fint->AddPedigree(/*[in]*/ *nExtraFemales, 
		     /*[in]*/ *nExtraMales, 
		     /*[out]*/ index, 
		     /*[out]*/ error); 
  }

  //  void GetNumberOfPedigrees(/*[out]*/ int* number) {
  //    fint->GetNumberOfPedigrees(/*[out]*/ number); 
  //  }

  //  void GeneratePedigrees(/*[in]*/ int* nExtraFemales, 
  //			 /*[in]*/ int* nExtraMales, 
  //			 /*[out]*/ int* removed, 
  //			 /*[out]*/ int* error) {
  //    fint->GeneratePedigrees(/*[in]*/ *nExtraFemales, 
  //			   /*[in]*/ *nExtraMales, 
  //			   /*[out]*/ removed, 
  //			   /*[out]*/ error);
  //  }

  //  void RemoveFixedRelation(/*[in]*/ int* parentindex, 
  //			   /*[in]*/ int* childindex, 
  //			   /*[out]*/ int* error) {
  //    fint->RemoveFixedRelation(/*[in]*/ *parentindex, 
  //			     /*[in]*/ *childindex, 
  //			     /*[out]*/ error); 
  //  }

  //  void AddFixedRelation(/*[in]*/ int* parentindex, 
  //			/*[in]*/ int* childindex, 
  //			/*[out]*/ int* removed, 
  //			/*[out]*/ int* error) {
  //    fint->AddFixedRelation(/*[in]*/ *parentindex, 
  //			  /*[in]*/ *childindex, 
  //			  /*[out]*/ removed, 
  //			  /*[out]*/ error); 
  //  }

  //  void RemovePerson(/*[in]*/ int* index, 
  //		    /*[out]*/ int* error) {
  //    fint->RemovePerson(/*[in]*/ *index, 
  //		      /*[out]*/ error); 
  //  }

  void AddPerson(/*[in]*/ int* male, 
		 /*[in]*/ int* YOB, 
		 /*[in]*/ int* isChild, 
		 /*[out]*/ int* index, 
		 /*[out]*/ int* error) {
    fint->AddPerson(/*[in]*/ *male, 
		   /*[in]*/ *YOB,
		   /*[in]*/ *isChild, 
		   /*[out]*/ index, 
		   /*[out]*/ error); 
  }

}

