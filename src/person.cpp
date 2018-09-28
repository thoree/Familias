/* $Id: person.C,v 1.4 2000/03/03 09:45:04 mostad Exp $ */

#include "special.h"
#include "table.h"
#include "person.h"
#include "alsys.h"

person::person(char* name, int ml) :
  alias(0), collapsed_alias(0), 
  Name(name), male(ml), 
  mother(0), father(0),
  child(0), paternal_sibling(0), maternal_sibling(0),
  next(0), is_processed(0) {}


void person::write_parents(table& tab) {
    if (!is_processed) {
	if (mother && father) {
	    tab.put(mother->Name); 
	    tab.put(father->Name);
	    tab.endcolumn();
	    tab.put(Name);
	    person* q = next;
	    while (q) {
		if (q->mother==mother && q->father==father) {
		    tab.put(q->Name);
		    q->is_processed = 1;
		}
		q = q->next;
	    }
	    tab.endcolumn();
	} else if (mother) {
	    tab.put(mother->Name);
	    tab.endcolumn();
	    tab.put(Name);
	    person* q = next;
	    while (q) {
		if (q->mother==mother && q->father==0) {
		    tab.put(q->Name);
		    q->is_processed = 1;
		}
		q = q->next;
	    }
	    tab.endcolumn();
	}else if (father) {
	    tab.put(father->Name);
	    tab.endcolumn();
	    tab.put(Name);
	    person* q = next;
	    while (q) {
		if (q->father==father && q->mother==0) {
		    tab.put(q->Name);
		    q->is_processed = 1;
		}
		q = q->next;
	    }
	    tab.endcolumn();
	}
    }
}

void person::clear_parents() {
    is_processed = 0;
    if (next) next->clear_parents();
}

void person::add_parent(person* p) {
    if (p->male) { 
	father = p;
	paternal_sibling = father->child;
	father->child = this;
    } else {
	mother = p;
	maternal_sibling = mother->child;
	mother->child = this;
    }
}

void person::remove_mother() {
    if (mother->child == this) mother->child = maternal_sibling;
    else {
	person* q = mother->child;
	while (q->maternal_sibling != this) 
	    q = q->maternal_sibling;
	q->maternal_sibling = maternal_sibling;
    }
    mother = 0;
}

void person::remove_father() {
    //We assume that there is a father.
    if (father->child == this) father->child = paternal_sibling;
    else {
	person* q = father->child;
	while (q->paternal_sibling != this) 
	    q = q->paternal_sibling;
	q->paternal_sibling = paternal_sibling;
    }
    father = 0;
}

void person::discard_children() {
    person* p = child;
    person* q;
    while (p) {
	q = p;
	if (male) {
	    p->father = 0;
	    p = q->paternal_sibling;
	    q->paternal_sibling = 0;
	} else {
	    p->mother = 0;
	    p = q->maternal_sibling;
	    q->maternal_sibling = 0;
	}	    
    }
}

person* person::remove_next(person* p) { 
    if (!next) return 0;
    if (p==next) {
	next = next->next;
	return this;
    }
    return next->remove_next(p);
}

void person::add_next(person* p) {
    p->next = next;
    next = p;
}

void person::add_at_end(person* p) {
    if (next) next->add_at_end(p);
    else next = p;
}

int person::has_ancestor(person* p) {
    if (mother && (mother==p || mother->has_ancestor(p))) return 1;
    if (father && (father==p || father->has_ancestor(p))) return 1;
    return 0;
}


