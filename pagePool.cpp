#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "pagePool.h"

bool PagePool::isEmpty() {
    return currAddr == endAddr;
}

void PagePool::askForMoreSpace() {
    Address rawSpace = (Address)malloc(MALLOC_REQUEST_SIZE);


#pragma GCC diagnostic ignored "-Wpointer-arith"
    if (rawSpace == NULL){
        startAddr = endAddr = currAddr = 0;
        return;
    }

    truncate(rawSpace); // make the start of the object a multiple of 4096
}

void PagePool::truncate(Address space) {

    int offset = space % 4096;

    if (offset) {

        startAddr = space + 4096 - offset;
        endAddr = startAddr + 255 * 4096;

    }else{
        startAddr = space;
        endAddr = space + MALLOC_REQUEST_SIZE;
    }

    currAddr = startAddr;
}

Address PagePool::requestNewPage() {
    if (this->isEmpty()) {

        this->askForMoreSpace();

        if (currAddr == 0){
            return 0;
        }
    }

    Address beginningOfPage = currAddr;
    currAddr += 4096;

    return beginningOfPage;
}

PagePool::PagePool() {
    this->startAddr = this->endAddr = this->currAddr = 0;

}