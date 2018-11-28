#ifndef MY_MALLOC_PAGEPOOL_H
#define MY_MALLOC_PAGEPOOL_H

#include "global.h" /* for MY_MALLOC_REQUEST_SIZE constant and for Address type */

/**
 * A class responsible for getting a big space from system's malloc
 * and returning page "splices" to other modules.
 */
class PagePool {

     Address startAddr; /** beginning address of allocated space */
     Address endAddr;  /** ending address of allocated space */
     Address currAddr; /** index displaying the next page start for allocation */


    /**
     * Check if all pages are allocated.
     * @return `True` if there are currently no more pages to give, `False` otherwise.
     */
    bool isEmpty();

    /**
     * Call system's malloc to get more space.
     */
    void askForMoreSpace();

    /**
     * Truncate the given space making it start at a multiple of 4096
     * @param space
     */
    void truncate(Address space);


public:
    PagePool();

    /**
     * Get a new page (space of size 4096 Bytes) for allocating
     * @return An address (pointer) to the beginning of the page.
     */
    Address requestNewPage();

};


#endif //MY_MALLOC_PAGEPOOL_H
