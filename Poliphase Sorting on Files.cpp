// Poliphase Sorting on Files.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "tape.h"
#include "bufferedtape.h"
#include "tapehandler.h"

int main()
{
    std::cout << "Hello World!\n";
	/*Tape* myTape = new Tape("./TestTape.txt", 10);

	std::string currentRecord;
	for (int i = 0; i < 10; i++) {
		myTape->readRecord(currentRecord);
		std::cout << currentRecord << "|" << std::endl;
	}

	std::cout << std::endl << "----------------------------------" << std::endl << std::endl;

	for (int i = 0; i < 10; i++) {
		myTape->writeRecord(i+3.14, i*3.14*i*2.71);
	}
	myTape->flush();

	std::cout << std::endl << "----------------------------------" << std::endl << std::endl;
	
	for (int i = 0; i < 10; i++) {
		myTape->readRecord(currentRecord);
		std::cout << currentRecord << "|" << std::endl;
	}*/

	/*BufferedTape myTape(10, 3);

	std::string currentRecord;
	for (int i = 0; i < 10; i++) {
		myTape.readRecord(currentRecord);
		std::cout << currentRecord << "|" << std::endl;
	}

	std::cout << std::endl << "----------------------------------" << std::endl << std::endl;

	for (int i = 0; i < 10; i++) 
	{
		myTape.writeRecord(i + 3.14, i * 3.14 * i * 2.71);
	}
	myTape.flush();

	std::cout << std::endl << "----------------------------------" << std::endl << std::endl;

	for (int i = 0; i < 10; i++) {
		myTape.readRecord(currentRecord);
		std::cout << currentRecord << "|" << std::endl;
	}*/

	TapeHandler myTape{};

	myTape.printCount();
	std::cout << " ---------------------------- " << std::endl;
	myTape.distribute();
	std::cout << " ---------------------------- " << std::endl;
	myTape.printCount();
	//myTape.printDetail();
	std::cout << " ---------------------------- " << std::endl;
	myTape.sort();
	std::cout << " ---------------------------- " << std::endl;
	myTape.printCount();

	return 0;
}

