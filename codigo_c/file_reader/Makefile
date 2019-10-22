CC=gcc
CCPP=g++

say_hello:
	@echo "Hello class!"
	@echo "Creating a FILE-READER OF TXT"
	@echo "------------------------------"
          
	@echo "For compiling, run: make all"
	@echo "For cleaning the .o, run make clean"
	@echo "Compiler available is " $(CCPP)
	@echo "-------------------------------"

all:
	$(CCPP) file_reader.cpp -o reader.o

clean:
	@echo "Cleaning up.."
	rm -rvf *.o


