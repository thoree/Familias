/* $Id: special.C,v 1.11 2000/08/07 07:44:09 mostad Exp $ */

#include <cstdio>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <sstream> 

using namespace std; 

#include "special.h"

void writedate(ostream& out) {
  char date[17];
  time_t thetime = time(0);
  char* stringtime = ctime(&thetime);
  int i;
  for (i=0; i<11; i++) date[i] = stringtime[i];
  for (i=11; i<16; i++) date[i] = stringtime[i+8];
  date[16]='\0';
  out<<date;
}

void system_delete_file(const char* filename) {
  remove(filename);
}

//Make a new copy of the input string:
char* newcopy(const char* input) {
   if (input != 0)
      return strcpy(new char[strlen(input)+1], input);
   return 0;
}

double getDouble(char* str, int& OK)
{
   double result = 0; 
   istringstream ist(str); 
   if (!ist || !(ist>>result) || !ist)
      OK = 0; 
   else 
      OK = 1; 
   return result; 
}

int getInteger(char* str, int& OK)
{
   int result = 0; 
   istringstream ist(str); 
   if (!ist || !(ist>>result) || !ist)
      OK = 0; 
   else 
      OK = 1; 
   return result; 
}

char* getNamePrefix(int nNames, char** names)
{
   int counter = 0; 
   char* result = new char[100]; 
   int i; 
   for (;;)
   {
      sprintf(result, "E%d", ++counter); 
      for (i=0; i<nNames; i++)
	 if (strncmp(names[i], result, strlen(result))==0) break; 
      if (i==nNames) return result;
   }
}

double mypow(double t, int exponent)
{
   if (exponent<0) return 0; 
   double result = 1; 
   while (exponent-->0)
      result *= t; 
   return result; 
}
