#include "tapehandler.h"

TapeHandler::TapeHandler(int pRecordCount, int pTapeCount) : cTapeCount(pTapeCount)
{
	cDummyCount = new int[pTapeCount];
	cRealCount = new int[pTapeCount];
	cTapes = new BufferedTape[pTapeCount];

	std::vector<double> Is, Rs;
	double lowerBound = 0;
	double upperBound = std::cbrt(DBL_MAX);
	std::uniform_real_distribution<double> unif(lowerBound, upperBound);
	std::default_random_engine engine;

	/* Putting starting records on base tape - last tape */
	for (int i = 0; i < pRecordCount; i++)
	{
		cTapes[pTapeCount - 1].writeRecord(unif(engine), unif(engine));
	}
	cTapes[pTapeCount - 1].rewind();
}

TapeHandler::TapeHandler(std::vector<double> pIs, std::vector<double> pRs, int pTapeCount) : cTapeCount(pTapeCount)
{
	cDummyCount = new int[pTapeCount];
	cRealCount	= new int[pTapeCount];
	cTapes		= new BufferedTape[pTapeCount];
	/* Putting starting records on base tape - last tape */
	for (std::vector<double>::size_type i = 0; i != pIs.size(); i++)
	{
		cTapes[pTapeCount - 1].writeRecord(pIs[i], pRs[i]);
	}
	cTapes[pTapeCount - 1].rewind();
}

int TapeHandler::getTapeCount()
{
	return cTapeCount;
}

void TapeHandler::distribute()
{
	for (int i = 0; i < cTapeCount; i++)
	{
		cDummyCount[i] = 0;
		cRealCount[i] = 0;
	}

	std::string record;
	bool recordsExist = cTapes[cTapeCount - 1].readRecord(record);
	if (!recordsExist)
	{
		return;
	}
	cTapes[0].writeRecord(record);
	cRealCount[0]++;
	int recordCount = 1, currentRecordCount = 0, currentPosition = 1, skippedPosition = 0;
	while (cTapes[cTapeCount - 1].readRecord(record))
	{
		cTapes[currentPosition].writeRecord(record);
		cRealCount[currentPosition]++;
		currentRecordCount++;

		if (currentRecordCount == recordCount)			//If we have filled curret tape with enough records for now
		{
			currentPosition++;							//Move forwards
			if (currentPosition == skippedPosition)		//If we should skip new spot, do it
			{
				currentPosition++;
			}
			if (currentPosition >= cTapeCount - 1)		//If we have gona too far, go back to start, move skipped, increase specified number of records
			{
				currentPosition = 0;
				skippedPosition++;
				skippedPosition %= (cTapeCount - 1);
				printCount();
				recordCount = cRealCount[skippedPosition] + cDummyCount[skippedPosition];
			}
			if (currentPosition == skippedPosition)		//If we should skip new spot, do it
			{
				currentPosition++;
			}
			currentRecordCount = 0;						//Reset record counting
		}
	}
	cDummyCount[currentPosition] += std::max(0, recordCount - currentRecordCount);	// If the current is missing some, insert dummies
	if (currentPosition++ != 0)							// If the run has not finished, insert dummies to the end.
	{
		for (; currentPosition < cTapeCount - 1; currentPosition++)
		{
			cDummyCount[currentPosition] += recordCount - currentRecordCount;
			currentRecordCount = 0;
		}
	}
}

void TapeHandler::printCount()
{
	for (int i = 0; i < cTapeCount; i++)
	{
		std::cout << "Tape number " << i << " has now \t" << cRealCount[i] + cDummyCount[i] << " records: \t" << cRealCount[i] << "(" << cDummyCount[i] << ")" << std::endl;
	}
}

TapeHandler::~TapeHandler()
{
	delete[] cDummyCount;
	delete[] cRealCount;
	delete[] cTapes;
}
