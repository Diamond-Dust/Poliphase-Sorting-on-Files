// Poliphase Sorting on Files.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "tape.h"

int main()
{
    std::cout << "Hello World!\n";
	Tape* myTape = new Tape("./TestTape.txt", 10);

	std::string currentRecord;
	for (int i = 0; i < 10; i++) {
		myTape->readRecord(currentRecord);
		myTape->nextRecord();
		std::cout << currentRecord << "|" << std::endl;
	}

	std::cout << std::endl << "----------------------------------" << std::endl << std::endl;

	for (int i = 0; i < 10; i++) {
		myTape->writeRecord(i+3.14, i*3.14*i*2.71);
		myTape->nextRecord();
	}

	std::cout << std::endl << "----------------------------------" << std::endl << std::endl;
	
	for (int i = 0; i < 10; i++) {
		myTape->readRecord(currentRecord);
		myTape->nextRecord();
		std::cout << currentRecord << "|" << std::endl;
	}

	return 0;
}

