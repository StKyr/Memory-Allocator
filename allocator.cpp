#include <cstdio>   /* for fprintf */
#include "allocator.h"
#include "mymalloc.h"

// initialization of Allocator (static) instance (Singleton design pattern is used)
Allocator *  Allocator::instance = 0;


extern "C" void *mymalloc(size_t cbytes){

    Allocator *allocator = Allocator::getInstance();
    return allocator->allocateFor(cbytes);
}



extern "C" void myfree(void *ptr){
    Allocator *allocator = Allocator::getInstance();
    allocator->freePtr(ptr);
}




extern "C" void *myrelocate(void *ptr){
    Allocator *allocator = Allocator::getInstance();
    return allocator->relocatePtr(ptr);
}



extern "C" size_t mysize(void *ptr){
    Allocator *allocator = Allocator::getInstance();
    return allocator->findSize(ptr);
}



 Address Allocator::shiftToClosestMultipleOf4096(Address address){
    return address - (address % 4096);
}


 void * Allocator::allocateFor(size_t cbytes){

    if (cbytes > 4096 || cbytes <= 0){ // delegate memory allocation to system's malloc
        return malloc(cbytes);

    }else{ // it is our job

        PageList* pageList = sizeIndex.getPageList(cbytes); // get the list of nodes of pages for that class of sizes


        PageListNode* pageListNode = pageList->searchForSpace(); // traverse to find a node with free space

        if (pageListNode == nullptr){ // no space found, create a new node

            Address address = pagePool.requestNewPage();
            if (address == 0){ // system's malloc failed
                return NULL;
            }

            pageListNode = pageList->addPageNode(address, 4096);

            // store new node into hashTable
            Address key = shiftToClosestMultipleOf4096(address);
            hashTable.store(key, pageListNode);

        }

        Address address = pageListNode->occupySpace(); // this is the actual allocation


        return (void*)address;
    }

}

 void Allocator::freePtr(void *ptr) {

     Address key = shiftToClosestMultipleOf4096((Address)ptr);
     PageListNode *pageListNode = hashTable.get(key);

     if (pageListNode == nullptr){ // not our object, call original free
         free(ptr);
         return;
     }

     if (! pageListNode->isValidObjectAddress((Address) ptr)){
         if (! _MUTE_PARAM_) fprintf(stderr, "***mymalloc: Invalid pointer %p at `myfree()'.***\n", ptr);
         return;
     }


     bool success = pageListNode->markFree((Address)ptr); // actual freeing (attempting)

     if (! success){
         if (! _MUTE_PARAM_) fprintf(stderr, "***mymalloc: Double free of pointer %p at `myfree()'.***\n", ptr);
         return;
     }

     // check if node is completely free and if so, move it to the beginning of the list for easy access
     PageList *pageList = pageListNode->getPageListBelonging();
     if (pageListNode->isCompletelyFree()) {
         pageList->movePageNodeToBeginning(pageListNode);
     }
}



Allocator::Allocator() {
    this->pagePool = PagePool();
    this->hashTable = HashTable();
    this->sizeIndex = SizeIndex();
}

Allocator *Allocator::getInstance() {
    // the implemantation of the singleton pattern. Always returns the same instance
    if (! instance){
        instance = new Allocator;
    }

    return instance;
}



void *Allocator::relocatePtr(void *ptr){

    Address key = shiftToClosestMultipleOf4096((Address)ptr);

    PageListNode *pageListNode = hashTable.get(key);
    if (pageListNode == nullptr){ // not our object
        return nullptr;
    }
    int index = pageListNode->findpositionOfObjectInPage((Address) ptr);
    return (void *)(pageListNode->getStartOfPage() + index*pageListNode->getPageListBelonging()->getObjectsSize());
}

size_t Allocator::findSize(void *ptr) {

    Address key = shiftToClosestMultipleOf4096((Address)ptr);
    PageListNode *pageListNode = hashTable.get(key);
    if (pageListNode == nullptr){ // not our object
        return 0;
    }

    return (size_t)pageListNode->getPageListBelonging()->getObjectsSize();

}
