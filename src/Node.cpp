/* $Id: Node.C,v 1.4 2000/03/03 09:44:57 mostad Exp $ */

#include <cstdlib>

#include "Node.h"
#include "special.h"

const int Node::MaxCutsetSize = 3; 

Node::Node()
{
   succ = this;
   pred = this;
   nNeighbours = 0;
   neighbour = 0;
   mark = 0;
}

Node::~Node()
{
   delete[] neighbour;
}

void Node::makeCutsets()
{
   addGraph();
   markWith0();
   findCutset();
}

Node* Node::getNext()
{
   return succ;
}

void Node::findCutset()
{
// Removed 2012-03-08
//   cout<<"Starting findCutset\n";
   printSet();

   int n = findNumberOfNonTemps();
   int i;
   for (i=1; i<=n; i++)
   {
      if (testCutset(i, n)) return;
      if (i==MaxCutsetSize) break;
   }
   deleteAllTemps();
   //If no cutsets are found:
   //We give up; cutsets larger than this is not worth having...
   //As the remaining nodes are marked with 0, they are automatically
   //not counted as cutsets.
}

int Node::testCutset(int i, int n) 
{
// Removed 2012-03-08
//   cout<<i<<' '<<n<<'\n';


   if (i==0) return testCutset();
   Node* p = getNextNonTemp();
   for (int j = 0; j <= n-i; j++)
   {
      p->mark = 1;
      if (p->testCutset(i-1, n-j-1)) return 1;
      p->mark = 0;
      p = p->getNextNonTemp();
   }
   return 0;
}

int Node::testCutset()
{
// Removed 2012-03-08
//   cout<<"Starting testCutset\n";

   Node* n;
   Node* m;
   Node* p; 
   //Er dette riktig aa gjoere alltid?
   if ((n = findUnmarked())==0) return 0;

   n->markGraph2();
   if ((m = findUnmarked())==0) 
   {
      removeMark2();
      return 0;
   }


// Removed 2012-03-08
//   cout<<"FoundCutset\n";
   printSet();



      
   //We have found a cutset
   //Remove as a cutset the set marked with 1
   //(we assume this node is one of them).
   removeSetMarkedAsThis();

   for (;;)
   {
      n->removeSetMarkedAsThis();
      if ((p = n->findNonTempNode())==0) 
	 deleteAllTemps();
      else
      {
	 n->markWith0();
	 replaceWithTempNode(p);
	 p->findCutset();
      }
      if ((n = m) == 0) {
	 //cout<<"Found cutset:\n";
	 //printSet();
	 establishCutset();
	 return 1;
      }
      n->markGraph2();
      m = findUnmarked();
   }
}

int Node::findNumberOfNonTemps()
{
   int counter = 0;
   Node* n = this;
   do {
      if (!n->isTemp()) counter++;
      n = n->succ;
   } while (n!=this);
   return counter;
}

int Node::numberOfElements()
{
   int counter = 0;
   Node* n = this;
   do {
      counter++;
      n = n->succ;
   } while (n!=this);
   return counter;
}
   
Node* Node::getNextNonTemp()
{
   Node* result = succ;
   do {
      if (!result->isTemp()) return result;
      result = result->succ;
   } while (result!=this);
   if (isTemp()) return 0;
   return this;
}

Node* Node::findNonTempNode()
{
   Node* n = this;
   do {
      if (!n->isTemp()) return n;
      n = n->succ;
   } while (n!=this);
   return 0;
}

Node* Node::findUnmarked()
{
   Node* n = this;
   do {
      if (n->mark==0) return n;
      n = n->succ;
   } while (n!=this);
   return 0;
}

void Node::removeSetMarkedAsThis()
{
   int n = numberOfElements();
   Node* p = succ;
   remove();
   Node* m;
   for (int i=1; i<n; i++)
      if (p->mark==mark) 
      {
	 m = p->succ;
	 add(p);
	 p = m;
      }
      else
	 p = p->succ;
}

void Node::markGraph2()
{
   mark = 2;
   for (int i=0; i<nNeighbours; i++)
      if (neighbour[i]->mark==0)
	 neighbour[i]->markGraph2();
}

void Node::removeMark2()
{
   Node* n = this;
   do {
      if (n->mark==2) n->mark = 0;
      n = n->succ;
   } while (n!=this);
}
   
void Node::markWith0()
{
   Node* n = this;
   do {
      n->mark = 0;
      n = n->succ;
   } while (n!=this);
}
   
void Node::markWith1()
{
   Node* n = this;
   do {
      n->mark = 1;
      n = n->succ;
   } while (n!=this);
}
   
void Node::add(Node* n)
{
   if (n->succ!=n) n->remove();
   n->succ    = succ;
   succ->pred = n;
   n->pred    = this;
   succ       = n;
}

void Node::addGraph()
{
   mark = 1;
   for (int i = 0; i<nNeighbours; i++)
      if (neighbour[i]->mark==0) 
      {
	 add(neighbour[i]);
	 neighbour[i]->addGraph();
      }
}

void Node::remove()
{
   succ->pred = pred; 
   pred->succ = succ;
   succ       = this;
   pred       = this;
}

void Node::deleteAllTemps()
{
   Node* n = this;
   Node* m;
   do 
      if (n->isTemp()) {
	 m = n->succ;
	 n->remove();
	 deleteTempNeighbour(n);
	 n = m;
      }
      else 
	 n = n->succ;
   while (n!=this);
}   

void Node::addNeighbour(Node* n)
{
   Node** newNeighbour = new Node*[nNeighbours+1];
   for (int i = 0; i<nNeighbours; i++)
      newNeighbour[i] = neighbour[i];
   delete[] neighbour;
   neighbour = newNeighbour;
   neighbour[nNeighbours++] = n;
}

void Node::deleteTempNeighbour(Node* p)
{
   Node* n = this;
   do {
      if (!n->isTemp()) n->removeAsNeighbour(p);
      n = n->succ;
   } while (n!=this);
   delete p;
}

void Node::removeAsNeighbour(Node* n)
{
   for (int i = 0; i<nNeighbours; i++)
      if (neighbour[i]==n) 
      {
	 neighbour[i] = neighbour[--nNeighbours];
	 return;
      }
}

void Node::replaceWithTempNode(Node* p)
{
   int n = p->numberOfElements();

   TempNode* result = new TempNode;
   result->neighbour = new Node*[n];
   Node* d = p; 
   do {
      if (setRemoveNeighbour(d)) 
      {
	 d->replaceNeighbours(this, result);
	 result->neighbour[result->nNeighbours++] = d;
      }
      d = d->succ;
   } while (d != p);
   p->add(result);
}

void Node::replaceNeighbours(Node* a, Node* b)
{
   Node* n = a; 
   do {
      if (hasNeighbour(n)) removeAsNeighbour(n);
      n = n->succ;
   } while (n!=a);
   //The routine should only be called when there are 
   //neighbours that are removed, so the line below is safe.
   neighbour[nNeighbours++] = b;
}

int Node::setRemoveNeighbour(Node* d)
{
   Node* n = this;
   int neighbourFound = 0;
   do 
   {
      if (n->hasNeighbour(d)) 
      {
	 neighbourFound = 1;
	 n->removeAsNeighbour(d);
      } 
      n = n->succ;
   } 
   while (n!=this);
   return neighbourFound; 
}

int Node::hasNeighbour(Node* d)
{
   for (int i=0; i<nNeighbours; i++)
      if (neighbour[i]==d) return 1;
   return 0;
}

void Node::printAll()
{
// Removed 2012-03-08
//   cout<<"******************** Node: "; print();
//   cout<<"mark =     "<<mark<<'\n';
//   for (int i=0; i<nNeighbours; i++)
//   {
//      cout<<"Neighbour: "; neighbour[i]->print();
//   }
//   cout<<"*****************************************\n";
}

void Node::printSet()
{
// Removed 2012-03-08
//   cout<<"**************************************************\n";
//   Node* n = this;
//   do {
//      n->printAll();
//      n = n->succ;
//   } while (n!=this);
//   cout<<"**************************************************\n";
}
 
void TempNode::findCutset() {
// Removed 2012-03-08
  //   cout<<"ERROR!!\n";
  //   exit(1);
}

void TempNode::print() {
// Removed 2012-03-08
  //   cout<<"TempNode\n";
}

void TempNode::establishCutset()
{
// Removed 2012-03-08
  //   cout<<"Internal error: establishCutset called for ";
  //   cout<<"an object which is not a pers-object.\n";
  //   exit(1);
}

