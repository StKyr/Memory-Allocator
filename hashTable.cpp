#include "hashTable.h"

int HashTable::defaultHashFunction(uintptr_t key){
    return (unsigned)( key % ARRAY_SIZE );
}


void HashTable::store(uintptr_t key, PageListNode *value) {

    int index = hashFunction(key);
    HashEntry *hashEntry = new HashEntry(key, value);


    if (array[index] == nullptr){ // no conflicts
        array[index] = hashEntry;

    }else{ // conflicts -> insert to front and link with previous first

        HashEntry *copy = array[index];

        hashEntry->next = copy;
        copy->prev = hashEntry;

        array[index] = hashEntry;
    }

}

PageListNode *HashTable::get(uintptr_t key) {
    long index = hashFunction(key);

    HashEntry *hashEntry = array[index];

    if (hashEntry == nullptr) return nullptr; // not stored

    while (hashEntry != nullptr){ // traverse the list of conflicts to find it

        if (hashEntry->key == key){ // bingo!
            return hashEntry->node;
        }

        hashEntry = hashEntry->next; // check next node
    }

    return nullptr; // tough luck
}

HashTable::HashTable() {

    for (int i=0; i<ARRAY_SIZE; i++){ // initialize to NULL for easier check for conflicts
        this->array[i] = nullptr;
    }

    this->hashFunction = defaultHashFunction; // use that hashFunction
}

HashTable::HashTable(int (*customHashFunction)(uintptr_t)) { // -- NEVER USED --
    for (int i=0; i<ARRAY_SIZE; i++){
        this->array[i] = nullptr;
    }

    this->hashFunction = customHashFunction; // use a custom hash function
}
