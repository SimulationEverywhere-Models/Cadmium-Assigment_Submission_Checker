CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include
INCLUDEDESTIMES=-I ../../DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p results)

#TARGET TO COMPILE ALL THE TESTS TOGETHER (NOT SIMULATOR)
message.o: data_structures/message.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/message.cpp -o build/message.o

main.o: top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main.cpp -o build/main.o
	
assignment.o: test/assignment.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/assignment.cpp -o build/assignment.o

timechecker.o: test/timechecker.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/timechecker.cpp -o build/timechecker.o

plagiarismchecker.o: test/plagiarismchecker.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/plagiarismchecker.cpp -o build/plagiarismchecker.o

mailchecker.o: test/mailchecker.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/mailchecker.cpp -o build/mailchecker.o

evaluator.o: test/evaluator.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/evaluator.cpp -o build/evaluator.o

tests: assignment.o  message.o timechecker.o plagiarismchecker.o mailchecker.o evaluator.o
		$(CC) -g -o bin/ASSIGNMENT_TEST build/assignment.o build/message.o
		$(CC) -g -o bin/TIMECHECKER_TEST build/timechecker.o build/message.o
		$(CC) -g -o bin/PLAGIARISMCHECKER_TEST build/plagiarismchecker.o build/message.o
		$(CC) -g -o bin/MAILCHECKER_TEST build/mailchecker.o build/message.o
		$(CC) -g -o bin/EVALUATOR_TEST build/evaluator.o build/message.o

# TARGET TO COMPILE ONLY ABP SIMULATOR
simulator: main.o message.o 
	$(CC) -g -o bin/ASC build/main.o build/message.o 
	
#TARGET TO COMPILE EVERYTHING (ABP SIMULATOR + TESTS TOGETHER)
all: tests simulator

#CLEAN COMMANDS
clean: 
	rm -f bin/* build/* results/*