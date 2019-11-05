#include "tapehandler.h"

TapeHandler::TapeHandler(int pRecordCount, int pBufferSize, int pTapeCount) : cTapeCount(std::min(pTapeCount, _MAX_INT_DIG))
{
	pTapeCount = std::min(pTapeCount, _MAX_INT_DIG);
	cDummyCount = new int[pTapeCount]();
	cRealCount = new int[pTapeCount]();
	cSeriesCount = new int[pTapeCount]();
	cLastPutRecords = new double[pTapeCount];
	cFirstRecords = new std::string[pTapeCount];
	for (int i = 0; i < pTapeCount; i++)
	{
		cLastPutRecords[i] = INFINITY;
	}
	cTapes = new BufferedTape[pTapeCount];
	for (int i = 0; i < pTapeCount; i++)
	{
		cTapes[i] = BufferedTape(pBufferSize);
	}

	std::vector<double> Is, Rs;
	double lowerBound = 0;
	double upperBound = 10000/*std::cbrt(DBL_MAX)*/;
	std::uniform_real_distribution<double> unif(lowerBound, upperBound);
	std::default_random_engine engine;

	/* Putting starting records on base tape - last tape */
	for (int i = 0; i < pRecordCount; i++)
	{
		cTapes[pTapeCount - 1].writeRecord(unif(engine), unif(engine));
	}
	cTapes[pTapeCount - 1].rewind();

	/* For the initial printing, not used in the algorithm */
	cRealCount[pTapeCount - 1] = pRecordCount;
}

TapeHandler::TapeHandler(std::vector<double> pIs, std::vector<double> pRs, int pBufferSize, int pTapeCount) : cTapeCount(std::min(pTapeCount, _MAX_INT_DIG))
{
	pTapeCount = std::min(pTapeCount, _MAX_INT_DIG);
	cDummyCount = new int[pTapeCount]();
	cRealCount	= new int[pTapeCount]();
	cSeriesCount = new int[pTapeCount]();
	cLastPutRecords = new double[pTapeCount];
	cFirstRecords = new std::string[pTapeCount];
	for (int i = 0; i < pTapeCount; i++)
	{
		cLastPutRecords[i] = INFINITY;
	}
	cTapes		= new BufferedTape[pTapeCount];
	for (int i = 0; i < pTapeCount; i++)
	{
		cTapes[i] = BufferedTape(pBufferSize);
	}
	/* Putting starting records on base tape - last tape */
	for (std::vector<double>::size_type i = 0; i != pIs.size(); i++)
	{
		cTapes[pTapeCount - 1].writeRecord(pIs[i], pRs[i]);
	}
	cTapes[pTapeCount - 1].rewind();

	/* For the initial printing, not used in the algorithm */
	cRealCount[pTapeCount - 1] = pIs.size();
}

TapeHandler::TapeHandler(char* pFileName, int pBufferSize, int pTapeCount)
{
	pTapeCount = std::min(pTapeCount, _MAX_INT_DIG);
	cDummyCount = new int[pTapeCount]();
	cRealCount = new int[pTapeCount]();
	cSeriesCount = new int[pTapeCount]();
	cLastPutRecords = new double[pTapeCount];
	cFirstRecords = new std::string[pTapeCount];
	for (int i = 0; i < pTapeCount; i++)
	{
		cLastPutRecords[i] = INFINITY;
	}
	cTapes = new BufferedTape[pTapeCount];
	for (int i = 0; i < pTapeCount-1; i++)
	{
		cTapes[i] = BufferedTape(pBufferSize);
	}
	cTapes[pTapeCount - 1] = BufferedTape(pFileName, pBufferSize);

	/* For the initial printing, not used in the algorithm */
	std::string record;
	cTapes[pTapeCount - 1].printStart();
	while (cTapes[pTapeCount - 1].readRecord(record))
	{
		cRealCount[pTapeCount-1]++;
	}
	cTapes[pTapeCount - 1].rewind();
	cTapes[pTapeCount - 1].printEnd();
}

TapeHandler::TapeHandler(const TapeHandler& pTape)
{
	cTapeCount = pTape.cTapeCount;
	cDummyCount = new int[cTapeCount]();
	cRealCount = new int[cTapeCount]();
	cSeriesCount = new int[cTapeCount]();
	cLastPutRecords = new double[cTapeCount];
	cFirstRecords = new std::string[cTapeCount];
	memcpy(cDummyCount, pTape.cDummyCount, sizeof(int) * cTapeCount);
	memcpy(cRealCount, pTape.cRealCount, sizeof(int) * cTapeCount);
	memcpy(cSeriesCount, pTape.cSeriesCount, sizeof(int) * cTapeCount);
	memcpy(cLastPutRecords, pTape.cLastPutRecords, sizeof(double) * cTapeCount);
	std::copy(pTape.cFirstRecords, cFirstRecords + cTapeCount, cFirstRecords);
	cTapes = new BufferedTape[cTapeCount];
	for (int i = 0; i < cTapeCount; i++)
	{
		cTapes[i] = pTape.cTapes[i];
	}
}

TapeHandler& TapeHandler::operator=(const TapeHandler& pTape)
{
	delete[] cDummyCount;
	delete[] cRealCount;
	delete[] cLastPutRecords;
	delete[] cTapes;
	delete[] cFirstRecords;


	cTapeCount = pTape.cTapeCount;
	cDummyCount = new int[cTapeCount]();
	cRealCount = new int[cTapeCount]();
	cSeriesCount = new int[cTapeCount]();
	cLastPutRecords = new double[cTapeCount];
	cFirstRecords = new std::string[cTapeCount];
	memcpy(cDummyCount, pTape.cDummyCount, sizeof(int) * cTapeCount);
	memcpy(cRealCount, pTape.cRealCount, sizeof(int) * cTapeCount);
	memcpy(cSeriesCount, pTape.cSeriesCount, sizeof(int) * cTapeCount);
	memcpy(cLastPutRecords, pTape.cLastPutRecords, sizeof(double) * cTapeCount);
	for (int i = 0; i < pTape.cTapeCount; i++)
	{
		cFirstRecords[i] = pTape.cFirstRecords[i];
	}
	cTapes = new BufferedTape[cTapeCount];
	for (int i = 0; i < cTapeCount; i++)
	{
		cTapes[i] = pTape.cTapes[i];
	}

	return *this;
}

int TapeHandler::getTapeCount()
{
	return cTapeCount;
}

void TapeHandler::distribute(bool pPrint)
{
	/* Check if everything is on one tape */
	for (int i = 0; i < cTapeCount-1; i++)
	{
		if (cDummyCount[i] != 0 || cRealCount[i] != 0)
		{
			std::cerr << "Attempt to distribute already distributed records." << std::endl;
			return;
		}
	}

	cDummyCount[cTapeCount - 1] = 0;
	cSeriesCount[cTapeCount - 1] = 0;

	cPhaseCount = 0;
	std::string record;
	bool recordsExist = cTapes[cTapeCount - 1].readRecord(record);
	cRealCount[cTapeCount - 1]--;
	if (!recordsExist)
	{
		return;
	}
	cTapes[0].writeRecord(record);
	cLastPutRecords[0] = Record::getValue(record);
	cRealCount[0]++;
	cSeriesCount[0]++;
	int seriesCount = 1, currentSeriesCount = 0, currentPosition = 1, skippedPosition = 0;
	while (cTapes[cTapeCount - 1].readRecord(record))
	{
		cTapes[currentPosition].writeRecord(record);
		cRealCount[currentPosition]++;
		cRealCount[cTapeCount - 1]--;

		if (Record::getValue(record) < cLastPutRecords[currentPosition])	//Start new series only if new records is not in order
		{
			currentSeriesCount++;
			cSeriesCount[currentPosition]++;
		}
		cLastPutRecords[currentPosition] = Record::getValue(record);

		if (currentSeriesCount == seriesCount)			//If we have filled current tape with enough records for now
		{
			currentPosition++;							//Move forwards
			if (currentPosition == skippedPosition)		//If we should skip new spot, do it
			{
				currentPosition++;
			}
			if (currentPosition >= cTapeCount - 1)		//If we have gone too far, go back to start, move skipped, increase specified number of records
			{
				currentPosition = 0;
				skippedPosition++;
				if (skippedPosition >= cTapeCount - 1)
				{
					skippedPosition %= (cTapeCount - 1);
				}
				cPhaseCount++;
				seriesCount = cSeriesCount[skippedPosition] + cDummyCount[skippedPosition];

				if (pPrint)
				{
					printCount();
					std::cout << " --- " << std::endl;
				}
			}
			if (currentPosition == skippedPosition)		//If we should skip new spot, do it
			{
				currentPosition++;
			}
			currentSeriesCount = 0;						//Reset record counting
		}
	}
	cPhaseCount++;

	cDummyCount[currentPosition] += std::max(0, seriesCount - currentSeriesCount);	// If the current is missing some, insert dummies
	currentPosition++;					// If the run has not finished, insert dummies to the end.
	for (; currentPosition < cTapeCount - 1; currentPosition++)
	{
		if (currentPosition != skippedPosition)
		{
			cDummyCount[currentPosition] += seriesCount;
		}
	}

	/* Cleanup */
	cTapes[cTapeCount - 1].clear();
	for (int i = 0; i < cTapeCount - 1; i++)
	{
		cTapes[i].rewind();
	}

	if (pPrint)
	{
		printCount();
		std::cout << " --- " << std::endl;
	}
}

void TapeHandler::sort(bool pPrint)
{
	int emptyTapeIndex = cTapeCount - 1;
	/* Check if last tape is empty */
	if (cDummyCount[emptyTapeIndex] != 0 || cRealCount[emptyTapeIndex] != 0)
	{
		std::cerr << "Attempt to sort not distributed records." << std::endl;
		return;
	}

	/* Bitset will tell us whether given tape has already given us a full series. */
	cHasPutItsSeries = 0;
	/* cFirstRecords will now double as a way to remember top records from non-empty tapes */
	for (int i = 0; i < cTapeCount; i++)
	{
		if (i != emptyTapeIndex)
		{
			cTapes[i].readRecord(cFirstRecords[i]);
		}
	}

	bool finishedPhase;
	int phaseCount = cPhaseCount;
	while (phaseCount--)
	{
		cHasPutItsSeries = 0;
		cHasPutItsSeries[emptyTapeIndex] = true;
		finishedPhase = false;
		while (!finishedPhase)
		{
			while (cHasPutItsSeries.count() < cTapeCount)	//While some of the tapes have not put their series into the empty tape
			{
				std::string smallestRecord = getSmallestFirstRecord();
				if (!smallestRecord.empty())	//If it wasn't a dummy
				{
					cTapes[emptyTapeIndex].writeRecord(smallestRecord);
					cRealCount[emptyTapeIndex]++;
				}
			}
			cHasPutItsSeries = 0;
			for (int i = 0; i < cTapeCount; i++)
			{
				if (cFirstRecords[i] == "")
				{
					cHasPutItsSeries[i] = true;
				}
			}
			cSeriesCount[emptyTapeIndex]++;
			/* Find from where to start the empty tape countdown */
			for (int i = 0; i < cTapeCount; i++)
			{
				if (i != emptyTapeIndex && cSeriesCount[i] == 0)
				{
					cTapes[emptyTapeIndex].rewind();
					cTapes[emptyTapeIndex].readRecord(cFirstRecords[emptyTapeIndex]);
					emptyTapeIndex = i;
					cTapes[emptyTapeIndex].clear();
					finishedPhase = true;
				}
			}
			//printCount();
			//std::cout << " --- " << std::endl;
		}
		if (pPrint)
		{
			printDetail();
			std::cout << " --- " << std::endl;
			printCount();
			std::cout << " --- " << std::endl;
		}
	}
}

void TapeHandler::printCount()
{
	for (int i = 0; i < cTapeCount; i++)
	{
		std::cout << "Tape number " << i << " has now \t" << cRealCount[i] << " records in series: \t" << cSeriesCount[i] + cDummyCount[i] << " = " << cSeriesCount[i] << "(" << cDummyCount[i] << ")" << std::endl;
	}
}

void TapeHandler::printDetail()
{
	std::string record;
	int seriesNumber;
	double lastValue;
	for (int i = 0; i < cTapeCount; i++)
	{
		cTapes[i].printStart();
		cTapes[i].rewind();
		lastValue = INFINITY;
		if (cRealCount[i] + cDummyCount[i] > 0) 
		{
			seriesNumber = 0;
			std::cout << "Tape number " << i << " has now " << cRealCount[i] << " real records:" << std::endl;

			if (cFirstRecords[i] != "")
			{
				if (Record::getValue(cFirstRecords[i]) < lastValue)
				{
					std::cout << "\tSeries nr " << seriesNumber << ": \t" << std::endl;
					seriesNumber++;
				}
				lastValue = Record::getValue(cFirstRecords[i]);
				std::cout << "\t\t" << Record::getValue(cFirstRecords[i]) << " " << cFirstRecords[i];
			}
			
			while (cTapes[i].readRecord(record))
			{
				if (Record::getValue(record) < lastValue)
				{
					std::cout << "\tSeries nr " << seriesNumber << ": \t" << std::endl;
					seriesNumber++;
				}
				lastValue = Record::getValue(record);
				std::cout << "\t\t" << Record::getValue(record) << " " << record;
			}
			//cSeriesCount[i] = seriesNumber;
			if (cDummyCount[i] > 0)
			{
				std::cout << "\tAnd " << cDummyCount[i] << " dummy series." << std::endl;
			}
		}
		else
		{
			std::cout << "Tape number " << i << " has nothing in it." << std::endl;
		}
		cTapes[i].printEnd();
	}
}

void TapeHandler::printStats()
{
	int discWrites = 0, discReads = 0;
	for (int i = 0; i < cTapeCount; i++)
	{
		discReads += cTapes[i].cDiscReadCount;
		discWrites += cTapes[i].cDiscWriteCount;
	}
	std::cout	<< "Number of phases: "			<< cPhaseCount	<< std::endl
				<< "Number of disc writes: "	<< discWrites	<< std::endl
				<< "Number of disc reads: "		<< discReads	<< std::endl;

}

std::string TapeHandler::getSmallestFirstRecord()
{
	/* Check if there are any dummies*/
	int dummyIndex = -1;
	for (int i = 0; i < cTapeCount; i++)
	{
		if (!cHasPutItsSeries[i] && cDummyCount[i] > 0)	
		{
			cDummyCount[i]--;
			cHasPutItsSeries[i] = true;
			return "";
		}
	}


	/* Find the smallest first record */
	int minimumIndex = -1;
	double minimumValue = INFINITY;
	for (int i = 0; i < cTapeCount; i++)
	{
		if (!cHasPutItsSeries[i] && Record::getValue(cFirstRecords[i]) <= minimumValue)	//If there are still records in series and is smaller than last smallest
		{
			minimumValue = Record::getValue(cFirstRecords[i]);
			minimumIndex = i;
		}
	}
	std::string smallestRecord = cFirstRecords[minimumIndex];

	/* Clear up and set flags */
	bool nextRecord = cTapes[minimumIndex].readRecord(cFirstRecords[minimumIndex]);
	if (!nextRecord)
	{
		cFirstRecords[minimumIndex] = "";
		cRealCount[minimumIndex]--;
		cHasPutItsSeries[minimumIndex] = true;
		cSeriesCount[minimumIndex]--;
	}
	else
	{
		cRealCount[minimumIndex]--;
		if (Record::getValue(cFirstRecords[minimumIndex]) < Record::getValue(smallestRecord))	//If the series has ended
		{
			cHasPutItsSeries[minimumIndex] = true;
			cSeriesCount[minimumIndex]--;
		}
	}

	return smallestRecord;
}

TapeHandler::~TapeHandler()
{
	delete[] cDummyCount;
	delete[] cRealCount;
	delete[] cLastPutRecords;
	delete[] cTapes;
	delete[] cFirstRecords;
}
