#ifndef MY_MALLOC_ALLOCATOR_H
#define MY_MALLOC_ALLOCATOR_H


#include "sizeIndex.h"
#include "pages.h"
#include "pagePool.h"
#include "hashTable.h"

#include "global.h"



/**
 * The class that containss the business logic of the allocation (and de-allocation) process.
 * Its functions are responsible for performing granting and freeing space.
 * It uses the OOP's "Singleton" pattern to ensure that exactly one instance of that class is created and shared.
 */
class Allocator{

    static Allocator *instance; /** The actual instance to be shared */

    SizeIndex sizeIndex; /** The 8-position array of sizes pointing to lists of nodes pointing to pages */
    PagePool pagePool; /** The object responsible for malloc-ing actual space and returning pages one at a time */
    HashTable hashTable; /** A pretty fast structure to store objects for freeing */

    Allocator(); // private constructor to guarantee no instantiation

    static Address shiftToClosestMultipleOf4096(Address address);

public:

    /**
     * Get an instance of Allocator (same instance every time) (Also create the instance during first call)
     * @return The (same) static instance of Allocator.
     */
    static Allocator *getInstance();



    /**
     * The function responsible for allocating space for objects
     * @param cbytes The number of Bytes needed (size of object).
     * @return A NULL pointer or a pointer guaranteed to have space for that size.
     */
    void *allocateFor(size_t cbytes);


    /**
     * Notifies inner layers that the specific address can be used for storing again.
     * Prints error messages in case of double-freeing a pointer or passing a pointer not previously returned by `mymalloc` as argument.
     * @param ptr The address of the object to be freed.
     */
    void freePtr(void *ptr);

    /**
     * Retrieves the beginning address of the object a given pointer is pointing to.
     * @param ptr A pointer pointing somewhere in the allocated space of an object
     * @return The beginning address of the corresponding object or `NULL` if `ptr` is not a valid `mymalloc` pointer.
     */
    void *relocatePtr(void *ptr);


    /**
     * Calculate the size (in BYTEs) of an object
     * @param ptr A pointer pointing to the object
     * @return The size of the pointed object or 0 if `ptr` is not a valid `mymalloc` pointer.
     */
    size_t findSize(void *ptr);
};

#endif //MY_MALLOC_ALLOCATOR_H_H
