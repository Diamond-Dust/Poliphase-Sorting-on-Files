#pragma once
#include<string>
#include<vector>
#include<random>
#include "bufferedtape.h"

class TapeHandler
{
	public:
	TapeHandler(int pRecordCount = 256, int pTapeCount = 3);
	TapeHandler(std::vector<double> pIs, std::vector<double> pRs, int pTapeCount = 3);

	int getTapeCount();

	/* Distributes the records from the last tape to every other tape. */
	void distribute();

	void printCount();


	~TapeHandler();
	private:
		BufferedTape* cTapes;
		int* cDummyCount;
		int* cRealCount;
		int cTapeCount;
};
