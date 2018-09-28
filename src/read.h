/* $Id: read.h,v 1.5 2000/08/07 07:44:09 mostad Exp $ */

#ifndef __READ_H_
#define __READ_H_

#include <fstream>
using namespace std; 

class pater;

//The following routine reads input from a stream. If interactive==1, only
//interactive commands are recognized, if interactive==0, all command
//types are recognize (except "info"). It will
//read until it finds a legal command, possibly outputing error messages
//along the way. It will then call the appropriate routine of the 
//pater object supplied, unless the command is "quit" or if 
//the end of file is reached; then it will just terminate. 

//NOT USED: 
//extern void do_commands(istream& in, const int interactive, pater*& pat, int& error);

#endif
