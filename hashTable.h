#ifndef MY_MALLOC_HASHTABLE_H
#define MY_MALLOC_HASHTABLE_H

#include "pages.h"
#include "global.h"  /* for ARRAY_SIZE constant */

class HashEntry;


/**
 * A hash table for storing, during allocating space,
 * data (namely a pointer to the corresponding page-node)
 * required for de-allocating later on.
 */
class HashTable {


    HashEntry* array[ARRAY_SIZE]; /** The inner array for data to be stored **/

    int (*hashFunction)(uintptr_t key); /** the hash function to be used **/

public:
    HashTable();

    /**
     * A constructor allowing a user-defined hash function to be passed as a parameter.
     * @param customHashFunction The custom function to be used for hashing.
     */
    explicit HashTable(int (*customHashFunction)(uintptr_t key));


    /**
     * Store info of a page-node in the hash table
     * @param key The key associated with the data
     * @param value The value to be stored
     */
    void store(uintptr_t key, PageListNode *value);

    /**
     * Retrieve the data associated with the given key from the hash table
     * @param key The key to be passed to te hash function
     * @return The corresponding info stored in the table
     */
    PageListNode* get(uintptr_t key);

    /**
     * The hash function to be used by default
     * @param key The key to hash
     * @return A corresponding index of the array
     */
    static int defaultHashFunction(uintptr_t key);
};


/**
 * A packaging structure to store data inside the HashTable's array.
 * It is also a list-node (for conflict resolutions).
 */
class HashEntry{

public:
    HashEntry(uintptr_t key, PageListNode*node):prev(nullptr), next(nullptr), node(node), key(key){}

    // public members are ok. Defining setters and getters is actually equivalent to public members
    // and this class is only supposed to be used by HashTable.

    HashEntry *prev, *next; /** pointers to previous and next info-nodes created in conflicts */
    uintptr_t key;          /** The associated key to be stored as well */
    PageListNode *node;     /** The actual info to store */

};


#endif //MY_MALLOC_HASHTABLE_H
