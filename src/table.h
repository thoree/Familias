/* $Id: table.h,v 1.5 2000/08/07 07:44:10 mostad Exp $ */

#ifndef __TABLE_H_
#define __TABLE_H_

#include <fstream>
using namespace std; 

class tableitem;

class table {
    const char* head1;
    const char* head2;
    tableitem* first;
    tableitem* last;
    tableitem* before_curr_col;
    //The maximal length of _finished_ columns/lines:
    int collength1, collength2;
    //The current column:
    int currcol;
public:
    //Create a table with the two headings. The headings are deleted 
    //when the table is deleted.
    table(const char* heading1, const char* heading2);
    ~table();
    //Enter something into the current line in the current column.
    void put(char* item);
    //The strings that are input are NOT deleted afterwards.
    void put(double item);
    //End input to a column line:
    void endcolumn();
    //Test whether the table is empty:
    int contains_data() { return first != last; }
    //Write out table. We ASSUME it has been ended properly.
    void printout(ostream& out);
};

#endif
