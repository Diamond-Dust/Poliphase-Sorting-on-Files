#include "tape.h"

Tape::Tape(std::string pFilePath, int pTapeSize)
{
	cFile.open(pFilePath, std::fstream::in | std::fstream::out | std::fstream::trunc);
	std::locale myLocale("");
	cFile.imbue(myLocale);
	if (!cFile.is_open())
	{
		std::cerr << "Tape could not be opened." << std::endl;
		return;
	}
	cRecordNumber = pTapeSize;
	cCurrentRecord = 0;
	while (pTapeSize--)
	{
		std::cout << " tape1 " << cFile.tellg() << std::endl;
		cFile << EMPTY_RECORD;
	}
	cFile.clear();
	cFile.seekg(0, std::ios::beg);

	std::cout << " tape2 " << cFile.tellg() << std::endl;
}

bool Tape::clearRecord()
{
	return writeRecord(EMPTY_RECORD);
}

bool Tape::writeRecord(double pI, double pR)
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

bool Tape::writeRecord(std::string pOutput)
{
	if (!cFile.is_open())
	{
		std::cerr << "Record could not be read. Tape could not be opened." << std::endl;
		return false;
	}

	std::cout << " write1 " << cFile.tellg() << std::endl;

	cFile << pOutput;
	cCurrentRecord++;

	if (cCurrentRecord >= cRecordNumber)
	{
		cCurrentRecord = 0;
		cFile.clear();
		cFile.seekg(0);
	}

	std::cout << " write2 " << cFile.tellg() << std::endl;

	return true;
}

bool Tape::readRecord(std::string& pOutput)
{
	if (!cFile.is_open())
	{
		std::cerr << "Record could not be read. Tape could not be opened." << std::endl;
		return false;
	}

	std::cout << " read1 " << cFile.tellg() << std::endl;

	std::getline(cFile, pOutput);
	cCurrentRecord++;

	if (cCurrentRecord >= cRecordNumber)
	{
		cCurrentRecord = 0;
		cFile.clear();
		cFile.seekg(0);
	}

	std::cout << " read2 " << cFile.tellg() << std::endl;

	return true;
}

void Tape::flush()
{
	cFile.flush();
}

const std::string Tape::EMPTY_RECORD = "                                                                                \n";

Tape::~Tape()
{
	cFile.close();
}
