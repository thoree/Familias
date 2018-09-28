/* $Id: help.C,v 1.4 2000/03/03 09:45:02 mostad Exp $ */

#include "help.h"
#include "special.h"

void help(char c) {
// Removed 2012-03-08
/*
    if (c=='i') {
	cout<<"USE OF THE COMMAND information:\n\n";
	cout<<"information  			(to add information)\n";
	cout<<"- information			(to remove information)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"i\n";
    }
    else if (c=='m') {
	cout<<"USE OF THE COMMAND males:\n\n";
	cout<<"males NAME1, NAME2, NAME3 ...    (to add new males to the data)\n";
	cout<<"- males NAME1, NAME2, NAME3 ...  (to remove males from the data)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"m John Smith\n";
    }
    else if (c=='f') {
	cout<<"USE OF THE COMMAND females:\n\n";
	cout<<"females NAME1, NAME2, NAME3 ...  (to add new females to the data)\n";
	cout<<"- females NAME1, NAME2, NAME3 ...(to remove females from the data)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"f Elisabeth II, Elisabeth III\n";
    }
    else if (c=='p') {
	cout<<"USE OF THE COMMAND parents:\n\n";
	cout<<"parents PARENT; CHILD1, CHILD2, CHILD3 ...\n";
	cout<<"                                 (add PARENT as a parent of the children)\n";
	cout<<"parents PARENT1, PARENT2; CHILD1, CHILD2, CHILD3 ...\n";
	cout<<"                                 (add PARENT1 and PARENT2 as parents)\n";
	cout<<"- parents PARENT; CHILD1, CHILD2, CHILD3 ...\n";
	cout<<"                                 (remove PARENT as parent of the children)\n";
	cout<<"- parents PARENT1, PARENT2; CHILD1, CHILD2, CHILD3 ...\n";
	cout<<"                                 (remove PARENT1 and PARENT2 as parents)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"p Mother, Father; Son, Daughter\n";
    }
    else if (c=='c') {
	cout<<"USE OF THE COMMAND children:\n\n";
	cout<<"children CHILD1, CHILD2, CHILD3 ...; PARENT\n";
	cout<<"                                 (add PARENT as a parent of the children)\n";
	cout<<"children CHILD1, CHILD2, CHILD3 ...; PARENT1, PARENT2\n";
	cout<<"                                 (add PARENT1 and PARENT2 as parents)\n";
	cout<<"- children CHILD1, CHILD2, CHILD3 ...; PARENT\n";
	cout<<"                                 (remove PARENT as parent of the children)\n";
	cout<<"- children CHILD1, CHILD2, CHILD3 ...; PARENT1, PARENT2\n";
	cout<<"                                 (remove PARENT1 and PARENT2 as parents)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"c Son, Daughter; Mother, Father\n";
    }
    else if (c=='C') {
	cout<<"USE OF THE COMMAND clean:\n\n";
	cout<<"clean                            (removes all data from the  program)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"clean\n";
    }
    else if (c=='o') {
	cout<<"USE OF THE COMMAND odds:\n\n";
	cout<<"odds NAME1 = NAME2               (add the odds question NAME1 = NAME2)\n";
	cout<<"- odds                           (remove the current odds question)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"o PutativeFather = FatherOfChild\n";
    }
    else if (c=='s') {
	cout<<"USE OF THE COMMAND system:\n\n";
	cout<<"system SYSTEMNAME; MUTATIONRATE  (add an allele system; 100 poss. alleles)\n";
	cout<<"system SYSTEMNAME; MUTATIONRATE TOTALALLELENUMBER\n";
	cout<<"                                 (add system; TOTALALLELENUMBER poss. all.)\n";
	cout<<"- system SYSTEMNAME              (remove the allele system)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"sys HLA DQ A1; 0.01\n";
    }
    else if (c=='a') {
	cout<<"USE OF THE COMMAND alleles:\n\n";
	cout<<"alleles SYSTEMNAME; ALLNAME, FREQ; ALLNAME, FREQ ...\n";
	cout<<"                                 (add alleles with frequencies to system)\n";
	cout<<"- alleles SYSTEMNAME; ALLELENAME; ALLELENAME ...\n";
	cout<<"                                 (remove alleles from system)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"a HLA DQ A1; 1.1, 0.1; 4, 0.2\n";
    }
    else if (c=='d') {
	cout<<"USE OF THE COMMAND data:\n\n";
	cout<<"data SYSTEMNAME; PERS, ALL1, ALL2; PERS, ALL1, ALL2 ...\n";
	cout<<"                                 (add DNA-data for persons in this system)\n";
	cout<<"- data SYSTEMNAME; PERSON; PERSON; PERSON ...\n";
	cout<<"                                 (remove DNA-data for persons in system)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"d HLA DQ A1; Elisabeth II, 1.1, 1.1; Elisabeth III, 1.1, 4\n";
    }
    else if (c=='e') {
	cout<<"USE OF THE COMMAND execute:\n\n";
	cout<<"execute                          (compute probability or odds)\n";
	cout<<"execute PERSON1, PERSON2 ...; PERSON3, PERSON4 ...; ....\n";
	cout<<"                                 (compute probability or odds, using\n";
	cout<<"                                  cutset PERSON1, PERSON2,..., and\n";
	cout<<"                                  cutset PERSON3, PERSON4,..., and so on)\n"; 
	cout<<"EXAMPLES:\n";
//	cout<<"EXAMPLE:\n";
	cout<<"e\n";
	cout<<"e Bart; Homer, Marge\n";
    }
    else if (c=='w') {
	cout<<"USE OF THE COMMAND write:\n\n";
	cout<<"write FILENAME                   (write all data to file)\n";
	cout<<"write FILENAME information       (write information to file)\n";
	cout<<"write FILENAME family            (write family data to file)\n";
	cout<<"write FILENAME males             (write list of males to file)\n";
	cout<<"write FILENAME females           (write list of females to file)\n";
	cout<<"write FILENAME systems           (write allele-systems and data to file)\n";
	cout<<"write FILENAME system SYSTEMNAME (write SYSTEMNAME system to file)\n";
	cout<<"write FILENAME frequencies       (write allele-systems frequencies to file)\n";
	cout<<"write FILENAME frequencies SYSTEMNAME\n";
	cout<<"                                 (write SYSTEMNAME frequencies to file)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"w savefreq.data fr\n";
    }
    else if (c=='r') {
	cout<<"USE OF THE COMMAND read:\n\n";
	cout<<"read FILENAME                    (read FILENAME and apply to data)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"r savefreq.data\n";
    }
    else if (c=='q') {
	cout<<"USE OF THE COMMAND quit:\n\n";
	cout<<"quit                             (end the program)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"q\n";
    }
    else if (c=='l') {
	cout<<"USE OF THE COMMAND list:\n\n";
	cout<<"list                             (list all data)\n";
	cout<<"list information                 (list information)\n";
	cout<<"list family                      (list family data)\n";
	cout<<"list males                       (list all males)\n";
	cout<<"list females                     (list all females)\n";
	cout<<"list systems                     (list all allele system data)\n";
	cout<<"list system SYSTEMNAME           (list allele system SYSTEMNAME\n";
	cout<<"list frequencies                 (list allele systems frequencies)\n";
	cout<<"list frequencies SYSTEMNAME      (list SYSTEMNAME frequencies)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"l fr\n";
    }
    else if (c=='h') {
	cout<<"USE OF THE COMMAND help:\n\n";
	cout<<"help                             (list all commands)\n";
	cout<<"?                                (list all commands)\n";
	cout<<"help COMMANDNAME                 (list help on COMMANDNAME)\n";
	cout<<"COMMANDNAME?                     (list help on COMMANDNAME)\n\n";
	cout<<"EXAMPLE:\n";
	cout<<"write?\n";
    }
    else if (c=='\0') {
	cout<<"The following commands are available:\n";
	cout<<"information  Specify general information\n";
	cout<<"females      Specify names of females\n";
	cout<<"males        Specify names of males\n";
	cout<<"parents      Specify family relations\n";
	cout<<"children     Specify family relations\n";
	cout<<"odds         Specify the odds question\n";
	cout<<"system       Add an allele system\n";
	cout<<"alleles      Add alleles to an allele system\n";
	cout<<"data         Add DNA measurements\n";
	cout<<"execute      Compute the odds or probabilities\n";
	cout<<"write        Write data to file\n";
	cout<<"read         Read data from file\n";
	cout<<"quit         End the interactive program\n";
	cout<<"clean        Remove all data\n";
	cout<<"list         List contents of data structure\n";
	cout<<"help         Use \"help COMMANDNAME\" for info on each command\n";
    }
    else 
	cout<<"Internal error: help code not understood.\n";
*/
}

