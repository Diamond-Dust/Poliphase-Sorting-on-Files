// Poliphase Sorting on Files.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "tape.h"
#include "bufferedtape.h"
#include "tapehandler.h"

/*
	Command line arguments:
	1: Record gathering method	
		0	: random
		1	: by hand
		2	: file
	2: Record number / path
		If 0: was 0 or 1 here input an integer specifying how many records there will be. 
			0	: 256
		If 0: was 2, here put a path to the file.
	3: Buffer size
		0	:	20
		Any number greater than 0 will be accepted instead
	4: Tape number
		0	: 3
		Any number greater than 2 but lesser than _MAX_INT_DIG+1 will be accepted instead
	5: Printing policy
		0	: The series and record counts will be printed at the end of each phase.
		1	: No printing will happen.
		2	: 0 + print out every record at the end and start.
	Any error or lack of a value will assume 0 on the corresponding location.
*/
int main(int argc, char* argv[])
{
	//Filling unfilled input arguments
	if (argc < 6)
	{
		for (int i = argc; i < 6; i++)
		{
			argv[i] = (char*)calloc(1, sizeof(char));
		}
	}

	TapeHandler myTape;
	int recordNumber = std::atoi(argv[2]);
	recordNumber = (recordNumber < 1) ? 256 : recordNumber;
	int bufferSize = std::atoi(argv[3]);
	bufferSize = (bufferSize < 1) ? 20 : bufferSize;
	int tapeNumber = std::atoi(argv[4]);
	tapeNumber = (tapeNumber < 3) ? 3 : tapeNumber;
	bool willPrint = std::atoi(argv[5]) != 1;
	if (std::atoi(argv[1]) == 1)
	{
		std::vector<double> Is, Rs;
		double I, R;
		for (int i = 0; i < recordNumber; i++)
		{
			std::cin >> I >> R;
			Is.push_back(I);
			Rs.push_back(R);
		}
		myTape = TapeHandler(Is, Rs, bufferSize, tapeNumber);
	}
	else if (std::atoi(argv[1]) == 2)
	{

		myTape = TapeHandler(argv[2], bufferSize, tapeNumber);
	}
	else
	{
		myTape = TapeHandler(recordNumber, bufferSize, tapeNumber);
	}

	if (std::atoi(argv[5]) == 2)
	{
		std::cout << " ---------------------------- " << std::endl;
		myTape.printDetail();
	}

	if (willPrint)
	{
		myTape.printCount();
		std::cout << " ---------------------------- " << std::endl;
	}

	myTape.distribute(willPrint);
	if (willPrint)
	{
		std::cout << " ---------------------------- " << std::endl;
		myTape.printCount();
		std::cout << " ---------------------------- " << std::endl;
	}
	/*if (std::atoi(argv[5]) == 0)
	{
		std::cout << " ---------------------------- " << std::endl;
		myTape.printDetail();
	}
	if (willPrint)
	{
		std::cout << " ---------------------------- " << std::endl;
		myTape.printCount();
		std::cout << " ---------------------------- " << std::endl;
	}*/
	myTape.sort(willPrint);
	if (willPrint)
	{
		std::cout << " ---------------------------- " << std::endl;
		myTape.printCount();
	}


	if (std::atoi(argv[5]) == 2)
	{
		std::cout << " ---------------------------- " << std::endl;
		myTape.printDetail();
	}
	if (willPrint)
	{
		std::cout << " ---------------------------- " << std::endl;
		myTape.printCount();
	}

	if (willPrint)
	{
		std::cout << " ---------------------------- " << std::endl;
		myTape.printStats();
	}

	std::cout << "Sorting has ended. Press enter to delete files." << std::endl;
	std::getchar();

	return 0;
}

