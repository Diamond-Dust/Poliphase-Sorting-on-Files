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
	TapeHandler(int pRecordCount = 256, int pBufferSize = 20, int pTapeCount = 3);
	TapeHandler(std::vector<double> pIs, std::vector<double> pRs, int pBufferSize = 20, int pTapeCount = 3);
	TapeHandler(char* pFileName, int pBufferSize=20, int pTapeCount = 3);

	TapeHandler(const TapeHandler& pTape);
	TapeHandler& operator=(const TapeHandler& pTape);

	int getTapeCount();

	/* Distributes the records from the last tape to every other tape. */
	void distribute(bool pPrint);

	/* Sorts the previously distributed records using poliphase sort */
	void sort(bool pPrint);

	void printCount();
	void printDetail();
	void printStats();

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
