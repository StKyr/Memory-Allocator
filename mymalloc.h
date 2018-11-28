#ifndef MY_MALLOC_MYMALLOC_H
#define MY_MALLOC_MYMALLOC_H

#include <stdlib.h> /* for size_t */



/**
 * Allocate space.
 * @param cbytes The number of required bytes
 */
#ifdef __cplusplus
extern "C"
#endif
void *mymalloc(size_t cbytes);

/**
 * Free the given pointer (that was previously allocated by `mymalloc`)
 * @param ptr The pointer to be freed
 */
#ifdef __cplusplus
extern "C"
#endif
void myfree(void *ptr);



/**
 * Find the beginning address of an object (that was previously allocated with `mymalloc`)
 * @param ptr A pointer pointing somewhere inside the allocated space of the object
 * @return A pointer pointing to the beginning of the object or a NULL pointer
 */
#ifdef __cplusplus
extern "C"
#endif
void *myrelocate(void *ptr);


/**
 * Retrieve the size of an object (that was previously allocated with `mymalloc`),
 * as long as the size of the object is no more than 4096 Bytes.
 * @param ptr A pointer pointing in the beginning of the object
 * @return The size of the object
 */
#ifdef __cplusplus
extern "C"
#endif
size_t mysize(void *ptr);


#endif //MY_MALLOC_MYMALLOC_H
