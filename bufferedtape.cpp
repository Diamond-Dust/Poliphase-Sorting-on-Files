#include "bufferedtape.h"

const std::string BufferedTape::EMPTY_RECORD = "                                                                                \n";
const int BufferedTape::RECORD_LENGTH = 81;

BufferedTape::BufferedTape() : BufferedTape(0, 100) { }

BufferedTape::BufferedTape(int pTapeSize, int pBufferSize) : cBufferCount(0), cBufferSize(pBufferSize), cMaxBufferSize(pBufferSize), hasRead(false)
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

	while (pTapeSize--)
	{
		cFile.write(EMPTY_RECORD.c_str(), RECORD_LENGTH);
	}
	rewind();

	cBuffer = (char*)malloc(sizeof(char) * (pBufferSize * RECORD_LENGTH + 1));	//Needs +1 to combat overflow error  - UB
}

bool BufferedTape::clearRecord()
{
	return writeRecord(EMPTY_RECORD);
}

bool BufferedTape::writeRecord(double pI, double pR)
{
	std::stringstream stream;
	stream << std::scientific << pI;
	int spaceCountNeeded = 40 - stream.str().length();
	while (spaceCountNeeded--)
	{
		stream << " ";
	}
	stream << std::scientific << pR;
	spaceCountNeeded = 80 - stream.str().length();
	while (spaceCountNeeded--)
	{
		stream << " ";
	}
	stream << "\n";

	std::string record = stream.str();
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

	/* If at the end of the file, go back to start and notify caller */
	/*if (cFile.peek() == EOF && cBufferCount >= cBufferSize)
	{
		//std::cout << "-----| " << cFile.tellg() << "|-----" << std::endl;
		cFile.clear();
		cFile.seekg(0);
		cFile.clear();
		//std::cout << "-----| " << cFile.tellg() << "|-----" << std::endl;
		cBufferSize = cMaxBufferSize;
		hasRead = false;
		return false;
	}*/

	/* If the last action was WRITE or we filled our buffer, we need to READ from file. */
	if (!hasRead || cBufferCount >= cBufferSize)
	{
		cBufferCount = 0;

		//std::cout << "-----| " << cFile.tellg() << "|-----" << std::endl;
		int sizeToRead = cBufferSize * RECORD_LENGTH * sizeof(char);
		cFile.read(cBuffer, sizeToRead);
		hasRead = true;

		int readRecords = cFile.gcount() / (RECORD_LENGTH * sizeof(char));
		//std::cout << "-----| " << cFile.tellg() << "|-----" << std::endl;

		/* If we have reached the end of the file, loop around and notify the caller. */
		if (readRecords == 0)
		{
			//cFile.seekp(0);
			//cFile.clear();
			//cBufferSize = cMaxBufferSize;
			//cBufferCount = 0;
			//hasRead = false;
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
	cFile.write(cBuffer, sizeToWrite);
	cFile.flush();
	cBufferCount = 0;
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
	cSave.set(cBuffer, sizeof(char) * (cBufferCount * RECORD_LENGTH), cBufferCount, cMaxBufferSize, cBufferSize, hasRead, cFile.tellg());
}

void BufferedTape::printEnd()
{
	cBufferCount = cSave.cBufferCount;
	cMaxBufferSize = cSave.cMaxBufferSize;
	cBufferSize = cSave.cBufferSize;
	hasRead = cSave.hasRead;
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
		int sizeToWrite = cBufferCount * RECORD_LENGTH * sizeof(char);
		cFile.write(cBuffer, sizeToWrite);
		cFile.flush();
		cBufferCount = 0;
	}

	return true;
}

BufferedTape::~BufferedTape()
{
	free(cBuffer);
	cFile.close();
	std::remove(cFileName);
}
