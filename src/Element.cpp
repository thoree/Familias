/* $Id: Element.C,v 1.4 2000/03/03 09:44:56 mostad Exp $ */

#include "Element.h"

Element::Element()
{
   next = 0;
}

Element::Element(Element* nx)
{
   next = nx; 
}

Element* Element::getNext() const
{
   return next; 
}

void Element::deleteNext() 
{
   if (next==0) return; 
   Element* newNext = next->next; 
   delete next; 
   next = newNext; 
}

int Element::getListLength() const
{
   int result = 1; 
   Element* e = next; 
   while (e)
   {
      result++; 
      e = e->next;
   }
   return result; 
}

void Element::append(Element* nx)
{
   next = nx; 
}
