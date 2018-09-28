/* $Id: Element.h,v 1.3 1999/09/25 12:43:52 mostad Exp $ */

#ifndef __ELEMENT_H_
#define __ELEMENT_H_

class Element 
{
   public:
      Element(); 
      Element(Element* nx); 
      virtual ~Element() {}
      
      Element* getNext() const; 
      void deleteNext(); 
      int getListLength() const; 
      void append(Element* nx); 

   private:

      Element* next; 
};


#endif

