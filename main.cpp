#include <iostream>
#include <cstdio>
#include <assert.h>
#include <unistd.h>
#include "mymalloc.h"

using namespace std;

void Case_dummy();
void Case_reallocationAfterFreeing();
void Case_allocationOfArrays();
void Case_multipleSizes();
void Case_multiplePages();
void Case_reallyLargeObject();

void Case_relocatingObject();
void Case_findingSizeOfObjects();

void Case_Error_doubleFreeingPointer();
void Case_Error_writingOverAFreedObject();
void Case_Error_freeingAnInBetweenPointer();


// run one or more specific cases
int parseCommandLineParams(int argc, char*argv[]){
    int i;

    for (int i=1; i<argc; i++){

        std::string _case = argv[i];

        if (_case == "dummy") Case_dummy();
        else if (_case == "reallocationAfterFreeing") Case_reallocationAfterFreeing();
        else if (_case == "allocationOfArrays") Case_allocationOfArrays();
        else if (_case == "multipleSizes") Case_multipleSizes();
        else if (_case == "multiplePages") Case_multiplePages();
        else if (_case == "reallyLargeObject") Case_reallyLargeObject();
        else if (_case == "relocatingObject") Case_relocatingObject();
        else if (_case == "findingSizeOfObjects") Case_findingSizeOfObjects();
        else if (_case == "doubleFreeingPointer") Case_Error_doubleFreeingPointer();
        else if (_case == "writingOverAFreedObject") Case_Error_writingOverAFreedObject();
        else if (_case == "freeingAnInBetweenPointer") Case_Error_freeingAnInBetweenPointer();
        else{
            cout << "Error: Invalid case: "<<_case<<endl;
            exit(2);
        }
    }

    return argc >= 2;
}


int main(int argc, char *argv[]){


    if (! parseCommandLineParams(argc, argv)) {

        // run all cases

        Case_dummy();
        Case_reallocationAfterFreeing();
        Case_allocationOfArrays();
        Case_multipleSizes();
        Case_multiplePages();
        Case_reallyLargeObject();

        Case_relocatingObject();
        Case_findingSizeOfObjects();

        Case_Error_doubleFreeingPointer();
        Case_Error_writingOverAFreedObject();
        Case_Error_freeingAnInBetweenPointer();
    }

    return 0;
}



typedef struct {
    char x;
    char y;
}Tiny;


typedef struct {
    int arr[50];
}Medium;


typedef struct {
    int arr[200];
}Big;


void Case_dummy(){
    cout<<">>> Running Case: Dummy" << endl;

    Tiny *p = (Tiny *)mymalloc(sizeof(Tiny));
    cout <<"-Allocated space for object at address "<<p<<"."<<endl;

    p->x = 10;
    printf("-Set value %d in member x of object.\n", p->x);

    p->y = 20;
    printf("-Set value %d in member y of object.\n", p->y);


    myfree(p);
    cout <<"-Freed object."<<endl;
    cout<<endl;
}


void Case_reallocationAfterFreeing(){
    cout<<">>> Running Case: Reallocation after freeing" << endl;

    Tiny *p1 = (Tiny *)mymalloc(sizeof(Tiny));
    cout <<"-Allocated space for object at address "<<p1<<"."<<endl;

    myfree(p1);
    cout << "-Freed object."<<endl;

    Tiny *p2 = (Tiny *)mymalloc(sizeof(Tiny));
    cout << "-Allocated space for another object of same type. Address is "<<p2<<endl;

    assert(p1 == p2);
    cout << " =>which is the same as the first address."<<endl;

    cout<<endl;
}

void Case_allocationOfArrays(){
    cout<<">>> Running Case: Allocation Of Arrays" << endl;


    Tiny *arrayOfTinies = (Tiny *)mymalloc(10*sizeof(Tiny));
    cout <<"-Allocated space for an array of 10 objects." << endl;

    for (int i=0; i<10; i++){
        Tiny x;
        arrayOfTinies[i] = x;
    }

    cout <<"-Inserted (static) objects into array." <<endl;
    myfree(arrayOfTinies);

    Tiny *arrayOfPointers[10];
    for (int i=0; i<10; i++){
        Tiny *x = (Tiny *)mymalloc(sizeof(Tiny));
        arrayOfPointers[i] = x;
        cout <<"-Allocated space for object "<<i<<" and inserted it into (static) array."<<endl;
    }

    for (int i=0; i<10; i++){
        myfree(arrayOfPointers[i]);
    }

    cout<<endl;
}
void Case_multipleSizes(){
    cout<<">>> Running Case: Multiple sizes" << endl;

    Tiny *tiny = (Tiny *)mymalloc(sizeof(Tiny));
    cout << "-Allocated space for a tiny object at "<<tiny<<"."<<endl;

    Medium *medium = (Medium *)mymalloc(sizeof(Medium));
    cout << "-Allocated space for a medium-sized object at "<<medium<<"."<<endl;

    Big *big = (Big *)mymalloc(sizeof(Big));
    cout << "-Allocated space for a Big object at "<<big<<"."<<endl;

    cout << "=> They are all in different pages." <<endl;


    myfree(tiny);
    myfree(big);
    myfree(medium);

    cout<<endl;
}


void Case_multiplePages(){
    cout<<">>> Running Case: Multiple pages" << endl;


    Tiny *p5 = (Tiny *)mymalloc(2048); // way too much space
    Tiny *p6 = (Tiny *)mymalloc(2048);
    cout << "-Allocated two objects, p5("<<p5<<") and p6("<<p6<<"), that take up one page."<<endl;

    Tiny *p3 = (Tiny *)mymalloc(2048);
    Tiny *p4 = (Tiny *)mymalloc(2048);
    cout << "-Allocated two objects, p3("<<p3<<") and p4("<<p4<<"), that take up a second page."<<endl;

    Tiny *p1 = (Tiny *)mymalloc(2048);
    Tiny *p2 = (Tiny *)mymalloc(2048);
    cout << "-Allocated two objects, p1("<<p1<<") and p2("<<p2<<"), that take up a third page."<<endl;

    cout << "+Note: p1, p2 are on the first node, p3, p4 are on the second and p5, p6 on the third,"<<endl;
    cout << "+      as each new node is inserted at the beginning of the list."<<endl;


    myfree(p3);
    myfree(p2);
    cout <<"-Freed p2 and p3."<<endl;

    Tiny *p7 = (Tiny *)mymalloc(2048);
    assert(p7 == p2);
    cout << "-Allocated space for p7("<<p7<<") which is the same address as p2 (and not p3)."<<endl;

    myfree(p4);
    cout <<"-Freed p4 (now second node is empty)."<<endl;
    Tiny *p8 = (Tiny *)mymalloc(2048);
    assert(p8 == p3);
    cout << "-Allocated space for p8("<<p8<<") which is the same address as p3 (since node of p3,p4 moved to the beginning)."<<endl;


    cout<<endl;
}
void Case_reallyLargeObject(){
    cout<<">>> Running Case: Really large object" << endl;

    void *x = mymalloc(32);
    void *y = mymalloc(4096);

    cout <<"-Allocated two objects of sizes 32 and 4096 at addresses "<<x<<" and "<<y<<"."<<endl;

    void *z = mymalloc(100000);
    cout <<"-Allocated a very large object at address "<<z<<" (internally handled by `malloc()`)."<<endl;

    cout <<"=>Addresses differ a lot."<<endl;

    myfree(x);
    myfree(y);
    myfree(z);

    cout <<endl;

}

void Case_relocatingObject(){
    cout<<">>> Running Case: Relocating object" << endl;

    void *obj = mymalloc(500);
    cout <<"-Allocated space for object at "<<obj<<"."<<endl;

#pragma GCC diagostic ignored "-Wpointer-arithm"
    void *offset = obj + 10;
    cout <<"-Moved pointer 10 addresses. (at "<<offset<<")"<<endl;

    void *relocation = myrelocate(offset);
    cout <<"-Relocated beginning of object at "<<relocation<<"."<<endl;

    assert(obj == relocation);
    cout <<"=>Which is the original address of the object."<<endl;

    cout <<endl;
}


void Case_findingSizeOfObjects(){
    cout<<">>> Running Case: Finding size of objects" << endl;

    Tiny *tiny = (Tiny *)mymalloc(sizeof(Tiny));
    cout << "-Allocated space for a tiny object at "<<tiny<<"."<<endl;

    Medium *medium = (Medium *)mymalloc(sizeof(Medium));
    cout << "-Allocated space for a medium-sized object at "<<medium<<"."<<endl;

    Big *big = (Big *)mymalloc(sizeof(Big));
    cout << "-Allocated space for a big object at "<<big<<"."<<endl;

    cout << "-Tiny object has size "<<mysize(tiny)<<"."<<endl;
    cout << "-Medium object has size "<<mysize(medium)<<"."<<endl;
    cout << "-Big object has size "<<mysize(big)<<"."<<endl;

    assert(mysize(tiny)>=sizeof(*tiny));
    assert(mysize(medium)>=sizeof(*medium));
    assert(mysize(big)>=sizeof(*big));

    cout<<endl;
}

void Case_Error_doubleFreeingPointer(){
    cout<<">>> Running Case: Double freeing pointer" << endl;

    Tiny *tiny = (Tiny *)mymalloc(sizeof(tiny));
    cout << "-Allocated space for object at "<<tiny<<"."<<endl;

    myfree(tiny);
    cout << "-Freed object."<<endl;

    cout <<"-Attempting to free it again. (program pausing for printing synchronization)"<<endl;

    sleep(2);
    myfree(tiny);

    cout <<endl;
}
void Case_Error_writingOverAFreedObject(){
    cout<<">>> Running Case: Writing over a freed object" << endl;

    Tiny *tiny = (Tiny *)mymalloc(sizeof(tiny));
    cout << "-Allocated space for object at "<<tiny<<"."<<endl;

    tiny->x = 10;
    tiny->y = 20;
    printf("-Set x and y properties to 10 and 20.\n");fflush(stdout);

    myfree(tiny);
    cout << "-Freed object."<<endl;

    Tiny *tiny2 = (Tiny *)mymalloc(sizeof(tiny));
    cout << "-Allocated space for another object at "<<tiny<<" (same address)."<<endl;

    tiny2->x = 100;
    tiny2->y = 200;
    printf("-Set x and y properties to 100 and 200.\n");fflush(stdout);


    tiny->x = 0;
    cout<<"-Set x property of (now freed) first object to 0."<<endl;

    assert(tiny2->x == 0);
    printf("-Second object's x value is %d.\n", tiny2->x); fflush(stdout);

    myfree(tiny2);
    cout <<endl;
}


void Case_Error_freeingAnInBetweenPointer(){
    cout<<">>> Running Case: Freeing an in-between pointer" << endl;

    void *obj = mymalloc(500);
    cout <<"-Allocated space for object at "<<obj<<"."<<endl;

#pragma GCC diagostic ignored "-Wpointer-arithm"
    void *offset = obj + 10;
    cout <<"-Moved pointer 10 addresses. (at "<<offset<<")"<<endl;

    cout<<"-Attempting to free that pointer (program pausing for synchronization)" << endl;
    sleep(2);
    myfree(offset);

    myfree(obj);

    cout <<endl;
}