#include <iostream>
#include "sizeIndex.h"

#include <cmath>

 int SizeIndex::indexOf(int size) {

    int logarithm = (int)ceil(log2(size));

    if (logarithm < 5){
        return 0;
    }else{
        return logarithm - 5;
    }
}

PageList *SizeIndex::getPageList(size_t size) {

    int x = indexOf((int)size);

    return indexArray[indexOf((int)size)];
}

SizeIndex::SizeIndex() {

    for (int i=0; i<8; i++){
        this->indexArray[i] = new PageList((int)pow(2,i+5));
    }

}


