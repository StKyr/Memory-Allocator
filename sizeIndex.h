#ifndef MY_MALLOC_SIZEINDEX_H
#define MY_MALLOC_SIZEINDEX_H


#include "pages.h"
#include "global.h"


/**
 * A class containing an array of lists for handling objects of specific size
 */
class SizeIndex {

    PageList* indexArray[8];   /** The array of lists */
    /*
     * indexArray[0] : 32
     * indexArray[1] : 64
     * indexArray[2] : 128
     * indexArray[3] : 256
     * indexArray[4] : 512
     * indexArray[5] : 1024
     * indexArray[6] : 2048
     * indexArray[7] : 4096
     */

    /**
     * Find the appropriate array index for a given size
     * @param size the size of the object
     * @return An index from 0 to 7 for the appropriate cell
     */
    int indexOf(int size);


public:
    SizeIndex();

    PageList* getPageList(size_t size);



};


#endif //MY_MALLOC_SIZEINDEX_H
