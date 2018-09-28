/* $Id: % */

#ifndef __NODE_H_
#define __NODE_H_

///Class for finding cutsets.
/** To use this class, a number of nodes should be made, 
  and then they should be connected with neighbour relations,
  using the function addNeighbour.
  When makeCutsets is called, we assume:
  1. The neighbourhood relation is symmetric.
  2. The graph is connected.
  The makeCutsets will result in calls to the pure virtual
  function establishCutset for Node-objects that are part of a cutset.
  The other Nodes in the cutset may then be obtained by calling getNext, 
  whilch will return Nodes in a circular list. 
  The pure virtual function print, used for test purposes must also 
  be defined in inheriting functions.
  Note that after makeCutsets is called once, the status of the system
  is not proper for further calls.
  */
class Node {
      
   public:

      ///Top public function:
      void makeCutsets();

      //Will return the next member of the cutset, when one has been made.
      Node* getNext();

      /// Add a neighbour.
      void addNeighbour(Node* n);

   protected:

      Node();

      virtual ~Node();

   private: //Pure virtual functions that must be defined in inheriting objects:

      ///This function is called when a cutset is found.
      /** It transfers the information to the other levels of the object */
      virtual void establishCutset() = 0;

      ///Print just the name of the person, or TempNode
      virtual void print() = 0;

   private: //Data in each Node:

      Node* succ;
      Node* pred;

   protected:

      int nNeighbours;
      Node** neighbour;

   private:
      
      int mark;

      static const int MaxCutsetSize;

   private: //Virtual procedures, for Node and TempNode:

      virtual int isTemp() { return 0; }

      /** This is the entry method for finding the cutsets.
	One assumes that this node is not a TempNode, and
	that it is in as set, which is also a valid graph. 
	Some of the Nodes in this set may be TempNodes. 
	Such nodes are never directly neighbours. 
	At the end of this method, Nodes that are suggested
	as cutsets are collected in their own set, and marked
	with 1. Nodes marked with 0 are not suggested to 
	be in a cutset. All TempNodes are removed, 
	and Nodes have neighbours only in their own cutset. */
      virtual void findCutset();

      //Virtual procedure that must be defined in inherite objects:

   private: //Truly private methods:

      /** We assume some elements have been marked with 1
	(if there are any, this Node is marked).
	In the set segment of length n starting with the
	next NonTemp node, we shall mark i more. 
	We return 1 if our search is successful, 0 otherwise */
      int testCutset(int i, int n);

      /// Find out if the nodes currently marked 1 is a cutset.
      /** All other nodes in this graph, which is the same as
	this set, are marked 0. */
      int testCutset();
      
      ///Find the number of non Temp objects in this set
      int findNumberOfNonTemps();

      ///Find the number of elements in this set.
      int numberOfElements();

      ///Get the next element that is not a TempNode
      Node* getNextNonTemp();

      ///Find a nonTemp Node in this set.
      Node* findNonTempNode();
      
      /// Find one unmarked Node in this set:
      Node* findUnmarked();

      /** Remove from the surrounding set all those Nodes 
	marked with the same value as this one. */
      void removeSetMarkedAsThis();

      /** Mark recursively, by going through the unmarked graph
	all Nodes with 2 */
      void markGraph2();

      ///All Nodes in this set marked with 2 is marked with 0.
      void removeMark2();

      ///Mark all set elements with 0
      void markWith0();

      ///Mark all set elements with 1
      void markWith1();

      /**Add the given node p to this set.
	Remove p from its previous set. */
      void add(Node* p);

      /** Add all nodes connected to this one via the graph
	to this set. Note: All are marked with 1, and we
	assume at the start that all are marked with 0.*/
      void addGraph();
      
      ///Remove this Node from its set.
      void remove();

      /**Delete the Temp objects in this set, removing neighbour
	relations as neccessary */
      void deleteAllTemps();

      //The given TempNode is a neighbour to some elements
      //of this set. Remove the connection, and delete p
      //at the end.
      void deleteTempNeighbour(Node* p);

      ///Remove n as a neighbour to this one. 
      void removeAsNeighbour(Node* n);

      /// Make a new temp node of the set this node
      ///is contained in, with relations to the set
      ///of the given node. Add to set of given node. 
      ///The neighbours of this set to the set of n is removed.
      void replaceWithTempNode(Node* p);

      /** This node will have one or more neighbours in the set of a.
	All these neighbours should be replaced with the single
	neighbour b */
      void replaceNeighbours(Node* a, Node* b);

      /**Return 1 if this set has d as a neighbour, 0 otherwise.
	All elements in this set remove d as a neighbour, 
	when necessary */
      int setRemoveNeighbour(Node* d);

      ///Return 1 if this has d as a neighbour, 0 if not.
      int hasNeighbour(Node* d);

      ///Print the name, mark, and all neighbours:
      void printAll();

      ///Print the contents of the whole set:
      void printSet();

};


class TempNode : public Node {

      int isTemp() { return 1; }

      void findCutset();

      void print();

      void establishCutset();

};


#endif
