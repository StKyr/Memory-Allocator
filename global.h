#ifndef MY_MALLOC_GLOBAL_H
#define MY_MALLOC_GLOBAL_H
#include <cstdlib> /* for NULL */
#include <stdint.h> /* for uintptr_t */

/**
 * File contains definitions of constants used throughout the module.
 * It is included in every header file regardless of whether that file
 * needs it specifically. (for uniformity)
 * Change here if needed.
 */





typedef uintptr_t Address; /** Just for semantincs, so that not to work wih (void *) */
// uintptr_t is guaranteed to have the same size as void *.

#define nullptr NULL                  // nullptr is standardised in C++11.
#define ARRAY_SIZE 19997              /** Size of inner array in HashTable (19,997 is closest prime to 20,000). */
#define MALLOC_REQUEST_SIZE 1048576   /** How many bytes PagePool asks each time from malloc (1048576 = 1MB). */

#define _MUTE_PARAM_ 0               /** If set to 1, no potential error messages will be printed in stderr */

#endif //MY_MALLOC_GLOBAL_H
