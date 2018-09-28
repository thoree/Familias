/* $Id: table.C,v 1.7 2000/08/07 07:44:10 mostad Exp $ */

#include <cstdlib>
#include <cstring>
#include <sstream> 

#include "special.h"
#include "table.h"

class tableitem {
public:
    tableitem* next;
    tableitem() : next(0) {}
    virtual ~tableitem() {}
    virtual void printout(ostream& out, int& rem_chars, int needs_comma) = 0;
    virtual int length() = 0;
};

class str_item : public tableitem {
protected:
    char* str;
    int len;
public:
    str_item(){}
    ~str_item(){}
    void printout(ostream& out, int& rem_char, int needs_comma) {
	if (needs_comma) { out<<",  "; rem_char -= 3;}
	else { out<<"  "; rem_char -= 2;}
	out<<str;
	rem_char -= len;
    }
    int length() { return len;}
};

class string_item : public str_item {
public:
    string_item(char* s) {
	str = s;
	len = strlen(s);
    } 
    ~string_item() {}
};

char* my_place_in_string(double d) {
    char* result = new char[16];

    //Cant get this to work with new complier; fix later: 
    //if (1) {
    //	stringstream ost(result, 15);
    //	ost<<d;
    //	ost<<'X';
    //}
    stringstream ss (stringstream::in | stringstream::out); 
    ss << d; 
    ss >> result; 
    int length = 0;


    //    while (result[length]!='X') length++;
    length = 15; 


    result[length] = '\0';
    return result;
}

class float_item : public str_item {
public:
    float_item(double d) {
	str = my_place_in_string(d);
	len = strlen(str);
    }
    ~float_item() { delete[] str; }
};

class end_column : public tableitem {
public:
    end_column() {}
    ~end_column() {}
    void printout(ostream& out, int& rem_char, int) {
	while (rem_char) {
	    rem_char--;
	    out<<' ';
	}
    }
    int length() { return 0;}
};
	

table::table(const char* heading1, const char* heading2) :
head1(heading1), head2(heading2) {
    first = last = before_curr_col = new end_column; //dummy
    collength1 = strlen(heading1) + 4;
    collength2 = strlen(heading2) + 4;
    currcol = 1;
}  

void table::put(char* item) {
    last->next = new string_item(item);
    last = last->next;
}

void table::put(double item) {
    last->next = new float_item(item);
    last = last->next;
}

void table::endcolumn() {
    tableitem* p = before_curr_col->next;
    int length = 2; //The two spaces at the end.
    if (p) length += 2 + p->length();
    while((p = p->next)) length += 3 + p->length();
    if (currcol==1) {
	if (collength1 < length) collength1 = length;
	currcol = 2;
    }
    else {
	if (collength2 < length) collength2 = length;
	currcol = 1;
    }
    last->next = new end_column;
    last = last->next;
    before_curr_col = last;
}

void table::printout(ostream& out) {
    int i;
    out<<'+';
    for (i=0; i<collength1; i++) out<<'-';
    out<<'+';
    for (i=0; i<collength2; i++) out<<'-';
    out<<"+\n|  "<<head1;
    for (i=0; i<collength1 - int(strlen(head1)) - 2; i++) out<<" ";
    out<<"|  "<<head2;
    for (i=0; i<collength2 - int(strlen(head2)) - 2; i++) out<<" ";
    out<<"|\n+";
    for (i=0; i<collength1; i++) out<<'-';
    out<<'+';
    for (i=0; i<collength2; i++) out<<'-';
    out<<"+\n";
    tableitem* p = first->next;
    int char_rem;
    while (p) {
	//At start of line
	out<<'|';
	char_rem = collength1;
	p->printout(out, char_rem, 0);
	p = p->next;
	while (char_rem) {
	    p->printout(out, char_rem, 1);
	    p = p->next;
	}
	out<<'|';
	char_rem = collength2;
	p->printout(out, char_rem, 0);
	p = p->next;
	while (char_rem) {
	    p->printout(out, char_rem, 1);
	    p = p->next;
	}
	out<<"|\n";
    }
    out<<'+';
    for (i=0; i<collength1; i++) out<<'-';
    out<<'+';
    for (i=0; i<collength2; i++) out<<'-';
    out<<"+\n";
}

table::~table() {
    last = first;
    while (first) {
	first = first->next;
	delete last;
	last = first;
    }
}

