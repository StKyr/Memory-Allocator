#include <stdio.h>
#include "mymalloc.h"

int main(void){
	void *p = mymalloc(500);
	printf("%p\n", p);
	return 0;
}
