CPPC = g++
AR = ar
ARFLAGS = crs
INCLUDE_PATH = ./include/
SOURCE_PATH = ./src/
OUTPUT_PATH = ./bin/
SAMPLES_PATH = ./samples/
CPPFLAGS = -I$(INCLUDE_PATH)
OBJECTIVES = $(BIN_PATH)allocator.o $(BIN_PATH)pages.o $(BIN_PATH)sizeIndex.o $(BIN_PATH)pagePool.o $(BIN_PATH)hashTable.o
NECESSARY = $(INCLUDE_PATH)global.h



sample: $(OUTPUT_PATH)main.o $(OBJECTIVES) $(NECESSARY)
	$(CPPC) $(CPPFLAGS)  -o $(OUTPUT_PATH)sample ($OUTPUT_PATH)main.o $(OBJECTIVES)

$(OUTPUT_PATH)main.o: $(SAMPLES_PATH)main.cpp
	$(CPPC) $(CPPFLAGS) -c $(SAMPLES_PATH)main.cpp -o $(BIN_PATH)main.o

allocator.o: $(SOURCE_PATH)allocator.cpp allocator.h mymalloc.h $(NECESSARY)
	$(CPPC) $(CPPFLAGS) -c $(SOURCE_PATH)allocator.cpp -o $(BIN_PATH)allocator.o

pages.o: $(SOURCE_PATH)pages.cpp pages.h $(NECESSARY)
	$(CPPC) $(CPPFLAGS) -c $(SOURCE_PATH)pages.cpp -o $(BIN_PATH)pages.o

sizeIndex.o: $(SOURCE_PATH)sizeIndex.cpp sizeIndex.h $(NECESSARY)
	$(CPPC) $(CPPFLAGS) -c $(SOURCE_PATH)sizeIndex.cpp -o $(BIN_PATH)sizeIndex.o

pagePool.o: $(SOURCE_PATH)pagePool.cpp pagePool.h $(NECESSARY)
	$(CPPC) $(CPPFLAGS) -c $(SOURCE_PATH)pagePool.cpp -o $(BIN_PATH)pagePool.o

hashTable.o: $(SOURCE_PATH)hashTable.cpp hashTable.h $(NECESSARY)
	$(CPPC) $(CPPFLAGS) -c $(SOURCE_PATH)hashTable.cpp -o $(BIN_PATH)hashTable.o


# not sure if this works
objectives: $(OBJECTIVES) $(NECESSARY)



library: $(OBJECTIVES) $(NECESSARY)
	$(AR) $(ARFLAGS) libmymalloc.a $(OBJECTIVES)


custom: $(OBJECTIVETS) $(NECESSARY) $(USER_CODE)
	$(CPPC) $(CPPFLAGS) -static $(USER_CODE) -L. -lmymalloc -o a.out


.PHONY: clean

clean:
	rm -f $(OBJECTIVES) $(OUTPUT_PATH)main.o $(OUTPUT_PATH)sample libmymalloc.a a.out


