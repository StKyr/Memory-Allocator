#ifndef MY_MALLOC_PAGES_H
#define MY_MALLOC_PAGES_H

#include <sys/types.h>
#include "global.h"



class PageListNode;


/**
 *  A list of nodes storing objects of the same size in pages.
 *  It mainly exists for namespacing (of list functions)
 */
class PageList{

    PageListNode *startNode;  /** The starting node of the list */
    int objectsSize;         /** The size of objects nodes of this list will be allocating */

public:

    PageList(int objectsSize):objectsSize(objectsSize), startNode(nullptr){}
    ~PageList();

    /**
     * Traverse the nodes of the list untill a node with free space for allocating is found.
     * @return A pointer to that node or a NULL pointer if none is found.
     */
    PageListNode *searchForSpace();

    /**
     * Create and add a new node at the beginning of the list.
     *
     * @param startAddr A pointer of the page of the node / The starting address of the page
     * @param objectSize The size of the objects this node will be allocating for.
     * @param pageLength The length of the page
     * @return A pointer to the node created.
     */
    PageListNode *addPageNode(Address startAddr, int pageLength);

    /**
     * Return the size of the objects this list is in charge of.
     * @return The object size
     */
    int getObjectsSize() { return this->objectsSize;}

    /**
     * Repositions a node to the beginning of the list for quick access in next search
     * @param node The node to be repositioned
     */
    void movePageNodeToBeginning(PageListNode* node);

};




typedef u_int8_t _8bit;  /** A data type consisted of exactly 8 bits */


class BitVector{

    _8bit vector[16];
    /*
     * 1: this slot is occupied
     * 0: this slot is free
     */

public:
    BitVector();

    void markOccupied(int index);
    void markFree(int index);

    bool isFree(int index);
    bool isOccupied(int index){ return !isFree(index);}
};






/**
 * A node responsible for allocating space for objects in a specific page.
 */
class PageListNode{

    int nObjects;             /** how many objects fit in a single page */
    BitVector bitVector;      /** An index struct for saving which slots are occupied */
    Address startingAddr;     /** starting address of the page */
    Address endingAddr;       /** ending address of the page */

    PageListNode *prev, *next;  /** previous and next node in the list */

    PageList *myPageList;     /** A reference to the page list belonging */

public:
    PageListNode(Address startAddr, int pageLength, PageList *pageListBelonging);

    /**
     * Check if a given address belongs in the page of this node
     * @param addr The address to be checked
     * @return `True` if the address belongs in this page, `False` otherwise
     */
    bool belongsToMe(Address addr);

    /**
     * Check if a given address is valid (is a pointer at the beginning of a stored object and not in between)
     * @param addr The address to be checked
     * @return `True` if address is valid, `False` otherwise
     */
    bool isValidObjectAddress(Address addr);

    /**
     * Mark the slot of the specific address as free space.
     *
     * @param addr The address of the freed object
     * @return `False` if the specific slot was previously empty, `True` otherwise.
     */
    bool markFree(Address addr);

    /**
     * Check if there is free space in the page.
     * @return `True` if there is free space, `False` otherwise.
     */
    bool hasSpace();

    /**
     * Marks the first free slot in the page as occupied and returns its address.
     * @return The address of the beginning of the occupied space.
     */
    Address occupySpace();

    /**
     * Checks if the whole page is free (no slots are occupied).
     * @return `True` if there are no slots occupied, `False` otherwise.
     */
    bool isCompletelyFree();


    int findpositionOfObjectInPage(Address addr);


    PageList *getPageListBelonging() {return this->myPageList;}
    Address getStartOfPage() {return this->startingAddr;}
    bool isFirst() {return this->prev == nullptr;}
    bool isLast() { return this->next == nullptr;}
    PageListNode *getNext() {return this->next;}
    PageListNode *getPrev() {return this->prev;}
    void setPrev(PageListNode *node) {this->prev = node;}
    void setNext(PageListNode *node) {this->next = node;}


};




#endif //MY_MALLOC_PAGES_H
