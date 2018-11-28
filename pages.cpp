
#include "pages.h"
#include <cstdlib>

PageListNode *PageList::searchForSpace() {

    if (this->startNode == nullptr) return nullptr; // no nodes inserted yet

    PageListNode *current = this->startNode;
    while (! current->isLast()){  // traverse the list
        if (current->hasSpace()){
            return current;
        }
        current = current->getNext();
    }

    if (current->hasSpace()) { // also check the last one
        return current;
    }else{
        return nullptr;
    }
}

PageListNode *PageList::addPageNode(Address startAddr, int pageLength) {

    // a normal add-to-front function

    PageListNode *copyNode = this->startNode;
    PageListNode *newNode = new PageListNode(startAddr, pageLength, this);

    newNode->setNext(copyNode);
    if (copyNode) copyNode->setPrev(newNode);

    this->startNode = newNode;
    return newNode;
}

PageList::~PageList() { //--DOESNT MATTER - NEVER USED

    if (this->startNode == nullptr) return;


    PageListNode *current = this->startNode;

    // go to last
    while (! current->isLast()){
        current = current->getNext();
    }

    if (current == this->startNode){
        delete current;
        return;
    }

    current = current->getPrev();

    // remove in backwards
    while (! current->isFirst()){
        current = current->getPrev();
        delete current->getNext();
    }
    delete current;
}

void PageList::movePageNodeToBeginning(PageListNode *node) {

    // link your left node with your right
    if (! node->isFirst()) node->getPrev()->setNext( node->getNext() );
    if (! node->isLast() ) node->getNext()->setPrev( node->getPrev() );

    // link node with previously first node
    node->setNext( this->startNode );
    if (this->startNode != nullptr) this->startNode->setPrev( node );

    // set node as starting node
    this->startNode = node;

}


PageListNode::PageListNode(Address startAddr, int pageLength, PageList *pageListBelonging){
    this->prev = nullptr;
    this->next = nullptr;

    this->startingAddr = startAddr;
    this->endingAddr = startAddr + pageLength;

    this->nObjects = pageLength / pageListBelonging->getObjectsSize();

    this->myPageList = pageListBelonging;

}

bool PageListNode::belongsToMe(Address addr) {

    return (addr >= this->startingAddr && addr < this->endingAddr);
}

bool PageListNode::markFree(Address addr) {

    // which object (ie 1st, 2nd, ...) of the node this pointer actually is
    int index = (int) (addr - this->startingAddr) / this->getPageListBelonging()->getObjectsSize();

    if (this->bitVector.isFree(index)){
        return false; // double free error
    }

    this->bitVector.markFree(index);
    return true;
}

bool PageListNode::hasSpace() {
    for (int i=0; i<this->nObjects; i++){
        if (this->bitVector.isFree(i)){
            return true;
        }
    }
    return false;
}

Address PageListNode::occupySpace() {
    for (int i=0; i<this->nObjects; i++){

        if (this->bitVector.isFree(i)){
            this->bitVector.markOccupied(i);
            return (Address) (startingAddr + i*this->getPageListBelonging()->getObjectsSize());
        }
    }

#pragma GCC diagnostic ignored "-Wconversion-null"
    return NULL;
}

bool PageListNode::isCompletelyFree() {
    for (int i=0; i<this->nObjects; i++){

        if (this->bitVector.isOccupied(i)){

            return false;
        }
    }
    return true;
}


bool PageListNode::isValidObjectAddress(Address addr) {

    int offset = (int) (addr - this->startingAddr);

    return (offset < 4096) && (offset % this->getPageListBelonging()->getObjectsSize() == 0);
}


int PageListNode::findpositionOfObjectInPage(Address addr){

    int offset = (int) (addr - this->startingAddr);
    return offset / this->getPageListBelonging()->getObjectsSize();

}





BitVector::BitVector() {
    int i;
    for (i=0; i<16; i++){
        this->vector[i] = 0b00000000;
    }
}



bool BitVector::isFree(int index) {
    int cellIndex = index/8;
    _8bit cell = this->vector[cellIndex];

    int exactPosition = index % 8;

    _8bit mask = 1 << exactPosition; // mask: 00001000 (if exactPosition = 3)

    _8bit result = cell & mask; // xxxxYxxx AND 00001000 = 0000Y000

    return result == 0b00000000;
}

void BitVector::markFree(int index) {
    int cellIndex = index/8;
    _8bit cell = this->vector[cellIndex];
    int exactPosition = index % 8;

    _8bit mask = 1 << exactPosition;        // mask: (ie) 00100000
    mask = 0b11111111 ^ mask;               // 11111111 XOR 00100000 = 11011111

    this->vector[cellIndex] = cell & mask;  // xxYxxxxx AND 11011111 = xx0xxxxx
}

void BitVector::markOccupied(int index) {
    int cellIndex = index/8;
    _8bit cell = this->vector[cellIndex];
    int exactPosition = index % 8;

    _8bit mask = 1 << exactPosition;   // mask: (ie) 00010000

    this->vector[cellIndex] = cell | mask; // xxxYxxxx OR 00010000 = xxx1xxxx
}
