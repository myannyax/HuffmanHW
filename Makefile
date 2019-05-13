key := -Wall -Werror -std=c++17 -pedantic -Wextra

all: main

test: h_test

main: bin bin/huffman.o bin/HuffTree.o bin/main.o
	g++ bin/huffman.o bin/HuffTree.o bin/main.o $(key) -o main

bin/huffman.o: src/huffman.cpp
	g++ -c src/huffman.cpp -o bin/huffman.o 

bin/main.o: src/main.cpp
	g++ -c src/main.cpp -o bin/main.o

bin/HuffTree.o: src/HuffTree.cpp
	g++ -c src/HuffTree.cpp -o bin/HuffTree.o

h_test: bin bin/autotest.o bin/huffman_test.o bin/test.o bin/huffman.o bin/HuffTree.o
	g++ bin/autotest.o bin/huffman_test.o bin/test.o bin/huffman.o bin/HuffTree.o $(key) -o h_test

bin/autotest.o: test/autotest.cpp
	g++ -c test/autotest.cpp -o bin/autotest.o

bin/test.o: test/test.cpp
	g++ -c test/test.cpp -o bin/test.o

bin/huffman_test.o: test/huffman_test.cpp
	g++ -c test/huffman_test.cpp -o bin/huffman_test.o
	
bin :
	mkdir -p bin

clean:
	rm -rf bin/*.o main h_test
	rmdir bin
