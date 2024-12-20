# Detect the operating system
OS := $(shell uname)

# Set the executable extension based on the operating system
ifeq ($(OS), Darwin)
    EXE_EXT :=
else
    EXE_EXT := .exe
endif

logAppendsources = ./src/logappend.cpp
logReadsources = ./src/logRead.cpp

logAppendTarget = ./out/logappend$(EXE_EXT)
logReadTarget = ./out/logread$(EXE_EXT)

compiler = g++

COMPILER_FLAGS = -std=c++11 -I./include/ -I./include/cryptopp/ -L./include/cryptopp890/ -lcryptopp


logAppend: inputSanatizer.o encryptionHandler.o
	$(compiler) $(logAppendsources) ./out/inputSanatizer.o ./out/encryptionHandler.o  -I./include/ $(COMPILER_FLAGS) -o $(logAppendTarget)


logRead: inputSanatizer.o encryptionHandler.o
	$(compiler) $(logReadsources) ./out/inputSanatizer.o ./out/encryptionHandler.o  $(COMPILER_FLAGS) -o $(logReadTarget) 


encryptionHandler.o:
	$(compiler) ./src/encryptionHandler.cpp -c -I./include -I./include/cryptopp/ -L./include/cryptopp890/ -lcryptopp -o ./out/encryptionHandler.o

inputSanatizer.o: encryptionHandler.o
	$(compiler) ./src/inputSanatizer.cpp ./out/encryptionHandler.o -c -I./include $(COMPILER_FLAGS) -o ./out/inputSanatizer.o



logappend.o: inputSanatizer.o encryptionHandler.o
	$(compiler) $(logAppendsources) ./out/inputSanatizer.o ./out/encryptionHandler.o $(COMPILER_FLAGS) -o ./out/logappend.o


clean:
	rm -f ./out/*.o
	rm -f ./out/*.exe
	rm -f ./out/logAppend
	rm -f ./out/logRead
	rm -f ./tests/out/*


tests: inputSanatizer.o encryptionHandler.o
	$(compiler) ./tests/inputSanitizerTests.cpp ./out/inputSanatizer.o ./out/encryptionHandler.o $(COMPILER_FLAGS) -I./include -o ./tests/out/inputSanitizerTests
	./tests/out/inputSanitizerTests$(EXE_EXT)

	$(compiler) ./tests/validArrivalLeaveTest.cpp ./out/inputSanatizer.o ./out/encryptionHandler.o $(COMPILER_FLAGS) -o ./tests/out/validArrivalLeaveTest
	./tests/out/validArrivalLeaveTest$(EXE_EXT)
	
	$(compiler) ./tests/sTagTests.cpp ./out/inputSanatizer.o ./out/encryptionHandler.o $(COMPILER_FLAGS) -o ./tests/out/sTagTests
	./tests/out/sTagTests$(EXE_EXT)


	$(compiler) ./tests/validTimeStampTest.cpp ./out/inputSanatizer.o ./out/encryptionHandler.o $(COMPILER_FLAGS) -o ./tests/out/validTimeStampTest
	./tests/out/validTimeStampTest$(EXE_EXT)

	$(compiler) ./tests/rTagTests.cpp ./out/inputSanatizer.o ./out/encryptionHandler.o $(COMPILER_FLAGS) -o ./tests/out/rTagTests
	./tests/out/rTagTests$(EXE_EXT)
