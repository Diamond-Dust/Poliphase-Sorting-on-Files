#pragma once
#include<string>
#include<vector>
#include<random>
#include<bitset>
#include "bufferedtape.h"
#include "recordcomparer.h"

class TapeHandler
{
	public:
	/* Max number of tapes is _MAX_INT_DIG */
	TapeHandler(int pRecordCount = 1024*128, int pTapeCount = 3);
	TapeHandler(std::vector<double> pIs, std::vector<double> pRs, int pTapeCount = 3);

	int getTapeCount();

	/* Distributes the records from the last tape to every other tape. */
	void distribute();

	/* Sorts the previously distributed records using poliphase sort */
	void sort();

	void printCount();
	void printDetail();

	std::string getSmallestFirstRecord();


	~TapeHandler();
	private:
		BufferedTape* cTapes;
		int* cDummyCount;
		int* cRealCount;
		int* cSeriesCount;
		double* cLastPutRecords;
		std::bitset<_MAX_INT_DIG> cHasPutItsSeries;
		std::string* cFirstRecords;
		int cTapeCount;
		int cPhaseCount;
};
