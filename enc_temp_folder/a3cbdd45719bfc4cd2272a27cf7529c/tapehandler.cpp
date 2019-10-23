#include "tapehandler.h"

TapeHandler::TapeHandler(int pRecordCount, int pTapeCount) : cTapeCount(std::min(pTapeCount, _MAX_INT_DIG))
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
}

TapeHandler::TapeHandler(std::vector<double> pIs, std::vector<double> pRs, int pTapeCount) : cTapeCount(std::min(pTapeCount, _MAX_INT_DIG))
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
	/* Check if everything is on one tape */
	for (int i = 0; i < cTapeCount-1; i++)
	{
		if (cDummyCount[i] != 0 || cRealCount[i] != 0)
		{
			std::cerr << "Attempt to distribute already distributed records." << std::endl;
			return;
		}
	}

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
	cLastPutRecords[0] = Record::getValue(record);
	cRealCount[0]++;
	bool finishedFirstDistributionPhase = false;
	int seriesCount = 1, currentSeriesCount = 0, currentPosition = 1, skippedPosition = 0;
	while (cTapes[cTapeCount - 1].readRecord(record))
	{
		cTapes[currentPosition].writeRecord(record);
		cRealCount[currentPosition]++;

		if (Record::getValue(record) < cLastPutRecords[currentPosition])	//Start new series only if new records is not in order
		{
			cSeriesCount[currentPosition]++;
			currentSeriesCount++;
		}

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
					finishedFirstDistributionPhase = true;
					skippedPosition %= (cTapeCount - 1);
				}
				//printCount();
				seriesCount = cSeriesCount[skippedPosition] + cDummyCount[skippedPosition];
			}
			if (currentPosition == skippedPosition)		//If we should skip new spot, do it
			{
				currentPosition++;
			}
			currentSeriesCount = 0;						//Reset record counting
		}
	}
	cDummyCount[currentPosition] += std::max(0, seriesCount - currentSeriesCount);	// If the current is missing some, insert dummies
	if (currentPosition++ != 0)							// If the run has not finished, insert dummies to the end.
	{
		for (; currentPosition < cTapeCount - 1; currentPosition++)
		{
			cDummyCount[currentPosition] += seriesCount - currentSeriesCount;
			currentSeriesCount = 0;
		}
	}
	cTapes[cTapeCount - 1].clear();
}

void TapeHandler::sort()
{
	int emptyTapeIndex = cTapeCount - 1;
	/* Check if last tape is empty */
	if (cDummyCount[emptyTapeIndex] != 0 || cRealCount[emptyTapeIndex] != 0)
	{
		std::cerr << "Attempt to sort not distributed records." << std::endl;
		return;
	}

	/* Bitset will tell us whether given tape has alseardy given us a full series. */
	cHasPutItsSeries = 0;
	/* cFirstRecords will now double as a way to remember top records from non-empty tapes */
	for (int i = 0; i < cTapeCount; i++)
	{
		if (i != emptyTapeIndex)
		{
			cTapes[i].readRecord(cFirstRecords[i]);
		}
	}

	bool finishedPhase = false;
	while (!finishedPhase)
	{
		cHasPutItsSeries[emptyTapeIndex] = true;
		finishedPhase = false;
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
		/* Find from where to start the empty tape countdown */
		for (int i = 0; i < cTapeCount; i++)
		{
			if (i == emptyTapeIndex)
			{
				cSeriesCount[i]++;
			}
			else
			{
				if (cSeriesCount[i] == 0)
				{
					emptyTapeIndex = i;
					cTapes[emptyTapeIndex].clear();
					finishedPhase = true;
				}
			}
		}
		printCount();
	}
	//printDetail();
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
		lastValue = INFINITY;
		if (cRealCount[i] + cDummyCount[i] > 0) 
		{
			seriesNumber = 0;
			std::cout << "Tape number " << i << " has now " << cRealCount[i] << " real records:" << std::endl;
			while (seriesNumber < cSeriesCount[i])
			{
				if (cTapes[i].readRecord(record))	// If read failed, it means that we were on the end of the tape
				{
					if (Record::getValue(record) < lastValue)
					{
						std::cout << "\tSeries nr " << seriesNumber << ": \t" << std::endl;
						seriesNumber++;
					}
					lastValue = Record::getValue(record);
					std::cout << "\t\t" << record;
				}
			}
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
			//dummyIndex = i;
			//break;
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
	cRealCount[minimumIndex]--;
	if (Record::getValue(cFirstRecords[minimumIndex]) < Record::getValue(smallestRecord))	//If the series has ended
	{
		cHasPutItsSeries[minimumIndex] = true;
		cSeriesCount[minimumIndex]--;
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
