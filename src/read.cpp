/* $Id: read.C,v 1.12 2001/04/12 16:00:37 mostad Exp $ */

#include <cstring>
#include <cctype>
#include <cstdlib>

#include "read.h"
#include "help.h"
#include "pater.h"
#include "special.h"


/*
//Read in new lines, discarding too long lines, with an error message. 
//Return 0 if we reach end of file, 1 otherwise.
int get_line(istream& in, char* buf, int buflength, 
	     int info, int& error) 
{
   char c = ' ';
   while (in.get(buf, buflength,'\n')) 
   {
      if (in.get(c) && c != '\n') 
      {
// Removed 2012-03-08
//	 if (info>0)
//	    cout<<"ERROR: The following line was too long, "
//	       "and was discarded:\n"
//		<<buf<<'\n';
	 error = 1; 
	 //Throw away rest of line:
	 while (in.get(c) && c != '\n');
      } 
      else 
      {
	 char* bp = buf;
	 while (*bp!='\0')
	    if (*bp=='\15') 
	    {
	       *bp = '\0';
	       return 1;
	    } 
	    else 
	       bp++;
	 return 1;
      }
   }
   return 0;
}

//A "name" is terminated by one of the characters , | = ? ; \0
//Trailing blanks are removed from the name. 
//A NEW string is returned with the contents;
//0 is returned if all is blank. 
//The bp at return points to the first character after the name 
//which is not ',' or space. (May be '\0').
char* get_name(char*& bp) {
    char* start = bp;
    int length = 0;
    while (*bp != '\0' && *bp != ',' && *bp != '|' && *bp != ';' &&
	   *bp != '='  && *bp != '?') {
	bp++;
	length++;
    }
    while (*bp == ',' || isspace(*bp)) bp++;
    while (length > 0 && isspace(start[length-1])) length--;
    if (length==0) return 0;
    char* result = new char[length+1];
    result[length--] = '\0';
    for (; length>=0; length--) result[length] = start[length];
    return result;
}

//A "filename" is terminated by one of the characters , | = ? ; \0 OR SPACE
//A NEW string is returned with the contents;
//0 is returned if all is blank. 
//The bp at return points to the first character after the name 
//which is not ',' or space. (May be '\0').
char* get_filename(char*& bp) {
    char* start = bp;
    int length = 0;
    while (*bp != '\0' && *bp != ',' && *bp != '|' && *bp != ';' &&
	   *bp != '='  && *bp != '?' && !isspace(*bp)) {
	bp++;
	length++;
    }
    while (*bp == ',' || isspace(*bp)) bp++;
    while (length > 0 && isspace(start[length-1])) length--;
    if (length==0) return 0;
    char* result = new char[length+1];
    result[length--] = '\0';
    for (; length>=0; length--) result[length] = start[length];
    return result;
}

//Will return with 1 if the next character is ch, 0 otherwise.
//The bp at return points to the first character (if ch found, after this)
//which is not ',' or space. (May be '\0').
int get_char(char*& bp, char ch) {
    if (*bp==ch) {
	bp++;
	while (*bp == ',' || isspace(*bp)) bp++;
	return 1;
    }
    while (*bp == ',' || isspace(*bp)) bp++;
    return 0;
}

//A "commandword" is terminated by one of the characters , | = ? \0 OR SPACE
//If what remains is some nonzero part of the given "command", 1 is returned,
//otherwise 0. We may assume command consists of one or more letters.
//We also assume at the start that *bp == *command.
//The bp at return points to the first character after the name 
//which is not ',' or space. (May be '\0').
int test_command(char*& bp, const char* command) {
    while (*bp != '\0' && *bp == *command) {bp++; command++; }
    if (isspace(*bp) || *bp == '?' || *bp == '\0' ||
	*bp == '|' || *bp == ',') {
	while (isspace(*bp) || *bp == ',') *bp++;
	return 1;
    }
    return 0;
}

//Try to read any of the legal interactive command words.
//If the first character is not correct, return '\0'.
//Otherwise, if the word is not correct, return 'E'.
//Otherwise, return with the first letter of the command.
//The bp at return points to the first character after the command
//which is not ',', '|' or space. (May be '\0').
//NOTE THE clean COMMAND!
char read_command(char*& bp) {
    if (*bp=='i') if (test_command(bp, "information")) return 'i'; else return 'E';
    if (*bp=='m') if (test_command(bp, "males")) return 'm'; else return 'E';
    if (*bp=='f') if (test_command(bp, "females")) return 'f'; else return 'E';
    if (*bp=='p') if (test_command(bp, "parents")) return 'p'; else return 'E';
    if (*bp=='c') {
	if (test_command(bp, "children")) return 'c'; 
	else if (strncmp(bp, "lean",4)==0) {
	    bp += 4;
	    while (isspace(*bp) || *bp == ',') *bp++;
	    return 'C';
	}
	else return 'E';
    }
    if (*bp=='o') if (test_command(bp, "odds")) return 'o'; else return 'E';
    if (*bp=='s') if (test_command(bp, "system")) return 's'; else return 'E';
    if (*bp=='a') if (test_command(bp, "alleles")) return 'a'; else return 'E';
    if (*bp=='d') if (test_command(bp, "data")) return 'd'; else return 'E';
    if (*bp=='e') if (test_command(bp, "execute")) return 'e'; else return 'E';
    if (*bp=='w') if (test_command(bp, "write")) return 'w'; else return 'E';
    if (*bp=='r') if (test_command(bp, "read")) return 'r'; else return 'E';
    if (*bp=='q') if (test_command(bp, "quit")) return 'q'; else return 'E';
    if (*bp=='l') if (test_command(bp, "list")) return 'l'; else return 'E';
    if (*bp=='h') if (test_command(bp, "help")) return 'h'; else return 'E';
    if (*bp=='?') {
	bp++; while (*bp==',' || *bp=='|' || isspace(*bp)) bp++; return 'h';
    } 
    return '\0';
}

//Read information interactively:
char* read_information() {
    const int buflength = 100;
    int infobuflength = buflength;
    char* infobuffer = new char[infobuflength+1];
    return infobuffer;
}

void line_error(char* buf, int info, int& error) 
{
// Removed 2012-03-08
//   if (info>0)
//   {
//      cout<<"ERROR: Line not understood:\n"<<buf<<'\n';
//      cout<<"To list all possible commands, type \"help\".\n";
//   }
   error = 1; 
}

void file_error(char* buf, int info, int& error) 
{
// Removed 2012-03-08
//   if (info>0)
//      cout<<"ERROR: Line not understood:\n"<<buf<<'\n';
   error = 1; 
}

void end_error(char* buf, int info, int& error) 
{
// Removed 2012-03-08
//   if (info>0)
//      cout<<"ERROR: The end of the line not understood:\n"<<buf<<'\n';
   error = 1; 
}

//Write to the given stream. bp may contain extra instructions.
//Output error messages if these are not understandable. 
void write(char* bp, ostream& out, pater& pat, char command, 
	   int info, int& error) {
    if (*bp=='\0') pat.write(out);
    else if (*bp=='i' && test_command(bp, "information")) {
	pat.write_info(out);
	if (*bp!='\0') end_error(bp, info, error);
    }
    else if (*bp=='m' && test_command(bp, "males")) {
	pat.write_persons(1, out);
	if (*bp!='\0') end_error(bp, info, error);
    }
    else if (*bp=='s' && test_command(bp, "systems")) {
	char* name;
	if ((name = get_name(bp))) {
	    pat.write_system(out, name, info, error);
	    if (*bp!='\0') end_error(bp, info, error);
	} else if (*bp=='\0') 
	    pat.write_alleles(out);
	else help(command);
    }
    else if (*bp=='f') {
	bp++;
	if (*bp=='a' && test_command(bp, "amily")) {
	    pat.write_family(out);
	    if (*bp!='\0') end_error(bp, info, error);
	} else if (*bp=='e' && test_command(bp, "emales")) {
	    pat.write_persons(0, out);
	    if (*bp!='\0') end_error(bp, info, error);
	} else if (*bp=='r' && test_command(bp, "requencies")) {
	    char* name;
	    if ((name = get_name(bp))) {
		pat.write_system_freq(out, name, info, error);
		if (*bp!='\0') end_error(bp, info, error);
	    } else if (*bp=='\0')
		pat.write_freq(out);
	    else help(command);
	} else for (;;) {
		if (*bp=='\0') 
		{
// Removed 2012-03-08
//		   if (info>0)
//		      cout<<"ERROR: \"f\" is ambiguous; use either \"fa\""
//			 " for \"family\",\n\"fe\" for \"females\", or "
//			 " \"fr\" for \"frequencies\".\n";
		   error = 1; 
		   break;
		}
		if (isspace(*bp)) bp++;
		else {
		    help(command);
		    break;
		}
	}
    }
    else help(command);
}

void read_parents_command(char* bp, int has_min, pater& pat, 
			  int info, int& error) {
    char* parent1;
    char* parent1copy;
    char* parent2;
    char* parent2copy;
    char* child;
    char* childcopy;
    if ((parent1 = get_name(bp))) {
	if (get_char(bp,';')) {
	    if ((child = get_name(bp))) {
		do {
		    parent1copy = strcpy(new char[(int)strlen(parent1)+1],parent1);
		    if (has_min) pat.remove_parent(parent1copy, child,
						   info, error);
		    else pat.add_parent(parent1copy, child, info, error);
		} while ((child = get_name(bp)));
		delete[] parent1;
		if (*bp !='\0') end_error(bp, info, error);
	    } else help('p');
	} else if ((parent2 = get_name(bp)) && get_char(bp,';') &&
		   (child = get_name(bp))) {
	    do {
		parent1copy = strcpy(new char[(int)strlen(parent1)+1],parent1);
		parent2copy = strcpy(new char[(int)strlen(parent2)+1],parent2);
		childcopy = strcpy(new char[(int)strlen(child)+1],child);
		if (has_min) {
		    pat.remove_parent(parent1copy, childcopy, info, error);
		    pat.remove_parent(parent2copy, child, info, error);
		} else {
		    pat.add_parent(parent1copy, childcopy, info, error);
		    pat.add_parent(parent2copy, child, info, error);
		}
	    } while ((child = get_name(bp)));
	    delete[] parent1; delete[] parent2;
	    if (*bp != '\0') end_error(bp, info, error);
	} else help('p');
    } else help('p');
}

void file_read_parents_command(char* bp, char* buf, pater& pat, int info, int& error) {
    char* parent1;
    char* parent1copy;
    char* parent2;
    char* parent2copy;
    char* child;
    char* childcopy;
    if ((parent1 = get_name(bp))) {
	if (get_char(bp,'|')) {
	    if ((child = get_name(bp))) {
		do {
		    parent1copy = strcpy(new char[(int)strlen(parent1)+1],parent1);
		    pat.add_parent(parent1copy, child, info, error);
		} while ((child = get_name(bp)));
		delete[] parent1;
		if (*bp !='|') end_error(bp, info, error);
	    } else file_error(buf, info, error);
	} else if ((parent2 = get_name(bp)) && get_char(bp,'|') &&
		   (child = get_name(bp))) {
	    do {
		parent1copy = strcpy(new char[(int)strlen(parent1)+1],parent1);
		parent2copy = strcpy(new char[(int)strlen(parent2)+1],parent2);
		childcopy = strcpy(new char[(int)strlen(child)+1],child);
		pat.add_parent(parent1copy, childcopy, info, error);
		pat.add_parent(parent2copy, child, info, error);
	    } while ((child = get_name(bp)));
	    delete[] parent1; delete[] parent2;
	    if (*bp != '|') end_error(bp, info, error);
	} else file_error(buf, info, error);
    } else file_error(buf, info, error);
}

void read_children_command(char* bp, char* buf, int has_min, pater& pat, 
			   int info, int& error) {
    const int maxchildren = 20;
    char* child[maxchildren];
    char* parent;
    char* parentcopy;
    char* childcopy;
    int n_children = 0;
    int i;
    do {
	if (n_children==maxchildren) 
	{
// Removed 2012-03-08
//	   if (info>0)
//	      cout<<"ERROR: Too many children in a single command:\n"<<buf<<'\n';
	   error = 1; 
	   return;
	}
	if (!(child[n_children++] = get_name(bp))) {
	    help('c'); return; 
	}
    } while (!get_char(bp,';'));
    if ((parent = get_name(bp))) {
	for (i=0; i<n_children; i++) {
	    parentcopy = strcpy(new char[(int)strlen(parent)+1],parent);
	    childcopy = strcpy(new char[(int)strlen(child[i])+1],child[i]);
	    if (has_min) pat.remove_parent(parentcopy, childcopy, info, error); 
	    else pat.add_parent(parentcopy, childcopy, info, error);
	}
	delete[] parent;
	if ((parent = get_name(bp))) {
	    for (i=0; i<n_children; i++) {
		parentcopy = strcpy(new char[(int)strlen(parent)+1],parent);
		childcopy = strcpy(new char[(int)strlen(child[i])+1],child[i]);
		if (has_min) pat.remove_parent(parentcopy, childcopy, 
					       info, error); 
		else pat.add_parent(parentcopy, childcopy, info, error);
	    }
	    delete[] parent;
	}
	if (*bp!='\0') end_error(bp, info, error);
    } else help('c');
    for (i=0; i<n_children; i++) delete[] child[i];
}

void read_allele_command(char* bp, int has_min, pater& pat, 
			 int info, int& error) {
    char* systemname;
    char* systemnamecopy;
    char* allelename;
    char* probability;
    char* savebp;
    if ((systemname = get_name(bp)) && get_char(bp,';')) {
	if (has_min)
	    if ((allelename = get_name(bp))) {
		do {
		    systemnamecopy = strcpy(new char[(int)strlen(systemname)+1],
					    systemname);
		    pat.remove_allele(systemnamecopy, allelename, info, error); 
		} while (get_char(bp,';') && (allelename = get_name(bp)));
		if (*bp!='\0') end_error(bp, info, error);
	    } else help('a');
	else 
	    if ((allelename = get_name(bp))) {
		if ((probability = get_filename(bp))) {
		    for(;;) {
			systemnamecopy = strcpy(new char
						[(int)strlen(systemname)+1],
						systemname);
			pat.add_allele(systemnamecopy, allelename, 
				       atof(probability), info, error);
			delete[] probability;
			if (!get_char(bp,';')) break;
			savebp = bp;
			if ((allelename = get_name(bp))) {
			    if ((probability = get_filename(bp))) continue;
			    delete[] allelename;
			} 
			bp = savebp;
			break;
		    } 
		    if (*bp!='\0') end_error(bp, info, error);
		} else {
		    help('a');
		    delete[] allelename;
		}
	    } else help('a');
	delete[] systemname;
    } else help('a');
}

void file_read_allele_command(char* bp, char* buf, 
			      char* systemname, pater& pat, 
			      int info, int& error) {
    char* allelename;
    char* probability;
    if ((allelename = get_name(bp))) {
	if (get_char(bp,'|') && (probability = get_filename(bp))) {
	    pat.add_allele(systemname, allelename, atof(probability), info, error);
	    delete[] probability;
	    if (!get_char(bp,'|')) end_error(bp, info, error);
	    return;
	}
	delete[] allelename;
    } else file_error(buf, info, error);
    delete[] systemname;
}

void read_execute_command(char* bp, pater& pat, int info, int& error) {
  char* name;
  int use_cutsets = 0;
  if (*bp != '\0') {
//     help('e');
//     return;
//  }

//
//    use_cutsets = 1;
//    while ((name = get_name(bp))) {
//      do {
//	if (pat.add_person_to_cutset(name, info, error)) {
//	  pat.remove_cutsets();
//	  return;
//	}
//      } while ((name = get_name(bp)));
//      if (*bp == '\0') {
//	pat.end_cutset(info, error);
//	break;
//      }
//      if (get_char(bp,';')) {
//	pat.end_cutset(info, error);
//	continue;
//      }
//      help('e');
//      pat.remove_cutsets();
//      return;
//    }
//    if (*bp != '\0') {
//      help('e'); 
//      pat.remove_cutsets();
//      return;
//    }
//  }
//  

  if (use_cutsets==0 && info==2) info = 1; 
  pat.execute(info, error);
  pat.remove_cutsets();
}

void read_data_command(char* bp, int has_min, pater& pat, 
		       int info, int& error) {
    char* systemname;
    char* systemnamecopy;
    char* name;
    char* allele1;
    char* allele2;
    char* savebp;
    if ((systemname = get_name(bp))) {
	if (get_char(bp,';')) 
	    if (has_min)
		if ((name = get_name(bp))) {
		    do {
			systemnamecopy = strcpy(new char
						[(int)strlen(systemname)+1],
						systemname);
			pat.remove_data(systemnamecopy, name, info, error);
		    } while (get_char(bp,';') && (name = get_name(bp)));
		    if (*bp!='\0') end_error(bp, info, error);
		} else help('d');
	    else
		if ((name = get_name(bp))) {
		    if ((allele1 = get_name(bp))) {
			if ((allele2 = get_name(bp))) {
			    for (;;) {
				systemnamecopy = 
				    strcpy(new char[(int)strlen(systemname)+1],
					   systemname);
				pat.add_data(systemnamecopy, name, allele1, 
					     allele2, info, error);
				if (!get_char(bp,';')) break;
				savebp = bp;
				if ((name = get_name(bp))) {
				    if ((allele1 = get_name(bp))) {
					if ((allele2 = get_name(bp))) continue;
					delete[] allele1;
				    }
				    delete[] name;
				}
				bp = savebp;
				break;
			    }
			    if (*bp!='\0') end_error(bp, info, error);
			} else {
			    delete[] name;
			    delete[] allele1;
			    help('d');
			}
		    } else {
			delete[] name;
			help('d');
		    }
		} else help('d');
	else help('d');
	delete[] systemname;
    } else help('d');
}

void file_read_data_command(char* bp, char* buf, 
			    char* systemname, pater& pat, 
			    int info, int& error) {
    char* name;
    char* allele1;
    char* allele2;
    if ((name = get_name(bp))) {
	if (get_char(bp,'|') && (allele1 = get_name(bp))) {
	    if ((allele2 = get_name(bp))) {
		pat.add_data(systemname, name, allele1, allele2, info, error);
		if (!get_char(bp,'|')) end_error(bp, info, error);
		return;
	    }
	    delete[] allele1;
	}
	delete[] name;
    }  
    delete[] systemname;
    file_error(buf, info, error);
}

const int default_nalleles = 100;
void read_system_command(char* bp, int has_min, pater& pat, 
			 int info, int& error) {
    const int standardModel = 0; 
    const double dummyRange = 0.1; 
    char* name;
    char* secondname;
    char* thirdname;
    if (has_min) {
	if ((name = get_name(bp))) {
	    pat.remove_system(name, info, error); 
	    if (*bp != '\0') end_error(bp, info, error);
	} else help('s');
    } else 
	if ((name = get_name(bp))) {
	    if (get_char(bp,';') && (secondname = get_filename(bp))) {
		if ((thirdname = get_filename(bp))) {
		    pat.add_system(name, 
				   atof(secondname), 
				   atof(secondname), 
				   standardModel, 
				   standardModel, 
				   atoi(thirdname), 
				   dummyRange, 
				   dummyRange, 
				   info, 
				   error);
		    if (*bp != '\0') end_error(bp, info, error);
		    delete[] thirdname;
		} else {
		    pat.add_system(name, 
				   atof(secondname), 
				   atof(secondname), 
				   standardModel, 
				   standardModel, 
				   default_nalleles, 
				   dummyRange, 
				   dummyRange, 
				   info, 
				   error);
		    if (*bp != '\0') end_error(bp, info, error);
		} 
		delete[] secondname;
	    } else {
		delete[] name;
		help('s');
	    }
	} else help('s');
}

//Read and do an interactive command from this buffer. 
//We have read any minus sign, 
//and bp points to something non-blank. Return 'q' if the command is quit,
//'F' if file commands may be possible, and 'E' otherwise.
char read_interactive_command(char* bp, char* buf, int has_min, int interactive, 
			      pater*& pat, int info, int& error) 
{
    char* name;
    char* secondname;

    char c = read_command(bp);
    if (c=='\0') return 'F';

    //Below, we should always return 'q' or 'E':
    if (c=='E') line_error(buf, info, error);
    else if (get_char(bp,'?')) {
	help(c);
	if (*bp!='\0') end_error(bp, info, error);
    }
    else if (c=='i') 
	if (*bp=='\0') 
	    if (has_min) pat->remove_information(info, error);
	    else if (interactive) 
		if (pat->has_information()) pat->add_information("", info, error);
		else pat->add_information(read_information(), info, error);
	    else 
	    {
// Removed 2012-03-08
//	       if (info>0)
//		  cout<<"ERROR: An information command is illegal in a file.\n";
	       error = 1; 
	    }
    
	else help(c);
    else if (c=='m') {
	while ((name = get_name(bp))) {
	    if (has_min) pat->remove_person(1, name, info, error);
	    else pat->add_person(1, name, info, error);
	}
	if (*bp != '\0') end_error(bp, info, error);
    }
    else if (c=='f') {
	while ((name = get_name(bp))) {
	    if (has_min) pat->remove_person(0, name, info, error);
	    else pat->add_person(0, name, info, error);
	}
	if (*bp != '\0') end_error(bp, info, error);
    }
    else if (c=='p') read_parents_command(bp, has_min, *pat, info, error);
    else if (c=='c') read_children_command(bp, buf, has_min, *pat, info, error);
    else if (c=='C') {
	delete pat;
	pat = new pater;
// Removed 2012-03-08
//	if (interactive) cout<<"All information removed.\n";
	if (*bp!='\0') end_error(bp, info, error);
    }
    else if (c=='o')
	if (has_min) {
	    pat->remove_odds(info, error);
	    if (*bp!='\0') end_error(bp, info, error);
	} else if ((name = get_name(bp))) {
	    if (get_char(bp,'=') && (secondname = get_name(bp))) {
		pat->add_odds(name, secondname, info, error);
		if (*bp!='\0') end_error(bp, info, error);
	    } else {
		delete[] name;
		help(c);
	    }
	} else help(c);
    else if (c=='s') read_system_command(bp, has_min, *pat, info, error);
    else if (c=='a') read_allele_command(bp, has_min, *pat, info, error);
    else if (c=='d') read_data_command(bp, has_min,   *pat, info, error);
    //Below are only commands that cannot have an '-'.
    else if (has_min) help(c);
    else if (c=='e') read_execute_command(bp, *pat, info, error);
    else if (c=='w') 
	if (*bp=='\0') 
	{
// Removed 2012-03-08
//	   if (info>0)
//	      cout<<"ERROR: The write command must be followed by a file name.\n";
	   error = 1; 
	}
	else if ((name = get_filename(bp))) {
	    ofstream file(name);
	    if (!file.good()) 
	    {
// Removed 2012-03-08
//	       if (info>0)
//		  cout<<"ERROR: Cannot open file \""<<name<<"\".\n";
	       error = 1; 
	    }
	    else {
		pat->write_top(file);
		write(bp, file, *pat, c, info, error);
	        file<<separator;
		pat->has_written();
	    }
	    delete[] name;
	} else help(c);
    else if (c=='r')
	if (*bp=='\0') 
	{
// Removed 2012-03-08
//	   if (info>0)
//	      cout<<"ERROR: The read command must be followed by a file name.\n";
	   error = 1; 
	}
	else if ((name = get_filename(bp))) 
	{
	   if (1) //to delete ifstream before name
	   {
	     //#ifdef UNIXVERSION
	      ifstream file(name);
	      //#else
	      //ifstream file(name, ios::nocreate);
	      //#endif
	      if (!file)
	      {
// Removed 2012-03-08
//		 if (info>0)
//		    cout<<"ERROR: Cannot open file \""<<name<<"\".\n";
		 error = 1; 
	      }
	      else 
	      {
		 do_commands(file, 0, pat, error);
		 if (*bp != '\0') end_error(bp, info, error);
	      }
	   }
	   delete[] name;
	} else help(c);
    else if (c=='q') 
       { 
	    if (*bp=='\0') return 'q';
	    else line_error(buf, info, error);
        }
// Removed 2012-03-08
//    else if (c=='l') write(bp, cout, *pat, c, info, error);
    else if (c=='h') 
	if (*bp=='\0') help('\0'); 
	else {
	    c = read_command(bp);
	    if (c=='\0' || c=='E') help('\0');
	    else {
		help(c);
		if (*bp!='\0') end_error(bp, info, error);
	    }
	}
    
    return 'E';
}

void read_file_command(char* bp, char* buf, int buflength, istream& in,
		       char*& allelesystem, pater& pat, int info, 
		       int& error) 
{
   const int standardModel = 0; 
   const double dummyRange = 0.1; 
   if (info==2) info = 1; 

    char* name;
    char* secondname;
    //Three possible multi-line commands:
    if (strcmp(bp,"RESULTS")==0) {
	//Read in the following line:
	if (!get_line(in,buf,buflength, info, error)) return;
	for(;;) { //Read in lines until a starline.
	    if (!get_line(in,buf,buflength, info, error)) return;
	    if (strcmp(buf,starline)==0) return;
	}
    }	
    if (strcmp(bp,"DNA PROBABILITY COMPUTATIONS REPORT")==0) {
	for (;;) {
	    //Read in the rest of the heading:
	    if (!get_line(in,buf,buflength, info, error)) return;
	    if (strcmp(buf,starline)==0) break;
	}

	//Read in information, ending with a starline, or with EOF.
	int n_stars = 0;
	int infobuflength = buflength;
	char* infobuffer = new char[infobuflength+3];
	int i = 0;
	char c = ' ';

	for (;;) {
	    if (!in.get(c)) {
		for (;;) {
		    if (i==0) {
			delete[] infobuffer;
			return;
		    }
		    if (isspace(infobuffer[i-1])) i--;
		    else {
			infobuffer[i] = '\n';
			infobuffer[i+1] = '\0';
			pat.add_information(infobuffer, info, error);
			delete[] infobuffer; 
			return;
		    }
		}
	    }
	    if (c=='*') {
		if (n_stars==number_of_stars-1) {
		    while (in.get(c)) if (c=='\n') break;
		    i -= number_of_stars-1;
		    if (i==0) {
			delete[] infobuffer;
			return; 
		    }
		    infobuffer[i] = '\0';
		    if (pat.has_information()) 
		       pat.add_information("", info, error);
		    else 
		       pat.add_information(infobuffer, info, error);
		    delete[] infobuffer;
		    return;
		}
		n_stars++;
	    } else n_stars = 0;
	    if (c=='\15') continue;
	    if (i==infobuflength) {
		char* replacement = new char[infobuflength+buflength+3];
		for (i=0; i<infobuflength; i++)
		    replacement[i] = infobuffer[i];
		delete[] infobuffer;
		infobuffer = replacement;
		infobuflength += buflength;
	    }
	    infobuffer[i++] = c;
	}
    }
    if (*bp=='+') { //This is a "table":
	char* tabletype;
	if (!get_line(in, buf, buflength, info, error)) return;
	if (*buf=='|') {
	    bp = buf;
	    while (isspace(*(++bp)));
	    if ((tabletype = get_filename(bp))) {
		if (strcmp(tabletype,"parents")==0 ||
		    strcmp(tabletype,"allele")==0 ||
		    strcmp(tabletype,"person")==0) {
		    if (!get_line(in, buf, buflength, info, error)) {
			delete[] tabletype;
			return;
		    }
		    if (*buf=='+') {
			if (!get_line(in, buf, buflength, info, error)) {
			    delete[] tabletype;
			    return;
			}
			if (*buf=='|') {
			    for(;;) {
				bp = buf;
				while (isspace(*(++bp)));
				if (tabletype[1]=='a') 
				    file_read_parents_command(bp, buf, pat, info, error);
				else if (tabletype[1]=='l') 
				    if (allelesystem)
					file_read_allele_command
					    (bp, buf, strcpy(new char[(int)strlen(allelesystem)+1], allelesystem), pat, info, error);
				    else 
				    {
// Removed 2012-03-08
//				       if (info>0)
//					  cout<<"ERROR: Cannot read lines from"
//					     " an allele table before any "
//					     "\"ALLELE SYSTEM\" heading.\n";
				       error = 1; 
				    }
				else if (tabletype[1]=='e')
				    if (allelesystem)
					file_read_data_command
					    (bp, buf, strcpy(new char[(int)strlen(allelesystem)+1], allelesystem), pat, info, error);
				    else 
				    {
// Removed 2012-03-08
//				       if (info>0)
//					  cout<<"ERROR: Cannot read lines from"
//					     " a DNA data table before any "
//					     "\"ALLELE SYSTEM\" heading.\n";
				       error = 1; 
				    }
				if (!get_line(in, buf, buflength, info, error)||
				    (*buf=='+')){
				    delete[] tabletype;
				    return;
				}
				if (*buf!='|') break;
			    }
			}
		    }
		}
		delete[] tabletype;
	    }
	}
	file_error(buf, info, error);
	return;
    }
    //Three possible one-line commands:
    if (strncmp(bp,"Mutation probability:",21)==0) {
	if (allelesystem==0) 
	{
// Removed 2012-03-08
//	   if (info>0)
//	   {
//	      cout<<"ERROR: A \"Mutation probability\" line cannot be given ";
//	      cout<<"before any \"ALLELE SYSTEM\" heading.\n";
//	   }
	   error = 1; 
	   return;
	}
	bp = bp + 21;
	while (isspace(*bp)) bp++;
	if ((name = get_filename(bp))) {
	    if (strncmp(bp,"number of possible alleles:",27)==0) {
		bp = bp + 27;
		while (isspace(*bp)) bp++;
		if ((secondname = get_filename(bp))) {
		    char* allelesystemcopy = strcpy(new char[(int)strlen(allelesystem)+1],
						    allelesystem);
		    pat.add_system(allelesystemcopy, 
				   atof(name), 
				   atof(name), 
				   standardModel, 
				   standardModel, 
				   atoi(secondname), 
				   dummyRange, 
				   dummyRange, 
				   info, 
				   error);
		    if (*bp!='\0') end_error(bp, info, error);
		    delete[] secondname;
		} else file_error(buf, info, error);
	    } else {
		char* allelesystemcopy = strcpy(new char[(int)strlen(allelesystem)+1],
						allelesystem);
		pat.add_system(allelesystemcopy, 
			       atof(name), 
			       atof(name), 
			       standardModel, 
			       standardModel, 
			       default_nalleles, 
			       dummyRange, 
			       dummyRange, 
			       info, 
			       error);
		if (*bp!='\0') end_error(bp, info, error);
	    } 
	    delete[] name;
	} else file_error(buf, info, error);
	return;
    }
    if (strncmp(bp,"Question for odds computation: is ",34)==0) {
	bp = bp + 34;
	while (isspace(*bp)) bp++;
	if ((name = get_name(bp))) {
	    if (get_char(bp,'=') && (secondname = get_name(bp))) {
		pat.add_odds(name, secondname, info, error);
		get_char(bp,'?');
		if (*bp!='\0') end_error(bp, info, error);
	    } else {
		delete[] name;
		file_error(buf, info, error);
	    }
	} else file_error(buf, info, error);	
	return;
    }
    if (strncmp(bp,"ALLELE SYSTEM ",14)==0) {
	bp = bp + 14;
	while (isspace(*bp)) bp++;
	if ((name = get_name(bp))) {
	    delete[] allelesystem;
	    allelesystem = name;
	    if (*bp!='\0') end_error(bp, info, error);
	} else file_error(buf, info, error);
	return;
    }
    //A number of one-line null commands:
    if (*bp=='\\') return; //comment line
    if (*bp=='/') return; //comment line
    if (*bp=='#') return; //comment line
    if (*bp=='%') return; //comment line
    if (*bp=='$') return; //comment line
    if (strcmp(bp,starline)==0) return;
    if (strcmp(bp,"FAMILY STRUCTURE")==0) return;
    if (strcmp(bp,"Family relations:")==0) return;
    if (strcmp(bp,"No family relations registered.")==0) return;
    if (strcmp(bp,"General population frequencies of alleles:")==0) return;
    if (strcmp(bp,"No alleles registered.")==0) return;
    if (strcmp(bp,"Observed alleles in this system:")==0) return;
    if (strcmp(bp,"No observations of alleles registered.")==0) return;
    if (strncmp(bp,"The odds that ",14)==0) return;
    if (strncmp(bp,"The probability of the data given the family ",45)==0)
	return;
    //All possibilities are exhausted:
    file_error(buf, info, error);
}

//The following routine reads input from a stream. If interactive==1, only
//interactive commands are recognized, if interactive==0, all command
//types are recognized (except "info"). It will
//read until it finds a legal command, possibly outputing error messages
//along the way. It will then call the appropriate routine of the 
//pater object supplied, unless the command is "quit" or if 
//the end of file is reached; then it will just terminate. 
void do_commands(istream& in, const int interactive, 
		 pater*& pat, int& error) 
{
    const int buflength = 100;
    char buf[buflength];
    char* bp; //will  point to the place in the buffer we are at.
    char c;
    int has_min;
    //The following data item is only used when interactive==0:
    //Then, it is 0 unless one has read a file command "defining"
    //an allele system. In that case, it gives the name of the last
    //such system.
    char* allelesystem = 0;
    
    int info; 
    if (interactive) 
       info = 2; 
    else
       info = 1; 

    for (;;) {
	//Return if end of file is reached:
	if (!in) break;
// Removed 2012-03-08
//	if (interactive) cout<<"interactive> ";

	//Read in new line:
	in.get(buf, buflength,'\n');
	if (in.get(c) && c != '\n') 
	{
// Removed 2012-03-08
//	   if (info>0)
//	      cout<<"ERROR: The entered line is too long, and is discarded:\n"
//		  <<buf<<'\n';
	   if (!interactive) error = 1; 
	   //Throw away rest of line:
	   while (in.get(c) && c != '\n');
	   continue;
	}

	//Discard possible '\15' at end of line:
	bp = buf;
	while (*bp!='\0') 
	    if (*bp=='\15') {
		*bp = '\0';
		break;
	    } else bp++;

	//Read and store possible minus sign, and blanks:
	bp = buf;
	while (isspace(*bp)) bp++;
	if (*bp=='\0') continue; //blank line ignored
	if (*bp=='-') {
	    has_min = 1; 
	    bp++;
	    while (isspace(*bp)) bp++;
	    if (*bp=='\0') 
	    {
// Removed 2012-03-08
//	       if (info>0)
//		  cout<<"ERROR: Line containing only\"-\".\n";
	       if (!interactive) error = 1; 
	       continue;
	    }
	} else has_min = 0;

	//Read and do the possible interactive commands. 
	//Return with 'q' if quit,
	//return with 'F' if file commands may be possible.
	c = read_interactive_command(bp, buf, has_min, 
				     interactive, pat, 
				     info, error);
	if (interactive) error = 0; //assumed already handled
	if (c=='q') break;
	if (c=='F') 
	{
	    if (interactive) line_error(buf, info, error);
	    else if (has_min)
	    {
// Removed 2012-03-08
//	       if (info>0)
//		  cout<<"ERROR: Command line not understood:\n"<<buf<<'\n';
	       error = 1; 
	    }
	    else read_file_command(bp, buf, buflength, 
				   in, allelesystem, *pat, 
				   info, error);
	    if (interactive) error = 0; //assumed already handled
	}
    }
    delete[] allelesystem;
}

*/
