#include "bufferedtape.h"

const std::string BufferedTape::EMPTY_RECORD = "                                                                                \n";
const int BufferedTape::RECORD_LENGTH = 81;

BufferedTape::BufferedTape(int pBufferSize) : cBufferCount(0), cBufferSize(pBufferSize), cMaxBufferSize(pBufferSize), hasRead(false), cDiscReadCount(0), cDiscWriteCount(0)
{
	if (!tmpnam_s(cFileName, L_tmpnam*sizeof(char)))
	{
		cFile.open(cFileName, std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
	}
	if (!cFile.is_open())
	{
		std::cerr << "Tape could not be opened." << std::endl;
		return;
	}

	cBuffer = (char*)malloc(sizeof(char) * (pBufferSize * RECORD_LENGTH + 1));	//Needs +1 to combat overflow error  - UB
}

BufferedTape::BufferedTape(char* pFileName, int pBufferSize) : cBufferCount(0), cBufferSize(pBufferSize), cMaxBufferSize(pBufferSize), hasRead(false), cDiscReadCount(0), cDiscWriteCount(0)
{
	strcpy_s(cFileName, pFileName);
	cFile.open(cFileName, std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
	if (!cFile.is_open())
	{
		std::cerr << "Tape could not be opened." << std::endl;
		return;
	}

	cBuffer = (char*)malloc(sizeof(char) * (pBufferSize * RECORD_LENGTH + 1));	//Needs +1 to combat overflow error  - UB
}

BufferedTape::BufferedTape(const BufferedTape& pTape) : cBufferCount(pTape.cBufferCount), cBufferSize(pTape.cBufferSize), cMaxBufferSize(pTape.cBufferSize), hasRead(pTape.hasRead), cDiscReadCount(pTape.cDiscReadCount), cDiscWriteCount(pTape.cDiscWriteCount)
{
	strcpy_s(cFileName, pTape.cFileName);
	cFile.open(cFileName, std::fstream::in | std::fstream::out | std::fstream::binary);
	if (!cFile.is_open())
	{
		std::cerr << "Tape could not be opened." << std::endl;
		return;
	}

	cBuffer = (char*)malloc(sizeof(char) * (pTape.cBufferSize * RECORD_LENGTH + 1));
	strncpy_s(cBuffer, pTape.cBufferSize * RECORD_LENGTH + 1, pTape.cBuffer, pTape.cBufferSize * RECORD_LENGTH);
}

BufferedTape& BufferedTape::operator=(const BufferedTape& pTape)
{
	free(cBuffer);
	cFile.close();
	std::remove(cFileName);


	cBufferCount = pTape.cBufferCount;
	cBufferSize = pTape.cBufferSize;
	cMaxBufferSize = pTape.cBufferSize;
	hasRead = pTape.hasRead;
	cDiscReadCount = pTape.cDiscReadCount;
	cDiscWriteCount = pTape.cDiscWriteCount;

	strcpy_s(cFileName, pTape.cFileName);
	cFile.open(cFileName, std::fstream::in | std::fstream::out | std::fstream::binary);
	if (!cFile.is_open())
	{
		std::cerr << "Tape could not be opened." << std::endl;
		return *this;
	}

	cBuffer = (char*)malloc(sizeof(char) * (pTape.cBufferSize * RECORD_LENGTH + 1));
	strncpy_s(cBuffer, pTape.cBufferSize * RECORD_LENGTH + 1, pTape.cBuffer, pTape.cBufferSize * RECORD_LENGTH);

	return *this;
}

bool BufferedTape::clearRecord()
{
	return writeRecord(EMPTY_RECORD);
}

bool BufferedTape::writeRecord(double pI, double pR)
{
	cRecordStream.str(std::string());
	cRecordStream << std::scientific << pI;
	int spaceCountNeeded = 40 - cRecordStream.str().length();
	while (spaceCountNeeded--)
	{
		cRecordStream << " ";
	}
	cRecordStream << std::scientific << pR;
	spaceCountNeeded = 80 - cRecordStream.str().length();
	while (spaceCountNeeded--)
	{
		cRecordStream << " ";
	}
	cRecordStream << "\n";

	std::string record = cRecordStream.str();
	return writeRecord(record);
}

bool BufferedTape::readRecord(std::string& pOutput)
{
	if (!cFile.is_open())
	{
		std::cerr << "Record could not be read. Tape could not be opened." << std::endl;
		return false;
	}

	/* If the last action was WRITE and there is something to write, flush it. */
	if (!hasRead && cBufferCount>0)
	{
		flush();
	}

	/* If the last action was WRITE or we filled our buffer, we need to READ from file. */
	if (!hasRead || cBufferCount >= cBufferSize)
	{
		cBufferCount = 0;

		int sizeToRead = cBufferSize * RECORD_LENGTH * sizeof(char);
		cFile.read(cBuffer, sizeToRead);

		cDiscReadCount++;

		hasRead = true;

		int readRecords = cFile.gcount() / (RECORD_LENGTH * sizeof(char));

		/* If we have reached the end of the file, notify the caller. */
		if (readRecords == 0)
		{
			return false;
		}

		/* If the end of the file is coming faster than end of the buffer, make the buffer smaller for now. */
		if (readRecords < cBufferSize)
		{
			cBufferSize = readRecords;
			cFile.clear();
		}
	}

	pOutput.assign(cBuffer + cBufferCount * RECORD_LENGTH, RECORD_LENGTH * sizeof(char));
	cBufferCount++;
	hasRead = true;

	return true;
}

void BufferedTape::flush()
{
	if (!cFile.is_open())
	{
		std::cerr << "Record could not be read. Tape could not be opened." << std::endl;
		return;
	}

	int sizeToWrite = cBufferCount * RECORD_LENGTH * sizeof(char);
	if (sizeToWrite > 0)
	{
		cFile.write(cBuffer, sizeToWrite);
		cDiscWriteCount++;
		cFile.flush();
		cBufferCount = 0;
	}
}

void BufferedTape::clear()
{
	cFile.close();
	cFile.open(cFileName, std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
	if (!cFile.is_open())
	{
		std::cerr << "Tape could not be opened." << std::endl;
		return;
	}
}

void BufferedTape::rewind()
{
	if (!hasRead)	// Flush the buffer if last action was WRITE
	{
		flush();
	}
	cFile.clear();
	cFile.seekp(0);
	cFile.clear();
}

void BufferedTape::printStart()
{
	cSave.set(cBuffer, sizeof(char) * (cBufferCount * RECORD_LENGTH), cBufferCount, cMaxBufferSize, cBufferSize, hasRead, cDiscReadCount, cDiscWriteCount, cFile.tellg());
}

void BufferedTape::printEnd()
{
	cBufferCount = cSave.cBufferCount;
	cMaxBufferSize = cSave.cMaxBufferSize;
	cBufferSize = cSave.cBufferSize;
	hasRead = cSave.hasRead;
	cDiscReadCount = cSave.cDiscReadCount;
	cDiscWriteCount = cSave.cDiscWriteCount;
	cFile.seekg(cSave.savedPosition);
	cFile.clear();
	strncpy_s(cBuffer, sizeof(char) * (cBufferCount * RECORD_LENGTH) + 1, cSave.cBuffer, sizeof(char) * (cBufferCount * RECORD_LENGTH));
	free(cSave.cBuffer);
	cSave.cBuffer = (char*)malloc(sizeof(char) * 1);
}

bool BufferedTape::writeRecord(std::string pOutput)
{
	if (!cFile.is_open())
	{
		std::cerr << "Record could not be read. Tape could not be opened." << std::endl;
		return false;
	}

	/* If READ was the last operation, we need to take care of obsolete buffer */
	if (hasRead)
	{
		/*	If we have already reached the end of the file, loop around. 
			If not, just move the pointer to the last unread position.  */
		if (cFile.peek() == EOF)
		{
			cFile.seekp(0);
			cFile.clear();
			cBufferSize = cMaxBufferSize;
		}
		else
		{
			int sizeToMove = -RECORD_LENGTH * (cBufferSize - cBufferCount);
			cFile.seekp(sizeToMove, std::ios_base::cur);
		}
		cBufferCount = 0;
	}

	strcpy_s(cBuffer + cBufferCount * RECORD_LENGTH, sizeof(char) * RECORD_LENGTH + 1, pOutput.c_str());
	hasRead = false;
	cBufferCount++;

	/* If we have filled our buffer, we need to dump it. */
	if (cBufferCount >= cMaxBufferSize)
	{
		flush();
	}

	return true;
}

BufferedTape::~BufferedTape()
{
	free(cBuffer);
	cFile.close();
	std::remove(cFileName);
}
