/* $Id: special.h,v 1.13 2001/04/12 16:49:41 mostad Exp $ */

#ifndef __SPECIAL_H_
#define __SPECIAL_H_

// 2012-02-01: What is this??
//#ifdef UNIXVERSION
//#include <strstream>
//#else
//#include <strstrea>
//#endif
// replaced with 
//# include <strstream> ??
#include <iostream> 
#include <fstream>
using namespace std; 

const char separator[80] = "******************************************************************************\n";
const char starline[79] = "******************************************************************************";
const int number_of_stars = 78;

const char version[] = "1.7";
const double numericVersion = 1.7; 
const char dump_file_name[] = "dump";

#ifdef UNIXVERSION

const int MAX_TABLE_SIZE = 100000; //could be larger??

#else

const int MAX_TABLE_SIZE = 32767;

#endif

extern void writedate(ostream& out);
extern void system_delete_file(const char* filename);
extern ifstream* open_file(const char* name);

//Make a new copy of the input string:
extern char* newcopy(const char* input);

//Extract a double from the string: 
extern double getDouble(char* str, int& OK); 

//Extract an integer from the string: 
extern int getInteger(char* str, int& OK); 

extern char* getNamePrefix(int nNames, char** names); 

extern double mypow(double t, int exponent); 

#endif
